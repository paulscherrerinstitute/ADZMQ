#ifndef NDPluginZMQ_H
#define NDPluginZMQ_H

#include "NDPluginDriver.h"

#include "ADZMQAPI.h"

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255
#endif

/** Base class for NDArray ZMQ streaming plugins. */
class ADZMQ_API NDPluginZMQ : public NDPluginDriver {
public:
    NDPluginZMQ(const char *portName, const char *serverHost, int queueSize, int blockingCallbacks,
                 const char *NDArrayPort, int NDArrayAddr,
                 int maxBuffers, size_t maxMemory,
                 int priority, int stackSize);

    virtual ~NDPluginZMQ();

    /* These methods override those in the base class */
    virtual void report(FILE *fp, int detail);
    virtual void processCallbacks(NDArray *pArray);

protected:
    std::string getAttributesAsJSON(NDAttributeList *pAttributeList);
    virtual bool sendNDArray(NDArray *pArray);

    bool send(const char *message, size_t length, bool more);
    bool send(const std::string message, bool more);
    bool send(NDArray *pArray, bool more);

private:
    void *context;
    void *socket;
    char serverHost[HOST_NAME_MAX];
    int socketType;
    bool socketBind;
};

#define NUM_NDPLUGIN_ZMQ_PARAMS 0

#endif
