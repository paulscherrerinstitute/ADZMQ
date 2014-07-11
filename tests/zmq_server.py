from __future__ import print_function

import time
import zmq
import numpy

context = zmq.Context()
sock = context.socket(zmq.PUB)
sock.bind('tcp://*:5432')

cols = 800
rows = 600

header_t = """{
    "htype" : ["chunk-1.0"],
    "shape" : [%d,%d],
    "type"  : "%s",
    "frame" : %d
}"""

frame = 0
while True:
    time.sleep(1)
    # generate data
    data = numpy.random.random_integers(0, 255, (rows, cols)).astype(numpy.uint8)
    data[:50,:50] = 255
    # send header
    header = header_t % (cols, rows, data.dtype, frame)
    sock.send(header, zmq.SNDMORE)
    # send data
    sock.send(data)

    frame += 1
