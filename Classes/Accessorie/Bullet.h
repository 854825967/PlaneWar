#ifndef BULLET_H
#define BULLET_H
#include "Collider.h"

class Bullet : public Collider {
public:
    static Bullet * create(const BulletConfig * pConfig, const s32 atk, const s8 type);
    
    inline s32 getAtk() const {return m_atk;}
    inline s8 type() const {return m_type;}
    inline const Point & getOffset() const {return m_pConfig->offset;}
    
    void disappear(float);
protected:
    Bullet(const BulletConfig * pConfig, const s32 atk, const s8 type);
    void playActionOver(Armature * armature, MovementEventType type, const char * name);
    
protected:
    s8 m_type;
    s32 m_atk;
    const BulletConfig * m_pConfig;
    Armature * m_pView;
};

#endif //BULLET_H
