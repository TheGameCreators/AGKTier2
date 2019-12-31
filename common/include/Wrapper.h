// Wrapper Header
#ifndef _H_WRAPPER_
#define _H_WRAPPER_

#ifdef DrawText
	#undef DrawText
#endif

// Includes
#include "Common.h"
#include "agk.h"

#ifdef LoadImage
#undef LoadImage
#endif

#ifdef PlaySound
#undef PlaySound
#endif

#define AGK_MAX_VIRTUAL_BUTTONS 101

#define AGK_FLAG_PORTRAIT			0x0001
#define AGK_FLAG_PORTRAIT2			0x0002
#define AGK_FLAG_LANDSCAPE			0x0004
#define AGK_FLAG_LANDSCAPE2			0x0008
#define AGK_FLAG_CAN_RESIZE			0x0010

#define AGK_SENSOR_ACTIVATE_ACCEL	0x01
#define AGK_SENSOR_ACTIVATE_GYRO	0x02
#define AGK_SENSOR_ACTIVATE_PROX	0x04
#define AGK_SENSOR_ACTIVATE_LIGHT	0x08
#define AGK_SENSOR_ACTIVATE_MAG		0x10
#define AGK_SENSOR_ACTIVATE_ROT		0x20

// capability flags
#define AGK_CAP_UINT_INDICES		0x0001

// Bullet physics forward declarations
//class btBroadphaseInterface;
//class btDefaultCollisionConfiguration;
//class btCollisionDispatcher;
//class btSequentialImpulseConstraintSolver;
//class btDiscreteDynamicsWorld;

#define AGK_FILE_TYPE_READ			0x01
#define AGK_FILE_TYPE_WRITE			0x02


// GDK Class
namespace AGK
{
	class MyDestructionListener;

	class cFileEntry
	{
		protected:
			uString m_sReal;
			uString m_sLowercase;
			cFileEntry *m_pNext;

			static cHashedList<cFileEntry> g_cFileList;
			static void TraverseDirectory( const char* dir );

		public:
			cFileEntry() { m_pNext = UNDEF; }
			~cFileEntry() {}

			static void InitFileList();
			static void AddNewFile( const char *szFullPath );
			static void ConvertToReal( uString &path );
			static void ClearAll();
	};
	
	class cDirectoryItem
	{
		public:
			uString m_sItem;
			cDirectoryItem *m_pNext;
			UINT iFlags; 

			cDirectoryItem() { m_pNext = 0; iFlags = 0; }
			~cDirectoryItem() { }
	};

	class cMemblock
	{
		public:
			UINT m_iID;
			unsigned char* m_pData;
			UINT m_iSize;
			UINT m_iUser1;
			UINT m_iUser2;
			UINT m_iUser3;
			UINT m_iUser4;

			cMemblock() { m_iID = 0; m_pData = 0; m_iSize = 0; m_iUser1 = 0xffffffff; m_iUser2 = 0xffffffff; m_iUser3 = 0xffffffff; m_iUser4 = 0xffffffff; }
			~cMemblock() { if ( m_pData ) delete [] m_pData; }
	};

	class agk
	{
		private:
			// this prevents the creation of an instance of this class by other classes.
			agk();

			// this is messy, but we're stuck with it now, it's not worth cleaning up
			friend class cText;
			friend class cSprite;
			friend class cImage;
			friend class Physics;
			friend class cNetCon;
			friend class AGKSocket;
			friend class AGKPacket;
			friend class cFile;
			friend class cMusicMgr;
			friend class cSpriteMgr;
			friend class cSpriteMgrEx;
			friend class cParticleEmitter;
			friend class cSoundMgr;
			friend class cVirtualButton;
			friend class cVirtualJoystick;
			friend class cJoystick;
			friend class cEditBox;
			friend class ZipFile;
			friend class AGKShader;
			friend class cMesh;
			friend class cObject3D;
			friend class cObjectMgr;
			friend class DebugDraw;
			friend class Skeleton2D;
			friend class GLDebugDrawer;
			friend class btIDebugDraw;
			friend class AGKSkyBox;
			friend class AGK3DParticleEmitter;

		public:

			// *********************
			// internal PUBLIC variables
			// *********************

			// Remote Debugging globals
			static bool m_bTriggerBreakPointNow;
        
            static int m_iChartboostIsDisplaying;
			
		protected:

			// *********************
			// internal variables
			// *********************

			// 3D Physics
			/*static btBroadphaseInterface *m_pBroadphase;
			static btDefaultCollisionConfiguration* m_pCollisionConfiguration;
			static btCollisionDispatcher* m_pDispatcher;
			static btSequentialImpulseConstraintSolver* m_pSolver;
			static btDiscreteDynamicsWorld* m_pDynamicsWorld;*/

			// calculated viewport dimensions
			static float m_fTargetViewportX;
			static float m_fTargetViewportY;
			static float m_fTargetViewportWidth;
			static float m_fTargetViewportHeight;

			static int m_iScissorX;
			static int m_iScissorY;
			static int m_iScissorWidth;
			static int m_iScissorHeight;
			static bool m_bScissorUser;
			static bool m_bScissorEnabled;
			static float m_fScissorUserX;
			static float m_fScissorUserY;
			static float m_fScissorUserX2;
			static float m_fScissorUserY2;

			// app start time in seconds
			static double m_fTimeStart; 
			// current app time in seconds
			static double m_fTimeCurr;
			// time current frame started in seconds
			static double m_fTimeFrameStart;
			static double m_fTimeLimitFrameStart;
			// time spent processing the last frame in seconds 
			static float m_fTimeDelta; 
			static UINT m_iTimeMilliseconds;
			static float m_fFps;
			static bool m_bUsingVSync;

			static UINT m_iPolygonsDrawn;
			static UINT m_iShadowPolygonsDrawn;
			static UINT m_iVerticesProcessed;

			// width and height of the device (current state of device target) (i.e. desktop resolution)
			static int m_iRealDeviceWidth;
			static int m_iRealDeviceHeight;

			// current render target size that may not be equal to the device size.
			static int m_iRenderWidth;
			static int m_iRenderHeight;
			
			// intended device size, if real device is much smaller AGK will reduce textures
			static int m_iIntendedWidth;
			static int m_iIntendedHeight;

			static int m_iUseNewDefaultFonts;

			static bool m_bUsingFBO;
			static UINT m_iFBOWidth;
			static UINT m_iFBOHeight;

			// fog variables
			static int m_iFogMode; // 0=off, 1=pixel fog
			static UINT m_iFogColor1; // used for comparison with m_iFogColor2 only
			static UINT m_iFogColor2; // used for comparison with m_iFogColor1 only
			static int m_iFogColor2IsSet; // 1=The user has specifically set m_iFogColor2
			static float m_fFogMinDist;
			static float m_fFogMaxDist;

			// shadow variables
			static int m_iShadowMode; // 0=Off, 1=Uniform, 2=Lightspace Perspective (LiPSM), 3=Cascade
			static int m_iShadowSmoothing; // 0=none, 1=multisampled
			static int m_iShadowMapWidth;
			static int m_iShadowMapHeight;
			static float m_fShadowRange;
			static float m_fShadowBias;
			static float m_fShadowLightStep;
			static float m_fShadowCascade1;
			static float m_fShadowCascade2;
			static float m_fShadowCascade3;
			static FrameBuffer *m_pShadowMapFBO;
			static FrameBuffer *m_pShadowMapFBO2;
			static FrameBuffer *m_pShadowMapFBO3;
			static FrameBuffer *m_pShadowMapFBO4;
			static cImage *m_pShadowMap;
			static cImage *m_pShadowMap2;
			static cImage *m_pShadowMap3;
			static cImage *m_pShadowMap4;
		
			// width and height of the workspace (display), as requested by the user in InitGL()
			static int m_iDisplayWidth;
			static int m_iDisplayHeight;
			static float m_iDisplayExtraX;
			static float m_iDisplayExtraY;
			static float m_fDisplayAspect;
			static int m_iDisplayIsLandscape;
			static bool m_bUsingDisplayAspect;
			static bool m_bUsingDeviceAspect;
			static float m_fStretchValue;
			static bool m_bClearDepth;
			static bool m_bClearColor;
			static int m_iResolutionMode;
			static int m_iExtraAGKPlayerAssetsMode;
			static int m_iOrientation;
			static float m_fCurrentAngle;
			static float m_fTargetAngle;
			static UINT m_bAGKFlags;
			static UINT m_iTransitionMode;
			static UINT m_iDefaultMagFilter;
			static UINT m_iDefaultMinFilter;
			static UINT m_iDefaultWrapU;
			static UINT m_iDefaultWrapV;
			static uString m_sCompanyName;
			static uString m_sAppName;
			static uString m_sCL;
			static UINT m_iClearColor;
			static UINT m_iBorderColor;
			static float m_fAlphaTestThreashold;
			static bool m_bSortCreated;
			static bool m_bIsCapturing;
			static UINT m_iUncollectedCaptureImage;
            static bool m_bIsChoosing;
            static cImage* m_pUncollectedChosenImage;

			static UINT m_iPulleySprite1;
			static UINT m_iPulleySprite2;
			static float m_iPulleyRatio;
			static int m_iPulleyColConnected;
			
			static int wasMusicPlaying;
			static int wasVideoPlaying;
			static int m_iPaused;
			static int m_iResumed;
			static int m_iResumed2;

			static int m_iVideoPlayMode;
			static UINT m_iVideoTextureRaw;
			static cImage *m_pVideoTexture;
			static cObject3D *m_pVideoTextureQuad;
			static AGKShader *m_pVideoTextureShader;
			static FrameBuffer *m_pVideoTextureFBO;

			static cImage *m_pCameraTexture;
			static int m_iDeviceCameraID;
			static int m_iDeviceCameraType;

			static int m_iSGX540GPU;

			// input variables
			// mouse
			static cMouse *m_pMouse[ 4 ];

			// accelerometer
			static float m_fAccelX;
			static float m_fAccelY;
			static float m_fAccelZ;
			static float m_fFakeAccelX;
			static float m_fFakeAccelY;

			static float m_fGyroX;
			static float m_fGyroY;
			static float m_fGyroZ;

			static float m_fProximityDist;

			static float m_fLightLevel;

			static float m_fMagneticX;
			static float m_fMagneticY;
			static float m_fMagneticZ;
			
			static float m_fRotVecX;
			static float m_fRotVecY;
			static float m_fRotVecZ;
			static float m_fRotVecW;

			static float m_fGPSLong;
			static float m_fGPSLat;
			static float m_fGPSAltitude;

			static UINT m_bSensorFlags;

			// keyboard
			#define AGK_MAX_KEYS 512
			static unsigned char m_iPrevKeyDown[ AGK_MAX_KEYS ];
			static unsigned char m_iKeyDown[ AGK_MAX_KEYS ];
			static unsigned char m_iResetKey[ AGK_MAX_KEYS ];
			static unsigned int m_iLastKey;
			static int m_iKeyboardMode;

			// joystick
			static cJoystick* m_pJoystick[ AGK_NUM_JOYSTICKS ];
			static cVirtualJoystick *m_pVirtualJoystick[ 4 ];

			// virtual buttons
			static cVirtualButton *m_pVirtualButton[ AGK_MAX_VIRTUAL_BUTTONS ];

			// pointer state
			static int m_iLastPointerDown;
			static int m_iPointerDown;
			static float m_fPointerX;
			static float m_fPointerY;

			// AGK joystick
			static float m_fFakeJoystickX;
			static float m_fFakeJoystickY;
			static float m_fJoystickDeadZone;

			// text input variables
			static bool m_bInputStartedPrev;
			static bool m_bInputStarted;
			static bool m_bInputCancelled;
			static uString m_sCurrInput;
			static int m_iLastChar;
			static int m_iCurrChar;
			static uString m_sCharBuffer;
			static float m_fCursorBlinkDelay;
			static UINT m_iInputMaxChars;
			static uString m_sInputLabel;
			
			// single music manager for agk functions
			static cMusicMgr m_cMusicMgr;
			static int m_iDeviceVolume;

			// directory variables
			static cDirectoryItem *m_pCurrentDirectories;
			static cDirectoryItem *m_pCurrentFiles;

			static cDirectoryItem *m_pCurrentDirectoryIter;
			static cDirectoryItem *m_pCurrentFileIter;
			static int m_iCurrentDirectoryMode; // 0=read, 1= write, 2=both
			static int m_iCurrentFileMode; // 0=read, 1= write, 2=both
            static bool m_bUpdateFileLists;

			// date/time variables
			static int m_pMonthDays[ 12 ];

			// advert variables
			static uString m_sAdMobCode;
			static uString m_sAdMobRewardAdCode;
			static uString m_sInneractiveCode;
			static uString m_sChartboostCode1;
			static uString m_sChartboostCode2;
			static uString m_sAmazonAdCode;
			static uString m_sAdClientID;
			static uString m_sInneractiveURL;
			static uString m_sInneractiveImage;
			static cImage *m_pAdImage;
			static cSprite *m_pAdSprite;
			static cHTTPConnection *pHTTPInneractive;
			static float m_fAdStartTime;
			static int m_iInneractiveStage;
			static float m_fAdX;
			static float m_fAdY;
			static float m_fAdWidth;
            static float m_fAdOffsetX;
            static float m_fAdOffsetY;
			static int m_iAdType;
			static int m_iAdHorz;
			static int m_iAdVert;
            static int m_iAdTest;
            static int m_iAdVisible;

			// video variables
			static float m_fVideoX;
			static float m_fVideoY;
			static float m_fVideoWidth;
			static float m_fVideoHeight;

			// 3D variables
			static int m_iProjMatrixMode;
			static bool m_bRightHand;
			static AGKMatrix4 m_matOrtho;
			static cCamera *m_pCurrentCamera;
			static int m_i3DDepth;

			// state change variables
			static char m_iCurrentBlendEnabled;
			static char m_iCurrentBlendFunc1;
			static char m_iCurrentBlendFunc2;
			static char m_iCurrentDepthTest;
			static UINT m_iCurrentBoundVBO;
			static UINT m_iCurrentBoundIndexVBO;
			static char m_iCurrentDepthFunc;
			static char m_iCurrentDepthWrite;
			static char m_iCurrentCullMode;	
			static float m_fCurrentDepthBias;
			static float m_fCurrentDepthNear;
			static float m_fCurrentDepthFar;

			// drawing variables
			static UINT* m_pLines;
			static UINT m_iNumLines;
			static UINT m_iLineArraySize;

			// collision variables
			static CollisionResults m_cCollisionResults;

			// ***********************
			// internal functions
			// ***********************
			static void PlatformSetDevicePtr( void* ptr );
			static void InitCommon();
			static void PlatformInitCommon();
			static void PlatformInitExternal( void *ptr, int width, int height, void(*swap)(void*), void* swapParam );
            static void PlatformInitGL( void *ptr );
			static void PlatformInitConsole();
			static void PlatformDestroyGL( void );
			static void PlatformInitFilePaths();
			static void RecalculateDisplay();
			static void PlatformBindDefaultFramebuffer();
			static void PlatformSync();
			static void PlatformSwap();
			static void PlatformClearScreenRaw( float red, float green, float blue, float alpha );
			static void PlatformClearScreen();
			static void PlatformClearDepthBuffer();
			static void PlatformFlush();
			static void PlatformUpdateDeviceSize();
			static void PlatformCleanUp();
			static void PlatformDeviceVolume();
			static void PlatformUpdateTime();
			static void PlatformResetTime();
			static void PlatformReportError( const uString &sMsg );
			static void PlatformSetViewport( int x, int y, int width, int height );
			static void PlatformCompleteInputInit();
			static int PlatformInputPointerPressed(float x, float y);
			static void PlatformStartTextInput( const char *sInitial );
			static void PlatformStopTextInput();
            static void PlatformChangeTextInput( const char *str );
            static void PlatformUpdateTextInput();
			static void PlatformDrawTextInput();
			static void PlatformMessage( const char* msg );
			static void PlatformPrepareDefaultDraw();
			static void PlatformUpdateWritePath();
			static void PlatformRestoreWritePath();
			static void PlatformScissor( int x, int y, int width, int height );
			static bool PlatformShowCaptureScreen();
			static void PlatformHideCaptureScreen();
            static void PlatformShowChooseScreen();
			static bool PlatformChooseFile( uString &out, const char *ext, int fullPath );
			static void PlatformResumed();
			static void PlatformResumedOpenGL();
			static int PlatformGetAdPortal();
			static void PlatformSetOrientationAllowed( int p, int p2, int l, int l2 );
			static void PlatformSetBlendMode( int mode ); // 0=opaque, 1=alpha blending, 2=additive blending
			static void PlatformDeleteBuffer( UINT buffer );
			static int PlatformGetNumProcessors();
			static bool PlatformGetDeviceID( uString &out );
			static float PlatformDeviceScale();
			static int PlatformGetMaxVSUniforms();
			static int PlatformGetMaxPSUniforms();
			static int PlatformGetMaxVertexTextures();
			static int PlatformGetMaxVaryings();
			static int PlatformSupportsPSHighp();
			static void PlatformSetAlignment( int align );
			static void PlatformDisableScissor();
			static void PlatformDisableStencil();

			// state changes
			static void PlatformSetBlendFunc( int mode1, int mode2 ); // 0=zero, 1=one, 2=sAlpha, 3=1-sAlpha, 4=dAlpha, 5=1-dAlpha, 6=sColor, 7=1-sColor, 8=dColor, 9=1-dColor, 10=saturate alpha
			static void PlatformSetBlendEnabled( int mode ); // 0=off, 1=on
			static void PlatformSetDepthTest( int mode ); // 0=off, 1=on
			static void PlatformSetDepthFunc( int mode ); // 0=never, 1=less, 2=equal, 3=lequal, 4=greater, 5=notequal, 6=gequal, 7=equal, 8=always
			static void PlatformSetDepthWrite( int mode ); // 0=off, 1=on
			static void PlatformSetDepthBias( float bias ); // 0=off
			static void PlatformSetDepthRange( float zNear, float zFar );
			static void PlatformSetCullMode( int mode ); // 0=off, 1=on
			static bool PlatformBindBuffer( UINT buffer );
			static bool PlatformBindIndexBuffer( UINT buffer );

			// social plugins
			static void  PlatformSocialPluginsSetup           ( void );
			static void  PlatformSocialPluginsDestroy         ( void );

			// RATING COMMANDS
			static void  PlatformRateApp                      ( const char* szID, const char* title, const char* message );

			// IN APP PURCHASE COMMANDS
			static void  PlatformInAppPurchaseSetKeys		  ( const char *szData1, const char *szData2 );
			static void  PlatformInAppPurchaseSetTitle        ( const char* szTitle );
			static void  PlatformInAppPurchaseAddProductID    ( const char* szID, int type );
			static void  PlatformInAppPurchaseSetup           ( void );
			static void  PlatformInAppPurchaseActivate        ( int iID );
			static int   PlatformGetInAppPurchaseState        ( void );
			static int   PlatformGetInAppPurchaseAvailable    ( int iID );
            static char* PlatformGetInAppPurchaseLocalPrice   ( int iID );
            static char* PlatformGetInAppPurchaseDescription  ( int iID );
			static bool  PlatformHasInAppPurchase             ( void );
			static void  PlatformInAppPurchaseRestore		  ();
			static char* PlatformGetInAppPurchaseSignature    ( int iID );

			// ADMOB COMMANDS
			static void  PlatformAdMobSetupRelative			  ( const char* szID, int horz, int vert, float offsetX, float offsetY, int type );
			static void  PlatformAdMobFullscreen			  ();
			static void  PlatformAdMobCacheFullscreen		  ();
            static void  PlatformAdMobPosition                ( int horz, int vert, float offsetx, float offsety );
			static void  PlatformSetAdMobVisible              ( int iVisible );
			static void  PlatformAdMobRequestNewAd            ( void );
			static void  PlatformAdMobDestroy                 ( void );
			static bool  PlatformHasAdMob		              ( void );
			static int   PlatformAdMobGetFullscreenLoaded	  ( void );
			static void  PlatformAdMobRewardAd				  ();
			static void  PlatformAdMobCacheRewardAd			  ();
			static int   PlatformAdMobGetRewardAdLoaded		  ( void );
			static int   PlatformAdMobGetRewardAdRewarded	  ( void );
			static void  PlatformAdMobResetRewardAd			  ( void );
			static void  PlatformAdMobSetTesting			  ( int testing );

			// CHARTBOOST COMMANDS
			static void  PlatformChartboostSetup			  ( void );
			static void  PlatformChartboostFullscreen		  ( void );
			static int   PlatformChartboostGetFullscreenLoaded( void );
			static void  PlatformChartboostRewardAd			  ();
			static void  PlatformChartboostCacheRewardAd	  ();
			static int   PlatformChartboostGetRewardAdLoaded  ( void );
			static int   PlatformChartboostGetRewardAdRewarded( void );
			static void  PlatformChartboostResetRewardAd	  ( void );

			// Amazon Ads
			static void  PlatformAmazonAdSetup				  ( void );
            static void  PlatformAmazonAdSetTesting			  ( int testing );
			static void  PlatformAmazonAdFullscreen			  ( void );
			static int   PlatformAmazonGetFullscreenLoaded	  ( void );

			// FACEBOOK COMMANDS
			static void  PlatformFacebookSetup                ( const char* szID );
			static void  PlatformFacebookLogin                ( void );
			static void  PlatformFacebookLogout               ( void );
			static void  PlatformFacebookShowLikeButton       ( const char* szURL, int iX, int iY, int iWidth, int iHeight );
			static void  PlatformFacebookDestroyLikeButton    ( void );
			static void  PlatformFacebookPostOnMyWall         ( const char* szLink, const char* szPicture, const char* szName, const char* szCaption, const char* szDescription );
			static void  PlatformFacebookPostOnFriendsWall    ( const char* szID, const char* szLink, const char* szPicture, const char* szName, const char* szCaption, const char* szDescription );
			static void  PlatformFacebookInviteFriend         ( const char* szID, const char* szMessage );
			static void  PlatformFacebookGetFriends           ( void );
			static int   PlatformFacebookGetFriendsState      ( void );
			static int   PlatformFacebookGetFriendsCount      ( void );
			static char* PlatformFacebookGetFriendsName       ( int iIndex );
			static char* PlatformFacebookGetFriendsID         ( int iIndex );
			static void  PlatformFacebookDownloadFriendsPhoto ( int iIndex );
			static int   PlatformGetFacebookDownloadState     ( void );
			static char* PlatformGetFacebookDownloadFile      ( void );
			static int   PlatformGetFacebookLoggedIn          ( void );
			static bool  PlatformHasFacebook	              ( void );
			static char* PlatformFacebookGetUserID			  ( void );
			static char* PlatformFacebookGetUserName		  ( void );
			static char* PlatformFacebookGetAccessToken		  ( void );

			// TWITTER COMMANDS
			static void  PlatformTwitterSetup                 ( const char* szKey, const char* szSecret );
			static void  PlatformTwitterMessage               ( const char* szMessage );
			static bool  PlatformHasTwitter		              ( void );

			// local notifications
			static void PlatformCreateLocalNotification( int iID, int datetime, const char *szMessage, const char *szDeepLink );
			static void PlatformCancelLocalNotification( int iID );

			static int PlatformGetIP( uString &sIP );
			static int PlatformGetIPv6( uString &sIP, int *iInterface=0 );
			
			static void UpdateAd();
			static void DrawAd();
			
			static void CreateDefaultImage( const char* filename, UINT size, UINT* pData );
			static bool GetDeviceCanRotate();
			static bool GetAGKShouldRotate();
			static bool GetDeviceHasSplash();

			static UINT PlatformLittleEndian( UINT u );
			static int PlatformLittleEndian( int i );
			static UINT PlatformLocalEndian( UINT u );
			static int PlatformLocalEndian( int i );

			static void UpdatePhysicsWalls();
			static void ParseCurrentDirectory();
			static void ResetScissor();
		
			//static void ConvertMediaPath( uString &sPath );

			static cHashedList<cImage> m_cImageList;
			static cHashedList<cSprite> m_cSpriteList;
			static cHashedList<cText> m_cTextList;
			static cHashedList<AGKFont> m_cFontList;
			static cHashedList<b2Joint> m_cJointList;
			static cHashedList<cForce> m_cForceList;
			static cHashedList<cFile> m_cFileList;
			static cHashedList<cFolder> m_cFolderList;
			static cHashedList<cNetwork> m_cNetworkList;
			static cHashedList<cNetworkMessage> m_cNetMessageList;
			static cHashedList<cHTTPConnection> m_cHTTPList;
			static cHashedList<BroadcastListener> m_cBroadcastListenerList;
			static cHashedList<AGKSocket> m_cSocketList;
			static cHashedList<cNetworkListener> m_cSocketListenerList;
			static cHashedList<UDPManager> m_cUDPListenerList;
			static cHashedList<cParticleEmitter> m_cParticleEmitterList;
			static cHashedList<cEditBox> m_cEditBoxList;
			static cHashedList<ZipFile> m_cZipFileList;
			static cHashedList<cMemblock> m_cMemblockList;
			static cHashedList<Skeleton2D> m_cSkeleton2DList;
			static cHashedList<Tween> m_cTweenList;
			static cHashedList<TweenChain> m_cTweenChainList;

			static cHashedList<cObject3D> m_cObject3DList;
			static cHashedList<cCamera> m_cCameraList;
			static cHashedList<AGKShader> m_cShaderList;
			static cHashedList<AGK3DParticleEmitter> m_3DParticleEmitterList;

			// music
			static cHashedList<AGKMusicOGG> m_cMusicOGGList;

			// lights
			static cHashedList<AGKPointLight> m_cPointLightList;
			static AGKDirectionalLight m_cDirectionalLight;
			static AGKVector m_AmbientColor;

			// skybox
			static AGKSkyBox *m_pSkyBox;

			//static cSpriteMgr m_cSpriteMgr;
			static cSpriteMgrEx m_cSpriteMgrFront;
			static cObjectMgr m_cObjectMgr;

			// error variables
			static int m_iErrorMode;
			static uString m_sLastError;
			static bool m_bErrorOccurred;
			static cLock m_kErrorLock;
			static void(*m_fWarningCallback)(const char*); 
			static void(*m_fErrorCallback)(const char*);
			static void(*m_fDebugCallback)(const char*, const char*);

			// misc variables
			static int m_iPrintSizeChanged;
			static cText *m_pPrintText;
			static uString m_cPrintStr;
			static char m_szConvStr[ 20 ]; // string used to temporarily hold floats and ints converted to strings
			static bool m_bGenerateMipmaps;
			static cSprite *m_pLastColCheck;
			static cSprite *m_pLastColCheck2;
			static UINT m_iRandSeed;
			static float m_fViewOffsetX;
			static float m_fViewOffsetY;
			static float m_fViewZoom;
			static int m_iViewZoomMode;
			static cSprite *m_pSplashSprite;
			static bool m_bFirstSync;
			static float m_fSyncTime;
			static int m_iSyncMode;
			static bool m_bOrientationSet;
			static int m_iCurrentOrientationMode;
			static uString m_sPNToken;

			// mersenne twister RNG
			static UINT m_iRandMTArray[624];
			static int m_iRandMTIndex;

			static int m_iDrawingCount;

			static uString m_sCurrentDir;
			static uString m_sOrigSetDir;

			static b2World *m_phyWorld;
			static b2Body *m_phyGroundBody;
			static MyDestructionListener *m_DestructionListener;

			static b2Body *groundBodyBottom;
			static b2Body *groundBodyRight;
			static b2Body *groundBodyLeft;
			static b2Body *groundBodyTop;

			static float m_phyScale;
			static bool m_bDrawDebug;
			static int m_iVelIterations;
			static int m_iPosIterations;
			static bool m_bPhysicsStepped;
			static b2Contact *m_pContactIter;
			static cSprite *m_pSpriteContactIter;

			static int m_iMaxPolygonPoints;

			// benchmarking
			static float m_fUpdateTime;
			static float m_fPhysicsTime;
			static float m_fDrawingSetupTime;
			static float m_f3DDrawingSetupTime;
			static float m_fDrawingTime;

			static int m_iNumProcessors;

		public:

			static int IsSGX540GPU() { return m_iSGX540GPU; }
        
			static int GetRealPath( uString &inout );
            static void PlatformGetFullPathWrite( uString &inout );
            static void PlatformGetFullPathRead( uString &inout, int mode=0 );
			static int PlatformCreateRawPath( const char *path );
					
			// *********************************
			// public items, not for general use
			// *********************************

			static void DialogGroup( const char* group );

			static void SetCompanyName( const char* company );
			static void SetAppName( const char* name );
			static void RestoreWriteDir( void );
			static void OverrideDirectories( const char *szWrite, int useRead );
			static void MasterReset();

			static char* CreateString( unsigned int size );
			static void DeleteString( char* str );

			// internal public functions, called by automated processes on the device
			static void SetResolutionMode( int mode );
			static void SetExtraAGKPlayerAssetsMode ( int mode );
            static void InitJoysticks();
			static void InitExternal( void *ptr, int width, int height, void(*swap)(void*), void* swapParam ); // ptr is reserved, must be 0
			static void InitGL( void* ptr );
			static void InitConsole();
			static void InitFilePaths();
			static void UpdatePtr( void* ptr );
			static void UpdatePtr2( void* ptr ); // does minimal amount of changes
			static void WindowMoved();
			static void ResetAllStates ( void );
			static void CleanUp();
			static void CompositionChanged();
			static void PNToken( const char* token );
			static int GetInternalDataI( int index );
		
			static void OrientationChanged( int mode );
			static int CanOrientationChange( int mode );
			static void SetOrientationMatrix();

			static void AppPausing();
			static void AppResuming();
			static void LosingContext();
			static void Paused();
			static void Resumed( int mode=1 );
			static void ResumedOpenGL( int mode=1 );

			static int CanResize() { return (m_bAGKFlags & AGK_FLAG_CAN_RESIZE) ? 1 : 0; }

			// touch input commands, the device stub file will need to call these to notify the AGK about input events
			static void TouchPressed(UINT ID, int x, int y);
			static void TouchMoved(UINT ID, int x, int y);
			static void TouchReleased(UINT ID, int x, int y);
		
			static void MouseLeftButton(UINT ID, int state);
			static void MouseRightButton(UINT ID, int state);
			static void MouseMiddleButton(UINT ID, int state);
			static void MouseFourthButton(uint32_t ID, int state);
			static void MouseFifthButton(uint32_t ID, int state);
			static void MouseMove(UINT ID, int x, int y);
			static void MouseWheel(UINT ID, float w);
		
			static void KeyDown( UINT index );
			static void KeyUp( UINT index );
			static void CharDown( UINT c );

			// set physical or virtual keyboard
			static void KeyboardMode( int mode );

			// sensors
			static void Accelerometer( float x, float y, float z );
			static void Gyro( float x, float y, float z );
			static void Proximity( float distance );
			static void LightSensor( float light );
			static void Magnetic( float x, float y, float z );
			static void RotationSensor( float x, float y, float z, float w );
			static void GPS( float longitude, float latitude, float altitude );
		
			// joysticks
			static void JoystickAxis( void* pDevice, int stick, int axis, float value );
			static void JoystickButton( void* pDevice, int button, int state );
			
			// input related functions
			static float DeviceToDisplayRatioX();
			static float DeviceToDisplayRatioY();
			static float DeviceToScreenX( int x );
			static float DeviceToScreenY( int y );
			static int ScreenToDeviceX( float x );
			static int ScreenToDeviceY( float y );
			static float ViewportToScreenX( int x );
			static float ViewportToScreenY( int y );
			static int ScreenToViewportX( float x );
			static int ScreenToViewportY( float y );
			static float RotatePointX( float x, float y );
			static float RotatePointY( float x, float y );

			static float ScreenToWorldX( float x );
			static float ScreenToWorldY( float y );
			static float WorldToScreenX( float x );
			static float WorldToScreenY( float y );

			static float GetStretchValue() { return m_fStretchValue; }

			// tell the AGK about the current device volume (if, as in the case of Samsung, the AGK is required to handle this itself)
			static void DeviceVolume( int vol );
			static int GetDeviceVolume() { return m_iDeviceVolume; }

			// sound input commands, the device stub file will need to call these to notify the GDK about sound events
			static void HandleMusicEvents( void *ptr );
			static void HandleVideoEvents();
			static void ChangeVideoPointer( void *ptr );

			static float PhyToWorldX( float value ) { return value / m_phyScale; }
			static float PhyToWorldY( float value ) { return (value * m_fStretchValue) / m_phyScale; }
			static float WorldToPhyX( float value ) { return value * m_phyScale; }
			static float WorldToPhyY( float value ) { return (value / m_fStretchValue) * m_phyScale; }

			static void ClearJoint( b2Joint *pJoint );

			static bool IsRightHand() { return m_bRightHand; }
        
            // social command helpers
            static int FacebookHandleOpenURL( void* url );
            static void HandleDeepLink( const char* link );

			// capabilities
			static int CanUseIntIndices();
			static int CanUseShadowSamplers();

		public:

			// *************************
			// public items
			// *************************

			static int m_iAccelerometerExists;
			static int m_iGyroSensorExists;
			static int m_iProximitySensorExists;
			static int m_iLightSensorExists;
			static int m_iMagneticSensorExists;
			static int m_iRotationSensorExists;
			static int m_iGPSSensorExists;

			// core functions
			static void AddVulkanDeviceExtensions( const char* szExtensions );
			static void AddVulkanInstanceExtensions( const char* szExtensions );
			static char* GetRendererName();
			static void SetScreenResolution( int width, int height );
			static void SetWindowPosition( int x, int y );
			static void SetWindowSize( int width, int height, int fullscreen );
			static void SetWindowSize( int width, int height, int fullscreen, int allowOverSized );
			static void SetWindowAllowResize( int mode );
			static void MaximizeWindow();
			static void MinimizeApp();
			static void RestoreApp();
			static int GetWindowWidth();
			static int GetWindowHeight();
			static void SetImmersiveMode( int mode );
			static int GetDeviceWidth();
			static int GetRealDeviceWidth();
			static int GetDeviceHeight();
			static int GetRealDeviceHeight();
			static int GetMaxDeviceWidth();
			static int GetMaxDeviceHeight();
			static int GetDeviceDPI();
			static char* GetAppPackageName();
			static int GetDevicePlatform();
			static void SetVSync( int mode );
			static void SetSyncRate( float fps, int mode );
			static void SetVirtualResolution( int iWidth, int iHeight );
			static void SetDisplayAspect( float aspect );
			static int GetVirtualWidth();
			static int GetVirtualHeight();
			static float GetScreenBoundsLeft();
			static float GetScreenBoundsRight();
			static float GetScreenBoundsTop();
			static float GetScreenBoundsBottom();
			static float GetDisplayAspect();
			static int GetOrientation();
			static int GetPaused();
			static int GetResumed();
			static int GetResumed2();
			static char* GetURLSchemeText();
			static void ClearURLSchemeText();
			static char* GetDeviceName();
			static void GetDeviceName( uString &sString );
			static char* GetDeviceBaseName();
			static char* GetDeviceType();
			static int GetDeviceNetworkType();
			static int GetStorageRemaining( const char *path );
			static int GetStorageTotal( const char *path );
			static char* GetAppName();
			static char* GetCL();
			static void GetAppName( uString &sString );
			static void SetWindowTitle( const char* szTitle );
			static void UpdateDeviceSize(UINT w=0, UINT h=0);
			static void StepPhysics( float time );
			static void Sync();
			static void Break();
			static void Update( float time=0 );
			static void Update2D( float time=0 );
			static void Update3D( float time=0 );
			static void DrawAllLines();
			static void Render2DBack();
			static void Render2DFront();
			static void Render3D();
			static void RenderShadowMap();
			static void Render();
			static void Swap();
			static void UpdateInput();
			static void ClearScreen();
			static void ClearDepthBuffer();
			static void Sleep( UINT milliseconds );
			static float Timer();
			static void ResetTimer();
			static double PlatformGetRawTime();
			static float GetRunTime();
			static int GetSeconds();
			static float GetFrameTime();
			static UINT GetMilliseconds();
			static char* GetDeviceLanguage();
			static void SetSleepMode( int mode );
			static void SetAntialiasMode( int mode );

			static void SetExpansionFileKey( const char *key );
			static void SetExpansionFileVersion(int version);
			static int GetExpansionFileState();
			static int GetExpansionFileError();
			static void DownloadExpansionFile();
			static float GetExpansionFileProgress();
			static bool ExtractExpansionFile( const char* localFile, const char* expansionFile );

			static char* GetDeviceID();
			static void GetDeviceID( uString &out );
			static char* Sha1( const char* str );
			static void Sha1( const char* str, uString &out );
			static char* Sha256( const char* str );
			static char* Sha512( const char* str );
			static char* HTTPEncode( const char* str );
			static void HTTPEncode( const char* str, uString &out );
			static char* HTTPDecode( const char* str );
			static void HTTPDecode( const char* str, uString &out );
			static int GetNumProcessors();

			static int SortArray( AGKSortValue* pArray, UINT size );
			static UINT SortFloatToUINT( float f );
			static float SortUINTToFloat( UINT u );
			static UINT SortIntToUINT( int f );
			static int SortUINTToInt( UINT u );

			// math functions
			static void SetRandomSeed( UINT seed );
			static UINT Random( );
			static UINT Random( UINT from, UINT to );
			static int RandomSign( int value );
			static void SetRandomSeed2( UINT seed );
			static int Random2( );
			static int Random2( int from, int to );
			static float Sin( float a );
			static float Cos( float a );
			static float Tan( float a );
			static float SinRad( float a );
			static float CosRad( float a );
			static float TanRad( float a );
			static float ASin( float a );
			static float ACos( float a );
			static float ATan( float a );
			static float ATan2( float y, float x );
			static float ATanFull( float x, float y );
			static float ASinRad( float a );
			static float ACosRad( float a );
			static float ATanRad( float a );
			static float ATan2Rad( float y, float x );
			static float ATanFullRad( float x, float y );
			static int Trunc( float a );
			static int Floor( float a );
			static int Ceil( float a );
			static int Round( float a );
			static float Sqrt( float a );
			static float Abs( float a );
			static int Mod( int a, int b );
			static float FMod( float a, float b );
			static float Pow( float a, float b );
			static float Log( float a );

			// control functions
			static void SetAlphaTestThreshold( UINT threshold );
			static void SetGenerateMipmaps( int generate );
			static void SetOrientationAllowed( int portrait, int portrait2, int landscape, int landscape2 );
			static void SetTransitionMode( int mode );
			static void SetSortTextures( int sort );
			static void SetSortDepth( int sort );
			static void SetSortTransparentDepth( int sort );
			static void SetSortCreated( int sort );
			static float ScreenFPS();
			static UINT GetPolygonsDrawn();
			static UINT GetShadowPolygonsDrawn();
			static UINT GetVerticesProcessed();
			static void EnableClearDepth( UINT clear );
			static void EnableClearColor( UINT clear );
			static void SetBorderColor( UINT red, UINT green, UINT blue );
			static void SetClearColor( UINT red, UINT green, UINT blue );
			static void BindDefaultFramebuffer();
			static void SetDefaultMagFilter( UINT filter );
			static void SetDefaultMinFilter( UINT filter );
			static void SetDefaultWrapU( UINT mode );
			static void SetDefaultWrapV( UINT mode );
			static void SetViewOffset( float x, float y );
			static void SetViewZoom( float zoom );
			static void SetViewZoomMode( int mode );
			static float GetViewOffsetX();
			static float GetViewOffsetY();
			static float GetViewZoom();
			static void SetScissor( float x, float y, float x2, float y2 );
			static void SetIntendedDeviceSize( int width, int height );
			static void UseNewDefaultFonts( int mode );
			
			// functions that fail call this function
			static void DebugInfo( const char *type, const char *data );
			static void PluginError( const char* szErr );
			static void Error( const uString &sMsg );
			static void Warning( const uString &sMsg );
			static void SetErrorMode( int mode );
			static int GetLastError( uString &sOut );
			static char* GetLastError();
			static int GetErrorOccurred();
			static void SetWarningCallback( void(*callback)(const char*) );
			static void SetErrorCallback( void(*callback)(const char*) );
			static void SetDebugCallback( void(*callback)(const char*, const char*) );

			static void Message( const char* msg );
			
			// image functions
			static UINT GetImageSizeFromFile( const char* filename );
			static cImage* GetImagePtr ( UINT iImageIndex );
			static UINT GetImageTextureID ( UINT iImageIndex );
			static uint32_t GetOpenGLImageID ( uint32_t iImageIndex );
			static void* GetVulkanVRImageData ( uint32_t iImageIndex );
			static void SetVRImage ( uint32_t iImageIndex, int mode );
			static void LoadImage ( UINT iImageIndex, const char* sImageFilename, int bBlackToAlpha );
			static UINT LoadImage ( const char* sImageFilename, int bBlackToAlpha );
			static void LoadImage ( UINT iImageIndex, const char* sImageFilename );
			static UINT LoadImage ( const char* sImageFilename );
			static void LoadSubImage ( UINT iImageIndex, UINT iParentIndex, const char* sImageFilename );
			static UINT LoadSubImage ( UINT iParentIndex, const char* sImageFilename );
			static void LoadImageResized( UINT iImageID, const char* szFilename, float scaleX, float scaleY, int cache );
			static UINT LoadImageResized( const char* szFilename, float scaleX, float scaleY, int cache );
			static void PreCacheImage( const char* szFilename, float scaleX, float scaleY );
			static int GetCachingFinished();
			static void CreateImageColor( UINT imageID, UINT red, UINT green, UINT blue, UINT alpha );
			static UINT CreateImageColor( UINT red, UINT green, UINT blue, UINT alpha );
			static void CreateRenderImage( UINT imageID, UINT width, UINT height, UINT format, UINT mipmap );
			static UINT CreateRenderImage( UINT width, UINT height, UINT format, UINT mipmap );
			static UINT GetImageExists ( UINT iImageIndex );
			static void DeleteImage ( UINT iImageIndex );
			static void DeleteAllImages();
			static float GetImageWidth ( UINT iImageIndex );
			static float GetImageHeight ( UINT iImageIndex );
			static void SetImageMinFilter( UINT iImageIndex, UINT mode );
			static void SetImageMagFilter( UINT iImageIndex, UINT mode );
			static void SetImageWrapU( UINT iImageIndex, UINT mode );
			static void SetImageWrapV( UINT iImageIndex, UINT mode );
			static void SetImageMask( UINT iDstImage, UINT iSrcImage, int dst, int src, int x, int y );
			static void SaveImage( UINT iImageIndex, const char* filename );
			static void CopyImage( UINT newImage, UINT fromImage, int x, int y, int width, int height );
			static UINT CopyImage( UINT fromImage, int x, int y, int width, int height );
			static UINT GetImage( float x, float y, float width, float height );
			static void GetImage( UINT imageID, float x, float y, float width, float height );
			static char* GetImageFilename( UINT imageID );
            static void SetImageSavePixels( int mode );
			static void SetImageTransparentColor( UINT iImage, int r, int g, int b );
			static void ResizeImage( UINT image, int width, int height );

            static UINT ChooseImage();
            static UINT ShowChooseImageScreen();
            static void ChooseInternal(int value);
            static UINT GetChosenImage();
            static UINT IsChoosingImage();
		
            static UINT ShowImageCaptureScreen();
			static void CaptureImage();
            static void CaptureInternal(int value);
			static void CancelCapture();
			static UINT GetCapturedImage();
			static UINT IsCapturingImage();

			static int GetNumDeviceCameras();
			static int SetDeviceCameraToImage( UINT cameraID, UINT imageID );
			static void DeviceCameraUpdate();
			static void DeviceCameraResumed();
			static int GetDeviceCameraType( UINT cameraID );
            static cImage* GetDeviceCameraImage() { return m_pCameraTexture; }
        
			static char* DecodeQRCode( UINT image );
			static UINT EncodeQRCode( const char* text, int errormode );
        
			static void PrintImage( UINT image, float size );

			// render functions
			static void SetRenderToImage( int colorImage, int depthImage );
			static void SetRenderToScreen();
			static UINT IsSupportedDepthTexture();
			static int GetSupportedShaderVaryings();
			
			// sprite functions
			static cSprite* GetSpritePtr ( UINT iSpriteIndex );
			static UINT GetSpriteImageID ( UINT iSpriteIndex );
			static UINT CreateSprite ( UINT iImageIndex );
			static UINT LoadSprite ( const char* imagefile );
			static void CreateSprite ( UINT iSpriteIndex, UINT iImageIndex );
			static void LoadSprite ( UINT iSpriteIndex, const char* imagefile );
			static void CreateDummySprite ( UINT iSpriteIndex );
			static UINT CreateDummySprite ( );
			static void CloneSprite( UINT iSpriteIndex, UINT iOtherSprite );
			static UINT CloneSprite( UINT iOtherSprite );
			static void DeleteSprite ( UINT iSpriteIndex );
			static void DeleteAllSprites();
			static void SetSpritePosition ( UINT iSpriteIndex, float fX, float fY );
			static void SetSpritePositionByOffset ( UINT iSpriteIndex, float fX, float fY );
			static void SetSpriteX ( UINT iSpriteIndex, float fX );
			static void SetSpriteY ( UINT iSpriteIndex, float fY );
			static void SetSpriteAngle ( UINT iSpriteIndex, float fAng );
			static void SetSpriteAngleRad ( UINT iSpriteIndex, float fAng );
			static void SetSpriteDepth ( UINT iSpriteIndex, int iDepth );
			static void SetSpriteFlip ( UINT iSpriteIndex, int horz, int vert );
			static void SetSpriteSnap ( UINT iSpriteIndex, int snap );
			static void SetSpriteVisible ( UINT iSpriteIndex, int bVisible );
			static void SetSpriteActive ( UINT iSpriteIndex, int bActive );
			static void SetSpriteImage ( UINT iSpriteIndex, UINT iImageIndex );
			static void SetSpriteImage ( UINT iSpriteIndex, UINT iImageIndex, int bUpdateShape );
			static void SetSpriteAdditionalImage ( UINT iSpriteIndex, UINT iImageIndex, int iStage );
			static void SetSpriteUV ( UINT iSpriteIndex, float u1, float v1, float u2, float v2, float u3, float v3, float u4, float v4 );
			static void ResetSpriteUV( UINT iSpriteIndex );
			
			static UINT GetSpriteExists ( UINT iSpriteIndex );
			static float GetSpriteX ( UINT iSpriteIndex );
			static float GetSpriteY ( UINT iSpriteIndex );
			static float GetSpriteXByOffset ( UINT iSpriteIndex );
			static float GetSpriteYByOffset ( UINT iSpriteIndex );
			static float GetSpriteOffsetX ( UINT iSpriteIndex );
			static float GetSpriteOffsetY ( UINT iSpriteIndex );
			static float GetSpriteWidth ( UINT iSpriteIndex );
			static float GetSpriteHeight ( UINT iSpriteIndex );
			static float GetSpriteAngle ( UINT iSpriteIndex );
			static float GetSpriteAngleRad ( UINT iSpriteIndex );
			static float GetSpriteScaleX ( UINT iSpriteIndex );
			static float GetSpriteScaleY ( UINT iSpriteIndex );
			static int GetSpriteHitTest ( UINT iSpriteIndex, float x, float y );
			static int GetSpriteDepth( UINT iSpriteIndex );
			static UINT GetSpriteColorRed( UINT iSpriteIndex );
			static UINT GetSpriteColorGreen( UINT iSpriteIndex );
			static UINT GetSpriteColorBlue( UINT iSpriteIndex );
			static UINT GetSpriteColorAlpha( UINT iSpriteIndex );
			static int GetSpritePlaying( UINT iSpriteIndex );
			static int GetSpriteVisible( UINT iSpriteIndex );
			static int GetSpriteActive( UINT iSpriteIndex );
			static int GetSpriteGroup( UINT iSpriteIndex );
			static int GetSpriteTransparency( UINT iSpriteIndex );
			static int GetSpriteFlippedH(UINT iSpriteIndex);
			static int GetSpriteFlippedV(UINT iSpriteIndex);
			static int GetSpriteInScreen( UINT iSpriteIndex );

			static float GetSpriteXFromPixel( UINT iSpriteIndex, int x );
			static float GetSpriteYFromPixel( UINT iSpriteIndex, int y );
			static int GetSpritePixelFromX( UINT iSpriteIndex, float x );
			static int GetSpritePixelFromY( UINT iSpriteIndex, float y );

			static float GetWorldXFromSprite( UINT iSpriteIndex, float x, float y );
			static float GetWorldYFromSprite( UINT iSpriteIndex, float x, float y );
			static float GetSpriteXFromWorld( UINT iSpriteIndex, float x, float y );
			static float GetSpriteYFromWorld( UINT iSpriteIndex, float x, float y );
			
			static void SetSpriteSize( UINT iSpriteIndex, float width=-1, float height=-1 );
			static void SetSpriteAnimation( UINT iSpriteIndex, int iFrameWidth, int iFrameHeight, int iFrameCount );
			static void AddSpriteAnimationFrame( UINT iSpriteIndex, UINT iImageIndex );
			static void ClearSpriteAnimationFrames( UINT iSpriteIndex );
			static void SetSpriteSpeed( UINT iSpriteIndex, float fFps );
			static void PlaySprite( UINT iSpriteIndex );
			static void PlaySprite( UINT iSpriteIndex, float fFps );
			static void PlaySprite( UINT iSpriteIndex, float fFps, int iLoop );
			static void PlaySprite( UINT iSpriteIndex, float fFps, int iLoop, int iFromFrame, int iToFrame );
			static void StopSprite( UINT iSpriteIndex );
			static void ResumeSprite( UINT iSpriteIndex );
			static void SetSpriteFrame( UINT iSpriteIndex, int iFrame );
			static int GetSpriteCurrentFrame( UINT iSpriteIndex );
			static int GetSpriteFrameCount( UINT iSpriteIndex );
			static void SetSpriteTransparency( UINT iSpriteIndex, int mode );
			static void SetSpriteOffset( UINT iSpriteIndex, float x, float y );
			static void SetSpriteColor( UINT iSpriteIndex, int iRed, int iGreen, int iBlue, int iAlpha );
			static void SetSpriteColorRed( UINT iSpriteIndex, int iRed );
			static void SetSpriteColorGreen( UINT iSpriteIndex, int iGreen );
			static void SetSpriteColorBlue( UINT iSpriteIndex, int iBlue );
			static void SetSpriteColorAlpha( UINT iSpriteIndex, int iAlpha );
			static void SetSpriteUVBorder( UINT iSpriteIndex, float border );
			static void SetSpriteUVOffset( UINT iSpriteIndex, float u, float v );
			static void SetSpriteUVScale( UINT iSpriteIndex, float scaleU, float scaleV );
			static void FixSpriteToScreen( UINT iSpriteIndex, int mode );

			static void SetSpriteScale( UINT iSpriteIndex, float x, float y );
			static void SetSpriteScaleByOffset( UINT iSpriteIndex, float x, float y );
			static void SetSpriteScissor( UINT iSpriteIndex, float x, float y, float x2, float y2 );
			static void SetSpriteShader( UINT spriteID, UINT shaderID );

			static void SetSpriteGroup( UINT iSpriteIndex, int group );
			static void SetSpriteCategoryBits( UINT iSpriteIndex, UINT categories );
			static void SetSpriteCategoryBit( UINT iSpriteIndex, UINT category, int flag );
			static void SetSpriteCollideBits( UINT iSpriteIndex, UINT mask );
			static void SetSpriteCollideBit( UINT iSpriteIndex, UINT category, int flag );

			static void SetSpriteGroup( UINT iSpriteIndex, int group, int shapeID );
			static void SetSpriteCategoryBits( UINT iSpriteIndex, UINT categories, int shapeID );
			static void SetSpriteCategoryBit( UINT iSpriteIndex, UINT category, int flag, int shapeID );
			static void SetSpriteCollideBits( UINT iSpriteIndex, UINT mask, int shapeID );
			static void SetSpriteCollideBit( UINT iSpriteIndex, UINT category, int flag, int shapeID );

			static void DrawSprite( UINT iSpriteIndex );
			
			// physics commands
			static void SetPhysicsScale( float scale );
			static void SetPhysicsGravity( float x, float y );
			static void SetPhysicsDebugOn();
			static void SetPhysicsDebugOff();
			static void SetPhysicsThreading( int threads );
			static void SetPhysicsCCD( int mode );
			static void SetPhysicsSleeping( int mode );
			static void SetPhysicsMaxPolygonPoints( int points );
			static b2World* GetPhysicsWorld();

			//physics benchmarking
			static float GetPhysicsSolveTime();
			static int GetPhysicsIslandCount();

			static void SetPhysicsWallTop( int mode );
			static void SetPhysicsWallLeft( int mode );
			static void SetPhysicsWallRight( int mode );
			static void SetPhysicsWallBottom( int mode );
			
			// global forces
			static UINT CreatePhysicsForce( float x, float y, float power, float limit, float range, int fade );
			static void DeletePhysicsForce( UINT iForceIndex );
			static void SetPhysicsForcePosition( UINT iForceIndex, float x, float y );
			static void SetPhysicsForcePower( UINT iForceIndex, float power );
			static void SetPhysicsForceRange( UINT iForceIndex, float range );

			// sprites
			static void SetSpriteShape( UINT iSpriteIndex, int shape );
			static void SetSpriteShapeBox( UINT iSpriteIndex, float x, float y, float x2, float y2, float angle );
			static void SetSpriteShapeCircle( UINT iSpriteIndex, float x, float y, float radius );
			static void SetSpriteShapePolygon( UINT iSpriteIndex, UINT numPoints, UINT index, float x, float y );
			
			static void SetSpriteShape( UINT iSpriteIndex, int shape, int shapeID );
			static void SetSpriteShapeBox( UINT iSpriteIndex, float x, float y, float x2, float y2, float angle, int shapeID );
			static void SetSpriteShapeCircle( UINT iSpriteIndex, float x, float y, float radius, int shapeID );
			static void SetSpriteShapePolygon( UINT iSpriteIndex, UINT numPoints, UINT index, float x, float y, int shapeID );
			static void SetSpriteShapeChain( UINT iSpriteIndex, UINT numPoints, UINT index, int loop, float x, float y, int shapeID );
			
			static void AddSpriteShapeBox( UINT iSpriteIndex, float x, float y, float x2, float y2, float angle=0 );
			static void AddSpriteShapeCircle( UINT iSpriteIndex, float x, float y, float radius );
			static void AddSpriteShapePolygon( UINT iSpriteIndex, UINT numPoints, UINT index, float x, float y );
			static void AddSpriteShapeChain( UINT iSpriteIndex, UINT numPoints, UINT index, int loop, float x, float y );

			static void ClearSpriteShapes( UINT iSpriteIndex );
			static int GetSpriteNumShapes( UINT iSpriteIndex );

			static int GetSpriteShapeNumVertices( UINT iSpriteIndex, int shapeID );
			static float GetSpriteShapeVertexX( UINT iSpriteIndex, int shapeID, int vertex );
			static float GetSpriteShapeVertexY( UINT iSpriteIndex, int shapeID, int vertex );

			static void SetSpritePhysicsCOM( UINT iSpriteIndex, float x, float y );
			static void CalculateSpritePhysicsCOM( UINT iSpriteIndex );
			static float GetSpritePhysicsCOMX( UINT iSpriteIndex );
			static float GetSpritePhysicsCOMY( UINT iSpriteIndex );

			static void PrepareToDeleteSpriteContacts( cSprite *pSprite );
			static void SetSpritePhysicsOn( UINT iSpriteIndex, int mode=2 );
			static void SetSpritePhysicsOff( UINT iSpriteIndex );
			static void SetSpritePhysicsDelete( UINT iSpriteIndex );
			static void SetSpritePhysicsFriction( UINT iSpriteIndex, float friction );
			static void SetSpritePhysicsRestitution( UINT iSpriteIndex, float restitution );
			static void SetSpritePhysicsFriction( UINT iSpriteIndex, float friction, int shapeID );
			static void SetSpritePhysicsRestitution( UINT iSpriteIndex, float restitution, int shapeID );
			static void SetSpritePhysicsDensity( UINT iSpriteIndex, float density, int shapeID );
			static void SetSpritePhysicsCanRotate( UINT iSpriteIndex, int rotate );
			static void SetSpritePhysicsVelocity( UINT iSpriteIndex, float vx, float vy );
			static void SetSpritePhysicsAngularVelocity( UINT iSpriteIndex, float va );
			static void SetSpritePhysicsDamping( UINT iSpriteIndex, float damp );
			static void SetSpritePhysicsAngularDamping( UINT iSpriteIndex, float damp );
			static void SetSpritePhysicsIsBullet( UINT iSpriteIndex, int bullet );
			static void SetSpritePhysicsMass( UINT iSpriteIndex, float mass );
			static void SetSpritePhysicsIsSensor( UINT iSpriteIndex, int sensor );
			static void SetSpritePhysicsIsSensor( UINT iSpriteIndex, int sensor, int shapeID );
			
			static void SetSpritePhysicsForce( UINT iSpriteIndex, float x, float y, float vx, float vy );
			static void SetSpritePhysicsTorque( UINT iSpriteIndex, float torque );
			static void SetSpritePhysicsImpulse( UINT iSpriteIndex, float x, float y, float vx, float vy );
			static void SetSpritePhysicsAngularImpulse( UINT iSpriteIndex, float impulse );

			static float GetSpritePhysicsVelocityX( UINT iSpriteIndex );
			static float GetSpritePhysicsVelocityY( UINT iSpriteIndex );
			static float GetSpritePhysicsAngularVelocity( UINT iSpriteIndex );
			static float GetSpritePhysicsMass( UINT iSpriteIndex );

			// joints
			static void DeleteJoint( UINT iJointIndex );
			static int GetJointExists( UINT iJointIndex );
			static void CreateDistanceJoint( UINT iJointIndex, cSprite *pSprite1, cSprite *pSprite2, float x, float y, float x2, float y2, int colConnected );
			static void CreateRevoluteJoint( UINT iJointIndex, cSprite *pSprite1, cSprite *pSprite2, float x, float y, int colConnected );
			static void CreatePrismaticJoint( UINT iJointIndex, cSprite *pSprite1, cSprite *pSprite2, float x, float y, float vx, float vy, int colConnected );
			static void CreatePulleyJoint( UINT iJointIndex, cSprite *pSprite1, cSprite *pSprite2, float gnd1x, float gnd1y, float gnd2x, float gnd2y, float a1x, float a1y, float a2x, float a2y, float ratio, int colConnected );
			static void CreateMouseJoint( UINT iJointIndex, cSprite *pSprite1, float x, float y, float maxForce );
			static void CreateLineJoint( UINT iJointIndex, cSprite *pSprite1, cSprite *pSprite2, float x, float y, float vx, float vy, int colConnected );
			static void CreateWeldJoint( UINT iJointIndex, cSprite *pSprite1, cSprite *pSprite2, float x, float y, int colConnected );
			static void CreateRopeJoint( UINT iJointIndex, cSprite *pSprite1, cSprite *pSprite2, float x, float y, float x2, float y2, float maxLength, int colConnected );
			
			static void CreateDistanceJoint( UINT iJointIndex, UINT iSpriteIndex1, UINT iSpriteIndex2, float x, float y, float x2, float y2, int colConnected );
			static void CreateRevoluteJoint( UINT iJointIndex, UINT iSpriteIndex1, UINT iSpriteIndex2, float x, float y, int colConnected );
			static void CreatePrismaticJoint( UINT iJointIndex, UINT iSpriteIndex1, UINT iSpriteIndex2, float x, float y, float vx, float vy, int colConnected );
			static void CreatePulleyJoint( UINT iJointIndex, UINT iSpriteIndex1, UINT iSpriteIndex2, float gnd1x, float gnd1y, float gnd2x, float gnd2y, float a1x, float a1y, float a2x, float a2y, float ratio, int colConnected );
			static void CreateMouseJoint( UINT iJointIndex, UINT iSpriteIndex1, float x, float y, float maxForce );
			static void CreateLineJoint( UINT iJointIndex, UINT iSpriteIndex1, UINT iSpriteIndex2, float x, float y, float vx, float vy, int colConnected );
			static void CreateWeldJoint( UINT iJointIndex, UINT iSpriteIndex1, UINT iSpriteIndex2, float x, float y, int colConnected );
			static void CreateRopeJoint( UINT iJointIndex, UINT iSpriteIndex1, UINT iSpriteIndex2, float x, float y, float x2, float y2, float maxLength, int colConnected );

			// special joint that joins two joints
			static void CreateGearJoint( UINT iJointIndex, UINT iJoint1, UINT iJoint2, float ratio );

			// returns joint ID
			static UINT CreateDistanceJoint( cSprite *pSprite1, cSprite *pSprite2, float x, float y, float x2, float y2, int colConnected );
			static UINT CreateRevoluteJoint( cSprite *pSprite1, cSprite *pSprite2, float x, float y, int colConnected );
			static UINT CreatePrismaticJoint( cSprite *pSprite1, cSprite *pSprite2, float x, float y, float vx, float vy, int colConnected );
			static UINT CreatePulleyJoint( cSprite *pSprite1, cSprite *pSprite2, float gnd1x, float gnd1y, float gnd2x, float gnd2y, float a1x, float a1y, float a2x, float a2y, float ratio, int colConnected );
			static UINT CreateMouseJoint( cSprite *pSprite1, float x, float y, float maxForce );
			static UINT CreateLineJoint( cSprite *pSprite1, cSprite *pSprite2, float x, float y, float vx, float vy, int colConnected );
			static UINT CreateWeldJoint( cSprite *pSprite1, cSprite *pSprite2, float x, float y, int colConnected );
			static UINT CreateRopeJoint( cSprite *pSprite1, cSprite *pSprite2, float x, float y, float x2, float y2, float maxLength, int colConnected );
			
			static UINT CreateDistanceJoint( UINT iSpriteIndex1, UINT iSpriteIndex2, float x, float y, float x2, float y2, int colConnected );
			static UINT CreateRevoluteJoint( UINT iSpriteIndex1, UINT iSpriteIndex2, float x, float y, int colConnected );
			static UINT CreatePrismaticJoint( UINT iSpriteIndex1, UINT iSpriteIndex2, float x, float y, float vx, float vy, int colConnected );
			static UINT CreatePulleyJoint( UINT iSpriteIndex1, UINT iSpriteIndex2, float gnd1x, float gnd1y, float gnd2x, float gnd2y, float a1x, float a1y, float a2x, float a2y, float ratio, int colConnected );
			static UINT CreateMouseJoint( UINT iSpriteIndex1, float x, float y, float maxForce );
			static UINT CreateLineJoint( UINT iSpriteIndex1, UINT iSpriteIndex2, float x, float y, float vx, float vy, int colConnected );
			static UINT CreateWeldJoint( UINT iSpriteIndex1, UINT iSpriteIndex2, float x, float y, int colConnected );
			static UINT CreateRopeJoint( UINT iSpriteIndex1, UINT iSpriteIndex2, float x, float y, float x2, float y2, float maxLength, int colConnected );

			static void CreatePulleyJoint2( UINT iSpriteIndex1, UINT iSpriteIndex2, float ratio, int colConnected );
			static UINT FinishPulleyJoint( float gnd1x, float gnd1y, float gnd2x, float gnd2y, float a1x, float a1y, float a2x, float a2y );

			// special joint that joins two joints
			static UINT CreateGearJoint( UINT iJoint1, UINT iJoint2, float ratio );

			static void SetJointLimitOn( UINT iJointIndex, float lowerLimit, float upperLimit );
			static void SetJointLimitOff( UINT iJointIndex );
			static void SetJointMotorOn( UINT iJointIndex, float speed, float maxForce );
			static void SetJointMotorOff( UINT iJointIndex );
			static void SetJointMouseTarget( UINT iJointIndex, float x, float y );
			static void SetJointMouseMaxForce( UINT iJointIndex, float maxForce );

			static float GetJointReactionForceX( UINT iJointIndex );
			static float GetJointReactionForceY( UINT iJointIndex );
			static float GetJointReactionTorque( UINT iJointIndex );

			static void SetJointDamping( UINT iJointIndex, float dampingRatio, float frequency );

			// ray casts
			static int PhysicsRayCast( float x, float y, float x2, float y2 );
			static int PhysicsRayCastGroup( int group, float x, float y, float x2, float y2 );
			static int PhysicsRayCastCategory( UINT category, float x, float y, float x2, float y2 );

			static int SpriteRayCast( float x, float y, float x2, float y2 );
			static int SpriteRayCastGroup( int group, float x, float y, float x2, float y2 );
			static int SpriteRayCastCategory( UINT category, float x, float y, float x2, float y2 );

			static int SpriteRayCastSingle( UINT sprite, float x, float y, float x2, float y2 );
			static int SpriteRayCastSingle( cSprite* sprite, float x, float y, float x2, float y2 );
			
			// ray cast return
			static UINT GetRayCastSpriteID( );
			static cSprite* GetRayCastSprite( );
			static float GetRayCastX();
			static float GetRayCastY();
			static float GetRayCastNormalX();
			static float GetRayCastNormalY();
			static float GetRayCastFraction();

			// non physics collision functions
			static int GetSpriteInBox( UINT iSprite1, float x1, float y1, float x2, float y2 );
			static int GetSpriteInCircle( UINT iSprite1, float x1, float y1, float radius );
			static int GetSpriteCollision( UINT iSprite1, UINT iSprite2 );

			static int GetSpriteHit( float x, float y );
			static int GetSpriteHitGroup( int group, float x, float y );
			static int GetSpriteHitCategory( UINT category, float x, float y );

			// non physics distance functions
			static float GetSpriteDistance( UINT iSprite1, UINT iSprite2 );
			static float GetSpriteDistancePoint1X( );
			static float GetSpriteDistancePoint1Y( );
			static float GetSpriteDistancePoint2X( );
			static float GetSpriteDistancePoint2Y( );

			// physics collision functions
			static int GetFirstContact();
			static int GetNextContact();
			static float GetContactWorldX();
			static float GetContactWorldY();
			static UINT GetContactSpriteID1();
			static UINT GetContactSpriteID2();
			static cSprite* GetContactSprite1();
			static cSprite* GetContactSprite2();

			// body contacts
			static int GetSpriteFirstContact( UINT iSprite1 );
			static int GetSpriteNextContact( );
			static float GetSpriteContactWorldX( );
			static float GetSpriteContactWorldY( );
			static UINT GetSpriteContactSpriteID2( );
			static cSprite* GetSpriteContactSprite2( );

			static int GetPhysicsCollision( UINT iSprite1, UINT iSprite2 );
			static float GetPhysicsCollisionX( );
			static float GetPhysicsCollisionY( );
			static float GetPhysicsCollisionWorldX( );
			static float GetPhysicsCollisionWorldY( );


			// particle commands
			static void CreateParticles( UINT ID, float x, float y );
			static UINT CreateParticles( float x, float y );
			static int GetParticlesExists( UINT ID );
			static void DeleteParticles( UINT ID );
			static void SetParticlesPosition( UINT ID, float x, float y );
			static void SetParticlesDepth( UINT ID, int depth );
			static void SetParticlesFrequency( UINT ID, float freq );
			static void SetParticlesStartZone( UINT ID, float x1, float y1, float x2, float y2 );
			static void SetParticlesDirection( UINT ID, float vx, float vy );
			static void SetParticlesVelocityRange( UINT ID, float v1, float v2 );
			static void SetParticlesAngle( UINT ID, float angle );
			static void SetParticlesAngleRad( UINT ID, float angle );
			static void SetParticlesRotationRange( UINT ID, float angle1, float angle2 );
			static void SetParticlesRotationRangeRad( UINT ID, float angle1, float angle2 );
			static void SetParticlesFaceDirection( UINT ID, int mode );
			static void SetParticlesSize( UINT ID, float size );
			static void SetParticlesLife( UINT ID, float time );
			static void SetParticlesMax( UINT ID, int max );
			static void ResetParticleCount( UINT ID );
			static void SetParticlesImage( UINT ID, UINT imageID );
			static void SetParticlesColorInterpolation( UINT ID, int mode );
			static void SetParticlesVisible( UINT ID, int visible );
			static void SetParticlesActive( UINT ID, int active );
			static void SetParticlesTransparency( UINT ID, int mode );

			static float GetParticlesX( UINT ID );
			static float GetParticlesY( UINT ID );
			static int GetParticlesDepth( UINT ID );
			static int GetParticlesVisible( UINT ID );
			static int GetParticlesActive( UINT ID );
			static float GetParticlesFrequency( UINT ID );
			static float GetParticlesDirectionX( UINT ID );
			static float GetParticlesDirectionY( UINT ID );
			static float GetParticlesAngle( UINT ID );
			static float GetParticlesAngleRad( UINT ID );
			static float GetParticlesSize( UINT ID );
			static float GetParticlesLife( UINT ID );
			static int GetParticlesMaxReached( UINT ID );

			static void AddParticlesForce( UINT ID, float starttime, float endtime, float x, float y );
			static void ClearParticlesForces( UINT ID );
			static void AddParticlesColorKeyFrame( UINT ID, float time, UINT red, UINT green, UINT blue, UINT alpha );
			static void ClearParticlesColors( UINT ID );
			static void AddParticlesScaleKeyFrame( UINT ID, float time, float scale );
			static void ClearParticlesScales( UINT ID );
			static void FixParticlesToScreen( UINT ID, int mode );

			static void UpdateParticles( UINT ID, float time );
			static void OffsetParticles( UINT ID, float x, float y );

			static void DrawParticles( UINT ID );

			// text commands
			static void SetTextDefaultFontImage( UINT iImageID );
			static void SetTextDefaultExtendedFontImage( UINT iImageID );
			static void SetTextDefaultMinFilter( UINT mode );
			static void SetTextDefaultMagFilter( UINT mode );

			static void CreateText( UINT iTextIndex, const char *string );
			static UINT CreateText( const char *string );
			static void DeleteText( UINT iTextIndex );
			static void DeleteAllText();
			static void SetTextString( UINT iTextIndex, const char *string );
			static void SetTextPosition( UINT iTextIndex, float fX, float fY );
			static void SetTextX( UINT iTextIndex, float fX );
			static void SetTextY( UINT iTextIndex, float fY );
			static void SetTextAngle( UINT iTextIndex, float fAngle );
			static void SetTextAngleRad( UINT iTextIndex, float fAngleRad );
			static void SetTextSize( UINT iTextIndex, float fSize );
			static void SetTextSpacing( UINT iTextIndex, float fSpacing );
			static void SetTextLineSpacing( UINT iTextIndex, float fSpacing );
			static void SetTextDepth( UINT iTextIndex, int iDepth );
			static void SetTextVisible( UINT iTextIndex, int bVisible );
			static void SetTextAlignment( UINT iTextIndex, int mode );
			static void SetTextColor( UINT iTextIndex, UINT iRed, UINT iGreen, UINT iBlue, UINT iAlpha=255 );
			static void SetTextColorRed( UINT iTextIndex, UINT iRed );
			static void SetTextColorGreen( UINT iTextIndex, UINT iGreen );
			static void SetTextColorBlue( UINT iTextIndex, UINT iBlue );
			static void SetTextColorAlpha( UINT iTextIndex, UINT iAlpha );
			static void SetTextFontImage( UINT iTextIndex, UINT iImageID );
			static void SetTextExtendedFontImage( UINT iTextIndex, UINT iImageID );
			static void SetTextFont( UINT iTextIndex, UINT iFontID );
			static void FixTextToScreen( UINT iTextIndex, int mode );
			static void SetTextMaxWidth( UINT iTextIndex, float width );
			static void SetTextScissor( UINT iTextIndex, float x, float y, float x2, float y2 );
			static void SetTextTransparency( UINT iTextIndex, int mode );
			static void SetTextBold( UINT iTextIndex, UINT bold );
			
			static UINT GetTextColorRed( UINT iTextIndex );
			static UINT GetTextColorGreen( UINT iTextIndex );
			static UINT GetTextColorBlue( UINT iTextIndex );
			static UINT GetTextColorAlpha( UINT iTextIndex );
			static UINT GetTextExists( UINT iTextIndex );
			static float GetTextX( UINT iTextIndex );
			static float GetTextY( UINT iTextIndex );
			static UINT GetTextLength( UINT iTextIndex );
			static float GetTextTotalWidth( UINT iTextIndex );
			static float GetTextTotalHeight( UINT iTextIndex );
			static int GetTextHitTest( UINT iTextIndex, float x, float y );
			static int GetTextVisible( UINT iTextIndex );
			static int GetTextDepth( UINT iTextIndex );
			static float GetTextSize( UINT iTextIndex );
			static char* GetTextString( UINT iTextIndex );
			static float GetTextSpacing( UINT iTextIndex );
			static float GetTextLineSpacing( UINT iTextIndex );
			static int GetTextAlignment( UINT iTextIndex );
			
			static void SetTextCharPosition( UINT iTextIndex, UINT iCharIndex, float x, float y );
			static void SetTextCharX( UINT iTextIndex, UINT iCharIndex, float x );
			static void SetTextCharY( UINT iTextIndex, UINT iCharIndex, float y );
			static void SetTextCharAngle( UINT iTextIndex, UINT iCharIndex, float angle );
			static void SetTextCharAngleRad( UINT iTextIndex, UINT iCharIndex, float angle );
			static void SetTextCharColor( UINT iTextIndex, UINT iCharIndex, UINT red, UINT green, UINT blue, UINT alpha );
			static void SetTextCharColorRed( UINT iTextIndex, UINT iCharIndex, UINT red );
			static void SetTextCharColorGreen( UINT iTextIndex, UINT iCharIndex, UINT green );
			static void SetTextCharColorBlue( UINT iTextIndex, UINT iCharIndex, UINT blue );
			static void SetTextCharColorAlpha( UINT iTextIndex, UINT iCharIndex, UINT alpha );
			static void SetTextCharBold( UINT iTextIndex, UINT iCharIndex, UINT bold );
			
			static float GetTextCharX( UINT iTextIndex, UINT iCharIndex );
			static float GetTextCharY( UINT iTextIndex, UINT iCharIndex );
			static float GetTextCharAngle( UINT iTextIndex, UINT iCharIndex );
			static float GetTextCharAngleRad( UINT iTextIndex, UINT iCharIndex );
			static UINT GetTextCharColorRed( UINT iTextIndex, UINT iCharIndex );
			static UINT GetTextCharColorGreen( UINT iTextIndex, UINT iCharIndex );
			static UINT GetTextCharColorBlue( UINT iTextIndex, UINT iCharIndex );
			static UINT GetTextCharColorAlpha( UINT iTextIndex, UINT iCharIndex );

			static void DrawText( UINT iTextIndex );

			// Font commands
			static UINT LoadFont( const char *szFontFile );
			static void LoadFont( UINT iFontID, const char *szFontFile );
			static int GetFontExists( UINT iFontID );
			static int GetSystemFontExists( const char *szFontFile );
			static void DeleteFont( UINT iFontID );

			// 2D Skeleton commands
			static void CreateSkeleton2D( UINT iSkeleton );
			static UINT CreateSkeleton2D();
			static int GetSkeleton2DExists( UINT iSkeleton );
			static void DeleteSkeleton2D( UINT iSkeleton );
			static void LoadSkeleton2DFromSpineFile( UINT iSkeleton, const char* filename, float scale, UINT atlasImage, int loadAnim );
			static UINT LoadSkeleton2DFromSpineFile( const char* filename, float scale, UINT atlasImage, int loadAnim );
			static void LoadSkeleton2DFromSpriterFile( UINT iSkeleton, const char* filename, float scale, UINT atlasImage );
			static UINT LoadSkeleton2DFromSpriterFile( const char* filename, float scale, UINT atlasImage );
			static void SetSkeleton2DPosition( UINT iSkeleton, float x, float y );
			static void SetSkeleton2DRotation( UINT iSkeleton, float r );
			static void SetSkeleton2DFlip( UINT iSkeleton, int flipH, int flipV );
			static void SetSkeleton2DDepth( UINT iSkeleton, UINT depth );
			static void FixSkeleton2DToScreen( UINT iSkeleton, int mode );
			static void SetSkeleton2DVisible( UINT iSkeleton, int mode );

			static float GetSkeleton2DX( UINT iskeleton );
			static float GetSkeleton2DY( UINT iskeleton );
			static float GetSkeleton2DAngle( UINT iskeleton );
			static int GetSkeleton2DDepth(UINT iskeleton);

			//static UINT AddSkeleton2DBone( UINT iSkeleton, UINT parent, const char* name );
			static int GetSkeleton2DBone( UINT iSkeleton, const char* name );
			static int GetSkeleton2DBoneParent( UINT iSkeleton, int bone );
			static float GetSkeleton2DBoneX( UINT iSkeleton, int bone );
			static float GetSkeleton2DBoneY( UINT iSkeleton, int bone );
			static float GetSkeleton2DBoneAngle( UINT iSkeleton, int bone );
			static float GetSkeleton2DBoneCurrX( UINT iSkeleton, int bone );
			static float GetSkeleton2DBoneCurrY( UINT iSkeleton, int bone );
			static float GetSkeleton2DBoneCurrAngle( UINT iSkeleton, int bone );
			static void SetSkeleton2DBonePosition( UINT iSkeleton, int bone, float x, float y );
			static void SetSkeleton2DBoneAngle( UINT iSkeleton, int bone, float r );
			static void SetSkeleton2DBoneScale( UINT iSkeleton, int bone, float sx, float sy );
			static void SetSkeleton2DBoneMode( UINT iSkeleton, int bone, int mode );

			static void FixSpriteToSkeleton2D( UINT spriteID, UINT iSkeletonID, int bone, int zorder );
			
			static void PlaySkeleton2DAnimation( UINT iSkeleton, const char* anim, float starttime, int loop, float tweentime );
			static void SetSkeleton2DAnimationFrame( UINT iSkeleton, const char* anim, float time, float tweentime );
			static void SetSkeleton2DAnimationSpeed( UINT iSkeleton, float speed );
			static float GetSkeleton2DCurrentTime( UINT iSkeleton );
			static void StopSkeleton2DAnimation( UINT iSkeleton );
			static int GetSkeleton2DIsAnimating( UINT iSkeleton );
			static int GetSkeleton2DIsTweening( UINT iSkeleton );
			static float GetSkeleton2DAnimationTime( UINT iSkeleton, const char* anim );

			// tween commands

			static void DeleteTween( UINT tweenID );
			static int GetTweenExists( UINT tweenID );
			static void SetTweenDuration( UINT tweenID, float duration );
			static int TweenLinear();
			static int TweenSmooth1();
			static int TweenSmooth2();
			static int TweenEaseIn1();
			static int TweenEaseIn2();
			static int TweenEaseOut1();
			static int TweenEaseOut2();
			static int TweenBounce();
			static int TweenOvershoot();

			// custom tweens
			static void CreateTweenCustom( UINT tweenID, float duration );
			static UINT CreateTweenCustom( float duration );
			static int GetTweenCustomExists( UINT tweenID );
			static void SetTweenCustomFloat1( UINT tweenID, float begin, float end, int interpolation );
			static void SetTweenCustomFloat2( UINT tweenID, float begin, float end, int interpolation );
			static void SetTweenCustomFloat3( UINT tweenID, float begin, float end, int interpolation );
			static void SetTweenCustomFloat4( UINT tweenID, float begin, float end, int interpolation );
			static void SetTweenCustomInteger1( UINT tweenID, int begin, int end, int interpolation );
			static void SetTweenCustomInteger2( UINT tweenID, int begin, int end, int interpolation );
			static void SetTweenCustomInteger3( UINT tweenID, int begin, int end, int interpolation );
			static void SetTweenCustomInteger4( UINT tweenID, int begin, int end, int interpolation );
			static float GetTweenCustomFloat1( UINT tweenID );
			static float GetTweenCustomFloat2( UINT tweenID );
			static float GetTweenCustomFloat3( UINT tweenID );
			static float GetTweenCustomFloat4( UINT tweenID );
			static int GetTweenCustomInteger1( UINT tweenID );
			static int GetTweenCustomInteger2( UINT tweenID );
			static int GetTweenCustomInteger3( UINT tweenID );
			static int GetTweenCustomInteger4( UINT tweenID );

			static void PlayTweenCustom( UINT tweenID, float delay );
			static void PauseTweenCustom( UINT tweenID );
			static void ResumeTweenCustom( UINT tweenID );
			static void UpdateTweenCustom( UINT tweenID, float time );
			static void StopTweenCustom( UINT tweenID );
			static int GetTweenCustomPlaying( UINT tweenID );

			// sprite tweening
			static void CreateTweenSprite( UINT tweenID, float duration );
			static UINT CreateTweenSprite( float duration );
			static int GetTweenSpriteExists( UINT tweenID );
			static void SetTweenSpriteX( UINT tweenID, float beginX, float endX, int interpolation );
			static void SetTweenSpriteY( UINT tweenID, float beginY, float endY, int interpolation );
			static void SetTweenSpriteXByOffset( UINT tweenID, float beginX, float endX, int interpolation );
			static void SetTweenSpriteYByOffset( UINT tweenID, float beginY, float endY, int interpolation );
			static void SetTweenSpriteAngle( UINT tweenID, float beginA, float endA, int interpolation );
			static void SetTweenSpriteSizeX( UINT tweenID, float beginSX, float endSX, int interpolation );
			static void SetTweenSpriteSizeY( UINT tweenID, float beginSY, float endSY, int interpolation );
			static void SetTweenSpriteRed( UINT tweenID, int beginR, int endR, int interpolation );
			static void SetTweenSpriteGreen( UINT tweenID, int beginG, int endG, int interpolation );
			static void SetTweenSpriteBlue( UINT tweenID, int beginB, int endB, int interpolation );
			static void SetTweenSpriteAlpha( UINT tweenID, int beginA, int endA, int interpolation );

			static void PlayTweenSprite( UINT tweenID, UINT spriteID, float delay );
			static void PauseTweenSprite( UINT tweenID, UINT spriteID );
			static void ResumeTweenSprite( UINT tweenID, UINT spriteID );
			static void UpdateTweenSprite( UINT tweenID, UINT spriteID, float time );
			static void StopTweenSprite( UINT tweenID, UINT spriteID );
			static int GetTweenSpritePlaying( UINT tweenID, UINT spriteID );

			// text tweening
			static void CreateTweenText( UINT tweenID, float duration );
			static UINT CreateTweenText( float duration );
			static int GetTweenTextExists( UINT tweenID );
			static void SetTweenTextX( UINT tweenID, float beginX, float endX, int interpolation );
			static void SetTweenTextY( UINT tweenID, float beginY, float endY, int interpolation );
			static void SetTweenTextAngle( UINT tweenID, float beginA, float endA, int interpolation );
			static void SetTweenTextSize( UINT tweenID, float beginS, float endS, int interpolation );
			static void SetTweenTextSpacing( UINT tweenID, float beginSP, float endSP, int interpolation );
			static void SetTweenTextLineSpacing( UINT tweenID, float beginLSP, float endLSP, int interpolation );
			static void SetTweenTextRed( UINT tweenID, int beginR, int endR, int interpolation );
			static void SetTweenTextGreen( UINT tweenID, int beginG, int endG, int interpolation );
			static void SetTweenTextBlue( UINT tweenID, int beginB, int endB, int interpolation );
			static void SetTweenTextAlpha( UINT tweenID, int beginA, int endA, int interpolation );

			static void PlayTweenText( UINT tweenID, UINT textID, float delay );
			static void PauseTweenText( UINT tweenID, UINT textID );
			static void ResumeTweenText( UINT tweenID, UINT textID );
			static void UpdateTweenText( UINT tweenID, UINT textID, float time );
			static void StopTweenText( UINT tweenID, UINT textID );
			static int GetTweenTextPlaying( UINT tweenID, UINT textID );

			// text char tweening
			static void CreateTweenChar( UINT tweenID, float duration );
			static UINT CreateTweenChar( float duration );
			static int GetTweenCharExists( UINT tweenID );
			static void SetTweenCharX( UINT tweenID, float beginX, float endX, int interpolation );
			static void SetTweenCharY( UINT tweenID, float beginY, float endY, int interpolation );
			static void SetTweenCharAngle( UINT tweenID, float beginA, float endA, int interpolation );
			static void SetTweenCharRed( UINT tweenID, int beginR, int endR, int interpolation );
			static void SetTweenCharGreen( UINT tweenID, int beginG, int endG, int interpolation );
			static void SetTweenCharBlue( UINT tweenID, int beginB, int endB, int interpolation );
			static void SetTweenCharAlpha( UINT tweenID, int beginA, int endA, int interpolation );

			static void PlayTweenChar( UINT tweenID, UINT textID, UINT charID, float delay );
			static void PauseTweenChar( UINT tweenID, UINT textID, UINT charID );
			static void ResumeTweenChar( UINT tweenID, UINT textID, UINT charID );
			static void UpdateTweenChar( UINT tweenID, UINT textID, UINT charID, float time );
			static void StopTweenChar( UINT tweenID, UINT textID, UINT charID );
			static int GetTweenCharPlaying( UINT tweenID, UINT textID, UINT charID );

			// object tweening
			static void CreateTweenObject( UINT tweenID, float duration );
			static UINT CreateTweenObject( float duration );
			static int GetTweenObjectExists( UINT tweenID );
			static void SetTweenObjectX( UINT tweenID, float beginX, float endX, int interpolation );
			static void SetTweenObjectY( UINT tweenID, float beginY, float endY, int interpolation );
			static void SetTweenObjectZ( UINT tweenID, float beginZ, float endZ, int interpolation );
			static void SetTweenObjectAngleX( UINT tweenID, float beginAX, float endAX, int interpolation );
			static void SetTweenObjectAngleY( UINT tweenID, float beginAY, float endAY, int interpolation );
			static void SetTweenObjectAngleZ( UINT tweenID, float beginAZ, float endAZ, int interpolation );
			static void SetTweenObjectScaleX( UINT tweenID, float beginSX, float endSX, int interpolation );
			static void SetTweenObjectScaleY( UINT tweenID, float beginSY, float endSY, int interpolation );
			static void SetTweenObjectScaleZ( UINT tweenID, float beginSZ, float endSZ, int interpolation );
			static void SetTweenObjectRed( UINT tweenID, int beginR, int endR, int interpolation );
			static void SetTweenObjectGreen( UINT tweenID, int beginG, int endG, int interpolation );
			static void SetTweenObjectBlue( UINT tweenID, int beginB, int endB, int interpolation );
			static void SetTweenObjectAlpha( UINT tweenID, int beginA, int endA, int interpolation );

			static void PlayTweenObject( UINT tweenID, UINT objectID, float delay );
			static void PauseTweenObject( UINT tweenID, UINT objectID );
			static void ResumeTweenObject( UINT tweenID, UINT objectID );
			static void UpdateTweenObject( UINT tweenID, UINT objectID, float time );
			static void StopTweenObject( UINT tweenID, UINT objectID );
			static int GetTweenObjectPlaying( UINT tweenID, UINT objectID );

			// camera tweening
			static void CreateTweenCamera( UINT tweenID, float duration );
			static UINT CreateTweenCamera( float duration );
			static int GetTweenCameraExists( UINT tweenID );
			static void SetTweenCameraX( UINT tweenID, float beginX, float endX, int interpolation );
			static void SetTweenCameraY( UINT tweenID, float beginY, float endY, int interpolation );
			static void SetTweenCameraZ( UINT tweenID, float beginZ, float endZ, int interpolation );
			static void SetTweenCameraAngleX( UINT tweenID, float beginAX, float endAX, int interpolation );
			static void SetTweenCameraAngleY( UINT tweenID, float beginAY, float endAY, int interpolation );
			static void SetTweenCameraAngleZ( UINT tweenID, float beginAZ, float endAZ, int interpolation );
			static void SetTweenCameraFOV( UINT tweenID, float beginF, float endF, int interpolation );
			
			static void PlayTweenCamera( UINT tweenID, UINT cameraID, float delay );
			static void PauseTweenCamera( UINT tweenID, UINT cameraID );
			static void ResumeTweenCamera( UINT tweenID, UINT cameraID );
			static void UpdateTweenCamera( UINT tweenID, UINT cameraID, float time );
			static void StopTweenCamera( UINT tweenID, UINT cameraID );
			static int GetTweenCameraPlaying( UINT tweenID, UINT cameraID );

			// tween chaining
			static void CreateTweenChain( UINT chainID );
			static UINT CreateTweenChain();
			static void DeleteTweenChain( UINT chainID );
			static void ClearTweenChain( UINT chainID );
			static void AddTweenChainCustom( UINT chainID, UINT tweenID, float delay );
			static void AddTweenChainSprite( UINT chainID, UINT tweenID, UINT spriteID, float delay );
			static void AddTweenChainText( UINT chainID, UINT tweenID, UINT textID, float delay );
			static void AddTweenChainChar( UINT chainID, UINT tweenID, UINT textID, UINT charID, float delay );
			static void AddTweenChainObject( UINT chainID, UINT tweenID, UINT objectID, float delay );
			static void AddTweenChainCamera( UINT chainID, UINT tweenID, UINT cameraID, float delay );

			static void PlayTweenChain( UINT chainID );
			static void PauseTweenChain( UINT chainID );
			static void StopTweenChain( UINT chainID );
			static int GetTweenChainPlaying( UINT chainID );
			static void SetTweenChainTime( UINT chainID, float time );
			static float GetTweenChainEndTime( UINT chainID );

			static void UpdateAllTweens( float time );
			static void UpdateTweenChain( UINT chainID, float time );
		

			// print commands
			static void Print( const uString &string );
			static void Print( const char* szString );
			static void Print( int i );
			static void Print( UINT u );
			static void Print( float f );
			static void PrintC( const uString &string );
			static void PrintC( const char* szString );
			static void PrintC( int i );
			static void PrintC( UINT u );
			static void PrintC( float f );
			static void SetPrintSize( float fSize );
			static float GetPrintSize( void ) { return m_pPrintText->GetSize(); } // 251011
			static void SetPrintSpacing( float fSpacing );
			static void SetPrintColor( UINT iRed, UINT iGreen, UINT iBlue );
			static void SetPrintColor( UINT iRed, UINT iGreen, UINT iBlue, UINT iAlpha );
			static void SetPrintFont( int fontID );

			// raw input commands
			static int GetMultiTouchExists();
			static int GetMouseExists();
			static int GetKeyboardExists();
			static int GetJoystickExists();
			static int GetCameraExists();

			// sensors
			static int GetAccelerometerExists();
			static int GetGyroSensorExists();
			static int GetProximitySensorExists();
			static int GetLightSensorExists();
			static int GetMagneticSensorExists();
			static int GetRotationVectorSensorExists();
			static int GetGPSSensorExists();

			// touch commands
			static UINT GetRawTouchCount( int bIncludeUnknown=0 );
			static UINT GetRawFirstTouchEvent( int bIncludeUnknown=0 );
			static UINT GetRawNextTouchEvent( );
			static int GetRawTouchType( UINT iIndex );
			static float GetRawTouchStartX( UINT iIndex );
			static float GetRawTouchStartY( UINT iIndex );
			static float GetRawTouchCurrentX( UINT iIndex );
			static float GetRawTouchCurrentY( UINT iIndex );
			static float GetRawTouchLastX( UINT iIndex );
			static float GetRawTouchLastY( UINT iIndex );
			static int GetRawTouchReleased( UINT iIndex );
			static void SetRawTouchValue( UINT iIndex, int value );
			static int GetRawTouchValue( UINT iIndex );
			static float GetRawTouchTime( UINT iIndex );
			static void SetRawTouchMoveSensitivity( int distance );

			// mouse
			static float GetRawMouseX();
			static float GetRawMouseY();
			static float GetRawMouseWheel();
			static float GetRawMouseWheelDelta();
			static int GetRawMouseLeftPressed();
			static int GetRawMouseLeftState();
			static int GetRawMouseLeftReleased();
			static int GetRawMouseRightPressed();
			static int GetRawMouseRightState();
			static int GetRawMouseRightReleased();
			static int GetRawMouseMiddlePressed();
			static int GetRawMouseMiddleState();
			static int GetRawMouseMiddleReleased();
			static int GetRawMouseFourthPressed();
			static int GetRawMouseFourthState();
			static int GetRawMouseFourthReleased();
			static int GetRawMouseFifthPressed();
			static int GetRawMouseFifthState();
			static int GetRawMouseFifthReleased();
			static void SetRawMouseVisible( int visible );
			static void SetRawMousePosition( float x, float y );
		
			// sensors

			// accelerometer
			static void FixOrientationByDefault();
			static float GetRawAccelX();
			static float GetRawAccelY();
			static float GetRawAccelZ();

			// gyroscope
			static float GetRawGyroVelocityX();
			static float GetRawGyroVelocityY();
			static float GetRawGyroVelocityZ();

			// proximity
			static float GetRawProximityDistance();

			// light sensor
			static float GetRawLightLevel();

			// magnetic
			static float GetRawMagneticX();
			static float GetRawMagneticY();
			static float GetRawMagneticZ();

			// rotation sensor
			static float GetRawRotationVectorX();
			static float GetRawRotationVectorY();
			static float GetRawRotationVectorZ();
			static float GetRawRotationVectorW();
        
            static float GetRawRotationVectorX2();
            static float GetRawRotationVectorY2();
            static float GetRawRotationVectorZ2();
            static float GetRawRotationVectorW2();

			// GPS
			static void StartGPSTracking();
			static void StopGPSTracking();
			static float GetRawGPSLatitude();
			static float GetRawGPSLongitude();
			static float GetRawGPSAltitude();

			// joystick
			static void CompleteRawJoystickDetection();
			static int GetRawJoystickExists( UINT index );
			static int GetRawJoystickConnected( UINT index );
			static char* GetRawJoystickName( UINT index );
			static float GetRawJoystickX( UINT index );
			static float GetRawJoystickY( UINT index );
			static float GetRawJoystickZ( UINT index );
			static float GetRawJoystickRX( UINT index );
			static float GetRawJoystickRY( UINT index );
			static float GetRawJoystickRZ( UINT index );
			static int GetRawJoystickSlider( UINT index, UINT slider );
			static int GetRawJoystickPOV( UINT index, UINT pov );
			static int GetRawJoystickButtonPressed( UINT index, UINT button );
			static int GetRawJoystickButtonState( UINT index, UINT button );
			static int GetRawJoystickButtonReleased( UINT index, UINT button );
			static void SetRawJoystickDeadZone( float threshold );

			// virtual joysticks
			static void AddVirtualJoystick( UINT index, float x, float y, float size );
			static void DeleteVirtualJoystick( UINT index );
			static int GetVirtualJoystickExists( UINT index );
			static float GetVirtualJoystickX( UINT index );
			static float GetVirtualJoystickY( UINT index );
			static void SetVirtualJoystickAlpha( UINT index, UINT alpha1, UINT alpha2 );
			static void SetVirtualJoystickPosition( UINT index, float x, float y );
			static void SetVirtualJoystickSize( UINT index, float size );
			static void SetVirtualJoystickActive( UINT index, int active );
			static void SetVirtualJoystickVisible( UINT index, int visible );
			static void SetVirtualJoystickImageInner( UINT index, UINT imageID );
			static void SetVirtualJoystickImageOuter( UINT index, UINT imageID );
			static void SetVirtualJoystickDeadZone( float threshold );
			
			// virtual buttons
			static void AddVirtualButton( UINT index, float x, float y, float size );
			static void DeleteVirtualButton( UINT index );
			static int GetVirtualButtonExists( UINT index );
			static int GetVirtualButtonPressed( UINT index );
			static int GetVirtualButtonReleased( UINT index );
			static int GetVirtualButtonState( UINT index );
			static void SetVirtualButtonColor( UINT index, UINT red, UINT green, UINT blue );
			static void SetVirtualButtonAlpha( UINT index, UINT alpha );
			static void SetVirtualButtonPosition( UINT index, float x, float y );
			static void SetVirtualButtonSize( UINT index, float size );
			static void SetVirtualButtonSize( UINT index, float sizeX, float sizeY );
			static void SetVirtualButtonActive( UINT index, int active );
			static void SetVirtualButtonVisible( UINT index, int visible );
			static void SetVirtualButtonImageUp( UINT index, UINT imageID );
			static void SetVirtualButtonImageDown( UINT index, UINT imageID );
			static void SetVirtualButtonText( UINT index, const char *str );

			// keyboard
			static int GetRawKeyPressed( UINT key );
			static int GetRawKeyState( UINT key );
			static int GetRawKeyReleased( UINT key );
			static int GetRawLastKey();
			static char* GetCharBuffer();
			static int GetCharBufferLength();
		
			// AGK emulated input commands
			static int GetPointerPressed();
			static int GetPointerState();
			static int GetPointerReleased();
			static int GetPointerGesture();
			static float GetPointerX();
			static float GetPointerY();

			static float GetDirectionX();
			static float GetDirectionY();
			static float GetDirectionAngle();
			static float GetDirectionSpeed();

			static float GetJoystickX();
			static float GetJoystickY();
			static void SetJoystickScreenPosition( float x, float y, float size );
			static void SetJoystickDeadZone( float threshold );

			static int GetButtonPressed( UINT index );
			static int GetButtonState( UINT index );
			static int GetButtonReleased( UINT index );
			static void SetButtonScreenPosition( UINT index, float x, float y, float size );

			// text input
			static void StartTextInput( );
            static void StartTextInput( const char* initial );
			static void StopTextInput( );
			static int GetTextInputState();
			static int GetTextInputCompleted();
			static int GetTextInputCancelled();
			static char* GetTextInput();
			static int GetTextInput( uString &out );
			static UINT GetPrevChar();
			static UINT GetLastChar();
			static void SetCursorBlinkTime( float seconds );
			static void SetTextInputMaxChars( int max=0 );
			static void SetTextInputLabel( const char* str );

			// edit boxes
			static void CreateEditBox( UINT index );
			static UINT CreateEditBox();
			static int GetEditBoxExists( UINT index );
			static void DeleteEditBox( UINT index );
			static int GetEditBoxHasFocus( UINT index );
			static UINT GetCurrentEditBox();
			static void SetEditBoxPosition( UINT index, float x, float y );
			static void SetEditBoxSize( UINT index, float width, float height );
			static void SetEditBoxDepth( UINT index, int depth );
			static void SetEditBoxBorderSize( UINT index, float size );
			static void SetEditBoxBorderColor( UINT index, UINT red, UINT green, UINT blue, UINT alpha );
			static void SetEditBoxBackgroundColor( UINT index, UINT red, UINT green, UINT blue, UINT alpha );
			static void SetEditBoxText( UINT index, const char *str ); // 120212 - paul notice, how a command name which is contained in another similar command MUST be placed first!!
			static void SetEditBoxTextColor( UINT index, UINT red, UINT green, UINT blue );
			static void SetEditBoxCursorColor( UINT index, UINT red, UINT green, UINT blue );
			static void SetEditBoxFontImage( UINT index, UINT image );
            static void SetEditBoxExtendedFontImage( UINT index, UINT image );
			static void SetEditBoxFont( UINT index, UINT fontID );
			static void SetEditBoxTextSize( UINT index, float size );
			static void SetEditBoxCursorPosition( UINT index, int pos );
			static void SetEditBoxFocus( UINT index, int iActive );
			static void SetEditBoxActive( UINT index, int iActive );
			static void SetEditBoxVisible( UINT index, int iActive );
			static void SetEditBoxBorderImage( UINT index, UINT image );
			static void SetEditBoxBackgroundImage( UINT index, UINT image );
			static void SetEditBoxCursorBlinkTime( UINT index, float time );
			static void SetEditBoxCursorWidth( UINT index, float width );
			static void SetEditBoxMaxChars( UINT index, UINT max );
			static void SetEditBoxMaxLines( UINT index, UINT max );
			static void SetEditBoxMultiLine( UINT index, int mulitline );
			static void SetEditBoxScissor( UINT index, float x, float y, float x2, float y2 );
			static void SetEditBoxPasswordMode( UINT index, int mode );
            static void SetEditBoxUseAlternateInput( UINT index, int mode );
			static void SetEditBoxWrapMode( UINT index, int mode );
			static void SetEditBoxInputType( UINT index, int inputType );
			static void FixEditBoxToScreen( UINT index, int fix );
			static char* GetEditBoxText( UINT index );
			static float GetEditBoxX( UINT index );
			static float GetEditBoxY( UINT index );
			static float GetEditBoxWidth( UINT index );
			static float GetEditBoxHeight( UINT index );
			static int GetEditBoxChanged( UINT index );
			static int GetEditBoxActive( UINT index );
			static int GetEditBoxDepth(UINT index);
			static int GetEditBoxVisible( UINT index );
			static int GetEditBoxLines( UINT index );
			static int GetEditBoxCursorPosition( UINT index );

			// sound comands
			static void LoadSound( UINT iID, const char* sFilename );
			static UINT LoadSound( const char* sFilename );
			static void LoadSound( UINT iID, const uString &sFile );
			static UINT LoadSound( const uString &sFile );
			static void LoadSoundOGG( UINT iID, const char* sFilename );
			static UINT LoadSoundOGG( const char* sFilename );
			static UINT PlaySound( UINT iID );
			static UINT PlaySound( UINT iID, int iVol );
			static UINT PlaySound( UINT iID, int iVol, int iLoop );
			static UINT PlaySound( UINT iID, int iVol, int iLoop, int iPriority );
			static void StopSound( UINT iID );
			static void DeleteSound( UINT iID );
			static UINT GetSoundInstances( UINT iID );
			static UINT GetSoundsPlaying( UINT iID );
			static void SetSoundSystemVolume( int iVol );
			static UINT GetSoundExists( UINT iID );
			static float GetSoundMaxRate();
			static float GetSoundMinRate();
			static void SaveSound( UINT iID, const char* sImageFilename );

			static int GetSoundInstancePlaying( UINT iID );
			static int GetSoundInstanceVolume( UINT iID );
			static float GetSoundInstanceRate( UINT iID );
			static void SetSoundInstanceVolume( UINT iID, int vol );
			static void SetSoundInstanceRate( UINT iID, float rate );
			static void SetSoundInstanceBalance( UINT iID, float balance );
			static int GetSoundInstanceLoopCount( UINT iID );
			static void StopSoundInstance( UINT iID );

			// music comands
			static UINT LoadMusic( const char* sFile );
			static void LoadMusic( UINT iID, const char* sFile );
			static void SetMusicFileVolume( UINT ID, int vol );
			static void PlayMusic( );
			static void PlayMusic( UINT iID );
			static void PlayMusic( UINT iID, int bLoop );
			static void PlayMusic( UINT iID, int bLoop, UINT iStartID, UINT iEndID );
			static void PauseMusic( );
			static void ResumeMusic( );
			static void StopMusic( );
			static void DeleteMusic( UINT iID );
			static int GetMusicPlaying();
			static void SetMusicSystemVolume( int iVol );
			static UINT GetMusicExists( UINT iID );
			static float GetMusicDuration( UINT iID );
			static float GetMusicPosition();
			static void SeekMusic( float seconds, int mode );

			// OGG music
			static UINT LoadMusicOGG( const char* sFile );
			static void LoadMusicOGG( UINT musicID, const char* sFile );
			static int GetMusicExistsOGG( UINT musicID );
			static void SetMusicVolumeOGG( UINT musicID, int vol );
			static void PlayMusicOGG( UINT musicID );
			static void PlayMusicOGG( UINT musicID, int iLoop );
			static void PauseMusicOGG( UINT musicID );
			static void ResumeMusicOGG( UINT musicID );
			static void StopMusicOGG( UINT musicID );
			static void DeleteMusicOGG( UINT musicID );
			static int GetMusicPlayingOGG( UINT musicID );
			static int GetMusicLoopCountOGG( UINT musicID );
			static float GetMusicDurationOGG( UINT musicID );
			static float GetMusicPositionOGG( UINT musicID );
			static void SeekMusicOGG( UINT musicID, float seconds, int mode );
			static void SetMusicSystemVolumeOGG( int iVol );
			static void SetMusicLoopTimesOGG(UINT musicID, float startTime, float endTime);
			static void SetMusicLoopCountOGG(UINT musicID, int loop);

			// record sound
			static void RecordSound( const char* szFilename );
			static void StopSoundRecording();
			static int IsSoundRecording();

			static void VibrateDevice( float seconds );

			static void SetClipboardText( const char* szText );
			static char* GetClipboardText();

			// file system commands
			static char* SimplifyPath( const char *szPath );
			static char* JoinPaths( const char *szPath, const char *szPath2 );
			static int IsAbsolutePath( const char *szPath );
			static int CountWindowsDrives();
			static char* GetWindowsDrive( int index );

			static UINT OpenRawFolder( const char *szPath );
			static void CloseRawFolder( UINT ID );
			static int GetRawFolderNumFiles( UINT ID );
			static int GetRawFolderNumFolders( UINT ID );
			static char* GetRawFolderFileName( UINT ID, int index );
			static char* GetRawFolderFolderName( UINT ID, int index );

			static void SetRawWritePath( const char *str );
			static char* GetWritePath();
			static char* GetReadPath();
			static char* GetDocumentsPath();

			static char* ChooseRawFile( const char* ext );
			static char* ChooseRawFile( const char* ext, int returnFullPath );

			static int GetFileExists( const char *szFile );
			static void DeleteFile( const char *szFile );
			
			static UINT OpenToWrite( const char *szFile, int append );
			static void OpenToWrite( UINT ID, const char *szFile, int append );
			static UINT OpenToWrite( const char *szFile );
			static void OpenToWrite( UINT ID, const char *szFile );			
			
			static UINT OpenToRead( const char *szFile );
			static void OpenToRead( UINT ID, const char *szFile );

			static int FileIsOpen( UINT ID );
			static void CloseFile( UINT iFileID );
			static int FileEOF( UINT iFileID );
			static int GetFileSize( UINT iFileID );
			static int GetFilePos( UINT iFileID );
			static void SetFilePos( UINT iFileID, int pos );
			
			static void WriteByte( UINT iFileID, int b );
			static void WriteInteger( UINT iFileID, int i );
			static void WriteFloat( UINT iFileID, float f );
			static void WriteString( UINT iFileID, const char* str );
			static void WriteString2( UINT iFileID, const char* str );
			static void WriteLine( UINT iFileID, const char* str );

			static int ReadByte( UINT iFileID );
			static int ReadInteger( UINT iFileID );
			static float ReadFloat( UINT iFileID );
			static char* ReadString( UINT iFileID );
			static char* ReadString2( UINT iFileID );
			static char* ReadLine( UINT iFileID );

			// directory commands
			static int SetCurrentDir( const char* );
			static char* GetCurrentDir();
			static int SetFolder( const char* );
			static char* GetFolder();

			static int MakeFolder( const char* );
			static void DeleteFolder( const char* );
			
			static char* GetFirstFolder();
			static char* GetFirstFolder( int mode );
			static char* GetNextFolder();
			static int GetFolderCount();
            static int GetFolderCount( int mode );

			static char* GetFirstFile();
			static char* GetFirstFile( int mode );
			static char* GetNextFile();
			static int GetFileCount();
            static int GetFileCount( int mode );

			// broadcast commands
			static UINT CreateBroadcastListener( UINT port );
			static UINT CreateBroadcastListener( const char *szIP, UINT port );
			static void DeleteBroadcastListener( UINT iID );
			static UINT GetBroadcastMessage( UINT iID );

			// socket commands
			static UINT ConnectSocket( const char *szIP, int port, int timeout );
			static int ConnectSocket( UINT socketID, const char *szIP, int port, int timeout );
			static int GetSocketConnected( UINT socketID );
			static int GetSocketExists( UINT socketID );
			static void DeleteSocket( UINT socketID );
			static AGKSocket* GetSocketPtr( UINT socketID );

			static char* GetSocketRemoteIP( UINT socketID );
			static int FlushSocket( UINT scoketID );

			static int GetSocketBytesAvailable( UINT socketID );
			static int GetSocketByte( UINT socketID );
			static int GetSocketInteger( UINT socketID );
			static float GetSocketFloat( UINT socketID );
			static char* GetSocketString( UINT socketID );

			static int SendSocketByte( UINT socketID, int value );
			static int SendSocketInteger( UINT socketID, int value );
			static int SendSocketFloat( UINT socketID, float value );
			static int SendSocketString( UINT socketID, const char *value );

			// socket listener
			static UINT CreateSocketListener( const char *szIP, int port );
			static int CreateSocketListener( UINT listenerID, const char *szIP, int port );
			static UINT GetSocketListenerConnection( UINT listenerID );
			static void DeleteSocketListener( UINT listenerID );

			// network commands
			static UINT HostNetwork( const char *szNetworkName, const char *szMyName, int port );
			static UINT HostNetwork( const char *szNetworkName, const char *szMyName, int port, int portv6 );
			static void SetNetworkNoMoreClients( UINT iNetID );
			static void SetNetworkAllowClients( UINT iNetID );

			static UINT JoinNetwork( const char *szNetworkName, const char *szMyName );
			static UINT JoinNetwork( const char *szIP, UINT port, const char *szMyName );

			static int GetNetworkExists( UINT iNetID );
			static UINT IsNetworkActive( UINT iNetID );
			static char* GetDeviceIP();
			static char* GetDeviceIPv6();
            static int GetNetworkInterface();
			
			static void CloseNetwork( UINT iNetID );
			static void SetNetworkLatency( UINT iNetID, UINT latency );

			static UINT GetNetworkMyClientID( UINT iNetID );
			static UINT GetNetworkNumClients( UINT iNetID );
			static UINT GetNetworkFirstClient( UINT iNetID );
			static UINT GetNetworkNextClient( UINT iNetID );

			static void KickNetworkClient( UINT iNetID, UINT client );

			static UINT GetNetworkClientDisconnected( UINT iNetID, UINT client );
			static void DeleteNetworkClient( UINT iNetID, UINT client );
			static char* GetNetworkClientName( UINT iNetID, UINT client );
			static char* GetNetworkClientIP( uint32_t iNetID, uint32_t client );
			static float GetNetworkClientPing( UINT iNetID, UINT client );
			static UINT GetNetworkServerID( UINT iNetID );
			static char* GetNetworkServerIP( uint32_t iNetID );
			
			static void SetNetworkLocalInteger( UINT iNetID, const char *name, int i );
			static void SetNetworkLocalInteger( UINT iNetID, const char *name, int i, int mode );
			static void SetNetworkLocalFloat( UINT iNetID, const char *name, float f );
			static void SetNetworkLocalFloat( UINT iNetID, const char *name, float f, int mode );
			static int GetNetworkClientInteger( UINT iNetID, UINT client, const char *name );
			static float GetNetworkClientFloat( UINT iNetID, UINT client, const char *name );

			static UINT CreateNetworkMessage( );
			static UINT CopyNetworkMessage( UINT iFromMsgID );
			static void AddNetworkMessageInteger( UINT iMsgID, int value );
			static void AddNetworkMessageFloat( UINT iMsgID, float value );
			static void AddNetworkMessageString( UINT iMsgID, const char *value );
			static char* GetNetworkMessageFromIP( UINT iMsgID );
			static int GetNetworkMessageFromPort( UINT iMsgID );
			static UINT GetNetworkMessageFromClient( UINT iMsgID );
			static int GetNetworkMessageInteger( UINT iMsgID );
			static float GetNetworkMessageFloat( UINT iMsgID );
			static char* GetNetworkMessageString( UINT iMsgID );
			static void DeleteNetworkMessage( UINT iMsgID );

			static void SendNetworkMessage( UINT iNetID, UINT toClient, UINT iMsgID );
			static UINT GetNetworkMessage( UINT iNetID );
			
			static void SetNetworkClientUserData( UINT iNetID, UINT client, UINT index, int value );
			static int GetNetworkClientUserData( UINT iNetID, UINT client, UINT index );

			// UDP
			static UINT CreateUDPListener( const char* ip, int port );
			static int CreateUDPListener( UINT listenerID, const char* ip, int port );
			static void SendUDPNetworkMessage( UINT listenerID, UINT messageID, const char* toIP, int toPort );
			static UINT GetUDPNetworkMessage( UINT listenerID );
			static void DeleteUDPListener( UINT listenerID );

			// HTTP commands
			static UINT CreateHTTPConnection();
			static void DeleteHTTPConnection( UINT iHTTP );

			static int GetInternetState();
			
			static UINT SetHTTPHost( UINT iHTTP, const char *szHost, int iSecure );
			static UINT SetHTTPHost( UINT iHTTP, const char *szHost, int iSecure, const char *szUser, const char *szPass );
			static void CloseHTTPConnection( UINT iHTTP );
			static void SetHTTPTimeout( UINT iHTTP, int milliseconds );
			static void SetHTTPVerifyCertificate( UINT iHTTP, int mode );

			static void AddHTTPHeader( UINT iHTTP, const char* headerName, const char* headerValue );
			static void RemoveHTTPHeader( UINT iHTTP, const char* headerName );
			
			static char* SendHTTPRequest( UINT iHTTP, const char *szServerFile ); //blocks
			static char* SendHTTPRequest( UINT iHTTP, const char *szServerFile, const char *szPostData ); //blocks
			static int SendHTTPRequestASync( UINT iHTTP, const char *szServerFile ); //does not block
			static int SendHTTPRequestASync( UINT iHTTP, const char *szServerFile, const char *szPostData ); //does not block
			static int SendHTTPFile( UINT iHTTP, const char *szServerFile, const char *szPostData, const char *szLocalFile ); //does not block
			static char* GetHTTPResponse( UINT iHTTP ); // 120112 - notice the shorter command goes first (for AGK compiler)
			static int GetHTTPResponseReady( UINT iHTTP );
			static const char* GetHTTPResponseC( UINT iHTTP );
			static int GetHTTPFile( UINT iHTTP, const char *szServerFile, const char *szLocalFile ); //does not block
			static int GetHTTPFile( UINT iHTTP, const char *szServerFile, const char *szLocalFile, const char *szPostData ); //does not block
			static int GetHTTPFileComplete( UINT iHTTP );
			static float GetHTTPFileProgress( UINT iHTTP );
			static int GetHTTPStatusCode( UINT iHTTP );
        
            static void OpenBrowser( const char* url );

			static UINT RunApp( const char *szFilename, const char *szParameters );
			static UINT GetAppRunning( UINT appID );
			static void TerminateApp( UINT appID );

			static void ViewFile( const char* szFilename );
			static void ShareText( const char* szText );
			static void ShareImage( const char* szFilename );
			static void ShareImageAndText( const char* szFilename, const char* szText );
			static void ShareFile( const char* szFilename );
		
			// string commands
			static char* Str( int value );
			static char* Str( float value );
			static char* Str( float value, UINT decimals );
			static char* Bin( int i );
			static char* Hex( int i );
			static char* StringToBase64( const char* input );
			static char* HexToBase64( const char* input );
			static int Val( const char* str );
			static int Val( const char* str, int base );
			static float ValFloat( const char* str );
			static char* Left ( const char* str, UINT count );
			static char* Right ( const char* str, UINT count );
			static char* Mid ( const char* str, UINT count, int length );
			static UINT Asc ( const char* str );
			static UINT Len ( const char* str );
			static UINT ByteLen ( const char* str );
			static char* Chr ( UINT iCharValue );
			static char* Lower ( const char* str );
			static char* Upper ( const char* str );
			static char* Spaces ( UINT count );
			static int FindStringCount( const char* str, const char* findStr );
			static int FindStringCount( const char* str, const char* findStr, int ignoreCase, int start );
			static int FindString( const char* str, const char* findStr );
			static int FindString( const char* str, const char* findStr, int ignoreCase, int start );
			static int FindStringReverse( const char* str, const char* findStr );
			static int FindStringReverse( const char* str, const char* findStr, int ignoreCase, int start );
			static int CompareString( const char* str, const char* str2 );
			static int CompareString( const char* str, const char* str2, int ignoreCase, int maxChars );
			static char* ReplaceString( const char* str, const char* find, const char* replace, int qty );
			static char* StripString( const char* str, const char* chars );
			static char* TrimString( const char* str, const char* chars );
			static char* TruncateString( const char* str, const char* character );
			static int CountStringTokens( const char* str, const char* delimiters );
			static char* GetStringToken( const char* str, const char* delimiters, int token );
			static int CountStringTokens2( const char* str, const char* delimiter );
			static char* GetStringToken2( const char* str, const char* delimiter, int token );

			// benchmarking
			static UINT GetManagedSpriteCount();
			static UINT GetManagedSpriteSortedCount();
			static UINT GetManagedSpriteDrawnCount();
			static UINT GetPixelsDrawn();
			static UINT GetManagedSpriteDrawCalls();

			static UINT GetParticleDrawnPointCount();
			static UINT GetParticleDrawnQuadCount();

			static float GetUpdateTime();
			static float GetPhysicsTime();
			static float GetDrawingSetupTime();
			static float GetDrawingTime();

			static UINT GetLoadedImages();
			static UINT GetUnassignedImages();
			static char* GetUnassignedImageFileName( UINT index );

			static float GetImageMemoryUsage();

			// date/time
			static int GetYearFromUnix64( INT64 unixtime, int* daysinyear=0 );
			static int GetMonthFromUnix64( INT64 unixtime );
			static int GetDayFromUnix64( INT64 unixtime );
			static int GetHoursFromUnix64( INT64 unixtime );
			static int GetMinutesFromUnix64( INT64 unixtime );
			static int GetSecondsFromUnix64( INT64 unixtime );
			static INT64 GetUnixFromDate64( int year, int month, int days, int hours, int minutes, int seconds );
			static INT64 GetUnixTime64();

			// tier 1 does not support 64 bit values, dates above 2038 will overflow
			static int GetYearFromUnix32( int unixtime, int* daysinyear=0 );
			static int GetYearFromUnix( int unixtime ); // 090112 - T1 not allowed - int* daysinyear=0 );
			static int GetMonthFromUnix( int unixtime );
			static int GetDaysFromUnix( int unixtime );
			static int GetHoursFromUnix( int unixtime );
			static int GetMinutesFromUnix( int unixtime );
			static int GetSecondsFromUnix( int unixtime );
			static int GetUnixFromDate( int year, int month, int days, int hours, int minutes, int seconds );
			static int GetUnixTime();

			static int GetLeapYear( int year );
			static char* GetCurrentDate();
			static char* GetCurrentTime();
			static int GetDayOfWeek();

			// adverts
			static void SetInneractiveDetails( const char* szCode );
            static void SetAdMobDetails( const char* szID );
			static void SetAdMobRewardAdDetails( const char* szID );
			static void SetChartboostDetails( const char* szKey1, const char* szKey2 );
			static void SetAmazonAdDetails( const char* szKey );

			static void LoadConsentStatusAdMob( const char* szPubID, const char* privacyPolicy );
			static int GetConsentStatusAdMob();
			static void RequestConsentAdMob();
			static void OverrideConsentAdMob( int consent );
			static void OverrideConsentChartboost( int consent );
        
            static void SetAmazonAdTesting( int mode );
			static void SetAdMobTesting( int mode );

			static void ShowFullscreenAdvertAdMob();
			static void ShowFullscreenAdvertChartboost();
			static void ShowFullscreenAdvertAmazon();

			static int GetFullscreenAdvertLoadedAdMob();
			static int GetFullscreenAdvertLoadedChartboost();
			static int GetFullscreenAdvertLoadedAmazon();

			static void ShowRewardAdAdMob();
			static int GetRewardAdLoadedAdMob();
			static int GetRewardAdRewardedAdMob();
			static void ResetRewardAdMob();

			static void ShowRewardAdChartboost();
			static void CacheRewardAdChartboost();
			static int GetRewardAdLoadedChartboost();
			static int GetRewardAdRewardedChartboost();
			static void ResetRewardChartboost();

			static void CreateFullscreenAdvert();
			static void CreateAdvert( int type, int horz, int vert, int test );
            static void CreateAdvertEx( int type, int horz, int vert, int test, float offsetx, float offsety );
			static void SetAdvertPosition( float x, float y, float width );
			static void SetAdvertLocation( int horz, int vert, float width );
            static void SetAdvertLocationEx( int horz, int vert, float offsetx, float offsety, float width );
            static void SetAdvertVisible( int iVisible );
            static void RequestAdvertRefresh ( void );
			static void DeleteAdvert( );

			//
			// V108 - 
			//

			// log command (mainly for HTML5 but others can use it; output window)
			static void Log( const char *szMessage );
        
			// internal social functions
            static void SocialPluginsSetup ( void );
            static void SocialPluginsDestroy ( void );

			// youtube
			static void PlayYoutubeVideo( const char* developerKey, const char* videoID, float startTime );

			// video commands
			static int LoadVideo( const char *szFilename );
			static void DeleteVideo();
			static void SetVideoDimensions( float x, float y, float width, float height );
			static void PlayVideo();
			static void PlayVideoToImage( UINT imageID );
            static void PauseVideo(); 
			static void StopVideo();
			static int GetVideoPlaying();
			static float GetVideoPosition();
			static float GetVideoDuration();
			static void SetVideoVolume( float volume );
			static float GetVideoWidth();
			static float GetVideoHeight();
			static void VideoResumed();
			static void VideoUpdate();
			static void SetVideoPosition( float seconds );

			// screen recording
			static void StartScreenRecording( const char *szFilename, int microphone );
			static void StopScreenRecording();
			static int IsScreenRecording();
        
            // Smart watch
            static void ActivateSmartWatch( const char *szReserved );
            static int GetSmartWatchState();
            static void SendSmartWatchData( const char* szJson );
            static char* ReceiveSmartWatchData();

			// Text to speech
			static void TextToSpeechSetup();
			static int GetTextToSpeechReady();
			static void Speak( const char *text );
			static void Speak( const char *text, int delay );
			static void SetSpeechRate( float rate );
			static void SetSpeechLanguage( const char* lang );
			static void SetSpeechLanguageByID( const char* sID );
            static int GetSpeechNumVoices();
            static char* GetSpeechVoiceLanguage( int index );
            static char* GetSpeechVoiceName( int index );
			static char* GetSpeechVoiceID( int index );
			static int IsSpeaking();
			static void StopSpeaking();
        
			// RATING COMMANDS
			static void  RateApp                      ( const char* szID );
            static void  RateApp                      ( const char* szID, const char* title );
			static void  RateApp                      ( const char* szID, const char* title, const char* message );
			static void RequestAppReview(); // iOS only

			// IN APP PURCHASE COMMANDS
			static void  InAppPurchaseSetKeys         ( const char* szData1, const char* szData2 );
			static void  InAppPurchaseSetTitle        ( const char* szTitle );
			static void  InAppPurchaseAddProductID    ( const char* szID, int type );
			static void  InAppPurchaseSetup           ( void );
			static void  InAppPurchaseActivate        ( int iID );
			static int   GetInAppPurchaseState        ( void );
			static int   GetInAppPurchaseAvailable    ( int iID );
            static char* GetInAppPurchaseLocalPrice   ( int iID );
            static char* GetInAppPurchaseDescription  ( int iID );
			static void  InAppPurchaseRestore		  ();
			static char* GetInAppPurchaseSignature    ( int iID );

			// FACEBOOK COMMANDS
			static void  FacebookSetup                ( const char* szID );
			static void  FacebookLogin                ( void );
			static void  FacebookLogout               ( void );
			static void  FacebookShowLikeButton       ( const char* szURL, int iX, int iY, int iWidth, int iHeight );
			static void  FacebookDestroyLikeButton    ( void );
			static void  FacebookPostOnMyWall         ( const char* szLink, const char* szPicture, const char* szName, const char* szCaption, const char* szDescription );
			static void  FacebookPostOnFriendsWall    ( const char* szID, const char* szLink, const char* szPicture, const char* szName, const char* szCaption, const char* szDescription );
			static void  FacebookInviteFriend         ( const char* szID, const char* szMessage );
			static void  FacebookGetFriends           ( void );
			static int   FacebookGetFriendsState      ( void );
			static int   FacebookGetFriendsCount      ( void );
			static char* FacebookGetFriendsName       ( int iIndex );
			static char* FacebookGetFriendsID         ( int iIndex );
			static void  FacebookDownloadFriendsPhoto ( int iIndex );
			static int   GetFacebookDownloadState     ( void );
			static char* GetFacebookDownloadFile      ( void );
			static int   GetFacebookLoggedIn          ( void );
			static char* FacebookGetUserID			  ( void );
			static char* FacebookGetUserName		  ( void );
			static char* FacebookGetAccessToken		  ( void );
			static void  FacebookActivateAppTracking  ( void );

			// TWITTER COMMANDS
			static void  TwitterSetup                 ( const char* szKey, const char* szSecret );
			static void  TwitterMessage               ( const char* szMessage );

			// NOTIFICATION COMMANDS
			static void  NotificationCreate           ( const char* szDateTime, const char* szMessage, const char* szData );
			static void  NotificationReset            ( void );
			static int   GetNotification              ( void );
			static char* GetNotificationData          ( void );

			// V108 Ultra notification (WinRT Metro-style)
			static int   GetNotificationType          ( void );
			static void  SetNotificationImage		  ( int iImageIndex );
			static void  SetNotificationText          ( const char* pText );

			// local notifications
			static void SetLocalNotification( int iID, int datetime, const char *szMessage, const char* szDeepLink );
			static void SetLocalNotification( int iID, int datetime, const char *szMessage );
			static void CancelLocalNotification( int iID );
			static int GetLocalNotificationExists( int iID );
			static int GetLocalNotificationTime( int iID );
			static char* GetLocalNotificationMessage( int iID );
			static void CheckLocalNotifications();

			// push notifications
			static void SetPushNotificationKeys( const char* data1, const char* reserved );
			static int PushNotificationSetup();
			static char* GetPushNotificationToken();

			// Android permissions
			static int CheckPermission( const char* szPermission );
			static void RequestPermission( const char* szPermission );

			// Shared variables
			static void SetSharedVariableAppGroup( const char* group );
			static void SaveSharedVariable( const char *varName, const char *varValue );
			static char* LoadSharedVariable( const char *varName, const char *defaultValue );
			static void DeleteSharedVariable( const char *varName );

			// Cloud data
			static void SetupCloudData( const char* reserved );
			static int GetCloudDataAllowed();
			static int GetCloudDataChanged();
			static char* GetCloudDataVariable( const char* varName, const char* defaultValue );
			static void SetCloudDataVariable( const char* varName, const char* varValue );
			static void DeleteCloudDataVariable( const char* varName );

			// Firebase
			static void FirebaseSetup();
			static void FirebaseLogEvent( const char *event_name );

			// zip functions
			static void CreateZip( UINT zipID, const char* filename );
			static UINT CreateZip( const char* filename );
			static void AddZipEntry( UINT zipID, const char* path, const char* zipPath );
			static void CloseZip( UINT zipID );
			static void ExtractZip( const char* zipfilename, const char* path);
			static void ExtractZip(const char* zipfilename, const char* path, const char* password);
			static void ExtractZipASync(const char* zipfilename, const char* path, const char* password);
			static float GetZipExtractProgress();
			static int GetZipExtractComplete();
			static void CancelZipExtract();

			// memblock functions (do not exist at this time)
			static UINT CreateMemblock( UINT size );
			static void CreateMemblock( UINT memID, UINT size );
			static int GetMemblockExists( UINT memID );
			static void DeleteMemblock( UINT memID );
			static unsigned char* GetMemblockPtr( UINT memID );
			static void CopyMemblock( UINT memSrcID, UINT memDstID, UINT srcOffset, UINT dstOffset, UINT size );
			
			static int GetMemblockSize( UINT memID );
			static int GetMemblockByte( UINT memID, UINT offset );
			static int GetMemblockByteSigned( UINT memID, UINT offset );
			static int GetMemblockShort( UINT memID, UINT offset );
			static int GetMemblockInt( UINT memID, UINT offset );
			static float GetMemblockFloat( UINT memID, UINT offset );
			static char* GetMemblockString( UINT memID, UINT offset, UINT length );

			static void SetMemblockByte( UINT memID, UINT offset, int value );
			static void SetMemblockByteSigned( UINT memID, UINT offset, int value );
			static void SetMemblockShort( UINT memID, UINT offset, int value );
			static void SetMemblockInt( UINT memID, UINT offset, int value );
			static void SetMemblockFloat( UINT memID, UINT offset, float value );
			static void SetMemblockString( UINT memID, UINT offset, const char* value );

			static void CreateMemblockFromImage( UINT memID, UINT imageID );
			static UINT CreateMemblockFromImage( UINT imageID );
			static void CreateImageFromMemblock( UINT imageID, UINT memID );
			static UINT CreateImageFromMemblock( UINT memID );

			static void CreateMemblockFromSound( UINT memID, UINT soundID );
			static UINT CreateMemblockFromSound( UINT soundID );
			static void CreateSoundFromMemblock( UINT soundID, UINT memID );
			static UINT CreateSoundFromMemblock( UINT memID );

			static void CreateMemblockFromFile( UINT memID, const char* filename );
			static UINT CreateMemblockFromFile( const char* filename );
			static void CreateFileFromMemblock( const char* filename, UINT memID );

			static void CreateMemblockFromObjectMesh( UINT memID, UINT objID, UINT meshIndex );
			static UINT CreateMemblockFromObjectMesh( UINT objID, UINT meshIndex );
			static void CreateObjectFromMeshMemblock( UINT objID, UINT memID );
			static UINT CreateObjectFromMeshMemblock( UINT memID );
			static void SetObjectMeshFromMemblock( UINT objID, UINT meshIndex, UINT memID );
			static void AddObjectMeshFromMemblock( UINT objID, UINT memID );
			
			static void SetMeshMemblockVertexPosition( UINT memID, UINT vertexIndex, float x, float y, float z );
			static void SetMeshMemblockVertexNormal( UINT memID, UINT vertexIndex, float x, float y, float z );
			static void SetMeshMemblockVertexUV( UINT memID, UINT vertexIndex, float u, float v );
			static void SetMeshMemblockVertexColor( UINT memID, UINT vertexIndex, int red, int green, int blue, int alpha );
			static unsigned short InternalGetMemblockPosOffset( cMemblock *pMem );
			static unsigned short InternalGetMemblockNormOffset( cMemblock *pMem );
			static unsigned short InternalGetMemblockUVOffset( cMemblock *pMem );
			static unsigned short InternalGetMemblockColorOffset( cMemblock *pMem );
			static float GetMeshMemblockVertexX( UINT memID, UINT vertexIndex );
			static float GetMeshMemblockVertexY( UINT memID, UINT vertexIndex );
			static float GetMeshMemblockVertexZ( UINT memID, UINT vertexIndex );
			static float GetMeshMemblockVertexNormalX( UINT memID, UINT vertexIndex );
			static float GetMeshMemblockVertexNormalY( UINT memID, UINT vertexIndex );
			static float GetMeshMemblockVertexNormalZ( UINT memID, UINT vertexIndex );
			static float GetMeshMemblockVertexU( UINT memID, UINT vertexIndex );
			static float GetMeshMemblockVertexV( UINT memID, UINT vertexIndex );
			static int GetMeshMemblockVertexRed( UINT memID, UINT vertexIndex );
			static int GetMeshMemblockVertexGreen( UINT memID, UINT vertexIndex );
			static int GetMeshMemblockVertexBlue( UINT memID, UINT vertexIndex );
			static int GetMeshMemblockVertexAlpha( UINT memID, UINT vertexIndex );

			//
			// V108 ULTRA RAW COMMANDS
			//
			// Internal
			static void InitialiseSensorCommands	( void );
			static void FreeSensorCommands			( void );
			// Real Commands
			static int  GetNFCExists				( void );
			
			// V108 NFC
			static UINT GetRawNFCCount				( void );
			static UINT GetRawFirstNFCDevice		( void );
			static UINT GetRawNextNFCDevice			( void );
			static char* GetRawNFCName				( UINT iIndex );
			static int SendRawNFCData				( UINT iIndex, const char* pMessageText );
			static int GetRawNFCDataState			( UINT iIndex );
			static char* GetRawNFCData				( UINT iIndex );

			// GameCenter

			static int GetGameCenterExists();
			static void GameCenterSetup();
			static void GameCenterSetup( const char *szKey, const char *szData );
			static void GameCenterLogin();
			static void GameCenterLogout();
			static int GetGameCenterLoggedIn();
			static char* GetGameCenterPlayerID();
			static char* GetGameCenterPlayerDisplayName();
			static void GameCenterSubmitScore( int iScore, const char* szBoardID );
			static void GameCenterShowLeaderBoard ( const char* szBoardID ); 
			static void GameCenterSubmitAchievement ( const char* szAchievementID, int iPercentageComplete );
			static void GameCenterAchievementsShow ( );
			static void GameCenterAchievementsReset ( );

			// drawing commands
			static UINT MakeColor( UINT red, UINT green, UINT blue );
			static UINT GetColorRed( UINT color );
			static UINT GetColorGreen( UINT color );
			static UINT GetColorBlue( UINT color );
			static void DrawLineInternal( float x, float y, float x2, float y2, UINT color1, UINT color2 );
			static void DrawLine( float x, float y, float x2, float y2, UINT red, UINT green, UINT blue );
			static void DrawLine( float x, float y, float x2, float y2, UINT color1, UINT color2 );
			static void DrawBox( float x, float y, float x2, float y2, UINT color1, UINT color2, UINT color3, UINT color4, int filled );
			static void DrawEllipse( float x, float y, float radiusx, float radiusy, UINT color1, UINT color2, int filled );

			//
			// V108 3D Commands
			//

			static void SetGlobal3DDepth( int depth );

			// 3d objects
			static void CreateObjectBox( UINT objID, float width, float height, float length );
			static UINT CreateObjectBox( float width, float height, float length );
			static void CreateObjectSphere( UINT objID, float diameter, int rows, int columns );
			static UINT CreateObjectSphere( float diameter, int rows, int columns );
			static UINT CreateObjectCapsule( float diameter, float height, int axis );
			static void CreateObjectCapsule( UINT objID, float diameter, float height, int axis );
			static void CreateObjectCone( UINT objID, float height, float diameter, int segments );
			static UINT CreateObjectCone( float height, float diameter, int segments );
			static void CreateObjectCylinder( UINT objID, float height, float diameter, int segments );
			static UINT CreateObjectCylinder( float height, float diameter, int segments );
			static void CreateObjectPlane( UINT objID, float width, float height );
			static UINT CreateObjectPlane( float width, float height );
			static void CreateObjectQuad( UINT objID );
			static UINT CreateObjectQuad();

			static void CreateObjectFromHeightMap( UINT objID, const char* szImageFile, float width, float height, float length, int smoothing, int split ); 
			static void CreateObjectFromRawHeightMap(UINT objID, const char* szImageFile, float width, float height, float length, int smoothing, int split, int rawWidth, int rawHeight );
			static UINT CreateObjectFromRawHeightMap( const char* szImageFile, float width, float height, float length, int smoothing, int split, int rawWidth, int rawHeight);
			static UINT CreateObjectFromHeightMap( const char* szImageFile, float width, float height, float length, int smoothing, int split );

			static UINT CreateObjectFromObjectMesh( UINT fromObjID, UINT meshIndex );
			static void CreateObjectFromObjectMesh( UINT objID, UINT fromObjID, UINT meshIndex );

			// for static objects
			static void LoadObject( UINT objID, const char *szFilename );
			static void LoadObject( UINT objID, const char *szFilename, float height );
			static UINT LoadObject( const char *szFilename );
			static UINT LoadObject( const char *szFilename, float height );

			// for animated objects
			static void LoadObjectWithChildren( UINT objID, const char *szFilename );
			static UINT LoadObjectWithChildren( const char *szFilename );

			static void SaveObject( UINT objID, const char *szFilename );

			static void CloneObject( UINT newobjID, UINT objID );
			static UINT CloneObject( UINT objID );
			static void InstanceObject( UINT newobjID, UINT objID );
			static UINT InstanceObject( UINT objID );

			static int GetObjectExists( UINT objID );
			static void DeleteObject( UINT objID );
			static void DeleteObjectWithChildren( UINT objID );
			static void DeleteObjectTree( UINT objID );
			static cObject3D* GetObjectPtr( UINT objID );
			static void DeleteAllObjects();

			static UINT GetObjectNumChildren( UINT objID );
			static UINT GetObjectChildID( UINT objID, int childIndex );

			static UINT GetObjectNumBones( UINT objID );
			static UINT GetObjectBoneByName( UINT objID, const char *name );

			// mesh
			static UINT GetObjectNumMeshes( UINT objID );
			static UINT GetObjectNumTextures(UINT objID);
			static char* GetObjectMeshName( UINT objID, UINT meshIndex );
			static char* GetObjectTextureName(UINT objID, UINT textureIndex);
			static void SetObjectMeshVisible(UINT objID, UINT meshIndex, int mode);
			static void SetObjectMeshCollisionMode(UINT objID, int meshIndex, int mode);
			static void SetObjectMeshImage( UINT objID, UINT meshIndex, UINT imageID, UINT textureStage );
			static void SetObjectMeshLightMap( UINT objID, UINT meshIndex, UINT imageID );
			static void SetObjectMeshNormalMap( UINT objID, UINT meshIndex, UINT imageID );
			static void SetObjectMeshNormalMapScale( UINT objID, UINT meshIndex, float scaleU, float scaleV );
			static void SetObjectMeshShader( UINT objID, UINT meshIndex, UINT shaderID );
			static char* GetObjectMeshVSSource( UINT objID, UINT meshIndex );
			static char* GetObjectMeshPSSource( UINT objID, UINT meshIndex );
			static void SetObjectMeshUVOffset( UINT objID, UINT meshIndex, UINT textureStage, float offsetU, float offsetV );
			static void SetObjectMeshUVScale( UINT objID, UINT meshIndex, UINT textureStage, float scaleU, float scaleV );
			static float GetObjectMeshSizeMinX( UINT objID, UINT meshIndex );
			static float GetObjectMeshSizeMaxX( UINT objID, UINT meshIndex );
			static float GetObjectMeshSizeMinY( UINT objID, UINT meshIndex );
			static float GetObjectMeshSizeMaxY( UINT objID, UINT meshIndex );
			static float GetObjectMeshSizeMinZ( UINT objID, UINT meshIndex );
			static float GetObjectMeshSizeMaxZ( UINT objID, UINT meshIndex );

			static void FixObjectToObject( UINT objID, UINT toObjID );
			static void FixObjectToBone( UINT objID, UINT toObjID, UINT toBoneIndex );

			// animation
			static int GetObjectNumAnimations( UINT objID );
			static char* GetObjectAnimationName( UINT objID, int index );
			static void PlayObjectAnimation( UINT objID, const char *animName, float starttime, float endtime, int loop, float tweentime );
			static void SetObjectAnimationFrame(UINT objID, const char *animName, float time, float tweentime );
			static void StopObjectAnimation( UINT objID );
			static void ResetObjectAnimation( UINT objID );
			static void SetObjectAnimationSpeed(  UINT objID, float speed );
			static int GetObjectIsAnimating( UINT objID );
			static int GetObjectIsTweening( UINT objID );
			static float GetObjectAnimationTime( UINT objID );
			static float GetObjectAnimationDuration( UINT objID, const char *animName );

			// bone transform
			static void SetObjectBonePosition( UINT objID, UINT boneIndex, float x, float y, float z );
			static void SetObjectBoneRotation( UINT objID, UINT boneIndex, float angx, float angy, float angz );
			static void SetObjectBoneRotationQuat( UINT objID, UINT boneIndex, float w, float x, float y, float z );
			static void SetObjectBoneLookAt( UINT objID, UINT boneIndex, float x, float y, float z, float roll );
			static void SetObjectBoneCanAnimate( UINT objID, UINT boneIndex, int animate );

			static void RotateObjectBoneLocalX( UINT objID, UINT boneIndex, float amount );
			static void RotateObjectBoneLocalY( UINT objID, UINT boneIndex, float amount );
			static void RotateObjectBoneLocalZ( UINT objID, UINT boneIndex, float amount );

			static char* GetObjectBoneName( UINT objID, UINT boneIndex );
			static float GetObjectBoneX( UINT objID, UINT boneIndex );
			static float GetObjectBoneY( UINT objID, UINT boneIndex );
			static float GetObjectBoneZ( UINT objID, UINT boneIndex );
			static float GetObjectBoneAngleX( UINT objID, UINT boneIndex );
			static float GetObjectBoneAngleY( UINT objID, UINT boneIndex );
			static float GetObjectBoneAngleZ( UINT objID, UINT boneIndex );
			static float GetObjectBoneQuatW( UINT objID, UINT boneIndex );
			static float GetObjectBoneQuatX( UINT objID, UINT boneIndex );
			static float GetObjectBoneQuatY( UINT objID, UINT boneIndex );
			static float GetObjectBoneQuatZ( UINT objID, UINT boneIndex );

			static float GetObjectBoneWorldX( UINT objID, UINT boneIndex );
			static float GetObjectBoneWorldY( UINT objID, UINT boneIndex );
			static float GetObjectBoneWorldZ( UINT objID, UINT boneIndex );
			static float GetObjectBoneWorldAngleX( UINT objID, UINT boneIndex );
			static float GetObjectBoneWorldAngleY( UINT objID, UINT boneIndex );
			static float GetObjectBoneWorldAngleZ( UINT objID, UINT boneIndex );
			static float GetObjectBoneWorldQuatW( UINT objID, UINT boneIndex );
			static float GetObjectBoneWorldQuatX( UINT objID, UINT boneIndex );
			static float GetObjectBoneWorldQuatY( UINT objID, UINT boneIndex );
			static float GetObjectBoneWorldQuatZ( UINT objID, UINT boneIndex );

			// object transform
			static void SetObjectPosition( UINT objID, float x, float y, float z );
			static void SetObjectRotation( UINT objID, float angx, float angy, float angz );
			static void SetObjectRotationQuat( UINT objID, float w, float x, float y, float z );
			static void SetObjectScale( UINT objID, float x, float y, float z );
			static void SetObjectScalePermanent( UINT objID, float x, float y, float z );

			static void MoveObjectLocalX( UINT objID, float amount );
			static void MoveObjectLocalY( UINT objID, float amount );
			static void MoveObjectLocalZ( UINT objID, float amount );

			static void RotateObjectLocalX( UINT objID, float amount );
			static void RotateObjectLocalY( UINT objID, float amount );
			static void RotateObjectLocalZ( UINT objID, float amount );

			static void RotateObjectGlobalX( UINT objID, float amount );
			static void RotateObjectGlobalY( UINT objID, float amount );
			static void RotateObjectGlobalZ( UINT objID, float amount );

			static float GetObjectX( UINT objID );
			static float GetObjectY( UINT objID );
			static float GetObjectZ( UINT objID );
			static float GetObjectAngleX( UINT objID );
			static float GetObjectAngleY( UINT objID );
			static float GetObjectAngleZ( UINT objID );
			static float GetObjectQuatW( UINT objID );
			static float GetObjectQuatX( UINT objID );
			static float GetObjectQuatY( UINT objID );
			static float GetObjectQuatZ( UINT objID );

			static float GetObjectWorldX( UINT objID );
			static float GetObjectWorldY( UINT objID );
			static float GetObjectWorldZ( UINT objID );
			static float GetObjectWorldAngleX( UINT objID );
			static float GetObjectWorldAngleY( UINT objID );
			static float GetObjectWorldAngleZ( UINT objID );
			static float GetObjectWorldQuatW( UINT objID );
			static float GetObjectWorldQuatX( UINT objID );
			static float GetObjectWorldQuatY( UINT objID );
			static float GetObjectWorldQuatZ( UINT objID );

			static float GetObjectSizeMinX( UINT objID );
			static float GetObjectSizeMaxX( UINT objID );
			static float GetObjectSizeMinY( UINT objID );
			static float GetObjectSizeMaxY( UINT objID );
			static float GetObjectSizeMinZ( UINT objID );
			static float GetObjectSizeMaxZ( UINT objID );

			static void SetObjectLookAt( UINT objID, float x, float y, float z, float roll );
			static void FixObjectPivot( UINT objID );

			static float GetObjectHeightMapHeight( UINT objID, float x, float z );

			// object properties
			static void SetObjectImage( UINT objID, UINT imageID, UINT texStage );
			static void SetObjectLightMap( UINT objID, UINT imageID );
			static void SetObjectNormalMap( UINT objID, UINT imageID );
			static void SetObjectNormalMapScale( UINT objID, float scaleU, float scaleV );
			static void SetObjectShader( UINT objID, UINT shaderID );
			static void SetObjectColor( UINT objID, int red, int green, int blue, int alpha );
			static void SetObjectAlpha( UINT objID, int alpha );
			static void SetObjectColorEmissive( UINT objID, int red, int green, int blue );
			static void SetObjectLightMode( UINT objID, int mode );
			static void SetObjectScreenCulling( UINT objID, int mode );
			static void SetObjectUVOffset( UINT objID, UINT textureStage, float offsetU, float offsetV );
			static void SetObjectUVScale( UINT objID, UINT textureStage, float scaleU, float scaleV );
			static void SetObjectFogMode( UINT objID, int mode );
			static void SetObjectCastShadow( int objID, int mode );
			static void SetObjectReceiveShadow( int objID, int mode );

			static void SetObjectDepthReadMode( UINT objID, int mode );
			static void SetObjectDepthWrite( UINT objID, int mode );
			static void SetObjectDepthBias( UINT objID, float bias );
			static void SetObjectDepthRange( UINT objID, float zNear, float zFar );
			static void SetObjectTransparency( UINT objID, int mode );
			static void SetObjectBlendModes( UINT objID, int src, int dst );
			static void SetObjectAlphaMask( UINT objID, int mode );
			static void SetObjectCullMode( UINT objID, int mode );
			static void SetObjectVisible( UINT objID, int mode );

			static int GetObjectDepthReadMode( UINT objID );
			static int GetObjectDepthWrite( UINT objID );
			static float GetObjectDepthBias( UINT objID );
			static int GetObjectTransparency( UINT objID );
			static int GetObjectCullMode( UINT objID );
			static int GetObjectVisible( UINT objID );
			static int GetObjectInScreen( UINT objID );
			static int GetObjectCastShadowMode( int objID );
			static int GetObjectReceiveShadowMode( int objID );
			static int GetObjectColorRed( int objID );
			static int GetObjectColorGreen( int objID );
			static int GetObjectColorBlue( int objID );
			static int GetObjectAlpha( int objID );

			static char* GetObjectName( UINT objID );
			
			static void SetObjectShaderConstantByName( UINT objID, const char* szName, float value1, float value2, float value3, float value4 );
			static void SetObjectShaderConstantArrayByName( UINT objID, const char* szName, UINT arrayIndex, float value1, float value2, float value3, float value4 );
			static void SetObjectShaderConstantDefault( UINT objID, const char* szName );

			static void DrawObject( UINT objID );

			static float GetScreenXFrom3D( float x, float y, float z );
			static float GetScreenYFrom3D( float x, float y, float z );

			static float Get3DVectorXFromScreen( float x, float y );
			static float Get3DVectorYFromScreen( float x, float y );
			static float Get3DVectorZFromScreen( float x, float y );

			// collision
			static void SetObjectCollisionMode( UINT objID, int mode );
			static bool InternalSphereCast( UINT objID, const AGKVector &p, const AGKVector &v, float radius );
			static int ObjectRayCast( UINT objID, float oldx, float oldy, float oldz, float newx, float newy, float newz );
			static int ObjectSphereCast( UINT objID, float oldx, float oldy, float oldz, float newx, float newy, float newz, float radius );
			static int ObjectSphereSlide( UINT objID, float oldx, float oldy, float oldz, float newx, float newy, float newz, float radius );
			static UINT GetObjectRayCastNumHits();
			static UINT GetObjectRayCastHitID( UINT index );
			static float GetObjectRayCastX( UINT index );
			static float GetObjectRayCastY( UINT index );
			static float GetObjectRayCastZ( UINT index );
			static float GetObjectRayCastSlideX( UINT index );
			static float GetObjectRayCastSlideY( UINT index );
			static float GetObjectRayCastSlideZ( UINT index );
			static float GetObjectRayCastNormalX( UINT index );
			static float GetObjectRayCastNormalY( UINT index );
			static float GetObjectRayCastNormalZ( UINT index );
			static float GetObjectRayCastBounceX( UINT index );
			static float GetObjectRayCastBounceY( UINT index );
			static float GetObjectRayCastBounceZ( UINT index );
			static float GetObjectRayCastDistance( UINT index );

			// fog
			static void SetFogMode( int mode );
			static void SetFogColor( int red, int green, int blue );
			static void SetFogSunColor( int red, int green, int blue );
			static void SetFogRange( float minDist, float maxDist );

			static int GetFogMode();
			static int GetFogColorsEqual();

			// sky box
			static void SetSkyBoxVisible( int active );
			static void SetSkyBoxSkyColor( int red, int green, int blue );
			static void SetSkyBoxHorizonColor( int red, int green, int blue );
			static void SetSkyBoxSunColor( int red, int green, int blue );
			static void SetSkyBoxHorizonSize( float size, float height );
			static void SetSkyBoxSunVisible( int visible );
			static void SetSkyBoxSunSize( float sun, float halo );

			// shaders
			static void LoadShader( UINT shaderID, const char* szVertexFile, const char* szPixelFile );
			static UINT LoadShader( const char* szVertexFile, const char* szPixelFile );
			static void LoadShaderFromString( UINT shaderID, const char* szVertexSource, const char* szPixelSource );
			static UINT LoadShaderFromString( const char* szVertexSource, const char* szPixelSource );
			static void LoadFullScreenShader( UINT shaderID, const char* szPixelFile );
			static UINT LoadFullScreenShader( const char* szPixelFile );
			static void LoadSpriteShader( UINT shaderID, const char* szPixelFile );
			static UINT LoadSpriteShader( const char* szPixelFile );
			static void SetShaderConstantByName( UINT shaderID, const char* szName, float value1, float value2, float value3, float value4 );
			static void SetShaderConstantArrayByName( UINT shaderID, const char* szName, UINT arrayIndex, float value1, float value2, float value3, float value4 );
			static int GetShaderExists( UINT shaderID );
			static void DeleteShader( UINT shaderID );

			// cameras
			static void SetCameraPosition( UINT cameraID, float x, float y, float z );
			static void SetCameraRotationQuat( UINT cameraID, float w, float x, float y, float z );
			static void SetCameraRotation( UINT cameraID, float angx, float angy, float angz );

			static void MoveCameraLocalX( UINT cameraID, float amount );
			static void MoveCameraLocalY( UINT cameraID, float amount );
			static void MoveCameraLocalZ( UINT cameraID, float amount );

			static void RotateCameraLocalX( UINT cameraID, float amount );
			static void RotateCameraLocalY( UINT cameraID, float amount );
			static void RotateCameraLocalZ( UINT cameraID, float amount );

			static void RotateCameraGlobalX( UINT cameraID, float amount );
			static void RotateCameraGlobalY( UINT cameraID, float amount );
			static void RotateCameraGlobalZ( UINT cameraID, float amount );

			static float GetCameraX( UINT cameraID );
			static float GetCameraY( UINT cameraID );
			static float GetCameraZ( UINT cameraID );
			static float GetCameraAngleX( UINT cameraID );
			static float GetCameraAngleY( UINT cameraID );
			static float GetCameraAngleZ( UINT cameraID );
			static float GetCameraQuatW( UINT cameraID );
			static float GetCameraQuatX( UINT cameraID );
			static float GetCameraQuatY( UINT cameraID );
			static float GetCameraQuatZ( UINT cameraID );

			static void SetCameraLookAt( UINT cameraID, float x, float y, float z, float roll );
			static void SetCameraRange( UINT cameraID, float fNear, float fFar );
			static void SetCameraFOV( UINT cameraID, float fov );
			static void SetCameraAspect( UINT cameraID, float aspect );
			static void SetCameraOrthoWidth( UINT cameraID, float width );
			static void SetCameraBounds( UINT cameraID, float left, float right, float top, float bottom );
			static void SetCameraOffCenter( UINT cameraID, int mode );

			static float GetCameraFOV( UINT cameraID );

			static void SetCurrentCamera( cCamera *pCamera );
			static cCamera* GetCurrentCamera() { return m_pCurrentCamera; }

			// lights
			static void CreatePointLight( UINT lightID, float x, float y, float z, float radius, int red, int green, int blue );
			static int GetPointLightExists( UINT lightID );
			static void DeletePointLight( UINT lightID );
			static void ClearPointLights();
			static void SetPointLightPosition( UINT lightID, float x, float y, float z );
			static void SetPointLightColor( UINT lightID, int red, int green, int blue );
			static void SetPointLightRadius( UINT lightID, float radius );
			static void SetPointLightMode( UINT lightID, int mode );

			// only allow one directional light
			//static void CreateLightDirectional( UINT lightID, float vx, float vy, float vz, UINT red, UINT green, UINT blue );
			//static int GetLightDirectionalExists( UINT lightID );
			//static void DeleteLightDirectional( UINT lightID );
			//static void ClearLightDirectionals();
			static void SetSunDirection( float vx, float vy, float vz );
			static void SetSunColor( int red, int green, int blue );
			static void SetSunActive( int active );

			static void SetAmbientColor( int red, int green, int blue );

			// Shadows
			static int GetShadowMappingSupported();
			static void SetShadowMappingMode( int mode );
			static int GetShadowMappingMode();
			static void SetShadowSmoothing( int mode );
			static int GetShadowSmoothing();
			static void SetShadowMapSize( int width, int height );
			static void SetShadowRange( float range );
			static void SetShadowBias( float bias );
			static void SetShadowLightStepSize( float step );
			static void SetShadowCascadeValues( float cascade1, float cascade2, float cascade3 );

			// 3D Particles

			static void Create3DParticles( UINT ID, float x, float y, float z );
			static UINT Create3DParticles( float x, float y, float z );
			static int Get3DParticlesExists( UINT ID );
			static void Delete3DParticles( UINT ID );
			static void Set3DParticlesPosition( UINT ID, float x, float y, float z );
			static void Set3DParticlesFrequency( UINT ID, float freq );
			static void Set3DParticlesStartZone( UINT ID, float x1, float y1, float z1, float x2, float y2, float z2 );
			static void Set3DParticlesDirection( UINT ID, float vx, float vy, float vz, float roll );
			static void Set3DParticlesVelocityRange( UINT ID, float v1, float v2 );
			static void Set3DParticlesDirectionRange( UINT ID, float angle, float angle2 );
			//static void Set3DParticlesFaceDirection( UINT ID, int mode );
			static void Set3DParticlesSize( UINT ID, float size );
			static void Set3DParticlesLife( UINT ID, float time );
			static void Set3DParticlesMax( UINT ID, int max );
			static void Reset3DParticleCount( UINT ID );
			static void Set3DParticlesImage( UINT ID, UINT imageID );
			static void Set3DParticlesColorInterpolation( UINT ID, int mode );
			static void Set3DParticlesVisible( UINT ID, int visible );
			static void Set3DParticlesActive( UINT ID, int active );
			static void Set3DParticlesTransparency( UINT ID, int mode );

			static float Get3DParticlesX( UINT ID );
			static float Get3DParticlesY( UINT ID );
			static float Get3DParticlesZ( UINT ID );
			static int Get3DParticlesVisible( UINT ID );
			static int Get3DParticlesActive( UINT ID );
			static float Get3DParticlesFrequency( UINT ID );
			static float Get3DParticlesDirectionX( UINT ID );
			static float Get3DParticlesDirectionY( UINT ID );
			static float Get3DParticlesDirectionZ( UINT ID );
			static float Get3DParticlesDirectionRange1( UINT ID );
			static float Get3DParticlesDirectionRange2( UINT ID );
			static float Get3DParticlesSize( UINT ID );
			static float Get3DParticlesLife( UINT ID );
			static int Get3DParticlesMaxReached( UINT ID );

			static void Add3DParticlesForce( UINT ID, float starttime, float endtime, float x, float y, float z );
			static void Clear3DParticlesForces( UINT ID );
			static void Add3DParticlesColorKeyFrame( UINT ID, float time, UINT red, UINT green, UINT blue, UINT alpha );
			static void Clear3DParticlesColors( UINT ID );
			static void Add3DParticlesScaleKeyFrame( UINT ID, float time, float scale );
			static void Clear3DParticlesScales( UINT ID );

			static void Update3DParticles( UINT ID, float time );
			static void Offset3DParticles( UINT ID, float x, float y, float z );

			static void Draw3DParticles( UINT ID );

			//3D Physics Commands

			//3D Physics World Commands
			static void Create3DPhysicsWorld();
			static void Create3DPhysicsWorld( float scaleFactor );
			static void Set3DPhysicsGravity( float x, float y, float z );
			static void Set3DPhysicsGravity( UINT vectorID );
			static void Step3DPhysicsWorld();
			static void Reset3DPhysicsWorld();
			static void Delete3DPhysicsWorld();
			static void Debug3DPhysicsWorld();// Can not use Debugger at this time.Unable to draw wireframe.
			static int Get3DPhysicsTotalObjects();
			static int Get3DPhysicsActiveObjects();
			static int Get3DPhysicsTotalJoints();

			//CollisionShapes
			static void SetObjectShapeBox( UINT objID );
			static void SetObjectShapeBox( UINT objID, float sizeX, float sizeY, float sizeZ );
			static void SetObjectShapeBox( UINT objID, UINT vectorID );
			static void SetObjectShapeSphere( UINT objID );
			static void SetObjectShapeSphere( UINT objID, float diameter );
			static void SetObjectShapeCylinder( UINT objID, int axis );
			static void SetObjectShapeCylinder( UINT objID, int axis, float height, float diameter );
			static void SetObjectShapeCone( UINT objID, int axis );
			static void SetObjectShapeCone( UINT objID, int axis, float height, float diameter );
			static void SetObjectShapeCapsule( UINT objID, int axis );
			static void SetObjectShapeCapsule( UINT objID, int axis, float sizeX, float sizeY, float sizeZ );
			static void SetObjectShapeCapsule( UINT objID, int axis, UINT vectorID );
			static void SetObjectShapeConvexHull( UINT objID );
			static void SetObjectShapeStaticPolygon( UINT objID );
			static void SetObjectShapeCompound( UINT objID );

			//RigidBodies
			static void Create3DPhysicsDynamicBody( UINT objID );
			static void Create3DPhysicsStaticBody( UINT objID );
			static void Create3DPhysicsKinematicBody( UINT objID ); 
			static void Delete3DPhysicsBody( UINT objID );
			static int Create3DPhysicsStaticPlane ( float normlX, float normalY, float normalZ, float offsetPosition );
			static void Set3DPhysicsStaticPlanePosition( UINT planeID, float posX, float posY, float posZ );
			static void Set3DPhysicsStaticPlaneRotation( UINT planeID, float angX, float angY, float angZ );
			static void Delete3DPhysicsStaticPlane( UINT planeID );
			static void SetObject3DPhysicsGroupAndMask( UINT objID, int group, int mask );
			static int GetObject3DPhysicsGroup( UINT objID );
			static int GetObject3DPhysicsMask( UINT objID );
			static void SetObject3DPhysicsCanSleep( UINT objID, int canSleep );
			static void SetObject3DPhysicsMass( UINT objID, float mass );
			static float GetObject3DPhysicsMass( UINT objID );
			static void SetObject3DPhysicsFriction( UINT objID, float friction );
			static float GetObject3DPhysicsFriction( UINT objID );
			static void SetObject3DPhysicsRollingFriction( UINT objID, float friction );
			static float GetObject3DPhysicsRollingFriction( UINT objID );
			static void SetObject3DPhysicsAnisotropicFriction( UINT objID, int type );
			static void SetObject3DPhysicsMaxLinearVelocity( UINT objID, float maxLinearVelocity );
			static float GetObject3DPhysicsMaxLinearVelocity( UINT objID );
			static void SetObject3DPhysicsLinearVelocity( UINT objID, float dirX, float dirY, float dirZ, float initialSpeed );
			static void SetObject3DPhysicsLinearVelocity( UINT objID, UINT vectorID, float initialSpeed );
			static float GetObject3DPhysicsLinearVelocityX( UINT objID );
			static float GetObject3DPhysicsLinearVelocityY( UINT objID );
			static float GetObject3DPhysicsLinearVelocityZ( UINT objID );
			static void SetObject3DPhysicsAngularVelocity( UINT objID, float angX, float angY, float angZ, float initialSpeed );
			static void SetObject3DPhysicsAngularVelocity( UINT objID, UINT vectorID, float initialSpeed );
			static float GetObject3DPhysicsAngularVelocityX( UINT objID );
			static float GetObject3DPhysicsAngularVelocityY( UINT objID );
			static float GetObject3DPhysicsAngularVelocityZ( UINT objID );
			static void SetObject3DPhysicsDamping( UINT objID, float linearDamp, float angularDamp );
			static float GetObject3DPhysicsLinearDamp( UINT objID );
			static float GetObject3DPhysicsAngularDamp( UINT objID );
			static void SetObject3DPhysicsSleepingThreshold( UINT objID, float angular, float linear );
			static float GetObject3DPhysicsAngularSleepingThreshold( UINT objID );
			static float GetObject3DPhysicsLinearSleepingThreshold( UINT objID );
			static void SetObject3DPhysicsDeactivationTime( UINT objID, float time );
			static void SetObject3DPhysicsRestitution( UINT objID, float friction);
			static float GetObject3DPhysicsRestitution(  UINT objID );

			//ContactReports
			static int GetObject3DPhysicsFirstContact( UINT objID ); 
			static float GetObject3DPhysicsContactX(); 
			static float GetObject3DPhysicsContactY(); 
			static float GetObject3DPhysicsContactZ(); 
			static int GetObject3DPhysicsContactVector( int outPosVec3 );
			static int GetObject3DPhysicsContactObjectB();
			static int GetObject3DPhysicsNextContact();
			static int GetObjects3DPhysicsContactPositionVector( UINT objA, UINT objB, int outPosVec3 );

			//Joints/
			static int Create3DPhysicsPickJoint( UINT objID, int positionVec3 );
			static void Update3DPhysicsPickJoint( UINT jointID, int positionVec3 );
			static void Delete3DPhysicsPickJoint( UINT jointID );
			static int Create3DPhysicsHingeJoint( UINT objA, UINT objB, int positionVec3, int rotationVec3, int disableCollisions );
			static int Create3DPhysicsConeTwistJoint( UINT objA, UINT objB, int positionVec3, int rotationVec3, int disableCollisions );
			static int Create3DPhysicsSliderJoint( UINT objA, UINT objB, int positionVec3, int rotationVec3 );
			static int Create3DPhysicsFixedJoint( UINT objA, UINT objB, int positionVec3 );
			static int Create3DPhysics6DOFJoint( UINT objA, UINT objB, int positionVec3, float rotationVec3 );
			static void Set3DPhysicsJointSliderAngularLimits( UINT jointID, float lowerLimit, float upperLimit );
			static void Set3DPhysicsJointSliderLinearLimits( UINT jointID, float lowerLimit, float upperLimit );
			static void Set3DPhysicsJointConeTwistLimits( UINT jointID, float swingSpan1, float swingSpan2, float twistSpan );
			static void Set3DPhysicsJointHingeLimits( UINT jointID, float minAng, float maxAng );
			static void Set3DPhysicsJointBreakingThreshold( UINT jointID, float breakThreshold );
			static void Set3DPhysicsJointEnabled( UINT jointID, int isEnabled );
			static int Get3DPhysicsJointEnabled( UINT jointID );
			static int Get3DPhysicsJointPositionVector( UINT jointID );
			static int Get3DPhysicsJointRotationVector( UINT jointID );
			static void Delete3DPhysicsJoint( UINT jointID );

			//Joint Motors
			static void Set3DPhysicsHingeJointMotorIsEnabled( UINT jointID, int isEnabled );
		    static void Set3DPhysicsHingeJointMaxMotorImpulse( UINT jointID, float maxImpulse );
			static void Set3DPhysicsHingeJointMotorVelocity( UINT jointID, float targetVelocity );
			static void Set3DPhysicsTwistJointMotorIsEnabled( UINT jointID, int isEnabled );
			static void Set3DPhysicsTwistJointMaxMotorImpulse( UINT jointID, float maxImpulse );
			static void Set3DPhysicsTwistJointMotorRotationTarget( UINT jointID, int rotationVec3ID );
			static void Set3DPhysicsSliderJointPoweredLinearMotorIsEnabled( UINT jointID, int isEnabled );
			static void Set3DPhysicsSliderJointMaxLinearMotorForce( UINT jointID, float maxLinearForce );
			static void Set3DPhysicsSliderJointTargetLinearMotorVelocity( UINT jointID, float linearMotorVelocity );

			//CompoundCollisionShapes/
			static void AddObjectShapeBox( int objID, int positionVec3, int rotationVec3, int sizeVec3 );
			static void AddObjectShapeSphere( int objID, int positionVec3, float diameter );
			static void AddObjectShapeCapsule( int objID, int positionVec3, int rotationVec3, int sizeVec3, int axis );
			static void AddObjectShapeCone( int objID, int positionVec3, int rotationVec3, int sizeVec3, int axis);
			static void AddObjectShapeCylinder( int objID, int positionVec3, int rotationVec3, int sizeVec3, int axis);

			//SavingAndLoading/
			static int SaveObjectShape( UINT objID, const char* fileName );
			static int LoadObjectShape( UINT objID, const char* fileName );

			//Vector commands
			static int CreateVector3();
			static int CreateVector3( float x, float y, float z );
			static void SetVector3( UINT vectorID, float x, float y, float z );
			static void DeleteVector3( UINT vectorID );
			static float GetVector3X( UINT vectorID );
			static float GetVector3Y( UINT vectorID );
			static float GetVector3Z( UINT vectorID );
			static float GetVector3Distance( UINT vectorU, UINT vectorV );
			static float GetVector3Length( UINT vectorID );
			static float GetVector3Dot( UINT vectorU, UINT vectorV );
			static void GetVector3Cross( UINT resultVec, UINT vectorU, UINT vectorV );
			static void GetVector3Multiply( UINT resultVec, float multiplier );
			static void GetVector3Add( UINT resultVec, UINT addVec );

			//***************************************** MileStone3 **************************************************************
			//RayCasting/
			static int Create3DPhysicsRay();
			static void Delete3DPhysicsRay( UINT rayID );
			static int Ray3DPhysicsExist( UINT rayID );
			static void RayCast3DPhysics( UINT rayID, int fromVec3ID, int toVec3ID, int allOrClosest );
			static int RayCast3DPhysicsObject( UINT objID, UINT rayID, int fromVec3ID, int toVec3ID, int allOrClosest );
			static int Get3DPhysicsRayCastObjectHit( UINT rayID, float fractionIndex );
			static int Get3DPhysicsRayCastClosestObjectHit( UINT rayID );
			static float Get3DPhysicsRayCastFraction( UINT rayID );
			static int Get3DPhysicsRayCastContactPosition( UINT rayID, float fractionIndex, int outVec3ID );
			static int Get3DPhysicsRayCastClosestContactPosition( UINT rayID, int outVec3ID );
			static void Get3DPhysicsRayCastNormalVector( UINT rayID, int returnVec3ID );
			static int Get3DPhysicsRayCastNumHits( UINT rayID );
			static void SphereCast3DPhysics( UINT rayID, int fromVec3ID, int toVec3ID, float radius );
			static int SphereCast3DPhysicsObject( UINT objID, UINT rayID, int fromVec3ID, int toVec3ID, float radius );
			
			//CharacterController/
			static void Create3DPhysicsCharacterController( UINT objID, int axis, int objOffsetVec3, int objOrientationVec3, float crouchScale );
			static void Delete3DPhysicsCharacterController( UINT objID );
			static int Get3DPhysicsCharacterControllerExists( UINT objID );
			static void Set3DPhysicsCharacterControllerGravity( UINT objID, float gravity );
			static void Set3DPhysicsCharacterControllerFallSpeed( UINT objID, float fallSpeed );
			static void Set3DPhysicsCharacterControllerJumpSpeed( UINT objID, float jumpSpeed );
			static void Set3DPhysicsCharacterControllerMaxSlope( UINT objID, float maxSlopeDegress );
			static void Set3DPhysicsCharacterControllerStepHeight( UINT objID, float stepHeight );
			static float Get3DPhysicsCharacterControllerMaxSlope( UINT objID );
			static float Get3DPhysicsCharacterControllerGravity( UINT objID );
			static void Move3DPhysicsCharacterController( UINT objID, int direction, float velocity );
			static void Move3DPhysicsCharacterController( UINT objID, float x, float z, float velocity );
			static void Rotate3DPhysicsCharacterController( UINT objID, float angle );
			static void Jump3DPhysicsCharacterController( UINT objID );
			static void Crouch3DPhysicsCharacterController( UINT objID );
			static void Stand3DPhysicsCharacterController( UINT objID );
			static void Debug3DPhysicsCharacterController( UINT objID, int isDebug );
			static void Set3DPhysicsCharacterControllerPosition( UINT objID, float posX, float posY, float posZ );

			//Ragdoll/
			static void Create3DPhysicsRagDoll( UINT objID, float objTotalWeight );
			static int Add3DPhysicsRagDollBone( UINT startBoneID, UINT endBoneID, float diameter, int collisionGroup, int collisionMask );
			static void AssignTo3DPhysicsRagDollBoneObjectBone( UINT ragdollBoneID, UINT objBoneID );
			static void Add3DPhysicsRagDollHingeJoint( UINT boneAID, UINT boneBID, UINT objBoneID, int jointRotationVec3, float minLimit, float maxLimit );
			static void Add3DPhysicsRagDollTwistJoint( UINT boneAID, UINT boneBID, UINT objBoneID, int jointRotationVec3, int limitsVec3 );
			static void Finalize3DPhysicsRagDoll();
			static void Delete3DPhysicsRagdoll( UINT objID );
			static int Get3DPhysicsRagdollExist( UINT objID );
			static int Get3DPhysicsRagdollFromBoneObject( UINT objID );
			static int Is3dPhysicsRagdollStatic( UINT objID );
			static void Set3DphysicsRagdollStatic( UINT objID, int isStatic );
			static void Set3DPhysicsRagdollBonesVisible( UINT objID, int isVisible );
			static void Set3DPhysicsRagdollDamping( float linear, float angular );
			static void Set3DPhysicsRagdollSleepingThresholds( float linear, float angular );
			static void Set3DPhysicsRagdollDeactivationTime( float time );
			static void Set3DPhysicsRagdollDeactivation( UINT objID, int isDisabled );
		

			// AR
			static void ARSetup();
			static int ARGetStatus();
			static void ARUpdateInternal();
			static void ARPause();
			static void ARResume();
			static void ARDestroy();
			static void ARControlCamera();
			static void ARDrawBackground();
			static void ARSetPlaneDetectionMode( int mode );
			static void ARSetLightEstimationMode( int mode );

			static float ARGetLightEstimate();

			static int ARHitTest( float screenX, float screenY );
			static float ARGetHitTestX( int index );
			static float ARGetHitTestY( int index );
			static float ARGetHitTestZ( int index );
			static float ARGetHitTestNormalX( int index );
			static float ARGetHitTestNormalY( int index );
			static float ARGetHitTestNormalZ( int index );
			static int ARGetHitTestType( int index );
			static void ARHitTestFinish();

			static int ARGetPlanes( int reserved );
			static float ARGetPlaneX( int index );
			static float ARGetPlaneY( int index );
			static float ARGetPlaneZ( int index );
			static float ARGetPlaneAngleX( int index );
			static float ARGetPlaneAngleY( int index );
			static float ARGetPlaneAngleZ( int index );
			static float ARGetPlaneSizeX( int index );
			static float ARGetPlaneSizeZ( int index );
			static void ARGetPlanesFinish();
			
			static int ARCreateAnchorFromHitTest( int index );
			static int ARCreateAnchorFromPlane( int index );
			static void ARFixObjectToAnchor( int objID, int anchorID );
			static int ARGetAnchorStatus( int anchorID );
			static void ARDeleteAnchor( int anchorID );

			// AGK Studio commands
			static void SetObjectMeshCastShadow( int objID, int meshID, int mode );
			static void SetObjectShaderConstantArrayFloatByName( int objID, const char* varName, int index, float f1 );
			static void SetObjectShaderConstantArrayVec2ByName( int objID, const char* varName, int index, float f1, float f2 );
			static void SetObjectShaderConstantArrayVec3ByName( int objID, const char* varName, int index, float f1, float f2, float f3 );
			static void SetObjectShaderConstantArrayVec4ByName( int objID, const char* varName, int index, float f1, float f2, float f3, float f4 );
			static void SetShaderErrorMode( int mode );
			static void SetShaderConstantArrayFloatByName( int shaderID, const char* varName, int index, float f1 );
			static void SetShaderConstantArrayVec2ByName( int shaderID, const char* varName, int index, float f1, float f2 );
			static void SetShaderConstantArrayVec3ByName( int shaderID, const char* varName, int index, float f1, float f2, float f3 );
			static void SetShaderConstantArrayVec4ByName( int shaderID, const char* varName, int index, float f1, float f2, float f3, float f4 );
			static int IsInvertedDepth();
			static int GetClipSpaceMode();
			static int IsTopLeftOrigin();
			static float GetDrawing3DSetupTime();
			static uint32_t MakeColor(uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha);
			static int GetColorAlpha(int color);
			static void SetPresentMode( int mode );
			static void ForcePresent();

			static int GetAppInstalled( const char *packageName );

			// SnapChat
			static void SetSnapChatStickerSettings( float x, float y, int width, int height, float angle );
			static void ShareSnapChatImage( const char* imageFile, const char* stickerFile, const char* caption, const char* url );
	};
}

#endif
