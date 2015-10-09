#include "DepotScene.h"
#include "Header.h"
#include "GameConfig.h"
#include "EquipManager.h"
#include "MainMenuScene.h"
#include "CEquipView.h"
#include "CButton.h"

DepotScene::DepotScene() {
    ASSERT(s_pSelf ==  NULL);
    s_pSelf = this;
    BIND(BUND_ID_EQUIP_REPLACE_EQUIP_RES, this, DepotScene::OnCmdReplaceEquip);
    BIND(BUND_ID_EQUIP_SELL_RES, this, DepotScene::OnCmdSellEquipsRes);
    BIND(BUND_ID_VIEW_CELL_TOUCHED, this, DepotScene::OnCmdViewCellTouched);
    BIND(BUND_ID_LEVEL_UP_EQUIP, this, DepotScene::OnCmdLevelUpEquipRes);
}

DepotScene::~DepotScene() {
    s_pSelf = NULL;
    UNBIND(BUND_ID_EQUIP_REPLACE_EQUIP_RES, this, DepotScene::OnCmdReplaceEquip);
    UNBIND(BUND_ID_EQUIP_SELL_RES, this, DepotScene::OnCmdSellEquipsRes);
    UNBIND(BUND_ID_VIEW_CELL_TOUCHED, this, DepotScene::OnCmdViewCellTouched);
    UNBIND(BUND_ID_LEVEL_UP_EQUIP, this, DepotScene::OnCmdLevelUpEquipRes);
}

void DepotScene::menuReturnMainMenu(cocos2d::Ref *pSender) {
    Director::getInstance()->replaceScene(MainMenu::createScene());
}

void DepotScene::menuSellCallback(Ref * pSender) {
    mask();
    m_EquipView = CEquipView::create();
    m_EquipView->setPosition(g_pGameConfig->screenCenterPoint);
    m_EquipView->setTitle("出售");
    addChild(m_EquipView, GRADE_UI);
    EQUIP_VECTOR equips;
    g_pEquipManager->queryEquips(equips, EQUIP_TYPE_ALL);
    
    EQUIP_VECTOR::iterator itor = equips.begin();
    EQUIP_VECTOR::iterator iend = equips.end();
    while (iend != itor && !equips.empty()) {
        if ((*itor).isequiped == IS_EQUIPED) {
            itor = equips.erase(itor);
            iend = equips.end();
        } else {
            itor ++;
        }
    }
    
    m_EquipView->loadEquipsList(equips);
    m_EquipView->type = VIEW_TYPE_SELL_EQUIP;
    m_EquipView->setMultiSelected(true);
    m_EquipView->setCloseCallback(CC_CALLBACK_1(DepotScene::menuCloseView, this));
    m_EquipView->setCancelCallBack(CC_CALLBACK_1(DepotScene::menuCloseView, this));
    m_EquipView->setconfirmCallBack(CC_CALLBACK_1(DepotScene::menuSellEquips, this));
}

void DepotScene::menuStrenCallbakc(Ref * pSender) {
    m_levelup_equip.id = -1;
    m_materials.clear();
    initStrenEquipUI();
}

void DepotScene::menuCloseStrenCallback(cocos2d::Ref *pSender) {
    m_levelup_equip.id = -1;
    m_materials.clear();
    hideStrenEquipUI();
}

void DepotScene::OnCmdReplaceEquip(void * pContext, const s32 size) {
    menuCloseView(NULL);
    waitOver();
    s32 nCode = *(s32 *)pContext;
    if (ERROR_CODE::ERROR_CODE_NO_ERROR == nCode) {
        initEquipIcon();
        showPlane();
        recountForce();
    } else {
        string msg = string("更换装备出错, 错误代码:") + IntAsString(nCode);
        alert(msg.c_str());
    }
}

void DepotScene::OnCmdViewCellTouched(void *pContext, const s32 size) {
    CCASSERT(size == sizeof(s8), "check view type");
    SEquip * pEquip = NULL;
    if (!m_EquipView->p_equip_list.empty()) {
        pEquip = *(m_EquipView->p_equip_list.begin());
    }
    switch (*(s8 *)pContext) {
        case VIEW_TYPE_REPLACE_EQUIP:
        {
            EQUIP_LIST equips;
            g_pEquipManager->getGrooveEquips(equips);
            s32 nCap = 0;
            for (EQUIP_LIST::iterator itor = equips.begin(); itor != equips.end(); itor ++) {
                if (pEquip == NULL || (*itor).type != pEquip->type) {
                    nCap += g_pEquipManager->countCapacity(*itor);
                }
            }
            
            if (pEquip != NULL) {
                nCap += g_pEquipManager->countCapacity(*pEquip);
            }
            string text = "总战力:";
            text += IntAsString(nCap);
            m_EquipView->setValue(text);
            break;
        }
        case VIEW_TYPE_SELL_EQUIP:
        {
            s32 nCash = 0;
            for (P_EQUIP_LIST::iterator itor = m_EquipView->p_equip_list.begin();
                 itor != m_EquipView->p_equip_list.end();
                 itor ++) {
                nCash += (*itor)->level * 2 + 40;
            }
            
            string text = "总价格:";
            text += IntAsString(nCash);
            m_EquipView->setValue(text);
            break;
        }
        case VIEW_TYPE_SELECT_MATERIAL:
        {
            if (m_EquipView->p_equip_list.size() < 4) {
                m_EquipView->setCanSelect(true);
            } else {
                m_EquipView->setCanSelect(false);
            }
        }
        default:
            break;
    }
}

void DepotScene::OnCmdSellEquipsRes(void *pContext, const s32 size) {
    menuCloseView(NULL);
    waitOver();
    s32 nCode = *(s32 *)pContext;
    if (ERROR_CODE::ERROR_CODE_NO_ERROR == nCode) {
        //initEquipIcon();
    } else {
        ALERT("出售装备出错, 错误代码:%d", nCode);
    }
}

void DepotScene::OnCmdLevelUpEquipRes(void *pContext, const s32 size) {
    waitOver();
    s32 nCode = *(s32 *)pContext;
    if (ERROR_CODE::ERROR_CODE_NO_ERROR == nCode) {
        ALERT("战机升级成功");
        cleanMeterialIcon();
        if (g_pEquipManager->queryEquip(m_levelup_equip.id, m_levelup_equip)) {
            switch (m_levelup_equip.type) {
                case EQUIP_TYPE_PLANE:
                {
                    const PlaneConfig * pConfig = g_pGameConfig->getPlaneConfig(m_levelup_equip.model);
                    setEquipNameVal(pConfig->name);
                    setEquipLevelVal(IntAsString(m_levelup_equip.level));
                    setEquipAttr("攻击 ");
                    setEquipAttrVal(IntAsString(g_pEquipManager->countCapacity(m_levelup_equip)));
                    break;
                }
                case EQUIP_TYPE_WEAPON:
                {
                    const WeaponConfig * pConfig = g_pGameConfig->getWeaponConfig(m_levelup_equip.model);
                    setEquipNameVal(pConfig->name);
                    setEquipLevelVal(IntAsString(m_levelup_equip.level));
                    setEquipAttr("攻击 ");
                    setEquipAttrVal(IntAsString(g_pEquipManager->countCapacity(m_levelup_equip)));
                    break;
                }
                case EQUIP_TYPE_WINGMAN:
                {
                    const WingmanConfig * pConfig = g_pGameConfig->getWingmanConfig(m_levelup_equip.model);
                    setEquipNameVal(pConfig->name);
                    setEquipLevelVal(IntAsString(m_levelup_equip.level));
                    setEquipAttr("攻击 ");
                    setEquipAttrVal(IntAsString(g_pEquipManager->countCapacity(m_levelup_equip)));
                    break;
                }
                case EQUIP_TYPE_ARMOR:
                {
                    const ArmorConfig * pConfig = g_pGameConfig->getArmorConfig(m_levelup_equip.model);
                    setEquipNameVal(pConfig->name);
                    setEquipLevelVal(IntAsString(m_levelup_equip.level));
                    setEquipAttr("护甲 ");
                    setEquipAttrVal(IntAsString(g_pEquipManager->countCapacity(m_levelup_equip)));
                    break;
                }
            }
            
            s32 expForLevelUp = g_pEquipManager->levelExp(m_levelup_equip.level + 1) - m_levelup_equip.exp;
            setUpgradeVal(IntAsString(expForLevelUp));
            setExp(m_levelup_equip.exp - g_pEquipManager->levelExp(m_levelup_equip.level),
                   g_pEquipManager->levelExp(m_levelup_equip.level + 1) - g_pEquipManager->levelExp(m_levelup_equip.level));
            
            recountForce();
        } else {
            CCASSERT(false, "我擦类~~~~,升级一次装备就消失啦~~~扯啦~~~~");
        }
    } else {
        ALERT("升级装备出错, 错误代码:%d", nCode);
    }
}

void DepotScene::menuSelectedLevelupEquip(cocos2d::Ref *pSender) {
    if (m_EquipView->p_equip_list.size() != 1) {
        CCASSERT(0 == m_EquipView->p_equip_list.size(), "check code about equip view");
    } else {
        m_levelup_equip = *(*m_EquipView->p_equip_list.begin());
        Sprite * icon = g_pGameConfig->getIcon(m_levelup_equip.model, m_levelup_equip.type);
        setEquipName("名称 ");
        setEquipLevel("等级 ");
        switch (m_levelup_equip.type) {
            case EQUIP_TYPE_PLANE:
            {
                const PlaneConfig * pConfig = g_pGameConfig->getPlaneConfig(m_levelup_equip.model);
                setEquipNameVal(pConfig->name);
                setEquipLevelVal(IntAsString(m_levelup_equip.level));
                setEquipAttr("攻击 ");
                setEquipAttrVal(IntAsString(g_pEquipManager->countCapacity(m_levelup_equip)));
                break;
            }
            case EQUIP_TYPE_WEAPON:
            {
                const WeaponConfig * pConfig = g_pGameConfig->getWeaponConfig(m_levelup_equip.model);
                setEquipNameVal(pConfig->name);
                setEquipLevelVal(IntAsString(m_levelup_equip.level));
                setEquipAttr("攻击 ");
                setEquipAttrVal(IntAsString(g_pEquipManager->countCapacity(m_levelup_equip)));
                break;
            }
            case EQUIP_TYPE_WINGMAN:
            {
                const WingmanConfig * pConfig = g_pGameConfig->getWingmanConfig(m_levelup_equip.model);
                setEquipNameVal(pConfig->name);
                setEquipLevelVal(IntAsString(m_levelup_equip.level));
                setEquipAttr("攻击 ");
                setEquipAttrVal(IntAsString(g_pEquipManager->countCapacity(m_levelup_equip)));
                break;
            }
            case EQUIP_TYPE_ARMOR:
            {
                const ArmorConfig * pConfig = g_pGameConfig->getArmorConfig(m_levelup_equip.model);
                setEquipNameVal(pConfig->name);
                setEquipLevelVal(IntAsString(m_levelup_equip.level));
                setEquipAttr("护甲 ");
                setEquipAttrVal(IntAsString(g_pEquipManager->countCapacity(m_levelup_equip)));
                break;
            }
        }
        
        s32 expForLevelUp = g_pEquipManager->levelExp(m_levelup_equip.level + 1) - m_levelup_equip.exp;
        setUpgradeVal(IntAsString(expForLevelUp));
        setExp(m_levelup_equip.exp - g_pEquipManager->levelExp(m_levelup_equip.level),
               g_pEquipManager->levelExp(m_levelup_equip.level + 1) - g_pEquipManager->levelExp(m_levelup_equip.level));
        setEquipIcon(icon);
    }
    menuCloseView(NULL);
}

void DepotScene::menuSelectMaterial(cocos2d::Ref * pSender) {
    m_materials.clear();
    cleanMeterialIcon();
    if (!m_EquipView->p_equip_list.empty()) {
        s32 index = 0;
        for (P_EQUIP_LIST::iterator itor=m_EquipView->p_equip_list.begin();
             itor != m_EquipView->p_equip_list.end(); itor++) {
            m_materials << (*itor)->id;
            Sprite * icon = g_pGameConfig->getIcon((*itor)->model, (*itor)->type);
            setMeterialIcon(index++, icon);
        }
    }
    menuCloseView(NULL);
}

void DepotScene::menuReplaceEquip(cocos2d::Ref * pSender) {
    if (m_EquipView->p_equip_list.size() == 1) {
        SEquip * pEquip = *m_EquipView->p_equip_list.begin();
        g_pEquipManager->replaceEquip(pEquip->id, pEquip->type);
        waiting();
    } else {
        menuCloseView(NULL);
    }
}

void DepotScene::menuSellEquips(cocos2d::Ref * pSender) {
    if (!m_EquipView->p_equip_list.empty()) {
        ID_ARRAY vector;
        P_EQUIP_LIST::iterator itor = m_EquipView->p_equip_list.begin();
        P_EQUIP_LIST::iterator iend = m_EquipView->p_equip_list.end();
        while (itor != iend) {
            CCASSERT((*itor)->isequiped != IS_EQUIPED, "check equip manager");
            vector << (*itor)->id;
            itor++;
        }
        
        g_pEquipManager->sellEquips(vector);
        waiting();
    } else {
        menuCloseView(NULL);
    }
}

void DepotScene::menuCloseView(Ref * pSender) {
    unmask();
    removeChild(m_EquipView);
    m_EquipView = NULL;
}

void DepotScene::menuGroovesCall(Ref * pSender) {
    mask();
    string title;
    s8 type = ((Node*)pSender)->getTag();
    switch (type) {
        case EQUIP_TYPE_PLANE:
        {
            title += "战机";
            break;
        }
        case EQUIP_TYPE_ARMOR:
        {
            title += "护甲";
            break;
        }
        case EQUIP_TYPE_WEAPON:
        {
            title += "武器";
            break;
        }
        case EQUIP_TYPE_WINGMAN:
        {
            title += "僚机";
            break;
        }
        default:
            break;
    }
    
    m_EquipView = CEquipView::create();
    m_EquipView->setPosition(g_pGameConfig->screenCenterPoint);
    addChild(m_EquipView, GRADE_UI);
    m_EquipView->setTitle(title);
    EQUIP_VECTOR equips;
    g_pEquipManager->queryEquips(equips, type);
    
    EQUIP_VECTOR::iterator itor = equips.begin();
    EQUIP_VECTOR::iterator iend = equips.end();
    
    while (iend != itor) {
        if ((*itor).isequiped == IS_EQUIPED) {
            itor = equips.erase(itor);
            iend = equips.end();
        } else {
            itor++;
        }
    }
    
    m_EquipView->type = VIEW_TYPE_REPLACE_EQUIP;
    m_EquipView->loadEquipsList(equips);
    m_EquipView->setMultiSelected(false);
    m_EquipView->setCloseCallback(CC_CALLBACK_1(DepotScene::menuCloseView, this));
    m_EquipView->setCancelCallBack(CC_CALLBACK_1(DepotScene::menuCloseView, this));
    m_EquipView->setconfirmCallBack(CC_CALLBACK_1(DepotScene::menuReplaceEquip, this));
}

//点击强化装备图标 弹出选择框
void DepotScene::menuAddStrenEquip(cocos2d::Ref * pSender) {
    mask();
    m_EquipView = CEquipView::create();
    m_EquipView->setPosition(g_pGameConfig->screenCenterPoint);
    addChild(m_EquipView, GRADE_UI);
    m_EquipView->setTitle("选择装备");
    EQUIP_VECTOR equips;
    g_pEquipManager->queryEquips(equips, EQUIP_TYPE_ALL);
    
    EQUIP_VECTOR::iterator itor = equips.begin();
    EQUIP_VECTOR::iterator iend = equips.end();
    
    while (iend != itor) {
        if ((*itor).id == m_levelup_equip.id) {
            (*itor).isselected = true;
        }
        itor ++;
    }
    m_EquipView->type = VIEW_TYPE_SELECT_LEVEUP_EQUIP;
    m_EquipView->loadEquipsList(equips);
    m_EquipView->setMultiSelected(false);
    m_EquipView->setCloseCallback(CC_CALLBACK_1(DepotScene::menuCloseView, this));
    m_EquipView->setCancelCallBack(CC_CALLBACK_1(DepotScene::menuCloseView, this));
    m_EquipView->setconfirmCallBack(CC_CALLBACK_1(DepotScene::menuSelectedLevelupEquip, this));
}

//点击素材选择按钮 弹出选择框
void DepotScene::menuAddMaterial(cocos2d::Ref * pSender) {
    if (m_levelup_equip.id == -1) {
        ALERT("请选择要升级的装备");
        return;
    }
    
    mask();
    m_EquipView = CEquipView::create();
    m_EquipView->setPosition(g_pGameConfig->screenCenterPoint);
    addChild(m_EquipView, GRADE_UI);
    m_EquipView->setTitle("选择素材");
    EQUIP_VECTOR equips;
    g_pEquipManager->queryEquips(equips, EQUIP_TYPE_ALL);
    
    EQUIP_VECTOR::iterator itor = equips.begin();
    EQUIP_VECTOR::iterator iend = equips.end();
    
    while (iend != itor) {
        if ((*itor).isequiped == IS_EQUIPED || (*itor).id == m_levelup_equip.id) {
            itor = equips.erase(itor);
            iend = equips.end();
        } else {
            for(s32 i=0; i<m_materials.size(); i++) {
                if ((*itor).id == m_materials[i]) {
                    (*itor).isselected = true;
                }
            }
            itor ++;
        }
    }
    m_EquipView->type = VIEW_TYPE_SELECT_MATERIAL;
    m_EquipView->loadEquipsList(equips);
    m_EquipView->setMultiSelected(true);
    m_EquipView->setCloseCallback(CC_CALLBACK_1(DepotScene::menuCloseView, this));
    m_EquipView->setCancelCallBack(CC_CALLBACK_1(DepotScene::menuCloseView, this));
    m_EquipView->setconfirmCallBack(CC_CALLBACK_1(DepotScene::menuSelectMaterial, this));
}

//点击确定按钮 升级装备
void DepotScene::menuConfirmLeveUp(cocos2d::Ref * pSender) {
    if (m_levelup_equip.id == -1) {
        ALERT("请选择要升级的装备");
        return;
    }
    
    if (0 == m_materials.size()) {
        ALERT("请选择升级所需素材");
        return;
    }
    
    LOG("=======升级装备 id:%d=======", m_levelup_equip.id);
    for (s32 i=0; i<m_materials.size(); i++) {
        LOG("=====升级装备 素材 id:%d=====", m_materials[i]);
    }
    LOG("========升级装备   over========");
    
    waiting();
    g_pEquipManager->levelupEquip(m_levelup_equip, m_materials);
}
