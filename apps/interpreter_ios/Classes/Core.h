

#import <UIKit/UIKit.h>
#include "UntitledViewController.h"
#include "AGK.h"
#import <UserNotifications/UNUserNotificationCenter.h>

@class EAGLView;

@interface iphone_appAppDelegate : NSObject <UIApplicationDelegate, AVAudioPlayerDelegate, UNUserNotificationCenterDelegate> {
    UIWindow *window;
	UntitledViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet UntitledViewController *viewController;

@end

