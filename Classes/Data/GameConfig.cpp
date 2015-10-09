#include "GameConfig.h"
#include "tinyxml/tinyxml.h"
#include "Tools.h"
#include "Header.h"

GameConfig * g_pGameConfig = GameConfig::getInterface();

GameConfig * GameConfig::s_pSelf = NULL;

GameConfig * GameConfig::getInterface() {
    if (NULL ==  s_pSelf) {
        s_pSelf = NEW GameConfig;
    }

    return s_pSelf;
}

GameConfig::GameConfig() {

}

GameConfig::~GameConfig() {
    s_pSelf = NULL;
}

Sprite * GameConfig::getIcon(const string &model, const s8 type) {
    char szPath[128] = {0};
    switch (type) {
        case EQUIP_TYPE_PLANE:
        {
            const PlaneConfig * pConfig = g_pGameConfig->getPlaneConfig(model);
            SafeSprintf(szPath, sizeof(szPath), "Icons/%s", pConfig->icon.c_str());
            break;
        }
        case EQUIP_TYPE_ARMOR:
        {
            const ArmorConfig * pConfig = g_pGameConfig->getArmorConfig(model);
            SafeSprintf(szPath, sizeof(szPath), "Icons/%s", pConfig->icon.c_str());
            break;
        }
        case EQUIP_TYPE_WEAPON:
        {
            const WeaponConfig * pConfig = g_pGameConfig->getWeaponConfig(model);
            SafeSprintf(szPath, sizeof(szPath), "Icons/%s", pConfig->icon.c_str());
            break;
        }
        case EQUIP_TYPE_WINGMAN:
        {
            const WingmanConfig * pConfig = g_pGameConfig->getWingmanConfig(model);
            SafeSprintf(szPath, sizeof(szPath), "Icons/%s", pConfig->icon.c_str());
            break;
        }
        default:
            CCASSERT(false, "check ur equip type");
            break;
    }
    
    return Sprite::create(szPath);
}

//get player's plane config
const PlaneConfig * GameConfig::getPlaneConfig(const string & name) {
    PLANE_CONFIG_MAP::iterator itor = m_PlaneConfigMap.find(name);
    if (itor !=  m_PlaneConfigMap.end()) {
        return &(itor->second);
    }
    
    PlaneConfig config;
    TiXmlDocument xml;
    string path = "Config/Plane/";
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename((path + name + ".xml").c_str());
    CCASSERT(diyfun::loadXml(xml, fullPath), "load xml error");
    
    TiXmlElement * pPlaneConfig = xml.RootElement();
    CCASSERT(TIXML_SUCCESS ==  pPlaneConfig->QueryIntAttribute("atk", &config.atk)
             && (NULL !=  (config.name = pPlaneConfig->Attribute("name")).c_str())
             && (NULL !=  (config.res = pPlaneConfig->Attribute("res")).c_str())
             && (NULL !=  (config.icon = pPlaneConfig->Attribute("icon")).c_str())
             && TIXML_SUCCESS ==  pPlaneConfig->QueryFloatAttribute("weapon_x", &config.weapon_pos.x)
             && TIXML_SUCCESS ==  pPlaneConfig->QueryFloatAttribute("weapon_y", &config.weapon_pos.y)
             
             && TIXML_SUCCESS ==  pPlaneConfig->QueryFloatAttribute("wingman_x", &config.wingman_pos.x)
             && TIXML_SUCCESS ==  pPlaneConfig->QueryFloatAttribute("wingman_y", &config.wingman_pos.y)
             
             && TIXML_SUCCESS ==  pPlaneConfig->QueryFloatAttribute("tl_x", &config.radius.tlPoint.x)
             && TIXML_SUCCESS ==  pPlaneConfig->QueryFloatAttribute("tl_y", &config.radius.tlPoint.y)
             
             && TIXML_SUCCESS ==  pPlaneConfig->QueryFloatAttribute("br_x", &config.radius.brPoint.x)
             && TIXML_SUCCESS ==  pPlaneConfig->QueryFloatAttribute("br_y", &config.radius.brPoint.y),
             "get plane base config error");

    config.boom = "enemyboom";
    
    TiXmlElement * pBulletGrp = pPlaneConfig->FirstChildElement("bullet_group");
    while (NULL !=  pBulletGrp) {
        s32 level;
        CCASSERT(TIXML_SUCCESS ==  pBulletGrp->QueryIntAttribute("level", &level)
            , "get bulletgrp level failed");

        BULLET_LIST bullet_list;
        TiXmlElement * pBullet = pBulletGrp->FirstChildElement("bullet");
        while (NULL !=  pBullet) {
            BulletConfig bullet;
            CCASSERT(serializeBulletConfig(pBullet, bullet), "serialize bullet config error");
            bullet_list.push_back(bullet);
            pBullet = pBullet->NextSiblingElement("bullet");
        }
        config.bulletMap.insert(make_pair(level, bullet_list));
        pBulletGrp = pBulletGrp->NextSiblingElement("bullet_group");
    }

    m_PlaneConfigMap.insert(make_pair(name, config));

    return getPlaneConfig(name);
}


const ArmorConfig * GameConfig::getArmorConfig(const string & name) {
    ARMOR_CONFIG_MAP::iterator itor = m_ArmorConfigMap.find(name);
    if (itor !=  m_ArmorConfigMap.end()) {
        return &(itor->second);
    }
    
    ArmorConfig config;
    TiXmlDocument xml;
    string path = "Config/Armor/";
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename((path + name + ".xml").c_str());
    CCASSERT(diyfun::loadXml(xml, fullPath), "load xml error");

    TiXmlElement * pArmorConfig = xml.RootElement();
    CCASSERT(TIXML_SUCCESS ==  pArmorConfig->QueryIntAttribute("armor", &config.armor)
             && (NULL !=  (config.name = pArmorConfig->Attribute("name")).c_str())
             && (NULL !=  (config.icon = pArmorConfig->Attribute("icon")).c_str()),
             "get armor base config error");
    
    m_ArmorConfigMap.insert(make_pair(name, config));
    return getArmorConfig(name);
}

const WeaponConfig * GameConfig::getWeaponConfig(const string & name) {
    WEAPON_CONFIG_MAP::iterator itor = m_WeaponConfigMap.find(name);
    if (itor !=  m_WeaponConfigMap.end()) {
        return &(itor->second);
    }
    
    WeaponConfig config;
    TiXmlDocument xml;
    string path = "Config/Weapon/";
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename((path + name + ".xml").c_str());
    CCASSERT(diyfun::loadXml(xml, fullPath), "load xml error");
    
    TiXmlElement * pWeaponConfig = xml.RootElement();
    CCASSERT(TIXML_SUCCESS ==  pWeaponConfig->QueryIntAttribute("atk", &config.atk)
             && (NULL !=  (config.name = pWeaponConfig->Attribute("name")).c_str())
             && (NULL !=  (config.icon = pWeaponConfig->Attribute("icon")).c_str()),
             "get weapon base config error");
    
    TiXmlElement * pBulletGrp = pWeaponConfig->FirstChildElement("bullet_group");
    while (NULL !=  pBulletGrp) {
        s32 level;
        CCASSERT(TIXML_SUCCESS ==  pBulletGrp->QueryIntAttribute("level", &level)
                 , "get bulletgrp level failed");
        
        BULLET_LIST bullet_list;
        TiXmlElement * pBullet = pBulletGrp->FirstChildElement("bullet");
        while (NULL !=  pBullet) {
            BulletConfig bullet;
            CCASSERT(serializeBulletConfig(pBullet, bullet), "serialize bullet config error");
            bullet_list.push_back(bullet);
            pBullet = pBullet->NextSiblingElement("bullet");
        }
        config.bulletMap.insert(make_pair(level, bullet_list));
        pBulletGrp = pBulletGrp->NextSiblingElement("bullet_group");
    }
    
    m_WeaponConfigMap.insert(make_pair(name, config));
    return getWeaponConfig(name);
}

const WingmanConfig * GameConfig::getWingmanConfig(const string & name) {
    WINGMAN_CONFIG_MAP::iterator itor = m_WingmanConfigMap.find(name);
    if (itor !=  m_WingmanConfigMap.end()) {
        return &(itor->second);
    }
    
    WingmanConfig config;
    TiXmlDocument xml;
    string path = "Config/Wingman/";
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename((path + name + ".xml").c_str());
    CCASSERT(diyfun::loadXml(xml, fullPath), "load xml error");
    
    TiXmlElement * pWinmanConfig = xml.RootElement();
    CCASSERT(TIXML_SUCCESS ==  pWinmanConfig->QueryIntAttribute("atk", &config.atk)
             && (NULL !=  (config.name = pWinmanConfig->Attribute("name")).c_str())
             && (NULL !=  (config.res = pWinmanConfig->Attribute("res")).c_str())
             && (NULL !=  (config.icon = pWinmanConfig->Attribute("icon")).c_str()),
             "get weapon base config error");
    
    TiXmlElement * pLeft = pWinmanConfig->FirstChildElement("left");
    TiXmlElement * pLeftGrp = pLeft->FirstChildElement("bullet_group");
    while (NULL !=  pLeftGrp) {
        s32 level;
        CCASSERT(TIXML_SUCCESS ==  pLeftGrp->QueryIntAttribute("level", &level)
                 , "get bulletgrp level failed");
        
        BULLET_LIST bullet_list;
        TiXmlElement * pBullet = pLeftGrp->FirstChildElement("bullet");
        while (NULL !=  pBullet) {
            BulletConfig bullet;
            CCASSERT(serializeBulletConfig(pBullet, bullet), "serialize bullet config error");
            bullet_list.push_back(bullet);
            pBullet = pBullet->NextSiblingElement("bullet");
        }
        config.bulletMap[WINGMAN_LEFT].insert(make_pair(level, bullet_list));
        pLeftGrp = pLeftGrp->NextSiblingElement("bullet_group");
    }
    
    
    TiXmlElement * pRight = pWinmanConfig->FirstChildElement("right");
    TiXmlElement * pRighttGrp = pRight->FirstChildElement("bullet_group");
    while (NULL !=  pRighttGrp) {
        s32 level;
        CCASSERT(TIXML_SUCCESS ==  pRighttGrp->QueryIntAttribute("level", &level)
                 , "get bulletgrp level failed");
        
        BULLET_LIST bullet_list;
        TiXmlElement * pBullet = pRighttGrp->FirstChildElement("bullet");
        while (NULL !=  pBullet) {
            BulletConfig bullet;
            CCASSERT(serializeBulletConfig(pBullet, bullet), "serialize bullet config error");
            bullet_list.push_back(bullet);
            pBullet = pBullet->NextSiblingElement("bullet");
        }
        config.bulletMap[WINGMAN_RIGHT].insert(make_pair(level, bullet_list));
        pRighttGrp = pRighttGrp->NextSiblingElement("bullet_group");
    }
    
    m_WingmanConfigMap.insert(make_pair(name, config));
    return getWingmanConfig(name);
}

const MissionConfig * GameConfig::getMissionConfig(const string &name) {
    MISSION_CONFIG_MAP::iterator itor = m_MissionConfigMap.find(name);
    if (itor !=  m_MissionConfigMap.end()) {
        return &(itor->second);
    }
    
    MissionConfig config;
    TiXmlDocument xml;
    string path = "Config/Mission/";
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename((path + name + ".xml").c_str());
    CCASSERT(diyfun::loadXml(xml, fullPath), "load xml error");
    
    TiXmlElement * pMissionConfig = xml.RootElement();
    CCASSERT(pMissionConfig != NULL, "check ur mission config");
    TiXmlElement * pEventList = pMissionConfig->FirstChildElement("event_list");
    if (pEventList != NULL) {
        TiXmlElement * pEvent = pEventList->FirstChildElement("event");
        while (pEvent != NULL) {
            EventConfig event;
            CCASSERT(serializeEventConfig(pEvent, event), "check ur event config");
            config.events.push_back(event);
            pEvent = pEvent->NextSiblingElement("event");
        }
    }
    
    TiXmlElement * pBoss = pMissionConfig->FirstChildElement("boss");
    if (pBoss != NULL) {
        CCASSERT(NULL !=  (config.boss = pBoss->Attribute("name")).c_str(), "chuck ur boss config");
    }
    
    m_MissionConfigMap.insert(make_pair(name, config));
    return getMissionConfig(name);
}

const BossConfig * GameConfig::getBossConfig(const string &name) {
    BOSS_CONFIG_MAP::iterator itor = m_BossConfigMap.find(name);
    if (itor !=  m_BossConfigMap.end()) {
        return &(itor->second);
    }
    
    BossConfig config;
    TiXmlDocument xml;
    string path = "Config/Boss/";
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename((path + name + ".xml").c_str());
    CCASSERT(diyfun::loadXml(xml, fullPath), "load xml error");
    
    TiXmlElement * pBoss = xml.RootElement();
    CCASSERT(pBoss != NULL, "check ur mission config");
    
    CCASSERT((NULL !=  (config.res = pBoss->Attribute("res")).c_str())
             && (NULL !=  (config.name = pBoss->Attribute("name")).c_str())
             && TIXML_SUCCESS ==  pBoss->QueryFloatAttribute("tl_x", &config.radius.tlPoint.x)
             && TIXML_SUCCESS ==  pBoss->QueryFloatAttribute("tl_y", &config.radius.tlPoint.y)
             && TIXML_SUCCESS ==  pBoss->QueryFloatAttribute("br_x", &config.radius.brPoint.x)
             && TIXML_SUCCESS ==  pBoss->QueryFloatAttribute("br_y", &config.radius.brPoint.y)
             && TIXML_SUCCESS ==  pBoss->QueryFloatAttribute("pos_x", &config.pos.x)
             && TIXML_SUCCESS ==  pBoss->QueryFloatAttribute("pos_y", &config.pos.y)
             && TIXML_SUCCESS ==  pBoss->QueryIntAttribute("atk", &config.atk)
             && TIXML_SUCCESS ==  pBoss->QueryIntAttribute("armor", &config.armor)
             && TIXML_SUCCESS ==  pBoss->QueryFloatAttribute("rotate", &config.rotate),
             "check ur boss config");
    
    config.boom = "bossboom1";
    
    TiXmlElement * pEvent = pBoss->FirstChildElement("event");
    while (pEvent != NULL) {
        BossEvent event;
        CCASSERT(TIXML_SUCCESS ==  pEvent->QueryFloatAttribute("cycle", &event.eventCycle),
                 "check ur boss event base info");
        const char * value = pEvent->Attribute("action");
        if (value != NULL) {
            event.action = value;
        } else {
            event.action = "";
        }
        
        TiXmlElement * pAction = pEvent->FirstChildElement("action");
        while (pAction != NULL) {
            ActionConfig action;
            CCASSERT(serializeActionConfig(pAction, action), "check ur boss action");
            event.actionList.push_back(action);
            pAction = pAction->NextSiblingElement("action");
        }
        
        TiXmlElement * pBullet = pEvent->FirstChildElement("bullet");
        while (pBullet != NULL) {
            BulletConfig bullet;
            CCASSERT(serializeBulletConfig(pBullet, bullet), "check ur boss bullet");
            event.bullets.push_back(bullet);
            pBullet = pBullet->NextSiblingElement("bullet");
        }
        config.boss_events.push_back(event);
        pEvent = pEvent->NextSiblingElement("event");
    }
    
    m_BossConfigMap.insert(make_pair(name, config));
    return getBossConfig(name);
    
}

bool GameConfig::serializeEventConfig(const TiXmlElement * xmlEle, EventConfig &event) {
    CCASSERT(TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("delay", &event.delay),
             "serialize event base config error");
    const TiXmlElement * pEnemy = xmlEle->FirstChildElement("enemy");
    while (pEnemy != NULL) {
        EnemyConfig enemy;
        CCASSERT(serializeEnemyConfig(pEnemy, enemy), "serialize enemy config error");
        event.enemy_events.push_back(enemy);
        pEnemy = pEnemy->NextSiblingElement("enemy");
    }
    
    return true;
}

bool GameConfig::serializeEnemyConfig(const TiXmlElement *xmlEle, EnemyConfig &enemy) {
    CCASSERT((NULL !=  (enemy.res = xmlEle->Attribute("res")).c_str())
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("tl_x", &enemy.radius.tlPoint.x)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("tl_y", &enemy.radius.tlPoint.y)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("br_x", &enemy.radius.brPoint.x)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("br_y", &enemy.radius.brPoint.y)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("pos_x", &enemy.pos.x)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("pos_y", &enemy.pos.y)
             && TIXML_SUCCESS ==  xmlEle->QueryIntAttribute("atk", &enemy.atk)
             && TIXML_SUCCESS ==  xmlEle->QueryIntAttribute("armor", &enemy.armor)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("rotate", &enemy.rotate)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("interval", &enemy.interval)
             && TIXML_SUCCESS ==  xmlEle->QueryIntAttribute("repeat", &enemy.repeat)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("delay", &enemy.delay)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("cycle", &enemy.cycle),
             "serialize enemy config base info error");
    
    enemy.repeat -= 1;
    enemy.boom = "enemyboom";
    
    const TiXmlElement * pAction = xmlEle->FirstChildElement("action");
    while (pAction != NULL) {
        ActionConfig action;
        CCASSERT(serializeActionConfig(pAction, action), "serialize action error");
        enemy.actionList.push_back(action);
        pAction = pAction->NextSiblingElement("action");
    }
    
    const TiXmlElement * pBulletGroup = xmlEle->FirstChildElement("bullet_group");
    if (pBulletGroup != NULL) {
        const TiXmlElement * pBullet = pBulletGroup->FirstChildElement("bullet");
        while (pBullet != NULL) {
            BulletConfig bullet;
            CCASSERT(serializeBulletConfig(pBullet, bullet), "serialize bullet config error");
            enemy.bullets.push_back(bullet);
            pBullet = pBullet->NextSiblingElement("bullet");
        }
    }
    
    return true;
}

ActionInterval * GameConfig::getActionByConfig(const ActionConfig * pConfig, const Point & point,
                                               const float scalex, const float scaley) {
    CCASSERT(NULL !=  pConfig, "check ur point, it's null, fuck u");
    srand(::GetCurrentTimeTick());
    ActionInterval * pAction = NULL;

    switch (pConfig->type)
    {
    case ACTION_MOVETO:
    case ACTION_MOVEBY:
        {
            float x, y;
            (pConfig->point1.x ==  RANDOM_POSITION)?(x = CCRANDOM_0_1() * (screenSize.width - 200) + 100):(x = pConfig->point1.x);
            (pConfig->point1.y ==  RANDOM_POSITION)?(y = CCRANDOM_0_1() * (screenSize.height - 200) + 100):(y = pConfig->point1.y);
            if (ACTION_MOVETO ==  pConfig->type) {
                pAction = MoveTo::create(pConfig->time, Vec2(point.x + x * scalex, point.y + y * scaley));
            } else if (ACTION_MOVEBY ==  pConfig->type) {
                pAction = MoveBy::create(pConfig->time, Vec2(point.x + x * scalex, point.y + y * scaley));
            } else {
                ASSERT(false);
            }
        }
        break;
    case ACTION_BEZIERTO:
    case ACTION_BEZIERBY:
        {
            float x1, y1, x2, y2, x3, y3;
            (pConfig->point1.x ==  RANDOM_POSITION)?(x1 = CCRANDOM_0_1() * (screenSize.width - 200) + 100):(x1 = pConfig->point1.x);
            (pConfig->point1.y ==  RANDOM_POSITION)?(y1 = CCRANDOM_0_1() * (screenSize.height - 200) + 100):(y1 = pConfig->point1.y);

            (pConfig->point2.x ==  RANDOM_POSITION)?(x2 = CCRANDOM_0_1() * (screenSize.width - 200) + 100):(x2 = pConfig->point2.x);
            (pConfig->point2.y ==  RANDOM_POSITION)?(y2 = CCRANDOM_0_1() * (screenSize.height - 200) + 100):(y2 = pConfig->point2.y);

            (pConfig->point3.x ==  RANDOM_POSITION)?(x3 = CCRANDOM_0_1() * (screenSize.width - 200) + 100):(x3 = pConfig->point3.x);
            (pConfig->point3.y ==  RANDOM_POSITION)?(y3 = CCRANDOM_0_1() * (screenSize.height - 200) + 100):(y3 = pConfig->point3.y);

            ccBezierConfig bezierConfig;
            bezierConfig.controlPoint_1 = Vec2(point.x + x1 * scalex, point.y + y1 * scaley);
            bezierConfig.controlPoint_2 = Vec2(point.x + x2 * scalex, point.y + y2 * scaley);
            bezierConfig.endPosition = Vec2(point.x + x3 * scalex, point.y + y3 * scaley);
            if (ACTION_BEZIERTO ==  pConfig->type) {
                pAction = BezierTo::create(pConfig->time, bezierConfig);
            } else if (ACTION_BEZIERBY ==  pConfig->type) {
                pAction = BezierBy::create(pConfig->time, bezierConfig);
            } else {
                ASSERT(false);
            }
        }
        break;
    case ACTION_ROTATETO:
        {
            pAction = RotateTo::create(pConfig->time, pConfig->rotate);
        }
        break;
    case ACTION_ROTATEBY:
        {
            pAction = RotateBy::create(pConfig->time, pConfig->rotate);
        }
        break;
    default:
        ASSERT(false);
        break;
    }
    
    return pAction;
}

bool GameConfig::serializeActionConfig(const TiXmlElement * xmlEle, ActionConfig & actionCfg) {
    CCASSERT(TIXML_SUCCESS ==  xmlEle->QueryIntAttribute("type", &actionCfg.type)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("time", &actionCfg.time)
             && TIXML_SUCCESS ==  xmlEle->QueryBoolAttribute("updaterotate", &actionCfg.updaterotate)
             && TIXML_SUCCESS ==  xmlEle->QueryBoolAttribute("relative", &actionCfg.relative),
             "load Action config error");
    
    switch (actionCfg.type) {
        case ACTION_MOVETO:
        case ACTION_MOVEBY:
        {
            CCASSERT(TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("x", &actionCfg.point1.x)
                     && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("y", &actionCfg.point1.y),
                     "load Action config error");
            break;
        }
        case ACTION_BEZIERTO:
        case ACTION_BEZIERBY:
        {
            CCASSERT(TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("x1", &actionCfg.point1.x)
                     && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("y1", &actionCfg.point1.y)
                     && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("x2", &actionCfg.point2.x)
                     && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("y2", &actionCfg.point2.y)
                     && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("x3", &actionCfg.point3.x)
                     && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("y3", &actionCfg.point3.y),
                     "load Action config error");
            break;
        }
        case ACTION_ROTATETO:
        case ACTION_ROTATEBY:
        {
            CCASSERT(TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("rotate", &actionCfg.rotate),
                     "load Action config error");
            break;
        }
        default:
            CCASSERT(false, "unknown type of action");
            break;
    }
    
    return true;
}

bool GameConfig::serializeBulletConfig(const TiXmlElement * xmlEle, BulletConfig & bullet) {
    CCASSERT((NULL !=  (bullet.res = xmlEle->Attribute("res")).c_str())
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("interval", &bullet.interval)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("cycle", &bullet.cycle)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("delay", &bullet.delay)
             && TIXML_SUCCESS ==  xmlEle->QueryIntAttribute("repeat", &bullet.repeat)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("offset_x", &bullet.offset.x)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("offset_y", &bullet.offset.y)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("tl_x", &bullet.radius.tlPoint.x)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("tl_y", &bullet.radius.tlPoint.y)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("br_x", &bullet.radius.brPoint.x)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("br_y", &bullet.radius.brPoint.y)
             && TIXML_SUCCESS ==  xmlEle->QueryFloatAttribute("rotate", &bullet.rotate),
             "get bullet config falied");
    bullet.repeat -= 1;
    
    const TiXmlElement * pAction = xmlEle->FirstChildElement("action");
    while (NULL !=  pAction) {
        ActionConfig actionCfg;
        
        CCASSERT(serializeActionConfig(pAction, actionCfg), "serialize action error");
        
        bullet.actionList.push_back(actionCfg);
        pAction = pAction->NextSiblingElement("action");
    }
    
    return true;
}
