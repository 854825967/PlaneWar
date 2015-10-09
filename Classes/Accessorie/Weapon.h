#ifndef WEAPON_H
#define WEAPON_H

#include "cocos2d.h"
#include "Struct.h"

class Weapon : public cocos2d::Node {
public:
    static Weapon * create(const SEquip & equip);
    ~Weapon();
protected:
    Weapon(const WeaponConfig * pConfig);
    
    void OnCmdFire(void * context, const s32 size);
    void OnCmdHoldFire(void * context, const s32 size);
    void OnCmdBulletUp(void * context, const s32 size);
protected:
    const WeaponConfig * m_pConfig;
    s8 m_bullet_level;
    s32 m_atk;
};

#endif //WEAPON_H
