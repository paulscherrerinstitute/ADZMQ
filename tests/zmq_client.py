from __future__ import print_function

import time
import zmq
import numpy
import json

context = zmq.Context()
sock = context.socket(zmq.SUB)
sock.setsockopt(zmq.SUBSCRIBE, '')

sock.connect('tcp://localhost:1234')

while True:
    # receive header
    header = sock.recv()
    print(header)
    info = json.loads(header)

    # receive data
    data = numpy.frombuffer(sock.recv(), dtype=str(info['type']))
    data.reshape(info['shape'])
    print(data)
