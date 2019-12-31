// Common Includes
#import "core.h"
#import "EAGLView.h"
#include "interpreter.h"
#include "MediaPlayer/MediaPlayer.h"
#import <UserNotifications/UNNotificationResponse.h>
#import <UserNotifications/UNNotification.h>
#import <UserNotifications/UNNotificationRequest.h>
#import <UserNotifications/UNNotificationContent.h>

// Platform specific
void Output ( char const* pdasds )
{
}

void PlatformAppQuit()
{
	exit(0);
}

using namespace AGK;
@implementation iphone_appAppDelegate
@synthesize window;
@synthesize viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    float scale = [[UIScreen mainScreen] scale];
	if ( scale == 0 ) scale = 1;
	App.g_dwDeviceWidth = [UIScreen mainScreen].bounds.size.width * scale;
	App.g_dwDeviceHeight = [UIScreen mainScreen].bounds.size.height * scale;
    
	// Tell the UIDevice to send notifications when the orientation changes
	[[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
	[[NSNotificationCenter defaultCenter] addObserver:self 
											 selector:@selector(orientationChanged:) 
												 name:@"UIDeviceOrientationDidChangeNotification" 
											   object:nil];
	
	//[ window addSubview: viewController.view ];
    [window setRootViewController:viewController];
	[ window makeKeyAndVisible ];
    
	// call app begin
	try
	{
		App.Begin();
	}
	catch(...)
	{
		uString err; agk::GetLastError(err);
		err.Prepend( "Error: " );
		agk::Message( err.GetStr() );
		PlatformAppQuit();
		return YES;
	}
	[viewController setActive];
    
    NSDictionary *localNotify = launchOptions[UIApplicationLaunchOptionsLocalNotificationKey];
    if ( localNotify )
    {
        NSDictionary *aps = [localNotify objectForKey:@"aps"];
        if ( aps )
        {
            NSString *deeplink = [aps objectForKey:@"deeplink"];
            if ( deeplink ) agk::HandleDeepLink( [deeplink UTF8String] );
        }
    }

	NSDictionary *remoteNotify = launchOptions[UIApplicationLaunchOptionsRemoteNotificationKey];
    if ( remoteNotify )
    {
        NSDictionary *aps = [remoteNotify objectForKey:@"aps"];
        if ( aps )
        {
            NSString *deeplink = [aps objectForKey:@"deeplink"];
            if ( deeplink ) agk::HandleDeepLink( [deeplink UTF8String] );
        }
    }
    
    // Add to manage notification-related behaviors on iOS 10
    [UNUserNotificationCenter currentNotificationCenter].delegate = self;
    
	// success
    return YES;
}

//Called when a notification is delivered to a foreground app.
-(void)userNotificationCenter:(UNUserNotificationCenter *)center willPresentNotification:(UNNotification *)notification withCompletionHandler:(void (^)(UNNotificationPresentationOptions options))completionHandler
{
    //NSLog(@"User Info : %@",notification.request.content.userInfo);
    completionHandler(UNAuthorizationOptionSound | UNAuthorizationOptionAlert | UNAuthorizationOptionBadge);
}

//Called to let your app know which action was selected by the user for a given notification.
-(void)userNotificationCenter:(UNUserNotificationCenter *)center didReceiveNotificationResponse:(UNNotificationResponse *)response withCompletionHandler:(void(^)())completionHandler
{
    //NSLog(@"User Info : %@",response.notification.request.content.userInfo);
    
    // get the whole string from the notification
    NSDictionary *aps = [response.notification.request.content.userInfo objectForKey:@"aps"];
    if ( aps )
    {
        NSString *deeplink = [aps objectForKey:@"deeplink"];
        if ( deeplink ) agk::HandleDeepLink( [deeplink UTF8String] );
    }
    
    completionHandler();
}

- (NSUInteger)application:(UIApplication *)application supportedInterfaceOrientationsForWindow:(UIWindow *)window 
{
    return UIInterfaceOrientationMaskAll;
}

- (void)application:(UIApplication *)application didRegisterUserNotificationSettings:(UIUserNotificationSettings *)notificationSettings {
    if ( (notificationSettings.types & UIRemoteNotificationTypeAlert) != 0 )
    {
        [application registerForRemoteNotifications];
    }
}

- (void)application:(UIApplication*)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData*)deviceToken
{
    const unsigned char *dataBuffer = (const unsigned char *)[deviceToken bytes];
    
    NSUInteger          dataLength  = [deviceToken length];
    NSMutableString     *hexString  = [NSMutableString stringWithCapacity:(dataLength * 2)];
    
    for (int i = 0; i < dataLength; ++i)
        [hexString appendString:[NSString stringWithFormat:@"%02x", (unsigned int)dataBuffer[i]]];
    
    agk::PNToken( [hexString UTF8String] );
}

- (void)application:(UIApplication*)application didFailToRegisterForRemoteNotificationsWithError:(NSError*)error
{
    agk::PNToken( "Error" );
	NSLog(@"Failed to get token, error: %@", error);
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	[UIApplication sharedApplication].applicationIconBadgeNumber=0;
	[[UIApplication sharedApplication] cancelAllLocalNotifications];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    [viewController setAppActive:0];
	[viewController setInactive];
	agk::AppPausing();
    glFinish();
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    agk::AppResuming();
	agk::Resumed();
    [viewController setAppActive:1];
	[viewController setActive];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	// end the app
	[viewController setInactive];
	App.End();
    agk::CleanUp();
}

- (void)dealloc
{
    [viewController release];
    [window release];
    [super dealloc];
}

- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{
	agk::HandleMusicEvents( NULL );
}

- (void)orientationChanged:(NSNotification *)notification
{
	UIDeviceOrientation orientation = [[UIDevice currentDevice] orientation];
	float angle = 0;
	
	int mode = 1;
	switch( orientation )
	{
		case UIDeviceOrientationPortrait: mode=1; break; 
		case UIDeviceOrientationPortraitUpsideDown: mode=2; angle = 3.1415927f; break; 
		case UIDeviceOrientationLandscapeLeft: mode=3; angle = 1.5707963f; break;
		case UIDeviceOrientationLandscapeRight: mode=4; angle = -1.5707963f; break; 
		default: return;
	}
	
	agk::OrientationChanged( mode );
}

@end


@implementation iphone_appAppDelegate ( Facebook )

- ( BOOL ) application: ( UIApplication* ) application handleOpenURL: ( NSURL* ) url 
{
    // called by Facebook when returning back to our application after signing in,
    // this version is called by OS 4.2 and previous
    
    return agk::FacebookHandleOpenURL(url) > 0;
}

- ( BOOL ) application: ( UIApplication* ) application openURL: ( NSURL* ) url sourceApplication: ( NSString* ) sourceApplication annotation: ( id ) annotation 
{
    // same as above but for OS 4.3 and later
    
    return agk::FacebookHandleOpenURL(url) > 0;
}

- ( BOOL ) application: ( UIApplication* ) application continueUserActivity:(NSUserActivity*) userActivity restorationHandler:(void (^)(NSArray<id<UIUserActivityRestoring>> *restorableObjects)) restorationHandler
{
    return agk::FacebookHandleOpenURL(userActivity.webpageURL) > 0;
}

- ( void ) application: ( UIApplication* ) application didReceiveLocalNotification: ( UILocalNotification* ) notification 
{
    // called when the app receives a local notification
    [[UIApplication sharedApplication] cancelLocalNotification:notification];
    
    // reset the icon badge to 0
	application.applicationIconBadgeNumber = 0;
}

@end

@implementation ASIdentifierManager : NSObject @end

// use this if you want to remove the AdMob SDK (remove libGoogleAdMobAds.a and AdSupport.framework)
@implementation GADBannerView : UIView @end
@implementation GADRequest : NSObject @end
@implementation GADInterstitial : NSObject @end
@implementation GADAdSize : NSObject @end
@implementation GADExtras : NSObject @end
@implementation GADMobileAds : NSObject @end
GADAdSize const *kGADAdSizeBanner;
GADAdSize const *kGADAdSizeLargeBanner;
GADAdSize const *kGADAdSizeMediumRectangle;
GADAdSize const *kGADAdSizeFullBanner;
GADAdSize const *kGADAdSizeLeaderboard;
GADAdSize const *kGADAdSizeSmartBannerPortrait;
GADAdSize const *kGADAdSizeSmartBannerLandscape;
GADAdSize const *kGADAdSizeFluid;
@implementation GADRewardBasedVideoAd : NSObject @end
@implementation PACConsentForm : NSObject @end
@implementation PACConsentInformation : NSObject @end

// use this if you want to remove the Chartboost framework (also remove AdSupport.framework)
@implementation Chartboost : NSObject @end
NSString * CBLocationGameScreen;

// use this if you want to remove the Amazon Ad framework
@implementation AmazonAdRegistration : NSObject @end
@implementation AmazonAdOptions : NSObject @end
@implementation AmazonAdInterstitial : NSObject @end

// use this if you want to remove the Firebase frameworks
/*
 @implementation FIRApp : NSObject @end
 @implementation FIRAnalytics : NSObject @end
 */

/*
// use this if you want to remove the SnapChat commands
@implementation SCSDKPhotoSnapContent : NSObject @end
@implementation SCSDKSnapPhoto : NSObject @end
*/
