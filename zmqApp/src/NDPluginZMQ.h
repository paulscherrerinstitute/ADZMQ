#ifndef NDPluginZMQ_H
#define NDPluginZMQ_H

#include "NDPluginDriver.h"

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255
#endif

/** Base class for NDArray ZMQ streaming plugins. */
class NDPluginZMQ : public NDPluginDriver {
public:
    NDPluginZMQ(const char *portName, const char *serverHost, int queueSize, int blockingCallbacks, 
                 const char *NDArrayPort, int NDArrayAddr,
                 int maxBuffers, size_t maxMemory,
                 int priority, int stackSize);

    /* These methods override those in the base class */
    virtual void processCallbacks(NDArray *pArray);

protected:
    std::string getAttributesAsJSON(NDAttributeList *pAttributeList);

private:
    void *context;
    void *socket;
    char serverHost[HOST_NAME_MAX];
};

#define NUM_NDPLUGIN_ZMQ_PARAMS 0
    
#endif
