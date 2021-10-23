import os
import socket
import re


def handleReq(clientSocket):
    # if not exists, send req to get it

    # recv data
    recvData = clientSocket.recv(1024).decode("UTF-8")
    # print("recved data:", recvData)

    # find the target file
    target = re.findall("GET (.+) HTTP/1.1", recvData)[0].replace("/", "")
    # print("target: " + target)
    
    try:
        file = open(target)
        # print("File is found in proxy server.")
        response = file.read()
        clientSocket.send(bytes("HTTP/1.1 200 OK\r\nContent-type: text/html;charset=ISO-8859-1\n\r\n\r", encoding = "utf-8"))
        clientSocket.sendall(bytes(response, encoding = "utf-8"))
        # print("Send, done.")
    
    except:
        
        # print("File is not exist.\nSend request to server...")
        proxyClientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        serverName = target.split(":")[0]
        proxyClientSocket.connect((serverName, 8000))
        proxyClientSocket.sendall(recvData.encode("UTF-8"))
        response = proxyClientSocket.recv(2014)
        # print("File is found in server.")
        
        clientSocket.sendall(response)
        # print("Send, done.")
        
        # cache
        cache = open(target, 'w')
        cache.writelines(response.decode("UTF-8").replace('\r\n', '\n'))
        cache.close()
        # print("Cache, done.")

def startProxy(port):
    proxyServerSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    proxyServerSocket.bind(("localhost", port))
    proxyServerSocket.listen(5)
    clientSocket, addr = proxyServerSocket.accept()
    handleReq(clientSocket)
    clientSocket.close()
    proxyServerSocket.close()

print("Proxy is waiting for connecting...")
startProxy(8080)
    