#ifndef CButton_H
#define CButton_H
#include "cocos2d.h"
#include <string>
using namespace std;

class CButton : public cocos2d::Node {
public:
    static CButton * create(const string & text,
                            const cocos2d::Point & point,
                            const string &  normalImage,
                            const string &  selectedImage,
                            const string &  disabledImage,
                            const cocos2d::ccMenuCallback & callback = NULL);
    
    void setCallback(const cocos2d::ccMenuCallback & callback);
    
    ~CButton();
    
protected:
    CButton();
    
    bool init(const string & text,
              const cocos2d::Point & point,
              const string &  normalImage,
              const string &  selectedImage,
              const string &  disabledImage,
              const cocos2d::ccMenuCallback & callback);
    
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event);
    virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event * event);
    virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event * event);
    virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event * event);
    
protected:
    cocos2d::Label * m_label;
    cocos2d::Node * m_normal;
    cocos2d::Node * m_selected;
    cocos2d::Node * m_disabled;
    cocos2d::ccMenuCallback m_callback;
    cocos2d::EventListenerTouchOneByOne * m_touchListener;
};

#endif //CButton_H
