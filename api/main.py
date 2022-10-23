"""
    @author: Rodrigo Schardong
"""


from flask import Flask, request, render_template, jsonify, redirect, url_for
from flask_cors import CORS
import socket
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

import post as p
import get as g

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
    

app = Flask(__name__)
CORS(app)
url = str(GetIp())

requestBuffer = []

@app.route('/dogfeeder', methods = ["POST"])
def pHandler():
    """
        print("Receiving client Post Request Data: " + str(request.data))
    """
    
    global requestBuffer
    requesBuffer = p.Handler(request.data, requestBuffer)
    return "True"

@app.route('/dogfeeder', methods = ["GET"])
def gHandler():
    """
        print('Receiving client Get Request Data: ' + str(request.data))
    """
    
    return g.Handler(request.data, requestBuffer)

app.run(host = url, port = 8080)



