#import <AVFoundation/AVFoundation.h>
//#import <Cocoa/Cocoa.h>
#include "AGK.h"
#include "../renderer/OpenGL2/OpenGL2.h"
#include "DDHidLib.h"
#include <pthread.h>

#include "QRCodeReader.h"
#include "BinaryBitmap.h"
#include "Binarizer.h"
#include "GreyscaleLuminanceSource.h"
#include "GlobalHistogramBinarizer.h"
#include "ReaderException.h"
#include "QREncode.h"

#include "zlib.h"

#import <CoreLocation/CoreLocation.h>

#include "png.h"
#include "jpeglib.h"

#include <net/if.h>

extern unsigned char libImageAscii[];
extern unsigned char libImageAsciiExt[];

int agk::m_iKeyboardMode = 1; // physical keyboard

#define kMaxTextureSize	 2048
#define D3DCOLOR unsigned int;
#define D3DCOLOR_ARGB(a,r,g,b) ((unsigned int)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define D3DCOLOR_RGBA(r,g,b,a) D3DCOLOR_ARGB(a,r,g,b)

typedef enum {
	kTexture2DPixelFormat_Automatic = 0,
	kTexture2DPixelFormat_RGBA8888,
	kTexture2DPixelFormat_RGB565,
	kTexture2DPixelFormat_A8,
} Texture2DPixelFormat;

using namespace AGK;

//joystick listener class
@interface JoystickListener : NSObject
{
@public
	
}
@end

@implementation JoystickListener

- (void) ddhidJoystick: (DDHidJoystick *) joystick
                 stick: (unsigned) stick
              xChanged: (int) value
{
	//NSLog( @"Joystick: %p, stick: %d, xchanged: %d", joystick, stick, value );
	agk::JoystickAxis( joystick, stick, 0, value/65536.0f );
}

- (void) ddhidJoystick: (DDHidJoystick *) joystick
                 stick: (unsigned) stick
              yChanged: (int) value
{
	//NSLog( @"Joystick: %p, stick: %d, ychanged: %d", joystick, stick, value );
	agk::JoystickAxis( joystick, stick, 1, value/65536.0f );
}

- (void) ddhidJoystick: (DDHidJoystick *) joystick
                 stick: (unsigned) stick
             otherAxis: (unsigned) otherAxis
          valueChanged: (int) value
{
	//NSLog( @"Joystick: %p, stick: %d, axis: %d, changed: %d", joystick, stick, otherAxis, value );
	
	// adjust for xbox third stick (triggers) defaulting to -1
	if ( stick == 2 ) value = value/2 + 32768;
	agk::JoystickAxis( joystick, stick, otherAxis, value/65536.0f );
}

- (void) ddhidJoystick: (DDHidJoystick *) joystick
                 stick: (unsigned) stick
             povNumber: (unsigned) povNumber
          valueChanged: (int) value
{
	//NSLog( @"Joystick: %p, stick: %d, POV: %d, changed: %d", joystick, stick, povNumber, value );
}

- (void) ddhidJoystick: (DDHidJoystick *) joystick
            buttonDown: (unsigned) buttonNumber
{
	//NSLog( @"Joystick: %p, button: %d, down", joystick, buttonNumber );
    switch( buttonNumber )
    {
        case 8: agk::JoystickButton( joystick, 7, 1 ); break;
        case 9: agk::JoystickButton( joystick, 6, 1 ); break;
        case 6: agk::JoystickButton( joystick, 8, 1 ); break;
        case 7: agk::JoystickButton( joystick, 9, 1 ); break;
        case 13: agk::JoystickButton( joystick, 12, 1 ); break;
        case 11: agk::JoystickButton( joystick, 13, 1 ); break;
        case 12: agk::JoystickButton( joystick, 15, 1 ); break;
        default: agk::JoystickButton( joystick, buttonNumber, 1 );
    }
}

- (void) ddhidJoystick: (DDHidJoystick *) joystick
              buttonUp: (unsigned) buttonNumber
{
	//NSLog( @"Joystick: %p, button: %d, up", joystick, buttonNumber );
    switch( buttonNumber )
    {
        case 8: agk::JoystickButton( joystick, 7, 0 ); break;
        case 9: agk::JoystickButton( joystick, 6, 0 ); break;
        case 6: agk::JoystickButton( joystick, 8, 0 ); break;
        case 7: agk::JoystickButton( joystick, 9, 0 ); break;
        case 13: agk::JoystickButton( joystick, 12, 0 ); break;
        case 11: agk::JoystickButton( joystick, 13, 0 ); break;
        case 12: agk::JoystickButton( joystick, 15, 0 ); break;
        default: agk::JoystickButton( joystick, buttonNumber, 0 );
    }
}

@end

@interface AGKMovieView : NSView
{
    
};
@end

@implementation AGKMovieView

// make sure this view never get events
-(NSView*)hitTest:(NSPoint)aPoint
{
    return self.superview;
}

@end

JoystickListener *g_pJoystickListener = 0;

extern "C" {
FILE* AGKfopen( const char *szPath, const char* mode )
{
	FILE* pFile = fopen( szPath, mode );
	return pFile;
}
}

namespace AGK
{
    GLFWwindow *g_pWindow = 0;
    float g_fWindowTimer = 0;
    
	bool g_bIsExternal = false;
	void (*SwapExternal)(void*) = 0;
	void *g_pSwapParam = 0;

	//NSOpenGLContext *context;
	//GLuint defaultFramebuffer, colorRenderbuffer, depthRenderbuffer;

	uint64_t uFixTime = 0;
	mach_timebase_info_data_t machTimeInfo;

	// music
	AVAudioPlayer *musicPlayer;
	   
	// sounds
	ALCcontext *audioContext = NULL;
	ALCdevice *audioDevice = NULL;
    
    // video
    AVURLAsset *g_videoAsset = 0;
    AVPlayer *g_videoPlayer = 0;
    AVPlayerLayer *g_videoLayer = 0;
    AVPlayerItem *g_videoItem = 0;
    int g_videoPlaying = 0;
    AGKMovieView *g_videoView = 0;
    uString g_sVideoFile;
    
    // location
    CLLocationManager *g_pLocationManager = 0;
    
    // capture variables
    NSView *mMainView = 0;
    NSView *mCaptureViewParent = 0;
    /*
    QTCaptureView *mCaptureView = 0;
    QTCaptureSession *mCaptureSession = 0;
    QTCaptureDeviceInput *mCaptureVideoDeviceInput = 0;
    QTCaptureDecompressedVideoOutput *mCaptureOutput = 0;
     */
    CVImageBufferRef mCapturedStream = 0;
    NSButton *mCaptureButton = 0;
    NSButton *mCaptureCancelButton = 0;
    cImage *m_pCaptureImage = 0;

	// default screen size
    int g_iIsAGKFullscreen = 0;
	int g_iDefaultWidth = 0;
	int g_iDefaultHeight = 0;
	
	class cSoundInst
	{
		public:
            UINT m_iID;
            UINT m_uLastUsed;
			int m_iParent;
			int m_iVolume;
            int m_iLoop;
            float m_fRate;
            float m_fBalance;
            int m_iLoopCount;
	
			cSoundInst *m_pPrevInst;
			cSoundInst *m_pNextInst;
		
			ALuint bufferID;		
			ALuint sourceID;			//base source ID
		
			cSoundInst() 
			{ 
				m_uLastUsed = 0;
				m_iParent = 0;
				m_iVolume = 100;
				m_pPrevInst = UNDEF;
				m_pNextInst = UNDEF;
				m_iLoop = 0;
                m_fRate = 1.0f;
                m_fBalance = 0;
                m_iLoopCount = 0;
			}
		
			void Reset()
			{
				m_iParent = 0;
                m_iLoop = 0;
                m_fRate = 1.0f;
                m_fBalance = 0;
                m_iLoopCount = 0;
				if( sourceID ) 
				{
					alSourceStop(sourceID);
					alDeleteSources(1, &sourceID);
				}
				if ( bufferID )
				{
					alDeleteBuffers(1, &bufferID);
				}
				sourceID = 0;
				bufferID = 0;
			}
	};
    
    cHashedList<cSoundInst> m_cSoundInstances(256);
	
	char szAppFolderName[ MAX_PATH ] = "";
	char szRootDir[ MAX_PATH ] = "";
	char szWriteDir[ MAX_PATH ] = "";
	char szOriginalWriteDir[ MAX_PATH ] = "";
	
	NSBundle *g_pBundle = NULL;
	NSFileManager *g_pFileManager = NULL;
	
	// text input variables
	int g_iTextCursorPos = 0;
	cSprite *g_pTextInputCursor = 0;
	cSprite *g_pTextInputArea = 0;
	cSprite *g_pTextInputBack = 0;
	cText *g_pTextInputText = 0;
	float g_fCursorBlinkTime = 0;
	int g_iShowCursor = 1;
	float g_fTextStartX = 0;
	float g_fTextStartY = 0;
	
	cSprite *pTextBackground = 0;
}

@interface ButtonListener : NSObject
{

};
- (void)capturePressed;
- (void)cancelPressed;
@end

@implementation ButtonListener
- (void)capturePressed
{
    /*
    if ( !m_pCaptureImage || !mCaptureOutput ) agk::CancelCapture();
    else 
    {
      
        BOOL success = [mCaptureSession addOutput:mCaptureOutput error:nil ];
        if ( !success ) NSLog( @"Failed to add output" );
    }
     */
}

- (void)cancelPressed
{
    agk::CancelCapture();
}

- (void)captureImage
{
    /*
    if ( !m_pCaptureImage || !mCaptureOutput ) 
    {
        agk::CancelCapture();
        return;
    }
    
    [ mCaptureSession stopRunning ];
    
    NSCIImageRep *imageRep = [NSCIImageRep imageRepWithCIImage:[CIImage imageWithCVImageBuffer:mCapturedStream]];
    NSImage *image = [[NSImage alloc] initWithSize:[imageRep size]];
    [image addRepresentation:imageRep];
    
    
    m_pCaptureImage->Load( (const char*) image );
    
    // clean up
    [[[NSApplication sharedApplication] mainWindow] setContentView:mMainView ];
    
    [ mCaptureSession release ];
    mCaptureSession = 0;
    
    [mCaptureVideoDeviceInput release];
    mCaptureVideoDeviceInput = 0;
    
    [mCaptureOutput release];
    mCaptureOutput = 0;
    
    [ mCaptureViewParent release ];
    mCaptureViewParent = 0;
    
    [ mCaptureView release ];
    mCaptureView = 0;
    
    [ mCaptureButton release ];
    mCaptureButton = 0;
    
    [ mCaptureCancelButton release ];
    mCaptureCancelButton = 0;
    
    m_pCaptureImage = 0;
    
    CVBufferRelease(mCapturedStream);
    mCapturedStream = nil;
    
    agk::CaptureInternal(1);
     */
}

/*
- (void)captureOutput:(QTCaptureOutput *)captureOutput didOutputVideoFrame:(CVImageBufferRef)videoFrame withSampleBuffer:(QTSampleBuffer *)sampleBuffer fromConnection:(QTCaptureConnection *)connection
{
    // Retain the videoFrame so it won't disappear
    // don't forget to release!
    CVBufferRetain(videoFrame);
    
    @synchronized (self) {
        mCapturedStream = videoFrame;
    }
    
    [self performSelectorOnMainThread:@selector(captureImage) withObject:nil waitUntilDone:NO];
}
 */
@end

@interface AGKMovieListener : NSObject 
{
@public
    
}
- (void) playbackFinished: (NSNotification*) pNotification;
@end

@implementation AGKMovieListener

-(void)playbackFinished:(NSNotification*)aNotification
{
    agk::StopVideo();
}

@end

@interface AGKMusicListener : NSObject <AVAudioPlayerDelegate>
{
@public
    
}
- (void) audioPlayerDidFinishPlaying: (AVAudioPlayer*) player successfully:(BOOL)flag;
@end

@implementation AGKMusicListener

-(void) audioPlayerDidFinishPlaying: (AVAudioPlayer*) player successfully:(BOOL)flag
{
    agk::HandleMusicEvents(0);
}

@end

@interface AGKLocationListener : NSObject <CLLocationManagerDelegate>
{
@public
    
}
- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations;
- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation;
- (void)locationManager:(CLLocationManager *)manager didChangeAuthorizationStatus:(CLAuthorizationStatus)status;
@end

@implementation AGKLocationListener

- (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations
{
    CLLocation* location = [locations lastObject];
    NSDate* eventDate = location.timestamp;
    NSTimeInterval howRecent = [eventDate timeIntervalSinceNow];
    if (abs(howRecent) < 15.0) {
        agk::GPS( (float) location.coordinate.longitude, (float) location.coordinate.latitude, (float) location.altitude );
    }
}

- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)location fromLocation:(CLLocation *)oldLocation
{
    NSDate* eventDate = location.timestamp;
    NSTimeInterval howRecent = [eventDate timeIntervalSinceNow];
    if (abs(howRecent) < 15.0) {
        agk::GPS( (float) location.coordinate.longitude, (float) location.coordinate.latitude, (float) location.altitude );
    }
}

- (void)locationManager:(CLLocationManager *)manager didChangeAuthorizationStatus:(CLAuthorizationStatus)status
{
    switch( status )
    {
        case kCLAuthorizationStatusDenied:
        case kCLAuthorizationStatusRestricted: agk::m_iGPSSensorExists = 0; break;
        case kCLAuthorizationStatusAuthorized:
        case kCLAuthorizationStatusNotDetermined: agk::m_iGPSSensorExists = 1; break;
    }
}

@end


namespace AGK
{
    ButtonListener *g_buttonListener = 0;
    AGKMovieListener *g_pAGKMovieListener = 0;
    AGKMusicListener *g_pAGKMusicListener = 0;
    AGKLocationListener *g_pLocationListener = 0;
}


void cFileEntry::TraverseDirectory( const char* dir )
{
	NSString* pPathString = [ [ NSString alloc ] initWithUTF8String:dir ];
	NSArray* contents = [ g_pFileManager contentsOfDirectoryAtPath:pPathString error:NULL ];
	if ( contents ) 
	{	
		for ( int i = 0; i < [ contents count ]; i++ )
		{
			BOOL bIsDirectory = FALSE;
			NSString *pString = [ contents objectAtIndex:i ];
			NSString *fullPath = [pPathString stringByAppendingPathComponent:pString];
			[ g_pFileManager fileExistsAtPath:fullPath isDirectory:(&bIsDirectory) ];
			if ( bIsDirectory )
			{
				// directory
				char str[ 1024 ];
				strcpy( str, dir );
				strcat( str, [ pString UTF8String ] );
				AddNewFile( str );
				
				strcat( str, "/" );
				TraverseDirectory( str );
				
			}
			else
			{
				// file
				char str[ 1024 ];
				strcpy( str, dir );
				strcat( str, [ pString UTF8String ] );
				AddNewFile( str );
			}
		}
	} 
    
    [ pPathString release ];
}

void cFileEntry::InitFileList()
{
	TraverseDirectory( szRootDir );
	TraverseDirectory( szWriteDir );
}

void agk::SetWindowPosition( int x, int y )
{
    if ( !g_pWindow ) return;
    glfwSetWindowPos( g_pWindow, x, y );
}

void agk::SetWindowSize( int width, int height, int fullscreen )
{
	SetWindowSize( width, height, fullscreen, 0 );
}

void agk::SetWindowSize( int width, int height, int fullscreen, int allowOverSized )
{
    if ( !g_pWindow ) return;
    
    // GLFW fullscreen is not the same as Mac fullscreen (green button)
    // must turn off Mac fullscreen if it is being used
    NSWindow *window = [[[NSApplication sharedApplication] windows] objectAtIndex:0];
    if ( (window.styleMask & NSFullScreenWindowMask) != 0 )
    {
        [ window toggleFullScreen:nil ];
    }
    
    if ( g_iDefaultWidth == 0 ) g_iDefaultWidth = agk::GetDeviceWidth();
    if ( g_iDefaultHeight == 0 ) g_iDefaultHeight = agk::GetDeviceHeight();
    
    if ( width < 0 ) width = 0;
    if ( height < 0 ) height = 0;
    
    if ( width == 0 ) width = g_iDefaultWidth;
    if ( height == 0 ) height = g_iDefaultHeight;
    
    if ( allowOverSized == 0 )
    {
        if ( width > agk::GetMaxDeviceWidth() ) width = agk::GetMaxDeviceWidth();
        if ( height > agk::GetMaxDeviceHeight() ) height = agk::GetMaxDeviceHeight();
    }
    
    static int oldX = 0;
    static int oldY = 0;
    
    if ( fullscreen )
    {
        if ( g_iIsAGKFullscreen ) return;
        g_iIsAGKFullscreen = 1;
        
        int nmonitors, i;
        int wx, wy, ww, wh;
        int mx, my, mw, mh;
        int overlap, bestoverlap;
        GLFWmonitor *bestmonitor;
        GLFWmonitor **monitors;
        const GLFWvidmode *mode;
        
        bestoverlap = 0;
        bestmonitor = NULL;
        
        glfwGetWindowPos(g_pWindow, &wx, &wy);
        oldX = wx; oldY = wy;
        glfwGetWindowSize(g_pWindow, &ww, &wh);
        monitors = glfwGetMonitors(&nmonitors);
        
        for (i = 0; i < nmonitors; i++) {
            mode = glfwGetVideoMode(monitors[i]);
            glfwGetMonitorPos(monitors[i], &mx, &my);
            mw = mode->width;
            mh = mode->height;
            
            int mini = wx + ww < mx + mw ? wx + ww : mx + mw;
            int maxi = wx > mx ? wx : mx;
            
            overlap = mini - maxi;
            if ( overlap < 0 ) overlap = 0;
            
            mini = wy + wh < my + mh ? wy + wh : my + mh;
            maxi = wy > my ? wy : my;
            
            int overlap2 = mini - maxi;
            if ( overlap2 < 0 ) overlap2 = 0;
            
            overlap *= overlap2;
            
            if (bestoverlap < overlap) {
                bestoverlap = overlap;
                bestmonitor = monitors[i];
            }
        }
        
        mode = glfwGetVideoMode(bestmonitor);
        glfwSetWindowMonitor( g_pWindow, bestmonitor, 0, 0, mode->width, mode->height, mode->refreshRate );
    }
    else
    {
        if ( !g_iIsAGKFullscreen )
        {
            glfwSetWindowSize( g_pWindow, width, height );
        }
        else
        {
            g_iIsAGKFullscreen = 0;
            glfwSetWindowMonitor( g_pWindow, NULL, oldX, oldY, width, height, 0 );
        }
    }
}

void agk::SetWindowAllowResize( int mode )
{
	if ( mode == 0 ) m_bAGKFlags &= ~AGK_FLAG_CAN_RESIZE;
	else m_bAGKFlags |= AGK_FLAG_CAN_RESIZE;
	
	NSWindow *mainWindow = [[[NSApplication sharedApplication] windows] objectAtIndex:0];
    
    if ( (mainWindow.styleMask & NSFullScreenWindowMask) != 0 ) return; // don't change window style whilst it is full screen

	if ( mode == 0 )
	{
		[mainWindow setStyleMask:[mainWindow styleMask] & ~NSResizableWindowMask];
	}
	else
	{
		[mainWindow setStyleMask:[mainWindow styleMask] | NSResizableWindowMask];
	}
}

void agk::MaximizeWindow()
{
	
}

void agk::MinimizeApp()
{
	//[[NSApplication sharedApplication] miniaturizeAll:nil];
	[[[[NSApplication sharedApplication] windows] objectAtIndex:0] miniaturize];
}

void agk::RestoreApp()
//****
{
	[[[[NSApplication sharedApplication] windows] objectAtIndex:0] deminiaturize];
}

void agk::SetImmersiveMode( int mode )
{
	
}

void agk::SetScreenResolution( int width, int height )
//****
{
	agk::SetWindowSize( width, height, 0 );
}

char* agk::GetURLSchemeText()
//****
{
	char* str = new char[1]; *str = 0;
	return str;
}

void agk::ClearURLSchemeText()
//****
{

}

void agk::GetDeviceName( uString &outString )
//****
{
	outString.SetStr( "mac" );
}

char* agk::GetDeviceName( )
//****
{
	char *str = new char[ 4 ];
	strcpy( str, "mac" );
	return str;
}

char* agk::GetDeviceBaseName( )
//****
{
	char *str = new char[ 4 ];
	strcpy( str, "mac" );
	return str;
}

char* agk::GetDeviceType( )
//****
{
	SInt32 major, minor;
	Gestalt(gestaltSystemVersionMajor, &major);
	Gestalt(gestaltSystemVersionMinor, &minor);

	char *str = new char[ 30 ];
	sprintf( str, "%d.%d", major, minor );
	return str;
}

int agk::GetDeviceNetworkType()
{
	return -1;
}

int agk::GetStorageRemaining( const char *path )
{
	return -1;
}

int agk::GetStorageTotal( const char *path )
{
	return -1;
}
void agk::GetAppName( uString &outString )
//****
{
	outString.SetStr( m_sAppName );
}

char* agk::GetAppName( )
//****
{
	char *str = new char[ m_sAppName.GetLength() ];
	strcpy( str, m_sAppName.GetStr() );
	return str;
}

bool agk::GetDeviceHasSplash()
{
	//if ( agk::GetFileExists( "Default.jpg" ) ) return true;
	return false;
}

char* agk::GetDeviceLanguage( )
//****
{
	NSString * language = [[NSLocale preferredLanguages] objectAtIndex:0];
	const char* szLang = [language UTF8String];

	char *str = new char[ strlen(szLang)+1 ];
	strcpy(str, szLang);
	return str;
}

void agk::SetSleepMode( int mode )
//****
{
	// do nothing on mac
}

void agk::SetExpansionFileKey( const char *key )
//****
{
	// do nothing
}

void agk::SetExpansionFileVersion(int version)
{
	// do nothing on mac
}

int agk::GetExpansionFileState()
{
	return 0;
}

int agk::GetExpansionFileError()
{
	return 0;
}

void agk::DownloadExpansionFile()
{
	// do nothing on mac
}

float agk::GetExpansionFileProgress()
{
	return 0;
}

bool agk::ExtractExpansionFile( const char* localFile, const char* expansionFile )
{
	return false;
}

void agk::SetWindowTitle( const char *szTitle )
{
	glfwSetWindowTitle( g_pWindow, szTitle );
}

bool agk::GetDeviceCanRotate()
{
	return false;
}

bool agk::GetAGKShouldRotate()
{
	return false;
}

void agk::PlatformSetOrientationAllowed( int portrait, int portrait2, int landscape, int landscape2 )
{
	// do nothing on Mac
}

bool agk::PlatformGetDeviceID( uString &out )
{
	out.SetStr("");
	return true;
}

float agk::PlatformDeviceScale()
{
	return 1.0f;
}

int agk::PlatformGetNumProcessors()
{
    NSUInteger a = [[NSProcessInfo processInfo] processorCount];
	return a;
}

struct AGKData
{
	int width;
	int height;
};

void agk::PlatformInitFilePaths()
{
	g_pBundle = [NSBundle mainBundle];
	g_pFileManager = [[NSFileManager alloc] init];
	
	// get app name and size for write folder
	//strcpy( szAppFolderName, "Unknown" );
	m_sAppName.SetStr( "Unknown" );
	char szRoot[ MAX_PATH ];
	uint32_t size = MAX_PATH;
	if ( _NSGetExecutablePath(szRoot, &size) == 0 )
	{
		realpath( szRoot, szRootDir );

		char *szEXE = strrchr( szRootDir, '.' );
		if ( szEXE ) 
		{
			*szEXE = 0;
			while( szEXE > szRootDir && *szEXE != '/' ) szEXE--;
		}
		else 
		{
			szEXE = strrchr( szRootDir, '/' );
			if ( !szEXE ) szEXE = szRootDir;
		}		
		
		if ( *szEXE == '/' ) szEXE++;
		
		m_sAppName.SetStr( szEXE );
						
		//uString sModule( szRootDir );
		//sModule.Strip( "\\/. \t" );
		//strcpy( szAppFolderName, sModule.GetStr() );
	}
	
	// get main bundle resource path
	NSString *path = [g_pBundle resourcePath];
	strcpy( szRootDir, [path UTF8String] );
	strcat( szRootDir, "/" );
	
	// create a write directory in the main documents folder
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES); 
	NSString *supportDirectoryPath = [paths objectAtIndex:0];
	const char* szAppSupport = [ supportDirectoryPath UTF8String ];
	strcpy( szWriteDir, szAppSupport );
	strcat( szWriteDir, "/" );
	if ( m_sCompanyName.GetLength() > 0 )
	{
		strcat( szWriteDir, m_sCompanyName.GetStr() );
		strcat( szWriteDir, "/" );
	}
	strcat( szWriteDir, m_sAppName );
	strcat( szWriteDir, "/" );
	//strcat( szWriteDir, szAppFolderName );
	//strcat( szWriteDir, "/" );
    
	NSString* pWriteDir = [ [ NSString alloc ] initWithUTF8String:szWriteDir ];
	[ g_pFileManager createDirectoryAtPath:pWriteDir withIntermediateDirectories:YES attributes:nil error:NULL ];
	[ pWriteDir release ];
	
	// make sure this value is set here incase restore is called without changing path
	strcpy( szOriginalWriteDir, szWriteDir );
    
    //ParseCurrentDirectory();
	m_bUpdateFileLists = true;
}

void agk::PlatformUpdateWritePath()
{
	// get my documents folder
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES); 
	NSString *supportDirectoryPath = [paths objectAtIndex:0];
	const char* szAppSupport = [ supportDirectoryPath UTF8String ];
	strcpy( szWriteDir, szAppSupport );
	strcat( szWriteDir, "/AGK Player/" );
	if ( m_sAppName.GetLength() > 0 )
	{
		uString sModule( m_sAppName ); // make documents path simpler
		sModule.Replace( '\\', '_' );
		sModule.Replace( '/', '_' );
		sModule.Strip( "\\/.: \t*" );
		strcat( szWriteDir, sModule );
		strcat( szWriteDir, "/" );
	}
	
	NSString* pWriteDir = [ [ NSString alloc ] initWithUTF8String:szWriteDir ];
	[ g_pFileManager createDirectoryAtPath:pWriteDir withIntermediateDirectories:YES attributes:nil error:NULL ];
	[ pWriteDir release ];
	
	//ParseCurrentDirectory();
	m_bUpdateFileLists = true;
}

void agk::PlatformRestoreWritePath()
{
	if ( strlen ( szOriginalWriteDir ) > 0 ) strcpy( szWriteDir, szOriginalWriteDir );
	
	//ParseCurrentDirectory();
	m_bUpdateFileLists = true;
}

void agk::OverrideDirectories( const char *szWrite, int useRead )
{
	if ( szWrite && *szWrite )
	{
		strcpy( szWriteDir, szWrite );
		cFileEntry::ClearAll();
		m_bUpdateFileLists = true;
	}
	else
	{
		char *szTemp = GetAppName();
		SetAppName( szTemp );
		delete [] szTemp;
	}
}

void agk::PlatformSetDevicePtr( void* ptr )
{
	NSWindow *window = [[[NSApplication sharedApplication] windows] objectAtIndex:0];
}

NSArray * joysticks = 0;

void agk::InitJoysticks()
{
    // joystick setup
    g_pJoystickListener = [ JoystickListener alloc ];
    
    // on the MAC I get 'does not recognise selector exception!'
    // requires the DDHidLib from "platform/mac/DDHidLib"
    if ( !joysticks )
        joysticks = [[DDHidJoystick allJoysticks] retain ];
    
    for ( int i = 0; i < [joysticks count] && i < AGK_NUM_JOYSTICKS; i++ )
    {
        DDHidJoystick *pJoystick = [joysticks objectAtIndex:i];
        
        m_pJoystick[ i ] = new cJoystick( pJoystick );
        
        [ pJoystick setDelegate:g_pJoystickListener ];
        [ pJoystick startListening ];
        
        /*
         NSLog( @"Joystick Found" );
         
         NSLog( @"Buttons: %d", [[ pJoystick buttonElements ] count ] );
         NSLog( @"Sticks: %d", [ pJoystick countOfSticks ] );
         for( int i = 1; i < [ pJoystick countOfSticks ]; i++ )
         {
         NSArray *properties = [[[ pJoystick objectInSticksAtIndex:i ] objectInStickElementsAtIndex:0] elements] ;
         int count = [properties count];
         NSLog( @"Sticks: %d - %d", i, count );
         }
         */
    }
}

void agk::PlatformInitCommon()
{
	agk::PlatformDisableScissor();
	
	uFixTime = mach_absolute_time();
	mach_timebase_info(&machTimeInfo);
    SetRandomSeed(uFixTime);
		
	// sound setup
	cSoundMgr::Init();
    
    // location setup
    g_pLocationManager = [[CLLocationManager alloc] init];
    g_pLocationListener = [[AGKLocationListener alloc] init];
    switch( [CLLocationManager authorizationStatus] )
    {
        case kCLAuthorizationStatusDenied:
        case kCLAuthorizationStatusRestricted: m_iGPSSensorExists = 0; break;
        case kCLAuthorizationStatusAuthorized:
        case kCLAuthorizationStatusNotDetermined: m_iGPSSensorExists = 1; break;
    }
    g_pLocationManager.delegate = g_pLocationListener;
        
	//agk::Warning( extensions );
	//agk::Warning( "" );
	
	cImage::GetMaxTextureSize(); // preloads the value so that any threads don't call the GL function behind it
	PlatformSetAlignment( 1 ); // not using memory alignment

	AGKShader::CreateDefaultShaders();
	
	// text input setup
	float DevToVirX = agk::GetVirtualWidth() / (float)agk::m_fTargetViewportWidth;
	float DevToVirY = agk::GetVirtualHeight() / (float)agk::m_fTargetViewportHeight;
	
	float width = 250 * DevToVirX;
	float height = 22 * DevToVirY;
	
	if ( width > agk::GetVirtualWidth() ) width = (float) agk::GetVirtualWidth();
	
	g_fTextStartX = (agk::GetVirtualWidth() - width)/2.0f + 3*DevToVirX;
	g_fTextStartY = agk::GetVirtualHeight()/3.0f + 2*DevToVirY;
	
	g_pTextInputCursor = new cSprite();
	g_pTextInputCursor->SetSize( 2 * DevToVirX, 18 * DevToVirY );
	g_pTextInputCursor->SetColor( 102, 213, 255, 255 );
	g_pTextInputCursor->SetPosition( g_fTextStartX, g_fTextStartY );
	g_pTextInputCursor->FixToScreen(1);

	g_pTextInputArea = new cSprite();
	g_pTextInputArea->SetSize( width, height );
	g_pTextInputArea->SetColor( 255,255,255,255 );
	g_pTextInputArea->SetPosition( (agk::GetVirtualWidth() - width) / 2.0f, agk::GetVirtualHeight()/3.0f );
	g_pTextInputArea->FixToScreen(1);

	width += 8 * DevToVirX;
	height += 8 * DevToVirY;

	g_pTextInputBack = new cSprite();
	g_pTextInputBack->SetSize( width, height );
	g_pTextInputBack->SetColor( 190,190,190,255 );
	g_pTextInputBack->SetPosition( (agk::GetVirtualWidth() - width) / 2.0f, agk::GetVirtualHeight()/3.0f - 4*DevToVirY );
	g_pTextInputBack->FixToScreen(1);

	g_pTextInputText = new cText(30);
	g_pTextInputText->SetPosition( g_fTextStartX, g_fTextStartY );
	g_pTextInputText->SetColor( 0,0,0 );
	g_pTextInputText->SetSpacing( 0 );
	g_pTextInputText->FixToScreen(1);
	
	//cImage *pArial = new cImage( "/Arial.png" );
	//g_pTextInputText->SetFontImage( pArial );
	g_pTextInputText->SetFont( 0 );
	
	pTextBackground = new cSprite();
	pTextBackground->SetColor( 0,0,0, 128 );
	pTextBackground->SetSize( agk::GetVirtualWidth(), agk::GetVirtualHeight() );
	pTextBackground->SetPosition(0,0);
	pTextBackground->FixToScreen(1);
}

void agk::PlatformInitExternal( void* ptr, int width, int height, void(*swap)(void*), void* swapParam )
{
	if ( !swap ) agk::Warning( "AGK was initialised with InitExternal but no swap function was given" );

	g_bIsExternal = true;
	SwapExternal = swap;
	g_pSwapParam = swapParam;

	m_iRenderWidth = width;
	m_iRenderHeight = height;
	cCamera::UpdateAllAspectRatio( m_iRenderWidth/(float)m_iRenderHeight );

	m_iRealDeviceWidth = m_iRenderWidth;
	m_iRealDeviceHeight = m_iRenderHeight;

	PlatformInitCommon();
}

struct egldata
{
    GLFWwindow* window;
};

void agk::PlatformInitGL( void* ptr )
{
	g_bIsExternal = false;
    
    g_pWindow = ((egldata*)ptr)->window;
    
	NSWindow *window = [[[NSApplication sharedApplication] windows] objectAtIndex:0];
	m_iRealDeviceWidth = [[ window contentView ] frame ].size.width;
	m_iRealDeviceHeight = [[ window contentView ] frame ].size.height;
    
    glfwGetFramebufferSize( g_pWindow, &m_iRenderWidth, &m_iRenderHeight );
	cCamera::UpdateAllAspectRatio( m_iRenderWidth/(float)m_iRenderHeight );
    
	PlatformInitCommon();
    
    g_fWindowTimer = agk::Timer() + 0.5;
}

void agk::PlatformInitConsole()
{
	uFixTime = mach_absolute_time();
	mach_timebase_info(&machTimeInfo);
    SetRandomSeed(uFixTime);
    
	// sound setup
	cSoundMgr::Init();
}

void agk::UpdatePtr( void *ptr )
{
	
}

void agk::UpdatePtr2( void *ptr )
{
	
}

int agk::GetInternalDataI( int index )
{
	return 0;
}

void agk::WindowMoved()
{
	
}

void agk::PlatformSetBlendMode( int mode )
{	
	switch( mode )
	{
		case 0: // opaque
		{
			agk::PlatformSetDepthWrite( 1 );
			agk::PlatformSetDepthFunc( AGK_DEPTH_LESS );
			agk::PlatformSetBlendEnabled( 0 );
			break;
		}

		case 1: // alpha blending
		{
			agk::PlatformSetDepthWrite( 0 );
			agk::PlatformSetDepthFunc( AGK_DEPTH_LEQUAL );
			agk::PlatformSetBlendEnabled( 1 );
			agk::PlatformSetBlendFunc( AGK_BLEND_SRC_ALPHA, AGK_BLEND_ONE_MINUS_SRC_ALPHA );
			break;
		}

		case 2: // additive blending
		{
			agk::PlatformSetDepthWrite( 0 );
			agk::PlatformSetDepthFunc( AGK_DEPTH_LEQUAL );
			agk::PlatformSetBlendEnabled( 1 );
			agk::PlatformSetBlendFunc( AGK_BLEND_ONE, AGK_BLEND_ONE );
			break;
		}
	}
}

void agk::SetVSync( int mode )
{
	if ( mode < 0 ) mode = 0;
	glfwSwapInterval( mode );

	if ( mode > 0 ) m_iSyncMode = 2;
	else m_iSyncMode = 0;

	m_bUsingVSync = mode > 0;
}

void agk::Sleep( UINT milliseconds )
//****
{
	// convert from milliseconds to microseconds
	usleep( milliseconds*1000 ); 
}

void agk::PlatformCleanUp()
{
    SocialPluginsDestroy();
    agk::StopSound(0); // stop all
    AGKMusicOGG::DeleteAll();
	cSoundMgr::Destroy();
}

int agk::GetMaxDeviceWidth()
//****
{
    NSScreen *mainScreen = [NSScreen mainScreen];
    NSRect screenRect = [mainScreen frame];
	return screenRect.size.width;
}

int agk::GetMaxDeviceHeight()
//****
{
    NSScreen *mainScreen = [NSScreen mainScreen];
    NSRect screenRect = [mainScreen frame];
	return screenRect.size.height;
}

int agk::GetDeviceDPI()
//****
{
	return 0;
}

char* agk::GetAppPackageName()
//****
{
	char *str = new char[1];
	*str = 0;
	return str;
}

int agk::GetDevicePlatform()
//****
{
	return 0;
}

void agk::PlatformUpdateDeviceSize()
{
	NSWindow *window = [[[NSApplication sharedApplication] windows] objectAtIndex:0];
	m_iRealDeviceWidth = [[ window contentView ] frame ].size.width;
	m_iRealDeviceHeight = [[ window contentView ] frame ].size.height;
    
    glfwGetFramebufferSize( g_pWindow, &m_iRenderWidth, &m_iRenderHeight );
	cCamera::UpdateAllAspectRatio( m_iRenderWidth/(float)m_iRenderHeight );
        
    /*
    //capture
    if ( mCaptureView )
    {
        [mCaptureView setFrame:NSMakeRect( 0,50, m_iRealDeviceWidth, m_iRealDeviceHeight-50) ];
        [mCaptureButton setFrame:NSMakeRect( m_iRealDeviceWidth/2-160,10, 150, 30) ];
        [mCaptureCancelButton setFrame:NSMakeRect( m_iRealDeviceWidth/2+10,10, 150, 30) ];
    }
     */
    
    agk::SetVideoDimensions(m_fVideoX, m_fVideoY, m_fVideoWidth, m_fVideoHeight );
}

void agk::PlatformUpdateTime ( void )
{
	uint64_t now = mach_absolute_time()-uFixTime;
	m_fTimeCurr = now * 1e-9 * (machTimeInfo.numer / (double)machTimeInfo.denom);
    m_iTimeMilliseconds = agk::Round(now * 1e-6 * (machTimeInfo.numer / (double)machTimeInfo.denom));
	//printf( "%.4f ", m_fTimeCurr ); 
}

void agk::PlatformResetTime ( void )
{
    uFixTime = mach_absolute_time();
    m_fTimeCurr = 0;
	m_iTimeMilliseconds = 0;
}

double agk::PlatformGetRawTime ( void )
{
	uint64_t now = mach_absolute_time()-uFixTime;
	return now * 1e-9 * (machTimeInfo.numer / (double)machTimeInfo.denom);
}

void agk::CompositionChanged()
{

}

void agk::PlatformSync()
{
	if ( !g_bIsExternal )
    {
        if ( g_fWindowTimer > 0 && agk::Timer() > g_fWindowTimer )
        {
            int winX, winY;
            glfwGetWindowPos(g_pWindow, &winX, &winY );
            glfwSetWindowPos(g_pWindow, winX, winY+1);
            g_fWindowTimer = 0;
        }
        PlatformSwap();
    }
	else 
	{
		if ( SwapExternal ) SwapExternal( g_pSwapParam );
	}
}

void agk::PlatformCompleteInputInit()
{
	
}

void agk::KeyboardMode( int mode )
{
	m_iKeyboardMode = mode;
}

int agk::PlatformInputPointerPressed(float x, float y)
{
	return 1;
}

void agk::PlatformStartTextInput( const char *sInitial )
{
	if ( m_bInputStarted ) return;
	
	float DevToVirX = agk::GetVirtualWidth() / (float)agk::m_fTargetViewportWidth;
	float DevToVirY = agk::GetVirtualHeight() / (float)agk::m_fTargetViewportHeight;
	
	float width = 250 * DevToVirX;
	float height = 22 * DevToVirY;
	
	if ( width > agk::GetVirtualWidth() ) width = (float) agk::GetVirtualWidth();
	
	g_fTextStartX = (agk::GetVirtualWidth() - width)/2.0f + 3*DevToVirX;
	g_fTextStartY = agk::GetVirtualHeight()/3.0f + 2*DevToVirY;
	
	g_pTextInputCursor->SetSize( 2 * DevToVirX, 18 * DevToVirY );
	g_pTextInputCursor->SetColor( 102, 213, 255, 255 );
	g_pTextInputCursor->SetPosition( g_fTextStartX, g_fTextStartY );
	
	g_pTextInputArea->SetSize( width, height );
	g_pTextInputArea->SetColor( 255,255,255,255 );
	g_pTextInputArea->SetPosition( (agk::GetVirtualWidth() - width) / 2.0f, agk::GetVirtualHeight()/3.0f );
	
	width += 8 * DevToVirX;
	height += 8 * DevToVirY;
	
	g_pTextInputBack->SetSize( width, height );
	g_pTextInputBack->SetColor( 190,190,190,255 );
	g_pTextInputBack->SetPosition( (agk::GetVirtualWidth() - width) / 2.0f, agk::GetVirtualHeight()/3.0f - 4*DevToVirY );
	
	m_sCurrInput.SetStr( sInitial );
	g_pTextInputText->SetString( sInitial );
	g_pTextInputText->SetPosition( g_fTextStartX, g_fTextStartY );
	g_pTextInputText->SetColor( 0,0,0 );
	g_pTextInputText->SetSpacing( 0 );
	g_pTextInputText->SetSize( 20 * DevToVirY );
	
	g_iTextCursorPos = g_pTextInputText->GetLength();
}

void agk::PlatformStopTextInput() 
{
	// do nothing
}

void agk::PlatformChangeTextInput( const char* str )
{
	m_sCurrInput.SetStr( str );
}

void agk::PlatformUpdateTextInput()
{
	if ( !m_bInputStarted ) return;
	
	if ( m_iLastChar != m_iCurrChar )
	{
		if ( m_iInputMaxChars > 0 && m_sCurrInput.GetNumChars() >= m_iInputMaxChars )
		{
			g_iShowCursor = 1;
			g_fCursorBlinkTime = m_fCursorBlinkDelay;
		}
		else
		{
			if ( g_iTextCursorPos < (int) m_sCurrInput.GetNumChars() )
			{
				m_sCurrInput.InsertUnicodeAt( g_iTextCursorPos, m_iCurrChar );
			}
			else
			{
				m_sCurrInput.AppendUnicode( m_iCurrChar );
			}
            
			g_iTextCursorPos++;
			if ( g_iTextCursorPos > (int) m_sCurrInput.GetNumChars() ) g_iTextCursorPos = m_sCurrInput.GetNumChars();
		}
	}
	
	// enter pressed
	if ( !m_iPrevKeyDown[ AGK_KEY_ENTER ] && m_iKeyDown[ AGK_KEY_ENTER ] )
	{
		m_bInputStarted = false;
		m_bInputCancelled = false;
	}
	
	// escape pressed
	if ( !m_iPrevKeyDown[ AGK_KEY_ESCAPE ] && m_iKeyDown[ AGK_KEY_ESCAPE ] )
	{
		m_sCurrInput.SetStr( "" );
		m_bInputCancelled = true;
		m_bInputStarted = false;
	}
	
	// backspace pressed
	static float fBackspaceTimer = 0;
	bool bBackspaceRepeat = false;
	if ( !m_iKeyDown[ AGK_KEY_BACK ] ) fBackspaceTimer = 0;
	
	if ( fBackspaceTimer > 0 )
	{
		fBackspaceTimer -= agk::GetFrameTime();
		if ( fBackspaceTimer <= 0 ) 
		{
			fBackspaceTimer = 0.05f;
			bBackspaceRepeat = true;
		}
	}
	
	if ( (!m_iPrevKeyDown[ AGK_KEY_BACK ] && m_iKeyDown[ AGK_KEY_BACK ]) || bBackspaceRepeat )
	{
		if ( g_iTextCursorPos > 0 )
		{
			m_sCurrInput.DeleteCharAt( g_iTextCursorPos-1 );
			g_iTextCursorPos--;
		}
		g_iShowCursor = 1;
		g_fCursorBlinkTime = m_fCursorBlinkDelay;
		
		if ( fBackspaceTimer <= 0 ) fBackspaceTimer = 0.5f;
	}
	
	// delete repeat
	static float fDeleteTimer = 0;
	bool bDeleteRepeat = false;
	if ( !m_iKeyDown[ AGK_KEY_DELETE ] ) fDeleteTimer = 0;
	
	if ( fDeleteTimer > 0 )
	{
		fDeleteTimer -= agk::GetFrameTime();
		if ( fDeleteTimer <= 0 ) 
		{
			fDeleteTimer = 0.05f;
			bDeleteRepeat = true;
		}
	}
	
	// delete pressed
	if ( (!m_iPrevKeyDown[ AGK_KEY_DELETE ] && m_iKeyDown[ AGK_KEY_DELETE ]) || bDeleteRepeat )
	{
		if ( g_iTextCursorPos < (int)m_sCurrInput.GetNumChars() )
		{
			m_sCurrInput.DeleteCharAt( g_iTextCursorPos );
		}
		
		if ( fDeleteTimer <= 0 ) fDeleteTimer = 0.5f;
	}
	
	// left repeat
	static float fLeftTimer = 0;
	bool bLeftRepeat = false;
	if ( !m_iKeyDown[ AGK_KEY_LEFT ] ) fLeftTimer = 0;
	
	if ( fLeftTimer > 0 )
	{
		fLeftTimer -= agk::GetFrameTime();
		if ( fLeftTimer <= 0 ) 
		{
			fLeftTimer = 0.05f;
			bLeftRepeat = true;
		}
	}
	
	// left arrow pressed
	if ( (!m_iPrevKeyDown[ AGK_KEY_LEFT ] && m_iKeyDown[ AGK_KEY_LEFT ]) || bLeftRepeat )
	{
		g_iTextCursorPos--;
		if ( g_iTextCursorPos < 0 ) g_iTextCursorPos = 0;
		g_iShowCursor = 1;
		g_fCursorBlinkTime = m_fCursorBlinkDelay;
		
		if ( fLeftTimer <= 0 ) fLeftTimer = 0.5f;
	}
	
	// right repeat
	static float fRightTimer = 0;
	bool bRightRepeat = false;
	if ( !m_iKeyDown[ AGK_KEY_RIGHT ] ) fRightTimer = 0;
	
	if ( fRightTimer > 0 )
	{
		fRightTimer -= agk::GetFrameTime();
		if ( fRightTimer <= 0 ) 
		{
			fRightTimer = 0.05f;
			bRightRepeat = true;
		}
	}
	
	// right arrow pressed
	if ( (!m_iPrevKeyDown[ AGK_KEY_RIGHT ] && m_iKeyDown[ AGK_KEY_RIGHT ]) || bRightRepeat )
	{
		g_iTextCursorPos++;
		if ( g_iTextCursorPos > (int) m_sCurrInput.GetNumChars() ) g_iTextCursorPos = m_sCurrInput.GetNumChars();
		g_iShowCursor = 1;
		g_fCursorBlinkTime = m_fCursorBlinkDelay;
		
		if ( fRightTimer <= 0 ) fRightTimer = 0.5f;
	}
	
	// end pressed
	if ( !m_iPrevKeyDown[ AGK_KEY_END ] && m_iKeyDown[ AGK_KEY_END ] )
	{
		g_iTextCursorPos = m_sCurrInput.GetNumChars();
		g_iShowCursor = 1;
		g_fCursorBlinkTime = m_fCursorBlinkDelay;
	}
	
	// home pressed
	if ( !m_iPrevKeyDown[ AGK_KEY_HOME ] && m_iKeyDown[ AGK_KEY_HOME ] )
	{
		g_iTextCursorPos = 0;
		g_iShowCursor = 1;
		g_fCursorBlinkTime = m_fCursorBlinkDelay;
	}
}

void agk::PlatformDrawTextInput()
{
	if ( !m_bInputStarted ) return;
	
	agk::PlatformSetDepthTest( 0 );
	
	pTextBackground->SetSize( GetVirtualWidth(), GetVirtualHeight() );
	pTextBackground->Draw();
	
	g_pTextInputBack->Draw();
	g_pTextInputArea->Draw();
	
	float fX = g_pTextInputArea->GetX();
	float fY = g_pTextInputArea->GetY();
	float fWidth = g_pTextInputArea->GetWidth();
	float fHeight = g_pTextInputArea->GetHeight();
	float fX2 = g_pTextInputArea->GetX() + g_pTextInputArea->GetWidth();
	float fY2 = g_pTextInputArea->GetY() + g_pTextInputArea->GetHeight();

	// swap Y for viewport
	if ( agk::m_bUsingFBO )
	{
		int iScissorX = agk::ScreenToViewportX( fX );
		int iScissorY = agk::ScreenToViewportY( fY );
		int iScissorWidth = agk::ScreenToViewportX( fX2 ) - iScissorX;
		int iScissorHeight = agk::ScreenToViewportY( fY2 ) - iScissorY;

		agk::PlatformScissor( iScissorX, iScissorY, iScissorWidth, iScissorHeight );
	}
	else
	{
		int iScissorX = agk::ScreenToViewportX( fX );
		int iScissorY = agk::ScreenToViewportY( fY2 );
		int iScissorWidth = agk::ScreenToViewportX( fX2 ) - iScissorX;
		int iScissorHeight = agk::ScreenToViewportY( fY ) - iScissorY;

		agk::PlatformScissor( iScissorX, iScissorY, iScissorWidth, iScissorHeight );
	}

	g_pTextInputText->SetString( m_sCurrInput );

	float posX = g_pTextInputText->GetX();
	float posY = g_pTextInputText->GetY();
	if ( g_iTextCursorPos >= 0 )
	{
		if ( g_iTextCursorPos >= (int)g_pTextInputText->GetLength() ) 
		{
			posX += g_pTextInputText->GetCharX( g_pTextInputText->GetLength()-1 ) + g_pTextInputText->GetCharWidth( g_pTextInputText->GetLength()-1 );
			posY += g_pTextInputText->GetCharY( g_pTextInputText->GetLength()-1 );
		}
		else 
		{
			posX += g_pTextInputText->GetCharX( g_iTextCursorPos );
			posY += g_pTextInputText->GetCharY( g_iTextCursorPos );
		}
	}

	float DevToVirY = agk::GetVirtualHeight() / agk::m_fTargetViewportHeight;
	float padX = 2*DevToVirY;
	if ( posX + padX > fX + fWidth )
	{
		float newX = g_pTextInputText->GetX() - ((posX + padX) - (fX + fWidth));
		g_pTextInputText->SetPosition( newX, g_pTextInputText->GetY() );
	}
	else if ( posX < fX + padX )
	{
		float newX = g_pTextInputText->GetX() + ((fX + padX) - posX);
		if ( newX > fX + 1 ) newX = fX + 1;
		g_pTextInputText->SetPosition( newX, g_pTextInputText->GetY() );
	}

	g_pTextInputText->Draw();

	g_fCursorBlinkTime -= agk::GetFrameTime();
	if ( g_fCursorBlinkTime <= 0 )
	{
		g_iShowCursor = 1 - g_iShowCursor;
		g_fCursorBlinkTime = m_fCursorBlinkDelay;
	}

	if ( g_iShowCursor ) 
	{
		float posX = g_pTextInputText->GetX();
		float posY = g_pTextInputText->GetY();
		if ( g_iTextCursorPos >= 0 )
		{
			if ( g_iTextCursorPos >= (int)g_pTextInputText->GetLength() ) 
			{
				posX += g_pTextInputText->GetCharX( g_pTextInputText->GetLength()-1 ) + g_pTextInputText->GetCharWidth( g_pTextInputText->GetLength()-1 );
				posY += g_pTextInputText->GetCharY( g_pTextInputText->GetLength()-1 );
			}
			else 
			{
				posX += g_pTextInputText->GetCharX( g_iTextCursorPos );
				posY += g_pTextInputText->GetCharY( g_iTextCursorPos );
			}
		}

		g_pTextInputCursor->SetPosition( posX, posY );
		g_pTextInputCursor->Draw();
	}
}

void agk::PlatformResumed()
{
	// do nothing
}

void agk::PlatformResumedOpenGL()
{
	// do nothing
}

void agk::PlatformDeviceVolume()
{
	// mac doesn't require app control of the device volume
}

UINT agk::PlatformLittleEndian( UINT u )
{
	// translate from local endian to little endian
	return u;
}

int agk::PlatformLittleEndian( int i )
{
	return i;
}

UINT agk::PlatformLocalEndian( UINT u )
{
	// translate from little endian to local endian
	return u;
}

int agk::PlatformLocalEndian( int i )
{
	return i;
}

float agk::Sin( float a )
//****
{
	return sin( a * PI / 180 );
}

float agk::Cos( float a )
//****
{
	return cos( a * PI / 180 );
}

float agk::Tan( float a )
//****
{
	return tan( a * PI / 180 );
}

float agk::SinRad( float a )
//****
{
	return sin( a );
}

float agk::CosRad( float a )
//****
{
	return cos( a );
}

float agk::TanRad( float a )
//****
{
	return tan( a );
}

float agk::ASin( float a )
//****
{
	return asin( a ) * 180 / PI;
}

float agk::ACos( float a )
//****
{
	return acos( a ) * 180 / PI;
}

float agk::ATan( float a )
//****
{
	return atan( a ) * 180 / PI;
}

float agk::ATan2( float y, float x )
//****
{
	return atan2( y, x ) * 180 / PI;
}

float agk::ASinRad( float a )
//****
{
	return asin( a );
}

float agk::ACosRad( float a )
//****
{
	return acos( a );
}

float agk::ATanRad( float a )
//****
{
	return atan( a );
}

float agk::ATan2Rad( float y, float x )
//****
{
	return atan2( y, x );
}

int agk::Trunc( float a )
{
	if ( a > 0 ) return Floor( a );
	else return Ceil( a );
}

int agk::Floor( float a )
{
	return lrintf( floor( a ) );
}

int agk::Ceil( float a )
{
	return lrintf( ceil( a ) );
}

int agk::Round( float a )
{
	return lrintf( a );
}

float agk::Sqrt( float a )
{
	if ( a > 0 ) return sqrt( a );
	else return 0;
}

float agk::Abs( float a )
{
	if ( a < 0 ) a = -a;
	return a;
}

int agk::Mod( int a, int b )
//****
{
	return a % b;
}

float agk::FMod( float a, float b )
//****
{
	return fmod( a, b );
}

float agk::Pow( float a, float b )
//****
{
	return pow( a, b );
}

float agk::Log( float a )
//****
{
	return log( a );
}

void RemovePath( uString &sPath )
{
	int index = sPath.RevFind( '/' );
	if ( index >= 0 )
	{
		uString out;
		sPath.SubString( out, index+1 );
		sPath.SetStr( out );
	}
}

bool cImage::ChooseFromSystem()
{
    // do nothing on Mac
    return true;
}

void agk::PlatformShowChooseScreen()
{
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setCanChooseDirectories:FALSE];
    [panel setAllowsMultipleSelection:FALSE];
    
    NSInteger result = [panel runModalForTypes:[NSArray arrayWithObjects:@"jpg",@"png",@"jpeg",@"bmp",nil ]];
    if ( result == NSOKButton )
    {
        m_pUncollectedChosenImage = new cImage();
                
        NSString *pPath = [panel filename];
        uString sPath( "raw:" ); sPath.Append( [pPath UTF8String] );
        
        m_pUncollectedChosenImage->Load( sPath );
    }
        
    m_bIsChoosing = false;
}

bool agk::PlatformShowCaptureScreen()
{
    if ( !agk::GetCameraExists() ) return false; 
    
    UINT iID = m_cImageList.GetFreeID( 0x7fffffff );
	
	if ( iID == 0 )
	{
#ifdef _AGK_ERROR_CHECK
		uString errStr( "Failed to capture image - no free image IDs found" );
		agk::Error( errStr );
#endif
		return false;
	}
    
	cImage* pImage = new cImage();
    pImage->m_iSpecialLoadMode = 1;
    m_cImageList.AddItem( pImage, iID );
    m_iUncollectedCaptureImage = iID;
    
    m_pCaptureImage = pImage;
    
    // Create the capture session
    //capture
    /*
    if ( mCaptureSession ) return true;
    mCaptureSession = [[QTCaptureSession alloc] init];
    
    // Connect inputs and outputs to the session    
    
    BOOL success = NO;
    NSError *error;
    
    // Find a video device  
    
    QTCaptureDevice *videoDevice = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeVideo];
    success = [videoDevice open:&error];
    
    
    // If a video input device can't be found or opened, try to find and open a muxed input device
    
    if (!success) {
        videoDevice = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeMuxed];
        success = [videoDevice open:&error];
        
    }
    
    if (!success) {
        videoDevice = nil;
        return false;        
    }
    
    if ( !g_buttonListener ) g_buttonListener = [[ ButtonListener alloc ] init];
    
    //Add the video device to the session as a device input
        
    mCaptureVideoDeviceInput = [[QTCaptureDeviceInput alloc] initWithDevice:videoDevice];
    success = [mCaptureSession addInput:mCaptureVideoDeviceInput error:&error];
    if (!success) {
        // Handle error
        return false;
    }
    
    mCaptureOutput = [[QTCaptureDecompressedVideoOutput alloc] init];
    [mCaptureOutput setDelegate:g_buttonListener ];
    
    // Associate the capture view in the UI with the session
    
    mCaptureViewParent = [[NSView alloc] initWithFrame:NSMakeRect(0, 0, agk::m_iRealDeviceWidth, m_iRealDeviceHeight) ];
    
    mCaptureView = [[QTCaptureView alloc] initWithFrame:NSMakeRect(0, 50, agk::m_iRealDeviceWidth, m_iRealDeviceHeight-50) ];
    
    mCaptureButton = [[NSButton alloc] initWithFrame:NSMakeRect(m_iRealDeviceWidth/2 - 160, 10, 150, 30) ];
    mCaptureCancelButton = [[NSButton alloc] initWithFrame:NSMakeRect(m_iRealDeviceWidth/2 + 10, 10, 150, 30) ];
    
    [ mCaptureButton setTitle:@"Capture Image" ];
    [ mCaptureCancelButton setTitle:@"Cancel" ];
    
    [ mCaptureButton setTarget: g_buttonListener ];
    [ mCaptureButton setAction:@selector(capturePressed) ];
    
    [ mCaptureCancelButton setTarget: g_buttonListener ];
    [ mCaptureCancelButton setAction:@selector(cancelPressed) ];
    
    [mCaptureView setCaptureSession:mCaptureSession];
    
    [mCaptureViewParent addSubview:mCaptureView ];
    [mCaptureViewParent addSubview:mCaptureButton ];
    [mCaptureViewParent addSubview:mCaptureCancelButton ];
    
    mMainView = [[[NSApplication sharedApplication] mainWindow] contentView ];
    [[[NSApplication sharedApplication] mainWindow] setContentView:mCaptureViewParent ];
    
    [mCaptureSession startRunning];
     */
        
    return true;
}

void agk::PlatformHideCaptureScreen()
{
    //capture
    /*
    if ( mCaptureViewParent )
    {
        [[[NSApplication sharedApplication] mainWindow] setContentView:mMainView ];
        
        [ mCaptureSession stopRunning ];
        [ mCaptureSession release ];
        mCaptureSession = 0;
        
        [mCaptureVideoDeviceInput release];
        mCaptureVideoDeviceInput = 0;
        
        [mCaptureOutput release];
        mCaptureOutput = 0;
        
        [ mCaptureViewParent release ];
        mCaptureViewParent = 0;
    
        [ mCaptureView release ];
        mCaptureView = 0;
        
        [ mCaptureButton release ];
        mCaptureButton = 0;
        
        [ mCaptureCancelButton release ];
        mCaptureCancelButton = 0;
    }
    
    if ( m_iUncollectedCaptureImage ) 
    {
        agk::DeleteImage( m_iUncollectedCaptureImage );
        m_iUncollectedCaptureImage = 0;
    }
    
    m_pCaptureImage = 0;
     */
}

bool cImage::CaptureFromCamera()
{
	// do nothing on Mac
    return true;
}

int agk::GetNumDeviceCameras()
//****
{
	return 0;
}

int agk::SetDeviceCameraToImage( UINT cameraID, UINT imageID )
{
	return 0;
}

void agk::DeviceCameraUpdate()
{
	
}

void agk::DeviceCameraResumed()
{
	
}

int agk::GetDeviceCameraType( UINT cameraID )
//****
{
	return 0;
}

void cImage::Print( float size )
{
    if ( size < 0 ) size = 0;
	if ( size > 100 ) size = 100;
	size = size * 0.0095f;
    
   // get pixels from OpenGL
	UINT* bits = 0;
	GetRawData( (unsigned char**) &bits );
	if ( !bits )
	{
		agk::Warning("Failed to get image data");
		return;
	}
    
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, bits, (m_iOrigWidth*m_iOrigHeight*4), NULL);
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault | kCGImageAlphaLast;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    
    CGImageRef iOSImage = CGImageCreate(m_iOrigWidth, m_iOrigHeight, 8, 32, m_iOrigWidth*4, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
    NSImage *nsImage = [[NSImage alloc] initWithCGImage:iOSImage size:NSZeroSize ];
    
	NSPrintInfo *info = [NSPrintInfo sharedPrintInfo];
    [info setHorizontalPagination:NSClipPagination];
    [info setVerticalPagination:NSClipPagination];
    [info setHorizontallyCentered:NO];
    [info setVerticallyCentered:NO];
    
    float aspect = m_iOrigHeight / (float)m_iOrigWidth;
    if ( aspect < 1 )
    {
        [info setOrientation:NSPaperOrientationLandscape ];
    }
    else 
    {
        [info setOrientation:NSPaperOrientationPortrait ];
    }
        
    int sizeX = [info paperSize].width;
	int sizeY = [info paperSize].height;
	int borderX = agk::Round( sizeX * 0.025f );
	int borderY = agk::Round( sizeY * 0.025f );
	if ( borderY < borderX ) borderX = borderY;
	else borderY = borderX;
    
    [info setLeftMargin:borderX ];
    [info setTopMargin:borderY ];
    [info setRightMargin:borderX ];
    [info setBottomMargin:borderY ];
    
    float printerAspect = sizeY / (float)sizeX;
    if ( printerAspect >= aspect )
    {
        sizeX = agk::Round( sizeX * size );
        sizeY = agk::Round( sizeX * aspect );
    }
    else
    {
        sizeY = agk::Round( sizeY * size );
        sizeX = agk::Round( sizeY / aspect );
    }
    
    NSSize pagesize = [info paperSize];
    NSImageView *view = [[NSImageView alloc] initWithFrame:NSMakeRect(0,0,sizeX,sizeY) ];
    [view setImage:nsImage];
    [view setImageScaling:NSScaleToFit];
    
    NSPrintOperation *printOp =
    [NSPrintOperation printOperationWithView:view printInfo:info];
    [printOp setShowsPrintPanel:YES];
    [printOp runOperation];
    
    [nsImage release];
    [view release ];
    delete [] bits;
}

bool cImage::PlatformGetDataFromFile( const char* szFile, unsigned char **pData, unsigned int *out_width, unsigned int *out_height )
{
    uString sPath( szFile );
    
    if ( !agk::GetRealPath( sPath ) ) 
	{
		uString err; err.Format( "Could not find image: %s", szFile );
		agk::Error( err );
		return false;
	}
	
    int width;
    int height;
    bool hasAlpha;
    
    bool result = false;
    const char *szExt = strrchr( szFile, '.' );
    char *szExtL = agk::Lower( szExt );
    
    bool bIsPNG = false;
    bool bIsJPG = false;
    
    if ( strcmp( szExtL, ".png" ) == 0 ) bIsPNG = true;
    else bIsJPG = true;
    
    delete [] szExtL;
    
    if ( bIsPNG ) result = loadPngImage( sPath.GetStr(), width, height, hasAlpha, pData );
    else if ( bIsJPG ) result = loadJpegImage( sPath.GetStr(), width, height, hasAlpha, pData );
    
    if ( !result )
    {
        uString str( "Failed to load raw image ", 100 ); str.Append( szFile );
        str.Append( ", " ); str.Append( sPath.GetStr() );
        agk::Error( str );
        return false;
    }
    
    if ( out_width ) *out_width = width;
    if ( out_height ) *out_height = height;
    
    if ( !hasAlpha )
	{
		// add alpha channel
		unsigned char *tempData = new unsigned char[ width * height * 4 ];
		for ( int y = 0; y < height; y++ )
		{
			unsigned int index = y*width;

			for ( int x = 0; x < width; x++ )
			{
				tempData[ index*4 ] = (*pData)[ index*3 ];
				tempData[ index*4 + 1 ] = (*pData)[ index*3 + 1 ];
				tempData[ index*4 + 2 ] = (*pData)[ index*3 + 2 ];
				tempData[ index*4 + 3 ] = 255;
				index++;
			}
		}

		delete [] (*pData);
		*pData = tempData;
	}
    
    return true;
}

void agk::VibrateDevice( float seconds )
//****
{
	// do nothing
}

void agk::SetClipboardText( const char* szText )
//****
{
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    [pasteboard clearContents];
    [pasteboard setString:[NSString stringWithUTF8String:szText] forType:NSStringPboardType];
}

char* agk::GetClipboardText()
//****
{
    NSPasteboard *pasteboard = [NSPasteboard generalPasteboard];
    if ( pasteboard )
    {
        NSString *string = [pasteboard stringForType:NSStringPboardType];
        if ( string )
        {
            const char *text = [string UTF8String];
            if ( text && *text )
            {
                char *str = new char[ strlen(text) + 1 ];
                strcpy( str, text );
                return str;
            }
        }
    }
    
    char *str = new char[1]; *str = 0;
    return str;
}

// Music

void cMusicMgr::PlatformAddFile( cMusic *pMusic )
{
/*
	uString sFilename = pMusic->m_sFile;
	sFilename.Lower();
	if ( sFilename.FindStr(".mp3") && !cFile::Exists( pMusic->m_sFile ) )
	{
		pMusic->m_sFile.Trunc( '.' );
		pMusic->m_sFile.Append( ".m4a" );
	}
*/
}

bool cMusicMgr::PlatformPrepare( UINT iID )
{
    cMusic *pMusic = m_pMusicFiles[ iID ];
	
	m_pCurrentlyPlaying = UNDEF;
	
    //music
	if ( musicPlayer )
	{
		[ musicPlayer stop ];
		[ musicPlayer release ];
		musicPlayer = UNDEF;
	}
    
    NSString* pString = [ [ NSString alloc ] initWithUTF8String: pMusic->m_sFile.GetStr() ];
    NSURL *fileURL = [[NSURL alloc] initFileURLWithPath: pString];
	NSError *error = nil;
	musicPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:fileURL error:&error];
	[pString release];
    [fileURL release];
	
	if ( error )
	{
		uString err( "Failed to load music file ", 50 );
		err.Append( pMusic->m_sFile );
		agk::Error( err );
		return false;
	}
    
    musicPlayer.delegate = g_pAGKMusicListener;
    
    return true;
}

void cMusicMgr::Play( UINT iID, bool bLoop, UINT iStartID, UINT iEndID )
{
	if ( iStartID < 1 ) iStartID = 1;
	if ( iStartID >= MAX_MUSIC_FILES ) iStartID = MAX_MUSIC_FILES - 1;
	if ( iEndID < iStartID ) iEndID = iStartID;
	if ( iEndID >= MAX_MUSIC_FILES ) iEndID = MAX_MUSIC_FILES - 1;
	
	m_iStartID = iStartID;
	m_iEndID = iEndID;
	m_bLoop = bLoop;
	
	if ( iID < 1 || iID >= MAX_MUSIC_FILES ) 
	{
#ifdef _AGK_ERROR_CHECK
		uString str( "Could not play music file ", 100 ); 
		str.AppendUInt( iID ).Append( " - ID must be between 1 and " ).AppendInt( MAX_MUSIC_FILES-1 );
		agk::Error( str );
#endif
		return;
	}
	
	if ( !m_pMusicFiles[ iID ] )
	{
#ifdef _AGK_ERROR_CHECK
		uString str( "Could not play music file ", 100 );
		str.AppendUInt( iID ).Append(" - ID does not exist" );
		agk::Error( str );
#endif
		return;
	}
    
    if ( !g_pAGKMusicListener ) g_pAGKMusicListener = [[AGKMusicListener alloc] init];
	
    if ( !PlatformPrepare( iID ) ) return;
    
    cMusic *pMusic = m_pMusicFiles[ iID ];
    
    m_pCurrentlyPlaying = pMusic;
	
	[ musicPlayer setNumberOfLoops:0 ];
	musicPlayer.volume = (pMusic->m_iVolume * m_iMasterVolume) / 10000.0f;
	[ musicPlayer play ];
}

void cMusicMgr::Pause()
{
	if ( musicPlayer ) [ musicPlayer pause ];
}

void cMusicMgr::Resume()
{
	if ( musicPlayer ) [ musicPlayer play ];
}

void cMusicMgr::Stop()
{
	if ( musicPlayer ) [ musicPlayer stop ];
	m_pCurrentlyPlaying = UNDEF;
	
    /*
    if ( musicPlayer )
	{
		if ( [musicPlayer rate] != 0 ) 
		{
			[ musicPlayer stop ];
		}
		[ musicPlayer release ];
		musicPlayer = UNDEF;
	}
     */
}

void cMusicMgr::SetMasterVolume( int vol )
{
	if ( vol > 100 ) vol = 100;
	if ( vol < 0 ) vol = 0;
	m_iMasterVolume = vol;
    
    if ( musicPlayer )
	{
		if ( m_pCurrentlyPlaying ) musicPlayer.volume = m_iMasterVolume / 100.0f;
		else musicPlayer.volume = m_iMasterVolume / 100.0f;
	}
}

float cMusicMgr::GetDuration( UINT ID )
{
    if ( !musicPlayer ) return 0;
    return musicPlayer.duration;
}

void cMusicMgr::Seek( float seconds, int mode )
{
    if ( !musicPlayer ) return;
    
    float newTime = seconds;
    if ( mode == 1 ) newTime += musicPlayer.currentTime;
    if ( newTime < 0 ) newTime = 0;
    
    [ musicPlayer setCurrentTime:newTime ];
}

float cMusicMgr::GetPosition()
{
    if ( !musicPlayer ) return 0;
    return musicPlayer.currentTime;
}

void cMusicMgr::HandleEvent()
{
	if ( m_pCurrentlyPlaying == UNDEF ) return;
	if ( !musicPlayer ) return;
		
	if ( ![musicPlayer isPlaying] )
	{
		UINT iCurrID = m_pCurrentlyPlaying->m_iID;
		UINT iNextID = 0;
		
		if ( iCurrID >= m_iEndID )
		{
			if ( m_bLoop ) 
			{
				for ( UINT i = m_iStartID; i <= m_iEndID; i++ )
				{
					if ( m_pMusicFiles[ i ] ) 
					{
						iNextID = i;
						break;
					}
				}
			}
		}
		else
		{
			for ( UINT i = iCurrID+1; i <= m_iEndID; i++ )
			{
				if ( m_pMusicFiles[ i ] ) 
				{
					iNextID = i;
					break;
				}
			}
			
			if ( iNextID == 0 && m_bLoop ) 
			{
				for ( UINT i = m_iStartID; i <= iCurrID; i++ )
				{
					if ( m_pMusicFiles[ i ] ) 
					{
						iNextID = i;
						break;
					}
				}
			}
		}
		
		if ( iNextID > 0 ) Play( iNextID, m_bLoop, m_iStartID, m_iEndID );
		else m_pCurrentlyPlaying = UNDEF;
	}
}

// OGG Music

struct AGKOGGSoundData
{
    ALuint bufferID[ 3 ];
    ALuint sourceID;
    ALenum format;
    
    unsigned int m_iCurrBuffer;
    int m_iResetFlag[ 3 ];
};

void AGKMusicOGG::PlatformInit()
{
    AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;
    
    if ( !m_pSoundData )
    {
        m_pSoundData = (void*) new AGKOGGSoundData();
        pTemp = (AGKOGGSoundData*)m_pSoundData;
        
        pTemp->m_iCurrBuffer = 0;
        pTemp->m_iResetFlag[ 0 ] = 0;
        pTemp->m_iResetFlag[ 1 ] = 0;
        pTemp->m_iResetFlag[ 2 ] = 0;
    }
    
    if ( m_fmt.nChannels == 2 ) pTemp->format = AL_FORMAT_STEREO16;
    else if ( m_fmt.nChannels == 1 ) pTemp->format = AL_FORMAT_MONO16;
    else
    {
        agk::Error( "Unsupported ogg file number of channels, only 1 (mono) and 2 (stereo) channels supported." );
        return;
    }
    
    alGenBuffers(3, pTemp->bufferID);
    
    alGenSources(1, &(pTemp->sourceID));
    alSourcei(pTemp->sourceID, AL_LOOPING, AL_FALSE);
    alSourcei(pTemp->sourceID, AL_SOURCE_RELATIVE, AL_TRUE);
    
    float fVol = m_iVolume / 100.0f;
    alSourcef(pTemp->sourceID, AL_GAIN, fVol);
}

void AGKMusicOGG::PlatformCleanUp()
{
    if ( m_pSoundData )
    {
        AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;
        
        if( pTemp->sourceID )
        {
            alSourceStop(pTemp->sourceID);
            alDeleteSources(1, &pTemp->sourceID);
        }
        if ( pTemp->bufferID[0] ) alDeleteBuffers( 1, &pTemp->bufferID[0] );
        if ( pTemp->bufferID[1] ) alDeleteBuffers( 1, &pTemp->bufferID[1] );
        if ( pTemp->bufferID[2] ) alDeleteBuffers( 1, &pTemp->bufferID[2] );
        
        delete pTemp;
        m_pSoundData = 0;
    }
}

int AGKMusicOGG::PlatformPlay()
{
    if ( !m_pSoundData ) return 0;
    AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;
    
    alSourcePlay(pTemp->sourceID);
    
    return 1;
}

void AGKMusicOGG::PlatformSetVolume()
{
    if ( !m_pSoundData ) return;
    AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;
    
    float fVol = (m_iVolume * g_iMasterVolume) / 10000.0f;
    alSourcef(pTemp->sourceID, AL_GAIN, fVol);
}

void AGKMusicOGG::PlatformPause()
{
    if ( !m_pSoundData ) return;
    AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;
    
    alSourcePause(pTemp->sourceID);
}

void AGKMusicOGG::PlatformResume()
{
    if ( !m_pSoundData ) return;
    AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;
    
    alSourcePlay(pTemp->sourceID);
}

void AGKMusicOGG::PlatformStop()
{
    if ( !m_pSoundData ) return;
    AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;
    
    alSourceStop( pTemp->sourceID );
    alSourcei( pTemp->sourceID, AL_BUFFER, 0 );
    
    pTemp->m_iResetFlag[ 0 ] = 0;
    pTemp->m_iResetFlag[ 1 ] = 0;
    pTemp->m_iResetFlag[ 2 ] = 0;
}

void AGKMusicOGG::PlatformClearBuffers()
{
    if ( !m_pSoundData ) return;
    AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;
    
    alSourceStop( pTemp->sourceID );
    alSourcei( pTemp->sourceID, AL_BUFFER, 0 );
    
    pTemp->m_iResetFlag[ 0 ] = 0;
    pTemp->m_iResetFlag[ 1 ] = 0;
    pTemp->m_iResetFlag[ 2 ] = 0;
}

void AGKMusicOGG::PlatformReset()
{
    if ( !m_pSoundData ) return;
    AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;
    
    if( pTemp->sourceID )
    {
        alSourceStop(pTemp->sourceID);
        alDeleteSources(1, &pTemp->sourceID);
    }
    if ( pTemp->bufferID[0] ) alDeleteBuffers( 1, &pTemp->bufferID[0] );
    if ( pTemp->bufferID[1] ) alDeleteBuffers( 1, &pTemp->bufferID[1] );
    if ( pTemp->bufferID[2] ) alDeleteBuffers( 1, &pTemp->bufferID[2] );
    
    pTemp->sourceID = 0;
    pTemp->bufferID[0] = 0;
    pTemp->bufferID[1] = 0;
    pTemp->bufferID[2] = 0;
    
    pTemp->m_iResetFlag[ 0 ] = 0;
    pTemp->m_iResetFlag[ 1 ] = 0;
    pTemp->m_iResetFlag[ 2 ] = 0;
}

int AGKMusicOGG::PlatformGetNumBuffers()
{
    if ( !m_pSoundData ) return 0;
    AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;
    
    int processed = 0;
    alGetSourcei( pTemp->sourceID, AL_BUFFERS_PROCESSED, &processed);
    
    int queued = 0;
    alGetSourcei( pTemp->sourceID, AL_BUFFERS_QUEUED, &queued);
    
    //NSLog( @"Queued: %d, Processed: %d, Curr: %d", queued, processed, pTemp->m_iCurrBuffer );
    
    return queued - processed;
}

int AGKMusicOGG::PlatformGetMaxBuffers() { return 3; }

int AGKMusicOGG::PlatformAddBuffer( int *reset )
{
    if ( !m_pSoundData ) return 0;
    AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;
    
    // get rid of used buffers
    int processed = 0;
    alGetSourcei( pTemp->sourceID, AL_BUFFERS_PROCESSED, &processed);
    
    while( processed > 0 )
    {
        processed--;
        ALuint buf;
        alSourceUnqueueBuffers( pTemp->sourceID, 1, &buf );
    }
    
    // add new buffer
    int currBuf = pTemp->m_iCurrBuffer;
    if ( reset ) *reset = pTemp->m_iResetFlag[ currBuf ];
    pTemp->m_iResetFlag[ currBuf ] = (m_iFlags & AGK_MUSIC_OGG_BUFFER_END) ? 1 : 0;
    
   	pTemp->m_iCurrBuffer++;
    if ( pTemp->m_iCurrBuffer >= PlatformGetMaxBuffers() ) pTemp->m_iCurrBuffer = 0;
    
    alBufferData( pTemp->bufferID[currBuf], pTemp->format, m_pDecodeBuffer, m_iBufferSize, m_fmt.nSamplesPerSec);
    alSourceQueueBuffers( pTemp->sourceID, 1, &(pTemp->bufferID[currBuf]) );
    
    return 1;
}

// Sound

void cSoundMgr::PlatformInit()
{	
	if ( !audioDevice )
	{
		audioDevice = alcOpenDevice(NULL); // select the "preferred device"
		if(!audioDevice) agk::Error( "Failed to setup audio device" );
		else 
		{
			audioContext = alcCreateContext(audioDevice, NULL);
			if(!audioContext) agk::Error( "" );
			alcMakeContextCurrent(audioContext);
		}
	}
    
    m_fMinPlaybackRate = 0.5f;
    m_fMaxPlaybackRate = 2.0f;
    m_fStepPlaybackRate = 0.01f; //unknown
}

void cSoundMgr::AppPaused()
{
    
}

void cSoundMgr::AppResumed()
{
    
}

void cSoundMgr::PlatformAddFile( cSoundFile *pSound )
{
	// no further processing needed on Mac
	return;
}

void cSoundMgr::PlatformUpdate()
{
	int buffers = 0;
	cSoundInst *pSound = m_pSounds;
	cSoundInst *pNext = UNDEF;
	while ( pSound )
	{
		pNext = pSound->m_pNextInst;
		
		if ( pSound->sourceID )
		{
			int state = 0;
			alGetSourcei( pSound->sourceID, AL_SOURCE_STATE, &state );

			//alGetSourcei(pSound->sourceID, AL_SOURCE_STATE, &state);
			alGetSourcei(pSound->sourceID, AL_BUFFERS_PROCESSED, &buffers);
			if ( buffers > 0 )
			{
				while( buffers > 0 )
				{
					buffers--;
					ALuint buf;
					alSourceUnqueueBuffers( pSound->sourceID, 1, &buf );
					pSound->m_iLoopCount++;
				}
				
				if ( pSound->m_iLoop == 1 || pSound->m_iLoopCount+1 < pSound->m_iLoop )
				{
					alSourceQueueBuffers(pSound->sourceID, 1, &(pSound->bufferID));
					if ( state != AL_PLAYING ) alSourcePlay(pSound->sourceID);
					state = AL_PLAYING;
				}
			}

			if ( state != AL_PLAYING )
            {
                pSound->m_iLoopCount++;
                
                if ( m_pSoundFiles[ pSound->m_iParent ] ) m_pSoundFiles[ pSound->m_iParent ]->m_iInstances--;
                // sound is finished, cleanup and move to used list
                pSound->Reset();
                pSound->m_uLastUsed = agk::GetSeconds();
                if ( pSound->m_pPrevInst ) pSound->m_pPrevInst->m_pNextInst = pSound->m_pNextInst;
                else m_pSounds = pSound->m_pNextInst;
                
                if ( pSound->m_pNextInst ) pSound->m_pNextInst->m_pPrevInst = pSound->m_pPrevInst;
                
                m_cSoundInstances.RemoveItem( pSound->m_iID );
                
                // add to head of use list
                pSound->m_pPrevInst = UNDEF;
                pSound->m_pNextInst = m_pUsedSounds;
                m_pUsedSounds = pSound;
                
                if ( pSound->m_pNextInst ) pSound->m_pNextInst->m_pPrevInst = pSound;
            }
		}
		
		pSound = pNext;
	}
	
	int iSeconds = agk::GetSeconds();
	
	// check for used sounds that haven't been used in a while and delete them
	pSound = m_pUsedSounds;
	while ( pSound )
	{
		pNext = pSound->m_pNextInst;
		
		// delete after 10 seconds on the used list
		if ( iSeconds - pSound->m_uLastUsed > 10 )
		{
			if ( pSound->m_pPrevInst ) pSound->m_pPrevInst->m_pNextInst = pSound->m_pNextInst;
			else m_pUsedSounds = pSound->m_pNextInst;
			
			if ( pSound->m_pNextInst ) pSound->m_pNextInst->m_pPrevInst = pSound->m_pPrevInst;
			delete pSound;
		}
		
		pSound = pNext;
	}
}

void cSoundMgr::PlatformUpdateVolume()
{
	cSoundInst *pSound = m_pSounds;
	while ( pSound )
	{
		float fVol = m_iGlobalVolume / 100.0f;
		fVol *= ( pSound->m_iVolume / 100.0f );
		if ( pSound->sourceID ) alSourcef(pSound->sourceID, AL_GAIN, fVol);
		
		pSound = pSound->m_pNextInst;
	}
}

void cSoundMgr::PlatformCleanUp()
{
    m_cSoundInstances.ClearAll();

	cSoundInst *pSound;
	while ( m_pSounds )
	{
		pSound = m_pSounds;
		m_pSounds = m_pSounds->m_pNextInst;
		delete pSound;
	}
	
	while ( m_pUsedSounds )
	{
		pSound = m_pUsedSounds;
		m_pUsedSounds = m_pUsedSounds->m_pNextInst;
		delete pSound;
	}

	if ( audioContext ) alcDestroyContext( audioContext );
	if ( audioDevice ) alcCloseDevice( audioDevice );

	audioContext = 0;
	audioDevice = 0;
}

UINT cSoundMgr::PlatformCreateInstance( cSoundMgr::cSoundFile *pSound, int iVol, int iLoop, int iPriority )
{
	if ( !pSound ) return 0;
	if ( iVol < 0 ) iVol = 0;
	if ( iVol > 100 ) iVol = 100;
	
	cSoundInst *pSoundInst = UNDEF;
	// find any used instances
	if ( m_pUsedSounds )
	{
		pSoundInst = m_pUsedSounds;
		m_pUsedSounds = m_pUsedSounds->m_pNextInst;
		if ( m_pUsedSounds ) m_pUsedSounds->m_pPrevInst = UNDEF;
	}
    
    if ( iLoop < 0 ) iLoop = 0;
	
	// if no used sounds awaiting re-use, create a new one
	if ( !pSoundInst ) pSoundInst = new cSoundInst();
	pSoundInst->m_iID = m_cSoundInstances.GetFreeID();
	pSoundInst->m_iParent = pSound->m_iID;
	pSoundInst->m_iVolume = iVol;
	pSoundInst->m_iLoop = iLoop;
	pSoundInst->m_iLoopCount = 0;
	
	// add it to the running list
	pSoundInst->m_pPrevInst = UNDEF;
	pSoundInst->m_pNextInst = m_pSounds;
	m_pSounds = pSoundInst;
	if ( pSoundInst->m_pNextInst ) pSoundInst->m_pNextInst->m_pPrevInst = pSoundInst;
	
	ALenum format;
	if ( pSound->m_fmt.nChannels == 2 )
	{
		if ( pSound->m_fmt.wBitsPerSample == 8 ) format = AL_FORMAT_STEREO8;
		else if ( pSound->m_fmt.wBitsPerSample == 16 ) format = AL_FORMAT_STEREO16;
		else 
		{
            uString err;
            err.Format( "Unsupported wav file bit size, only 8bit and 16bit supported. File: %s", pSound->m_sFile.GetStr() );
			agk::Error( err );
			return 0;
		}
	}
	else if ( pSound->m_fmt.nChannels == 1 )
	{
		if ( pSound->m_fmt.wBitsPerSample == 8 ) format = AL_FORMAT_MONO8;
		else if ( pSound->m_fmt.wBitsPerSample == 16 ) format = AL_FORMAT_MONO16;
		else 
		{
			uString err;
            err.Format( "Unsupported wav file bit size, only 8bit and 16bit supported. File: %s", pSound->m_sFile.GetStr() );
			agk::Error( err );
			return 0;
		}
	}
	else
	{
		agk::Error( "Unsupported wav file number of channels, only 1 (mono) and 2 (stereo) channels supported." );
		return 0;
	}
    
    m_cSoundInstances.AddItem( pSoundInst, pSoundInst->m_iID );
	
	// setup mac specific variables
	alGenBuffers(1, &(pSoundInst->bufferID));
    alBufferData(pSoundInst->bufferID,  format, pSound->m_pRawData, pSound->m_uDataSize, pSound->m_fmt.nSamplesPerSec);
	alGenSources(1, &(pSoundInst->sourceID));
	alSourcei(pSoundInst->sourceID, AL_LOOPING, AL_FALSE);
    alSourcei(pSoundInst->sourceID, AL_SOURCE_RELATIVE, AL_TRUE);
    
    if ( pSoundInst->m_iLoop == 0 )
    {
        alSourcei(pSoundInst->sourceID, AL_BUFFER, pSoundInst->bufferID);
    }
    else
    {
        alSourceQueueBuffers( pSoundInst->sourceID, 1, &(pSoundInst->bufferID) );
        alSourceQueueBuffers( pSoundInst->sourceID, 1, &(pSoundInst->bufferID) );
    }

    
	float fVol = m_iGlobalVolume / 100.0f;
	fVol *= ( pSoundInst->m_iVolume / 100.0f );
	alSourcef(pSoundInst->sourceID, AL_GAIN, fVol);
	
	alSourcePlay(pSoundInst->sourceID);
	pSound->m_iInstances++;
    
    return pSoundInst->m_iID;
}

void cSoundMgr::PlatformStopInstances( UINT iID )
{
	cSoundInst *pNext = UNDEF;
	cSoundInst *pSound = m_pSounds;
	while ( pSound )
	{
		pNext = pSound->m_pNextInst;
		
		if ( iID == 0 || pSound->m_iParent == iID )
		{
			if ( m_pSoundFiles[ pSound->m_iParent ] ) m_pSoundFiles[ pSound->m_iParent ]->m_iInstances = 0;
		
			// stop sound, remove from playing list
			pSound->Reset();
			pSound->m_uLastUsed = agk::GetSeconds();
			if ( pSound->m_pPrevInst ) pSound->m_pPrevInst->m_pNextInst = pSound->m_pNextInst;
			else m_pSounds = pSound->m_pNextInst;
            
            m_cSoundInstances.RemoveItem( pSound->m_iID );
			
			if ( pSound->m_pNextInst ) pSound->m_pNextInst->m_pPrevInst = pSound->m_pPrevInst;
			
			// add to head of use list
			pSound->m_pPrevInst = UNDEF;
			pSound->m_pNextInst = m_pUsedSounds;
			m_pUsedSounds = pSound;
			
			if ( pSound->m_pNextInst ) pSound->m_pNextInst->m_pPrevInst = pSound;
		}
		
		pSound = pNext;
	}
}

void cSoundMgr::SetInstanceVolume( UINT instance, int vol )
{
	cSoundInst *pSound = m_cSoundInstances.GetItem( instance );
	if ( !pSound ) return;
    
	if ( vol < 0 ) vol = 0;
	if ( vol > 100 ) vol = 100;
    
	pSound->m_iVolume = vol;
    
	float fVol = m_iGlobalVolume / 100.0f;
	fVol *= ( pSound->m_iVolume / 100.0f );
    
	alSourcef(pSound->sourceID, AL_GAIN, fVol);
}

void cSoundMgr::SetInstanceRate( UINT instance, float rate )
{
	cSoundInst *pSound = m_cSoundInstances.GetItem( instance );
	if ( !pSound ) return;
    
	if ( rate < m_fMinPlaybackRate ) rate = m_fMinPlaybackRate;
	if ( rate > m_fMaxPlaybackRate ) rate = m_fMaxPlaybackRate;
    
	pSound->m_fRate = rate;
    
	alSourcef( pSound->sourceID, AL_PITCH, rate );
}

void cSoundMgr::SetInstanceBalance( UINT instance, float balance )
{
	cSoundInst *pSound = m_cSoundInstances.GetItem( instance );
	if ( !pSound ) return;
    
	if ( balance < -1 ) balance = -1;
	if ( balance > 1 ) balance = 1;
    
	pSound->m_fBalance = balance;
    
	float sourcePosAL[] = {pSound->m_fBalance, 0.0f, 0.0f}; 
	alSourcefv( pSound->sourceID, AL_POSITION, sourcePosAL ); 
}

int cSoundMgr::GetInstanceVolume( UINT instance )
{
	cSoundInst *pSound = m_cSoundInstances.GetItem( instance );
	if ( !pSound ) return 0;
	return pSound->m_iVolume;
}

float cSoundMgr::GetInstanceRate( UINT instance )
{
	cSoundInst *pSound = m_cSoundInstances.GetItem( instance );
	if ( !pSound ) return 0;
	return pSound->m_fRate;
}

int cSoundMgr::GetInstancePlaying( UINT instance )
{
	cSoundInst *pSound = m_cSoundInstances.GetItem( instance );
	if ( !pSound ) return 0;
	else return 1;
}

int cSoundMgr::GetInstanceLoopCount( UINT instance )
{
	cSoundInst *pSound = m_cSoundInstances.GetItem( instance );
	if ( !pSound ) return 0;
	return pSound->m_iLoopCount;
}

void cSoundMgr::StopInstance( UINT instance )
{
	cSoundInst *pSound = m_cSoundInstances.GetItem( instance );
	if ( !pSound ) return;
    
    if ( m_pSoundFiles[ pSound->m_iParent ] ) m_pSoundFiles[ pSound->m_iParent ]->m_iInstances--;
    
	// stop sound, remove from playing list
	pSound->Reset();
	pSound->m_uLastUsed = agk::GetSeconds();
	if ( pSound->m_pPrevInst ) pSound->m_pPrevInst->m_pNextInst = pSound->m_pNextInst;
	else m_pSounds = pSound->m_pNextInst;
    
	m_cSoundInstances.RemoveItem( pSound->m_iID );
	
	if ( pSound->m_pNextInst ) pSound->m_pNextInst->m_pPrevInst = pSound->m_pPrevInst;
	
	// add to head of use list
	pSound->m_pPrevInst = UNDEF;
	pSound->m_pNextInst = m_pUsedSounds;
	m_pUsedSounds = pSound;
	
	if ( pSound->m_pNextInst ) pSound->m_pNextInst->m_pPrevInst = pSound;
}

// youtube videos

void agk::PlayYoutubeVideo( const char* developerKey, const char* videoID, float startTime )
//****
{
	uString sURL;
	sURL.Format( "https://www.youtube.com/watch?v=%s&t=%d", videoID, (int)startTime );
	OpenBrowser( sURL );
}


// video commands

float g_fVideoDuration = 0;

int agk::LoadVideo( const char *szFilename )
//****
{
    g_fVideoDuration = 0;
    
    if ( !g_pAGKMovieListener ) g_pAGKMovieListener = [[AGKMovieListener alloc] init];
    
    if ( g_videoPlayer )
    {
        DeleteVideo();
    }
    
    g_videoPlayer = [[AVPlayer alloc] init];
    [[NSNotificationCenter defaultCenter] addObserver:g_pAGKMovieListener selector:@selector(playbackFinished:) name:AVPlayerItemDidPlayToEndTimeNotification object:g_videoPlayer];
        
    g_videoLayer = [[AVPlayerLayer playerLayerWithPlayer:g_videoPlayer] retain];
    
    if ( g_videoAsset ) [ g_videoAsset release ];
    if ( g_videoItem ) [ g_videoItem release ];
    
    g_videoItem = 0;
    g_videoAsset = 0;
    
    m_fVideoX = 0;
	m_fVideoY = 0;
	m_fVideoWidth = (float) agk::GetVirtualWidth();
	m_fVideoHeight = (float) agk::GetVirtualHeight();
    
    int iX = agk::ScreenToDeviceX( m_fVideoX );
    int iY = agk::ScreenToDeviceY( m_fVideoY );
    int iX2 = agk::ScreenToDeviceX( m_fVideoX+m_fVideoWidth );
    int iY2 = agk::ScreenToDeviceY( m_fVideoY+m_fVideoHeight );
    
    if ( !g_videoView )
    {
        g_videoView = [[AGKMovieView alloc] initWithFrame:NSMakeRect(0,0, GetDeviceWidth(), GetDeviceHeight()) ]; 
    }
    
    [ g_videoView setFrame:NSMakeRect(iX,GetDeviceHeight()-iY2,iX2-iX,iY2-iY)];
    
    [g_videoView setWantsLayer:YES];
    [g_videoView setLayer:g_videoLayer];
    
    //[ g_videoLayer setFrame:CGRectMake(iX,GetDeviceHeight()-iY2,iX2-iX,iY2-iY)];
    //[ g_videoLayer setFrame:CGRectMake(0,0,iX2-iX,iY2-iY)];
    
    g_videoLayer.autoresizingMask = kCALayerWidthSizable | kCALayerHeightSizable;
    //g_videoLayer.hidden = YES;
    [g_videoLayer setVideoGravity:AVLayerVideoGravityResize];
    
    
    g_sVideoFile.SetStr(szFilename);
    
    uString sPath( g_sVideoFile );
    if ( !GetRealPath( sPath ) )
	{
		agk::Error( "Failed to load video file - file not found" );
		return 0;
	}
    
    NSString* pString = [ [ NSString alloc ] initWithUTF8String: sPath ];
    
    NSURL *url = [NSURL fileURLWithPath: pString];
    g_videoAsset = [[AVURLAsset URLAssetWithURL:url options:nil] retain];
    NSArray *assetKeysToLoadAndTest = [NSArray arrayWithObjects:@"playable", @"tracks", @"duration", nil];
    [g_videoAsset loadValuesAsynchronouslyForKeys:assetKeysToLoadAndTest completionHandler:^(void) {
        if (!g_videoAsset.isPlayable || g_videoAsset.hasProtectedContent) {
            return;
        }
        
        // Create a new AVPlayerItem and make it our player's current item.
        g_videoItem = [[AVPlayerItem playerItemWithAsset:g_videoAsset] retain];
        [g_videoPlayer replaceCurrentItemWithPlayerItem:g_videoItem];
    }];
    
    /*
    g_video = [[QTMovie alloc] initWithFile: pString error:nil];
    [ g_video.movieAttributes setValue:[NSNumber numberWithBool:YES] forKey:@"QTMovieOpenForPlaybackAttribute"];
    
    [ g_videoView setMovie:g_video ];
    NSRect rect = NSMakeRect(0,0, GetDeviceWidth(), GetDeviceHeight());
    [ g_videoView setFrame:rect ];
    */
    
        
    //[[NSNotificationCenter defaultCenter] addObserver:g_pAGKMovieListener selector:@selector(playbackFinished:) name:QTMovieDidEndNotification object:g_video];
    
    [ pString release ];
    
    return 1;
}

void agk::HandleVideoEvents()
{
	// do nothing on mac
}

void agk::ChangeVideoPointer( void *ptr )
{
	
}

void agk::DeleteVideo()
//****
{
	StopVideo();
    
    if ( g_videoPlayer )
    {
        [[NSNotificationCenter defaultCenter] removeObserver:g_pAGKMovieListener name:AVPlayerItemDidPlayToEndTimeNotification object:g_videoPlayer];
        [ g_videoPlayer release ];
    }
    if ( g_videoAsset ) [ g_videoAsset release ];
    if ( g_videoLayer ) [ g_videoLayer release ];
    if ( g_videoItem ) [ g_videoItem release ];
    
    g_videoPlayer = 0;
    g_videoLayer = 0;
    g_videoAsset = 0;
    g_videoItem = 0;
}

void agk::SetVideoDimensions( float x, float y, float width, float height )
//****
{
    m_fVideoX = x;
	m_fVideoY = y;
	m_fVideoWidth = width;
	m_fVideoHeight = height;
    
    if ( !g_videoLayer ) return;
    
    int iX = agk::ScreenToDeviceX( x );
    int iY = agk::ScreenToDeviceY( y );
    int iX2 = agk::ScreenToDeviceX( x+width );
    int iY2 = agk::ScreenToDeviceY( y+height );
    
    //[ g_videoLayer setFrame:CGRectMake(iX,GetDeviceHeight()-iY2,iX2-iX,iY2-iY)];
    [ g_videoView setFrame:NSMakeRect(iX,GetDeviceHeight()-iY2,iX2-iX,iY2-iY)];
    //[ g_videoLayer setFrame:CGRectMake(0,0,iX2-iX,iY2-iY)];
}

void agk::VideoUpdate()
{
	// do nothing
}

void agk::PlayVideoToImage( UINT imageID )
//****
{
	agk::Message("Video to texture is not currently supported on Mac");
	
	//m_iVideoPlayMode = 2;
}

void agk::PlayVideo()
//****
{
    if ( !g_videoPlayer ) return;
    if ( !g_videoLayer ) return;
    
    NSWindow *window = [[[NSApplication sharedApplication] windows] objectAtIndex:0];
    [[window contentView] addSubview:g_videoView ];
    
    [ g_videoPlayer play ];
    
    m_iVideoPlayMode = 1;
}

void agk::PauseVideo()
//****
{
    if ( !g_videoPlayer ) return;
    
    [ g_videoPlayer pause ];
}

void agk::StopVideo()
//****
{
    if ( g_videoView )
    {
        [ g_videoView removeFromSuperview ];
    }
    if ( !g_videoPlayer ) return;
    
    [ g_videoPlayer pause ];
    [ g_videoPlayer seekToTime:kCMTimeZero ];
    
    m_iVideoPlayMode = 0;
}

int agk::GetVideoPlaying()
//****
{
    if ( !g_videoPlayer ) return 0;
    
    return g_videoPlayer.rate > 0 ? 1 : 0;
}

float agk::GetVideoPosition()
//****
{
    if ( !g_videoPlayer ) return 0;
    return g_videoPlayer.currentTime.value / (float) g_videoPlayer.currentTime.timescale;
}

float agk::GetVideoDuration()
//****
{
    if ( !g_videoItem ) return 0;
    if ( !g_videoAsset ) return 0;
    return g_videoAsset.duration.value / (float) g_videoAsset.duration.timescale;
}

void agk::SetVideoVolume( float volume )
//****
{
    // ?
}

float agk::GetVideoWidth()
//****
{
    if ( !g_videoItem ) return -1;
    if ( !g_videoAsset ) return -1;
    return g_videoAsset.naturalSize.width;
}

float agk::GetVideoHeight()
//****
{
    if ( !g_videoItem ) return -1;
    if ( !g_videoAsset ) return -1;
    return g_videoAsset.naturalSize.height;
}

void agk::SetVideoPosition( float seconds )
//****
{
    if ( !g_videoPlayer ) return;
    
    int32_t timeScale = g_videoPlayer.currentItem.asset.duration.timescale;
    [g_videoPlayer seekToTime:CMTimeMakeWithSeconds(seconds, timeScale) toleranceBefore:kCMTimeZero toleranceAfter:kCMTimeZero];
}

// Screen recording

void agk::StartScreenRecording( const char *szFilename, int microphone )
//****
{
	
}

void agk::StopScreenRecording()
//****
{

}

int agk::IsScreenRecording()
//****
{
	return 0;
}

// Smart Watch

void agk::ActivateSmartWatch( const char *szReserved )
//****
{
    
}

int agk::GetSmartWatchState()
//****
{
    return -1;
}

void agk::SendSmartWatchData( const char* szJson )
//****
{
    
}

char* agk::ReceiveSmartWatchData()
//****
{
    char* str = new char[1]; *str = 0;
    return str;
}

// Text to speech

void agk::TextToSpeechSetup()
//****
{
	
}

int agk::GetTextToSpeechReady()
//****
{
	return 0;
}

void agk::Speak( const char *text )
//****
{
	
}

void agk::Speak( const char *text, int delay )
//****
{
	
}

void agk::SetSpeechRate( float rate )
//****
{

}

int agk::GetSpeechNumVoices()
//****
{
    return 0;
}

char* agk::GetSpeechVoiceLanguage( int index )
//****
{
    char *str = new char[1]; *str = 0;
    return str;
}

char* agk::GetSpeechVoiceName( int index )
//****
{
    char *str = new char[1]; *str = 0;
    return str;
}

char* agk::GetSpeechVoiceID( int index )
//****
{
    char *str = new char[1]; *str = 0;
    return str;
}

void agk::SetSpeechLanguage( const char* lang )
//****
{

}

void agk::SetSpeechLanguageByID( const char* sID )
//****
{

}

int agk::IsSpeaking()
//****
{
	return 0;
}

void agk::StopSpeaking()
//****
{
	
}

int uString::ToInt() const
{
	//NSString* pString = [ [ NSString alloc ] initWithCString: m_pData.GetStr() length: m_pData.GetLength() * sizeof ( char ) ];
	//return [string intValue];
	return atoi(m_pData);
}

float uString::ToFloat() const
{
	return atof(m_pData);
}

void agk::PlatformReportError( const uString &sMsg )
{
	printf("%s\n", sMsg.GetStr());
    fflush(stdout);
}

void agk::PlatformMessage( const char* msg )
{
	NSString* pString = [ [ NSString alloc ] initWithCString: msg ];
	NSAlert *alert = [[NSAlert alloc] init];
	[ alert setMessageText:@"Message" ];
	[ alert setInformativeText:pString ];
	[ pString release ];
	[ alert runModal ];
	[ alert release ];
}

// Thread functions

void AGKThread::PlatformInit( )
{
	pThread = new pthread_t;
	m_pStop = new pthread_cond_t;
	
	pthread_cond_init( (pthread_cond_t*)m_pStop, NULL );
}

UINT STDCALL StartNewThread( void* param )
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	UINT result = ((AGKThread*)param)->EntryPoint( param );
	[pool drain];
	return result;
}

void AGKThread::PlatformStart( )
{
	//int result = pthread_create( (pthread_t*)pThread, NULL, (void*(*)(void*))EntryPoint, this );
	int result = pthread_create( (pthread_t*)pThread, NULL, (void*(*)(void*))StartNewThread, this );
	if ( result != 0 ) agk::Warning( "Failed to start pthread" );
}

void AGKThread::PlatformStop( )
{
	pthread_cond_signal( (pthread_cond_t*)m_pStop );
}

void AGKThread::PlatformTerminate( )
{
	if ( m_bRunning ) 
	{
		agk::Warning("Forcing a thread to terminate, this may cause a crash...");
		//pthread_cancel(*pThread);
	}
}

void AGKThread::PlatformCleanUp( )
{
	if ( m_pStop ) 
	{
		pthread_cond_destroy( (pthread_cond_t*)m_pStop );
		delete ((pthread_cond_t*)m_pStop);
	}
	m_pStop = 0;
	
	if ( pThread )
	{
		delete (pthread_t*)pThread;
		pThread = 0;
	}
}

void AGKThread::PlatformJoin( )
{
	pthread_join( *((pthread_t*)pThread), NULL );
}

void AGKThread::PlatformSleepSafe( UINT milliseconds )
{
	if ( m_bTerminate ) return;
	pthread_mutex_t mutex;
	pthread_mutex_init( &mutex, NULL );
	pthread_mutex_lock( &mutex );
	timespec waittime;
	waittime.tv_sec = milliseconds / 1000;
	waittime.tv_nsec = (milliseconds%1000) * 1000000;
	pthread_cond_timedwait_relative_np( (pthread_cond_t*)m_pStop, &mutex, &waittime );
	pthread_mutex_unlock( &mutex );
	pthread_mutex_destroy( &mutex );
}

// filesystem commands

void agk::SetRawWritePath( const char* str )
{
	agk::Warning( "SetRawWritePath is deprecated, the preferred method is now OpenRawFolder and \"raw:\" file paths" );
    strcpy( szWriteDir, str );
	UINT length = strlen( str );
	if ( str[ length-1 ] != '/' && str[ length-1 ] != '\\' ) strcat( szWriteDir, "/" );
    
	char *str2 = szWriteDir;
	while ( *str2 )
	{
		if ( *str2 == '\\' ) *str2 = '/';
		str2++;
	}
    
	m_bUpdateFileLists = true;
}

char* agk::GetWritePath()
{
	// allow write directory to be determined
	char *szStr = new char[ strlen(szWriteDir)+1 ];
	strcpy( szStr, szWriteDir );
	return szStr;
}

char* agk::GetReadPath()
{
	char *szStr = new char[ strlen(szRootDir)+1 ];
	strcpy( szStr, szRootDir );
	return szStr;
}

char* agk::GetDocumentsPath()
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); 
	NSString *docsDirectoryPath = [paths objectAtIndex:0];
	const char* szDocs = [ docsDirectoryPath UTF8String ];

	char *szStr = new char[ strlen(szDocs)+1 ];
	strcpy( szStr, szDocs );
	return szStr;
}

bool agk::PlatformChooseFile( uString &outstr, const char *ext, int fullPath )
{
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setCanChooseDirectories:FALSE];
    [panel setAllowsMultipleSelection:FALSE];
    
    NSInteger result = [panel runModal];
    if ( result != NSOKButton ) return false;
    
    NSString *pPath = [panel filename];
    
    if ( 1 == fullPath )
    {
		outstr.SetStr( [pPath UTF8String] );
    }
    else
    {
		uString sFilename( [pPath UTF8String] );
		if ( sFilename.Find('/') >= 0 )
		{
			uString sub;
			sFilename.SubString(sub, sFilename.RevFind('/')+1);
			sFilename.SetStr(sub);
		}
		uString sFullPath( sFilename );
		agk::PlatformGetFullPathWrite( sFullPath );
        
		NSString* pPathString = [ [ NSString alloc ] initWithUTF8String:sFullPath.GetStr() ];
		[g_pFileManager copyItemAtPath:pPath toPath:pPathString error:nil ];    
		[pPathString release ];
        
		outstr.SetStr(sFilename);
	}
	
    return true;
}

void agk::PlatformGetFullPathWrite( uString &inoutstr )
{
	inoutstr.Replace( '\\', '/' );
	if ( inoutstr.FindStr( "../" ) >= 0 ) 
	{
		uString sub;
		inoutstr.SubString( sub, inoutstr.RevFind( '/' )+1 );
		inoutstr.SetStr( sub );
	}
    
	if ( inoutstr.CharAt( 0 ) == '/' ) 
	{
		uString sub;
		inoutstr.SubString( sub, 1 );
		inoutstr.SetStr( sub );
		inoutstr.Prepend( szWriteDir );
	}
	else 
	{
		inoutstr.Prepend( m_sCurrentDir );
		inoutstr.Prepend( szWriteDir );
	}
    
    cFileEntry::ConvertToReal( inoutstr );
}

void agk::PlatformGetFullPathRead( uString &inoutstr, int mode )
{
	inoutstr.Replace( '\\', '/' );
	
	if ( mode == 1 ) 
	{
		RemovePath( inoutstr );
		inoutstr.Prepend( szRootDir );
		cFileEntry::ConvertToReal( inoutstr );
		return;
	}
	
	if ( inoutstr.FindStr( "../" ) >= 0 ) 
	{
		uString sub;
		inoutstr.SubString( sub, inoutstr.RevFind( '/' )+1 );
		inoutstr.SetStr( sub );
	}
    
	if ( inoutstr.CharAt( 0 ) == '/' ) 
	{
		uString sub;
		inoutstr.SubString( sub, 1 );
		inoutstr.SetStr( sub );
		inoutstr.Prepend( szRootDir );
	}
	else 
	{
		inoutstr.Prepend( m_sCurrentDir );
		inoutstr.Prepend( szRootDir );
	}
    
    cFileEntry::ConvertToReal( inoutstr );
}


int agk::PlatformCreateRawPath( const char* path )
{
	if ( !path || !*path ) return 0;
	if ( path[0] != '/' ) 
	{
		uString err; err.Format( "Invalid path \"%s\", must be absolute", path );
		agk::Error( err );
		return 0;
	}

	char* withoutFile = new char[ strlen(path)+1 ];
	strcpy( withoutFile, path );
	char *szSlash = strrchr( withoutFile, '/' );
    if ( szSlash ) *szSlash = '\0';
	
	// create directory
    NSString* pWriteDir = [ [ NSString alloc ] initWithUTF8String:withoutFile ];
    [ g_pFileManager createDirectoryAtPath:pWriteDir withIntermediateDirectories:YES attributes:nil error:NULL ];
    [ pWriteDir release ];
    
    delete [] withoutFile;
    return 1;
}

int cFolder::OpenFolder( const char* szPath )
{
	if ( m_pFiles ) delete [] m_pFiles;
	m_pFiles = 0;
	m_iNumFiles = 0;

	if ( m_pFolders ) delete [] m_pFolders;
	m_pFolders = 0;
	m_iNumFolders = 0;

	if ( strncmp( szPath, "raw:", 4 ) == 0 )
	{
		szPath += 4;
	}

	if ( !agk::IsAbsolutePath( szPath ) )
	{
		uString err; err.Format( "Failed to open folder \"%s\", it must be an absolute path", szPath );
		agk::Error( err );
		return 0;
	}

	NSString* pPathString = [ [ NSString alloc ] initWithUTF8String:szPath ];
	NSArray* contents = [ g_pFileManager contentsOfDirectoryAtPath:pPathString error:NULL ];
	if ( !contents )
	{
		uString err; err.Format( "Failed to open folder \"%s\", it may not exist or this app doesn't have permission to accesss it", szPath );
		agk::Error( err );
		return 0;
	}

	for ( int i = 0; i < [ contents count ]; i++ )
	{
		BOOL bIsDirectory = FALSE;
		NSString *pString = [ contents objectAtIndex:i ];
		NSString *fullPath = [pPathString stringByAppendingPathComponent:pString];
		[ g_pFileManager fileExistsAtPath:fullPath isDirectory:(&bIsDirectory) ];
		if ( bIsDirectory ) m_iNumFolders++;
		else m_iNumFiles++;
	}
    
    m_pFiles = new uString[ m_iNumFiles ];
	m_pFolders = new uString[ m_iNumFolders ];

	int fileCount = 0;
	int folderCount = 0;
    
	for ( int i = 0; i < [ contents count ]; i++ )
	{
		BOOL bIsDirectory = FALSE;
		NSString *pString = [ contents objectAtIndex:i ];
		NSString *fullPath = [pPathString stringByAppendingPathComponent:pString];
		[ g_pFileManager fileExistsAtPath:fullPath isDirectory:(&bIsDirectory) ];
		if ( bIsDirectory )
		{
			// directory
			m_pFolders[ folderCount++ ].SetStr( [ pString UTF8String ] );			
		}
		else
		{
			// file
			m_pFiles[ fileCount++ ].SetStr( [ pString UTF8String ] );
		}
	}
    [ pPathString release ];

	return 1;
}

bool AGK::cFile::ExistsWrite( const char *szFilename )
{
	if ( !szFilename || !*szFilename ) return false;
	if ( strncmp(szFilename, "raw:", 4) == 0 ) return false;
	
	if ( strchr( szFilename, ':' ) ) return false;
	if ( strstr(szFilename, "..\\") || strstr(szFilename, "../") ) return false;
	
	UINT length = strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' ) return false;
	
    uString sPath( szFilename );
    agk::PlatformGetFullPathWrite(sPath);
	
	struct stat buf;
	if ( stat( sPath.GetStr(), &buf ) != 0 ) return false;
	
	return true;
}

bool AGK::cFile::ExistsRead( const char *szFilename, int *mode )
{
	if ( !szFilename || !*szFilename ) return false;
	if ( strncmp(szFilename, "raw:", 4) == 0 ) return false;
	
	if ( strchr( szFilename, ':' ) ) return false;
	if ( strstr(szFilename, "..\\") || strstr(szFilename, "../") ) return false;
	
	UINT length = strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' ) return false;
	
    uString sPath(szFilename);
    agk::PlatformGetFullPathRead(sPath);
	
	if ( mode ) *mode = 0;
	FILE *pFile = AGKfopen( sPath, "rb" );
	if ( !pFile )
	{
		// try resource folder without path
		uString str( szFilename );
		RemovePath( str );
		sPath.SetStr( szRootDir );
		sPath.Append( str );
		cFileEntry::ConvertToReal( sPath );
		
		if ( mode ) *mode = 1;
		pFile = AGKfopen( sPath, "rb" );
	}
	
	if ( !pFile ) return false;
	
	fclose( pFile );
	return true;
}

bool cFile::ExistsRaw( const char *szFilename )
{
	if ( !szFilename || !*szFilename ) return false;
	UINT length = strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' ) return false;

	if ( strncmp(szFilename, "raw:", 4) != 0 ) return false;
	if ( !agk::IsAbsolutePath( szFilename ) ) return false;
	
	// absolute path to anywhere allowed
	struct stat buf;
	if ( stat( szFilename+4, &buf ) != 0 ) return false;
	return true;
}

bool AGK::cFile::Exists( const char *szFilename )
{
	if ( !ExistsRaw( szFilename ) )
	{
		if ( !ExistsWrite( szFilename ) )
		{
			if ( !ExistsRead( szFilename ) ) return false;
		}
	}

	return true;
}

bool AGK::cFile::GetModified( const char *szFilename, int &time )
{
	if ( !szFilename || !*szFilename ) return false;
	
	UINT length = strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' ) return false;
	
    struct stat fileInfo;
	uString sPath( szFilename );
	if ( !agk::GetRealPath( sPath ) ) return false;
		
	int result = stat( sPath.GetStr(), &fileInfo );
	if ( result != 0 ) return false;
	time = (int) fileInfo.st_mtime;
	return true;
}

void cFile::SetModified( const char *szFilename, int time )
{
	if ( !szFilename || !*szFilename ) return;
	
	size_t length = strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' ) return;
	
	uString sPath( szFilename );
	if ( cFile::ExistsRaw( szFilename ) ) sPath.SetStr( szFilename+4 );
	else if ( cFile::ExistsWrite( szFilename ) ) agk::PlatformGetFullPathWrite(sPath);
	else return;
	
	struct utimbuf fileInfo;
	fileInfo.actime = time;
	fileInfo.modtime = time;
	utime( sPath.GetStr(), &fileInfo );
}

UINT AGK::cFile::GetFileSize( const char *szFilename )
{
	cFile pFile;
	if ( !pFile.OpenToRead( szFilename ) ) return 0;
	UINT size = pFile.GetSize();
	pFile.Close();
	
	return size;
}

void AGK::cFile::DeleteFile( const char *szFilename )
{
	if ( !szFilename || !*szFilename ) return;
	
	UINT length = strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' )
	{
		agk::Error( "Invalid path for DeleteFile file, must not end in a forward or backward slash" );
		return;
	}
	
	uString sPath( szFilename );
	if ( cFile::ExistsRaw( szFilename ) ) sPath.SetStr( szFilename+4 );
	else if ( cFile::ExistsWrite( szFilename ) ) agk::PlatformGetFullPathWrite(sPath);
	else return;
	
	NSString* pPath = [ [ NSString alloc ] initWithCString:sPath ];
	[ g_pFileManager removeItemAtPath:pPath error:nil ];
	[ pPath release ];

	agk::m_bUpdateFileLists = true;
}

bool AGK::cFile::OpenToWrite( const char *szFilename, bool append )
{
	if ( !szFilename || !*szFilename ) return false;
	if ( pFile ) Close();
	mode = 1;
	
	int raw = 0;
	uString sPath( szFilename );
	if ( strncmp(szFilename, "raw:", 4) == 0 ) 
	{
		raw = 1;
		sPath.SetStr( szFilename+4 );
	}
	else agk::PlatformGetFullPathWrite(sPath);

	if ( !agk::PlatformCreateRawPath( sPath ) ) return false;
	
	if ( append ) pFile = AGKfopen( sPath, "ab" );
	else pFile = AGKfopen( sPath, "wb" );
	
	if ( !pFile )
	{
#ifdef _AGK_ERROR_CHECK
		uString err = "Failed to open file for writing ";
		err += sPath;
		agk::Error( err );
#endif
		return false;
	}
	
	// refresh any stored directory details for the new file
	if ( 0 == raw ) cFileEntry::AddNewFile( sPath );
	agk::m_bUpdateFileLists = true;
	
	return true;
}

bool AGK::cFile::OpenToRead( const char *szFilename )
{
	if ( !szFilename || !*szFilename ) return false;
	if ( pFile ) Close();
	mode = 0;
	
	uString sPath( szFilename );
    if ( !agk::GetRealPath( sPath ) )
	{
		uString err = "Could not find file ";
		err += szFilename;
		agk::Error( err );
		return false;
	}
	
	pFile = AGKfopen( sPath, "rb" );
	if ( !pFile )
	{
		uString err = "Failed to open file for reading ";
		err += szFilename;
		agk::Error( err );
		return false;
	}
	
	return true;
}

void AGK::cFile::Close()
{
	if ( pFile ) fclose( pFile );
	pFile = UNDEF;
}

UINT AGK::cFile::GetPos()
{
	if ( !pFile ) return 0;
	return ftell( pFile );
}

void AGK::cFile::Seek( UINT pos )
{
	if ( !pFile ) return;
	fseek( pFile, pos, SEEK_SET );
}

void AGK::cFile::Flush()
{
	if ( !pFile ) return;
	fflush( pFile );
}

UINT AGK::cFile::GetSize()
{
	if ( !pFile ) return 0;
	fpos_t pos;
	fgetpos( pFile, &pos );
	fseek( pFile, 0, SEEK_END );
	UINT size = ftell( pFile );
	fsetpos( pFile, &pos );
	return size;
}

void AGK::cFile::Rewind()
{
	if ( !pFile ) return;
	rewind( pFile );
}

bool AGK::cFile::IsEOF()
{
	if ( !pFile ) return true;
	return feof( pFile ) != 0;
}

void cFile::WriteByte( unsigned char b )
{
	if ( !pFile ) return;
	if ( mode != 1 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot write to file opened for reading" );
#endif
		return;
	}

	fwrite( &b, 1, 1, pFile );
}

void AGK::cFile::WriteInteger( int i )
{
	if ( !pFile ) return;
	if ( mode != 1 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot write to file opened for reading" );
#endif
		return;
	}
	
	//convert everything to little endian for cross platform compatibility
	i = agk::PlatformLittleEndian( i );
	fwrite( &i, 4, 1, pFile );
}

void AGK::cFile::WriteFloat( float f )
{
	if ( !pFile ) return;
	if ( mode != 1 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot write to file opened for reading" );
#endif
		return;
	}
	fwrite( &f, 4, 1, pFile );
}

void AGK::cFile::WriteString( const char *str )
{
    if ( !str ) return;
	if ( !pFile ) return;
	if ( mode != 1 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot write to file opened for reading" );
#endif
		return;
	}
	UINT length = strlen( str );
	fwrite( str, 1, length+1, pFile );
}

void cFile::WriteString2( const char *str )
{
	if ( !str ) return;
	if ( !pFile ) return;
	if ( mode != 1 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot write to file opened for reading" );
#endif
		return;
	}
	UINT length = strlen( str );
	UINT l = agk::PlatformLittleEndian( length );
	fwrite( &l, 4, 1, pFile );
	fwrite( str, 1, length, pFile );
}

void AGK::cFile::WriteData( const char *str, UINT bytes )
{
	if ( !pFile ) return;
	if ( mode != 1 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot write to file opened for reading" );
#endif
		return;
	}
	
	fwrite( str, 1, bytes, pFile );
}

void AGK::cFile::WriteLine( const char *str )
{
	if ( !pFile ) return;
	if ( mode != 1 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot write to file opened for reading" );
#endif
		return;
	}
	UINT length = strlen( str );
	fwrite( str, 1, length, pFile );

	// strings terminate with CR (13,10) - so it resembles a regular text file when file viewed
	char pCR[2];
	pCR[0]=13;
	pCR[1]=10;
	fwrite( &pCR[0], 1, 1, pFile );
	fwrite( &pCR[1], 1, 1, pFile );
}

unsigned char AGK::cFile::ReadByte( )
{
	if ( !pFile ) return 0;
	if ( mode != 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot read from file opened for writing" );
#endif
		return 0;
	}
	unsigned char b = 0;
	fread( &b, 1, 1, pFile );
	return b;
}

int AGK::cFile::ReadInteger( )
{
	if ( !pFile ) return 0;
	if ( mode != 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot read from file opened for writing" );
#endif
		return 0;
	}
	int i = 0;
	fread( &i, 4, 1, pFile );
	// convert back to local endian, everything in the file is little endian.
	return i = agk::PlatformLocalEndian( i );
}

float AGK::cFile::ReadFloat( )
{
	if ( !pFile ) return 0;
	if ( mode != 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot read from file opened for writing" );
#endif
		return 0;
	}
	float f;
	fread( &f, 4, 1, pFile );
	return f;
}

int AGK::cFile::ReadString( uString &str )
{
	if ( !pFile ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot read from file, file not open" );
#endif
		return -1;
	}

	if ( mode != 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot read from file opened for writing" );
#endif
		return -1;
	}
	
	char *buffer = 0;
	int bufLen = 0;
	int pos = 0;
	int diff = 0;
	
	// read until a LF (10) line terminator is found, or eof.
	bool bNullFound = false;
	do
	{
		pos = bufLen;
		if ( bufLen == 0 )
		{
			bufLen = 256;
			buffer = new char[257];
			diff = 256;
		}
		else
		{
			int newLen = bufLen*3 / 2;
			char *newBuf = new char[ newLen+1 ];
			memcpy( newBuf, buffer, bufLen );
			delete [] buffer;
			diff = newLen - bufLen;
			buffer = newBuf;
			bufLen = newLen;
		}

		long lPos = ftell( pFile );
		UINT written = (UINT) fread( buffer+pos, 1, diff, pFile ); 
		buffer[pos+written] = 0;
		bool bFound = false;
		for ( UINT i = 0; i < written; i++ )
		{
			if ( buffer[ pos+i ] == 0 ) 
			{
				fseek( pFile, lPos+i+1, SEEK_SET );
				bFound = true;
				break;
			}
		}
		if ( bFound ) break;
	} while( !feof( pFile ) );

	str.SetStr( buffer );
	delete [] buffer;
	return str.GetLength();
}

int cFile::ReadString2( uString &str )
{
	if ( !pFile ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot read from file, file not open" );
#endif
		return -1;
	}

	if ( mode != 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot read from file opened for writing" );
#endif
		return -1;
	}

	str.ClearTemp();

	UINT length = 0;
	fread( &length, 4, 1, pFile );
	// convert back to local endian, everything in the file is little endian.
	length = agk::PlatformLocalEndian( length );

	char *buffer = new char[ length+1 ];
	fread( buffer, 1, length, pFile );
	buffer[ length ] = 0;
	str.SetStr( buffer );

	delete [] buffer;
		
	return str.GetLength();
}

int AGK::cFile::ReadLine( uString &str )
{
	if ( !pFile ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot read from file, file not open" );
#endif
		return -1;
	}

	if ( mode != 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot read from file opened for writing" );
#endif
		return -1;
	}

	str.ClearTemp();

	char *buffer = 0;
	int bufLen = 0;
	int pos = 0;
	int diff = 0;
	
	// read until a LF (10) line terminator is found, or eof.
	bool bNullFound = false;
	do
	{
		pos = bufLen;
		if ( bufLen == 0 )
		{
			bufLen = 256;
			buffer = new char[257];
			diff = 256;
		}
		else
		{
			int newLen = bufLen*3 / 2;
			char *newBuf = new char[ newLen+1 ];
			memcpy( newBuf, buffer, bufLen );
			delete [] buffer;
			diff = newLen - bufLen;
			buffer = newBuf;
			bufLen = newLen;
		}

		long lPos = ftell( pFile );
		UINT written = (UINT) fread( buffer+pos, 1, diff, pFile ); 
		buffer[pos+written] = 0;
		bool bFound = false;
		for ( UINT i = 0; i < written; i++ )
		{
			if ( buffer[ pos+i ] == '\n' ) 
			{
				buffer[ pos+i ] = 0;
				fseek( pFile, lPos+i+1, SEEK_SET );
				bFound = true;
				break;
			}
		}
		if ( bFound ) break;
	} while( !feof( pFile ) );

	str.SetStr( buffer );
	delete [] buffer;
	str.Trim( "\r\n" );
	return str.GetLength();
}
/*
int cFile::ReadLineFast( uString &str )
{
	if ( !pFile ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot read from file, file not open" );
#endif
		return -1;
	}

	if ( mode != 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot read from file opened for writing" );
#endif
		return -1;
	}

	str.ClearTemp();

	char tempstr[ 1024 ];
	if ( !fgets( tempstr, 1024, pFile ) ) return 0;
	str.SetStr( tempstr );

	while ( !IsEOF() && str.CharAt( str.GetNumChars()-1 ) != '\n' )
	{
		if ( !fgets( tempstr, 1024, pFile ) ) break;
		str.Append( tempstr );
	}

	str.Trim( "\n\r" );
	return str.GetLength();
}
*/
int AGK::cFile::ReadData( char *str, UINT length )
{
	if ( !pFile ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot read from file, file not open" );
#endif
		return -1;
	}

	if ( mode != 0 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot read from file opened for writing" );
#endif
		return -1;
	}
	
	return fread( str, 1, length, pFile );
}

void agk::ParseCurrentDirectory()
{
	m_bUpdateFileLists = false;

	// clear any old directories
	while ( m_pCurrentDirectories )
	{
		cDirectoryItem *pItem = m_pCurrentDirectories;
		m_pCurrentDirectories = m_pCurrentDirectories->m_pNext;
		delete pItem;
	}
	
	m_pCurrentDirectoryIter = 0;
	
	// clear any old files
	while ( m_pCurrentFiles )
	{
		cDirectoryItem *pItem = m_pCurrentFiles;
		m_pCurrentFiles = m_pCurrentFiles->m_pNext;
		delete pItem;
	}
	
	m_pCurrentFileIter = 0;
	
	// check root directory for files and directories (non-recursive)
	uString sPath( szRootDir );
	sPath.Append( m_sCurrentDir );
	cFileEntry::ConvertToReal( sPath );
	
	NSString* pPathString = [ [ NSString alloc ] initWithCString:sPath.GetStr() ];
	NSArray* contents = [ g_pFileManager contentsOfDirectoryAtPath:pPathString error:NULL ];
	if ( contents ) 
	{	
		for ( int i = 0; i < [ contents count ]; i++ )
		{
			BOOL bIsDirectory = FALSE;
			NSString *pString = [ contents objectAtIndex:i ];
			NSString *fullPath = [pPathString stringByAppendingPathComponent:pString];
			[ g_pFileManager fileExistsAtPath:fullPath isDirectory:(&bIsDirectory) ];
			if ( bIsDirectory )
			{
				// directory
				cDirectoryItem *pNewItem = new cDirectoryItem();
				pNewItem->iFlags |= AGK_FILE_TYPE_READ;
				pNewItem->m_sItem.SetStr( [ pString UTF8String ] );
				pNewItem->m_pNext = m_pCurrentDirectories;
				m_pCurrentDirectories = pNewItem;
			}
			else
			{
				// file
				cDirectoryItem *pNewItem = new cDirectoryItem();
				pNewItem->iFlags |= AGK_FILE_TYPE_READ;
				pNewItem->m_sItem.SetStr( [ pString UTF8String ] );
				pNewItem->m_pNext = m_pCurrentFiles;
				m_pCurrentFiles = pNewItem;
			}
		}
	}
	
	[ pPathString release ];
	
	// check write directory for files and directories (non-recursive)
	sPath.SetStr( szWriteDir );
	sPath.Append( m_sCurrentDir );
	cFileEntry::ConvertToReal( sPath );
	
	pPathString = [ [ NSString alloc ] initWithCString:sPath.GetStr() ];
	contents = [ g_pFileManager contentsOfDirectoryAtPath:pPathString error:NULL ];
	if ( contents ) 
	{	
		for ( int i = 0; i < [ contents count ]; i++ )
		{
			BOOL bIsDirectory = FALSE;
			NSString *pString = [ contents objectAtIndex:i ];
			NSString *fullPath = [pPathString stringByAppendingPathComponent:pString];
			[ g_pFileManager fileExistsAtPath:fullPath isDirectory:(&bIsDirectory) ];
			if ( bIsDirectory )
			{
				// directory
				bool bExists = false;
				cDirectoryItem *pItem = m_pCurrentDirectories;
				while ( pItem )
				{
					if ( pItem->m_sItem.CompareTo( [ pString UTF8String ] ) == 0 )
					{
						pItem->iFlags |= AGK_FILE_TYPE_WRITE;
						bExists = true;
						break;
					}
					pItem = pItem->m_pNext;
				}
				
				if ( !bExists )
				{
					cDirectoryItem *pNewItem = new cDirectoryItem();
					pNewItem->iFlags |= AGK_FILE_TYPE_WRITE;
					pNewItem->m_sItem.SetStr( [ pString UTF8String ] );
					pNewItem->m_pNext = m_pCurrentDirectories;
					m_pCurrentDirectories = pNewItem;
				}
			}
			else
			{
				// file
				bool bExists = false;
				cDirectoryItem *pItem = m_pCurrentFiles;
				while ( pItem )
				{
					if ( pItem->m_sItem.CompareTo( [ pString UTF8String ] ) == 0 )
					{
						pItem->iFlags |= AGK_FILE_TYPE_WRITE;
						bExists = true;
						break;
					}
					pItem = pItem->m_pNext;
				}
				
				if ( !bExists )
				{
					cDirectoryItem *pNewItem = new cDirectoryItem();
					pNewItem->iFlags |= AGK_FILE_TYPE_WRITE;
					pNewItem->m_sItem.SetStr( [ pString UTF8String ] );
					pNewItem->m_pNext = m_pCurrentFiles;
					m_pCurrentFiles = pNewItem;
				}
			}
		}
	}
	
	[ pPathString release ];
}

int agk::SetCurrentDir( const char* szPath )
{
	if ( !szPath || strlen( szPath ) == 0 )
	{
		m_sCurrentDir.SetStr( "" );
		//ParseCurrentDirectory();
		m_bUpdateFileLists = true;
		return 1;
	}
    
    if ( strcmp( szPath, ".." ) == 0 ) 
	{
		int pos = m_sCurrentDir.Find( '/' );
		if ( pos >= 0 && pos < m_sCurrentDir.GetNumChars()-1 )
		{
			m_sCurrentDir.Trunc( '/' );
			m_sCurrentDir.Trunc( '/' );
			m_sCurrentDir.AppendAscii( '/' );
		}
		else
		{
			m_sCurrentDir.SetStr( "" );
		}
		//ParseCurrentDirectory();
		m_bUpdateFileLists = true;
		return 1;
	}
	
	if ( strstr(szPath, "..") )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Invalid path for SetCurrentDir, must not traverse backwards up the directory tree using ../" );
#endif
		return 0;
	}
    
    uString sPath( szPath );
	
	if ( szPath[0] == '\\' || szPath[0] == '/' )
	{
        m_sCurrentDir.SetStr( "" );
		sPath.SetStr( szPath+1 );
	}
	
    sPath.Replace( '\\', '/' );
	
    // don't do a directory check, writing files will create any necessary directories
    /*
	uString sDirPath( szWriteDir );
	sDirPath.Append( m_sCurrentDir );
	sDirPath.Append( sPath );
	cFileEntry::ConvertToReal( sDirPath );
	
	NSString* pString = [ [ NSString alloc ] initWithCString:sDirPath.GetStr() ];
	if ( [ g_pFileManager fileExistsAtPath: pString ] == FALSE )
	{
		sDirPath.SetStr( szRootDir );
		sDirPath.Append( m_sCurrentDir );
		sDirPath.Append( sPath );
		cFileEntry::ConvertToReal( sDirPath );
		
		NSString* pString2 = [ [ NSString alloc ] initWithCString:sDirPath.GetStr() ];
		if ( [ g_pFileManager fileExistsAtPath: pString2 ] == FALSE ) 
		{
			[ pString release ];
			[ pString2 release ];
			return false;
		}
		[ pString2 release ];
	}
	[ pString release ];
     */
    
	/*
	//BAG HACK
    if ( strcmp(szPath,"media") == 0 && m_sCurrentDir.GetLength() == 0 )
    {
        return 1;
    }
    
    if ( strcmp(szPath,"/media") == 0 )
    {
        m_sCurrentDir.SetStr( "" );
		ParseCurrentDirectory();
		return 1;
    }
	*/
	
	// add a final slash
	UINT length = strlen( szPath );
	if ( szPath[ length-1 ] != '/' && szPath[ length-1 ] != '\\' ) sPath.AppendAscii( '/' );
	
	m_sCurrentDir.Append( sPath );
    m_sCurrentDir.Replace( '\\', '/' );
	
	//ParseCurrentDirectory();
	m_bUpdateFileLists = true;
	
	return 1;
}

int agk::MakeFolder( const char* folder )
{
    if ( !folder || strlen( folder ) == 0 )
	{
		return 0;
	}

	uString sPath;
	if ( strncmp(folder, "raw:", 4) == 0 )
	{
		sPath.SetStr( folder+4 );
	}
	else
	{
		// no special characters
		if ( strchr( folder, ':' ) || strchr( folder, '/' ) || strchr( folder, '\\' ) || strstr( folder, ".." )  )
		{
			agk::Error( "Invalid folder name for MakeFolder, it must not contain the special characters / : \\ .." );
			return 0;
		}
    
		char szDirectory[ MAX_PATH ];
		sPath.SetStr( szWriteDir );
		sPath.Append( agk::m_sCurrentDir );
		sPath.Append( folder );
	}
        
    // create directory
    NSString* pWriteDir = [ [ NSString alloc ] initWithUTF8String:sPath.GetStr() ];
    [ g_pFileManager createDirectoryAtPath:pWriteDir withIntermediateDirectories:YES attributes:nil error:NULL ];
    [ pWriteDir release ];
    
    return 1;
}

void agk::DeleteFolder( const char* folder )
{
    if ( !folder || strlen( folder ) == 0 )
	{
		return;
	}
    
	uString sPath;
	if ( strncmp(folder, "raw:", 4) == 0 )
	{
		sPath.SetStr( folder+4 );
	}
	else
	{
		// no special characters
		if ( strchr( folder, ':' ) || strchr( folder, '/' ) || strchr( folder, '\\' ) || strstr( folder, ".." )  )
		{
			agk::Error( "Invalid folder name for DeleteFolder, it must not contain the special characters / : \\ .." );
			return;
		}
    
		sPath.SetStr( szWriteDir );
		sPath.Append( m_sCurrentDir );
		sPath.Append( folder );
	}
    
    NSString* pWriteDir = [ [ NSString alloc ] initWithUTF8String:sPath.GetStr() ];
    [ g_pFileManager removeItemAtPath:pWriteDir error:nil];
    [ pWriteDir release ];
}

// input commands
int agk::GetMultiTouchExists()
{
	return 0;
}

int agk::GetMouseExists()
{
	return 1;
}

int agk::GetKeyboardExists()
{
	return 1;
}

int agk::GetCameraExists()
{
    NSError *error;
    /*
    QTCaptureDevice *videoDevice = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeVideo];
    BOOL success = [videoDevice open:&error];
    
    if (!success) 
    {
        videoDevice = [QTCaptureDevice defaultInputDeviceWithMediaType:QTMediaTypeMuxed];
        success = [videoDevice open:&error];
    }
    
    if (!success) 
    {
        videoDevice = nil;
        return 0;        
    }
     */
    
    return 1;
}

int agk::GetGPSSensorExists()
{
	return m_iGPSSensorExists;
}

void cJoystick::DetectJoysticks()
{
    /*
    // this takes over 3 milliseconds when a controller is connected
    float start = agk::Timer();
    NSArray *joysticks = [DDHidJoystick allJoysticks];
    float end = agk::Timer() - start;
    NSLog( @"%f", end );
     */
}

void cJoystick::PlatformUpdate()
{
	// mac joysticks updated asynchronously
}

void agk::SetRawMouseVisible( int visible )
{
	if ( visible )
    {
        CGDisplayShowCursor( kCGDirectMainDisplay );
    }
    else
    {
        CGDisplayHideCursor( kCGDirectMainDisplay );
    }
}

void agk::SetRawMousePosition( float x, float y )
{
    int realX = agk::ScreenToDeviceX( x );
    int realY = agk::ScreenToDeviceY( y );
    
    NSWindow *window = [[[NSApplication sharedApplication] windows] objectAtIndex:0];
    NSRect screenRect = [[NSScreen mainScreen] frame];
    float screenX = realX + window.frame.origin.x;
    float screenY = realY + (screenRect.size.height - window.frame.origin.y - [[window contentView] frame].size.height);
    CGWarpMouseCursorPosition(CGPointMake(screenX,screenY));
    CGAssociateMouseAndMouseCursorPosition(true);
    
    x = agk::DeviceToScreenX( realX );
    y = agk::DeviceToScreenY( realY );
    
    if ( m_pMouse[ 0 ] )
    {
        m_pMouse[ 0 ]->SetPosition( x, y );
        if ( !m_pMouse[ 0 ]->IsCaptured() )
        {
            m_fPointerX = RotatePointX( x, y );
            m_fPointerY = RotatePointY( x, y );
        }
    }
}

INT64 agk::GetUnixTime64()
{
	time_t unix = time(NULL);
	return unix;
}

int agk::GetUnixTime()
{
	time_t unix = time(NULL);
	if ( unix > 2147483646 ) unix = 2147483647;
	if ( unix < -2147483647 ) unix = -2147483647;
	return (int) unix;
}

int agk::GetDayOfWeek()
{
	time_t a = time ( NULL );
	tm*    b = localtime ( &a );
	int    c = b->tm_wday;
	return c;
}

//****f* Time/General/GetCurrentDate
// FUNCTION
//   Returns a string representing the current date in the format YYYY-MM-DD, for example 2012-01-09.
// SOURCE
char* agk::GetCurrentDate()
//****
{
	time_t a = time ( NULL );
	tm*    b = localtime ( &a );
	
	int year = b->tm_year + 1900;
	int month = b->tm_mon+1;
	int days = b->tm_mday;

	uString date;
	date.Format( "%04d-%02d-%02d", year, month, days );
	
	char* str = new char[ date.GetLength()+1 ];
	strcpy( str, date.GetStr() );
	return str;
}

//****f* Time/General/GetCurrentTime
// FUNCTION
//   Returns a string representing the current time in the format HH-MM-SS, for example 15:23:04.
// SOURCE
char* agk::GetCurrentTime()
//****
{
	time_t a = time ( NULL );
	tm*    b = localtime ( &a );
	
	int hours = b->tm_hour;
	int minutes = b->tm_min;
	int seconds = b->tm_sec;

	uString time;
	time.Format( "%02d:%02d:%02d", hours, minutes, seconds );
	
	char* str = new char[ time.GetLength()+1 ];
	strcpy( str, time.GetStr() );
	return str;
}

void agk::LoadConsentStatusAdMob( const char* szPubID, const char* privacyPolicy )
//****
{

}

int agk::GetConsentStatusAdMob()
//****
{
	return 0;
}

void agk::RequestConsentAdMob()
//****
{

}

void agk::OverrideConsentAdMob( int consent )
//****
{
	
}

void agk::OverrideConsentChartboost( int consent )
//****
{
	
}

int agk::PlatformGetAdPortal()
{
	//return 659; // Windows Phone 7
	return 642; // iPhone
	//return 551; // Ovi
	//return 559; // Android
	//return 635; // Blackberry
	//return 641; // SonyEricsson 
	//return 661; // Bada
	//return 738; // Palm
	//return 818; // Samsung
	//return 947; // iPad, doesn't seem to produce ads
}


void cEditBox::PlatformStartText()
{
    // do nothing on mac
}

void cEditBox::PlatformEndText()
{
	// do nothing on mac
}

void cEditBox::PlatformUpdateExternal()
{
	
}

bool cEditBox::PlatformUpdateText()
{
    // do nothing on mac
    return false;
}

void cEditBox::PlatformUpdateTextEnd()
{
	// do nothing
}

//****f* HTTP/General/OpenBrowser
// FUNCTION
//   Opens the default browser of the current platform with points it to the page given.
// SOURCE
void agk::OpenBrowser( const char *url )
//****
{
    uString sURL( url );
    if ( sURL.FindStr( ":" ) < 0 ) sURL.Prepend( "http://" );
    sURL.ReplaceStr( " ", "%20" );
    
    NSString *pURL = [[NSString alloc] initWithUTF8String:sURL.GetStr()];
//    [[NSWorkspace sharedWorkspace] openFile:sURL];
    
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:pURL]];
    [ pURL release ];
}

UINT agk::RunApp( const char *szFilename, const char *szParameters )
{
	uString sPath( szFilename );
	GetRealPath( sPath );	
	
	// count parameters
    int count = 2;
    const char *ptr = szParameters;
    while( *ptr )
    {
        if ( *ptr == '"' )
        {
            do { ptr++; } while( *ptr && (*ptr != '"' || *(ptr-1) == '\\') );
            if ( !*ptr ) 
            {
				Error( "Malformed parameter list, no matching double quote" );
				return 0;
			}
        }
        
        if ( *ptr == ' ' && (ptr == szParameters || *(ptr-1) != '\\') ) count++;
        ptr++;
    }
    
    char **vparams = new char*[ count+1 ];
    for ( int i = 0; i <= count; i++ ) vparams[i] = 0;
	
    vparams[ 0 ] = new char[ strlen(szFilename)+1 ];
    strcpy( vparams[0], szFilename );
    
    // parse parameters into array
    count = 1;
    ptr = szParameters;
    const char *ptrBegin = szParameters;
    bool bTrimQuotes = false;
    while( *ptr )
    {
        if ( *ptr == '"' )
        {
			if ( ptr == szParameters || *(ptr-1) == ' ' ) 
			{
				ptrBegin++;
				bTrimQuotes = true;
			}
            do { ptr++; } while( *ptr && (*ptr != '"' || *(ptr-1) == '\\') );
            if ( !*ptr ) 
            {
				for ( int i = 0; i < count; i++ )
					if ( vparams[i] ) delete [] vparams[i];
				delete [] vparams;
				Error( "Malformed parameter list, no matching double quote" );
				return 0;
			}
        }
        
        if ( *ptr == ' ' && (ptr == szParameters || *(ptr-1) != '\\') )
        {
            int length = (int)(ptr - ptrBegin);
            if ( ptr != ptrBegin && *(ptr-1) == '"' && bTrimQuotes ) length--;
            if ( length <= 0 ) 
            {
				for ( int i = 0; i < count; i++ )
					if ( vparams[i] ) delete [] vparams[i];
				delete [] vparams;
				Error( "Malformed parameter list, more than one space between parameters, or empty quotes" );
				return 0;
			}
            
            vparams[ count ] = new char[ length+1 ];
            strncpy( vparams[ count ], ptrBegin, length );
            vparams[ count ][ length ] = 0;
            
            ptrBegin = ptr+1;
            bTrimQuotes = false;
            count++;
        }
        ptr++;
    }
    
    // last param
    int length = (int)(ptr - ptrBegin);
    if ( ptr != szParameters && *(ptr-1) == '"' && bTrimQuotes ) length--;
    if ( length <= 0 )
    {
        vparams[ count ] = 0;
    }
    else
    {
        vparams[ count ] = new char[ length+1 ];
        strncpy( vparams[ count ], ptrBegin, length );
        vparams[ count ][ length ] = 0;
    }
	
	// create child process
	pid_t pid = fork();
	if ( pid < 0 ) return 0; // failed
	else if ( pid == 0 ) // child
	{
		execvp( sPath.GetStr(), vparams ); 
		_exit(127); // failed to start child process, don't call exit() without the underscore
	}
	else // parent
	{
		for ( int i = 0; i < count; i++ )
			if ( vparams[i] ) delete [] vparams[i];
		delete [] vparams;
		return pid;
	}
}

UINT agk::GetAppRunning( UINT appID )
{
	if ( appID <= 0 ) return 0;
	
	int result = kill( appID, 0 ); // doesn't actually kill the process, just checks it
	if ( result == 0 ) 
	{
		// child exists, check if it has stopped
		int status;
		int result2 = waitpid( appID, &status, WNOHANG );
		if ( result2 == appID ) return 0;
		else if ( result2 == 0 ) return 1;
		else return 0;
	}
	else return 0;
}

void agk::TerminateApp( UINT appID )
{
	if ( appID <= 0 ) return;
	
	kill( appID, SIGKILL ); // kills the process
	
	int status;
	waitpid( appID, &status, WNOHANG ); // if we don't call this then the process becomes a zombie
}

void agk::ViewFile( const char* szFilename )
{
	
}

void agk::ShareText( const char* szText )
{
	
}

void agk::ShareImage( const char* szFilename )
{
	
}

void agk::ShareImageAndText( const char* szFilename, const char* szText )
{

}

void agk::ShareFile( const char* szFilename )
//****
{

}

void agk::FacebookActivateAppTracking()
//****
{
	
}

int agk::GetInternetState()
//****
{
	// todo
	return 1;
}

void agk::SetPushNotificationKeys( const char* data1, const char* reserved )
//****
{
	// do nothing on mac
}

int agk::PushNotificationSetup()
//****
{
	return 0;
}


char* agk::GetPushNotificationToken()
//****
{
	char *str = new char[ 1 ];
	*str = 0;
	return str;
}

// V108 ULTRABOOK COMMANDS

int agk::GetNotificationType()
{
	return 0;
}

void agk::SetNotificationImage(int iImageIndex)
{
}

void agk::SetNotificationText(const char* pText)
{
}

int agk::GetNFCExists()
{
	return 0;
}

UINT agk::GetRawNFCCount()
{
	return 0;
}

UINT agk::GetRawFirstNFCDevice()
{
	return 0;
}

UINT agk::GetRawNextNFCDevice()
{
	return 0;
}

char* agk::GetRawNFCName(UINT iIndex)
{
	return 0;
}

int agk::SendRawNFCData(UINT iIndex, const char* pMessageText)
{
	return 0;
}

int agk::GetRawNFCDataState(UINT iIndex)
{
	return 0;
}

char* agk::GetRawNFCData(UINT iIndex)
{
	return 0;
}

void agk::StartGPSTracking()
{
	if ( m_iGPSSensorExists )
    {
        g_pLocationManager.desiredAccuracy = kCLLocationAccuracyBest;
        g_pLocationManager.distanceFilter = kCLDistanceFilterNone;
        [ g_pLocationManager startUpdatingLocation ];
    }
}

void agk::StopGPSTracking()
{
	[ g_pLocationManager stopUpdatingLocation ];
}

float agk::GetRawGPSLatitude()
{
	return m_fGPSLat;
}

float agk::GetRawGPSLongitude()
{
	return m_fGPSLong;
}

float agk::GetRawGPSAltitude()
{
	return m_fGPSAltitude;
}

// GameCenter

int agk::GetGameCenterExists()
{
	return 0;
}

void agk::GameCenterSetup()
{
	
}

void agk::GameCenterLogin()
{
	
}

void agk::GameCenterLogout()
{
	
}

int agk::GetGameCenterLoggedIn()
{
	return 0;
}

char* agk::GetGameCenterPlayerID()
{
	char *str = new char[1]; *str = 0;
	return str;
}

char* agk::GetGameCenterPlayerDisplayName()
{
	char *str = new char[1]; *str = 0;
	return str;
}

void agk::GameCenterSubmitScore( int iScore, const char* szBoardID )
{
	
}

void agk::GameCenterShowLeaderBoard ( const char* szBoardID )
{
	
}

void agk::GameCenterSubmitAchievement ( const char* szAchievementID, int iPercentageComplete )
{
	
}

void agk::GameCenterAchievementsShow ( void )
{
	
}

void agk::GameCenterAchievementsReset ( void )
{
	
}


int agk::PlatformGetIP( uString &sIP )
{
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    
    sIP.SetStr("");
        
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while (temp_addr != NULL) {
            if( temp_addr->ifa_addr->sa_family == AF_INET) {
                const char* szIP = inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr);
                if ( strcmp(szIP,"127.0.0.1") != 0 ) {
                    sIP.SetStr( szIP );
                    break;
                }
            }
            
            temp_addr = temp_addr->ifa_next;
        }
    }
    
    // Free memory
    freeifaddrs(interfaces);
    
    if ( sIP.GetLength() == 0 ) 
	{
		sIP.SetStr( "127.0.0.1" );
		//return 0;
	}
	
	return 1;
}

int agk::PlatformGetIPv6( uString &sIP, int *iInterface )
{
    struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    
    sIP.SetStr("");
    int currLevel = 0;
    
    char szName[16]; 
    *szName = 0;
        
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while (temp_addr != NULL) {
			if( temp_addr->ifa_addr->sa_family == AF_INET6) {
                char szIP[ 65 ];
				inet_ntop( AF_INET6, &(((struct sockaddr_in6*)temp_addr->ifa_addr)->sin6_addr), szIP, 65 );
                int level = 0;
                if ( strncmp( szIP, "::1", 3 ) == 0 ) level = -1; // loopback
                else if ( strncmp( szIP, "ff", 2 ) == 0 ) level = -1; // multicast address
                else if ( strncmp( szIP, "fe", 2 ) == 0 ) level = 1; // link local address
                else if ( strncmp( szIP, "fc", 2 ) == 0 ) level = 2; // site local address
                else if ( strncmp( szIP, "fd", 2 ) == 0 ) level = 2; // site local address
                else level = 3;
                
                //NSLog( @"  Addr: %s, Level: %d", szIP, level );
                
                if ( level > currLevel )
                {
                    currLevel = level;
                    strcpy(szName, temp_addr->ifa_name);
                    sIP.SetStr( szIP );
                }
            }
            
            temp_addr = temp_addr->ifa_next;
        }
    }
    
    // Free memory
    freeifaddrs(interfaces);
    
    if ( sIP.GetLength() == 0 ) 
	{
		sIP.SetStr( "::1" );
		return 1;
	}
    
    if ( iInterface ) 
    {
		*iInterface = if_nametoindex( szName );
    }
    
    return 1;
}

int agk::CheckPermission( const char* szPermission )
{
	return 2;
}

void agk::RequestPermission( const char* szPermission )
{

}

// Cloud data

void agk::SetupCloudData( const char* reserved )
{

}

int agk::GetCloudDataAllowed()
{
	return -2;
}

int agk::GetCloudDataChanged()
{
	return 0;
}

char* agk::GetCloudDataVariable( const char* varName, const char* defaultValue )
{
	char *str = new char[1]; *str = 0;
	return str;
}

void agk::SetCloudDataVariable( const char* varName, const char* varValue )
{
	
}

void agk::DeleteCloudDataVariable( const char* varName )
{
	
}

void agk::SetSharedVariableAppGroup( const char* group )
{
	
}

void agk::SaveSharedVariable( const char *varName, const char *varValue )
{
	
}

char* agk::LoadSharedVariable( const char *varName, const char *defaultValue )
{
	if ( !defaultValue )
	{
		char *str = new char[1];
		*str = 0;
		return str;
	}
	else
	{
		char *str = new char[ strlen(defaultValue)+1 ];
		strcpy( str, defaultValue );
		return str;
	}
}

void agk::DeleteSharedVariable( const char *varName )
{
	
}

void agk::FirebaseSetup()
{
	
}

void agk::FirebaseLogEvent( const char *event_name )
{
	
}

int AGKFont::PlatformGetSystemFontPath( const uString &sFontName, uString &sOut )
{
	return 0;
}

// AR commands

void agk::ARSetup()
//****
{

}

int agk::ARGetStatus()
//****
{
	return -1;
}

// not a Tier 1 command
void agk::ARUpdateInternal()
{

}

// not a Tier 1 command
void agk::ARPause()
{
	
}

// not a Tier 1 command
void agk::ARResume()
{
	
}

void agk::ARDestroy()
//****
{
	
}

void agk::ARControlCamera()
//****
{
	
}

void agk::ARDrawBackground()
//****
{
	
}

void agk::ARSetPlaneDetectionMode( int mode )
//****
{
	
}

void agk::ARSetLightEstimationMode( int mode )
//****
{

}

float agk::ARGetLightEstimate()
//****
{
	return 0;
}

int agk::ARHitTest( float screenX, float screenY )
//****
{
	return 0;
}

float agk::ARGetHitTestX( int index )
//****
{
	return 0;
}

float agk::ARGetHitTestY( int index )
//****
{
	return 0;
}

float agk::ARGetHitTestZ( int index )
//****
{
	return 0;
}

float agk::ARGetHitTestNormalX( int index )
//****
{
	return 0;
}

float agk::ARGetHitTestNormalY( int index )
//****
{
	return 0;
}

float agk::ARGetHitTestNormalZ( int index )
//****
{
	return 0;
}

int agk::ARGetHitTestType( int index )
//****
{
	return 0;
}

void agk::ARHitTestFinish()
//****
{
	
}

int agk::ARGetPlanes( int reserved )
//****
{
	return 0;
}

float agk::ARGetPlaneX( int index )
//****
{
	return 0;
}

float agk::ARGetPlaneY( int index )
//****
{
	return 0;
}

float agk::ARGetPlaneZ( int index )
//****
{
	return 0;
}

float agk::ARGetPlaneAngleX( int index )
//****
{
	return 0;
}

float agk::ARGetPlaneAngleY( int index )
//****
{
	return 0;
}

float agk::ARGetPlaneAngleZ( int index )
//****
{
	return 0;
}

float agk::ARGetPlaneSizeX( int index )
//****
{
	return 0;
}

float agk::ARGetPlaneSizeZ( int index )
//****
{
	return 0;
}

void agk::ARGetPlanesFinish()
//****
{
	
}

int agk::ARCreateAnchorFromHitTest( int index )
//****
{
	return 0;
}

int agk::ARCreateAnchorFromPlane( int index )
//****
{
	return 0;
}

void agk::ARFixObjectToAnchor( int objID, int anchorID )
//****
{
	
}

int agk::ARGetAnchorStatus( int anchorID )
//****
{
	return 0;
}

void agk::ARDeleteAnchor( int anchorID )
//****
{
	
}

int agk::GetAppInstalled( const char *packageName )
//****
{
	return 0;
}

// SnapChat

void agk::SetSnapChatStickerSettings( float x, float y, int width, int height, float angle )
//****
{

}

void agk::ShareSnapChatImage( const char* imageFile, const char* stickerFile, const char* caption, const char* url )
//****
{

}
