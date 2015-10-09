#ifndef __DEPOT_SCENE_H__
#define __DEPOT_SCENE_H__
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include <list>
#include "PublicUI.h"
#include "Struct.h"

class CEquipView;
class CButton;

enum {
    MATERIAL_ICON_1,
    MATERIAL_ICON_2,
    MATERIAL_ICON_3,
    MATERIAL_ICON_4,
    
    //ADD BEFOR THIS
    MATERIAL_ICON_COUNT
};

class DepotScene : public PublicUI {
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene *  createScene();

private:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref *  pSender);
    void menuSellCallback(cocos2d::Ref * pSender);
    void menuStrenCallbakc(cocos2d::Ref * pSender);
    void menuReturnMainMenu(cocos2d::Ref * pSender);
    void menuCloseStrenCallback(cocos2d::Ref * pSender);
    
    //点击强化装备图标 弹出选择框
    void menuAddStrenEquip(cocos2d::Ref * pSender);
    
    //点击素材选择按钮 弹出选择框
    void menuAddMaterial(cocos2d::Ref * pSender);
    
    //点击确定按钮 升级装备
    void menuConfirmLeveUp(cocos2d::Ref * pSender);
    
    void menuGroovesCall(cocos2d::Ref * pSender);
    void menuCloseView(cocos2d::Ref * pSender);
    void menuReplaceEquip(cocos2d::Ref * pSender);
    void menuSellEquips(cocos2d::Ref * pSender);
    void menuSelectedLevelupEquip(cocos2d::Ref * pSender);
    void menuSelectMaterial(cocos2d::Ref * pSender);
    
    void OnCmdReplaceEquip(void * pContext, const s32 size);
    void OnCmdSellEquipsRes(void * pContext, const s32 size);
    void OnCmdViewCellTouched(void * pContext, const s32 size);
    void OnCmdLevelUpEquipRes(void * pContext, const s32 size);
    
	// implement the "static create()" method manually
	CREATE_FUNC(DepotScene);
    
    virtual void onEnter();
    virtual void onExit();
    
private:
	DepotScene();
	~DepotScene();
    void initButtonUI();
    void initUserInfoUI();
    void initOtherUI();
    void initEquipIcon();
    void initStrenEquipUI();
    void hideStrenEquipUI();
    
    void recountForce();
    
    void setGrooveIcon(const s8 type, Node * node);
    void setForce(const s32 force);
    void setUserEquip(Node * node);
    void setEquipInfo(const string & info);
    void setEquipName(const string & name);
    void setEquipNameVal(const string & val);
    void setEquipLevel(const string & level);
    void setEquipLevelVal(const string & val);
    void setEquipAttr(const string & attr);
    void setEquipAttrVal(const string & val);
    void setUpgradeNeed(const string & str);
    void setFullNeed(const string & str);
    void setUpgradeVal(const string & val);
    void setFullVal(const string & val);
    void setExp(const s32 nowExp, const s32 maxExp);
    
    void setEquipIcon(Node * node);
    void cleanMeterialIcon();
    void setMeterialIcon(const s32 index, Node * node);
    
    void showPlane();
    
    //展示玩家战机
    void showPlane(Node * node);
    void bulletFired(void * context, const s32 size);

private:
	static DepotScene * s_pSelf;
	cocos2d::Sprite * m_pBackground;
	Label *  m_LabelTitle;
    Label * m_LabelForce;
    Label * m_LabelShowForce;
    
    Sprite * m_SpriteShowPlane;
    CEquipView * m_EquipView;
    CButton * m_BtnMission;
    CButton * m_BtnLadder;
    CButton * m_BtnSell;
    CButton * m_BtnFriend;
    CButton * m_BtnEquip[EQUIP_TYPE_CAN_EQUIPED];
    
    Sprite * strenBk;
    Label *  m_LabelEquipName;
    Label *  m_LabelEquipNameVal;
    Label * m_LabelEquipLevel;
    Label * m_LabelEquipLevelVal;
    Label * m_LabelEquipAttr;
    Label * m_LabelEquipAttrVal;
    Label * m_LabelEquipInfo;
    CButton * m_BtnChooseEquip;
    CButton * m_BtnStren;
    CButton * m_BtnChooseMeterial[MATERIAL_ICON_COUNT];
    Node * m_NodeMeterialIcon[MATERIAL_ICON_COUNT];

    Sprite * m_SpriteExpBarBk;
    Label *  m_LabelUpgradeNeed;
    Label *  m_LabelUpgradeVal;
    Label * m_LabelFullNeed;
    Label * m_LabelFullVal;
    ProgressTimer *  m_ExpBar;
    
private:
    SEquip m_levelup_equip;
    ID_ARRAY m_materials;
};

#endif // __BATTLEGROUND_SCENE_H__
