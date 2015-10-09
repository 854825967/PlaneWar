#include "Bullet.h"
#include "GameConfig.h"
#include "CEventEngine.h"

Bullet * Bullet::create(const BulletConfig * pConfig, const s32 atk, const s8 type) {
    Bullet * bullet = NEW Bullet(pConfig, atk, type);
    bullet->autorelease();
    bullet->setScale(g_pGameConfig->scaleEleMin);
    return bullet;
}

Bullet::Bullet(const BulletConfig * pConfig, const s32 atk, const s8 type) {
    m_atk = atk;
    m_type = type;
    m_pConfig = pConfig;
    schedule(schedule_selector(Bullet::disappear), 0, 0, m_pConfig->cycle);
    
    init(m_pConfig->radius);
    string ejson = string("Animation/Bullet/") + m_pConfig->res + ".ExportJson";
    //加载等待动画
    ArmatureDataManager::getInstance()->addArmatureFileInfo(ejson);
    m_pView = Armature::create(m_pConfig->res);
    m_pView->getAnimation()->play("fly");
    setContentSize(m_pView->getContentSize());
    addChild(m_pView);
}

void Bullet::disappear(float) {
    unschedule(schedule_selector(Bullet::disappear));
    g_pEventEngine->BundlerCall(BUND_ID_BULLET_DISAPPEARED, this, sizeof(this));
    removeChild(m_pView);
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Animation/Effect/hit1.ExportJson");
    Armature * effect = Armature::create("hit1");
    addChild(effect);
    stopAllActions();
    effect->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Bullet::playActionOver));
    effect->getAnimation()->playByIndex(0);
}

void Bullet::playActionOver(cocostudio::Armature *armature, cocostudio::MovementEventType type, const char *name) {
    if (type == COMPLETE) {
        removeFromParentAndCleanup(true);
    }
}
