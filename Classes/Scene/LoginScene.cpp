#include "LoginScene.h"
#include "CButton.h"
#include "SDK/SDK.h"
#include "Tools.h"
#include "SInterfaceDefine.h"
#include "json/json.h"
#include "MainMenuScene.h"
#include "Plane.h"
#include "GameConfig.h"
#include "DataManager.h"
#include "EquipManager.h"
#include "CEventEngine.h"
#include "CHttpEngine.h"
#include "Boss.h"
USING_NS_CC;

Login * Login::s_pSelf = NULL;

Login::Login() {
	ASSERT(s_pSelf ==  NULL);
	s_pSelf = this;
}

Login::~Login() {
	s_pSelf = NULL;
}

Scene *  Login::createScene() {
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = Login::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool Login::init() {
	//////////////////////////////
	// 1. super init first
	if (!PublicUI::init()) {
		return false;
	}
    
	m_pBackground = Sprite::create("UI/Login/loginBk.png");
	m_pBackground->setScale(g_pGameConfig->scaleEleMax);
	m_pBackground->setPosition(g_pGameConfig->screenCenterPoint);
	this->addChild(m_pBackground, GRADE_BACKGOUND);
    
    //登陆按钮
    m_BtnLogin = CButton::create("", Vec2(0,0), "UI/Login/loginBtn.png", "UI/Login/loginPress.png", "", CC_CALLBACK_1(Login::buttonLoginCallBack, this));
    m_BtnLogin->setScale(g_pGameConfig->scaleEleX);
    m_BtnLogin->setAnchorPoint(Vec2(.5f, .5f));
    m_BtnLogin->setPosition(Vec2(g_pGameConfig->screenCenterPoint.x, 300 * g_pGameConfig->scaleEleX));
    this->addChild(m_BtnLogin,GRADE_UI);
    
    
    //登陆按钮
    m_BtnTry = CButton::create("", Vec2(0,0), "UI/Login/guestBtn.png", "UI/Login/guestPressBtn.png", "", CC_CALLBACK_1(Login::buttonGuestCallBack, this));
    m_BtnTry->setScale(g_pGameConfig->scaleEleX);
    m_BtnTry->setAnchorPoint(Vec2(.5f, .5f));
    m_BtnTry->setPosition(Vec2(g_pGameConfig->screenCenterPoint.x, 500 * g_pGameConfig->scaleEleX));
    this->addChild(m_BtnTry,GRADE_UI);
    
    string uid, ptoken;
    if (DataManager::getInterface()->getUidPtoken(uid, ptoken)) {
        waiting();
        char szUrl[512] = {0};
        SafeSprintf(szUrl, sizeof(szUrl), SERVER::rapid_url, uid.c_str(), ptoken.c_str());
        g_pHttpEngine->HGetRequest(HTTP_REQUEST_ID_RAPID, szUrl, s_pSelf, NULL, 0);
    }
    
    //加载等待动画
//    ArmatureDataManager::getInstance()->addArmatureFileInfo("Animation/Enemy/enemy8.ExportJson");
//    Armature * loadingArmature = Armature::create("enemy8");
//    loadingArmature->getAnimation()->playByIndex(0);
//    loadingArmature->setPosition(Point(400,400));
//    loadingArmature->setScale(g_pGameConfig->scaleEleMin);
//    this->addChild(loadingArmature);
    

    
	return true;
}

s32 OnConnectionBreak(const s32 id, const void *, const s32) {
    
}

s32 OnRecv(const s32 id, const void *, const s32) {
    
}

s32 OnConnectFailed(const s32 id, const void *, const s32) {
    ECHO("connect failed");
}

s32 OnConnected(const s32 id, const void *, const s32) {
    s32 nSize = 14;
    s32 nMsgID = 1;
    g_tcpMgr.CSend(id, &nSize, sizeof(nSize));
    g_tcpMgr.CSend(id, &nMsgID, sizeof(nMsgID));
    g_tcpMgr.CSend(id, "hello", 6);
}

void Login::onEnter() {
    PublicUI::onEnter();
    BIND(BUND_ID_GUEST, this, Login::guest);
    
    g_tcpMgr.CSetCallFunAddr(NET_EVENT_CONNECTED, OnConnected);
    
    g_tcpMgr.CConnect("10.240.29.21", 12580, NULL, 0);
}

void Login::onExit() {
    PublicUI::onExit();
    UNBIND(BUND_ID_GUEST, this, Login::guest);
}

void Login::OnHttpRes(const s32 id, const Json::Value * root, const void  * context, const s32 size) {
    switch (id) {
        case HTTP_REQUEST_ID_LOGIN:
        {
            if (root ==  NULL) {
                waitOver();
                alert("登陆异常,请检查你的网络连接");
                return;
            }

            s32 nCode = (*root)["code"].asInt();
            if (ERROR_CODE::ERROR_CODE_NO_ERROR ==  nCode) {
                std::string uid = (*root)["uid"].asString();
                std::string ptoken = (*root)["ptoken"].asString();
                DataManager::getInterface()->saveUidPtoken(uid.c_str(), ptoken.c_str());
                g_pDataManager->initDB(uid.c_str());
                char szUrl[512] = {0};
                SafeSprintf(szUrl, sizeof(szUrl), SERVER::userinfo_url, g_uid.c_str(), g_token.c_str());
                g_pHttpEngine->HGetRequest(HTTP_REQUEST_ID_USERINFO, szUrl, s_pSelf, NULL, 0);
            } else {
                waitOver();
                ALERT("登录出错,错误码:%d", nCode);
            }
            break;
        }
        case HTTP_REQUEST_ID_GUEST:
        {
            if (root ==  NULL) {
                waitOver();
                alert("登陆异常,请检查你的网络连接");
                return;
            }
            
            s32 nCode = (*root)["code"].asInt();
            if (ERROR_CODE::ERROR_CODE_NO_ERROR ==  nCode) {
                std::string uid = (*root)["uid"].asString();
                std::string ptoken = (*root)["ptoken"].asString();
                DataManager::getInterface()->saveUidPtoken(uid.c_str(), ptoken.c_str());
                g_pDataManager->initDB(uid.c_str());
                char szUrl[512] = {0};
                SafeSprintf(szUrl, sizeof(szUrl), SERVER::userinfo_url, g_uid.c_str(), g_token.c_str());
                g_pHttpEngine->HGetRequest(HTTP_REQUEST_ID_USERINFO, szUrl, s_pSelf, NULL, 0);
            } else {
                waitOver();
                ALERT("登录出错,错误码:%d", nCode);
            }
            break;
        }
        case HTTP_REQUEST_ID_USERINFO:
        {
            if (NULL ==  root) {
                waitOver();
                alert("获取用户信息异常, 请从新登陆", EVENT_REPLACE_SCENE_ACTION);
                return;
            }
 
            s32 nCode = (*root)["code"].asInt();
            if (ERROR_CODE::ERROR_CODE_NO_ERROR ==  nCode) {
                g_name = (*root)["data"]["name"].asString();
                g_exp = StringAsInt((*root)["data"]["userexp"].asString().c_str());
                g_money = StringAsInt((*root)["data"]["money"].asString().c_str());
                g_dimond = StringAsInt((*root)["data"]["dimond"].asString().c_str());
                g_power = StringAsInt((*root)["data"]["power"].asString().c_str());
                g_vip = StringAsInt((*root)["data"]["vip"].asString().c_str());
                
                char szUrl[512] = {0};
                SafeSprintf(szUrl, sizeof(szUrl), SERVER::equipinfo_url, g_uid.c_str(), g_token.c_str());
                g_pHttpEngine->HGetRequest(HTTP_REQUEST_ID_EQUIPINFO, szUrl, s_pSelf, NULL, 0);
            } else {
                waitOver();
                alert("获取用户信息出错,请重新登陆");
            }
            
            break;
        }
        case HTTP_REQUEST_ID_EQUIPINFO:
        {
            if (NULL ==  root) {
                waitOver();
                alert("获取装备信息出错,请重新登陆");
                return;
            }

            s32 nCode = (*root)["code"].asInt();
            if (ERROR_CODE::ERROR_CODE_NO_ERROR == nCode) {
                s32 nCount = (*root)["count"].asInt();
                EQUIP_LIST equips;
                for (s32 i = 0; i<nCount; i++) {
                    SEquip equip;
                    equip.id = StringAsInt((*root)["equipments"][i]["equipid"].asString().c_str());
                    equip.isequiped = StringAsInt((*root)["equipments"][i]["status"].asString().c_str());
                    equip.type = StringAsInt((*root)["equipments"][i]["type"].asString().c_str());
                    equip.model = (*root)["equipments"][i]["modelname"].asString();
                    equip.exp = StringAsInt((*root)["equipments"][i]["exp"].asString().c_str());
                    equip.level = StringAsInt((*root)["equipments"][i]["level"].asString().c_str());
                    equips.push_back(equip);
                }
                
                g_pEquipManager->saveEquips(equips);
                Director::getInstance()->replaceScene(MainMenu::createScene());
            } else {
                waitOver();
                alert("获取装备信息出错,请重新登陆", EVENT_REPLACE_SCENE_ACTION);
            }
            break;
        }
        case HTTP_REQUEST_ID_RAPID:
        {
            if (root ==  NULL) {
                waitOver();
                alert("登陆异常,请检查你的网络连接");
                return;
            }
            
            s32 nCode = (*root)["code"].asInt();
            if (ERROR_CODE::ERROR_CODE_NO_ERROR ==  nCode) {
                g_pDataManager->initDB(g_uid.c_str());
                char szUrl[512] = {0};
                SafeSprintf(szUrl, sizeof(szUrl), SERVER::userinfo_url, g_uid.c_str(), g_token.c_str());
                g_pHttpEngine->HGetRequest(HTTP_REQUEST_ID_USERINFO, szUrl, s_pSelf, NULL, 0);
            } else {
                waitOver();
                alert("登陆权限过期,请重新登陆");
            }
            break;
        }
        default:
            break;
    }
}

void Login::OnLoginWeiboRes(const s32 nCode, const char * uid, const char  * acces_token) {
    if (0 ==  nCode) {
    	ECHO("OnLoginWeiboRes: %d, %s, %s", nCode, uid, acces_token);
        char szUrl[512] = {0};
        SafeSprintf(szUrl, sizeof(szUrl), SERVER::login_url, uid, acces_token);
        g_pHttpEngine->HGetRequest(HTTP_REQUEST_ID_LOGIN, szUrl, s_pSelf, NULL, 0);
    }
}

void Login::buttonGuestCallBack(Ref * sender) {
    alert("试玩用户资料会在\n删除客户端之后丢失", BUND_ID_GUEST);
}

void Login::guest(void * context, const s32 size) {
    s_pSelf->waiting();
    g_pHttpEngine->HGetRequest(HTTP_REQUEST_ID_GUEST, SERVER::guest_url, s_pSelf, NULL, 0);
}

void Login::buttonLoginCallBack(Ref * sender) {
#ifdef __ANDROID__
    alert("Android用户暂未开放");
#else
    s_pSelf->waiting();
    WeiboLogin();
#endif //__ANDROID__
}

void Login::dataLoaded(float percent) {

}

