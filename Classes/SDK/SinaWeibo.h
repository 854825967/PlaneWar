#ifndef _SINAWEIBO_H_
#define _SINAWEIBO_H_
#ifdef CC_TARGET_OS_IPHONE
#import "WeiboSDK.h"

@interface SinaWeibo : NSObject<WeiboSDKDelegate, WBHttpRequestDelegate> {
    NSString *  wbtoken;
}
+(SinaWeibo  * ) getInterface;
+(void) setRootView : (UIViewController  * ) viewController;
+(void)release;
+(void)online;
+(void)offline;
+(void)login;
+(void)getFriend;
+(void)shareWeibo : (NSString  * ) context andPic:(NSString  * ) pic;
@property (strong, nonatomic) NSString  * wbtoken;
@end
#elif defined __ANDROID__
#include "SDK.h"
#endif

#endif //_SINAWEIBO_H_
