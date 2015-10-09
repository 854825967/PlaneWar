#ifndef CEQUIPVIEW_H
#define CEQUIPVIEW_H

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "Struct.h"

class CButton;

enum {
    VIEW_TYPE_REPLACE_EQUIP,
    VIEW_TYPE_SELL_EQUIP,
    VIEW_TYPE_SELECT_LEVEUP_EQUIP,
    VIEW_TYPE_SELECT_MATERIAL
};

class CEquipView : public cocos2d::Node, public cocos2d::extension::TableViewDataSource, public cocos2d::extension::TableViewDelegate {
public:
    static CEquipView * create();
    
    void setMultiSelected(bool b);
    void setTitle(const string & title);
    void setValue(const string & value);
    void loadEquipsList(const EQUIP_VECTOR & vector);
    void setCanSelect(bool b);
    /**
     * Delegate to respond touch event
     *
     * @param table table contains the given cell
     * @param cell  cell that is touched
     * @js NA
     * @lua NA
     */
    virtual void tableCellTouched(cocos2d::extension::TableView * table, cocos2d::extension::TableViewCell * cell);
    /**
     * a cell instance at a given index
     *
     * @param idx index to search for a cell
     * @return cell found at idx
     */
    virtual cocos2d::extension::TableViewCell * tableCellAtIndex(cocos2d::extension::TableView *table, ssize_t idx);
    /**
     * Returns number of cells in a given table view.
     *
     * @return number of cells
     */
    virtual ssize_t numberOfCellsInTableView(cocos2d::extension::TableView * table);
    
    cocos2d::Size tableCellSizeForIndex(cocos2d::extension::TableView * table, ssize_t idx);
    void setCloseCallback(const cocos2d::ccMenuCallback & callback);
    void setCancelCallBack(const cocos2d::ccMenuCallback & callback);
    void setconfirmCallBack(const cocos2d::ccMenuCallback & callback);
    
    P_EQUIP_LIST p_equip_list;
    s8 type;
private:
    CEquipView();
    
    bool init();
    
private:
    cocos2d::extension::TableView * m_pTableView;
    EQUIP_VECTOR m_data;
    CButton * m_pClose;
    bool m_multiSelect;
    cocos2d::ccMenuCallback m_closeback;
    CButton * m_pConfirm;
    CButton * m_pCancel;
    Label * m_pLabelEquipType;
    Sprite * m_pEquipNameBk;
    Label * m_pLabelValue;
    bool m_CanSelect;
};

#endif // CEQUIPVIEW_H
