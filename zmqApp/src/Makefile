TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

LIBRARY_IOC += ADZMQ

DBD += ADZMQSupport.dbd

ADZMQ_SRCS += ZMQDriver.cpp
ADZMQ_SRCS += NDPluginZMQ.cpp
ADZMQ_SRCS += JSON.cpp JSONValue.cpp

ifeq ($(OS_CLASS),WIN32)
    LIBZMQ = libzmq
else
    LIBZMQ = zmq
endif

ADZMQ_LIBS += $(LIBZMQ)
#==================================
PROD_NAME = zmqApp
PROD_IOC = $(PROD_NAME)

# <name>.dbd will be created from <name>Include.dbd
DBD += $(PROD_NAME).dbd
zmqApp_DBD += ADZMQSupport.dbd

zmqApp_SRCS += zmqApp_registerRecordDeviceDriver.cpp zmqAppMain.cpp

zmqApp_LIBS += ADZMQ $(LIBZMQ)

include $(ADCORE)/ADApp/commonDriverMakefile
#
#
include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

