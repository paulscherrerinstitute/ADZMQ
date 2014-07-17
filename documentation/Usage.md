ZeroMQ Basics
=============

In ZeroMQ patterns, the message flow can be either uni- or bi-directional.
This project currently considers only uni-directional message flow.
And the patterns supported are PUB/SUB and PUSH/PULL.


ZMQDriver
=========

ZMQDriver listens for incoming data. By ZeroMQ patterns, this can be either a puller or a subscriber.
The second parameter of *ZMQDriverConfigure* is to specify the server address and pattern.

| Pattern   |           Host             |
|-----------|----------------------------|
| PUB/SUB   | tcp://127.0.0.1:5432       |
| PUSH/PULL | tcp://*:5432 <br />        |
|           | tcp://127.0.01:4532 PULL   |

If the host contains the wildcard, it is assumed to be a puller, otherwise a subscriber.
If the puller needs to bind on a specific interface, the type must be explicitly specified.

NDPluginZMQ
===========

NDPluginZMQ pushes data out. By ZeroMQ patterns, this can be either a pusher or a publisher.
The second parameter of *NDZMQConfigure* is to specify the server address and pattern.

| Pattern   |           Host               |
|-----------|------------------------------|
| PUB/SUB   | tcp://*:1234 <br />          |
|           | tcp://127.0.0.1 PUB          |
| PUSH/PULL | tcp://127.0.0.1:5432         |

If the host contains the wildcard, it is assumed to be a publisher, otherwise a pusher.
If the publisher needs to bind on a specific interface, the type must be explicitly specified.

