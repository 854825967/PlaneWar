#ifndef IHTTPRES_H
#define IHTTPRES_H
#include "IHttpHandler.h"
#include "json/json.h"
#include "CEventEngine.h"
#include "SInterfaceDefine.h"
#include "Header.h"

class IHttpRes : public core::IHttpHandler {
public:
    void OnHttpBack(const s32 id, const s32 err, const void * data, const s32 datasize, const void * context, const s32 size) {
        if (NULL != data && 0 == err) {
            Json::Value root;
            Json::Reader reader;
            printf("http return:%s", (const char  * )data);
            if (reader.parse((const char  * )data, root)) {
                s32 nCode = root["code"].asInt();
                switch (nCode) {
                    case ERROR_CODE::ERROR_CODE_DIRTY_DATA:
                    {
                        EVENT_CALL(EVENT_DATA_DIRTY, NULL, 0);
                    }
                    default:
                        break;
                }
                this->OnHttpRes(id, &root, context, size);
                return;
            }
        } else {
        	LOG("http error %d", err);
        }
        
        this->OnHttpRes(id, NULL, context, size);
    }
    
protected:
    virtual void OnHttpRes(const s32 id, const Json::Value * pJsonRoot, const void * context, const s32 size) = 0;
};


#endif //IHTTPRES_H
