#include "CBox.h"
#include "MultiSys.h"
#include "CEventEngine.h"
USING_NS_CC;

CBox * CBox::create() {
    CBox * pRet = NEW CBox();
    if (pRet) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CBox::~CBox() {
    UNBIND(BUND_ID_CBOX_UNSELECTED, this, CBox::OnCmdUnSelected);
}

void CBox::OnCmdUnSelected(void *pContext, const s32 size) {
    m_isSelected = false;
    m_pSelectIcon->setVisible(m_isSelected);
}

void CBox::setSelect(const bool b) {
    m_isSelected = b;
    m_pSelectIcon->setVisible(m_isSelected);
}

void CBox::select() {
    m_isSelected = !m_isSelected;
    m_pSelectIcon->setVisible(m_isSelected);
}

bool CBox::isSelected() {
    return m_isSelected;
}

CBox::CBox() {
    BIND(BUND_ID_CBOX_UNSELECTED, this, CBox::OnCmdUnSelected);
    
    m_pIcon = NULL;
    m_isSelected = false;
    
    //装备背景
    m_pPanel = Sprite::create("UI/Public/Panel/equipInfoPanel.png");
    m_pPanel->setAnchorPoint(Vec2(0, 0));
    addChild(m_pPanel);
    setContentSize(m_pPanel->getContentSize());
    
    //装备LV标签
    Label * labelLv = Label::create("LV.", "WRYH.ttf", 35);
    labelLv->setVerticalAlignment(TextVAlignment::BOTTOM);
    labelLv->setAnchorPoint(Vec2(0, 0));
    labelLv->setPosition(Vec2(800, 130));
    m_pPanel->addChild(labelLv);
    
    
    m_pLv = Label::create("", "WRYH.ttf", 40);
    m_pLv->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_pLv->setAnchorPoint(Vec2(0, 0));
    m_pLv->setPosition(Vec2(850, 130));
    m_pPanel->addChild(m_pLv);
    
    m_pLabelName = Label::create("", "WRYH.ttf", 45);
    m_pLabelName->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_pLabelName->setAnchorPoint(Vec2(0, 0));
    m_pLabelName->setPosition(Vec2(250, 125));
    m_pPanel->addChild(m_pLabelName);
    
    m_pLabel1 = Label::create("", "WRYH.ttf", 45);
    m_pLabel1->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_pLabel1->setAnchorPoint(Vec2(0, 0));
    m_pLabel1->setPosition(Vec2(250, 40));
    m_pPanel->addChild(m_pLabel1);
    
    m_pLabel2 = Label::create("", "WRYH.ttf", 45);
    m_pLabel2->setVerticalAlignment(TextVAlignment::BOTTOM);
    m_pLabel2->setAnchorPoint(Vec2(0, 0));
    m_pLabel2->setPosition(Point(670, 40));
    m_pPanel->addChild(m_pLabel2);
    
    m_pSelectIcon = Sprite::create("UI/Public/Button/selected.png");
    m_pSelectIcon->setAnchorPoint(Vec2(0, 0));
    m_pSelectIcon->setVisible(m_isSelected);
    m_pSelectIcon->setPosition(Point(1180, 70));
    m_pPanel->addChild(m_pSelectIcon);
}

void CBox::setIcon(cocos2d::Node *icon) {
    if (m_pIcon != NULL) {
        removeChild(m_pIcon);
    }
    
    m_pIcon = icon;
    m_pIcon->setAnchorPoint(Vec2(0, 0));
    m_pIcon->setPosition(Vec2(53, 44));
    m_pIcon->setScale(0.75);
    m_pPanel->addChild(m_pIcon);
}

void CBox::setLv(const char * name) {
    m_pLv->setString(name);
}

void CBox::setEuipName(const char * name) {
    //装备名称标签
    m_pLabelName->setString(name);
}

void CBox::setLabel1(const string & text, const Color4B & color) {
    m_pLabel1->setTextColor(color);
    m_pLabel1->setString(text);
}

void CBox::setLabel2(const string & text, const Color4B & color) {
    m_pLabel2->setTextColor(color);
    m_pLabel2->setString(text);
}
