# ADZMQ Releases

## __1.3.0 (May XX, 2022)__

### ADZMQ
* Option to specify whether to bind or connect in the *serverHost* argument of the configuration command.
* Get socket options ZMQ_AFFINITY and ZMQ_RCVHWM from environment.

### NDPluginZMQ

* When format float point numbers to strings, use the maximum precision, i.e. 7 for 32bit float and 17 for 64bit double.
* Option to specify whether to bind or connect in the *serverHost* argument of the configuration command.
* Get socket options ZMQ_AFFINITY and ZMQ_SNDHWM from environment.

## __1.2.0 (Oct. 18, 2021)__

### NDPluginZMQ

* Use zero-copy and DONTWAIT on send