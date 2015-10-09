#ifndef CBOX_H
#define CBOX_H
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "Struct.h"

class CBox : public cocos2d::Node {
public:
    static CBox * create();
    ~CBox();
    
    void setSelect(const bool b);
    void select();
    void setIcon(cocos2d::Node * icon);
    void setLabel1(const string & text, const cocos2d::Color4B & color = Color4B(255, 255, 255, 100));
    void setLabel2(const string & text, const cocos2d::Color4B & color = Color4B(255, 255, 255, 100));
    void setEuipName(const char * name);
    void setLv(const char * level);
    bool isSelected();
    
    void OnCmdUnSelected(void * pContext, const s32 size);
protected:
    CBox();

protected:
    cocos2d::Sprite * m_pPanel; //栏背景
    cocos2d::Sprite * m_pLabelBkg; //标签背景
    cocos2d::Sprite * m_pSelectIcon;
    
    cocos2d::Label * m_pLabelName;
    cocos2d::Label * m_pLv;
    cocos2d::Label * m_pLabel1;
    cocos2d::Label * m_pLabel2;
    cocos2d::Node * m_pIcon;
    
    bool m_isSelected;
};

#endif //CBOX_H
