include /ioc/tools/driver.makefile
PROJECT = ADZMQ
BUILDCLASSES = Linux WIN32
EXCLUDE_VERSIONS = 3 7.0.7 7.0.8
ARCH_FILTER = %-x86_64 win%

USR_CPPFLAGS += -DBUILDING_ADZMQ_API

HEADERS += ../zmqApp/src/ADZMQAPI.h
HEADERS += ../zmqApp/src/NDPluginZMQ.h

SOURCES += ../zmqApp/src/ZMQDriver.cpp
SOURCES += ../zmqApp/src/JSON.cpp 
SOURCES += ../zmqApp/src/JSONValue.cpp

SOURCES += ../zmqApp/src/NDPluginZMQ.cpp
DBDS += ../zmqApp/src/ADZMQSupport.dbd

