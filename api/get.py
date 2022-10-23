import json

def Handler(request, buffer):
    out = json.dumps(buffer)
    print(out)
    return out