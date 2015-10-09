////////////////////////////////////////////////////////////////////
//                          _ooOoo_                               //
//                         o8888888o                              //
//                         88" . "88                              //
//                         (| ^_^ |)                              //
//                         O\ = /O                                //
//                      ____/`---'\____                           //
//                    .'  \\|     |//  `.                         //
//                   /  \\|||  :  |||//  \                        //
//                  /  _||||| -:- |||||-  \                       //
//                  |   | \\\  -  /// |   |                       //
//                  | \_|  ''\---/''  |   |                       //
//                  \  .-\__  `-`  ___/-. /                       //
//                ___`. .'  /--.--\  `. . ___                     //
//              .""' < `.___\_<|>_/___.' > '"".                   //
//            | | :  `- \`.;`\ _ /`;.`/ - ` : | |                 //
//            \  \ `-.   \_ __\ /__ _/   .-` /  /                 //
// == == == ==  `-.____`-.___\_____/___.-`____.-'   == == == ==   //
//                           ` = --- = '                          //
//      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^        //
//             佛祖保佑       无BUG       永不修改                   //
////////////////////////////////////////////////////////////////////
#ifndef HEADER_H
#define HEADER_H
#include "TLogger.h"
#include "Collider.h"
#include "cocostudio/CocoStudio.h"
using namespace cocostudio;


#define LOGIN_DATA_VALIDITY ((s64)30 * 24 * 60 * 60 * 1000)
#define BASE_SCREEN_WIDTH 640
#define BASE_SCREEN_HEIGHT 960
#define MAX_SCREEN_WIDTH 1536
#define MAX_SCREEN_HEIGHT 2048
#define RANDOM_POSITION 10000

extern string g_uid;
extern string g_token;

extern string g_name;
extern s32 g_exp;
extern s32 g_money;
extern s32 g_dimond;
extern s32 g_power;
extern s32 g_vip;

extern tlib::TLogger<1> g_log;

#define LOG(format, a...) {\
    char log[4096] = {0}; \
    SafeSprintf(log, 4096, format, ##a); \
    ECHO("%s\n", log); \
    g_log.Log(0, log); \
}

#define LOAD_ANIMATION(path, name) { \
    std::string armatrue = path; \
    armatrue +=  name; \
    ArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo( \
        (armatrue + ".png").c_str(), (armatrue + ".plist").c_str(), (armatrue + ".ExportJson").c_str() \
    ); \
}

class TiXmlDocument;

namespace diyfun {
    float getAngle(const cocos2d::Point & a, const cocos2d::Point & b);
    float getDistance(const cocos2d::Point & a, const cocos2d::Point & b);
    cocos2d::Point getWorldPosition(const cocos2d::Point & fatherPoint, const cocos2d::Point & sonPoint, const float fatherRotate);
    bool loadXml(TiXmlDocument & xml, const string & path);
}

#endif //HEADER_H
