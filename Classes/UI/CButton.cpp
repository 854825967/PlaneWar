#include "CButton.h"
#include "MultiSys.h"
USING_NS_CC;

CButton * CButton::create(const string & text,
                          const cocos2d::Point & point,
                          const string & normalImage,
                          const string & selectedImage,
                          const string & disabledImage,
                          const cocos2d::ccMenuCallback & callback) {
    CButton * pRet = new CButton();
    if ( pRet && pRet->init(text, point, normalImage, selectedImage, disabledImage, callback) ) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CButton::CButton() {
    m_touchListener = EventListenerTouchOneByOne::create();
    m_touchListener->setSwallowTouches(true);
    m_touchListener->onTouchBegan = CC_CALLBACK_2(CButton::onTouchBegan, this);
    m_touchListener->onTouchMoved = CC_CALLBACK_2(CButton::onTouchMoved, this);
    m_touchListener->onTouchEnded = CC_CALLBACK_2(CButton::onTouchEnded, this);
    m_touchListener->onTouchCancelled = CC_CALLBACK_2(CButton::onTouchCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_touchListener, this);
}

CButton::~CButton() {
    _eventDispatcher->removeEventListener(m_touchListener);
}

void CButton::setCallback(const cocos2d::ccMenuCallback & callback) {
    m_callback = callback;
}

bool CButton::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) {
    Point location = convertToNodeSpace(touch->getLocation());
    Rect rect = Rect(0, 0, getContentSize().width, getContentSize().height);
    if (!rect.containsPoint(location)){
        return false;
    }

    if (m_normal) {
        m_normal->setVisible(false);
    }
    
    if (m_selected) {
        m_selected->setVisible(true);
    }
    
    return true;
}

void CButton::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) {
    Point location = convertToNodeSpace(touch->getLocation());
    Rect rect = Rect(0, 0, getContentSize().width, getContentSize().height);
    if (!rect.containsPoint(location)){
        onTouchCancelled(touch, event);
    } else {
        if (m_normal) {
            m_normal->setVisible(true);
        }
        
        if (m_selected) {
            m_selected->setVisible(false);
        }
        if (m_callback) {
            m_callback(this);
        }
    }
}

void CButton::onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event) {
    if (m_normal) {
        m_normal->setVisible(true);
    }
    
    if (m_selected) {
        m_selected->setVisible(false);
    }
}

void CButton::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event * event) {

}

bool CButton::init(const string & text,
                 const cocos2d::Point & point,
                 const string & normalImage,
                 const string & selectedImage,
                 const string & disabledImage,
                 const cocos2d::ccMenuCallback & callback) {
    
    if (!Node::init()) {
        return false;
    }
    
    if (normalImage.size() > 0) {
        m_normal = Sprite::create(normalImage);
        m_normal->setAnchorPoint(Vec2(0, 0));
        m_normal->setVisible(true);
        setContentSize(m_normal->getContentSize());
        addChild(m_normal, 0);
    }
    
    if (selectedImage.size() > 0) {
        m_selected = Sprite::create(selectedImage);
        m_selected->setAnchorPoint(Vec2(0, 0));
        m_selected->setVisible(false);
        addChild(m_selected, 0);
    }
    
    if (disabledImage.size() > 0) {
        m_disabled = Sprite::create(disabledImage);
        m_disabled->setAnchorPoint(Vec2(0, 0));
        m_disabled->setVisible(false);
        addChild(m_disabled, 0);
    }
    
    m_label = Label::create(text, "WRYH.ttf", 40);
    m_label->setAnchorPoint(Vec2(.5f, .5f));
    m_label->setPosition(Vec2(getContentSize().width/2.0f + point.x, getContentSize().height/2.0f + point.y));
    addChild(m_label, 1);
    setAnchorPoint(Vec2(.5f, .5f));
    
    m_callback = callback;
    return true;
}
