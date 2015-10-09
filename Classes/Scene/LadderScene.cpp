#include "Header.h"
#include "Data/GameConfig.h"
#include "UI/CButton.h"
#include "LadderScene.h"
#include "MainMenuScene.h"
USING_NS_CC;


LadderScene * LadderScene::s_pSelf = NULL;

LadderScene::LadderScene() {
    ASSERT(s_pSelf == NULL);
    s_pSelf = this;
}

LadderScene::~LadderScene() {
    s_pSelf = NULL;
}

Scene* LadderScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LadderScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LadderScene::init() {
    //////////////////////////////
    // 1. super init first
    if ( !PublicUI::init() ) {
        return false;
    }
    initButtonUI();
    initUserInfoUI();
    initRivalInfoUI();
    initOtherUI();
    
    return true;
}

void LadderScene::menuReturnMainMenu(cocos2d::Ref *pSender) {
    Director::getInstance()->replaceScene(MainMenu::createScene());
}

void LadderScene::initButtonUI() {
    //关闭按钮
    CButton * BtnBack = CButton::create("", Point::ZERO,
                                        "UI/Public/Button/backBtn.png", "UI/Public/Button/backPressBtn.png", "",
                                        CC_CALLBACK_1(LadderScene::menuReturnMainMenu, this));
    BtnBack->setPosition(Vec2(g_pGameConfig->screenSize.width - (BtnBack->getContentSize().width) * g_pGameConfig->scaleEleMin * 0.5f
                              - 10 * g_pGameConfig->scalex, g_pGameConfig->screenSize.height
                              - (BtnBack->getContentSize().height) * g_pGameConfig->scaleEleMin * 0.5f - 5 * g_pGameConfig->scalex));
    BtnBack->setScale(g_pGameConfig->scaleEleX);
    this->addChild(BtnBack, GRADE_UI);
    
    CButton * m_BtnMission = CButton::create("闯关", ccp(70, -140),
                                             "UI/Public/Button/button.png", "UI/Public/Button/buttonPress.png", "");
    m_BtnMission->setPosition(ccp((m_BtnMission->getContentSize().width/2) * g_pGameConfig->scaleEleX + 38 * g_pGameConfig->scalex,
                                  (m_BtnMission->getContentSize().height/2) * g_pGameConfig->scaleEleX + 58 * g_pGameConfig->scaleEleX));
    m_BtnMission->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_BtnMission, GRADE_UI);
    
    
    CButton * m_BtnLadder = CButton::create("天梯", ccp(70, -140),
                                            "UI/Public/Button/button.png", "UI/Public/Button/buttonPress.png", "");
    m_BtnLadder->setPosition(ccp((m_BtnLadder->getContentSize().width) * g_pGameConfig->scaleEleX * 1.5f - 50 * g_pGameConfig->scalex
                                 + 38 * g_pGameConfig->scalex, (m_BtnLadder->getContentSize().height/2) * g_pGameConfig->scaleEleX
                                 + 58 * g_pGameConfig->scaleEleX));
    m_BtnLadder->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_BtnLadder, GRADE_UI);
    
    CButton * m_BtnDepot = CButton::create("仓库", ccp(70, -140),
                                           "UI/Public/Button/button.png", "UI/Public/Button/buttonPress.png", "");
    m_BtnDepot->setPosition(ccp((m_BtnDepot->getContentSize().width) * g_pGameConfig->scaleEleX * 2.5f - 100 * g_pGameConfig->scalex + 38 * g_pGameConfig->scalex,
                                (m_BtnDepot->getContentSize().height/2) * g_pGameConfig->scaleEleX + 58 * g_pGameConfig->scaleEleX));
    m_BtnDepot->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_BtnDepot, GRADE_UI);
    
    CButton * m_BtnFriend = CButton::create("好友", ccp(70, -140),
                                            "UI/Public/Button/button.png", "UI/Public/Button/buttonPress.png", ""
                                            );
    m_BtnFriend->setPosition(ccp((m_BtnDepot->getContentSize().width) * g_pGameConfig->scaleEleX * 3.5f - 150 * g_pGameConfig->scalex
                                 + 38 * g_pGameConfig->scalex, (m_BtnDepot->getContentSize().height/2) * g_pGameConfig->scaleEleX
                                 + 58 * g_pGameConfig->scaleEleX));
    m_BtnFriend->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_BtnFriend, GRADE_UI);
}

void LadderScene::initOtherUI() {
    //背景
    m_pBackground = Sprite::create("UI/Ladder/ladderBk.png");
    m_pBackground->setScale(g_pGameConfig->scaleEleMax);
    m_pBackground->setPosition(Point(g_pGameConfig->screenCenterPoint.x, g_pGameConfig->screenCenterPoint.y));
    this->addChild(m_pBackground, GRADE_BACKGOUND);
    
    //顶部操作区背景
    Sprite * topOpeBk = Sprite::create("UI/Public/Panel/topOpeBk.png");
    topOpeBk->setPosition(ccp(g_pGameConfig->screenCenterPoint.x, g_pGameConfig->screenSize.height - 85 * g_pGameConfig->scaleEleX));
    topOpeBk->setScale(g_pGameConfig->scaleEleX);
    this->addChild(topOpeBk, GRADE_BACKGOUND);
    
    //天梯页面标题
    m_LabelTitle = Label::create("天梯竞技场", "WRYH.ttf", 50);
    m_LabelTitle->setAnchorPoint(Point(0, 0.5f));
    m_LabelTitle->setScale(g_pGameConfig->scaleEleX);
    m_LabelTitle->setPosition(Point(45 * g_pGameConfig->scaleEleX, g_pGameConfig->screenSize.height - 60 * g_pGameConfig->scaleEleX));
    this->addChild(m_LabelTitle, GRADE_UI);
    
    //挑战标签背景
    Sprite * chaLabelBk =  Sprite::create("UI/Ladder/chaLabelBk.png");
    chaLabelBk->setPosition(ccp(g_pGameConfig->screenCenterPoint.x, g_pGameConfig->screenCenterPoint.y + 600 * g_pGameConfig->scaleEleX));
    chaLabelBk->setScale(g_pGameConfig->scaleEleX);
    this->addChild(chaLabelBk, GRADE_UI);

    //挑战次数背景
    auto chalCountBk = Sprite::create("UI/Ladder/chalCountBk.png");
    chalCountBk->setPosition(ccp(g_pGameConfig->screenCenterPoint.x, 440 * g_pGameConfig->scaleEleX));
    chalCountBk->setScale(g_pGameConfig->scaleEleX);
    addChild(chalCountBk, GRADE_UI);
    
    //挑战限制标签背景
    auto chaLimitBk = Sprite::create("UI/Ladder/chaLimitBk.png");
    chaLimitBk->setPosition(ccp(chalCountBk->getContentSize().width*0.5, chalCountBk->getContentSize().height*0.5 + 10));
    chalCountBk->addChild(chaLimitBk, GRADE_UI);
    
    //添加挑战次数按钮
    auto addCountBtn = Sprite::create("UI/Ladder/addCountBtn.png");
    addCountBtn->setPosition(ccp(chalCountBk->getContentSize().width*0.5 + 320,
                                 chalCountBk->getContentSize().height*0.5 + 15));
    chalCountBk->addChild(addCountBtn, GRADE_UI);
    
    Label * chaLabel =  Label::create("挑 战", "WRYH.ttf", 70);
    chaLabel->setPosition(ccp(g_pGameConfig->screenCenterPoint.x, g_pGameConfig->screenCenterPoint.y + 600 * g_pGameConfig->scaleEleX));
    chaLabel->setScale(g_pGameConfig->scaleEleX);
    this->addChild(chaLabel, GRADE_UI);
    
    
    
    Sprite * bottomOpeAreaBk = Sprite::create("UI/Public/Panel/bottomOpeAreaBk.png");
    bottomOpeAreaBk->setPosition(ccp(g_pGameConfig->screenCenterPoint.x, (bottomOpeAreaBk->getContentSize().height)*g_pGameConfig->scaleEleX + 208*g_pGameConfig->scaleEleX));
    bottomOpeAreaBk->setScale(g_pGameConfig->scaleEleX);
    this->addChild(bottomOpeAreaBk, GRADE_UI);
}

void LadderScene::initUserInfoUI() {
    Sprite * userInfoSprite = Sprite::create("UI/Ladder/userInfo.png");
    userInfoSprite->setPosition(ccp(g_pGameConfig->screenCenterPoint.x, g_pGameConfig->screenSize.height - 250*g_pGameConfig->scaleEleX));
    userInfoSprite->setScale(g_pGameConfig->scaleEleX);
    this->addChild(userInfoSprite, GRADE_UI);
}

void LadderScene::initRivalInfoUI() {
    //对手1标签背景
    Sprite * playerLabel1 =  Sprite::create("UI/Ladder/matchInfoLeftPanel.png");
    playerLabel1->setPosition(ccp(g_pGameConfig->screenCenterPoint.x - 230 * g_pGameConfig->scaleEleX,
                                  g_pGameConfig->screenCenterPoint.y + 420 * g_pGameConfig->scaleEleX));
    playerLabel1->setScale(g_pGameConfig->scaleEleX);
    addChild(playerLabel1, GRADE_UI);
    //对手1头像
    //    auto playerhead1 =  Sprite::create("UI/Ladder/headImage.png");
    //    playerhead1->setPosition(ccp(-playerhead1->getContentSize().width*0.5 + 18,
    //                                 playerLabel1->getContentSize().height*0.5 - 2));
    //    playerLabel1->addChild(playerhead1);
    //
    //对手2标签背景
    Sprite * playerLabel2 =  Sprite::create("UI/Ladder/matchInfoRightPanel.png");
    playerLabel2->setPosition(ccp(g_pGameConfig->screenCenterPoint.x + 230 * g_pGameConfig->scaleEleX,
                                  g_pGameConfig->screenCenterPoint.y + 220 * g_pGameConfig->scaleEleX));
    playerLabel2->setScale(g_pGameConfig->scaleEleX);
    addChild(playerLabel2, GRADE_UI);
    //
    //    //对手2头像
    //    auto playerhead2 =  Sprite::create("UI/Ladder/headImage.png");
    //    playerhead2->setPosition(ccp(playerLabel2->getContentSize().width + playerhead2->getContentSize().width*0.5 - 18,
    //                                 playerLabel2->getContentSize().height*0.5 - 2));
    //    playerLabel2->addChild(playerhead2);
    //
    //对手3标签背景
    Sprite * playerLabel3 =  Sprite::create("UI/Ladder/matchInfoLeftPanel.png");
    playerLabel3->setPosition(ccp(g_pGameConfig->screenCenterPoint.x - 230 * g_pGameConfig->scaleEleX,
                                  g_pGameConfig->screenCenterPoint.y + 20* g_pGameConfig->scaleEleX));
    playerLabel3->setScale(g_pGameConfig->scaleEleX);
    addChild(playerLabel3, GRADE_UI);
    //    //对手3头像
    //    auto playerhead3 =  Sprite::create("UI/Ladder/headImage.png");
    //    playerhead3->setPosition(ccp(-playerhead1->getContentSize().width*0.5 + 18,
    //                                 playerLabel1->getContentSize().height*0.5 - 2));
    //    playerLabel3->addChild(playerhead3);
    //
    //对手4标签背景
    Sprite * playerLabel4 =  Sprite::create("UI/Ladder/matchInfoRightPanel.png");
    playerLabel4->setPosition(ccp(g_pGameConfig->screenCenterPoint.x + 230 * g_pGameConfig->scaleEleX,
                                  g_pGameConfig->screenCenterPoint.y - 180 * g_pGameConfig->scaleEleX));
    playerLabel4->setScale(g_pGameConfig->scaleEleX);
    this->addChild(playerLabel4, GRADE_UI);
    //
    //    //对手4头像
    //    auto playerhead4 =  Sprite::create("UI/Ladder/headImage.png");
    //    playerhead4->setPosition(ccp(playerLabel4->getContentSize().width + playerhead4->getContentSize().width*0.5 - 18,
    //                                 playerLabel4->getContentSize().height*0.5 - 2));
    //    playerLabel4->addChild(playerhead4);
    //
    //对手5标签背景
    Sprite * playerLabel5 =  Sprite::create("UI/Ladder/matchInfoLeftPanel.png");
    playerLabel5->setPosition(ccp(g_pGameConfig->screenCenterPoint.x - 230 * g_pGameConfig->scaleEleX,
                                  g_pGameConfig->screenCenterPoint.y - 380 * g_pGameConfig->scaleEleX));
    playerLabel5->setScale(g_pGameConfig->scaleEleX);
    this->addChild(playerLabel5, GRADE_UI);
}
