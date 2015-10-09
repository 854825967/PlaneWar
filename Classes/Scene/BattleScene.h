//
//  BattleScene.h
//  PlaneWars
//
//  Created by Alax on 11/23/14.
//
//

#ifndef __PlaneWars__BattleScene__
#define __PlaneWars__BattleScene__

#include "Header.h"
#include "PublicUI.h"
#include "IHttpRes.h"
#include "Struct.h"
class Boss;
class Plane;
class Enemy;
class Bullet;
class MissionConfig;
typedef std::list<Bullet *> BULLET_GROUP;
typedef std::list<Enemy *> ENEMY_GROUP;

class Battle : public PublicUI, public IHttpRes {
public:
    static void setMissionConfig(const string & name);
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene *  createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
    // implement the "static create()" method manually
    CREATE_FUNC(Battle);
    
    void dataLoaded(float t);
    
protected:
    void globalTouchBegan(void * context, const s32 size);
    void globalTouchMoved(void * context, const s32 size);
    
    void collidtion(float);
    void OnHttpRes(const s32 id, const Json::Value * root, const void * context, const s32 size);
    void playActionOver(Armature * armature, MovementEventType type, const char * name);
    void playbossAi(Node * pSender);
    void event_exec(void * pContext, s32 id);
    void enemy_event(void * pContext, s32 id);
    void bulletFired(void * context, const s32 size);
    
    void bulletDisappear(void * context, const s32 size);
    void enemydisappear(void * context, const s32 size);
    void planedisappear(void * context, const s32 size);
    void planeinjured(void * context, const s32 size);
    void bossdisappear(void * context, const s32 size);
    
    void requestpve();
    
    void pve_vector(float);
    void pve_failed(float);
    void returnMission(float);
    
    Battle();
    ~Battle();
    
private:
    static Battle * s_pSelf;
    cocos2d::Sprite * m_pBackground;
    const MissionConfig * m_pConfig;
    Plane * m_pPlane;
    Armature * m_pReadygo;
    Armature * m_pBossIn;
    Armature * m_pMissionVictory;
    Armature * m_pMissionFaild;
    BULLET_GROUP m_Bullets[BULLET_TYPE_COUNT];
    ENEMY_GROUP m_enemyGroup;
    Boss * m_pBoss;
    SEquip m_equip;
};

#endif /* defined(__PlaneWars__BattleScene__) */
