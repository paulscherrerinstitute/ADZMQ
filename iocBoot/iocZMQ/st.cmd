#!../../bin/darwin-x86/zmqApp

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/zmqApp.dbd"
zmqApp_registerRecordDeviceDriver pdbbase

# Connect to ZeroMQ server
ZMQDriverConfig("ZMQ1", "tcp://127.0.0.1:5432", -1, -1)

NDStdArraysConfigure("Image1", 3, 0, "ZMQ1", 0)

# Create a ZeroMQ server to publish NDArray
NDZMQConfigure("NDZMQ1", "tcp://*:1234", 3, 0, "ZMQ1", 0)

## Load record instances

cd ${TOP}/iocBoot/${IOC}
iocInit
