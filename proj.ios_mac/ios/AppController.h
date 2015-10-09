#import <UIKit/UIKit.h>
#import "WeiboSDK.h"

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate, WeiboSDKDelegate> {
    UIWindow *window;
}

@property(nonatomic, readonly) RootViewController* viewController;

@end

