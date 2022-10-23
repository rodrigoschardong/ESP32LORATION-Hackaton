def Handler(request, buffer):
    #Operate new event or whatever
    print(request.decode("utf-8"))
    print(type(request))
    buffer.append(request.decode("utf-8"))
    return buffer