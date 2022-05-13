ADZMQ
=====

:author: Xiaoqiang Wang

Introduction
------------

In ZeroMQ patterns, the message flow can be either uni- or bi-directional.
This project currently considers only uni-directional message flow.
And the patterns supported are PUB/SUB and PUSH/PULL.

* ZMQDriver pulls data from a ZeroMQ server and generates NDArray.
* NDPluginZMQ publishes NDArray as ZeroMQ server.

The ZeroMQ message format is detailed `here <https://github.com/datastreaming/htypes/blob/master/array-1.0.md>`_.

.. contents:: Contents


ZMQDriver
---------
::

  # portName    The name of the asyn port driver to be created.
  # serverHost  The address of the ZMQ server. transport://address [SUB|PULL] [BIND|CONNECT].
  # maxBuffers  The maximum number of NDArray buffers that the NDArrayPool for this driver is
  #             allowed to allocate. Set this to -1 to allow an unlimited number of buffers.
  # maxMemory   The maximum amount of memory that the NDArrayPool for this driver is
  #             allowed to allocate. Set this to -1 to allow an unlimited amount of memory.
  # priority    The thread priority for the asyn port driver thread if ASYN_CANBLOCK is set in asynFlags.
  # stackSize   The stack size for the asyn port driver thread if ASYN_CANBLOCK is set in asynFlags.

  ZMQDriverConfig(const char *portName, const char *serverHost,
                    int maxBuffers, size_t maxMemory,
                    int priority, int stackSize)

ZMQDriver pulls data in. By ZeroMQ patterns, this can be either a puller or a subscriber.
The *serverHost* parameter of *ZMQDriverConfig* is to specify the server address and pattern.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 1
  :widths: 70 10 20

  * - serverHost
    - Pattern
    - Socket
  * - "tcp://127.0.0.1:5432"
    - PUB/SUB
    - Connect
  * - "tcp://127.0.0.1:5432 SUB BIND"
    - PUB/SUB
    - Bind
  * - "tcp://\*:5432", "tcp://127.0.0.1:5432 PULL"
    - PUSH/PULL
    - Bind
  * - "tcp://127.0.0.1:5432 PULL CONNECT"
    - PUSH/PULL
    - Connect


If the host contains the wildcard, it is assumed to be a puller, otherwise a subscriber.
If the puller needs to bind on a specific interface, the type must be explicitly specified.
If not specidifed, a subscriber is assumed to connect and a puller is assumed to bind to the address,
unless the address contains a wildcard, in which case it always binds.


NDPluginZMQ
-----------
::

  # portName           The name of the asyn port driver to be created.
  # serverHost         The address of the ZMQ server. transport://address [SUB|PULL] [BIND|CONNECT].
  # queueSize          The number of NDArrays that the input queue for this plugin can hold when
  #                    NDPluginDriverBlockingCallbacks=0.
  # blockingCallbacks  0=callbacks are queued and executed by the callback thread;
  #                    1=callbacks execute in the thread
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


NDPluginZMQ pushes data out. By ZeroMQ patterns, this can be either a pusher or a publisher.
The *serverHost* parameter of *NDZMQConfigure* is to specify the server address and pattern.

.. cssclass:: table-bordered table-striped table-hover
.. flat-table::
  :header-rows: 1
  :widths: 70 10 20

  * - serverHost
    - Pattern
    - Socket
  * - "tcp://\*:1234" or "tcp://127.0.0.1 PUB"
    - PUB/SUB
    - Bind
  * - "tcp://127.0.0.1 PUB CONNECT"
    - PUB/SUB
    - Connect
  * - "tcp://127.0.0.1:5432"
    - PUSH/PULL
    - Connect
  * - "tcp://127.0.0.1:5432 PUSH BIND"
    - PUSH/PULL
    - Bind

If the host contains the wildcard, it is assumed to be a publisher, otherwise a pusher.
If the publisher needs to bind on a specific interface, the type must be explicitly specified.
If not specidifed, a pusher is assumed to connect and a publisher is assumed to bind to the address,
unless the address contains a wildcard, in which case it always binds.

