#include "Header.h"
#include "DepotScene.h"
#include "GameConfig.h"
#include "EquipManager.h"
#include "MainMenuScene.h"
#include "CEquipView.h"
#include "CButton.h"
#include "Plane.h"
USING_NS_CC;
USING_NS_CC_EXT;

DepotScene * DepotScene::s_pSelf = NULL;


Scene *  DepotScene::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = DepotScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool DepotScene::init() {
	//////////////////////////////
	// 1. super init first
	if ( !PublicUI::init() ) {
		return false;
	}
   
    initOtherUI();
    initUserInfoUI();
    initButtonUI();
    initEquipIcon();
    
    return true;
}

void DepotScene::bulletFired(void *context, const s32 size) {
    addChild((Node*)context, GRADE_BULLET);
}

void DepotScene::initButtonUI() {
    //底部操作区背景
    Sprite * bottomOpeAreaBk = Sprite::create("UI/Public/Panel/bottomOpeAreaBk.png");
    bottomOpeAreaBk->setPosition(Vec2(g_pGameConfig->screenCenterPoint.x, (bottomOpeAreaBk->getContentSize().height) * g_pGameConfig->scaleEleX
                                     + 208 * g_pGameConfig->scaleEleX));
    bottomOpeAreaBk->setScale(g_pGameConfig->scaleEleX);
    this->addChild(bottomOpeAreaBk,GRADE_UI);

    m_BtnMission = CButton::create("仓库", Vec2(70, -140), "UI/Public/Button/button.png", "UI/Public/Button/buttonPress.png", "", NULL);
    m_BtnMission->setPosition(Vec2((m_BtnMission->getContentSize().width/2) * g_pGameConfig->scaleEleX + 38 * g_pGameConfig->scalex,
                              (m_BtnMission->getContentSize().height/2) * g_pGameConfig->scaleEleX + 58 * g_pGameConfig->scaleEleX));
    m_BtnMission->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_BtnMission,GRADE_UI);

    m_BtnLadder = CButton::create("强化", Vec2(70, -140),
                                  "UI/Public/Button/button.png", "UI/Public/Button/buttonPress.png", "",
                                  CC_CALLBACK_1(DepotScene::menuStrenCallbakc, this));
    m_BtnLadder->setPosition(Vec2((m_BtnLadder->getContentSize().width) * g_pGameConfig->scaleEleX * 1.5f - 50 * g_pGameConfig->scalex
                             + 38 * g_pGameConfig->scalex, (m_BtnLadder->getContentSize().height/2) * g_pGameConfig->scaleEleX
                             + 58 * g_pGameConfig->scaleEleX));
    m_BtnLadder->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_BtnLadder,GRADE_UI);

    m_BtnSell = CButton::create("出售", Vec2(70, -140),
                                "UI/Public/Button/button.png", "UI/Public/Button/buttonPress.png", "",
                                CC_CALLBACK_1(DepotScene::menuSellCallback, this));
    m_BtnSell->setPosition(Vec2((m_BtnSell->getContentSize().width) * g_pGameConfig->scaleEleX * 2.5f - 100 * g_pGameConfig->scalex + 38 * g_pGameConfig->scalex,
                            (m_BtnSell->getContentSize().height/2) * g_pGameConfig->scaleEleX + 58 * g_pGameConfig->scaleEleX));
    m_BtnSell->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_BtnSell,GRADE_UI);

    m_BtnFriend = CButton::create("好友", Vec2(70, -140), "UI/Public/Button/button.png", "UI/Public/Button/buttonPress.png", "");
    m_BtnFriend->setPosition(Vec2((m_BtnSell->getContentSize().width) * g_pGameConfig->scaleEleX * 3.5f - 150 * g_pGameConfig->scalex
                             + 38 * g_pGameConfig->scalex, (m_BtnSell->getContentSize().height/2) * g_pGameConfig->scaleEleX
                             + 58 * g_pGameConfig->scaleEleX));
    m_BtnFriend->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_BtnFriend,GRADE_UI);

    m_BtnEquip[EQUIP_TYPE_PLANE] = CButton::create("", Point::ZERO,
                                                   "UI/Depot/equipBtn.png", "UI/Depot/equipBtn.png", "",
                                                   CC_CALLBACK_1(DepotScene::menuGroovesCall, this));
    m_BtnEquip[EQUIP_TYPE_PLANE]->setPosition(Vec2((m_BtnEquip[EQUIP_TYPE_PLANE]->getContentSize().width) * g_pGameConfig->scaleEleX * 0.5f + 20 * g_pGameConfig->scaleEleX,
                               g_pGameConfig->screenCenterPoint.y - (m_BtnEquip[EQUIP_TYPE_PLANE]->getContentSize().height/2) * g_pGameConfig->scaleEleX
                               - 320 * g_pGameConfig->scaleEleX));
    m_BtnEquip[EQUIP_TYPE_PLANE]->setScale(g_pGameConfig->scaleEleX);
    m_BtnEquip[EQUIP_TYPE_PLANE]->setTag(EQUIP_TYPE_PLANE);
    this->addChild(m_BtnEquip[EQUIP_TYPE_PLANE],GRADE_UI);
    
    m_BtnEquip[EQUIP_TYPE_WINGMAN] = CButton::create("", Point::ZERO,
                                                     "UI/Depot/equipBtn.png", "UI/Depot/equipBtn.png", "",
                                                     CC_CALLBACK_1(DepotScene::menuGroovesCall, this));
    m_BtnEquip[EQUIP_TYPE_WINGMAN]->setPosition(
                               Vec2(g_pGameConfig->screenSize.width - (m_BtnEquip[EQUIP_TYPE_WINGMAN]->getContentSize().width) * g_pGameConfig->scaleEleX * 0.5f
                               - 20 * g_pGameConfig->scaleEleX, g_pGameConfig->screenCenterPoint.y
                               - (m_BtnEquip[EQUIP_TYPE_WINGMAN]->getContentSize().height/2) * g_pGameConfig->scaleEleX - 20 * g_pGameConfig->scaleEleX));
    m_BtnEquip[EQUIP_TYPE_WINGMAN]->setScale(g_pGameConfig->scaleEleX);
    m_BtnEquip[EQUIP_TYPE_WINGMAN]->setTag(EQUIP_TYPE_WINGMAN);
    this->addChild(m_BtnEquip[EQUIP_TYPE_WINGMAN],GRADE_UI);

    m_BtnEquip[EQUIP_TYPE_ARMOR] = CButton::create("", Point::ZERO,
                                                   "UI/Depot/equipBtn.png", "UI/Depot/equipBtn.png", "",
                                                   CC_CALLBACK_1(DepotScene::menuGroovesCall, this));
    m_BtnEquip[EQUIP_TYPE_ARMOR]->setPosition(Vec2(g_pGameConfig->screenSize.width - (m_BtnEquip[EQUIP_TYPE_ARMOR]->getContentSize().width) * g_pGameConfig->scaleEleX * 0.5f
                               - 20 * g_pGameConfig->scaleEleX, g_pGameConfig->screenCenterPoint.y
                               + (m_BtnEquip[EQUIP_TYPE_ARMOR]->getContentSize().height) * g_pGameConfig->scaleEleX * 0.5f + 400 * g_pGameConfig->scaleEleX));
    m_BtnEquip[EQUIP_TYPE_ARMOR]->setScale(g_pGameConfig->scaleEleX);
    m_BtnEquip[EQUIP_TYPE_ARMOR]->setTag(EQUIP_TYPE_ARMOR);
    this->addChild(m_BtnEquip[EQUIP_TYPE_ARMOR],GRADE_UI);
    
    m_BtnEquip[EQUIP_TYPE_WEAPON] = CButton::create("", Point::ZERO,
                                                    "UI/Depot/equipBtn.png", "UI/Depot/equipBtn.png", "",
                                                    CC_CALLBACK_1(DepotScene::menuGroovesCall, this));
    m_BtnEquip[EQUIP_TYPE_WEAPON]->setPosition(Vec2((m_BtnEquip[EQUIP_TYPE_WEAPON]->getContentSize().width) * g_pGameConfig->scaleEleX * 0.5f + 20 * g_pGameConfig->scaleEleX,
                               g_pGameConfig->screenCenterPoint.y + (m_BtnEquip[EQUIP_TYPE_WEAPON]->getContentSize().height/2) * g_pGameConfig->scaleEleX
                               + 100 * g_pGameConfig->scaleEleX));
    m_BtnEquip[EQUIP_TYPE_WEAPON]->setScale(g_pGameConfig->scaleEleX);
    m_BtnEquip[EQUIP_TYPE_WEAPON]->setTag(EQUIP_TYPE_WEAPON);
    this->addChild(m_BtnEquip[EQUIP_TYPE_WEAPON],GRADE_UI);
}

void DepotScene::initUserInfoUI() {
    //关闭按钮
    CButton * BtnBack = CButton::create("", Point::ZERO,
                                        "UI/Public/Button/backBtn.png", "UI/Public/Button/backPressBtn.png", "",
                                        CC_CALLBACK_1(DepotScene::menuReturnMainMenu, this));
    BtnBack->setPosition(Vec2(g_pGameConfig->screenSize.width - (BtnBack->getContentSize().width) * g_pGameConfig->scaleEleX * 0.5f
                             - 10 * g_pGameConfig->scalex, g_pGameConfig->screenSize.height
                             - (BtnBack->getContentSize().height) * g_pGameConfig->scaleEleX * 0.5f - 5 * g_pGameConfig->scalex));
    BtnBack->setScale(g_pGameConfig->scaleEleX);
    this->addChild(BtnBack, GRADE_UI);
    
    //标题
    m_LabelTitle = Label::create("仓库", "WRYH.ttf", 50);
    m_LabelTitle->setScale(g_pGameConfig->scaleEleMin);
    m_LabelTitle->setPosition(Point(65 * g_pGameConfig->scaleEleX + m_LabelTitle->getContentSize().width * 0.5f * g_pGameConfig->scaleEleX,
                                     g_pGameConfig->screenSize.height - m_LabelTitle->getContentSize().height * 0.5f * g_pGameConfig->scaleEleX
                                     - 25 * g_pGameConfig->scaleEleX));
    this->addChild(m_LabelTitle,GRADE_UI);
    
    //战斗力
    Label * m_LabelForce = Label::create("战斗力:", "WRYH.ttf", 50);
    m_LabelForce->setScale(g_pGameConfig->scaleEleX);
    m_LabelForce->setPosition(Point(65 * g_pGameConfig->scaleEleX + m_LabelForce->getContentSize().width * 0.5f * g_pGameConfig->scaleEleX,
                                    g_pGameConfig->screenSize.height - m_LabelForce->getContentSize().height * 0.5f * g_pGameConfig->scaleEleX
                                    - 260 * g_pGameConfig->scaleEleX));
    this->addChild(m_LabelForce,GRADE_UI);
    
    //设置战斗力
    m_LabelShowForce = Label::create("","WRYH.ttf",50);
    m_LabelShowForce->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_LabelShowForce->setAnchorPoint(Vec2(0, 0.5f));
    m_LabelShowForce->setPosition(Vec2(260.0f * g_pGameConfig->scaleEleX, g_pGameConfig->screenSize.height - 290.0f * g_pGameConfig->scaleEleX));
    m_LabelShowForce->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_LabelShowForce,GRADE_UI);
    
    //顶部操作区背景
    Sprite * topOpeBk = Sprite::create("UI/Public/Panel/topOpeBk.png");
    topOpeBk->setPosition(Vec2(g_pGameConfig->screenCenterPoint.x, g_pGameConfig->screenSize.height
                              - (topOpeBk->getContentSize().height) * g_pGameConfig->scaleEleX * 0.5f));
    topOpeBk->setScale(g_pGameConfig->scaleEleX);
    this->addChild(topOpeBk,GRADE_BACKGOUND);
}

void DepotScene::initOtherUI() {
    //背景
    m_pBackground = Sprite::create("UI/Depot/depotBk.png");
    m_pBackground->setScale(g_pGameConfig->scaleEleMax);
    m_pBackground->setPosition(Point(g_pGameConfig->screenCenterPoint.x,
                                     g_pGameConfig->screenCenterPoint.y));
    this->addChild(m_pBackground, GRADE_BACKGOUND);
    
    //装备展示区
    m_SpriteShowPlane = Sprite::create("UI/Depot/equipShowBk.png");
    m_SpriteShowPlane->setPosition(Vec2(g_pGameConfig->screenCenterPoint.x,
                               g_pGameConfig->screenCenterPoint.y));
    m_SpriteShowPlane->setScale(g_pGameConfig->scaleEleX);
    this->addChild(m_SpriteShowPlane,GRADE_BACKGOUND);
}

void DepotScene::initEquipIcon() {
    EQUIP_LIST equips;
    g_pEquipManager->getGrooveEquips(equips);
    
    for(EQUIP_LIST::const_iterator itor = equips.cbegin(); itor != equips.cend(); itor++) {
        Sprite * pSprite = g_pGameConfig->getIcon((*itor).model, (*itor).type);
        setGrooveIcon((*itor).type, pSprite);
    }
    
    setForce(g_pEquipManager->countCapacity(equips));
}

void DepotScene::recountForce() {
    EQUIP_LIST equips;
    g_pEquipManager->getGrooveEquips(equips);
    setForce(g_pEquipManager->countCapacity(equips));
}

//强化材料选择
void DepotScene::initStrenEquipUI() {
    mask();
    strenBk = Sprite::create("UI/Depot/equipStrenPanel.png");
    strenBk->setPosition(Vec2(g_pGameConfig->screenCenterPoint.x, g_pGameConfig->screenCenterPoint.y));
    strenBk->setScale(g_pGameConfig->scaleEleX);
    addChild(strenBk, GRADE_UI);

    
    m_BtnChooseEquip = CButton::create("选择装备", Point::ZERO, "UI/Depot/chooseEquipBtn.png", "UI/Depot/chooseEquipBtn.png", "",
                                       CC_CALLBACK_1(DepotScene::menuAddStrenEquip, this));
    m_BtnChooseEquip->setPosition(Vec2(200, strenBk->getContentSize().height - 440.0f));
    strenBk->addChild(m_BtnChooseEquip, GRADE_UI);

    
    m_SpriteExpBarBk = Sprite::create("UI/Depot/expBarBk.png");
    m_SpriteExpBarBk->setPosition(Vec2(strenBk->getContentSize().width - 550.0f,
                                      strenBk->getContentSize().height * 0.5f - 50));
    strenBk->addChild(m_SpriteExpBarBk, GRADE_UI);
    
    Sprite * expBarBk = Sprite::create("UI/Depot/expBar.png");
//    expBarBk->setPosition(Vec2(m_SpriteExpBarBk->getContentSize().width * 0.5f ,
//                               m_SpriteExpBarBk->getContentSize().height * 0.5f));
    //经验进度条
    m_ExpBar = ProgressTimer::create(expBarBk);
    m_ExpBar->setType(ProgressTimer::Type::BAR);
    m_ExpBar->setMidpoint(Vec2(0, 0.5f));
    m_ExpBar->setBarChangeRate(Vec2(1, 0));
    m_ExpBar->setPosition(Vec2(m_SpriteExpBarBk->getContentSize().width * 0.5f,m_SpriteExpBarBk->getContentSize().height * 0.5f - 0.5f));
    m_ExpBar->setMidpoint(Point(0, 0.5f));
    m_ExpBar->setBarChangeRate(Point(1, 0));
    m_SpriteExpBarBk->addChild(m_ExpBar, GRADE_UI, 0);

    
    //升级还需经验标签
    m_LabelUpgradeNeed = Label::create("升级所需经验", "WRYH.ttf", 40);
    m_LabelUpgradeNeed->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_LabelUpgradeNeed->setAnchorPoint(Vec2(0, 0));
    m_LabelUpgradeNeed->setPosition(Point(105.0f, strenBk->getContentSize().height * 0.5f - 40.0f));
    m_LabelUpgradeNeed->setTextColor(Color4B(0, 159, 225, -1));
    strenBk->addChild(m_LabelUpgradeNeed, GRADE_UI);
    
    //升级还需经验设置值
    m_LabelUpgradeVal= Label::create("", "WRYH.ttf", 40);
    m_LabelUpgradeVal->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_LabelUpgradeVal->setAnchorPoint(Vec2(0, 0));
    m_LabelUpgradeVal->setPosition(Point(380.0f, strenBk->getContentSize().height * 0.5f - 40.0f));
    m_LabelUpgradeVal->setTextColor(Color4B(0, 159, 225, -1));
    strenBk->addChild(m_LabelUpgradeVal, GRADE_UI);
    
    //满级还需经验标签
    m_LabelFullNeed = Label::create("满级所需经验", "WRYH.ttf", 40);
    m_LabelFullNeed->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_LabelFullNeed->setAnchorPoint(Vec2(0, 0));
    m_LabelFullNeed->setPosition(Point(105.0f, strenBk->getContentSize().height * 0.5f - 90.0f));
    m_LabelFullNeed->setTextColor(Color4B(0, 159, 225, -1));
    strenBk->addChild(m_LabelFullNeed, GRADE_UI);
    
    //满级还需经验设置值
    m_LabelFullVal = Label::create("", "WRYH.ttf", 40);
    m_LabelFullVal->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_LabelFullVal->setAnchorPoint(Vec2(0, 0));
    m_LabelFullVal->setPosition(Point(380.0f, strenBk->getContentSize().height * 0.5f - 90.0f));
    m_LabelFullVal->setTextColor(Color4B(0, 159, 225, -1));
    strenBk->addChild(m_LabelFullVal, GRADE_UI);

    m_BtnChooseMeterial[MATERIAL_ICON_1] = CButton::create("", Point::ZERO, "UI/Depot/chooseEquipBtn.png", "UI/Depot/chooseEquipBtn.png", "",
                                           CC_CALLBACK_1(DepotScene::menuAddMaterial, this));
    m_BtnChooseMeterial[MATERIAL_ICON_1]->setScale(0.8);
    m_BtnChooseMeterial[MATERIAL_ICON_1]->setPosition(Vec2(strenBk->getContentSize().width * 0.5f - 420.0f, 400.0f));
    strenBk->addChild(m_BtnChooseMeterial[MATERIAL_ICON_1], GRADE_UI);
    
    m_BtnChooseMeterial[MATERIAL_ICON_2] = CButton::create("", Point::ZERO, "UI/Depot/chooseEquipBtn.png", "UI/Depot/chooseEquipBtn.png", "",
                                           CC_CALLBACK_1(DepotScene::menuAddMaterial, this));
    m_BtnChooseMeterial[MATERIAL_ICON_2]->setScale(0.8);
    m_BtnChooseMeterial[MATERIAL_ICON_2]->setPosition(Vec2(strenBk->getContentSize().width * 0.5f - 140.0f, 400.0f));
    strenBk->addChild(m_BtnChooseMeterial[MATERIAL_ICON_2], GRADE_UI);
    
    m_BtnChooseMeterial[MATERIAL_ICON_3] = CButton::create("", Point::ZERO, "UI/Depot/chooseEquipBtn.png", "UI/Depot/chooseEquipBtn.png", "",
                                           CC_CALLBACK_1(DepotScene::menuAddMaterial, this));
    m_BtnChooseMeterial[MATERIAL_ICON_3]->setScale(0.8);
    m_BtnChooseMeterial[MATERIAL_ICON_3]->setPosition(Vec2(strenBk->getContentSize().width * 0.5f + 140.0f, 400.0f));
    strenBk->addChild(m_BtnChooseMeterial[MATERIAL_ICON_3], GRADE_UI);
    
    m_BtnChooseMeterial[MATERIAL_ICON_4] = CButton::create("", Point::ZERO, "UI/Depot/chooseEquipBtn.png", "UI/Depot/chooseEquipBtn.png", "",
                                           CC_CALLBACK_1(DepotScene::menuAddMaterial, this));
    m_BtnChooseMeterial[MATERIAL_ICON_4]->setScale(0.8);
    m_BtnChooseMeterial[MATERIAL_ICON_4]->setPosition(Vec2(strenBk->getContentSize().width * 0.5f + 420.0f, 400.0f));
    strenBk->addChild(m_BtnChooseMeterial[MATERIAL_ICON_4], GRADE_UI);
    
    m_NodeMeterialIcon[MATERIAL_ICON_1] = NULL;
    m_NodeMeterialIcon[MATERIAL_ICON_2] = NULL;
    m_NodeMeterialIcon[MATERIAL_ICON_3] = NULL;
    m_NodeMeterialIcon[MATERIAL_ICON_4] = NULL;
    
    //关闭按钮
    CButton * BtnBack = CButton::create("", Point::ZERO,
                                        "UI/Public/Button/backBtn.png", "UI/Public/Button/backPressBtn.png", "",
                                        CC_CALLBACK_1(DepotScene::menuCloseStrenCallback, this));
    BtnBack->setPosition(Vec2(strenBk->getContentSize().width - 115.0f, strenBk->getContentSize().height - 95.0f));
    strenBk->addChild(BtnBack, GRADE_UI);
    
    //强化按钮
    m_BtnStren = CButton::create("强 化", Point::ZERO,"UI/Public/Button/saleBtn.png", "UI/Public/Button/salePressBtn.png", "",
                                 CC_CALLBACK_1(DepotScene::menuConfirmLeveUp, this));
    m_BtnStren->setPosition(Vec2(strenBk->getContentSize().width - 300.0f, 150.0f));
    strenBk->addChild(m_BtnStren, GRADE_UI);
    
    m_LabelEquipInfo = Label::create("", "WRYH.ttf", 50);
    m_LabelEquipInfo->setAnchorPoint(Point(0, 0.5f));
    m_LabelEquipInfo->setDimensions(1100, 150);
    m_LabelEquipInfo->setPosition(Point(60, strenBk->getContentSize().height - 120.0f));
    m_LabelEquipInfo->setTextColor(Color4B(0, 159, 225, -1));
    strenBk->addChild(m_LabelEquipInfo, GRADE_UI);
    
    //名称
    m_LabelEquipName = Label::create("", "WRYH.ttf", 50);
    m_LabelEquipName->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_LabelEquipName->setAnchorPoint(Vec2(0, 0));
    m_LabelEquipName->setPosition(Point(395.0f, strenBk->getContentSize().height - 390.0f));
    m_LabelEquipName->setTextColor(Color4B(0, 159, 225, -1));
    strenBk->addChild(m_LabelEquipName, GRADE_UI);
    
    m_LabelEquipNameVal = Label::create("", "WRYH.ttf", 50);
    m_LabelEquipNameVal->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_LabelEquipNameVal->setAnchorPoint(Vec2(0, 0));
    m_LabelEquipNameVal->setPosition(Point(strenBk->getContentSize().width - 720.0f, strenBk->getContentSize().height - 390.0f));
    strenBk->addChild(m_LabelEquipNameVal, GRADE_UI);

    //等级
    m_LabelEquipLevel = Label::create("", "WRYH.ttf", 50);
    m_LabelEquipLevel->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_LabelEquipLevel->setAnchorPoint(Vec2(0, 0));
    m_LabelEquipLevel->setPosition(Point(395.0f, strenBk->getContentSize().height - 470.0f));
    m_LabelEquipLevel->setTextColor(Color4B(0, 159, 225, -1));
    strenBk->addChild(m_LabelEquipLevel, GRADE_UI);
    
    m_LabelEquipLevelVal = Label::create("", "WRYH.ttf", 50);
    m_LabelEquipLevelVal->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_LabelEquipLevelVal->setAnchorPoint(Vec2(0, 0));
    m_LabelEquipLevelVal->setPosition(Point(strenBk->getContentSize().width - 720.0f, strenBk->getContentSize().height - 470.0f));
    strenBk->addChild(m_LabelEquipLevelVal, GRADE_UI);
    
    //属性
    m_LabelEquipAttr = Label::create("", "WRYH.ttf", 50);
    m_LabelEquipAttr->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_LabelEquipAttr->setAnchorPoint(Vec2(0, 0));
    m_LabelEquipAttr->setPosition(Point(395.0f, strenBk->getContentSize().height - 550.0f));
    m_LabelEquipAttr->setTextColor(Color4B(0, 159, 225, -1));
    strenBk->addChild(m_LabelEquipAttr, GRADE_UI);
    
    m_LabelEquipAttrVal = Label::create("", "WRYH.ttf", 50);
    m_LabelEquipAttrVal->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_LabelEquipAttrVal->setAnchorPoint(Vec2(0, 0));
    m_LabelEquipAttrVal->setPosition(Point(strenBk->getContentSize().width - 720.0f, strenBk->getContentSize().height - 550.0f));
    strenBk->addChild(m_LabelEquipAttrVal, GRADE_UI);
}

void DepotScene::showPlane() {
    //展示玩家战机
    SEquip equip;
    g_pEquipManager->getGrooveEquip(EQUIP_TYPE_PLANE, equip);
    
    Plane * pPlane = Plane::create(equip);
    showPlane(pPlane);
    g_pEquipManager->getGrooveEquip(EQUIP_TYPE_WEAPON, equip);
    pPlane->addWeapon(equip);
    g_pEquipManager->getGrooveEquip(EQUIP_TYPE_WINGMAN, equip);
    pPlane->addWingman(equip);
    g_pEventEngine->BundlerCall(BUND_ID_PLANE_FIRE, this, sizeof(this));
    pPlane->showtime();
}

void DepotScene::onEnter() {
    PublicUI::onEnter();
    BIND(BUND_ID_BULLET_FIRED, this, DepotScene::bulletFired);
    showPlane();
}

void DepotScene::onExit() {
    PublicUI::onExit();
    UNBIND(BUND_ID_BULLET_FIRED, this, DepotScene::bulletFired);
}

void DepotScene::setGrooveIcon(const s8 type, Node * node) {
    static Node * pNode = NULL;
    if (pNode != NULL) {
        m_BtnEquip[type]->removeChild(pNode);
    }
    
    if (node != NULL) {
        pNode = node;
        pNode->setPosition(Vec2(158, 153));
        m_BtnEquip[type]->addChild(pNode);
    }
}

void DepotScene::setForce(const s32 force) {
    m_LabelShowForce->setString(IntAsString(force));
}

//展示玩家
void DepotScene::setUserEquip(Node * node)  {
    node->setPosition(Vec2(m_SpriteShowPlane->getContentSize().width * 0.5f, m_SpriteShowPlane->getContentSize().height * 0.5f - 60));
    node->setScale(1.5f);
    m_SpriteShowPlane->addChild(node);
}

void DepotScene::setEquipInfo(const string & info) {
    m_LabelEquipInfo->setString(info);
}

void DepotScene::setEquipName(const string & name) {
    m_LabelEquipName->setString(name);
}

void DepotScene::setEquipNameVal(const string & val) {
    m_LabelEquipNameVal->setString(val);
}

void DepotScene::setEquipLevel(const string & level) {
    m_LabelEquipLevel->setString(level);
}

void DepotScene::setEquipLevelVal(const string & val) {
    m_LabelEquipLevelVal->setString(val);
}

void DepotScene::setEquipAttr(const string & attr) {
    m_LabelEquipAttr->setString(attr);
}

void DepotScene::setEquipAttrVal(const string & val) {
    m_LabelEquipAttrVal->setString(val);
}

void DepotScene::setUpgradeNeed(const string & str) {
    m_LabelUpgradeNeed->setString(str);
}

void DepotScene::setUpgradeVal(const string & val) {
    m_LabelUpgradeVal->setString(val);
}

void DepotScene::setFullNeed(const string & str) {
    m_LabelFullNeed->setString(str);
}

void DepotScene::setFullVal(const string & val) {
    m_LabelFullVal->setString(val);
}

void DepotScene::setExp(const s32 nowExp, const s32 maxExp) {
    s32 percentage = nowExp * 100/maxExp;
    m_ExpBar->setPercentage(percentage);
}

void DepotScene::hideStrenEquipUI() {
    unmask();
    this->removeChild(strenBk);
}

void DepotScene::setEquipIcon(Node * node) {
    static Node * pNode = NULL;
    if (pNode != NULL) {
        m_BtnChooseEquip->removeChild(pNode);
    }
    
    if (node != NULL) {
        pNode = node;
        pNode->setPosition(Vec2(m_BtnChooseEquip->getContentSize().width * 0.5f,
                               m_BtnChooseEquip->getContentSize().height * 0.5f));
        m_BtnChooseEquip->addChild(pNode);
    }
}

void DepotScene::setMeterialIcon(const s32 index, Node * node) {
    if (m_NodeMeterialIcon[index] != NULL) {
        m_BtnChooseMeterial[index]->removeChild(m_NodeMeterialIcon[index]);
    }
    if (node != NULL) {
        m_NodeMeterialIcon[index] = node;
        m_NodeMeterialIcon[index]->setPosition(Vec2(m_BtnChooseMeterial[index]->getContentSize().width * 0.5f,
                               m_BtnChooseMeterial[index]->getContentSize().height * 0.5f));
        m_BtnChooseMeterial[index]->addChild(m_NodeMeterialIcon[index]);
    }
}

void DepotScene::cleanMeterialIcon() {
    m_BtnChooseMeterial[MATERIAL_ICON_1]->removeChild(m_NodeMeterialIcon[MATERIAL_ICON_1]);
    m_BtnChooseMeterial[MATERIAL_ICON_2]->removeChild(m_NodeMeterialIcon[MATERIAL_ICON_2]);
    m_BtnChooseMeterial[MATERIAL_ICON_3]->removeChild(m_NodeMeterialIcon[MATERIAL_ICON_3]);
    m_BtnChooseMeterial[MATERIAL_ICON_4]->removeChild(m_NodeMeterialIcon[MATERIAL_ICON_4]);
}

//展示玩家战机
void DepotScene::showPlane(Node * node)  {
    static Node * pNode = NULL;
    if (pNode != NULL) {
        removeChild(pNode);
    }
    pNode = node;
    node->setScale(g_pGameConfig->scaleEleMin);
    node->setPosition(Vec2(g_pGameConfig->screenCenterPoint.x, g_pGameConfig->screenCenterPoint.y - 10.0f));
    this->addChild(node, GRADE_PLAYER);
}
