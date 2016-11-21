# ZMQDriverConfig (
#     portName,   # The name of the asyn port driver to be created.
#     serverHost) # The IP address:port of the ZMQ server to connect to.
ZMQDriverConfig("$(PORT=ZMQ$(N=1))", "$(ADZMQ_ADDR)")
dbLoadRecords("ADBase.template", "P=$(PREFIX),R=$(RECORD=cam1:),PORT=$(PORT),ADDR=0,TIMEOUT=1")

set_pass0_restoreFile("ADBase_settings$(N=1).sav")
set_pass1_restoreFile("ADBase_settings$(N=1).sav")

afterInit create_monitor_set,"ADBase_settings.req",30,"P=$(PREFIX),R=$(RECORD=cam1:)","ADBase_settings$(N=1).sav"
