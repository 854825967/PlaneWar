#ifndef CEVENTENGINE_H
#define CEVENTENGINE_H

#include "TCallBack.h"
#include "TBundler.h"

enum {
    EVENT_DO_NOTHING = 0,
    //event id
    EVENT_LOGIN_ACTION = 1,
    EVENT_REPLACE_SCENE_ACTION,
    EVENT_DATA_DIRTY,
    
    
    //BUND_ID
    BUND_ID_NOTHING = 10000,
    //ui
    BUND_ID_GLOBAL_TOUCH_BEGAN,
    BUND_ID_GLOBAL_TOUCH_MOVED,
    BUND_ID_GLOBAL_TOUCH_ENDED,
    BUND_ID_GLOBAL_TOUCH_CANCELED,
    
    
    //obj
    BUND_ID_CBOX_UNSELECTED,
    BUND_ID_EQUIP_REPLACE_EQUIP_RES,
    BUND_ID_EQUIP_SELL_RES,
    BUND_ID_VIEW_CELL_TOUCHED,
    BUND_ID_LEVEL_UP_EQUIP,
    
    BUND_ID_PLANE_FIRE,
    BUND_ID_PLANE_HOLD_FIRE,
    BUND_ID_WEAPON_FIRE,
    BUND_ID_WINGMAN_FIRE,
    BUND_ID_EJECTOR_FIRE,
    
    BUND_ID_WEAPON_HOLD_FIRE,
    BUND_ID_WINGMAN_HOLD_FIRE,
    BUND_ID_EJECTOR_HOLD_FIRE,
    BUND_ID_BULLET_LEVEL_UP,
    
    BUND_ID_BULLET_FIRED,
    BUND_ID_BULLET_DISAPPEARED,
    
    BUND_ID_BATTLE_EVENT,
    BUND_ID_ENEMY_EVENT,
    BUND_ID_BOSS_EVENT,
    
    BUND_ID_ENEMY_DISAPPEAR,
    BUND_ID_PLANE_DISAPPEAR,
    BUND_ID_PLANE_INJURED,
    
    BUND_ID_BOSS_DISAPPEAR,
    
    BUND_ID_GUEST,
    
    //ADD BEFOR THIS
    EVENT_ID_COUNT
};

#define RGS_EVENT(id, call) g_pEventEngine->RegisterStaticEvent(id, call, #call);
#define UNRGS_EVENT(id, call) g_pEventEngine->UnRegisterStaticEvent(id, call);
#define EVENT_CALL(id, context, size) g_pEventEngine->EventCall(id, context, size);

#define BIND(id, bundler, address) g_pEventEngine->Bind(id, bundler, (member_address)&address, #address);
#define UNBIND(id, bundler, address) g_pEventEngine->UnBind(id, bundler, (member_address)&address);

typedef void ( * system_call)(void  * , const s32);
typedef void (tlib::base_class::*member_address)(void *, const s32);

class CEventEngine {
public:
    static CEventEngine * getInterface();

    void RegisterStaticEvent(const s32 nEventID, const system_call call, const char * debug);
    void UnRegisterStaticEvent(const s32 nEventID, const system_call call);
    void EventCall(const s32 nEventID, void * pContext, const s32 size);
    
    void Bind(const s32 nEventID, void * pBundler, member_address address, const char * debug = "");
    void UnBind(const s32 nEventID, void * pBundler, member_address address);
    void UnBind(void * pBundler);
    void BundlerCall(const s32 nEventID, void * pContext, const s32 size);
    inline void ReflushBundler() {
        m_BundlerPool.reflush();
    }
    
    ~CEventEngine();
private:
    CEventEngine();
    
private:
    static CEventEngine * s_pSelf;
    tlib::TCallBack<s32, system_call, void  * , const s32> m_EventPool;
    tlib::bundler_pool<s32, void *, const s32> m_BundlerPool;
};

extern CEventEngine * g_pEventEngine;

#endif //CEVENTENGINE_H
