#include "TextButton.h"
#include "Header.h"
#include "Data/GameConfig.h"
USING_NS_CC;

TextButton * TextButton::create(const char * text,
                                const cocos2d::Point & point,
                                const char  * normalImage,
                                const char  * selectedImage,
                                const char * disabledImage,
                                const cocos2d::ccMenuCallback & callback){
    
    TextButton * pRet = NEW TextButton(text, point, normalImage, selectedImage, disabledImage, callback);
    if (pRet) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}


TextButton::TextButton(const char * text,
                       const cocos2d::Point & point,
                       const char * normalImage,
                       const char * selectedImage,
                       const char * disabledImage,
                       const cocos2d::ccMenuCallback & callback)
: CButton(normalImage, selectedImage, disabledImage, callback) {
    LabelTTF * label = LabelTTF::create(text, "黑体", 40);
    label->setAnchorPoint(ccp(.5f, .5f));
    label->setPosition(point);
    m_menuItem->addChild(label);
    
    setAnchorPoint(ccp(.5f, .5f));
}
