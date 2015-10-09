#include "Header.h"
#include "GameConfig.h"
#include "CButton.h"
#include "MissionScene.h"
#include "MainMenuScene.h"
#include "BattleScene.h"
USING_NS_CC;


MissionScene * MissionScene::s_pSelf = NULL;

MissionScene::MissionScene() {
    ASSERT(s_pSelf == NULL);
    s_pSelf = this;
}

MissionScene::~MissionScene() {
    s_pSelf = NULL;
}

Scene* MissionScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MissionScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MissionScene::init() {
    //////////////////////////////
    // 1. super init first
    if ( !PublicUI::init() ) {
        return false;
    }

    initOtherUI();
    initMissionUI();
    return true;
}

void MissionScene::initOtherUI() {
    //背景
    m_pBackground = Sprite::create("UI/Mission/mission1Bk.png");
    m_pBackground->setScale(g_pGameConfig->scaleEleMax);
    m_pBackground->setPosition(Point(g_pGameConfig->screenCenterPoint.x, g_pGameConfig->screenCenterPoint.y));
    this->addChild(m_pBackground, GRADE_BACKGOUND);
    
    //关卡星座背景
    Sprite * starBk = Sprite::create("UI/Mission/xingzuo1.png");
    //    starBk->setPosition(ccp(768 * g_pGameConfig->scaleEleX, 1000 * g_pGameConfig->scaleEleX));
    starBk->setPosition(g_pGameConfig->screenCenterPoint);
    starBk->setScale(g_pGameConfig->scaleEleMin);
    this->addChild(starBk, GRADE_UI);
    
    //关闭按钮
    CButton * BtnBack = CButton::create("", Point::ZERO,
                                        "UI/Public/Button/backBtn.png", "UI/Public/Button/backPressBtn.png", "",
                                        CC_CALLBACK_1(MissionScene::menuReturnMainMenu, this));
    BtnBack->setPosition(Vec2(g_pGameConfig->screenSize.width - (BtnBack->getContentSize().width) * g_pGameConfig->scaleEleMin * 0.5f
                              - 10 * g_pGameConfig->scalex, g_pGameConfig->screenSize.height
                              - (BtnBack->getContentSize().height) * g_pGameConfig->scaleEleMin * 0.5f - 5 * g_pGameConfig->scalex));
    BtnBack->setScale(g_pGameConfig->scaleEleX);
    this->addChild(BtnBack, GRADE_UI);
    
    //路线
    Sprite * route = Sprite::create("UI/Mission/mission1route.png");
    route->setScale(g_pGameConfig->scaleEleMin);
    //    route->setPosition(Point(768 * g_pGameConfig->scaleEleX, 1155 * g_pGameConfig->scaleEleX));g_pGameConfig->screenCenterPoint
    route->setPosition(g_pGameConfig->screenCenterPoint);
    this->addChild(route, GRADE_UI);
    
    //顶部操作区背景
    auto topOpeBk = Sprite::create("UI/Public/Panel/topOpeBk.png");
    topOpeBk->setPosition(ccp(g_pGameConfig->screenCenterPoint.x, g_pGameConfig->screenSize.height - 85 * g_pGameConfig->scaleEleX));
    topOpeBk->setScale(g_pGameConfig->scaleEleMin);
    this->addChild(topOpeBk, GRADE_UI);
    
    //天梯页面标题
    m_LabelTitle = Label::create("闯关模式", "WRYH.ttf", 50);
    m_LabelTitle->setAnchorPoint(Point(0, 0.5f));
    m_LabelTitle->setScale(g_pGameConfig->scaleEleMin);
    m_LabelTitle->setPosition(Point(45 * g_pGameConfig->scaleEleMin, g_pGameConfig->screenSize.height - 60 * g_pGameConfig->scaleEleMin));
    this->addChild(m_LabelTitle, GRADE_UI);
    
    
    auto userInfoSprite = Sprite::create("UI/Ladder/userInfo.png");
    userInfoSprite->setPosition(ccp(g_pGameConfig->screenCenterPoint.x, g_pGameConfig->screenSize.height - 250*g_pGameConfig->scaleEleX));
    userInfoSprite->setScale(g_pGameConfig->scaleEleMin);
    this->addChild(userInfoSprite, GRADE_UI);
}

void MissionScene::initMissionUI() {
    // 关卡1背景图片
    CButton * mission1Bk = CButton::create("", Point::ZERO,
                                           "UI/Mission/missionBlue.png", "UI/Mission/missionBlue.png", "",
                                           CC_CALLBACK_1(MissionScene::missionBtn, this));
    mission1Bk->setTag(10001);
    mission1Bk->setPosition(ccp(g_pGameConfig->screenCenterPoint.x + 200 * g_pGameConfig->scaleEleMin, g_pGameConfig->screenCenterPoint.y - 575 * g_pGameConfig->scaleEleMin));
    mission1Bk->setScale(g_pGameConfig->scaleEleMin);
    this->addChild(mission1Bk, GRADE_UI);
    
    //关卡4
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Animation/UI/missionCircleRed.ExportJson");
    Armature * test = Armature::create("missionCircleRed");
    test->setPosition(ccp(g_pGameConfig->screenCenterPoint.x + 200 * g_pGameConfig->scaleEleX, g_pGameConfig->screenCenterPoint.y - 260 * g_pGameConfig->scaleEleMin));
    test->setScale(g_pGameConfig->scaleEleMin);
    test->getAnimation()->play("fly");
    this->addChild(test, GRADE_UI);
}

void MissionScene::missionBtn(cocos2d::Ref *pSender) {
    switch (((Node*)pSender)->getTag()) {
        case 10001:
            Battle::setMissionConfig("mission_1_normal");
            break;
        default:
            CCASSERT(false, "check ur mission");
            break;
    }
    Director::getInstance()->replaceScene(Battle::createScene());
}

void MissionScene::menuReturnMainMenu(cocos2d::Ref *pSender) {
    Director::getInstance()->replaceScene(MainMenu::createScene());
}
