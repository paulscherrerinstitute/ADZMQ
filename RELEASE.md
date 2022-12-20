# ADZMQ Releases

## __1.4.0 (Dec 20, 2022)__

* The *htype* field was mistakenly treated as an array (["array-1.0"]) since the beginning.
  Now it is a string "array-1.0".
* The *shape* field was in the order of x,y,z, which is specified by
  [chunk-1.0](https://github.com/paulscherrerinstitute/htypes/blob/master/chunk-1.0.md).
  After switching to [array-1.0](https://github.com/paulscherrerinstitute/htypes/blob/master/array-1.0.md),
  which assumes z,y,x order, the order is now reversed.

### NDPluginZMQ

* Class `NDPluginZMQ` now can be subclassed to support custom protocols. Any subclass needs only implementing
  the `sendNDArray` method. And `NDPluginZMQ` provides helper methods `send(std::string)` and `send(NDArray*)` for
  subclasses so that they do not depend on zmq directly.

## __1.3.0 (May 16, 2022)__

### ADZMQ
* Option to specify whether to bind or connect in the *serverHost* argument of the configuration command.
* Get socket options ZMQ_AFFINITY and ZMQ_RCVHWM from environment.

### NDPluginZMQ

* When format float point numbers to strings, use the maximum precision, i.e. 7 for 32bit float and 17 for 64bit double.
* Option to specify whether to bind or connect in the *serverHost* argument of the configuration command.
* Get socket options ZMQ_AFFINITY and ZMQ_SNDHWM from environment.
* Support int64, uint64, float32 and float64 typed NDArray
* Fix conversion of undefined NDAttributes. It used to create invalid json output.

## __1.2.0 (Oct. 18, 2021)__

### NDPluginZMQ

* Use zero-copy and DONTWAIT on send
