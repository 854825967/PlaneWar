//
//  BattleScene.cpp
//  PlaneWars
//
//  Created by Alax on 11/23/14.
//
//

#include "BattleScene.h"
#include "GameConfig.h"
#include "CTimer.h"
#include "Enemy.h"
#include "Boss.h"
#include "Bullet.h"
#include "Plane.h"
#include "EquipManager.h"
#include "SInterfaceDefine.h"
#include "CHttpEngine.h"
#include "MainMenuScene.h"
#include "MissionScene.h"

Battle * Battle::s_pSelf = NULL;
string s_mission_name = "";
s32 s_enemy_count = 0;
MISSION_EVENT::const_iterator s_itor;

Battle::Battle() {
    ASSERT(s_pSelf ==  NULL);
    s_pSelf = this;
}

Battle::~Battle() {
    s_pSelf = NULL;
}

void Battle::setMissionConfig(const string &name) {
    s_mission_name = name;
}

Scene *  Battle::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Battle::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

void Battle::onEnter() {
    PublicUI::onEnter();
    CCASSERT(s_mission_name != ""
             && NULL != (m_pConfig = g_pGameConfig->getMissionConfig(s_mission_name)),
             "set mission first");
    
    schedule(schedule_selector(Battle::collidtion), 1/60.0f);
    
    m_pBoss = NULL;
    
    mask();
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Animation/UI/readygo.ExportJson");
    m_pReadygo = Armature::create("readygo");
    m_pReadygo->setAnchorPoint(Vec2(.5f, .5f));
    this->addChild(m_pReadygo, GRADE_UI);
    m_pReadygo->setPosition(g_pGameConfig->screenCenterPoint);
    m_pReadygo->getAnimation()->play("readygo");
    m_pReadygo->setScale(g_pGameConfig->scaleEleMin * 1.5f);
    m_pReadygo->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Battle::playActionOver));
    
    s_enemy_count = 0;
    s_itor = m_pConfig->events.begin();
    if (s_itor != m_pConfig->events.end()) {
        CTimer * timer = CTimer::create(&(*s_itor), BUND_ID_BATTLE_EVENT, 0, 0, s_itor->delay);
        addChild(timer);
    }
    
    BIND(BUND_ID_BULLET_FIRED, this, Battle::bulletFired);
    BIND(BUND_ID_BULLET_DISAPPEARED, this, Battle::bulletDisappear);
    BIND(BUND_ID_BATTLE_EVENT, this, Battle::event_exec);
    BIND(BUND_ID_ENEMY_EVENT, this, Battle::enemy_event);
    BIND(BUND_ID_ENEMY_DISAPPEAR, this, Battle::enemydisappear);
    BIND(BUND_ID_PLANE_DISAPPEAR, this, Battle::planedisappear);
    BIND(BUND_ID_PLANE_INJURED, this, Battle::planeinjured);
    BIND(BUND_ID_GLOBAL_TOUCH_BEGAN, this, Battle::globalTouchBegan);
    BIND(BUND_ID_BOSS_DISAPPEAR, this, Battle::bossdisappear);
    
    SEquip equip;
    g_pEquipManager->getGrooveEquip(EQUIP_TYPE_PLANE, equip);
    m_pPlane = Plane::create(equip);
    m_pPlane->setScale(g_pGameConfig->scaleEleMin * .75f);
    g_pEquipManager->getGrooveEquip(EQUIP_TYPE_WEAPON, equip);
    m_pPlane->addWeapon(equip);
    g_pEquipManager->getGrooveEquip(EQUIP_TYPE_WINGMAN, equip);
    m_pPlane->addWingman(equip);
    
    g_pEquipManager->getGrooveEquip(EQUIP_TYPE_ARMOR, equip);
    m_pPlane->addArmor(equip);
    
    m_pPlane->setPosition(Vec2(g_pGameConfig->screenCenterPoint.x, 0));
    addChild(m_pPlane, GRADE_PLAYER);
    ActionInterval * action = EaseExponentialOut::create(MoveBy::create(2, Vec2(0, 500 * g_pGameConfig->scaleEleY)));
    m_pPlane->runAction(action);
}

void Battle::onExit() {
    PublicUI::onExit();
    s_mission_name = "";
    
    UNBIND(BUND_ID_BULLET_FIRED, this, Battle::bulletFired);
    UNBIND(BUND_ID_BULLET_DISAPPEARED, this, Battle::bulletDisappear);
    UNBIND(BUND_ID_BATTLE_EVENT, this, Battle::event_exec);
    UNBIND(BUND_ID_ENEMY_EVENT, this, Battle::enemy_event);
    UNBIND(BUND_ID_ENEMY_DISAPPEAR, this, Battle::enemydisappear);
    UNBIND(BUND_ID_PLANE_DISAPPEAR, this, Battle::planedisappear);
    UNBIND(BUND_ID_PLANE_INJURED, this, Battle::planeinjured);
    UNBIND(BUND_ID_BOSS_DISAPPEAR, this, Battle::bossdisappear);
}

// on "init" you need to initialize your instance
bool Battle::init() {
    //////////////////////////////
    // 1. super init first
    if (!PublicUI::init()) {
        return false;
    }
    
    m_pBackground = Sprite::create("UI/mainMenu/mainMenuBk.png");
    m_pBackground->setScale(g_pGameConfig->scaleEleMax);
    m_pBackground->setPosition(Point(g_pGameConfig->screenCenterPoint.x, g_pGameConfig->screenCenterPoint.y));
    this->addChild(m_pBackground, GRADE_BACKGOUND);
    return true;
}

void Battle::event_exec(void * pContext, s32 id) {
    const EventConfig * event = (const EventConfig *) pContext;
    ENEMY_EVENT::const_iterator itor = event->enemy_events.begin();
    ENEMY_EVENT::const_iterator iend = event->enemy_events.end();
    
    while (itor != iend) {
        CTimer * timer = CTimer::create(&(*itor), BUND_ID_ENEMY_EVENT, itor->interval, itor->repeat, itor->delay);
        addChild(timer);
        s_enemy_count += itor->repeat + 1;
        itor++;
    }
}

void Battle::planeinjured(void * context, const s32 size) {
    CCASSERT(m_pPlane == context, "fuck u");
    if (m_pPlane == context) {
        runAction(Sequence::create(
                                   MoveBy::create(0.05f, Vec2(20, 0)),
                                   MoveBy::create(0.1f, Vec2(-40, 0)),
                                   MoveBy::create(0.05f, Vec2(40, 0)),
                                   MoveBy::create(0.1f, Vec2(-40, 0)),
                                   MoveBy::create(0.05f, Vec2(20, 0)),
                                   NULL));
    }
}

void Battle::bossdisappear(void *context, const s32 size) {
    CCASSERT(context == m_pBoss, "where r u come from");
    m_pBoss = NULL;
    requestpve();
}

void Battle::planedisappear(void *context, const s32 size) {
    CCASSERT(m_pPlane == context, "where r u from");
    UNBIND(BUND_ID_GLOBAL_TOUCH_BEGAN, this, Battle::globalTouchBegan);
    UNBIND(BUND_ID_GLOBAL_TOUCH_MOVED, this, Battle::globalTouchMoved);
    UNBIND(BUND_ID_PLANE_DISAPPEAR, this, Battle::planedisappear);
    m_pPlane = NULL;
    unschedule(schedule_selector(Battle::collidtion));
    scheduleOnce(schedule_selector(Battle::pve_failed), 2.0f);
    planeinjured(NULL, 0);
}

void Battle::enemydisappear(void *context, const s32 size) {
    Enemy * enemy = (Enemy *)context;
    ENEMY_GROUP::iterator iend = m_enemyGroup.end();
    ENEMY_GROUP::iterator itor = ::find(m_enemyGroup.begin(), iend, enemy);
    if (itor == iend) {
        CCASSERT(false, "... where r u from");
    } else {
        m_enemyGroup.erase(itor);
    }
    
    s_enemy_count--;
    CCASSERT(s_enemy_count>=0, "check ur enemy");
    if (0 == s_enemy_count) {
        s_itor++;
        if (s_itor != m_pConfig->events.end()) {
            CTimer * timer = CTimer::create(&(*s_itor), BUND_ID_BATTLE_EVENT, 0, 0, s_itor->delay);
            addChild(timer);
        } else {
            if (m_pConfig->boss != "") {
                mask();
                g_pEventEngine->BundlerCall(BUND_ID_PLANE_HOLD_FIRE, this, sizeof(this));
                ArmatureDataManager::getInstance()->addArmatureFileInfo("Animation/UI/bosswarning.ExportJson");
                m_pBossIn = Armature::create("bosswarning");
                m_pBossIn->setAnchorPoint(Vec2(.5f, .5f));
                this->addChild(m_pBossIn, GRADE_UI);
                m_pBossIn->setPosition(g_pGameConfig->screenCenterPoint);
                m_pBossIn->getAnimation()->playWithIndex(0);
                m_pBossIn->setScale(g_pGameConfig->scaleEleMin * 1.5f);
                m_pBossIn->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Battle::playActionOver));
            } else {
                requestpve();
            }
        }
    }
}

void Battle::requestpve() {
    g_pEventEngine->BundlerCall(BUND_ID_PLANE_HOLD_FIRE, this, sizeof(this));
    char szUrl[256] = {0};
    SafeSprintf(szUrl, sizeof(szUrl), SERVER::countpve_url, g_uid.c_str(), g_token.c_str(), 10);
    g_pHttpEngine->HGetRequest(HTTP_REQUEST_ID_COUNT_PVE, szUrl, this, NULL, 0);
    unschedule(schedule_selector(Battle::collidtion));
}

void Battle::enemy_event(void * pContext, s32 id) {
    const EnemyConfig * enemyCfg = (const EnemyConfig *) pContext;
    Enemy * enemy = Enemy::create(enemyCfg);
    enemy->setRotation(enemyCfg->rotate);
    enemy->setScale(g_pGameConfig->scaleEleMin);
    enemy->setPosition(Vec2(enemyCfg->pos.x * g_pGameConfig->scaleEleX, enemyCfg->pos.y * g_pGameConfig->scaleEleY));
    addChild(enemy, GRADE_ENEMY);
    
    m_enemyGroup.push_back(enemy);
}

void Battle::playbossAi(cocos2d::Node *pSender) {
    m_pBoss->startAi();
    g_pEventEngine->BundlerCall(BUND_ID_PLANE_FIRE, this, sizeof(this));
}

void Battle::playActionOver(cocostudio::Armature *armature, cocostudio::MovementEventType type, const char *name) {
    if (armature == m_pReadygo) {
        if (type == COMPLETE) {
            removeChild(armature);
            m_pReadygo = NULL;
            unmask();
            BIND(BUND_ID_GLOBAL_TOUCH_MOVED, this, Battle::globalTouchMoved);
            g_pEventEngine->BundlerCall(BUND_ID_PLANE_FIRE, this, sizeof(this));
        }
    }
    
    if (armature == m_pBossIn) {
        if (type == COMPLETE) {
            removeChild(armature);
            m_pBossIn = NULL;
            unmask();
            const BossConfig * pconfig = g_pGameConfig->getBossConfig(m_pConfig->boss);
            m_pBoss = Boss::create(pconfig);
            addChild(m_pBoss, GRADE_ENEMY);
            if (m_pConfig->boss != "chisezhilang") {
                m_pBoss->setScale(g_pGameConfig->scaleEleMin);
            } else {
                m_pBoss->setScale(g_pGameConfig->scaleEleMin * 1.5);
            }
            m_pBoss->setPosition(Vec2(pconfig->pos.x * g_pGameConfig->scaleEleX, pconfig->pos.y * g_pGameConfig->scaleEleY));
            m_pBoss->setRotation(pconfig->rotate);
            ActionInterval * action = MoveBy::create(2, Vec2(0, -500 * g_pGameConfig->scaleEleY));
            m_pBoss->runAction(Sequence::create(action, CallFuncN::create(this, callfuncN_selector(Battle::playbossAi)), NULL));
        }
    }
    
    if (armature == m_pMissionVictory) {
        if (type == COMPLETE) {
            Node * icon = g_pGameConfig->getIcon(m_equip.model, m_equip.type);
            icon->setPosition(Vec2(-200, -275));
            icon->setScale(g_pGameConfig->scaleEleMin * 5);
            m_pMissionVictory->addChild(icon, GRADE_UI);
            icon->setOpacity(0);
            ActionInterval * scale = ScaleTo::create(.5f, g_pGameConfig->scaleEleMin * 1.5);
            ActionInterval * fadein = FadeIn::create(.5f);
            icon->runAction(Spawn::create(scale, fadein, NULL));
            
            UNBIND(BUND_ID_GLOBAL_TOUCH_MOVED, this, Battle::globalTouchMoved);
            
            scheduleOnce(schedule_selector(Battle::returnMission), 2.0f);
        }
    }
    
    if (armature == m_pMissionFaild) {
        if (type == COMPLETE) {
            UNBIND(BUND_ID_GLOBAL_TOUCH_MOVED, this, Battle::globalTouchMoved);
            
            scheduleOnce(schedule_selector(Battle::returnMission), 2.0f);
        }
    }
}

void Battle::OnHttpRes(const s32 id, const Json::Value * root, const void  * context, const s32 size) {
    switch (id) {
        case HTTP_REQUEST_ID_COUNT_PVE:
        {
            if (NULL == root) {
                ALERT("请检查你的网络连接");
                return;
            }
            
            if ((*root)["code"] == ERROR_CODE::ERROR_CODE_NO_ERROR) {
                m_equip.id = StringAsInt((*root)["data"]["equipid"].asString().c_str());
                m_equip.isequiped = StringAsInt((*root)["data"]["status"].asString().c_str());
                m_equip.type = (*root)["data"]["type"].asInt();
                m_equip.model = (*root)["data"]["modelname"].asString();
                m_equip.exp = (*root)["data"]["exp"].asInt();
                m_equip.level = (*root)["data"]["level"].asInt();
                g_pEquipManager->saveEquip(m_equip);
                
                scheduleOnce(schedule_selector(Battle::pve_vector), 2.0f);
            } else {
                ALERT("小火鸡,不要作弊,小心被封号");
            }
            break;
        }
        default:
            break;
    }
}

void Battle::returnMission(float) {
    Director::getInstance()->replaceScene(MissionScene::createScene());
}

void Battle::pve_failed(float) {
    mask();
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Animation/UI/missionFailed.ExportJson");
    m_pMissionFaild = Armature::create("missionFailed");
    m_pMissionFaild->setScale(g_pGameConfig->scaleEleMin);
    m_pMissionFaild->setAnchorPoint(Vec2(.5f, .5f));
    addChild(m_pMissionFaild, GRADE_MAX);
    m_pMissionFaild->setPosition(g_pGameConfig->screenCenterPoint);
    m_pMissionFaild->getAnimation()->playWithIndex(0);
    m_pMissionFaild->setScale(g_pGameConfig->scaleEleMin * 1.5f);
    m_pMissionFaild->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Battle::playActionOver));
}

void Battle::pve_vector(float) {
    mask();
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Animation/UI/missionVictory.ExportJson");
    m_pMissionVictory = Armature::create("missionVictory");
    m_pMissionVictory->setScale(g_pGameConfig->scaleEleMin);
    m_pMissionVictory->setAnchorPoint(Vec2(.5f, .5f));
    addChild(m_pMissionVictory, GRADE_MAX);
    m_pMissionVictory->setPosition(g_pGameConfig->screenCenterPoint);
    m_pMissionVictory->getAnimation()->playWithIndex(0);
    m_pMissionVictory->setScale(g_pGameConfig->scaleEleMin * 1.5f);
    m_pMissionVictory->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(Battle::playActionOver));
}

void Battle::bulletFired(void *context, const s32 size) {
    addChild((Node*)context, GRADE_BULLET);
    Bullet * bullet = (Bullet *)context;
    CCASSERT(bullet->type() < BULLET_TYPE_COUNT, "hi bullet, where are u from?");
    m_Bullets[bullet->type()].push_back(bullet);
}

void Battle::bulletDisappear(void *context, const s32 size) {
    Bullet * bullet = (Bullet *)context;
    CCASSERT(bullet->type() < BULLET_TYPE_COUNT, "hi bullet, where are u from?");
    BULLET_GROUP::iterator iend = m_Bullets[bullet->type()].end();
    BULLET_GROUP::iterator itor = ::find(m_Bullets[bullet->type()].begin(), iend, bullet);
    if (itor != iend) {
        m_Bullets[bullet->type()].erase(itor);
    } else {
        CCASSERT(false, "where aru u from");
    }
}

void Battle::collidtion(float) {
    if (m_pPlane == NULL) {
        return;
    }
    
    s64 ltick = ::GetCurrentTimeTick();
    {
        //玩家子弹与敌机碰撞检测
        BULLET_GROUP::iterator bullet_itor = m_Bullets[BULLET_TYPE_PLAYER].begin();
        BULLET_GROUP::iterator bullet_iend = m_Bullets[BULLET_TYPE_PLAYER].end();
        while (bullet_itor != bullet_iend) {
            BULLET_GROUP::iterator bullet_current = bullet_itor++;
            ENEMY_GROUP::iterator itor = m_enemyGroup.begin();
            ENEMY_GROUP::iterator iend = m_enemyGroup.end();
            
            if (m_pBoss != NULL) {
                if ((*bullet_current)->isCollider(m_pBoss)) {
                    m_pBoss->beinjured((*bullet_current)->getAtk());
                    (*bullet_current)->disappear(0);
                    continue;
                }
            }
            
            while (itor != iend) {
                ENEMY_GROUP::iterator current = itor++;
                if ((*bullet_current)->isCollider(*current)) {
                    (*current)->beinjured((*bullet_current)->getAtk());
                    (*bullet_current)->disappear(0);
                    break;
                }
            }
        }
    }
    
    {
        //敌机子弹与玩家碰撞检测
        BULLET_GROUP::iterator bullet_itor = m_Bullets[BULLET_TYPE_ENEMY].begin();
        BULLET_GROUP::iterator bullet_iend = m_Bullets[BULLET_TYPE_ENEMY].end();
        while (bullet_itor != bullet_iend) {
            if ((*bullet_itor)->isCollider(m_pPlane)) {
                m_pPlane->beInjured((*bullet_itor)->getAtk());
                (*bullet_itor)->disappear(0);
                ltick = ::GetCurrentTimeTick() - ltick;
                if (ltick > 15) {
                    ECHO("collidtion call tick:%ld", ltick);
                }
                return;
            }
            bullet_itor++;
        }
    }
    
    {
        //敌机与玩家碰撞检测
        ENEMY_GROUP::iterator itor = m_enemyGroup.begin();
        ENEMY_GROUP::iterator iend = m_enemyGroup.end();
        while (itor != iend) {
            if ((*itor)->isCollider(m_pPlane)) {
                m_pPlane->disappear();
                (*itor)->disappear(0);
                ltick = ::GetCurrentTimeTick() - ltick;
                if (ltick > 15) {
                    ECHO("collidtion call tick:%ld", ltick);
                }
                return;
            }
            itor++;
        }
    }
    
    if (m_pBoss != NULL && m_pPlane != NULL) {
        if (m_pPlane->isCollider(m_pBoss)) {
            m_pPlane->disappear();
            ltick = ::GetCurrentTimeTick() - ltick;
            if (ltick > 15) {
                ECHO("collidtion call tick:%ld", ltick);
            }
            return;
        }
    }
    ltick = ::GetCurrentTimeTick() - ltick;
    if (ltick > 15) {
        ECHO("collidtion call tick:%ld", ltick);
    }
}

static Point s_point;
void Battle::globalTouchBegan(void * context, const s32 size) {
    s_point = *(Point *)context;
}

void Battle::globalTouchMoved(void *context, const s32 size) {
    CCASSERT(size == sizeof(Point), "check global touch event");
    const Point * point = (Point *)context;

    Point move = ((*point) - s_point);
    Point planePoint = m_pPlane->getPosition();
    
    if (planePoint.x + move.x > g_pGameConfig->screenSize.width) {
        move.x = g_pGameConfig->screenSize.width - planePoint.x;
    } else if (planePoint.x + move.x < 0) {
        move.x = 0 - planePoint.x;
    }
    
    if (planePoint.y + move.y > g_pGameConfig->screenSize.height) {
        move.y = g_pGameConfig->screenSize.height - planePoint.y;
    } else if (planePoint.y + move.y < 0) {
        move.y = 0 - planePoint.y;
    }
    
    m_pPlane->setPosition(planePoint + move);
    s_point = *(Point *)context;
}

void Battle::dataLoaded(float percent) {
    
}

