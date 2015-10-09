#include "Ejector.h"
#include "CEventEngine.h"
#include "Bullet.h"
#include "GameConfig.h"
#include "Header.h"

Ejector::Ejector(const BulletConfig * pConfig, const s32 atk, const cocos2d::Point & point, const s8 type) {
    m_type = type;
    m_pConfig = pConfig;
    m_atk = atk;
    m_firePoint = point;
    BIND(BUND_ID_EJECTOR_HOLD_FIRE, this, Ejector::holdFire);
    schedule(schedule_selector(Ejector::fired), m_pConfig->interval, m_pConfig->repeat, m_pConfig->delay);
}

Ejector::~Ejector() {
    UNBIND(BUND_ID_EJECTOR_HOLD_FIRE, this, Ejector::holdFire);
}

Ejector * Ejector::create(const BulletConfig * pConfig, const s32 atk, const cocos2d::Point & point, const s8 type) {
    Ejector * ret = NEW Ejector(pConfig, atk, point, type);
    ret->autorelease();
    
    return ret;
}

void Ejector::holdFire(void * context, const s32) {
    if (this->getParent() == context) {
        removeFromParentAndCleanup(true);
    }
}

void Ejector::fired(float) {
    Bullet * bullet = Bullet::create(m_pConfig, m_atk, m_type);
    bullet->addAction(m_pConfig->actionList);
    bullet->setPosition(diyfun::getWorldPosition(getParent()->getPosition(),
                                                (m_firePoint + m_pConfig->offset) * getParent()->getScale(),
                                                 getParent()->getRotation()));
    bullet->setScale(getParent()->getScale());
    bullet->setRotation(m_pConfig->rotate);
    g_pEventEngine->BundlerCall(BUND_ID_BULLET_FIRED, bullet, sizeof(bullet));
}
