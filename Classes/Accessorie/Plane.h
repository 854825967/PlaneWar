#ifndef PLANE_H
#define PLANE_H

#include "Collider.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

class PlaneConfig;

class Plane : public Collider {
public:
    static Plane * create(const SEquip & equip);
    void bulletLevelUp();
    void addWeapon(const SEquip & equip);
    void addWingman(const SEquip & equip);
    void addArmor(const SEquip & equip);

    void bulletUp();
    void holdFire();
    void fire();

    //受伤
    void beInjured(const s32 value);
    void disappear();
    
    //非战斗接口
    void showtime();
    
    ~Plane();
private:
    Plane(const PlaneConfig * pConfig);
    void OnCmdFire(void * context, const s32 size);
    void OnCmdHoldFire(void * context, const s32 size);
    void playActionOver(Armature * armature, MovementEventType type, const char * name);
    void showtime(float);
private:
    const PlaneConfig * m_pConfig;
    Armature * m_pPlane;
    Armature * m_pBoom;
    s8 m_bullet_level;
    s32 m_atk;
    s32 m_armor;
};

#endif //PLANE_H
