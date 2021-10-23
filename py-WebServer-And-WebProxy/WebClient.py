import socket
import sys

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.connect(("localhost", 8000))

# send HTTP Get head
s.send("GET /index.html HTTP/1.1\n\rHost: localhost:8000\n\r")
msg = s.recv(1024)
s.close()

print ("Respnse:\n", msg.decode('utf-8'))