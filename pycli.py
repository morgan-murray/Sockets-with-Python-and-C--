#!/usr/bin/python

# Simple client

import socket

s = socket.socket(socket.AF_UNIX,socket.SOCK_STREAM)


s.connect("/tmp/sock_test")

message = "book"

ii = 4

while ii > 0:

    ii = ii - s.send(message);


