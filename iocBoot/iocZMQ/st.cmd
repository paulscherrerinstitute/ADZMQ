#!../../bin/darwin-x86/zmqApp

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/zmqApp.dbd"
zmqApp_registerRecordDeviceDriver pdbbase

epicsEnvSet("PREFIX", "13ZMQ1:")
epicsEnvSet("PORT",   "ZMQ1")
epicsEnvSet("QSIZE",  "20")
epicsEnvSet("XSIZE",  "800")
epicsEnvSet("YSIZE",  "600")
epicsEnvSet("NCHANS", "2048")

# Connect to ZeroMQ server
# ZMQDriverConfig (
#     portName,   # The name of the asyn port driver to be created.
#     serverHost, # The ZMQ server to connect to.
#     maxBuffers, # The maximum number of NDArray buffers that the NDArrayPool for this driver is
#                 # allowed to allocate. Set this to -1 to allow an unlimited number of buffers.
#     maxMemory,  # The maximum amount of memory that the NDArrayPool for this driver is
#                 # allowed to allocate. Set this to -1 to allow an unlimited amount of memory.
#     priority,   # The thread priority for the asyn port driver thread if ASYN_CANBLOCK is set in asynFlags.
#     stackSize,  # The stack size for the asyn port driver thread if ASYN_CANBLOCK is set in asynFlags.
ZMQDriverConfig("ZMQ1", "tcp://127.0.0.1:5432", -1, -1)
dbLoadRecords("$(ADCORE)/ADApp/Db/ADBase.template",     "P=$(PREFIX),R=cam1:,PORT=$(PORT),ADDR=0,TIMEOUT=1")

# Create a standard arrays plugin, set it to get data from ZMQ driver.
NDStdArraysConfigure("Image1", 3, 0, "$(PORT)", 0)
dbLoadRecords("$(ADCORE)/ADApp/Db/NDPluginBase.template","P=$(PREFIX),R=image1:,PORT=Image1,ADDR=0,TIMEOUT=1,NDARRAY_PORT=$(PORT),NDARRAY_ADDR=0")

dbLoadRecords("$(ADCORE)/ADApp/Db/NDStdArrays.template", "P=$(PREFIX),R=image1:,PORT=Image1,ADDR=0,TIMEOUT=1,TYPE=Int8,FTVL=UCHAR,NELEMENTS=480000")

# Create a ZeroMQ server to publish NDArray
# NDZMQConfigure (
#     portName,          # The name of the asyn port driver to be created.
#     serverHost,        # The ZMQ server to be created.
#     queueSize,         # The number of NDArrays that the input queue for this plugin can hold when
#                          NDPluginDriverBlockingCallbacks=0.
#     blockingCallbacks, # 0=callbacks are queued and executed by the callback thread; 1 callbacks execute in the thread
#                          of the driver doing the callbacks.
#     NDArrayPort,       # Port name of NDArray source
#     NDArrayAddr,       # Address of NDArray source
#     maxBuffers,        # Maximum number of NDArray buffers driver can allocate. -1=unlimited
#     maxMemory)         # Maximum memory bytes driver can allocate. -1=unlimited
NDZMQConfigure("NDZMQ1", "tcp://*:1234", 3, 0, "ZMQ1", 0)
dbLoadRecords("$(ADCORE)/ADApp/Db/NDPluginBase.template","P=$(PREFIX),R=ZMQ1:,PORT=NDZMQ1,ADDR=0,TIMEOUT=1,NDARRAY_PORT=$(PORT),NDARRAY_ADDR=0")


# Load all other plugins using commonPlugins.cmd
< $(ADCORE)/iocBoot/commonPlugins.cmd


cd ${TOP}/iocBoot/${IOC}
iocInit
create_monitor_set("auto_settings.req", 30, "P=$(PREFIX)")
