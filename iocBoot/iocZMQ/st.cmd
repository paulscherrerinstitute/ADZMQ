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
ZMQDriverConfig("ZMQ1", "tcp://127.0.0.1:5432", -1, -1)
dbLoadRecords("$(AREA_DETECTOR)/ADApp/Db/ADBase.template",     "P=$(PREFIX),R=cam1:,PORT=$(PORT),ADDR=0,TIMEOUT=1")

# Create a standard arrays plugin, set it to get data from first simDetector driver.
NDStdArraysConfigure("Image1", 3, 0, "$(PORT)", 0)
dbLoadRecords("$(AREA_DETECTOR)/ADApp/Db/NDPluginBase.template","P=$(PREFIX),R=image1:,PORT=Image1,ADDR=0,TIMEOUT=1,NDARRAY_PORT=$(PORT),NDARRAY_ADDR=0")

dbLoadRecords("$(AREA_DETECTOR)/ADApp/Db/NDStdArrays.template", "P=$(PREFIX),R=image1:,PORT=Image1,ADDR=0,TIMEOUT=1,TYPE=Int8,FTVL=UCHAR,NELEMENTS=480000")

# Create a ZeroMQ server to publish NDArray
NDZMQConfigure("NDZMQ1", "tcp://*:1234", 3, 0, "ZMQ1", 0)
dbLoadRecords("$(AREA_DETECTOR)/ADApp/Db/NDPluginBase.template","P=$(PREFIX),R=ZMQ1:,PORT=NDZMQ1,ADDR=0,TIMEOUT=1,NDARRAY_PORT=$(PORT),NDARRAY_ADDR=0")


# Load all other plugins using commonPlugins.cmd
< $(AREA_DETECTOR)/iocBoot/commonPlugins.cmd


cd ${TOP}/iocBoot/${IOC}
iocInit
create_monitor_set("auto_settings.req", 30, "P=$(PREFIX)")
