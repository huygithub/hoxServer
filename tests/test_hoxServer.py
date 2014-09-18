import socket
import threading
import time
import unittest

SERVER = "localhost"
PORT = 8000
MSGLEN = 512
mythreads = []

class mysocket:
    '''demonstration class only
      - coded for clarity, not efficiency
    '''

    def __init__(self, sock=None):
	    if sock is None:
		    self.sock = socket.socket(
         		socket.AF_INET, socket.SOCK_STREAM)
	    else:
		    self.sock = sock

    def connect(self, host, port):
	    self.sock.connect((host, port))

    def close(self):
        if not self.sock is None:
            self.sock.close();

    def mysend(self, msg):
        msgLen = len(msg)
        totalsent = 0
        while totalsent < msgLen:
            sent = self.sock.send(msg[totalsent:])
            if sent == 0:
                raise RuntimeError, \
	                "socket connection broken"
            totalsent = totalsent + sent

    def myreceive(self):
        msg = ''
        while len(msg) < MSGLEN:
            chunk = self.sock.recv(MSGLEN-len(msg))
            if chunk == '':
	            raise RuntimeError, "socket connection broken: [%s]" % msg
	            break
            msg = msg + chunk
            bEnd = msg.find("\n\n")
            if bEnd != -1:
                msg = msg.rstrip()
                break;
        return msg

############ main ##############

def worker(id):
    s = mysocket()
    #print "[%d] >>>>>>>>>> Connecting ...." % id
    s.connect(SERVER, PORT)
    #print ">>>>>>>>>> Sending ...."
    req = "op=LOGIN&pid=test%d&pw=\"my password%d\"" % (id, id)
    s.mysend("%s\n" % req)
    #print ">>>>>>>>>> Receiving ...."
    time.sleep(.1)
    response = s.myreceive()
    print "[%s]" % (response,)
    s.close();
    #print ">>>>>>>>>> END."

def run_player1(args):
    pid = "player1"
    pw = "somepw"
    s = mysocket()
    s.connect(SERVER, PORT)

    # --- LOGIN
    req = "op=LOGIN&pid=%s&password=\"%s\"" % (pid, pw)
    s.mysend("%s\n" % req)
    response = s.myreceive()
    print "[%s]" % (response,)

    # --- LIST of Tables.
    req = "op=LIST"
    s.mysend("%s\n" % req)
    response = s.myreceive()
    print "[%s]" % (response,)

    # --- NEW table.
    req = "op=NEW&itimes=20/300/25"
    s.mysend("%s\n" % req)
    response = s.myreceive()
    print "[%s]" % (response,)

    # --- Receive notifications
    response = s.myreceive()
    print "[%s]" % (response,)

    # --- LIST of Tables.
    req = "op=LIST"
    s.mysend("%s\n" % req)
    response = s.myreceive()
    print "[%s]" % (response,)

    response = s.myreceive()
    print "[%s]" % (response,)

    s.close();
    #print ">>>>>>>>>> END."

def run_player2(args):
    pid = "player2"
    pw = "somepw"
    s = mysocket()
    s.connect(SERVER, PORT)

    # --- LOGIN
    req = "op=LOGIN&pid=%s&pw=\"%s\"" % (pid, pw)
    s.mysend("%s\n" % req)
    response = s.myreceive()
    print "(%s) [%s]" % (pid, response,)

    time.sleep(.1)

    # --- LIST of Tables.
    req = "op=LIST"
    s.mysend("%s\n" % req)
    response = s.myreceive()
    print "(%s) [%s]" % (pid, response,)

    # NOTE: hard-coded table.
    tableId = "1"

    # --- JOIN
    req = "op=JOIN&tid=%s&color=%s" % (tableId, "Black")
    s.mysend("%s\n" % req)
    response = s.myreceive()
    print "(%s) [%s]" % (pid, response,)

    # --- LIST of Tables.
    #req = "op=LIST"
    #s.mysend("%s\n" % req)
    #response = s.myreceive()
    #print "(%s) [%s]" % (pid, response,)

    # --- MSG
    req = "op=MSG&tid=%s&msg=%s" % (tableId, "Hi there")
    s.mysend("%s\n" % req)

    #response = s.myreceive()
    #print "(%s) [%s]" % (pid, response,)

    # --- LEAVE
    req = "op=LEAVE&tid=%s" % (tableId,)
    s.mysend("%s\n" % req)
    response = s.myreceive()
    print "(%s) [%s]" % (pid, response,)

    s.close();
    #print ">>>>>>>>>> END."

############ Socket class ##############

class MySocket:
    """ Socket to connect to HOX server.
    """
    def __init__(self, sock=None):
        if sock is None:
            self.sock = socket.socket( socket.AF_INET, socket.SOCK_STREAM)
        else:
            self.sock = sock

    def connect(self, host, port):
        self.sock.connect((host, port))

    def close(self):
        if not self.sock is None:
            self.sock.close();

    def send(self, msg):
        msgLen = len(msg)
        totalsent = 0
        while totalsent < msgLen:
            sent = self.sock.send(msg[totalsent:])
            if sent == 0:
                raise RuntimeError, "socket connection broken"
            totalsent = totalsent + sent

    def receive(self):
        msg = ''
        while len(msg) < MSGLEN:
            chunk = self.sock.recv(MSGLEN-len(msg))
            if chunk == '':
                raise RuntimeError, "socket connection broken: [%s]" % msg
                break
            msg = msg + chunk
            bEnd = msg.find("\n\n")
            if bEnd != -1:
                msg = msg.rstrip()
                break;
        return msg

############ Player class ##############

class Player:
    """ The Player class 
    """
    def __init__(self, id, password):
        self.id     = id
        self.pw     = password
        self.socket = None

    def login(self, host, port):
        self.socket = MySocket()
        self.socket.connect(host, port)

        self.socket.send("op=LOGIN&pid=%s&password=%s\n" % (self.id, self.pw,))
        resp = self.socket.receive()
        return resp

    def logout(self):
        self.socket.send("op=LOGOUT\n")
        resp = self.socket.receive()
        self.socket.close()
        self.socket = None
        return resp

    def listTables(self):
        self.socket.send("op=LIST\n")
        resp = self.socket.receive()
        return resp

    def newTable(self, itimes):
        self.socket.send("op=NEW&itimes=%s\n" % itimes)
        resp = self.socket.receive()
        return resp

############ Unit Test ##############

class TestLogin(unittest.TestCase):
    """ Test Login/Logout
    """
    def testLoginBasic(self):
        """ Login Basic Test
        """
        p1 = Player("p1", "somepw")

        resp = p1.login(SERVER, PORT)
        self.assertEqual(resp, "op=LOGIN&code=0&content=%s" % (p1.id,))

        resp = p1.logout()
        self.assertEqual(resp, "op=LOGOUT&code=0&content=%s" % (p1.id,))

class TestTable(unittest.TestCase):
    """ Test Table operations
    """
    def testTableBasic(self):
        """ Table Basic Test with Empty list
        """
        p1 = Player("p1", "somepw")

        resp = p1.login(SERVER, PORT)
        self.assertEqual(resp, "op=LOGIN&code=0&content=%s" % (p1.id,))

        resp = p1.listTables()
        self.assertEqual(resp, "op=LIST&code=0&content=")

        resp = p1.logout()
        self.assertEqual(resp, "op=LOGOUT&code=0&content=%s" % (p1.id,))

    def testTableNew(self):
        """ Table New-Table Test
        """
        p1 = Player("p1", "somepw")

        resp = p1.login(SERVER, PORT)
        self.assertEqual(resp, "op=LOGIN&code=0&content=%s" % (p1.id,))

        resp = p1.newTable("20/300/25")
        self.assertEqual(resp,
            "op=NEW&code=0&content=1;0;0;20/300/25;20/300/25;20/300/25;p1;1500;;0;")

        resp = p1.logout()
        self.assertEqual(resp, "op=LOGOUT&code=0&content=%s" % (p1.id,))

############ suite function ##############
def suite():
    suite = unittest.TestSuite()

    #suite.addTest(TestLogin("testLoginBasic"))
    suite.addTest(unittest.makeSuite(TestLogin, 'test'))

    suite.addTest(unittest.makeSuite(TestTable, 'test'))
    return suite

############ main ##############
if __name__ == "__main__":
    runner = unittest.TextTestRunner(verbosity=2) # Default verbosity=1
    runner.run( suite() )

    #num_threads = 0;

    #for i in range(num_threads):
        #s = threading.Thread(target=worker,args=(i,))
        #mythreads.append(s)
        #s.start()

    ## Player1
    #s = threading.Thread(target=run_player1,args=(1,))
    #mythreads.append(s)
    #s.start()

    ## Player2
    #s = threading.Thread(target=run_player2,args=(2,))
    #mythreads.append(s)
    #s.start()

    #for t in mythreads:
        #t.join()

    #print ">>>>>>> DONE <<<<<<<<<<<<"
