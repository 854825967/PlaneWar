#ifndef __LADDER_SCENE_H__
#define __LADDER_SCENE_H__
#include "cocos2d.h"
#include <list>
#include "UI/PublicUI.h"
class LadderScene : public PublicUI {
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(LadderScene);
    
private:
    LadderScene();
    ~LadderScene();
    
    void menuReturnMainMenu(cocos2d::Ref * pSender);
    void initButtonUI();
    void initOtherUI();
    void initUserInfoUI();
    void initRivalInfoUI();
    
private:
    static LadderScene * s_pSelf;
    cocos2d::Sprite * m_pBackground;
    Label * m_LabelTitle;
};

#endif // __LADDER_SCENE_H__