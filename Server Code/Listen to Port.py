'''
    Simple socket server using threads
'''

import socket
import sys
from thread import *
import csv
import os

'''
#csv writing object
with open('Games List.csv', 'w') as csvfile:
    fieldnames = ['first_name', 'last_name']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
'''



#Games list file
gamesListName = 'Games List.csv'

if os.path.exists(gamesListName):
     gamesList = file(gamesListName, "r+")
else:
    #Create file then read/write to it
     gamesList = file(gamesListName, "w")
     gamesList.close
     gamesList = file(gamesListName, "r+")

gamesList.seek(0, 2)    #Go to end of file

'''
gamesList.write("Start this")
gamesList.write('Start this2')
gamesList.flush()
'''

HOST = ''   # Symbolic name meaning all available interfaces
PORT = 8888 # Arbitrary non-privileged port

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print 'Socket created'

#Bind socket to local host and port
try:
    s.bind((HOST, PORT))
except socket.error as msg:
    print 'Bind failed. Error Code : ' + str(msg[0]) + ' Message ' + msg[1]
    sys.exit()

print 'Socket bind complete'

#Start listening on socket
s.listen(10)
print 'Socket now listening'

#Function for handling connections. This will be used to create threads
def clientthread(conn):
    #Sending message to connected client
    conn.send('Welcome to the server. Type something and hit enter\n') #send only takes string

    #infinite loop so that function do not terminate and thread do not end.
    while True:

        #Receiving from client
        data = conn.recv(1024)
        reply = 'GOT:' + data
        print data

        #write data to file
        gamesList.write(data + ',')
        gamesList.flush()

        if not data:
            break
        if data == 'q':
            break

        conn.sendall(reply)

    #came out of loop
    conn.close()

#now keep talking with the client
try:
    while 1:
        #wait to accept a connection - blocking call
        conn, addr = s.accept()
        print 'Connected with ' + addr[0] + ':' + str(addr[1])

        #start new thread takes 1st argument as a function name to be run, second is the tuple of arguments to the function.
        start_new_thread(clientthread ,(conn,))
except KeyboardInterrupt:
  # do post-loop stuff
  print "Done"

s.close()