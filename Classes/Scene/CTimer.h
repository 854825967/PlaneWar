//
//  CTimer.h
//  PlaneWars
//
//  Created by Alax on 11/24/14.
//
//

#ifndef PlaneWars_CTimer_h
#define PlaneWars_CTimer_h

#include "MultiSys.h"
#include "cocos2d.h"
#include "CEventEngine.h"

class CTimer : public cocos2d::Node {
public:
    static CTimer * create(const void * pContext, const s32 id, const float interval, const s32 repeat, const float delay) {
        CTimer * ret = NEW CTimer(pContext, id, interval, repeat, delay);
        ret->autorelease();
        return ret;
    }
private:
    CTimer(const void * context, const s32 id, const float interval, const s32 repeat, const float delay) {
        m_pContext = (void *)context;
        m_id = id;
        schedule(schedule_selector(CTimer::timer_call), interval, repeat, delay);
    }
    
    void timer_call(float) {
        g_pEventEngine->BundlerCall(m_id, m_pContext, sizeof(m_pContext));
    }
    
private:
    void * m_pContext;
    s32 m_id;
};

#endif
