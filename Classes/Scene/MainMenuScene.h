#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__
#include "cocos2d.h"
#include <list>
#include "PublicUI.h"
#include "IHttpRes.h"
class CButton;
class Plane;

class MainMenu : public PublicUI {
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene *  createScene();
    
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref *  pSender);
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);
    
    
    
    virtual void onEnter();
    virtual void onExit();
    
    void dataLoaded(float t);
    
    ~MainMenu();
    
protected:
    MainMenu();
    
    void initButtonUI();
    void initUserInfoUI();
    void initOtherUI();
    
    void buttonMissionCallBack(Ref * sender);
    void buttonLadderCallBack(Ref * sender);
    void buttonDepotCallBack(Ref * sender);
    void buttonFriendCallBack(Ref * sender);
    
    //设置玩家等级
    void setLevel(const s32 level);
    //设置金钱数
    void setMoney(const s32 coin);
    //设置钻石数
    void setDiamond(const s32 diamond);
    //设置体力
    void setPower(const s32 nowPower, const s32 maxPower);
    //设置贵族等级
    void setVipGrade(const s32 grade);
    //设置玩家名
    void setUserName(const char * name);
    //设置玩家头像
    void setUserImage(Node * node);
    //展示玩家战机
    void showPlane(Plane * plane);
    
    void bulletFired(void * context, const s32 size);
    
private:
    static MainMenu * s_pSelf;
    cocos2d::Sprite * m_pBackground;
    cocos2d::Sprite * m_SpriteDialBk;
    cocos2d::Sprite * m_SpriteRoleHead;
    
    CButton * m_BtnMission;
    CButton * m_BtnLadder;
    CButton * m_BtnDepot;
    CButton * m_BtnFriend;
    CButton * m_BtnVipCharge;
    CButton * m_BtnCharge;
    
    Label * m_LabelLevel;
    Label * m_LabelCoin;
    Label * m_LabelDiamond;
    ProgressTimer *  m_PowerBar;
    Label * m_LabelVipGrade;
    Label * m_LabelUserName;
    Label * m_LabelShowPower;
    Sprite *  m_SpriteUserImage;
    
    Plane * m_pPlane;
};

#endif // __MAINMENU_SCENE_H__
