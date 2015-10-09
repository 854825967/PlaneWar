#include "CEquipView.h"
#include "CBox.h"
#include "EquipManager.h"
#include "GameConfig.h"
#include "Header.h"
#include "Tools.h"
#include "CButton.h"

USING_NS_CC;
USING_NS_CC_EXT;

CEquipView * CEquipView::create() {
    CEquipView * pRet = NEW CEquipView();
    
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
}

bool CEquipView::init() {
    setContentSize(Size(1320.0f * g_pGameConfig->scaleEleX, 225.0f * 6 * g_pGameConfig->scaleEleX));
    
    Sprite * backimg = Sprite::create("UI/Public/Panel/equipPanel.png");
    backimg->setScale(g_pGameConfig->scaleEleX);
    backimg->setPosition(Vec2(getContentSize().width/2.0f, getContentSize().height/2.0f - 40.0f * g_pGameConfig->scaleEleX));
    addChild(backimg);
    //setContentSize(backimg->getContentSize());
    setAnchorPoint(Vec2(.5f, .5f));
    
    m_pClose = CButton::create("", Point::ZERO, "UI/Public/Button/backBtn.png", "UI/Public/Button/backPressBtn.png", "");
    m_pClose->setScale(g_pGameConfig->scaleEleX);
    m_pClose->setPosition(Vec2(1250.0f * g_pGameConfig->scaleEleX, 1400.0f * g_pGameConfig->scaleEleX));
    addChild(m_pClose);
    
    m_pTableView = TableView::create(this, Size(1320.0f * g_pGameConfig->scaleEleX, 220.0f * 6 * g_pGameConfig->scaleEleX));
    m_pTableView->setAnchorPoint(Vec2(.5f, .5f));
    m_pTableView->setDirection(ScrollView::Direction::VERTICAL);
    m_pTableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
    m_pTableView->setDelegate(this);
    addChild(m_pTableView);
    m_pTableView->reloadData();
    
    m_pConfirm = CButton::create("确认", Point::ZERO, "UI/Public/Button/saleBtn.png", "UI/Public/Button/salePressBtn.png", "");
    m_pConfirm->setScale(g_pGameConfig->scaleEleX);
    m_pConfirm->setPosition(Vec2(1195.0f * g_pGameConfig->scaleEleX, -100.0f * g_pGameConfig->scaleEleX));
    addChild(m_pConfirm);

    m_pCancel = CButton::create("取消", Point::ZERO, "UI/Public/Button/saleBtn.png", "UI/Public/Button/salePressBtn.png", "");
    m_pCancel->setScale(g_pGameConfig->scaleEleX);
    m_pCancel->setPosition(Vec2(935.0f * g_pGameConfig->scaleEleX, -100.0f * g_pGameConfig->scaleEleX));
    addChild(m_pCancel);
    
    m_pEquipNameBk = Sprite::create("UI/Public/Button/chooseBtn.png");
    m_pEquipNameBk->setScale(g_pGameConfig->scaleEleX);
    m_pEquipNameBk->setPosition(Point(165.0f * g_pGameConfig->scaleEleX, getContentSize().height + 35.0f * g_pGameConfig->scaleEleX));
    addChild(m_pEquipNameBk);
    
    m_pLabelValue = Label::create("", "WRYH.ttf", 50);
    m_pLabelValue->setAnchorPoint(Point(0, 0.5f));
    m_pLabelValue->setPosition(Point(30.0f * g_pGameConfig->scaleEleX, - 100.0f * g_pGameConfig->scaleEleX));
    m_pLabelValue->setScale(g_pGameConfig->scaleEleX);
    addChild(m_pLabelValue);
    
    m_pLabelEquipType = Label::create("", "WRYH.ttf", 50);
    m_pLabelEquipType->setAnchorPoint(Point(0.5f, 0.5f));
    m_pLabelEquipType->setPosition(Point(m_pEquipNameBk->getContentSize().width/2.0f, m_pEquipNameBk->getContentSize().height/2.0f));
    m_pEquipNameBk->addChild(m_pLabelEquipType);

    m_CanSelect = true;
    return true;
}

void CEquipView::setCloseCallback(const cocos2d::ccMenuCallback &callback) {
    m_pClose->setCallback(callback);
}

void CEquipView::setCancelCallBack(const cocos2d::ccMenuCallback & callback) {
    m_pCancel->setCallback(callback);
}

void CEquipView::setconfirmCallBack(const cocos2d::ccMenuCallback & callback) {
    m_pConfirm->setCallback(callback);
}

void CEquipView::setMultiSelected(bool b) {
    m_multiSelect = b;
}

CEquipView::CEquipView() {
    m_multiSelect = false;
}

void CEquipView::setCanSelect(bool b) {
    m_CanSelect = b;
}

Size CEquipView::tableCellSizeForIndex(TableView *table, ssize_t idx) {
    return Size(1300.0f * g_pGameConfig->scaleEleX, 210.0f * g_pGameConfig->scaleEleX);
}

void CEquipView::tableCellTouched(TableView * table, TableViewCell * cell) {
    CBox * box = (CBox *)cell->getChildByTag(100);
    s32 idx = cell->getIdx();
    g_pEventEngine->BundlerCall(BUND_ID_VIEW_CELL_TOUCHED, &type, sizeof(type));
    SEquip * pEquip = &m_data[idx];
    if (box->isSelected()) {
        if (!m_multiSelect) {
            return;
        }
        P_EQUIP_LIST::iterator iend = p_equip_list.end();
        P_EQUIP_LIST::iterator itor = std::find(p_equip_list.begin(), iend, pEquip);
        if (itor != iend) {
            p_equip_list.erase(itor);
        }
    } else {
        if (!m_CanSelect) {
            return;
        }
        if (m_multiSelect) {
            P_EQUIP_LIST::iterator iend = p_equip_list.end();
            P_EQUIP_LIST::iterator itor = std::find(p_equip_list.begin(), iend, pEquip);
            if (itor == iend) {
                p_equip_list.push_back(pEquip);
            }
        } else {
            g_pEventEngine->BundlerCall(BUND_ID_CBOX_UNSELECTED, NULL, 0);
            p_equip_list.clear();
            p_equip_list.push_back(pEquip);
            for (s32 i=0; i<m_data.size(); i++) {
                m_data[i].isselected = false;
            }
        }
    }
    
    box->select();
    m_data[idx].isselected = box->isSelected();
}

void CEquipView::loadEquipsList(const EQUIP_VECTOR &vector) {
    m_data.clear();
    p_equip_list.clear();
    
    EQUIP_VECTOR::const_iterator itor = vector.cbegin();
    EQUIP_VECTOR::const_iterator iend = vector.cend();
    while (itor != iend) {
        m_data.push_back(*itor);
        
        itor++;
    }
    
    for (s32 i=0; i<m_data.size(); i++) {
        if (m_data[i].isselected) {
            p_equip_list.push_back(&m_data[i]);
        }
    }
    
    m_pTableView->reloadData();
}

TableViewCell * CEquipView::tableCellAtIndex(cocos2d::extension::TableView * table, ssize_t idx) {
    TableViewCell * cell = table->dequeueCell();
    if (!cell) {
        cell = TableViewCell::create();
    } else {
        Node * box = cell->getChildByTag(100);
        CCASSERT(box != NULL, "box point null, fuck u");
        cell->removeChild(box);
    }
    
    CBox * box = CBox::create();
    box->setScale(g_pGameConfig->scaleEleX);
    box->setEuipName(m_data[idx].model.c_str());
    string iconPath;
    string name;
    switch (m_data[idx].type) {
        case EQUIP_TYPE_PLANE:
        {
            const PlaneConfig * pConfig = g_pGameConfig->getPlaneConfig(m_data[idx].model);
            CCASSERT(pConfig != NULL, "check ur res, fuck u");
            iconPath += "Icons/" + pConfig->icon;
            name = pConfig->name;
            break;
        }
        case EQUIP_TYPE_ARMOR:
        {
            const ArmorConfig * pConfig = g_pGameConfig->getArmorConfig(m_data[idx].model);
            CCASSERT(pConfig != NULL, "check ur res, fuck u");
            iconPath += "Icons/" + pConfig->icon;
            name = pConfig->name;
            break;
        }
        case EQUIP_TYPE_WEAPON:
        {
            const WeaponConfig * pConfig = g_pGameConfig->getWeaponConfig(m_data[idx].model);
            CCASSERT(pConfig != NULL, "check ur res, fuck u");
            iconPath += "Icons/" + pConfig->icon;
            name = pConfig->name;
            break;
        }
        case EQUIP_TYPE_WINGMAN:
        {
            const WingmanConfig * pConfig = g_pGameConfig->getWingmanConfig(m_data[idx].model);
            CCASSERT(pConfig != NULL, "check ur res, fuck u");
            iconPath += "Icons/" + pConfig->icon;
            name = pConfig->name;
            break;
        }
        default:
        {
            CCASSERT(false, "check ur equip type, fuck u");
            break;
        }
    }
    
    Sprite * icon = Sprite::create(iconPath);
    box->setIcon(icon);
    box->setEuipName(name.c_str());
    box->setLv(IntAsString(m_data[idx].level).c_str());
    box->setTag(100);
    string text = "";
    switch (type) {
        case VIEW_TYPE_REPLACE_EQUIP:
        case VIEW_TYPE_SELECT_LEVEUP_EQUIP:
            text += "战斗力:" + IntAsString(g_pEquipManager->countCapacity(m_data[idx]));
            break;
        case VIEW_TYPE_SELECT_MATERIAL:
            text += "提供经验:" + IntAsString(g_pEquipManager->provideExp(m_data[idx].level));
            break;
        default:
            break;
    }
    
    string desc;
    if (m_data[idx].isequiped) {
        desc += " 已装备";
    }
    if (m_data[idx].islocked) {
        desc += " 已锁定";
    }
    box->setLabel2(desc, Color4B(4, 141, 48, -1));
    box->setLabel1(text, Color4B(255, 187, 0, -1));
    cell->addChild(box);
    box->setSelect(m_data[idx].isselected);
    
    return cell;
}

ssize_t CEquipView::numberOfCellsInTableView(cocos2d::extension::TableView *table) {
    return m_data.size();
}

void CEquipView::setTitle(const string & title) {
    m_pLabelEquipType->setString(title);

}

void CEquipView::setValue(const string & value) {
    m_pLabelValue->setString(value);
}
