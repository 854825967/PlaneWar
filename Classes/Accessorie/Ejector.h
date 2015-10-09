#ifndef EJECTOR_H
#define EJECTOR_H
#include "cocos2d.h"
#include "MultiSys.h"

class BulletConfig;

class Ejector : public cocos2d::Node {
public:
    static Ejector * create(const BulletConfig * pConfig, const s32 atk, const cocos2d::Point & point, const s8 type);
    ~Ejector();
    
protected:
    Ejector(const BulletConfig * pConfig, const s32 atk, const cocos2d::Point & point, const s8 type);
    void fired(float);
    
protected:
    void holdFire(void *, const s32);
    const BulletConfig * m_pConfig;
    cocos2d::Point m_firePoint;
    s32 m_atk;
    s8 m_type;
};

#endif //EJECTOR_H
