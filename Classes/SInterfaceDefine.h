#ifndef _SINTERFACEDEFINE_H_
#define _SINTERFACEDEFINE_H_

namespace SERVER {
    static const char * login_url = "http://alax1987.oicp.net/index.php/auth/nlogin?uid=%s&token=%s";
    static const char * guest_url = "http://alax1987.oicp.net/index.php/tourist/tlogin?user=tourist";
    static const char * rapid_url = "http://alax1987.oicp.net/index.php/auth/qlogin?uid=%s&ptoken=%s";
    static const char * userinfo_url = "http://alax1987.oicp.net/index.php/uinfo/getUinfo?uid=%s&ptoken=%s";
    static const char * equipinfo_url = "http://alax1987.oicp.net/index.php/einfo/getEinfo?uid=%s&ptoken=%s";
    static const char * replaceequip_url = "http://alax1987.oicp.net/index.php/einfo/changeE?uid=%s&ptoken=%s&eid=%d";
    static const char * sellequips_url = "http://alax1987.oicp.net/index.php/sell/sellE?uid=%s&ptoken=%s&eids=%s";
    static const char * levelup_url = "http://alax1987.oicp.net/index.php/upgrade/upgradeE?uid=%s&ptoken=%s&eids=%s";
    static const char * countpve_url = "http://alax1987.oicp.net/index.php/count/countpve?uid=%s&ptoken=%s&gamelevel=%d";
};

enum {
    HTTP_REQUEST_ID_LOGIN = 0,  //正常登陆
    HTTP_REQUEST_ID_RAPID = 1,  //快速登陆
    HTTP_REQUEST_ID_USERINFO = 2,   //获取用户基本信息
    HTTP_REQUEST_ID_EQUIPINFO = 3,  //拉取装备列表
    HTTP_REQUEST_ID_REPLACEEQUIP = 4, //更换装备
    HTTP_REQUEST_ID_SELL = 5,
    HTTP_REQUEST_ID_LEVEL_UP = 6,
    HTTP_REQUEST_ID_COUNT_PVE = 7,
    HTTP_REQUEST_ID_GUEST = 8,
    //ADD BEFORE THIS
    HTTP_REQUEST_ID_COUNT
};

namespace ERROR_CODE {
    enum {
        ERROR_CODE_UNKNOWN = 0,
        ERROR_CODE_NO_ERROR = 1,
        ERROR_CODE_LOGINFAILED = 2,
        ERROR_CODE_SINA_USER_NOT_EXIST = 3,
        ERROR_CODE_TOKEN_ERROR = 4,
        ERROR_CODE_SERVER_ERROR_5 = 5,
        ERROR_CODE_REPLACE_EQUIP_ERROR = 6,
        ERROR_CODE_DIRTY_DATA = 7,
    };
};

#endif //_SINTERFACEDEFINE_H_
