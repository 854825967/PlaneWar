//
//  Boss.cpp
//  PlaneWars
//
//  Created by Alax on 11/24/14.
//
//

#include "Boss.h"
#include "Ejector.h"
#include "CEventEngine.h"

Boss * Boss::create(const BossConfig * pConfig) {
    Boss * ret = NEW Boss(pConfig);
    ret->autorelease();
    
    return ret;
}

Boss::~Boss() {
    ECHO("BOSS::~BOSS");
}

Boss::Boss(const BossConfig * pConfig) {
    m_pConfig = pConfig;
    init(m_pConfig->radius);
    string ejson = string("Animation/Boss/") + m_pConfig->res + ".ExportJson";
    ArmatureDataManager::getInstance()->addArmatureFileInfo(ejson);
    m_pBoss = Armature::create(m_pConfig->res);
    m_pBoss->setAnchorPoint(Vec2(.5f, .5f));
    this->addChild(m_pBoss);
    m_pBoss->getAnimation()->play("fly");
    setContentSize(m_pBoss->getContentSize());
    m_pBoss->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Boss::playActionOver));
    m_armor= pConfig->armor;
}

void Boss::startAi() {
    m_event_itor = m_pConfig->boss_events.cbegin();
    if (m_event_itor != m_pConfig->boss_events.cend()) {
        addAction(m_event_itor->actionList);
        if (m_event_itor->action.length() > 0) {
            m_pBoss->getAnimation()->play(m_event_itor->action);
        }
        BULLET_LIST::const_iterator list_itor = m_event_itor->bullets.cbegin();
        BULLET_LIST::const_iterator list_iend = m_event_itor->bullets.cend();
        while (list_iend != list_itor) {
            Ejector * ejector = Ejector::create(&(*list_itor), m_pConfig->atk, Vec2(0, 0), BULLET_TYPE_ENEMY);
            addChild(ejector);
            
            list_itor++;
        }
        schedule(schedule_selector(Boss::nextEvent), m_event_itor->eventCycle);
    }
}

void Boss::beinjured(const s32 value) {
    m_armor -= value;
    if (m_armor <= 0) {
        disappear();
    }
}

void Boss::disappear() {
    g_pEventEngine->BundlerCall(BUND_ID_BOSS_DISAPPEAR, this, sizeof(this));
    unscheduleAllSelectors();
    stopAllActions();
    string ejson = string("Animation/Effect/") + m_pConfig->boom + ".ExportJson";
    ArmatureDataManager::getInstance()->addArmatureFileInfo(ejson);
    m_pBoom = Armature::create(m_pConfig->boom);
    m_pBoom->getAnimation()->play("boom");
    this->addChild(m_pBoom);
    m_pBoom->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Boss::playActionOver));
}

void Boss::playActionOver(Armature * armature, MovementEventType type, const char * name) {
    if (m_pBoom == armature) {
        if (type == COMPLETE) {
            removeFromParentAndCleanup(true);
        }
    }
}

void Boss::nextEvent(float) {
    m_event_itor++;
    if (m_event_itor == m_pConfig->boss_events.cend()) {
        m_event_itor = m_pConfig->boss_events.cbegin();
    }
    
    if (m_event_itor != m_pConfig->boss_events.cend()) {
        schedule(schedule_selector(Boss::nextEvent), m_event_itor->eventCycle);
        addAction(m_event_itor->actionList);
        if (m_event_itor->action.length() > 0) {
            m_pBoss->getAnimation()->play(m_event_itor->action);
        }
        BULLET_LIST::const_iterator list_itor = m_event_itor->bullets.cbegin();
        BULLET_LIST::const_iterator list_iend = m_event_itor->bullets.cend();
        while (list_iend != list_itor) {
            Ejector * ejector = Ejector::create(&(*list_itor), m_pConfig->atk, Vec2(0, 0), BULLET_TYPE_ENEMY);
            addChild(ejector);
            
            list_itor++;
        }
    }
}
