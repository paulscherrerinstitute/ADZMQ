from __future__ import print_function

import time
import zmq
import numpy
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--type', dest='stype', default='PUB')
parser.add_argument('--host', default='tcp://*:5432')
parser.add_argument('--rate', type=int, default=1)
parser.add_argument('--connect', action='store_true')

args = parser.parse_args()
if args.stype == 'PUB':
    stype = zmq.PUB
elif args.stype == 'PUSH':
    stype = zmq.PUSH
else:
    print("Unsupported socket type %s, use PUB"%args.stype)
    sys.exit(1)

print("Server %s %s %s"%(('bind at', 'connect to')[args.connect], args.host, args.stype))

context = zmq.Context()
sock = context.socket(stype)

if args.connect:
    sock.connect(args.host)
else:
    sock.bind(args.host)

cols = 800
rows = 600

header_t = """{
    "htype" : ["array-1.0"],
    "shape" : [%d,%d],
    "type"  : "%s",
    "frame" : %d,
    "ndattr": {
        "NumImages" : 1
    }
}"""

frame = 0
while True:
    time.sleep(1./args.rate)
    # generate data
    data = numpy.random.random_integers(0, 255, (rows, cols)).astype(numpy.uint8)
    data[:50,:50] = 255
    # send header
    header = header_t % (cols, rows, data.dtype, frame)
    sock.send_string(header, zmq.SNDMORE)
    # send data
    sock.send(data)

    frame += 1
