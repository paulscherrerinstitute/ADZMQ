ZeroMQ Basics
=============

In ZeroMQ patterns, the message flow can be either uni- or bi-directional.
This project currently considers only uni-directional message flow.
And the patterns supported are PUB/SUB and PUSH/PULL.


ZMQDriver
=========

```bash
  # portName    The name of the asyn port driver to be created.
  # serverHost  The address of the ZMQ server, and pattern to be used. transport://address [SUB|PULL].
  # maxBuffers  The maximum number of NDArray buffers that the NDArrayPool for this driver is 
  #             allowed to allocate. Set this to -1 to allow an unlimited number of buffers.
  # maxMemory   The maximum amount of memory that the NDArrayPool for this driver is 
  #             allowed to allocate. Set this to -1 to allow an unlimited amount of memory.
  # priority    The thread priority for the asyn port driver thread if ASYN_CANBLOCK is set in asynFlags.
  # stackSize   The stack size for the asyn port driver thread if ASYN_CANBLOCK is set in asynFlags.

  ZMQDriverConfig(const char *portName, const char *serverHost,
                    int maxBuffers, size_t maxMemory,
                    int priority, int stackSize)
```

ZMQDriver listens for incoming data. By ZeroMQ patterns, this can be either a puller or a subscriber.
The *serverHost* parameter of *ZMQDriverConfig* is to specify the server address and pattern.

| Pattern   |           serverHost                             |
|-----------|--------------------------------------------------|
| PUB/SUB   | "tcp://127.0.0.1:5432"                           |
| PUSH/PULL | "tcp://*:5432" <br /> "tcp://127.0.01:4532 PULL" |

If the host contains the wildcard, it is assumed to be a puller, otherwise a subscriber.
If the puller needs to bind on a specific interface, the type must be explicitly specified.



NDPluginZMQ
===========

```bash
  # portName           The name of the asyn port driver to be created.
  # serverHost         The IP address:port of the ZMQ server to be created.
  # queueSize,         The number of NDArrays that the input queue for this plugin can hold when 
  #                    NDPluginDriverBlockingCallbacks=0. 
  # blockingCallbacks  0=callbacks are queued and executed by the callback thread; 
  #                     1 callbacks execute in the thread
  #                    of the driver doing the callbacks.
  # NDArrayPort        Port name of NDArray source
  # NDArrayAddr        Address of NDArray source
  # maxBuffers         Maximum number of NDArray buffers driver can allocate. -1=unlimited
  # maxMemory          Maximum memory bytes driver can allocate. -1=unlimited

  NDZMQConfigure(const char *portName, const char *serverHost,
                int queueSize, int blockingCallbacks,
                const char *NDArrayPort, int NDArrayAddr,
                int maxBuffers, size_t maxMemory,
                int priority, int stackSize)

```

NDPluginZMQ pushes data out. By ZeroMQ patterns, this can be either a pusher or a publisher.
The *serverHost* parameter of *NDZMQConfigure* is to specify the server address and pattern.

| Pattern   |           serverHost                        |
|-----------|---------------------------------------------|
| PUB/SUB   | "tcp://*:1234" <br /> "tcp://127.0.0.1 PUB" |
| PUSH/PULL | "tcp://127.0.0.1:5432"                      |

If the host contains the wildcard, it is assumed to be a publisher, otherwise a pusher.
If the publisher needs to bind on a specific interface, the type must be explicitly specified.

