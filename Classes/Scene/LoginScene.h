#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__
#include "cocos2d.h"
#include "IHttpRes.h"
#include "PublicUI.h"

class CButton;

class Login : public PublicUI, public IHttpRes {

public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene *  createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
    virtual void onEnter();
    virtual void onExit();

	// implement the "static create()" method manually
	CREATE_FUNC(Login);
    
    // a selector callback
    void buttonGuestCallBack(Ref * sender);
    void buttonLoginCallBack(Ref * sender);
    static void OnLoginWeiboRes(const s32 nCode, const char * uid, const char * acces_token);

	void dataLoaded(float t);

    void guest(void * context, const s32 size);
    
protected:
    void OnHttpRes(const s32 id, const Json::Value * root, const void * context, const s32 size);
    
	Login();
	~Login();

private:
	static Login * s_pSelf;
    cocos2d::Sprite * m_pBackground;
    CButton *  m_BtnLogin;
    CButton *  m_BtnTry;
};

#endif // __LOGIN_SCENE_H__
