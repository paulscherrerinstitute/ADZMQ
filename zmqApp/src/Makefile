TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

LIBRARY_IOC += ADZMQ

USR_CPPFLAGS += -DBUILDING_ADZMQ_API

DBD += ADZMQSupport.dbd

ADZMQ_INC  += ADZMQAPI.h
ADZMQ_INC  += NDPluginZMQ.h

ADZMQ_SRCS += ZMQDriver.cpp
ADZMQ_SRCS += NDPluginZMQ.cpp
ADZMQ_SRCS += JSON.cpp JSONValue.cpp

ifeq ($(OS_CLASS),WIN32)
    LIBZMQ = libzmq
else
    LIBZMQ = zmq
endif

ADZMQ_LIBS += $(LIBZMQ) NDPlugin
#==================================
include $(ADCORE)/ADApp/commonLibraryMakefile
#
#
include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

