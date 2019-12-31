#include "Wrapper.h"

namespace AGK
{
	// our globals, maybe these should go in a namespace
	struct sFriend
	{
		char szName [ 260 ];
		char szID   [ 260 ];
	};
}

using namespace AGK;

//std::vector < sFriend* >    g_FriendsList;
int                         g_iFriendsState                             = 0;
int                         g_iFacebookHTTP                             = 0;
int                         g_iFacebookDownloadState                    = 0;
//SocialPlugins*              g_pSocialPlugins                            = nil;
//NSString*                   g_nsFacebookID                              = nil;
int                         g_iNotification                             = 0;
char                        g_szFacebookFileDestination [ 260 ]    = "";
char                        g_szNotificationData        [ 260 ]    = "";

// internally called by AGK
void agk::PlatformSocialPluginsSetup( void )
{
	
}

void agk::PlatformSocialPluginsDestroy( void )
{
	
}

// RATING COMMANDS
void agk::PlatformRateApp( const char* szID, const char* title, const char* message )
{
}

// IN APP PURCHASE COMMANDS
void agk::PlatformInAppPurchaseSetKeys( const char *szData1, const char *szData2 )
{

}	

void agk::PlatformInAppPurchaseSetTitle( const char* szTitle )
{
}

void  agk::PlatformInAppPurchaseAddProductID    ( const char* szID, int type )
{
}

void  agk::PlatformInAppPurchaseSetup           ( void )
{
}

void  agk::PlatformInAppPurchaseActivate        ( int iID )
{
}

int   agk::PlatformGetInAppPurchaseState        ( void )
{
	return 1; //finished or not started
}

int   agk::PlatformGetInAppPurchaseAvailable    ( int iID )
{
	return 0;
}

char* agk::PlatformGetInAppPurchaseLocalPrice( int iID )
{
	char *str = new char[1];
	*str = 0;
	return str;
}

char* agk::PlatformGetInAppPurchaseDescription( int iID )
{
	char *str = new char[1];
	*str = 0;
	return str;
}

bool  agk::PlatformHasInAppPurchase             ( void )
{
	return false;
}

void agk::PlatformInAppPurchaseRestore()
{
	
}

char* agk::PlatformGetInAppPurchaseSignature(int iID)
{
	char *str = new char[1];
	*str = 0;
	return str;
}

// ADMOB COMMANDS
void  agk::PlatformAdMobSetupRelative ( const char* szID, int horz, int vert, float offsetX, float offsetY, int type )
{
}

void  agk::PlatformAdMobFullscreen ()
{
}

void  agk::PlatformAdMobCacheFullscreen ()
{
}

void  agk::PlatformAdMobPosition( int horz, int vert, float offsetX, float offsetY )
{
}

void  agk::PlatformSetAdMobVisible              ( int iVisible )
{
}

void  agk::PlatformAdMobRequestNewAd            ( void )
{
}

void  agk::PlatformAdMobDestroy                 ( void )
{
}

bool  agk::PlatformHasAdMob             ( void )
{
	return false;
}

int  agk::PlatformAdMobGetFullscreenLoaded ()
{
	return 0;
}

void agk::PlatformAdMobRewardAd()
{

}

void agk::PlatformAdMobCacheRewardAd()
{

}

int agk::PlatformAdMobGetRewardAdLoaded()
{
	return 0;
}

int agk::PlatformAdMobGetRewardAdRewarded()
{
	return 0;
}

void agk::PlatformAdMobResetRewardAd()
{
	
}

void agk::PlatformAdMobSetTesting (int testing)
{

}

// CHARTBOOST

void  agk::PlatformChartboostSetup ()
{
}

void  agk::PlatformChartboostFullscreen ()
{
}

int  agk::PlatformChartboostGetFullscreenLoaded ()
{
	return 0;
}

void agk::PlatformChartboostRewardAd()
{
	
}

void agk::PlatformChartboostCacheRewardAd()
{
	
}

int agk::PlatformChartboostGetRewardAdLoaded()
{
	return 0;
}

int agk::PlatformChartboostGetRewardAdRewarded()
{
	return 0;
}

void agk::PlatformChartboostResetRewardAd()
{
	
}


// Amazon Ads

void  agk::PlatformAmazonAdSetup ()
{

}

void  agk::PlatformAmazonAdSetTesting (int testing)
{

}

void  agk::PlatformAmazonAdFullscreen ()
{

}

int  agk::PlatformAmazonGetFullscreenLoaded ()
{
	return 0;
}

// FACEBOOK COMMANDS
void  agk::PlatformFacebookSetup                ( const char* szID )
{
}

void  agk::PlatformFacebookLogin                ( void )
{
}

void  agk::PlatformFacebookLogout               ( void )
{
}

void  agk::PlatformFacebookShowLikeButton       ( const char* szURL, int iX, int iY, int iWidth, int iHeight )
{
}

void  agk::PlatformFacebookDestroyLikeButton    ( void )
{
}

void  agk::PlatformFacebookPostOnMyWall         ( const char* szLink, const char* szPicture, const char* szName, const char* szCaption, const char* szDescription )
{
}

void  agk::PlatformFacebookPostOnFriendsWall    ( const char* szID, const char* szLink, const char* szPicture, const char* szName, const char* szCaption, const char* szDescription )
{
}

void  agk::PlatformFacebookInviteFriend         ( const char* szID, const char* szMessage )
{
}

void  agk::PlatformFacebookGetFriends           ( void )
{
}

int   agk::PlatformFacebookGetFriendsState      ( void )
{
	return -1;
}

int   agk::PlatformFacebookGetFriendsCount      ( void )
{
	return 0;
}

char* agk::PlatformFacebookGetFriendsName       ( int iIndex )
{
	char *str = new char[1]; *str = 0;
	return str;
}

char* agk::PlatformFacebookGetFriendsID         ( int iIndex )
{
	char *str = new char[1]; *str = 0;
	return str;
}

void  agk::PlatformFacebookDownloadFriendsPhoto ( int iIndex )
{
}

int   agk::PlatformGetFacebookDownloadState     ( void )
{
	return -1;
}

char* agk::PlatformGetFacebookDownloadFile      ( void )
{
	char *str = new char[1]; *str = 0;
	return str;
}

int   agk::PlatformGetFacebookLoggedIn          ( void )
{
	return -1;
}

bool  agk::PlatformHasFacebook             ( void )
{
	return false;
}

char* agk::PlatformFacebookGetUserID			  ( void )
{
	char *str = new char[1]; *str = 0;
	return str;
}

char* agk::PlatformFacebookGetUserName		  ( void )
{
	char *str = new char[1]; *str = 0;
	return str;
}

char* agk::PlatformFacebookGetAccessToken		  ( void )
{
	char *str = new char[1]; *str = 0;
	return str;
}


// TWITTER COMMANDS
void  agk::PlatformTwitterSetup                 ( const char* szKey, const char* szSecret )
{
}

void  agk::PlatformTwitterMessage               ( const char* szMessage )
{
}

bool  agk::PlatformHasTwitter             ( void )
{
	return false;
}

// local notifications

void agk::PlatformCreateLocalNotification( int iID, int datetime, const char *szMessage, const char *szDeepLink )
{
	
}

void agk::PlatformCancelLocalNotification( int iID )
{
	
}