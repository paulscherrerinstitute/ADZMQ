include /ioc/tools/driver.makefile
PROJECT=ADZMQ
BUILDCLASSES = Linux
EXCLUDE_VERSIONS=3.14.8
CROSS_COMPILER_TARGET_ARCHS=$(EPICS_HOST_ARCH)
SRCS += ../zmqApp/src/ZMQDriver.cpp
SRCS += ../zmqApp/src/JSON.cpp 
SRCS += ../zmqApp/src/JSONValue.cpp

SRCS += ../zmqApp/src/NDPluginZMQ.cpp
DBDS += ../zmqApp/src/ADZMQSupport.dbd

