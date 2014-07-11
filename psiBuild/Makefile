include /ioc/tools/driver.makefile
PROJECT=ADZMQ
BUILDCLASSES = Linux
CROSS_COMPILER_TARGET_ARCHS=$(EPICS_HOST_ARCH)
SRCS += ../zmqApp/src/ZMQDriver.cpp
SRCS += ../zmqApp/src/JSON.cpp 
SRCS += ../zmqApp/src/JSONValue.cpp

SRCS += ../zmqApp/src/NDPluginZMQ.cpp
DBDS += ../zmqApp/src/ADZMQSupport.dbd

# convert SL5-x86, SL6-x86 to linux-x86
ARCH=linux-$(word 2,$(subst -, , $(EPICS_HOST_ARCH)))
USR_INCLUDES += -I ../../zmqApp/zmqSrc/
USR_LDFLAGS += -L ../../zmqApp/zmqSrc/os/$(ARCH)
USR_LIBS += zmq
