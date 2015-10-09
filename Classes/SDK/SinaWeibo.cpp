#include "SinaWeibo.h"

#ifdef __ANDROID__
#include "MultiSys.h"
#include <jni.h>
#include <jni/JniHelper.h>
#include <android/log.h>
#include "LoginScene.h"

char * jstringToString(JNIEnv * env, jstring jstr) {
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
		rtn = (char*)malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	return rtn;
}

void WeiboLogin() {
	cocos2d::JniMethodInfo minfo;
	bool b = cocos2d::JniHelper::getStaticMethodInfo(minfo,
			"org/cocos2dx/cpp/AppActivity",
			"login",
			"()Z");

	bool res = false;

	if (!b) {
		__android_log_print(ANDROID_LOG_ERROR, "cocos2d-x debug info", "JniHelp::getStaticMethodInfo error...");
	} else {
		res = (bool)minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
	}
}

#ifdef __cplusplus
	extern "C" {
#endif

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_loginRes(JNIEnv * env, jobject, jint code, jstring uid, jstring token) {
		const char * puid = jstringToString(env, uid);
		const char * ptoken = jstringToString(env, token);
		ECHO("login res, code:%d uid:%s token:%s", code, puid, ptoken);
		Login::OnLoginWeiboRes(code, puid, ptoken);
	}

#ifdef __cplusplus
	}
#endif

#endif //__ANDROID__
