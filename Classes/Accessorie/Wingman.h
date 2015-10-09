#ifndef WINGMAN_H
#define WINGMAN_H

#include "cocos2d.h"
#include "Struct.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

class Wingman : public cocos2d::Node {
public:
    static Wingman * create(const SEquip & equip, const s8 wingmanType = WINGMAN_LEFT);
    ~Wingman();
    
protected:
    Wingman(const WingmanConfig * pConfig, const s8 wingmanType);
    
    void playActionOver(Armature * armature, MovementEventType type, const char * name);
    
    void OnCmdFire(void * context, const s32 size);
    void OnCmdBulletUp(void * context, const s32 size);
    
protected:
    const WingmanConfig * m_pConfig;
    Armature * m_pWingman;
    s8 m_bullet_level;
    s8 m_type;
    s32 m_atk;
};

#endif //WINGMAN_H
