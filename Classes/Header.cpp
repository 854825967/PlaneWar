#include "Header.h"
#include "tinyxml/tinyxml.h"
USING_NS_CC;

string g_uid = "";
string g_token = "";

string g_name = "";
s32 g_exp = 0;
s32 g_money = 0;
s32 g_dimond = 0;
s32 g_power = 0;
s32 g_vip = 0;


tlib::TLogger<1> g_log;

namespace diyfun {
    float getAngle(const cocos2d::Point & a, const cocos2d::Point & b) {
        float angle;
        int t;
        double len_y = b.y - a.y;
        double len_x = b.x - a.x;

        (len_x<0)?(t = -1):(t = 1);
        angle = acos(len_y/(sqrt(len_x * len_x+len_y * len_y)));

        return t * angle/M_PI * 180;
    }

    float getDistance(const cocos2d::Point & a, const cocos2d::Point & b) {
        return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
    }

    cocos2d::Point getWorldPosition(const cocos2d::Point & fatherPoint, const cocos2d::Point & sonPoint, const float fatherRotate) {
        Point target;
        float radian;
        radian = -fatherRotate / 180 * M_PI;
        target.x = sonPoint.x * cos(radian) - sonPoint.y * sin(radian);
        target.y = sonPoint.x * sin(radian) + sonPoint.y * cos(radian);

        return fatherPoint + target;
    }
    
    bool loadXml(TiXmlDocument & xml, const string & path) {
#ifdef __ANDROID__
        long size;
        char * pFileContent = (char *)FileUtils::getInstance()->getFileData( path.c_str() , "r", &size);
        if (NULL == xml.Parse(pFileContent, 0, TIXML_ENCODING_UTF8)) {
            CCASSERT(false, path.c_str());
            return false;
        }
        return true;
#else
        return xml.LoadFile(path.c_str());
#endif
    }
}
