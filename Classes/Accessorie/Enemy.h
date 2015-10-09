//
//  Enemy.h
//  PlaneWars
//
//  Created by Alax on 11/24/14.
//
//

#ifndef __PlaneWars__Enemy__
#define __PlaneWars__Enemy__

#include "MultiSys.h"
#include "Header.h"
#include "Collider.h"
class EnemyConfig;

class Enemy : public Collider {
public:
    static Enemy * create(const EnemyConfig * pConfig);
    void beinjured(const s32 atk);
    void disappear(float);

private:
    Enemy(const EnemyConfig * pConfig);
    void fire();
    void playActionOver(Armature * armature, MovementEventType type, const char * name);
private:
    ~Enemy();
    const EnemyConfig * m_pConfig;
    Armature * m_pEnemy;
    s32 m_armor;
};

#endif /* defined(__PlaneWars__Enemy__) */
