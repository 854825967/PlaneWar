/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Intent;
import android.os.Bundle;

import com.sina.weibo.sdk.auth.WeiboAuth;
import com.sina.weibo.sdk.auth.WeiboAuthListener;
import com.sina.weibo.sdk.auth.sso.SsoHandler;
import com.sina.weibo.sdk.exception.WeiboException;

public class AppActivity extends Cocos2dxActivity {
//	private static AppActivity s_this;
//	private WeiboAuth m_auth;
//	private SsoHandler m_SsoHandler;
//	
//	@Override
//	protected void onCreate(final Bundle savedInstanceState) {
//		super.onCreate(savedInstanceState);
//		s_this = this;
//		m_auth = new WeiboAuth(this, Constants.APP_KEY, Constants.REDIRECT_URL, Constants.SCOPE);
//	}
//	
//	public static boolean login() {
//		s_this.sso_auth();
//		return true;
//	}
//	
//	public native static void loginRes(int code, String uid, String token);
//	
//	public void sso_auth() {
//		m_SsoHandler = new SsoHandler(AppActivity.this, m_auth);
//		m_SsoHandler.authorize(new AuthListener());
////		m_auth.authorize(new AuthListener(), WeiboAuth.OBTAIN_AUTH_CODE);
//	}
//	
//	class AuthListener implements WeiboAuthListener {
//		@Override
//		public void onCancel() {
//			// TODO Auto-generated method stub
//			System.out.println("onCancel");
//			loginRes(1, "", "");
//		}
//
//		@Override
//		public void onComplete(Bundle arg0) {
//			// TODO Auto-generated method stub
//			System.out.println("onComplete : " + arg0.toString());
//			loginRes(0, arg0.getString("uid"), arg0.getString("access_token"));
//		}
//
//		@Override
//		public void onWeiboException(WeiboException arg0) {
//			// TODO Auto-generated method stub
//			System.out.println("onWeiboException");
//			loginRes(1, "", "");
//		}
//	}
//	
//	@Override
//	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
//		super.onActivityResult(requestCode, resultCode, data); 
//		if (m_SsoHandler != null) {
//			m_SsoHandler.authorizeCallBack(requestCode, resultCode, data);
//	    }
//	}
}
