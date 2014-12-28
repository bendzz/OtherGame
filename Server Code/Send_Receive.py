import socket

# client code
s=None
port=8890

#Old port was 3939
#Or port 19834
def openConnection(TCP_PORT=port):
    connected = 1
    TCP_IP = '127.0.0.1'
    BUFFER_SIZE = 1024
    global s

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        s.connect((TCP_IP, TCP_PORT))
        #print "Opened connection to port ", port
    except Exception, e:
        print e
        connected = 0
    return connected
    #print u"ran"


def sendMessage(MESSAGE):
    s.send(MESSAGE)
    print u"sent data: ", MESSAGE
    #s.close()

def receiveMessage():
    BUFFER_SIZE = 1024
    data = s.recv(BUFFER_SIZE)
    print data
    #print u"client received data:", data.decode("utf-8")

def closeConnection():
    s.close()

connected = 0

print u"test"
connected = openConnection();

if connected == 1:
    print u"Sending message"
    sendMessage("Testing Testing")

    #print u"receiving message?"
    #receiveMessage()
