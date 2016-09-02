ADZMQ
=====

This project attempts to integrate ZeroMQ with areaDetector framework, in two folds:

* a driver pulls data from a ZeroMQ server and generates NDArray.
* a plugin publishes NDArray as ZeroMQ server.

The ZeroMQ message format is detailed [here](https://github.com/datastreaming/htypes/blob/master/chunk-1.0.md).


Documentation
-------------
[Configuration](documentation/Usage.md)


Build
-----
If you build under PSI environment, 
```bash
    cd psiBuild
    make
```

Otherwise, use the standard EPICS base system. Create a file *configure/RELEASE.local* with your build environment settings,
```
SUPPORT=C:/epics/synApps_5_7/support/
EPICS_BASE=C:/epics/base-3.14.12-VC12

# asyn
ASYN=$(SUPPORT)/asyn-4-22
BUSY=$(SUPPORT)/busy-1-6
CALC=$(SUPPORT)/calc-3-2
SSCAN=$(SUPPORT)/sscan-2-9
AUTOSAVE=$(SUPPORT)/autosave-5-1
# AREA_DETECTOR is needed for base and plugins
AREA_DETECTOR=C:/epics/areaDetector
ADBINARIES=$(AREA_DETECTOR)/ADBinaries
ADCORE=$(AREA_DETECTOR)/ADCore
```
And then run
```bash
    make
```

Test
----

After building, the examplar IOC *zmqApp* is created. Follow the test procedure,

1. Start the IOC, 
   ```bash
   cd iocBoot/iocZMQ
   ../../bin/linux-x86_64/zmqApp st.cmd
   ```

2. Start the ZeroMQ server. It creates an image of 800x600 of int8 type. The update
frequency is 1 Hz. 
   ```bash
   cd tests
   python zmq_server.py
   ```

3. Launch MEDM panel.
   ```bash
   medm -x -macro P=13ZMQ1:,R=cam1: ADBase.adl
   ```

4. Start acquisition and observe the image counter increases.

5. Now enable the plugin to publish NDArray as ZeroMQ server.
   ```bash
   caput 13ZMQ1:cam1:ArrayCallbacks 1
   caput 13ZMQ1:ZMQ1:EnableCallbacks 1
   ```

6. Launch a ZeroMQ client to receive data.
   ```bash
   cd tests
   python zmq_client.py
   ```

The data flow is illustrated by the following graph,
![Architecture Overview](documentation/Architecture.png)
