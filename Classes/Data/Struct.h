#ifndef _STRUCT_H_
#define _STRUCT_H_

#include "MultiSys.h"
#include "cocos2d.h"
#include "CHashMap.h"
#include "TArray.h"
#include <list>
#include <vector>
#include <string>
USING_NS_CC;

enum {
    EQUIP_TYPE_PLANE = 0,
    EQUIP_TYPE_ARMOR = 1,
    EQUIP_TYPE_WEAPON = 2,
    EQUIP_TYPE_WINGMAN = 3,
    
    //equip type befroe this can be equiped
    EQUIP_TYPE_CAN_EQUIPED = 4,
    EQUIP_TYPE_EXPSTONE = EQUIP_TYPE_CAN_EQUIPED,
    
    EQUIP_TYPE_CAN_BE_MATERIAL = 5,
    EQUIP_TYPE_ORE = EQUIP_TYPE_CAN_BE_MATERIAL,
    
    //ADD BEFOR THIS
    EQUIP_TYPE_ALL,
    EQUIP_TYPE_COUNT = EQUIP_TYPE_ALL
};

enum {
    BULLET_LEVEL_1,
    BULLET_LEVEL_2,
    BULLET_LEVEL_3,
    BULLET_LEVEL_GO,
    
    //ADD BEFOR THIS
    BULLET_LEVEL_COUNT
};

enum {
    BULLET_TYPE_PLAYER,
    BULLET_TYPE_ENEMY,
    
    BULLET_TYPE_COUNT
};

enum {
    IS_NOT_EQUIPED = 0,
    IS_EQUIPED
};

enum {
    UNLOCKED = 0,
    LOCKED,
};

typedef enum {
    ACTION_MOVETO = 0, 
    ACTION_MOVEBY = 1, 
    ACTION_BEZIERTO = 2, 
    ACTION_BEZIERBY = 3, 
    ACTION_ROTATETO = 4, 
    ACTION_ROTATEBY = 5, 
    ACTION_FADEIN = 6, 
    ACTION_FADEOUT = 7, 

    //ADD BEFORE THIS
    ACTION_TYPE_COUNT
}eActionType;

// == == == == == == == == db struct == == == == == == == == === //
struct SEquip {
    s8 type;
    s8 islocked;
    s8 isequiped;
    s32 id;
    s32 level;
    s32 exp;
    bool isselected;
    std::string model;
    
    SEquip & operator = (const SEquip & equip) {
        id = equip.id;
        type = equip.type;
        model = equip.model;
        level = equip.level;
        exp = equip.exp;
        islocked = equip.islocked;
        isequiped = equip.isequiped;
        
        return *this;
    }
    
    SEquip() {
        memset(this, 0, sizeof( * this));
    }
};
typedef vector<s32> ID_VECTOR;
typedef tlib::TArray<s32, 256> ID_ARRAY;
typedef list<SEquip *> P_EQUIP_LIST;
typedef list<SEquip> EQUIP_LIST;
typedef vector<SEquip> EQUIP_VECTOR;
typedef CHashMap<s32, SEquip> EQUIP_MAP;

// == == == == == == == == config struct == == == == == == == == == //
struct ActionConfig {
    s32 type;
    float time;
    float rotate;
    bool updaterotate;
    bool relative;
    cocos2d::Point point1;
    cocos2d::Point point2;
    cocos2d::Point point3;
};
typedef std::list<const ActionConfig> ACTION_CONFIG_LIST;

struct RadiusConfig {
    cocos2d::Point tlPoint;
    cocos2d::Point brPoint;
};

struct BulletConfig {
    float rotate;
    float delay;
    float cycle;
    float interval;
    s32 repeat;
    std::string res;
    cocos2d::Point offset;
    RadiusConfig radius;
    ACTION_CONFIG_LIST actionList;
};

typedef s32 LEVEL;
typedef std::list<BulletConfig> BULLET_LIST;
typedef CHashMap<LEVEL, BULLET_LIST> BULLET_MAP;

struct PlaneConfig {
    s32 atk;
    std::string name;
    std::string icon;
    std::string res;
    std::string boom;
    cocos2d::Point wingman_pos;
    cocos2d::Point weapon_pos;
    RadiusConfig radius;

    BULLET_MAP bulletMap;
};

struct ArmorConfig {
    s32 armor;
    std::string name;
    std::string icon;
};

struct WeaponConfig {
    s32 atk;
    std::string name;
    std::string icon;
    BULLET_MAP bulletMap;
};

enum {
    WINGMAN_LEFT,
    WINGMAN_RIGHT,
    
    WINGMAN_COUNT
};

struct WingmanConfig {
    s32 atk;
    std::string name;
    std::string icon;
    std::string res;
    BULLET_MAP bulletMap[WINGMAN_COUNT];
};

/*========mission config=========*/
struct EnemyConfig {
    s32 atk;
    s32 armor;
    s32 repeat;
    float rotate;
    float delay;
    float interval;
    float cycle;
    string boom;
    std::string res;
    RadiusConfig radius;
    cocos2d::Point pos;
    ACTION_CONFIG_LIST actionList;
    BULLET_LIST bullets;
};
typedef std::list<EnemyConfig> ENEMY_EVENT;

struct EventConfig {
    float delay;
    ENEMY_EVENT enemy_events;
};
typedef std::list<EventConfig> MISSION_EVENT;

struct BossEvent {
    float eventCycle;
    string action;
    ACTION_CONFIG_LIST actionList;
    BULLET_LIST bullets;
};
typedef std::list<BossEvent> BOSS_EVENT;

struct BossConfig {
    s32 atk;
    s32 armor;
    float rotate;
    string res;
    string name;
    string boom;
    RadiusConfig radius;
    cocos2d::Point pos;
    BOSS_EVENT boss_events;
};

struct MissionConfig {
    MISSION_EVENT events;
    string boss;
};
/*=======mission config over======*/

typedef string NAME;
typedef CHashMap<NAME, PlaneConfig> PLANE_CONFIG_MAP;
typedef CHashMap<NAME, ArmorConfig> ARMOR_CONFIG_MAP;
typedef CHashMap<NAME, WeaponConfig> WEAPON_CONFIG_MAP;
typedef CHashMap<NAME, WingmanConfig> WINGMAN_CONFIG_MAP;
typedef CHashMap<NAME, BossConfig> BOSS_CONFIG_MAP;
typedef CHashMap<NAME, MissionConfig> MISSION_CONFIG_MAP;
#endif //_STRUCT_H_
