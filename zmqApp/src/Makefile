TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

# ADDriver
LIBRARY_IOC += ADZMQ

DBD += ADZMQSupport.dbd

ADZMQ_SRCS += ZMQDriver.cpp
#ADZMQ_SRCS += NDPluginZMQ.cpp
ADZMQ_SRCS += JSON.cpp JSONValue.cpp

ADZMQ_LIBS += zmq
#==================================
PROD_NAME = zmqApp
PROD_IOC = $(PROD_NAME)

# <name>.dbd will be created from <name>Include.dbd
DBD += $(PROD_NAME).dbd
zmqApp_DBD += base.dbd
zmqApp_DBD += asyn.dbd
zmqApp_DBD += ADZMQSupport.dbd
zmqApp_DBD += NDPluginSupport.dbd

zmqApp_SRCS += zmqApp_registerRecordDeviceDriver.cpp zmqAppMain.cpp

zmqApp_LIBS += ADZMQ
zmqApp_LIBS += asyn
zmqApp_LIBS += NDPlugin
zmqApp_LIBS += $(EPICS_BASE_IOC_LIBS)
zmqApp_SYS_LIBS += c++

#
#
include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

