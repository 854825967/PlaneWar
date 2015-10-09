#ifndef _PUBLIC_UI_H_
#define _PUBLIC_UI_H_

#include "Header.h"
#include "TQueue.h"
#include "CEventEngine.h"
#include "CSelectModel.h"
#include "cocostudio/CocoStudio.h"
#include <list>
USING_NS_CC;
using namespace cocostudio;

enum {
    GRADE_BACKGOUND,
    GRADE_ENEMY,
    GRADE_BULLET,
    GRADE_PLAYER,
    GRADE_UI,
    GRADE_MAX
};

class CButton;

struct CMask {
    LayerColor * maskLayerColor;
    EventListenerTouchOneByOne *  touchListener;
    
    CMask() {
        memset(this, 0, sizeof(*this));
    }
};

typedef std::list<CMask> MASK_INFO_LIST;

#define ALERT(format, a...) { \
    char msg[256] = {0}; \
    SafeSprintf(msg, sizeof(msg), format, ##a); \
    alert(msg); \
}

extern tlib::CSelectModel g_tcpMgr;

class PublicUI : public cocos2d::Layer {

public:
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
	virtual void waiting();
	virtual void waitOver();
    virtual void alert(const char * info, const s32 evnetid = EVENT_DO_NOTHING);
    
    // flush http return
    void flush(float);
    
    ~PublicUI();
    
protected:
    void buttonAlert(Ref * sender);
    
    static void eventDirtydata(void *, const s32);
    
    
    void playActionOver(Armature * armature, MovementEventType type, const char * name);
    void touchover(cocos2d::Node * node);
    void addTouch(const cocos2d::Point & point);
    
    virtual bool onGlobalTouchBegan(Touch *touch, Event * event);
    virtual void onGlobalTouchEnded(Touch *touch, Event * event);
    virtual void onGlobalTouchCancelled(Touch *touch, Event * event);
    virtual void onGlobalTouchMoved(Touch *touch, Event * event);
    
    virtual bool onTouchBegan(Touch *touch, Event * event);
    virtual void onTouchEnded(Touch *touch, Event * event);
    virtual void onTouchCancelled(Touch *touch, Event * event);
    virtual void onTouchMoved(Touch *touch, Event * event);
    
    PublicUI();
    void mask();
    void unmask();
    
private:
    tlib::TQueue<s32, false, 8> m_EventIDQueue;
    MASK_INFO_LIST m_maskInfoList;
    cocos2d::EventListenerTouchOneByOne * m_listener;
};

#endif //_PUBLIC_UI_H_

