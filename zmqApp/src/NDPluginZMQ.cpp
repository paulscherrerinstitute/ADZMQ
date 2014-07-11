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

#include <zmq.h>
#include "NDPluginZMQ.h"

#include <epicsExport.h>


static const char *driverName="NDPluginZMQ";

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
                sjson << "\"" << pAttr->pName << "\":" << *((epicsInt8*)value);
                break;
            case NDAttrUInt8:
                sjson << "\"" << pAttr->pName << "\":" << *((epicsUInt8*)value);
                break;
            case NDAttrInt16:
                sjson << "\"" << pAttr->pName << "\":" << *((epicsInt16*)value);
                break;
            case NDAttrUInt16:
                sjson << "\"" << pAttr->pName << "\":" << *((epicsUInt16*)value);
                break;
            case NDAttrInt32:
                sjson << "\"" << pAttr->pName << "\":" << *((epicsInt32*)value);
                break;
            case NDAttrUInt32:
                sjson << "\"" << pAttr->pName << "\":" << *((epicsUInt32*)value);
                break;
            case NDAttrFloat32:
                sjson << "\"" << pAttr->pName << "\":" << *((epicsFloat32*)value);
                break;
            case NDAttrFloat64:
                sjson << "\"" << pAttr->pName << "\":" << *((epicsFloat64*)value);
                break;
            case NDAttrString:
                sjson << "\"" << pAttr->pName << "\":" << "\"" << (char*)value << "\"";
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

/** Callback function that is called by the NDArray driver with new NDArray data.
  * \param[in] pArray  The NDArray from the callback.
  */ 
void NDPluginZMQ::processCallbacks(NDArray *pArray)
{
    int arrayCounter;
    std::string type;
    std::ostringstream shape;
    std::ostringstream header;
    NDArrayInfo_t arrayInfo;

    const char* functionName = "processCallbacks";

    /* Most plugins want to increment the arrayCounter each time they are called, which NDPluginDriver
     * does.  However, for this plugin we only want to increment it when we actually got a callback we were
     * supposed to save.  So we save the array counter before calling base method, increment it here */
    getIntegerParam(NDArrayCounter, &arrayCounter);

    /* Call the base class method */
    NDPluginDriver::processCallbacks(pArray);

    /* We always keep the last array so read() can use it.  
     * Release previous one, reserve new one */
    if (this->pArrays[0]) this->pArrays[0]->release();
    pArray->reserve();
    this->pArrays[0] = pArray;
 
    pArray->getInfo(&arrayInfo);

    /* Get NDArray attributes */

    /* compose JSON header */
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
            fprintf(stderr, "%s:%s: Data type not supported\n", driverName, functionName); 
            return;
    }

    shape << '[';
    for (int i=0; i<pArray->ndims; i++) {
        shape << pArray->dims[i].size;
        if (i != pArray->ndims - 1) 
            shape << ',';
    }
    shape << ']';

    header << "{\"htype\":[\"chunk-1.0\"], " 
        << "\"type\":" << "\"" << type << "\", "
        << "\"shape\":" << shape.str() << ", "
        << "\"frame\":" << pArray->uniqueId << ", " 
        << "\"ndattr\":" << getAttributesAsJSON(pArray->pAttributeList)
        << "}";
            
    /* send header*/
    std::string msg = header.str();
    zmq_send(this->socket, msg.c_str(), msg.length(), ZMQ_SNDMORE);
    /* send data */
    zmq_send(this->socket, pArray->pData, arrayInfo.totalBytes, 0);

    arrayCounter++;
    /* Update the parameters.  */
    setIntegerParam(NDArrayCounter, arrayCounter);
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
                     NDArrayPort, NDArrayAddr, maxAddr, NUM_NDPLUGIN_ZMQ_PARAMS, maxBuffers, maxMemory, 
                     asynGenericPointerMask, asynGenericPointerMask,
                     0, 1, priority, stackSize)
{
    //const char *functionName = "NDPluginZMQ";
    strcpy(this->serverHost, serverHost);

    /* Set the plugin type string */    
    setStringParam(NDPluginDriverPluginType, driverName);

    /* Create ZMQ pub socket */
    this->context = zmq_ctx_new();
    this->socket = zmq_socket(context, ZMQ_PUB);
    zmq_bind(this->socket, this->serverHost);

    /* Try to connect to the NDArray port */
    connectToArrayPort();
}

/** Configuration command */
extern "C" int NDZMQConfigure(const char *portName, const char *serverHost, int queueSize, int blockingCallbacks,
                                 const char *NDArrayPort, int NDArrayAddr,
                                 int maxBuffers, size_t maxMemory,
                                 int priority, int stackSize)
{
    new NDPluginZMQ(portName, serverHost, queueSize, blockingCallbacks, NDArrayPort, NDArrayAddr,
                      maxBuffers, maxMemory, priority, stackSize);
    return(asynSuccess);
}

/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName",iocshArgString};
static const iocshArg initArg1 = { "serverHost",iocshArgString};
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
