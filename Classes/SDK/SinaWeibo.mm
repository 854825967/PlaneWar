#import "SinaWeibo.h"
#include "SDK.h"
#include "LoginScene.h"

static SinaWeibo * s_pSelf = nil;
static bool s_onLine = false;
static UIViewController * s_viewController = nil;

@interface SinaWeibo () <WeiboSDKDelegate>

@end

@implementation SinaWeibo

@synthesize wbtoken;

+ (SinaWeibo  * ) getInterface {
    if (s_pSelf ==  nil) {
        s_pSelf = [[SinaWeibo alloc] init];
    }
    return s_pSelf;
}

+(void) setRootView : (UIViewController  * ) viewController {
    s_viewController = viewController;
}

+ (void) release {
    if (s_pSelf !=  nil) {
        [s_pSelf release];
        s_pSelf = nil;
    }
}

+ (void) online {
    s_onLine = true;
}

+ (void) offline {
    s_onLine = false;
}

+ (void) login {
    WBAuthorizeRequest  * request = [WBAuthorizeRequest request];
    request.redirectURI = @"https://api.weibo.com/oauth2/default.html";
    request.scope = @"all";
    request.userInfo = @{@"SSO_From": @"SendMessageToWeiboViewController",
                         @"Other_Info_1": [NSNumber numberWithInt:123],
                         @"Other_Info_2": @[@"obj1", @"obj2"],
                         @"Other_Info_3": @{@"key1": @"obj1", @"key2": @"obj2"}};
    [WeiboSDK sendRequest:request];
}

+ (void) getFriend {
    
}

+ (void) shareWeibo : (NSString  * ) context andPic:(NSString  * ) pic {
    WBMessageObject * message = [WBMessageObject message];
    message.text = context;
    
    NSString * filePath = [NSHomeDirectory() stringByAppendingFormat:@"/Documents/%@", pic];
    WBImageObject  * image = [WBImageObject object];
    image.imageData = [NSData dataWithContentsOfFile:filePath];
    message.imageObject = image;
    
//    WBWebpageObject  * webpage = [WBWebpageObject object];
//    webpage.objectID = @"identifier1";
//    webpage.title = @"SmartStar";
//    webpage.description = @"轻松益智小游戏,玩着简单,但是你能玩过10万分吗?";
//    webpage.thumbnailData = [NSData dataWithContentsOfFile:filePath];
//    webpage.webpageUrl = @"https://itunes.apple.com/us/app/smartstar/id904126266";
//    message.mediaObject = webpage;
    
    WBSendMessageToWeiboRequest  * request = [WBSendMessageToWeiboRequest requestWithMessage:message];
//    request.userInfo = @{@"file":filePath};
//    request.shouldOpenWeiboAppInstallPageIfNotInstalled = NO;
    
    [WeiboSDK sendRequest:request];
    
    //return message;
}

- (void)didReceiveWeiboResponse:(WBBaseResponse  * )response {
    if ([response isKindOfClass:WBSendMessageToWeiboResponse.class]) {
        NSString  * title = @"发送结果";
        NSString  * message = [NSString stringWithFormat:@"响应状态: %d\n响应UserInfo数据: %@\n原请求UserInfo数据: %@",(int)response.statusCode, response.userInfo, response.requestUserInfo];
        UIAlertView  * alert = [[UIAlertView alloc] initWithTitle:title
                                                        message:message
                                                       delegate:nil
                                              cancelButtonTitle:@"确定"
                                              otherButtonTitles:nil];
        [alert show];
        [alert release];
        
        if (0 ==  response.statusCode) {
            
        } else {
            
        }
        
    } else if ([response isKindOfClass:WBAuthorizeResponse.class]) {
//        NSString  * title = @"认证结果";
//        NSString  * message = [NSString stringWithFormat:@"响应状态: %d\nresponse.userId: %@\nresponse.accessToken: %@\n响应UserInfo数据: %@\n原请求UserInfo数据: %@",(int)response.statusCode,[(WBAuthorizeResponse  * )response userID], [(WBAuthorizeResponse  * )response accessToken], response.userInfo, response.requestUserInfo];
//        UIAlertView  * alert = [[UIAlertView alloc] initWithTitle:title
//                                                        message:message
//                                                       delegate:nil
//                                              cancelButtonTitle:@"确定"
//                                              otherButtonTitles:nil];
//        
//        self.wbtoken = [(WBAuthorizeResponse  * )response accessToken];
//        
//        [alert show];
//        [alert release];
        
        Login::OnLoginWeiboRes((int)response.statusCode, [[(WBAuthorizeResponse  * )response userID] UTF8String], [[(WBAuthorizeResponse  * )response accessToken] UTF8String]);
        
        [SinaWeibo online];
    }
}
@end

void WeiboLogin() {
    [SinaWeibo login];
}
