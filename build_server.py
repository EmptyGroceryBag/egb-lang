import socket

def accept_connection():
    serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serversocket.bind(('', 9999))
    serversocket.listen(5)

    print("looking for connection...")

    while True:
        (connection, address) = serversocket.accept()
        print("accepted connection at " + str(connection))
        return None

accept_connection()
