"""
    @author: Rodrigo Schardong
"""


from flask import Flask, request, render_template, jsonify, redirect, url_for
from flask_cors import CORS
import socket
# import matplotlib.pyplot as plt
# import numpy as np
import pandas as pd
import openpyxl

import post as p
import get as g

EXCELPATH = "DataBank.xlsx"
def GetIp():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(('192.168.4.1', 1))
        IP = s.getsockname()[0]
    except:
        IP = 'except'
    finally:
        s.close()
    print("Local IP: " + IP)
    return IP
    
def ListToDF(buffer):
    return pd.DataFrame (buffer, columns = ['Json Packs'])

def Store(buffer):
    df = ListToDF(buffer)
    df.to_excel(r'DataBank.xlsx', index = False)

def Read():
    try:
        df = pd.read_excel(EXCELPATH)
        buffer = df.values.tolist()
        out = []
        for b in buffer:
            out.append(b[0])
        print(out)
        return out
    except:
        return []
    

app = Flask(__name__)
CORS(app)
url = str(GetIp())

#requestBuffer = []
requestBuffer = Read()


@app.route('/dogfeeder', methods = ["POST"])
def pHandler():
    """
        print("Receiving client Post Request Data: " + str(request.data))
    """
    
    global requestBuffer
    requesBuffer = p.Handler(request.data, requestBuffer)
    
    Store(requestBuffer)
    return "True"

@app.route('/dogfeeder', methods = ["GET"])
def gHandler():
    """
        print('Receiving client Get Request Data: ' + str(request.data))
    """
    
    return g.Handler(request.data, requestBuffer)

app.run(host = url, port = 8080)



