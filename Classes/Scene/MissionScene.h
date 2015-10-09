#ifndef __MISSION_SCENE_H__
#define __MISSION_SCENE_H__
#include "cocos2d.h"
#include <list>
#include "UI/PublicUI.h"
class MissionScene : public PublicUI {
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MissionScene);
    
private:
    MissionScene();
    ~MissionScene();
    
    void menuReturnMainMenu(cocos2d::Ref * pSender);
    void missionBtn(cocos2d::Ref * pSender);
    
    void initOtherUI();
    void initMissionUI();
    
private:
    static MissionScene * s_pSelf;
    cocos2d::Sprite * m_pBackground;
    Label * m_LabelTitle;
};

#endif // __MISSION_SCENE_H__