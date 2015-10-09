//
//  Boss.h
//  PlaneWars
//
//  Created by Alax on 11/24/14.
//
//

#ifndef __PlaneWars__Boss__
#define __PlaneWars__Boss__

#include "MultiSys.h"
#include "Header.h"
#include "Collider.h"
#include "Struct.h"

class BossConfig;

class Boss : public Collider {
public:
    static Boss * create(const BossConfig * pConfig);
    void startAi();
    void beinjured(const s32 value);
    ~Boss();
private:
    Boss(const BossConfig * pConfig);
    void playActionOver(Armature * armature, MovementEventType type, const char * name);
    void nextEvent(float);
    void disappear();
    
private:
    const BossConfig * m_pConfig;
    Armature * m_pBoss;
    Armature * m_pBoom;
    s32 m_armor;
    BOSS_EVENT::const_iterator m_event_itor;
};

#endif /* defined(__PlaneWars__Boss__) */
