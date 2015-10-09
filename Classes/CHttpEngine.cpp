#include "CHttpEngine.h"
#include "Tools.h"

#ifdef WIN32 
#include "curl/include/win32/curl/curl.h"
#elif(CC_TARGET_PLATFORM ==  CC_PLATFORM_IOS)
#include "curl/include/ios/curl/curl.h"
#endif

core::IHttpEngine * g_pHttpEngine = core::CHttpEngine::getInterface();

enum {
    HTTP_REQUEST_GET, 
    HTTP_REQUEST_POST, 
    //HTTP_REQUEST_HTTPS, 

    //ADD BEFORE THIS
    HTTP_REQUEST_TYPE_COUNT
};

namespace core {
    struct HTTPContext {
        s32 id;
        s8 type;
        union {
            HTTPStream * pUrl;
            HTTPStream * pBack;
        };
        IHttpHandler * pHandler;
        HTTPStream * pContext;
        s32 code;
        HTTPContext() {
            memset(this, 0, sizeof(*this));
        }
        
        HTTPContext(void * p) {
            memcpy(this, p, sizeof(*this));
        }
    };

    
    CHttpEngine * CHttpEngine::s_pSelf = NULL;
    
    CHttpEngine * CHttpEngine::getInterface() {
        if (NULL == s_pSelf) {
            s_pSelf = NEW CHttpEngine;
        }
        
        return s_pSelf;
    }
    
    CHttpEngine::CHttpEngine() {
        
    }
    
    CHttpEngine::~CHttpEngine() {
        Stop();
        delete[] m_pHttpQueue;
        s_pSelf = NULL;
    }
    
    bool CHttpEngine::HGetRequest(const s32 id, const char * url, IHttpHandler * pHandler, 
                                  const void * context, const s32 contextsize) {
        
        static s32 queueid = 0;
        if (m_shutdown) {
            return false;
        }

        HTTPContext http;
        http.id = id;
        http.type = HTTP_REQUEST_GET;
        http.pHandler = pHandler;
        http.pUrl = m_HTTPStreamPool.Create();
        http.pUrl->in(url, strlen(url) + 1);

        if (context != NULL && contextsize != 0) {
            http.pContext = m_HTTPStreamPool.Create();
            http.pContext->in(context, contextsize);
        }

        HTTPStream * pStream = m_HTTPStreamPool.Create();
        pStream->in(&http, sizeof(http));
        return m_pHttpQueue[queueid++ % m_nThreadCount].add2(pStream);
    }

    bool CHttpEngine::HPostRequest(const s32 id, const char * url, const void * data, const s32 datasize, IHttpHandler * pHandler, const void * context, const s32 contextsize) {

        return false;
    }

    s64 CHttpEngine::HttpBackCall(const s64 lTick) {
        s64 lStartTick = ::GetCurrentTimeTick();
        HTTPStream * pStream = NULL;

        while (true) {
            if (m_backQueue.read(pStream)) {
                ASSERT(pStream->size() == sizeof(HTTPContext));
                HTTPContext * pHttp = (HTTPContext *)pStream->buff();
                ASSERT(pHttp->pHandler !=  NULL);
                
                const void * pData = NULL;
                const s32 size = pHttp->pBack->size() - 1;
                if (size > 0) {
                    pData = pHttp->pBack->buff();
                }
                
                if (pHttp->pContext !=  NULL) {
                    pHttp->pHandler->OnHttpBack(pHttp->id, pHttp->code,
                                                pData, size,
                                                pHttp->pContext->buff(), pHttp->pContext->size());
                } else {
                    pHttp->pHandler->OnHttpBack(pHttp->id, pHttp->code,
                                                pData, size,
                                                NULL, 0);
                }
                
                if (pHttp->pContext != NULL) {
                    pHttp->pContext->clear();
                    m_HTTPStreamPool.Recover(pHttp->pContext);
                }
                pHttp->pBack->clear();
                m_HTTPStreamPool.Recover(pHttp->pBack);
                pStream->clear();
                m_HTTPStreamPool.Recover(pStream);
            } else if (m_backQueue.IsEmpty()) {
                return ::GetCurrentTimeTick() - lStartTick;
            }

            s64 lRunTick = ::GetCurrentTimeTick() - lStartTick;
            if (lRunTick >=  lTick) {
                return lRunTick;
            }
        }
    }

    struct ThreadArgs {
        CHttpEngine * pHttpEngine;
        s32 nQueueID;

        ThreadArgs() {
            memset(this, 0, sizeof( * this));
        }
    };

    void CHttpEngine::Start(const s8 nThreadCount) {
        m_shutdown = false;
        m_nThreadCount = nThreadCount;
        m_pHttpQueue = NEW TQueue<HTTPStream  * , false, HTTP_QUEUE_SIZE>[nThreadCount];
        for (s32 i = 0; i<nThreadCount; i++) {
            ThreadArgs * pArgs = NEW ThreadArgs;
            pArgs->pHttpEngine = this;
            pArgs->nQueueID = i;

#if defined WIN32 || defined WIN64
            HANDLE hThread = ::CreateThread(NULL, 0, CHttpEngine::HttpThread, (LPVOID)pArgs, 0, NULL);
            CloseHandle(hThread);
#endif //defined WIN32 || defined WIN64
            
#if defined linux || defined CC_TARGET_OS_IPHONE || defined __ANDROID__
            pthread_t id;
            s32 hRThread = pthread_create(&id, NULL, CHttpEngine::HttpThread, pArgs);
            ASSERT(hRThread ==  0);
#endif //defined linux || defined CC_TARGET_OS_IPHONE

        }

    }

    void CHttpEngine::Stop() {
        if (!m_shutdown) {
            m_shutdown = true;
            while (m_nThreadCount !=  0){
                CSleep(10);
            }
            HttpBackCall(9999999);
        }
    }

    static size_t write_callback(void  * ptr, size_t size, size_t nmemb, void * stream) {
        ((HTTPStream  * )stream)->in(ptr, nmemb * size);
        return nmemb * size;
    }  

    THREAD_FUN CHttpEngine::HttpThread(void * p) {
        CHttpEngine * pThis = ((ThreadArgs  * ) p)->pHttpEngine;
        s32 nQueueID = ((ThreadArgs  * ) p)->nQueueID;
        TQueue<HTTPStream  * , false, HTTP_QUEUE_SIZE> * pQueue = &(pThis->m_pHttpQueue[nQueueID]);
        HTTPStream * pStream = NULL;

        CURL * pCurlHandle = curl_easy_init();

        while (true) {
            if (pQueue->read(pStream)) {
                curl_easy_reset(pCurlHandle);
                curl_easy_setopt(pCurlHandle, CURLOPT_TIMEOUT, 10);
                curl_easy_setopt(pCurlHandle, CURLOPT_VERBOSE, 0L);  
                curl_easy_setopt(pCurlHandle, CURLOPT_WRITEFUNCTION, write_callback); 

                ASSERT(pStream->size() == sizeof(HTTPContext));
                HTTPContext * pHttp = (HTTPContext *)pStream->buff();
                //这个变量可作为接收或传递数据的作用
                curl_easy_setopt(pCurlHandle, CURLOPT_URL, pHttp->pUrl->buff());
                HTTPStream * pBack = pThis->m_HTTPStreamPool.Create();
                curl_easy_setopt(pCurlHandle, CURLOPT_WRITEDATA, pBack);
                
                pHttp->code = curl_easy_perform(pCurlHandle);
                char end = '\0';
                pBack->in(&end, sizeof(end));
                pHttp->pUrl->clear();
                pThis->m_HTTPStreamPool.Recover(pHttp->pUrl);
                pHttp->pBack = pBack;
                
                pThis->m_backQueue.add(pStream);
            } else if (pQueue->IsEmpty() && pThis->m_shutdown){
                curl_easy_cleanup(pCurlHandle);
                delete p;
                CAutoLock(&(pThis->m_freelock));
                pThis->m_nThreadCount--;
                return 0;
            } else {
                CSleep(10);
            }
        }
    }
}
