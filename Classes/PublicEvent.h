#ifndef PUBLICEVENT_H
#define PUBLICEVENT_H

#include "Header.h"
#include "LoginScene.h"
#include "MainMenuScene.h"
#include "DepotScene.h"

class PublicEvent {
public:
    static void init() {
        static bool init = false;
        if (!init) {
            RGS_EVENT(EVENT_REPLACE_SCENE_ACTION, PublicEvent::replaceScene);
            init = true;
        }
    }
    
private:
    static void replaceScene(void * pData, const s32 size) {
        s32 nSceneID = *(s32 *)(&pData);
        switch (nSceneID) {
            case SCENE_ID_LOGIN:
                Director::getInstance()->replaceScene(Login::createScene());
                break;
            case SCENE_ID_MAINMENU:
                Director::getInstance()->replaceScene(MainMenu::createScene());
                break;
            case SCENE_ID_DEPOT:
                Director::getInstance()->replaceScene(DepotScene::createScene());
                break;
            default:
                CCASSERT(false, "check ui scene id");
                break;
        }
    }
};

#endif //PUBLICEVENT_H
