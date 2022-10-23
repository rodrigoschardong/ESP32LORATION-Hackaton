"""
    @author: Rodrigo Schardong
"""


from flask import Flask, request, render_template, jsonify, redirect, url_for
from flask_cors import CORS
import socket
import matplotlib.pyplot as plt
import numpy as np

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

@app.route('/DogFeeder', methods = ["POST", "GET"])
def agvIP():
    if(request.method == 'POST'):
        clientIP = (request.data)
        print('Receiving client Request Data: ' + request.data)
        return p.Handler(request.data)
    else:
        print('Receiving client Request Data: ' + request.data)
        return g.Handler(request.data)


app.run(host = url, port = 8080)



