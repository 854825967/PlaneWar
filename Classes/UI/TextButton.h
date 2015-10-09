#ifndef TEXTBUTTON_H
#define TEXTBUTTON_H
#include "CButton.h"

class TextButton : public CButton {
public:
    static TextButton * create(const char * text,
                               const cocos2d::Point & point,
                               const char  * normalImage,
                               const char  * selectedImage,
                               const char * disabledImage = NULL,
                               const cocos2d::ccMenuCallback & callback = NULL);
    
private:
    TextButton(const char * text,
               const cocos2d::Point & point,
               const char * normalImage,
               const char * selectedImage,
               const char * disabledImage,
               const cocos2d::ccMenuCallback & callback);

};


#endif //TEXTBUTTON_H
