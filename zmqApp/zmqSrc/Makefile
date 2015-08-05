TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

INC += zmq.h zmq_utils.h

# These libraries are built from zmq 4.0.4
ifeq (linux-x86_64, $(findstring linux-x86_64, $(T_A)))
LIB_INSTALLS_Linux += ../os/linux-x86_64/libzmq.a

else ifeq (linux-x86, $(findstring linux-x86, $(T_A)))
LIB_INSTALLS_Linux += ../os/linux-x86/libzmq.a

else ifeq (darwin-x86, $(findstring darwin-x86, $(T_A)))
LIB_INSTALLS_Darwin += ../os/darwin-x86/libzmq.a

else ifeq (win32-x86, $(findstring win32-x86, $(T_A)))
LIB_INSTALLS_WIN32 += ../os/win32-x86/libzmq.lib
BIN_INSTALLS_WIN32 += ../os/win32-x86/libzmq.dll

else ifeq (windows-x64, $(findstring windows-x64, $(T_A)))
LIB_INSTALLS_WIN32 += ../os/windows-x64/libzmq.lib
BIN_INSTALLS_WIN32 += ../os/windows-x64/libzmq.dll

endif

#=============================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

