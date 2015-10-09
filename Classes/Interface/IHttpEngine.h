#ifndef IHTTPENGINE_H
#define IHTTPENGINE_H

#include "IHttpHandler.h"

namespace core {
    class IHttpEngine {
    public:
        virtual bool HGetRequest(const s32 id, const char * url, IHttpHandler * pHandler,
                                 const void * context, const s32 contextsize) = 0;
        
        virtual bool HPostRequest(const s32 id, const char * url, const void * data, const s32 datasize,
                                  IHttpHandler * pHandler, const void * context, const s32 contextsize) = 0;
        
        virtual s64 HttpBackCall(const s64 tick) = 0;

        virtual void Start(const s8 nThreadCount) = 0;
        virtual void Stop() = 0;
    };
}

#endif //IHTTPENGINE_H
