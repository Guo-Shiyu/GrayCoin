#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

from os import system
import socket
import sys
import re
import _thread

def handleRequest(tcpSocket):
	# 1. Receive request message from the client on connection socket
	buf = tcpSocket.recv(1024)

	# 2. Extract the path of the requested object from the message (second part of the HTTP header)
	target = re.findall("GET (.+) HTTP/1.1", str(buf, encoding = "utf-8"))[0].replace("/", "")

	# 3. Read the corresponding file from disk
	try:
		file = open(target)
	
	# 4. Store in temporary buffer
		page = file.read()
		file.close()
	
	# 5. Send the correct HTTP response error
		tcpSocket.send(bytes("HTTP/1.1 200 OK\r\nContent-type: text/html;charset=ISO-8859-1\n\r\n\r", encoding = "utf-8"))

	# 6. Send the content of the file to the socket
		tcpSocket.send(bytes(page, encoding = "utf-8"))
	
	except:
		# additonal feature 2
		tcpSocket.send(bytes("HTTP/1.1 404 Not Found\r\n\n\r\n\r", encoding = "utf-8"))
	

	# 7. Close the connection socket
	tcpSocket.close()

	#pass # Remove/replace when function is complete

		# additional feature 1
def startServer(serverAddress, serverPort = 8000):
	# 1. Create server socket
	sev = socket.socket()
	
	# 2. Bind the server socket to server address and server port
	sev.bind((serverAddress, serverPort))
	
	# 3. Continuously listen for connections to server socket
	sev.listen(5)
	conn, addr = sev.accept()
	#print("conn: ", conn, "addr: ", addr)
	
	# 4. When a connection is accepted, call handleRequest function, passing new connection socket (see https://docs.python.org/3/library/socket.html#socket.socket.accept)
	handleRequest(conn)
	
	# 5. Close server socket
	sev.close()

	#pass # Remove/replace when function is complete

#addtional feature 3
try:
   _thread.start_new_thread( startServer, ("localhost", 8081) )
   _thread.start_new_thread( startServer, ("localhost", 8082) )
except:
   print ("Error: can not start new thread")

print("Server start successfully")

startServer("localhost", 8000)


# additional feature 4 is in 'WebClient.py'