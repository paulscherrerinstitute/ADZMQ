/*
 * NDPluginZMQ.cpp
 *
 * Asyn driver for callbacks to stream area detector data using ZMQ.
 *
 * Author: Xiaoqiang Wang
 *
 * Created June 10, 2014
 */

#include <stdlib.h>
#include <string.h>
#include <string>
#include <iocsh.h>
#include <sstream>
#include <iomanip>

#include <zmq.h>
#include "NDPluginZMQ.h"
#include <ADCoreVersion.h>

#include <epicsExport.h>


static const char *driverName="NDPluginZMQ";

/** Helper function used by zmq_msg to release NDArray
 */
void free_NDArray(void *data, void *hint)
{
    ((NDArray *)hint)->release();
}

/** Helper function to convert NDAttributeList to JSON object
 * \param[in] pAttributeList The NDAttributeList.
 */
std::string NDPluginZMQ::getAttributesAsJSON(NDAttributeList *pAttributeList)
{
    std::stringstream sjson;
    sjson << '{';

    NDAttribute *pAttr = pAttributeList->next(NULL);
    while (pAttr != NULL) {
        NDAttrDataType_t attrDataType;
        size_t attrDataSize;
        void * value;

        pAttr->getValueInfo(&attrDataType, &attrDataSize);
        value = calloc(1, attrDataSize);
        pAttr->getValue(attrDataType, value, attrDataSize);

        switch (attrDataType) {
            case NDAttrInt8:
                sjson << "\"" << pAttr->getName() << "\":" << *((epicsInt8*)value);
                break;
            case NDAttrUInt8:
                sjson << "\"" << pAttr->getName() << "\":" << *((epicsUInt8*)value);
                break;
            case NDAttrInt16:
                sjson << "\"" << pAttr->getName() << "\":" << *((epicsInt16*)value);
                break;
            case NDAttrUInt16:
                sjson << "\"" << pAttr->getName() << "\":" << *((epicsUInt16*)value);
                break;
            case NDAttrInt32:
                sjson << "\"" << pAttr->getName() << "\":" << *((epicsInt32*)value);
                break;
            case NDAttrUInt32:
                sjson << "\"" << pAttr->getName() << "\":" << *((epicsUInt32*)value);
                break;
            case NDAttrFloat32:
                sjson << "\"" << pAttr->getName() << "\":" << std::setprecision(9) << *((epicsFloat32*)value);
                break;
            case NDAttrFloat64:
                sjson << "\"" << pAttr->getName() << "\":" << std::setprecision(17) << *((epicsFloat64*)value);
                break;
            case NDAttrString:
                sjson << "\"" << pAttr->getName() << "\":" << "\"" << (char*)value << "\"";
                break;
            default:
                break;
        }
        free(value);
        pAttr = pAttributeList->next(pAttr);
        if (pAttr != NULL)
            sjson << ',';
    }
    sjson << '}';

    return sjson.str();
}

bool NDPluginZMQ::sendNDArray(NDArray *pArray)
{
    std::string type;
    std::ostringstream shape;
    std::ostringstream header;
    NDArrayInfo_t arrayInfo;
    const char* functionName = "sendNDArray";

    pArray->getInfo(&arrayInfo);

    /* Compose JSON header */
    switch (pArray->dataType){
        case NDInt8:
            type = "int8";
            break;
        case NDUInt8:
            type = "uint8";
            break;
        case NDInt16:
            type = "int16";
            break;
        case NDUInt16:
            type = "uint16";
            break;
        case NDInt32:
            type = "int32";
            break;
        case NDUInt32:
            type = "uint32";
            break;
        default:
            asynPrint(pasynUserSelf, ASYN_TRACE_WARNING,
                    "%s::%s Data type not supported %d\n",
                    driverName, functionName, pArray->dataType);
            return false;
    }

    shape << '[';
    for (int i=0; i<pArray->ndims; i++) {
        shape << pArray->dims[i].size;
        if (i != pArray->ndims - 1)
            shape << ',';
    }
    shape << ']';

    header << "{\"htype\":[\"array-1.0\"], "
        << "\"type\":" << "\"" << type << "\", "
        << "\"shape\":" << shape.str() << ", "
        << "\"frame\":" << pArray->uniqueId << ", "
        << "\"timeStamp\":" << std::setprecision(17) << pArray->timeStamp << ", "
#if ADCORE_VERSION >= 3
        << "\"encoding\":" << "\"" << pArray->codec.name << "\", "
#endif
        << "\"ndattr\":" << getAttributesAsJSON(pArray->pAttributeList)
        << "}";

    /* Send header*/
    std::string msg = header.str();
    zmq_msg_t msg_header;
    zmq_msg_init_size(&msg_header, msg.length());
    memcpy(zmq_msg_data(&msg_header), msg.c_str(), msg.length());
    int rc = zmq_msg_send(&msg_header, this->socket, ZMQ_SNDMORE|ZMQ_DONTWAIT);
    if (rc == -1) {
        zmq_msg_close(&msg_header);
        return false;
    } else {
        /* Send data */
        pArray->reserve();
        zmq_msg_t msg_data;
        zmq_msg_init_data(&msg_data, pArray->pData, arrayInfo.totalBytes, free_NDArray, pArray);
        int rc = zmq_msg_send(&msg_data, this->socket, ZMQ_DONTWAIT);
        /* ZeroMQ ensures it sends none or all message parts, so it is unlikely to error only on the 2nd part */
        if (rc == -1) {
            zmq_msg_close(&msg_data);
            return false;
        }
    }
    return true;
}

/** Callback function that is called by the NDArray driver with new NDArray data.
  * \param[in] pArray  The NDArray from the callback.
  */
void NDPluginZMQ::processCallbacks(NDArray *pArray)
{
    int arrayCounter;
    bool wasDropped = false;
    bool wasThrottled = false;

    const char* functionName = "processCallbacks";

    /* Most plugins want to increment the arrayCounter each time they are called, which NDPluginDriver
     * does.  However, for this plugin we only want to increment it when we actually got a callback we were
     * supposed to save.  So we save the array counter before calling base method, increment it here */
    getIntegerParam(NDArrayCounter, &arrayCounter);

    /* Call the base class method */
#if ADCORE_VERSION >= 3
    NDPluginDriver::beginProcessCallbacks(pArray);
#else
    NDPluginDriver::processCallbacks(pArray);
    /* We always keep the last array so read() can use it.
     * Release previous one, reserve new one */
    if (this->pArrays[0]) this->pArrays[0]->release();
    pArray->reserve();
    this->pArrays[0] = pArray;
#endif

    this->unlock();

#if ADCORE_VERSION >= 3
    if (throttled(pArray))
        wasThrottled = true;
    else
#endif
    {
        if (!this->sendNDArray(pArray))
            wasDropped = true;
    }
    this->lock();

    if (wasThrottled || wasDropped) {
#if ADCORE_VERSION >= 3
        int reason = NDPluginDriverDroppedOutputArrays;
#else
        int reason = NDPluginDriverDroppedArrays;
#endif
        int droppedOutputArrays;
        getIntegerParam(reason, &droppedOutputArrays);
        if (wasThrottled)
            asynPrint(pasynUserSelf, ASYN_TRACE_WARNING,
                    "%s::%s maximum byte rate exceeded, dropped array uniqueId=%d\n",
                    driverName, functionName, pArray->uniqueId);
        if (wasDropped)
            asynPrint(pasynUserSelf, ASYN_TRACE_WARNING,
                    "%s::%s ZeroMQ socket dropped array uniqueId=%d\n",
                    driverName, functionName, pArray->uniqueId);
        droppedOutputArrays++;
        setIntegerParam(reason, droppedOutputArrays);
    }

    /* Update the parameters.  */
#if ADCORE_VERSION >= 3
    NDPluginDriver::endProcessCallbacks(pArray, true, true);
#else
    arrayCounter++;
    setIntegerParam(NDArrayCounter, arrayCounter);
#endif
    callParamCallbacks();
}

/** Constructor for NDPluginZMQ; all parameters are simply passed to NDPluginDriver::NDPluginDriver.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] queueSize The number of NDArrays that the input queue for this plugin can hold when
  *            NDPluginDriverBlockingCallbacks=0.  Larger queues can decrease the number of dropped arrays,
  *            at the expense of more NDArray buffers being allocated from the underlying driver's NDArrayPool.
  * \param[in] blockingCallbacks Initial setting for the NDPluginDriverBlockingCallbacks flag.
  *            0=callbacks are queued and executed by the callback thread; 1 callbacks execute in the thread
  *            of the driver doing the callbacks.
  * \param[in] NDArrayPort Name of asyn port driver for initial source of NDArray callbacks.
  * \param[in] NDArrayAddr asyn port driver address for initial source of NDArray callbacks.
  * \param[in] maxAddr The maximum  number of asyn addr addresses this driver supports. 1 is minimum.
  * \param[in] numParams The number of parameters supported by the derived class calling this constructor.
  * \param[in] maxBuffers The maximum number of NDArray buffers that the NDArrayPool for this driver is
  *            allowed to allocate. Set this to -1 to allow an unlimited number of buffers.
  * \param[in] maxMemory The maximum amount of memory that the NDArrayPool for this driver is
  *            allowed to allocate. Set this to -1 to allow an unlimited amount of memory.
  * \param[in] interfaceMask Bit mask defining the asyn interfaces that this driver supports.
  * \param[in] interruptMask Bit mask definining the asyn interfaces that can generate interrupts (callbacks)
  * \param[in] asynFlags Flags when creating the asyn port driver; includes ASYN_CANBLOCK and ASYN_MULTIDEVICE.
  * \param[in] autoConnect The autoConnect flag for the asyn port driver.
  * \param[in] priority The thread priority for the asyn port driver thread if ASYN_CANBLOCK is set in asynFlags.
  * \param[in] stackSize The stack size for the asyn port driver thread if ASYN_CANBLOCK is set in asynFlags.
  */
NDPluginZMQ::NDPluginZMQ(const char *portName, const char* serverHost, int queueSize, int blockingCallbacks,
                           const char *NDArrayPort, int NDArrayAddr,
                           int maxBuffers, size_t maxMemory,
                           int priority, int stackSize)
    /* Invoke the base class constructor.
     * We allocate 1 NDArray of unlimited size in the NDArray pool.
     * This driver can block (because writing a file can be slow), and it is not multi-device.
     * Set autoconnect to 1.  priority and stacksize can be 0, which will use defaults. */
    : NDPluginDriver(portName, queueSize, blockingCallbacks,
                     NDArrayPort, NDArrayAddr, 1,
#if ADCORE_VERSION < 3
                     0,
#endif
                     maxBuffers, maxMemory,
                     asynGenericPointerMask, asynGenericPointerMask,
                     0, 1, priority, stackSize
#if ADCORE_VERSION >= 3
                    ,1 /* single thread */, true /* compressionAware */
#endif
                     )
{
    const char *functionName = "NDPluginZMQ";
    char *cp;
    char type[10] = "";
    int rc = 0;

    /* server host in form of "transport://address [PUB|PUSH]"
     * separate host and type information */
    strcpy(this->serverHost, serverHost);
    if ((cp=strchr(this->serverHost, ' '))!=NULL) {
        *cp++ = '\0';
        strcpy(type, cp);
    }
    if (strcmp(type, "SUB") == 0 || strcmp(type, "PUB") == 0)
        this->socketType = ZMQ_PUB;
    else if (strcmp(type, "PULL") == 0 || strcmp(type, "PUSH") == 0)
        this->socketType = ZMQ_PUSH;
    else if (strlen(type) == 0) {
        /* If type is not specified, make a guess.
         * If "*" is found in host address, then it is assumed to be a PUB server type
         * */
        if (strchr(this->serverHost, '*')!=NULL) {
            this->socketType = ZMQ_PUB;
        } else {
            this->socketType = ZMQ_PUSH;
        }
    } else {
        fprintf(stderr, "%s: Unsupported socket type %s\n", functionName, type);
        return;
    }

    /* Set the plugin type string */
    setStringParam(NDPluginDriverPluginType, driverName);

    /* Create ZMQ pub socket */
    this->context = zmq_ctx_new();
    this->socket = zmq_socket(context, this->socketType);

    if (this->socketType == ZMQ_PUB) {
        rc = zmq_bind(this->socket, this->serverHost);
    } else if (this->socketType == ZMQ_PUSH) {
        rc = zmq_connect(this->socket, this->serverHost);
    }
    if (rc != 0) {
        fprintf(stderr, "%s: unable to bind/connect, %s\n",
                functionName,
                zmq_strerror(zmq_errno()));
        return;
    }

    /* Try to connect to the NDArray port */
    connectToArrayPort();
}

NDPluginZMQ::~NDPluginZMQ()
{
    if (this->socketType == ZMQ_PUB)
        zmq_unbind(this->socket, this->serverHost);
    else if (this->socketType == ZMQ_PUSH)
        zmq_disconnect(this->socket, this->serverHost);

    zmq_close(this->socket);
    zmq_ctx_destroy(this->context);
}

/** Configuration command */
extern "C" int NDZMQConfigure(const char *portName, const char *serverHost, int queueSize, int blockingCallbacks,
                                 const char *NDArrayPort, int NDArrayAddr,
                                 int maxBuffers, size_t maxMemory,
                                 int priority, int stackSize)
{
    NDPluginZMQ *pPlugin = new NDPluginZMQ(portName, serverHost, queueSize, blockingCallbacks, NDArrayPort, NDArrayAddr,
                      maxBuffers, maxMemory, priority, stackSize);
#if (ADCORE_VERSION > 2) || (ADCORE_VERSION == 2 && ADCORE_REVISION >= 5)
    return pPlugin->start();
#else
    return asynSuccess;
#endif
}

/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName",iocshArgString};
static const iocshArg initArg1 = { "transport://address [type]",iocshArgString};
static const iocshArg initArg2 = { "frame queue size",iocshArgInt};
static const iocshArg initArg3 = { "blocking callbacks",iocshArgInt};
static const iocshArg initArg4 = { "NDArrayPort",iocshArgString};
static const iocshArg initArg5 = { "NDArrayAddr",iocshArgInt};
static const iocshArg initArg6 = { "maxBuffers",iocshArgInt};
static const iocshArg initArg7 = { "maxMemory",iocshArgInt};
static const iocshArg initArg8 = { "priority",iocshArgInt};
static const iocshArg initArg9 = { "stackSize",iocshArgInt};
static const iocshArg * const initArgs[] = {&initArg0,
                                            &initArg1,
                                            &initArg2,
                                            &initArg3,
                                            &initArg4,
                                            &initArg5,
                                            &initArg6,
                                            &initArg7,
                                            &initArg8,
                                            &initArg9};
static const iocshFuncDef initFuncDef = {"NDZMQConfigure",10,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    NDZMQConfigure(args[0].sval, args[1].sval, args[2].ival, args[3].ival,
                     args[4].sval, args[5].ival, args[6].ival,
                     args[7].ival, args[8].ival, args[9].ival);
}

extern "C" void NDZMQRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

extern "C" {
epicsExportRegistrar(NDZMQRegister);
}
