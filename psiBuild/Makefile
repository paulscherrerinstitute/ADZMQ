include /ioc/tools/driver.makefile
PROJECT = ADZMQ
BUILDCLASSES = Linux WIN32
EXCLUDE_VERSIONS = 3.14.8
ARCH_FILTER = %-x86_64 win%

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
LIBOBJS_Darwin  += ../../zmqApp/zmqSrc/os/$(T_A)/libzmq.a
LIBOBJS_Linux  += ../zmqApp/zmqSrc/os/$(ARCH)/libzmq.a
USR_LDFLAGS_win32-x86  += ../../zmqApp/zmqSrc/os/win32-x86/libzmq.lib
USR_LDFLAGS_windows-x64  += ../../zmqApp/zmqSrc/os/windows-x64/libzmq.lib

SHRLIBS_win32-x86 += ../zmqApp/zmqSrc/os/win32-x86/libzmq.dll
SHRLIBS_windows-x64 += ../zmqApp/zmqSrc/os/windows-x64/libzmq.dll

TEMPLATES += ../zmqApp/Db/NDPluginZMQ.template
TEMPLATES += ../zmqApp/Db/NDPluginZMQ_settings.req
