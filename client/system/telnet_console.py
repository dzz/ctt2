import sys
import socket
from io import StringIO

app = None

class telnet_console:
    def __init__(self, application, telnet_host, telnet_port):
        global app
        app = application
        HOST = telnet_host
        PORT = telnet_port
        self.connected = False
        self.connection = None
        self.commandBuffer = ""
        self.buffer = StringIO()

        self.socket = socket.socket( socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setblocking(0)
        print("PYTEL: Socket created")
        self.socket.bind( (HOST, PORT) ) 
        print("PYTEL: Socket bound")
        self.socket.listen(10)

    def tick(self):
        if not self.connected:
            try:
                conn, addr = self.socket.accept()
            except:
                conn = None

            if conn is not None:
                print("PYTEL: Connected: [{0}]".format(addr))
                self.connection = conn
                self.connection.send(bytes("CTT2 - Debug Server 0.1\r\n--------------------\r\n","UTF-8"))
                self.connected = True
        else:
            try:
                data = self.connection.recv(1024).decode("UTF-8")
                if(data==""):
                    self.connected = False

                try:
                    compiled = compile( data, "remote","exec" )
                except Exception as e:
                    self.connection.sendall(bytes(str(e),"UTF-8"))

                try:
                    sys.stdout = self.buffer
                    exec(compiled)
                    sys.stdout = sys.__stdout__
                    self.connection.sendall(bytes(str(self.buffer.getvalue()) + "\r\n","UTF-8"))
                    self.buffer = StringIO()
                except Exception as e:
                    self.connection.sendall(bytes(str(e) + "\r\n","UTF-8"))

            except:
                pass


    def __del__(self):
        self.socket.close()


        