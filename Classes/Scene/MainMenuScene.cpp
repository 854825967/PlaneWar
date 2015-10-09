#include "Header.h"
#include "GameConfig.h"
#include "DataManager.h"
#include "MainMenuScene.h"
#include "ScrollNoticeLayer.h"
#include "SInterfaceDefine.h"
#include "Tools.h"
#include "Accessorie/Accessorie.h"
#include "Accessorie/Plane.h"
#include "CButton.h"
#include "EquipManager.h"
#include "DepotScene.h"
#include "MissionScene.h"
#include "CBox.h"
#include "CEquipView.h"
#include "BattleScene.h"
#include "LadderScene.h"
USING_NS_CC;
USING_NS_CC_EXT;


MainMenu * MainMenu::s_pSelf = NULL;

MainMenu::MainMenu() {
    ASSERT(s_pSelf ==  NULL);
    s_pSelf = this;
}

MainMenu::~MainMenu() {
    s_pSelf = NULL;
}

Scene *  MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainMenu::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenu::init() {
    //////////////////////////////
    // 1. super init first
    if ( !PublicUI::init() ) {
        return false;
    }
    
    m_pPlane = NULL;
    
    initOtherUI();
    initButtonUI();
    initUserInfoUI();
    
    return true;
}

void MainMenu::initButtonUI() {
    m_SpriteDialBk = Sprite::create("UI/Public/Panel/opeBK.png");
    m_SpriteDialBk->setScale(g_pGameConfig->scaleEleX);
    m_SpriteDialBk->setPosition(Point(g_pGameConfig->screenCenterPoint.x, (m_SpriteDialBk->getContentSize().height/2) * g_pGameConfig->scaleEleX));
    this->addChild(m_SpriteDialBk,GRADE_UI);
    
    m_BtnMission = CButton::create("闯关", Vec2(70, -140),
                                   "UI/Public/Button/button.png", "UI/Public/Button/buttonPress.png", "",
                                   CC_CALLBACK_1(MainMenu::buttonMissionCallBack, this));
    m_BtnMission->setPosition(Vec2((m_BtnMission->getContentSize().width/2) * g_pGameConfig->scaleEleX + 38 * g_pGameConfig->scalex,
                                  (m_BtnMission->getContentSize().height/2) * g_pGameConfig->scaleEleX + 58 * g_pGameConfig->scaleEleX));
    m_BtnMission->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_BtnMission,GRADE_UI);
    
    
    m_BtnLadder = CButton::create("天梯", Vec2(70, -140),
                                  "UI/Public/Button/button.png", "UI/Public/Button/buttonPress.png", "",
                                  CC_CALLBACK_1(MainMenu::buttonLadderCallBack, this));
    m_BtnLadder->setPosition(Vec2((m_BtnLadder->getContentSize().width) * g_pGameConfig->scaleEleX * 1.5f - 50 * g_pGameConfig->scalex
                                 + 38 * g_pGameConfig->scalex, (m_BtnLadder->getContentSize().height/2) * g_pGameConfig->scaleEleX
                                 + 58 * g_pGameConfig->scaleEleX));
    m_BtnLadder->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_BtnLadder,GRADE_UI);
    
    m_BtnDepot = CButton::create("仓库", Vec2(70, -140),
                                 "UI/Public/Button/button.png", "UI/Public/Button/buttonPress.png", "",
                                 CC_CALLBACK_1(MainMenu::buttonDepotCallBack, this));
    m_BtnDepot->setPosition(Vec2((m_BtnDepot->getContentSize().width) * g_pGameConfig->scaleEleX * 2.5f - 100 * g_pGameConfig->scalex + 38 * g_pGameConfig->scalex,
                                (m_BtnDepot->getContentSize().height/2) * g_pGameConfig->scaleEleX + 58 * g_pGameConfig->scaleEleX));
    m_BtnDepot->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_BtnDepot,GRADE_UI);
    
    m_BtnFriend = CButton::create("好友", Vec2(70, -140),
                                  "UI/Public/Button/button.png", "UI/Public/Button/buttonPress.png", "",
                                  CC_CALLBACK_1(MainMenu::buttonFriendCallBack, this));
    m_BtnFriend->setPosition(Vec2((m_BtnDepot->getContentSize().width) * g_pGameConfig->scaleEleX * 3.5f - 150 * g_pGameConfig->scalex
                                 + 38 * g_pGameConfig->scalex, (m_BtnDepot->getContentSize().height/2) * g_pGameConfig->scaleEleX
                                 + 58 * g_pGameConfig->scaleEleX));
    m_BtnFriend->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_BtnFriend,GRADE_UI);
    
    m_BtnVipCharge = CButton::create("", Point::ZERO, "UI/Public/Button/vipChargerBtnBk.png", "UI/Public/Button/vipChargerBtnPressBk.png", "", NULL);
    m_BtnVipCharge->setPosition(Vec2(g_pGameConfig->screenSize.width - m_BtnVipCharge->getContentSize().width * 0.5f * g_pGameConfig->scaleEleX
                                    - 40 * g_pGameConfig->scaleEleX, g_pGameConfig->screenSize.height
                                    - m_BtnVipCharge->getContentSize().height * 0.5f * g_pGameConfig->scaleEleX - 580 * g_pGameConfig->scaleEleX));
    m_BtnVipCharge->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_BtnVipCharge,GRADE_UI);
    
    m_BtnCharge = CButton::create("", Point::ZERO, "UI/Public/Button/vipChargerBtnBk.png", "UI/Public/Button/vipChargerBtnPressBk.png", "", NULL);
    m_BtnCharge->setPosition(Vec2(g_pGameConfig->screenSize.width - m_BtnCharge->getContentSize().width * 0.5f * g_pGameConfig->scaleEleX - 40 * g_pGameConfig->scaleEleX,
                                 g_pGameConfig->screenSize.height - m_BtnCharge->getContentSize().height * g_pGameConfig->scaleEleX - 710 * g_pGameConfig->scaleEleX));
    m_BtnCharge->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_BtnCharge,GRADE_UI);
    
}

void MainMenu::initUserInfoUI() {
    Sprite * powerBarLabelBk = Sprite::create("UI/Public/Panel/powerBarLabelBk.png");
    powerBarLabelBk->setPosition(Vec2(powerBarLabelBk->getContentSize().width * 0.5f * g_pGameConfig->scaleEleX + 250 * g_pGameConfig->scaleEleX,
                                     g_pGameConfig->screenSize.height - powerBarLabelBk->getContentSize().height * g_pGameConfig->scaleEleX * 0.5f));
    powerBarLabelBk->setScale(g_pGameConfig->scaleEleX);
    this->addChild(powerBarLabelBk,GRADE_UI);
    
    Sprite * powerBarBk = Sprite::create("UI/Public/Button/powerBarBk.png");
    powerBarBk->setPosition(Vec2(powerBarBk->getContentSize().width * 0.5f * g_pGameConfig->scaleEleX + 587 * g_pGameConfig->scaleEleX,
                                g_pGameConfig->screenSize.height - powerBarBk->getContentSize().height * g_pGameConfig->scaleEleX * 0.5f - 10 * g_pGameConfig->scaleEleX));
    powerBarBk->setScale(g_pGameConfig->scaleEleX);
    
    Sprite * topOpeAreaBk = Sprite::create("UI/Public/Panel/topOpeAreaBk.png");
    topOpeAreaBk->setPosition(Vec2(g_pGameConfig->screenCenterPoint.x,
                                  g_pGameConfig->screenSize.height - topOpeAreaBk->getContentSize().height * g_pGameConfig->scaleEleX * 0.5f));
    topOpeAreaBk->setScale(g_pGameConfig->scaleEleX);
    this->addChild(topOpeAreaBk,GRADE_UI);
    
    Sprite * coinBtnBk = Sprite::create("UI/Public/Button/coinBk.png");
    coinBtnBk->setPosition(Vec2(335 * g_pGameConfig->scaleEleX + coinBtnBk->getContentSize().width * 0.5f * g_pGameConfig->scaleEleX,
                               g_pGameConfig->screenSize.height -powerBarLabelBk->getContentSize().height * g_pGameConfig->scaleEleX
                               -coinBtnBk->getContentSize().height * g_pGameConfig->scaleEleX * 0.5f - 5 * g_pGameConfig->scaleEleX));
    coinBtnBk->setScale(g_pGameConfig->scaleEleX);
    this->addChild(coinBtnBk,GRADE_UI);
    
    Sprite * diamondBtnBk = Sprite::create("UI/Public/Button/coinPressBk.png");
    diamondBtnBk->setPosition(Vec2(300 * g_pGameConfig->scaleEleX + diamondBtnBk->getContentSize().width * 0.5f * g_pGameConfig->scaleEleX
                                  + coinBtnBk->getContentSize().width * g_pGameConfig->scaleEleX,
                                  g_pGameConfig->screenSize.height -powerBarLabelBk->getContentSize().height * g_pGameConfig->scaleEleX
                                  -coinBtnBk->getContentSize().height * g_pGameConfig->scaleEleX * 0.5f - 5 * g_pGameConfig->scaleEleX));
    diamondBtnBk->setScale(g_pGameConfig->scaleEleX);
    this->addChild(diamondBtnBk,GRADE_UI);
    
    
    m_PowerBar = ProgressTimer::create(powerBarBk);
    m_PowerBar->setType(ProgressTimer::Type::BAR);
    m_PowerBar->setMidpoint(Vec2(0, 0.5f));
    m_PowerBar->setBarChangeRate(Vec2(1, 0));
    m_PowerBar->setPosition(Vec2(powerBarBk->getContentSize().width * 0.5f * g_pGameConfig->scaleEleX + 587 * g_pGameConfig->scaleEleX,
                                g_pGameConfig->screenSize.height - powerBarBk->getContentSize().height * g_pGameConfig->scaleEleX * 0.5f
                                - 10 * g_pGameConfig->scaleEleX));
    m_PowerBar->setScale(g_pGameConfig->scaleEleX);
    m_PowerBar->setMidpoint(Point(0, 0.5f));
    m_PowerBar->setBarChangeRate(Point(1, 0));
    this->addChild(m_PowerBar,GRADE_UI);
}

void MainMenu::initOtherUI() {
    m_pBackground = Sprite::create("UI/mainMenu/mainMenuBk.png");
    m_pBackground->setScale(g_pGameConfig->scaleEleMax);
    m_pBackground->setPosition(Point(g_pGameConfig->screenCenterPoint.x, g_pGameConfig->screenCenterPoint.y));
    this->addChild(m_pBackground, GRADE_BACKGOUND);
    
    ScrollNoticeLayer * notice = ScrollNoticeLayer::create();
    notice->setPosition(Point(g_pGameConfig->screenCenterPoint.x, g_pGameConfig->screenSize.height -
                              notice->getContentSize().height * 0.5f * g_pGameConfig->scaleEleX - 460 * g_pGameConfig->scaleEleX));
    notice->setScale(g_pGameConfig->scaleEleX);
    this->addChild(notice,GRADE_UI);
}

void MainMenu::buttonMissionCallBack(Ref * sender) {
    Director::getInstance()->replaceScene(MissionScene::createScene());
}

void MainMenu::buttonLadderCallBack(Ref * sender) {
    Director::getInstance()->replaceScene(LadderScene::createScene());
}

void MainMenu::buttonDepotCallBack(Ref * sender) {
    Director::getInstance()->replaceScene(DepotScene::createScene());
}

void MainMenu::buttonFriendCallBack(Ref * sender) {
    alert("developing");
}

void MainMenu::setLevel(const s32 level) {
    m_LabelLevel = Label::create(IntAsString(level),"WRYH.ttf",45);
    m_LabelLevel->setPosition(Vec2(360 * g_pGameConfig->scaleEleX + m_LabelLevel->getContentSize().width * 0.5f * g_pGameConfig->scaleEleX,
                                  g_pGameConfig->screenSize.height - m_LabelLevel->getContentSize().height * g_pGameConfig->scaleEleX * 0.5f-25 * g_pGameConfig->scaleEleX));
    
    m_LabelLevel->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_LabelLevel,GRADE_UI);
}

void MainMenu::setMoney(const s32 coin) {
    m_LabelCoin = Label::create(IntAsString(coin),"WRYH.ttf",50);
    m_LabelCoin->setPosition(Vec2(500 * g_pGameConfig->scaleEleX + m_LabelCoin->getContentSize().width * 0.5f * g_pGameConfig->scaleEleX,
                                 g_pGameConfig->screenSize.height - m_LabelCoin->getContentSize().height * g_pGameConfig->scaleEleX * 0.5f
                                 -120 * g_pGameConfig->scaleEleX));
    m_LabelCoin->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_LabelCoin,GRADE_UI);
}

void MainMenu::setDiamond(const s32 diamond) {
    m_LabelDiamond = Label::create(IntAsString(diamond),"WRYH.ttf",50);
    m_LabelDiamond->setPosition(Vec2(920 * g_pGameConfig->scaleEleX + m_LabelDiamond->getContentSize().width * 0.5f * g_pGameConfig->scaleEleX,
                                    g_pGameConfig->screenSize.height - m_LabelDiamond->getContentSize().height * g_pGameConfig->scaleEleX * 0.5f-120 * g_pGameConfig->scaleEleX));
    m_LabelDiamond->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_LabelDiamond,GRADE_UI);
}

void MainMenu::setPower(const s32 nowPower, const s32 maxPower) {
    m_LabelShowPower = Label::create(IntAsString(nowPower)+"/"+IntAsString(maxPower),"WRYH.ttf",40);
    m_LabelShowPower->setAnchorPoint(Vec2(.5f, .5f));
    m_LabelShowPower->setPosition(Vec2(m_PowerBar->getContentSize().width * 0.5f,
                                      m_PowerBar->getContentSize().height * 0.5f));
    m_PowerBar->addChild(m_LabelShowPower,GRADE_UI);
    s32 percentage = nowPower * 100/maxPower;
    m_PowerBar->setPercentage(percentage);
}

void MainMenu::setVipGrade(const s32 grade) {
    m_LabelVipGrade = Label::create(IntAsString(grade),"WRYH.ttf",40);
    m_LabelVipGrade->setPosition(Vec2(g_pGameConfig->screenSize.width - 435 * g_pGameConfig->scaleEleX + m_LabelVipGrade->getContentSize().width * 0.5f * g_pGameConfig->scaleEleX,
                                     g_pGameConfig->screenSize.height - m_LabelVipGrade->getContentSize().height * g_pGameConfig->scaleEleX * 0.5f - 25 * g_pGameConfig->scaleEleX));
    m_LabelVipGrade->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_LabelVipGrade,GRADE_UI);
}

void MainMenu::setUserName(const char *  name) {
    if (name !=  NULL) {
        m_LabelUserName = Label::create(name,"WRYH.ttf",40);
        m_LabelUserName->setAnchorPoint(Vec2(.5f, .5f));
        m_LabelUserName->setPosition(Vec2(160 * g_pGameConfig->scaleEleX,
                                         g_pGameConfig->screenSize.height - m_LabelUserName->getContentSize().height * g_pGameConfig->scaleEleX * 0.5f-200 * g_pGameConfig->scaleEleX));
        m_LabelUserName->setScale(g_pGameConfig->scaleEleX);
        this->addChild(m_LabelUserName,GRADE_UI);
    }
}

void MainMenu::setUserImage(Node * node) {
    if (node !=  NULL) {
        m_SpriteUserImage = (Sprite  * )node;
        m_SpriteUserImage->setPosition(Vec2(m_SpriteUserImage->getContentSize().width * 0.5f * g_pGameConfig->scaleEleX,
                                           g_pGameConfig->screenSize.height - m_SpriteUserImage->getContentSize().height * g_pGameConfig->scaleEleX * 0.5f));
        m_SpriteUserImage->setScale(g_pGameConfig->scaleEleX);
        this->addChild(m_SpriteUserImage,GRADE_UI);
    }
}

//展示玩家战机
void MainMenu::showPlane(Plane * plane)  {
    if (m_pPlane != NULL) {
        removeChild(m_pPlane);
    }
    
    m_pPlane = plane;
    m_pPlane->setScale(g_pGameConfig->scaleEleMin);
    m_pPlane->setPosition(g_pGameConfig->screenCenterPoint);
    this->addChild(m_pPlane, GRADE_PLAYER);
}

void MainMenu::onEnter() {
    PublicUI::onEnter();
    
    //设置玩家等级
    setLevel(g_exp);
    //设置金钱数
    setMoney(g_money);
    //设置钻石数
    setDiamond(g_dimond);
    //设置体力
    setPower(g_power, 100);
    //设置VIP等级
    setVipGrade(g_vip);
    //设置玩家名
    setUserName(g_name.c_str());
    //设置玩家头像
    //setUserImage();
    
    //展示玩家战机
    SEquip equip;
    g_pEquipManager->getGrooveEquip(EQUIP_TYPE_PLANE, equip);
    

    Plane * pPlane = Plane::create(equip);
    showPlane(pPlane);
    g_pEquipManager->getGrooveEquip(EQUIP_TYPE_WEAPON, equip);
    pPlane->addWeapon(equip);
    g_pEquipManager->getGrooveEquip(EQUIP_TYPE_WINGMAN, equip);
    pPlane->addWingman(equip);
    
    BIND(BUND_ID_BULLET_FIRED, this, MainMenu::bulletFired);
    g_pEventEngine->BundlerCall(BUND_ID_PLANE_FIRE, this, sizeof(this));

    pPlane->showtime();
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Animation/Wingman/mijializipao.ExportJson");
    Armature * test = Armature::create("mijializipao");
    test->setAnchorPoint(Vec2(.5f, .5f));
    this->addChild(test);
    test->setPosition(g_pGameConfig->screenCenterPoint);
    test->getAnimation()->play("rage");
}

void MainMenu::onExit() {
    PublicUI::onExit();
    UNBIND(BUND_ID_BULLET_FIRED, this, MainMenu::bulletFired);
}

void MainMenu::bulletFired(void *context, const s32 size) {
    addChild((Node*)context, GRADE_BULLET);
}
