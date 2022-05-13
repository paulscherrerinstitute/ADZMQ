# NDZMQConfigure (
#       portName,           The name of the asyn port driver to be created.
#       serverHost,         The IP address:port of the ZMQ server to be created.
#       queueSize,          The number of NDArrays that the input queue for this plugin can hold when
#                           NDPluginDriverBlockingCallbacks=0.
#       blockingCallbacks,  0=callbacks are queued and executed by the callback thread;
#                           1 callbacks execute in the thread of the driver doing the callbacks.
#       NDArrayPort,        Port name of NDArray source
#       NDArrayAddr)        Address of NDArray source

NDZMQConfigure("NDZMQ$(N=1)", "$(NDZMQ_ADDR)", $(SIZE=20), 0, "$(PORT)", 0)
dbLoadRecords("NDPluginBase.template","P=$(PREFIX),R=$(RECORD=ZMQ$(N=1):),PORT=NDZMQ$(N=1),ADDR=0,TIMEOUT=1,NDARRAY_PORT=$(PORT)")

set_pass0_restoreFile("NDPluginBase_settings$(N=1).sav")
set_pass1_restoreFile("NDPluginBase_settings$(N=1).sav")

afterInit create_monitor_set,"NDPluginBase_settings.req",30,"P=$(PREFIX),R=$(RECORD=ZMQ$(N=1):)","NDPluginBase_settings$(N=1).sav"
