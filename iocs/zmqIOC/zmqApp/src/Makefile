TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

PROD_NAME = zmqApp
PROD_IOC += $(PROD_NAME)

# <name>.dbd will be created from <name>Include.dbd
DBD += $(PROD_NAME).dbd
$(PROD_NAME)_DBD += ADZMQSupport.dbd

$(PROD_NAME)_SRCS += zmqApp_registerRecordDeviceDriver.cpp zmqAppMain.cpp

# Add locally compiled object code
ifeq ($(OS_CLASS),WIN32)
    LIBZMQ = libzmq
else
    LIBZMQ = zmq
endif
$(PROD_NAME)_LIBS += ADZMQ $(LIBZMQ)

include $(ADCORE)/ADApp/commonDriverMakefile

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

