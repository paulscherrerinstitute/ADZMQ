include /ioc/tools/driver.makefile
PROJECT = ADZMQ
BUILDCLASSES = Linux
EXCLUDE_VERSIONS = 3.14.8
ARCH_FILTER = %-x86_64

# SL6-x86 -> linux-x86
# SL6-x86_64 -> linux-x86_64
ARCH = linux-$(word 2,$(subst -, , $(T_A)))

SOURCES += ../zmqApp/src/ZMQDriver.cpp
SOURCES += ../zmqApp/src/JSON.cpp 
SOURCES += ../zmqApp/src/JSONValue.cpp

SOURCES += ../zmqApp/src/NDPluginZMQ.cpp
DBDS += ../zmqApp/src/ADZMQSupport.dbd

# link to package internal zmq
IGNORE_MODULES += ZMQ
OLD_INCLUDE =
USR_INCLUDES += -I../../zmqApp/zmqSrc/
USR_LDFLAGS  += -L../../zmqApp/zmqSrc/os/$(ARCH)
USR_LIBS += zmq
