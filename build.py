import socket

# TODO: parse command line arguments

print("connecting to server...")
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('192.168.1.4', 9999))
