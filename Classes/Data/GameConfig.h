#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include "Struct.h"

class GameConfig;
class TiXmlElement;

extern GameConfig * g_pGameConfig;

class GameConfig {
public:
    static GameConfig * getInterface();
    ~GameConfig();

    //get player's plane config
    const PlaneConfig * getPlaneConfig(const string & model);
    const ArmorConfig * getArmorConfig(const string & model);
    const WeaponConfig * getWeaponConfig(const string & model);
    const WingmanConfig * getWingmanConfig(const string & model);
    const MissionConfig * getMissionConfig(const string & name);
    const BossConfig * getBossConfig(const string & name);
    
    cocos2d::Sprite * getIcon(const string & model, const s8 type);
    
    ActionInterval * getActionByConfig(const ActionConfig * pConfig,
                                       const Point & point,
                                       const float scalex,
                                       const float scaley);

public:
    bool serializeBulletConfig(const TiXmlElement * xmlEle, BulletConfig & bullet);
    bool serializeActionConfig(const TiXmlElement * xmlEle, ActionConfig & action);
    bool serializeEnemyConfig(const TiXmlElement * xmlEle, EnemyConfig &enemy);
    bool serializeEventConfig(const TiXmlElement * xmlEle, EventConfig &event);
    
public:
    cocos2d::Point screenCenterPoint;
    cocos2d::Size screenSize;
    cocos2d::Size virtualSize;
    float scalex;
    float scaley;
    float scaleEleX;
    float scaleEleY;
    float scaleEleMin;
    float scaleEleMax;
    
private:
    GameConfig();
    static GameConfig * s_pSelf;

    PLANE_CONFIG_MAP m_PlaneConfigMap;
    ARMOR_CONFIG_MAP m_ArmorConfigMap;
    WEAPON_CONFIG_MAP m_WeaponConfigMap;
    WINGMAN_CONFIG_MAP m_WingmanConfigMap;
    BOSS_CONFIG_MAP m_BossConfigMap;
    MISSION_CONFIG_MAP m_MissionConfigMap;
};

#endif //GAMECONFIG_H
