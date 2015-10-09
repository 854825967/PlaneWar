#ifndef CHTTPENGINE_H
#define CHTTPENGINE_H

#include "IHttpEngine.h"
#include "TQueue.h"
#include "TPool.h"
#include "CHashMap.h"
#include "CLock.h"
using namespace tlib;

#define HTTP_QUEUE_SIZE 16
#define HTTP_POOL_SIZE 32
typedef TStream<256, false>  HTTPStream;

namespace core {
    typedef CHashMap<s64, HTTPStream  * > MAP_ARGS;

    class CHttpEngine : public IHttpEngine {
    public:
        static CHttpEngine * getInterface();
        virtual bool HGetRequest(const s32 id, const char * url, IHttpHandler * pHandler, const void * context, 
                                 const s32 contextsize);
        
        virtual bool HPostRequest(const s32 id, const char * url, const void * data, const s32 datasize, 
                                  IHttpHandler * pHandler, const void * context, const s32 contextsize);

        virtual s64 HttpBackCall(const s64 tick);
        
        virtual void Start(const s8 nThreadCount);
        virtual void Stop();

        ~CHttpEngine();

    private:
        static THREAD_FUN HttpThread(void * p);
        CHttpEngine();
    private:
        static CHttpEngine * s_pSelf;
        
        s32 m_nThreadCount;
        TQueue<HTTPStream  * , false, HTTP_QUEUE_SIZE> * m_pHttpQueue;
        TQueue<HTTPStream  * , true, HTTP_QUEUE_SIZE> m_backQueue;
        TPOOL<HTTPStream, true, HTTP_POOL_SIZE> m_HTTPStreamPool;

        MAP_ARGS m_Map_BackArgs;
        bool m_shutdown;
        CLockUnit m_freelock;
    };
}

extern core::IHttpEngine * g_pHttpEngine;

#endif //CHTTPENGINE_H
