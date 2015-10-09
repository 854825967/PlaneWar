#include "PublicUI.h"
#include "CButton.h"
#include "GameConfig.h"
#include "CHttpEngine.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;

tlib::CSelectModel g_tcpMgr;

bool PublicUI::init() {
	if ( !Layer::init() ){
		return false;
	}
    setTouchEnabled(true);
    static bool rgs = false;
    if (!rgs) {
        RGS_EVENT(EVENT_DATA_DIRTY, PublicUI::eventDirtydata);
        rgs = true;
    }
    
	return true;
}

void PublicUI::onEnter() {
    Layer::onEnter();
    schedule(schedule_selector(PublicUI::flush), 0.33f);
}

void PublicUI::onExit() {
    Layer::onExit();
    unschedule(schedule_selector(PublicUI::flush));
}

void PublicUI::touchover(Node * node) {
    removeChild(node);
}

void PublicUI::playActionOver(Armature * armature, MovementEventType type, const char * name) {
    if (COMPLETE == type) {
        removeChild(armature);
    }
}

void PublicUI::addTouch(const Point &point) {
    ArmatureDataManager::getInstance()->addArmatureFileInfo("Animation/UI/touch.ExportJson");
    Armature * touch = Armature::create("touch");
    touch->getAnimation()->play("touch");
    touch->setPosition(point);
    touch->getAnimation()->setMovementEventCallFunc(this, movementEvent_selector(PublicUI::playActionOver));
    addChild(touch, GRADE_MAX);
}

bool PublicUI::onGlobalTouchBegan(Touch *touch, Event * event) {
    Point point = touch->getLocation();
    addTouch(point);
    g_pEventEngine->BundlerCall(BUND_ID_GLOBAL_TOUCH_BEGAN, &point, sizeof(point));
    return true;
}

void PublicUI::onGlobalTouchEnded(Touch *touch, Event * event) {
    Point point = touch->getLocation();
    ECHO("touch local %f, %f", point.x, point.y);
    g_pEventEngine->BundlerCall(BUND_ID_GLOBAL_TOUCH_ENDED, &point, sizeof(point));
}

void PublicUI::onGlobalTouchCancelled(Touch *touch, Event * event) {
    Point point = touch->getLocation();
    g_pEventEngine->BundlerCall(BUND_ID_GLOBAL_TOUCH_CANCELED, &point, sizeof(point));
}

void PublicUI::onGlobalTouchMoved(Touch *touch, Event * event) {
    Point point = touch->getLocation();
    g_pEventEngine->BundlerCall(BUND_ID_GLOBAL_TOUCH_MOVED, &point, sizeof(point));
}

PublicUI::PublicUI() {
    m_listener = EventListenerTouchOneByOne::create();
    m_listener->setSwallowTouches(false);
    m_listener->onTouchBegan = CC_CALLBACK_2(PublicUI::onGlobalTouchBegan, this);
    m_listener->onTouchMoved = CC_CALLBACK_2(PublicUI::onGlobalTouchMoved, this);
    m_listener->onTouchEnded = CC_CALLBACK_2(PublicUI::onGlobalTouchEnded, this);
    m_listener->onTouchCancelled = CC_CALLBACK_2(PublicUI::onGlobalTouchCancelled, this);
    _eventDispatcher->addEventListenerWithFixedPriority(m_listener, -138);
}

PublicUI::~PublicUI() {
    _eventDispatcher->removeEventListener(m_listener);
    
    MASK_INFO_LIST::iterator itor = m_maskInfoList.begin();
    while (itor != m_maskInfoList.end()) {
        _eventDispatcher->removeEventListener(itor->touchListener);
        
        itor++;
    }
}

void PublicUI::flush(float) {
    g_pEventEngine->ReflushBundler();
    g_pHttpEngine->HttpBackCall(15);
    //g_tcpMgr.CLoop(15);
}


bool PublicUI::onTouchBegan(Touch *touch, Event * event) {
    return true;
}

void PublicUI::onTouchEnded(Touch *touch, Event * event) {
    
}

void PublicUI::onTouchCancelled(Touch *touch, Event * event) {
    
}

void PublicUI::onTouchMoved(Touch *touch, Event * event) {
    
}

void PublicUI::mask() {
    //半透明层
    LayerColor * maskLayerColor = LayerColor::create();
    maskLayerColor->setContentSize(g_pGameConfig->screenSize);
    maskLayerColor->setColor(ccc3(0, 0, 0));
    maskLayerColor->setOpacity(150);
    addChild(maskLayerColor, GRADE_UI);
    
    EventListenerTouchOneByOne * listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(PublicUI::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(PublicUI::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(PublicUI::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(PublicUI::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, maskLayerColor);
    
    CMask mask;
    mask.maskLayerColor = maskLayerColor;
    mask.touchListener = listener;
    m_maskInfoList.push_front(mask);
}

void PublicUI::unmask() {
    MASK_INFO_LIST::iterator itor = m_maskInfoList.begin();
    if (itor != m_maskInfoList.end()) {
        _eventDispatcher->removeEventListener(itor->touchListener);
        removeChild(itor->maskLayerColor);
        m_maskInfoList.erase(itor);
    }
}

void PublicUI::waiting() {
    mask();
	//加载等待动画
	ArmatureDataManager::getInstance()->addArmatureFileInfo("Animation/UI/waiting.ExportJson");
	Armature * loadingArmature = Armature::create("waiting");
	loadingArmature->getAnimation()->playByIndex(0);
	loadingArmature->setPosition(g_pGameConfig->screenCenterPoint);
    loadingArmature->setScale(g_pGameConfig->scaleEleMin);
    m_maskInfoList.begin()->maskLayerColor->addChild(loadingArmature);
}

void PublicUI::waitOver() {
    unmask();
}

////弹出框(警告)
void PublicUI::alert(const char * info, const s32 eventid){
    mask();
    
 	//弹出框背景图片
 	auto bk = Sprite::create("UI/Public/Panel/confirmPanel.png");
 	bk->setPosition(Vec2(g_pGameConfig->screenSize.width/2, g_pGameConfig->screenSize.height/2));
 	bk->setScale(g_pGameConfig->scaleEleX);
    m_maskInfoList.begin()->maskLayerColor->addChild(bk, GRADE_UI);


    //警告信息
    auto alertLabel = Label::create(info, "WRYH.ttf", 60);
    alertLabel->setPosition(Point(bk->getContentSize().width * 0.5,  bk->getContentSize().height * 0.75));
    bk->addChild(alertLabel, GRADE_UI);
    
    CButton * alertButton = CButton::create("CONFIRM", Vec2(0,0), "UI/Public/Button/confirmBtn.png",
                                           "UI/Public/Button/confirmPressBtn.png", "", CC_CALLBACK_1(PublicUI::buttonAlert, this));
    m_EventIDQueue.add(eventid);
    alertButton->setPosition(Vec2(bk->getContentSize().width * 0.5,  bk->getContentSize().height * 0.25));
    bk->addChild(alertButton, GRADE_UI);
}

void PublicUI::buttonAlert(Ref * sender) {
    unmask();
    
    s32 id = -1;
    if (m_EventIDQueue.read(id)) {
        g_pEventEngine->BundlerCall(id, NULL, 0);
    }
}

void PublicUI::eventDirtydata(void *, const s32) {
    //alert("本地数据与服务器不同步, 请重新登录", EVENT_REPLACE_SCENE_ACTION);
}
