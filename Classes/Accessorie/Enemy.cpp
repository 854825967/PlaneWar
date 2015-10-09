//
//  Enemy.cpp
//  PlaneWars
//
//  Created by Alax on 11/24/14.
//
//

#include "Enemy.h"
#include "Struct.h"
#include "GameConfig.h"
#include "Ejector.h"
#include "CEventEngine.h"

Enemy * Enemy::create(const EnemyConfig * pConfig) {
    Enemy * ret = NEW Enemy(pConfig);
    ret->autorelease();
    
    return ret;
}

Enemy::Enemy(const EnemyConfig * pConfig) {
    m_pConfig = pConfig;
    init(m_pConfig->radius);
    m_armor = pConfig->armor;
    string ejson = string("Animation/Enemy/") + m_pConfig->res + ".ExportJson";
    ArmatureDataManager::getInstance()->addArmatureFileInfo(ejson);
    m_pEnemy = Armature::create(m_pConfig->res);
    m_pEnemy->getAnimation()->play("fly");
    this->addChild(m_pEnemy);
    addAction(m_pConfig->actionList);
    this->setContentSize(m_pEnemy->getContentSize());
    if (m_pConfig->cycle != 0) {
        scheduleOnce(schedule_selector(Enemy::disappear), m_pConfig->cycle);
    }
    fire();
}

Enemy::~Enemy() {
    
}

void Enemy::beinjured(const s32 atk) {
    m_armor -= atk;
    if (0 >= m_armor) {
        disappear(0);
    }
}

void Enemy::fire() {
    BULLET_LIST::const_iterator list_itor = m_pConfig->bullets.cbegin();
    BULLET_LIST::const_iterator list_iend = m_pConfig->bullets.cend();
    while (list_iend != list_itor) {
        Ejector * ejector = Ejector::create(&(*list_itor), m_pConfig->atk, Vec2(0, 0), BULLET_TYPE_ENEMY);
        addChild(ejector);
        
        list_itor++;
    }
}

void Enemy::playActionOver(cocostudio::Armature *armature, cocostudio::MovementEventType type, const char *name) {
    if (type == COMPLETE) {
        removeFromParentAndCleanup(true);
    }
}

void Enemy::disappear(float) {
    g_pEventEngine->BundlerCall(BUND_ID_ENEMY_DISAPPEAR, this, sizeof(this));

    unschedule(schedule_selector(Enemy::disappear));
    string ejson = string("Animation/Effect/") + m_pConfig->boom + ".ExportJson";
    ArmatureDataManager::getInstance()->addArmatureFileInfo(ejson);
    Armature * boomeffect = Armature::create(m_pConfig->boom);
    boomeffect->getAnimation()->play("boom");
    this->addChild(boomeffect);
    boomeffect->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Enemy::playActionOver));
}
