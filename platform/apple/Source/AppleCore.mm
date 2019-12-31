#include "AGK.h"
#include "OpenGLES2.h"

#import <sys/utsname.h>
#include <deque>

#include "zlib.h"
#include "SocialPlugins.h"
#include "MediaPlayer/MediaPlayer.h"

#import <GameKit/GameKit.h>
#import <CoreMotion/CoreMotion.h>
#import <CoreLocation/CoreLocation.h>
#import <CoreLocation/CLLocationManagerDelegate.h>

#include "png.h"
#include "jpeglib.h"

#ifndef LITEVERSION
    #import <FirebaseAnalytics/FIRApp.h>
    #import <FirebaseAnalytics/FIRAnalytics.h>
    #import "FacebookSDK/FBAppEvents.h"

  #if !defined(__i386__) && !defined(__x86_64__)
    #include <SCSDKCreativeKit/SCSDKCreativeKit.h>
  #endif
#endif

#import <SystemConfiguration/SystemConfiguration.h>

#include <net/if.h>

#import <ARKit/ARKit.h>
#import <WebKit/WKWebView.h>
#import <WebKit/WKWebViewConfiguration.h>
#import <WebKit/WKNavigationDelegate.h>

extern unsigned char libImageAscii[];
extern unsigned char libImageAsciiExt[];

int agk::m_iKeyboardMode = 2; // virtual keyboard

using namespace AGK;

extern SocialPlugins* g_pSocialPlugins;

//thread class
@interface iOSThread : NSThread
{
	@public
	AGKThread *pThread;
	NSConditionLock *finished;
}

-(iOSThread*) initWithThread:(AGKThread*)agkthread;

@end

@implementation iOSThread

-(iOSThread*)initWithThread:(AGKThread*)agkthread
{
	self = [super init];
    finished = 0;
	if ( self )
	{
		pThread = agkthread; 
		finished = [[NSConditionLock alloc] initWithCondition: 1 ]; 
	}
	
	return self;
}

-(void) dealloc
{
	if ( finished ) [ finished release ]; 
    finished = 0;
    [super dealloc];
}

-(void) main 
{ 
	NSAutoreleasePool *pool;
    pool = [[NSAutoreleasePool alloc] init];
	
	AGKThread::EntryPoint(pThread);
	[ finished lock ];
	[ finished unlockWithCondition: 1 ];
	
	[pool drain];
}

@end

extern "C" {
FILE* AGKfopen( const char *szPath, const char* mode )
{
	FILE* pFile = fopen( szPath, mode );
	return pFile;
}
}

namespace AGK
{   
    UIViewController *g_pViewController = 0;
	UIView *g_pMainWindow = 0;
    WKWebView *youtubeView = 0;
	
    float m_fDeviceScale = 1.0f;
	int g_iImmersiveMode = 0;
    
	double dStartTime = 0;
	double dCurrentTime = 0;

	// music
	AVAudioPlayer *musicPlayer;	
	UInt32 audioIsAlreadyPlaying;
	
	// sounds
	ALCcontext	*audioContext = NULL;
	ALCdevice	*audioDevice = NULL;
    
    // video
    //MPMoviePlayerController *player = 0;
    AVPlayer *videoplayer = 0;
    AVPlayerLayer *videoLayer = 0;
    AVPlayerItemVideoOutput *videoOutput = 0;
    uString g_sVideoFile;
    int g_iHasVideoStarted = 0;
    int g_iVideoOutputAttached = 0;
    
    // device camera to image
    AVCaptureSession *cameraSession = 0;
    AVCaptureVideoDataOutput *cameraOutput = 0;
        
    // motion
    CMMotionManager *g_pMotionManager = 0;
    CLLocationManager *g_pLocationManager = 0;
    
    // gamecenter
    int m_iGameCenterLogin = 0;
    
    // shared variables
    uString m_sSharedAppGroup;
    
    // cloud data
    int g_iCloudDataChanged = 0;
    int g_iCloudSetup = 0;
    
    uString g_sLastURLSchemeText;
    
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
		
			ALuint			bufferID;		
			ALuint			sourceID;			//base source ID
		
			cSoundInst() 
			{ 
				m_uLastUsed = 0;
				m_iParent = 0;
				m_iVolume = 100;
				m_iLoop = 0;
                m_fRate = 1.0f;
                m_fBalance = 0;
                m_iLoopCount = 0;
				m_pPrevInst = UNDEF;
				m_pNextInst = UNDEF;
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
	UITextField *pTextField = 0;
    UITextView *pTextView = 0;
	cSprite *pTextBackground = 0;
    bool g_bEditBoxHack = false;
    bool g_bMultiline = false;
    int g_iInputType = 0;
    bool g_bPasswordMode = false;
}

void SetSystemTextBox( const char* text )
{
    NSString *pString = [NSString stringWithCString:text encoding:NSUTF8StringEncoding];
    if ( g_bMultiline ) [pTextView setText:pString];
    else [pTextField setText:pString];
}

@interface EventListener : NSObject <UITextViewDelegate, UITextFieldDelegate>
{
@public
	uString m_sText;
	bool m_bCancelled;
	bool m_bFinished;
    bool m_bSkipResign;
}

@end

@implementation EventListener
- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [ pTextField resignFirstResponder ];
	[ pTextField removeFromSuperview ];
	if ( !m_bSkipResign ) m_bCancelled = false;
    m_bSkipResign = false;
	//NSLog( @"Should Return" );
	return YES;
}

- (void)textFieldDidEndEditing:(UITextView *)textField
{
	[ pTextField resignFirstResponder ];
	[ pTextField removeFromSuperview ];
	m_sText.SetStr( [textField.text cStringUsingEncoding:NSUTF8StringEncoding ] );

	if ( !m_bSkipResign )
    {
        m_bFinished = true;
        g_bMultiline = false;
        g_iInputType = 0;
    }
    m_bSkipResign = false;
}

- (void)textViewDidEndEditing:(UITextView *)textView
{
	[ pTextView resignFirstResponder ];
	[ pTextView removeFromSuperview ];
	m_sText.SetStr( [pTextView.text cStringUsingEncoding:NSUTF8StringEncoding ] );
    
    if ( !m_bSkipResign )
    {
        m_bFinished = true;
        g_bMultiline = false;
        g_iInputType = 0;
    }
    m_bSkipResign = false;
}
@end

@interface AGKImagePicker : NSObject <UIImagePickerControllerDelegate, UINavigationControllerDelegate, UIPopoverControllerDelegate>
{
@public
	UIImagePickerController* imagePickerController;
    UIPopoverController* popoverController;
	bool m_bCancelled;
	bool m_bFinished;
    AGK::cImage *m_pImage;
}

- (void) pickImage: (AGK::cImage*) pImage;
@end

@implementation AGKImagePicker
- (UIImage *)scaleAndRotateImage:(UIImage *)image {
    int kMaxResolution = 1024; // Or whatever
    
    CGImageRef imgRef = image.CGImage;
    
    CGFloat width = CGImageGetWidth(imgRef);
    CGFloat height = CGImageGetHeight(imgRef);
    
    
    CGAffineTransform transform = CGAffineTransformIdentity;
    CGRect bounds = CGRectMake(0, 0, width, height);
    if (width > kMaxResolution || height > kMaxResolution) {
        CGFloat ratio = width/height;
        if (ratio > 1) {
            bounds.size.width = kMaxResolution;
            bounds.size.height = roundf(bounds.size.width / ratio);
        }
        else {
            bounds.size.height = kMaxResolution;
            bounds.size.width = roundf(bounds.size.height * ratio);
        }
    }
    
    CGFloat scaleRatio = bounds.size.width / width;
    CGSize imageSize = CGSizeMake(CGImageGetWidth(imgRef), CGImageGetHeight(imgRef));
    CGFloat boundHeight;
    UIImageOrientation orient = image.imageOrientation;
    switch(orient) {
            
        case UIImageOrientationUp: //EXIF = 1
            transform = CGAffineTransformIdentity;
            break;
            
        case UIImageOrientationUpMirrored: //EXIF = 2
            transform = CGAffineTransformMakeTranslation(imageSize.width, 0.0);
            transform = CGAffineTransformScale(transform, -1.0, 1.0);
            break;
            
        case UIImageOrientationDown: //EXIF = 3
            transform = CGAffineTransformMakeTranslation(imageSize.width, imageSize.height);
            transform = CGAffineTransformRotate(transform, M_PI);
            break;
            
        case UIImageOrientationDownMirrored: //EXIF = 4
            transform = CGAffineTransformMakeTranslation(0.0, imageSize.height);
            transform = CGAffineTransformScale(transform, 1.0, -1.0);
            break;
            
        case UIImageOrientationLeftMirrored: //EXIF = 5
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(imageSize.height, imageSize.width);
            transform = CGAffineTransformScale(transform, -1.0, 1.0);
            transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
            break;
            
        case UIImageOrientationLeft: //EXIF = 6
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(0.0, imageSize.width);
            transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
            break;
            
        case UIImageOrientationRightMirrored: //EXIF = 7
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeScale(-1.0, 1.0);
            transform = CGAffineTransformRotate(transform, M_PI / 2.0);
            break;
            
        case UIImageOrientationRight: //EXIF = 8
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(imageSize.height, 0.0);
            transform = CGAffineTransformRotate(transform, M_PI / 2.0);
            break;
            
        default:
            [NSException raise:NSInternalInconsistencyException format:@"Invalid image orientation"];
            
    }
    
    UIGraphicsBeginImageContext(bounds.size);
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    if (orient == UIImageOrientationRight || orient == UIImageOrientationLeft) {
        CGContextScaleCTM(context, -scaleRatio, scaleRatio);
        CGContextTranslateCTM(context, -height, 0);
    }
    else {
        CGContextScaleCTM(context, scaleRatio, -scaleRatio);
        CGContextTranslateCTM(context, 0, -height);
    }
    
    CGContextConcatCTM(context, transform);
    
    CGContextDrawImage(UIGraphicsGetCurrentContext(), CGRectMake(0, 0, width, height), imgRef);
    UIImage *imageCopy = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return imageCopy;
}

- (void) pickImage: (AGK::cImage*) pImage
{
    m_pImage = pImage;
    
    if ( imagePickerController ) return;
    
    if ( [UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypePhotoLibrary] == NO )
    {
        agk::Message("Photo album unavailable");
        m_pImage = 0;
        agk::ChooseInternal(0);
        return;
    }
    
    popoverController = 0;
    imagePickerController = [ [ UIImagePickerController alloc ] init ];
    imagePickerController.delegate = self;
    imagePickerController.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    imagePickerController.allowsEditing = NO;
    
    // New way
    [g_pViewController presentViewController:imagePickerController animated:YES completion:nil];
    [g_pViewController setInactive];
    
    /*
    // Old way, no longer works since iOS 8
    UIWindow *window = [[UIApplication sharedApplication] keyWindow];
    if ( !window ) agk::Warning( "Could not find main window" ); 
    else 
    {
        if ( [[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad )
        {
            UIView *view = [window viewWithTag:0];
            popoverController = [[UIPopoverController alloc] initWithContentViewController:imagePickerController];
            CGSize size;
            size.width = view.frame.size.width - 100;
            size.height = view.frame.size.height - 100;
            popoverController.delegate = self;
            [popoverController retain];
            [ popoverController presentPopoverFromRect:CGRectMake(view.frame.size.width/2,0,1,1) inView:view permittedArrowDirections:UIPopoverArrowDirectionAny animated:NO];
            //[popoverController setPopoverContentSize:size animated:NO ];
        }
        else
        {
            [ window addSubview:imagePickerController.view ];
            [ window bringSubviewToFront: imagePickerController.view ];
        }
    }
    */
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    if ( !m_pImage ) 
    {
        if ( imagePickerController )
        {
            [g_pViewController dismissViewControllerAnimated:YES completion:^{
                [ imagePickerController release ];
                imagePickerController = 0;
            }];
            
            [g_pViewController setActive];
        }
        if ( popoverController )
        {
            [ popoverController dismissPopoverAnimated:YES ];
            [ popoverController release ];
            popoverController = 0;
        }
        return;
    }
    
    UIImage *image = (UIImage *)[info objectForKey:@"UIImagePickerControllerOriginalImage"];
    UIImage *image2 = [self scaleAndRotateImage:image ];
    NSData *data = [NSData dataWithData:UIImagePNGRepresentation(image2)];
    
    uString sPath;
    sPath.SetStr( "/AGK Chosen Image.png" );
    agk::PlatformGetFullPathWrite( sPath );
    NSString *pWriteDir = [ [ NSString alloc ] initWithUTF8String:sPath.GetStr() ];
    [ data writeToFile:pWriteDir atomically:YES];
    [ pWriteDir release ];
    m_pImage->Load( "/AGK Chosen Image.png" );
    
    if ( imagePickerController )
    {
        [g_pViewController dismissViewControllerAnimated:YES completion:^{
            [ imagePickerController release ];
            imagePickerController = 0;
        }];
         
        [g_pViewController setActive];
    }
    
    if ( popoverController )
    {
        [ popoverController dismissPopoverAnimated:YES ];
        [ popoverController release ];
        popoverController = 0;
    }
    
    agk::ChooseInternal(1);
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    if ( imagePickerController )
    {
        [g_pViewController dismissViewControllerAnimated:YES completion:^{
            [ imagePickerController release ];
            imagePickerController = 0;
        }];
        
        [g_pViewController setActive];
    }
    
    if ( popoverController )
    {
        [ popoverController dismissPopoverAnimated:YES ];
        [ popoverController release ];
        popoverController = 0;
    }
    
    m_pImage = 0;
    agk::ChooseInternal(0);
}

- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController2
{
    if ( imagePickerController )
    {
        [ imagePickerController.view removeFromSuperview ];
        [ imagePickerController release ];
        imagePickerController = 0;
    }
    
    [ popoverController release ];
    
    m_pImage = 0;
    agk::ChooseInternal(0);
}
@end

@interface AGKCameraPicker : NSObject <UIImagePickerControllerDelegate, UINavigationControllerDelegate>
{
@public
	UIImagePickerController* imagePickerController;
	bool m_bCancelled;
	bool m_bFinished;
    AGK::cImage *m_pImage;
    //UIView *videoPreviewView;
    //AVCaptureVideoPreviewLayer *captureVideoPreviewLayer;
    //AVCaptureSession *captureSession;
    //AVCaptureStillImageOutput *stillImageOutput;
    //UIButton *takePhoto;
}

- (void) pickImage: (AGK::cImage*) pImage;
@end


@implementation AGKCameraPicker
- (UIImage *)scaleAndRotateImage:(UIImage *)image {
    int kMaxResolution = 1024; // Or whatever
    
    CGImageRef imgRef = image.CGImage;
    
    CGFloat width = CGImageGetWidth(imgRef);
    CGFloat height = CGImageGetHeight(imgRef);
    
    
    CGAffineTransform transform = CGAffineTransformIdentity;
    CGRect bounds = CGRectMake(0, 0, width, height);
    if (width > kMaxResolution || height > kMaxResolution) {
        CGFloat ratio = width/height;
        if (ratio > 1) {
            bounds.size.width = kMaxResolution;
            bounds.size.height = roundf(bounds.size.width / ratio);
        }
        else {
            bounds.size.height = kMaxResolution;
            bounds.size.width = roundf(bounds.size.height * ratio);
        }
    }
    
    CGFloat scaleRatio = bounds.size.width / width;
    CGSize imageSize = CGSizeMake(CGImageGetWidth(imgRef), CGImageGetHeight(imgRef));
    CGFloat boundHeight;
    UIImageOrientation orient = image.imageOrientation;
    switch(orient) {
            
        case UIImageOrientationUp: //EXIF = 1
            transform = CGAffineTransformIdentity;
            break;
            
        case UIImageOrientationUpMirrored: //EXIF = 2
            transform = CGAffineTransformMakeTranslation(imageSize.width, 0.0);
            transform = CGAffineTransformScale(transform, -1.0, 1.0);
            break;
            
        case UIImageOrientationDown: //EXIF = 3
            transform = CGAffineTransformMakeTranslation(imageSize.width, imageSize.height);
            transform = CGAffineTransformRotate(transform, M_PI);
            break;
            
        case UIImageOrientationDownMirrored: //EXIF = 4
            transform = CGAffineTransformMakeTranslation(0.0, imageSize.height);
            transform = CGAffineTransformScale(transform, 1.0, -1.0);
            break;
            
        case UIImageOrientationLeftMirrored: //EXIF = 5
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(imageSize.height, imageSize.width);
            transform = CGAffineTransformScale(transform, -1.0, 1.0);
            transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
            break;
            
        case UIImageOrientationLeft: //EXIF = 6
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(0.0, imageSize.width);
            transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
            break;
            
        case UIImageOrientationRightMirrored: //EXIF = 7
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeScale(-1.0, 1.0);
            transform = CGAffineTransformRotate(transform, M_PI / 2.0);
            break;
            
        case UIImageOrientationRight: //EXIF = 8
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(imageSize.height, 0.0);
            transform = CGAffineTransformRotate(transform, M_PI / 2.0);
            break;
            
        default:
            [NSException raise:NSInternalInconsistencyException format:@"Invalid image orientation"];
            
    }
    
    UIGraphicsBeginImageContext(bounds.size);
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    if (orient == UIImageOrientationRight || orient == UIImageOrientationLeft) {
        CGContextScaleCTM(context, -scaleRatio, scaleRatio);
        CGContextTranslateCTM(context, -height, 0);
    }
    else {
        CGContextScaleCTM(context, scaleRatio, -scaleRatio);
        CGContextTranslateCTM(context, 0, -height);
    }
    
    CGContextConcatCTM(context, transform);
    
    CGContextDrawImage(UIGraphicsGetCurrentContext(), CGRectMake(0, 0, width, height), imgRef);
    UIImage *imageCopy = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return imageCopy;
}

- (void) pickImage: (AGK::cImage*) pImage
{
    m_pImage = pImage;
    
    if ( imagePickerController ) return;
    
    imagePickerController = [ [ UIImagePickerController alloc ] init ];
    imagePickerController.delegate = self;
    imagePickerController.sourceType = UIImagePickerControllerSourceTypeCamera;
    imagePickerController.allowsEditing = NO;
    
    [g_pViewController presentViewController:imagePickerController animated:YES completion:nil];
    [g_pViewController setInactive];
    
    /*
    if ( [[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPad )
    {
        [g_pViewController addChildViewController:imagePickerController];
        [g_pViewController.view addSubview:imagePickerController.view];
    }
    else
    {
        UIWindow *window = [[UIApplication sharedApplication] keyWindow];
        if ( !window ) agk::Warning( "Could not find main window" );
        else
        {
            [ window addSubview:imagePickerController.view ];
            [ window bringSubviewToFront: imagePickerController.view ];
        }
    }
     */
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    if ( !m_pImage ) 
    {
        if ( imagePickerController )
        {
            [g_pViewController dismissViewControllerAnimated:YES completion:^{
                [ imagePickerController release ];
                imagePickerController = 0;
            }];
            
            [g_pViewController setActive];
        }
        return;
    }
    
    UIImage *image = (UIImage *)[info objectForKey:@"UIImagePickerControllerOriginalImage"];
    UIImage *image2 = [self scaleAndRotateImage:image];
    NSData *data = [NSData dataWithData:UIImagePNGRepresentation(image2)];
    
    uString sPath;
    sPath.SetStr( "/AGK Chosen Image.png" );
    agk::PlatformGetFullPathWrite( sPath );
    NSString *pWriteDir = [ [ NSString alloc ] initWithUTF8String:sPath.GetStr() ];
    [ data writeToFile:pWriteDir atomically:YES];
    [ pWriteDir release ];
    m_pImage->Load( "/AGK Chosen Image.png" );
    
    if ( imagePickerController )
    {
        [g_pViewController dismissViewControllerAnimated:YES completion:^{
            [ imagePickerController release ];
            imagePickerController = 0;
        }];
        
        [g_pViewController setActive];
    }
    
    agk::CaptureInternal(1);
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    if ( imagePickerController )
    {
        [g_pViewController dismissViewControllerAnimated:YES completion:^{
            [ imagePickerController release ];
            imagePickerController = 0;
        }];
        
        [g_pViewController setActive];
    }
    
    m_pImage = 0;
    agk::CaptureInternal(0);
}
@end

/*
@implementation AGKCameraPicker
- (UIImage *)scaleAndRotateImage:(UIImage *)image {
    int kMaxResolution = 2048; // Or whatever
    
    CGImageRef imgRef = image.CGImage;
    
    CGFloat width = CGImageGetWidth(imgRef);
    CGFloat height = CGImageGetHeight(imgRef);
    
    
    CGAffineTransform transform = CGAffineTransformIdentity;
    CGRect bounds = CGRectMake(0, 0, width, height);
    if (width > kMaxResolution || height > kMaxResolution) {
        CGFloat ratio = width/height;
        if (ratio > 1) {
            bounds.size.width = kMaxResolution;
            bounds.size.height = roundf(bounds.size.width / ratio);
        }
        else {
            bounds.size.height = kMaxResolution;
            bounds.size.width = roundf(bounds.size.height * ratio);
        }
    }
    
    CGFloat scaleRatio = bounds.size.width / width;
    CGSize imageSize = CGSizeMake(CGImageGetWidth(imgRef), CGImageGetHeight(imgRef));
    CGFloat boundHeight;
    UIImageOrientation orient = image.imageOrientation;
    switch(orient) {
            
        case UIImageOrientationUp: //EXIF = 1
            transform = CGAffineTransformIdentity;
            break;
            
        case UIImageOrientationUpMirrored: //EXIF = 2
            transform = CGAffineTransformMakeTranslation(imageSize.width, 0.0);
            transform = CGAffineTransformScale(transform, -1.0, 1.0);
            break;
            
        case UIImageOrientationDown: //EXIF = 3
            transform = CGAffineTransformMakeTranslation(imageSize.width, imageSize.height);
            transform = CGAffineTransformRotate(transform, M_PI);
            break;
            
        case UIImageOrientationDownMirrored: //EXIF = 4
            transform = CGAffineTransformMakeTranslation(0.0, imageSize.height);
            transform = CGAffineTransformScale(transform, 1.0, -1.0);
            break;
            
        case UIImageOrientationLeftMirrored: //EXIF = 5
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(imageSize.height, imageSize.width);
            transform = CGAffineTransformScale(transform, -1.0, 1.0);
            transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
            break;
            
        case UIImageOrientationLeft: //EXIF = 6
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(0.0, imageSize.width);
            transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0);
            break;
            
        case UIImageOrientationRightMirrored: //EXIF = 7
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeScale(-1.0, 1.0);
            transform = CGAffineTransformRotate(transform, M_PI / 2.0);
            break;
            
        case UIImageOrientationRight: //EXIF = 8
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(imageSize.height, 0.0);
            transform = CGAffineTransformRotate(transform, M_PI / 2.0);
            break;
            
        default:
            [NSException raise:NSInternalInconsistencyException format:@"Invalid image orientation"];
            
    }
    
    UIGraphicsBeginImageContext(bounds.size);
    
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    if (orient == UIImageOrientationRight || orient == UIImageOrientationLeft) {
        CGContextScaleCTM(context, -scaleRatio, scaleRatio);
        CGContextTranslateCTM(context, -height, 0);
    }
    else {
        CGContextScaleCTM(context, scaleRatio, -scaleRatio);
        CGContextTranslateCTM(context, 0, -height);
    }
    
    CGContextConcatCTM(context, transform);
    
    CGContextDrawImage(UIGraphicsGetCurrentContext(), CGRectMake(0, 0, width, height), imgRef);
    UIImage *imageCopy = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return imageCopy;
}

- (void) pickImage: (AGK::cImage*) pImage
{
    m_pImage = pImage;
    
    //UIWindow *window = [[UIApplication sharedApplication] keyWindow];
    //if ( !window ) agk::Warning( "Could not find main window" ); 
    //else 
    {
        if ( !videoPreviewView ) videoPreviewView = [[UIView alloc] initWithFrame:g_pViewController.view.frame];
        
        captureSession = [[AVCaptureSession alloc] init];
        captureSession.sessionPreset = AVCaptureSessionPresetPhoto;
        
        captureVideoPreviewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:captureSession];
        
        captureVideoPreviewLayer.frame = videoPreviewView.bounds;
        [videoPreviewView.layer addSublayer:captureVideoPreviewLayer];
        
        AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
        
        NSError *error = nil;
        AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:device error:&error];
        if (!input) {
            // Handle the error appropriately.
            NSLog(@"ERROR: trying to open camera: %@", error);
        }
        
        if ([device isFocusPointOfInterestSupported] && [device isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus])
        {
            if ([device lockForConfiguration:&error]) {
                [device setFocusPointOfInterest:CGPointMake(0.5f,0.5f)];
                [device setFocusMode:AVCaptureFocusModeContinuousAutoFocus];
                [device unlockForConfiguration];
            }
        }
        
        [captureSession addInput:input];
        
        stillImageOutput = [[AVCaptureStillImageOutput alloc] init];
        NSDictionary *outputSettings = [[NSDictionary alloc] initWithObjectsAndKeys:
                                        AVVideoCodecJPEG, AVVideoCodecKey,
                                        nil];
        [stillImageOutput setOutputSettings:outputSettings];
        [outputSettings release];
        
        [ captureSession addOutput:stillImageOutput ];
        
        [captureSession startRunning];
        
        [g_pViewController.view addSubview:videoPreviewView ];
        
        takePhoto = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        [takePhoto setTitle:@"Take Photo" forState:UIControlStateNormal];
        takePhoto.frame = CGRectMake(g_pViewController.view.frame.size.width/2 - 50, g_pViewController.view.frame.size.height-30, 100, 30);
        [takePhoto addTarget:self action:@selector(capturePhoto) forControlEvents:UIControlEventTouchUpInside];
        [videoPreviewView addSubview:takePhoto];
        [videoPreviewView bringSubviewToFront:takePhoto];
    }
}

-(void) capturePhoto
{
    NSLog( @"Take Photo" );
    
    AVCaptureConnection *videoConnection = nil;
	for (AVCaptureConnection *connection in stillImageOutput.connections)
	{
		for (AVCaptureInputPort *port in [connection inputPorts])
		{
			if ([[port mediaType] isEqual:AVMediaTypeVideo] )
			{
				videoConnection = connection;
				break;
			}
		}
		if (videoConnection) { break; }
	}
    
    if ( !videoConnection ) 
    {
        NSLog( @"Failed to find video connection" );
        return;
    }
    
	NSLog(@"about to request a capture from: %@", stillImageOutput);
	[stillImageOutput captureStillImageAsynchronouslyFromConnection:videoConnection completionHandler: ^(CMSampleBufferRef imageSampleBuffer, NSError *error)
     {
		 NSData *imageData = [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:imageSampleBuffer];
         UIImage *image = [[UIImage alloc] initWithData:imageData];
         UIImage *image2 = [self scaleAndRotateImage:image];
         
         m_pImage->Load((const char*)image2);
         [image release];
         
         [videoPreviewView removeFromSuperview];
         agk::CaptureInternal(1);
	 }];
}

- (void)cancel
{
    [videoPreviewView removeFromSuperview];
    
    m_pImage = 0;
    agk::CaptureInternal(0);
}

-(void) dealloc
{
    if ( captureVideoPreviewLayer ) [ captureVideoPreviewLayer release ];
    if ( videoPreviewView ) [ videoPreviewView release ];
    if ( captureSession ) [ captureSession release ];
    if ( stillImageOutput ) [ stillImageOutput release ];
    
    captureVideoPreviewLayer = 0;
    videoPreviewView = 0;
    captureSession = 0;
    stillImageOutput = 0;
    
    [super dealloc];
}
@end
*/

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

/*
-(void)playbackFinished:(NSNotification*)aNotification
{
    g_iHasVideoStarted = 0;
    MPMoviePlayerController *moviePlayer = [aNotification object];
    [moviePlayer.view removeFromSuperview];
}
*/

@end


@interface AGKVideoCaptureDelegate : NSObject
{
@public
    
}
- (void)captureOutput:(AVCaptureOutput *)captureOutput
        didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection *)connection;

- (void)captureOutput:(AVCaptureOutput *)captureOutput
  didDropSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection *)connection;
@end

@implementation AGKVideoCaptureDelegate

- (void)captureOutput:(AVCaptureOutput *)captureOutput
        didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
        fromConnection:(AVCaptureConnection *)connection
{
    if ( !cameraSession ) return;
    if ( !cameraOutput ) return;
    if ( !HasCameraImageData() ) return;
    if ( !agk::GetDeviceCameraImage() ) return;
    
    CVImageBufferRef buffer = CMSampleBufferGetImageBuffer(sampleBuffer);
    
    if ( buffer == nil ) return;
    
    CopyCameraImageToAGKImage( buffer, agk::GetDeviceCameraImage() );
    
    // this crashes the app
    //CVBufferRelease( buffer );
}

- (void)captureOutput:(AVCaptureOutput *)captureOutput
        didDropSampleBuffer:(CMSampleBufferRef)sampleBuffer
        fromConnection:(AVCaptureConnection *)connection
{
    //NSLog( @"Camera dropped a frame" );
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
    if (abs((float)howRecent) < 15.0) {
        agk::GPS( (float) location.coordinate.longitude, (float) location.coordinate.latitude, (float) location.altitude );
    }
}

- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)location fromLocation:(CLLocation *)oldLocation
{
    NSDate* eventDate = location.timestamp;
    NSTimeInterval howRecent = [eventDate timeIntervalSinceNow];
    if (abs((float)howRecent) < 15.0) {
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
        case kCLAuthorizationStatusAuthorizedWhenInUse:
        case kCLAuthorizationStatusNotDetermined: agk::m_iGPSSensorExists = 1; break;
    }
}

@end

#import <ReplayKit/ReplayKit.h>

@interface AGKScreenRecorderDelegate : NSObject <RPScreenRecorderDelegate, RPPreviewViewControllerDelegate>{}@end
namespace AGK { AGKScreenRecorderDelegate *g_pScreenRecorderDelegate = 0; }
@implementation AGKScreenRecorderDelegate
- (void)screenRecorder:(RPScreenRecorder *)screenRecorder
didStopRecordingWithError:(NSError *)error
 previewViewController:(RPPreviewViewController *)previewViewController
{
    if(error)
    {
        agk::Warning( [error.localizedDescription UTF8String] );
    }
    
    if ( g_pScreenRecorderDelegate )
    {
        [g_pScreenRecorderDelegate release];
        g_pScreenRecorderDelegate = 0;
    }
}

- (void)previewControllerDidFinish:(RPPreviewViewController *)previewController
{
    if ( previewController ) [previewController dismissViewControllerAnimated:YES completion:nil];
    if ( g_pScreenRecorderDelegate )
    {
        [g_pScreenRecorderDelegate release];
        g_pScreenRecorderDelegate = 0;
    }
    [g_pViewController setActive];
}

- (void)StartRecording:(int)microphone
{
    RPScreenRecorder* recorder =  RPScreenRecorder.sharedRecorder;
    recorder.delegate = self;
    [recorder startRecordingWithMicrophoneEnabled:microphone?YES:NO handler:^(NSError *error)
    {
        if(error)
        {
            agk::Warning( [error.localizedDescription UTF8String] );
            [g_pScreenRecorderDelegate release];
            g_pScreenRecorderDelegate = 0;
        }
    }];
}

- (void)StopRecording
{
    RPScreenRecorder* recorder = RPScreenRecorder.sharedRecorder;
    
    [recorder stopRecordingWithHandler:^(RPPreviewViewController * previewViewController,
                                         NSError * error) {
        
        if(error)
        {
            agk::Error( [error.localizedDescription UTF8String] );
        }
        else if(previewViewController)
        {
            previewViewController.previewControllerDelegate = g_pScreenRecorderDelegate;
            previewViewController.modalPresentationStyle = UIModalPresentationFullScreen;
            
            [g_pViewController presentViewController:previewViewController animated:YES completion:nil];
            [g_pViewController setInactive];
        }
        
    }];
}
@end

#import <WatchConnectivity/WatchConnectivity.h>

class AGKWatchMessage
{
public:
    char *szData;
    AGKWatchMessage *m_pNext;
};

@interface AGKWatchDelegate : NSObject <WCSessionDelegate>
{
    int m_iSessionState;
    cLock *m_pLock;
    AGKWatchMessage *m_pMessages;
    AGKWatchMessage *m_pLastMessage;
}
@end
namespace AGK { AGKWatchDelegate *g_pWatchDelegate = 0; }
@implementation AGKWatchDelegate

- (id)init
{
    self = [super init];
    m_iSessionState = 0;
    m_pLock = new cLock();
    m_pMessages = 0;
    m_pLastMessage = 0;
    return self;
}

- (void) dealloc
{
    delete m_pLock;
    while( m_pMessages )
    {
        m_pLastMessage = m_pMessages;
        m_pMessages = m_pMessages->m_pNext;
        delete m_pLastMessage;
    }
    m_pLastMessage = 0;
    [super dealloc];
}

- (void) connect
{
    if ( m_iSessionState > 0 ) return;
    
    m_iSessionState = 1;
    
    if (![WCSession isSupported])
    {
        m_iSessionState = -1;
        return;
    }
    
    WCSession *session = [WCSession defaultSession];
    session.delegate = g_pWatchDelegate;
    [session activateSession];
}

- (void) session:(WCSession *)session activationDidCompleteWithState:(WCSessionActivationState)activationState error:(NSError *)error
{
    if ( activationState == WCSessionActivationStateActivated )
    {
        m_iSessionState = 2;
    }
    else
    {
        m_iSessionState = -2;
    }
}

- (void) sessionDidBecomeInactive:(WCSession *)session
{
    m_iSessionState = -2;
}

- (void) sessionDidDeactivate:(WCSession *)session
{
    m_iSessionState = 1;
    [[WCSession defaultSession] activateSession];
}

- (int) getState
{
    if ( m_iSessionState < 2 ) return m_iSessionState;
    else
    {
        if ( ![[WCSession defaultSession] isPaired] ) return -3;
        if ( ![[WCSession defaultSession] isWatchAppInstalled] ) return -4;
        
        return 2;
    }
}

- (void) sendJSON:(const char*)json
{
    if ( m_iSessionState != 2 )
    {
        agk::Warning( "Failed to send data to Apple watch as the connection wasn't ready" );
        return;
    }
    
    if ( ![[WCSession defaultSession] isWatchAppInstalled] )
    {
        return;
    }
    
    NSData *data = [NSData dataWithBytes:json length:strlen(json)];
    NSError *error = 0;
    NSDictionary *dataDict = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:&error];
    if ( !error )
    {
        [[WCSession defaultSession] transferUserInfo:dataDict];
    }
    else
    {
        agk::Warning( "Failed to send data to Apple watch, could not convert JSON to NSDictionary" );
    }
}

- (void) session:(WCSession *)session didReceiveUserInfo:(NSDictionary<NSString *,id> *)userInfo
{
    NSError *error = 0;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:userInfo options:kNilOptions error:&error];
    if ( error )
    {
        agk::Warning( "Failed to receive data from Apple watch, could not convert NSDictionary to JSON" );
        return;
    }
    NSString *pString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    const char* szJson = [pString UTF8String];
    
    cAutoLock autolock(m_pLock);
    
    AGKWatchMessage *pNew = new AGKWatchMessage();
    pNew->szData = new char[strlen(szJson)+1];
    strcpy( pNew->szData, szJson );
    pNew->m_pNext = 0;
    if( m_pLastMessage ) m_pLastMessage->m_pNext = pNew;
    else m_pMessages = pNew;
    m_pLastMessage = pNew;
    
    [pString release];
}

- (char*) getMessage
{
    cAutoLock autolock(m_pLock);
    
    if ( !m_pMessages ) return 0;
    
    AGKWatchMessage *pMsg = m_pMessages;
    m_pMessages = m_pMessages->m_pNext;
    if ( !m_pMessages ) m_pLastMessage = 0;
    
    char* result = pMsg->szData;
    delete pMsg;
    return result;
}
@end

// text to speech
namespace AGK
{
    AVSpeechSynthesizer *g_pTextToSpeech = 0;
    float g_fSpeechRate = AVSpeechUtteranceDefaultSpeechRate;
    AVSpeechSynthesisVoice *g_pSpeechVoice = nil;
}

struct AGKUtterance
{
    AVSpeechUtterance *utterance;
    float delay; // in seconds
};

@interface AGKSpeechDelegate : NSObject <AVSpeechSynthesizerDelegate>
{
    @public int m_iIsSpeaking;
    int m_iFinished;
    std::deque<AGKUtterance*> m_pUtterances;
    float m_fLastTime;
}
@end
namespace AGK { AGKSpeechDelegate *g_pSpeechDelegate = 0; }
@implementation AGKSpeechDelegate

- (id)init
{
    self = [super init];
    m_iIsSpeaking = 0;
    m_iFinished = 1;
    m_fLastTime = agk::Timer();
    return self;
}

- (void)addUtterance:(AGKUtterance*)utterance
{
    m_pUtterances.push_back(utterance);
    m_iIsSpeaking = 1;
}

- (void)update
{
    if ( !g_pTextToSpeech ) return;
    
    float timeDelta = agk::Timer() - m_fLastTime;
    m_fLastTime = agk::Timer();
    if ( timeDelta < 0 ) timeDelta = 0;
    
    if ( !m_iFinished ) return;
    
    if ( m_pUtterances.size() == 0 )
    {
        m_iIsSpeaking = 0;
        return;
    }
    
    AGKUtterance *pNextUtterance = m_pUtterances[0];
    if ( pNextUtterance->delay > 0 ) pNextUtterance->delay -= timeDelta;
    if ( pNextUtterance->delay <= 0 )
    {
        m_pUtterances.pop_front();
        m_iFinished = 0;
        [g_pTextToSpeech speakUtterance: pNextUtterance->utterance];
        [pNextUtterance->utterance release];
        delete pNextUtterance;
    }
}

- (void)stopAll
{
    m_iIsSpeaking = 0;
    m_iFinished = 1;
    
    for( int i = 0; i < m_pUtterances.size(); i++ )
    {
        [m_pUtterances[i]->utterance release];
        delete m_pUtterances[i];
    }
    m_pUtterances.clear();
    
    if ( g_pTextToSpeech ) [g_pTextToSpeech stopSpeakingAtBoundary:AVSpeechBoundaryImmediate];
}

- (void)speechSynthesizer:(AVSpeechSynthesizer *)synthesizer didCancelSpeechUtterance:(AVSpeechUtterance *)utterance
{
    m_iFinished = 1;
}

- (void)speechSynthesizer:(AVSpeechSynthesizer *)synthesizer didFinishSpeechUtterance:(AVSpeechUtterance *)utterance
{
    m_iFinished = 1;
}
@end

namespace AGK
{
	EventListener *g_EventListener = 0;
    AGKImagePicker *g_ImagePicker = 0;
    AGKCameraPicker *g_CameraPicker = 0;
    AGKMovieListener *g_pAGKMovieListener = 0;
    AGKLocationListener *g_pLocationListener = 0;
    AGKVideoCaptureDelegate *g_pVideoCaptureListener = 0;
}

// file entry class

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
				strcat( str, [ pString cStringUsingEncoding:NSUTF8StringEncoding ] );
				AddNewFile( str );
				
				strcat( str, "/" );
				TraverseDirectory( str );
				
			}
			else
			{
				// file
				char str[ 1024 ];
				strcpy( str, dir );
				strcat( str, [ pString cStringUsingEncoding:NSUTF8StringEncoding ] );
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
	// do nothing on ios
}

void agk::SetWindowSize( int width, int height, int fullscreen )
{
	SetWindowSize( width, height, fullscreen, 0 );
}

void agk::SetWindowSize( int width, int height, int fullscreen, int allowOverSized )
{
	// do nothing on ios
}

void agk::SetWindowAllowResize( int mode )
{
	// do nothing
}

void agk::MaximizeWindow()
{
	
}

void agk::MinimizeApp()
{
	// can this be done?
}

void agk::RestoreApp()
{
	// do nothing
}

void agk::SetImmersiveMode( int mode )
{
	g_iImmersiveMode = mode ? 1 : 0;
    if (@available(iOS 11.0, *))
    {
        if ([g_pViewController respondsToSelector:@selector(setNeedsUpdateOfHomeIndicatorAutoHidden)])
        {
            [ g_pViewController setNeedsUpdateOfHomeIndicatorAutoHidden ];
        }
	}
}

void agk::SetScreenResolution( int width, int height )
//****
{
	SetRendererResolution( (void*)g_pViewController, width, height );
	
	//NSLog( @"Device: %dx%dx%f", m_iRealDeviceWidth, m_iRealDeviceHeight, scale );
    //NSLog( @"Render: %dx%d", deviceWidth, deviceHeight );
	
	m_iRenderWidth = GetSurfaceWidth();
	m_iRenderHeight = GetSurfaceHeight();
	cCamera::UpdateAllAspectRatio( m_iRenderWidth/(float)m_iRenderHeight );
	   
    if ( m_bUsingDeviceAspect ) m_fDisplayAspect = m_iRenderWidth / (float) m_iRenderHeight;
    
    RecalculateDisplay();
    
    if ( g_pSocialPlugins && g_pSocialPlugins->bannerView_ )
	{
        agk::PlatformAdMobPosition(m_iAdHorz, m_iAdVert, m_fAdOffsetX, m_fAdOffsetY);
    }
    
    agk::SetVideoDimensions(m_fVideoX, m_fVideoY, m_fVideoWidth, m_fVideoHeight);
    
    agk::ClearScreen();
}

char* agk::GetURLSchemeText()
//****
{
	char* str = new char[g_sLastURLSchemeText.GetLength()+1];
    strcpy( str, g_sLastURLSchemeText.GetStr() );
	return str;
}

void agk::ClearURLSchemeText()
//****
{
	g_sLastURLSchemeText.SetStr("");
}

void agk::GetDeviceName( uString &outString )
//****
{
	struct utsname systemInfo;
    uname(&systemInfo);
	NSString* platform = [ NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];

	outString.SetStr( "ios|" );
	outString.Append( [platform cStringUsingEncoding:NSUTF8StringEncoding ] );
}

char* agk::GetDeviceName( )
//****
{
	struct utsname systemInfo;
    uname(&systemInfo);
	NSString* platform = [ NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
	
	char *str = new char[ 30 ];
	strcpy( str, "ios|" );
	strcat( str, [platform cStringUsingEncoding:NSUTF8StringEncoding ] );
	return str;
}

char* agk::GetDeviceBaseName( )
//****
{
	char *str = new char[ 4 ];
	strcpy( str, "ios" );
	return str;
}

char* agk::GetDeviceType( )
//****
{
	struct utsname systemInfo;
    uname(&systemInfo);
	NSString* platform = [ NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding];
	
	uString sType;
	sType.SetStr( [platform cStringUsingEncoding:NSUTF8StringEncoding ] );
	sType.Lower();

	char *str = new char[ sType.GetLength()+1 ];
	strcpy( str, sType.GetStr() );
	return str;
}

int agk::GetDeviceNetworkType()
{
    SCNetworkReachabilityRef reachability = SCNetworkReachabilityCreateWithName(NULL, "8.8.8.8");
    SCNetworkReachabilityFlags flags;
    BOOL success = SCNetworkReachabilityGetFlags(reachability, &flags);
    CFRelease(reachability);
    if (!success) {
        return -1;
    }
    BOOL isReachable = ((flags & kSCNetworkReachabilityFlagsReachable) != 0);
    BOOL needsConnection = ((flags & kSCNetworkReachabilityFlagsConnectionRequired) != 0);
    BOOL isNetworkReachable = (isReachable && !needsConnection);
    
    if (!isNetworkReachable) {
        return -1;
    } else if ((flags & kSCNetworkReachabilityFlagsIsWWAN) != 0) {
        return 0;
    } else {
        return 1;
    }
}

int agk::GetStorageRemaining( const char *path )
{
    uString sPath;
    if ( strncmp( path, "raw:", 4 ) == 0 )
    {
        sPath.SetStrUTF8( path+4 );
    }
    else
    {
        sPath.SetStr( path );
        PlatformGetFullPathWrite( sPath );
    }
    
    NSDictionary *attribs = [[NSFileManager defaultManager] attributesOfFileSystemForPath:[NSString stringWithUTF8String:sPath.GetStr()] error:nil];
    if ( !attribs ) return -1;
    
    NSNumber *bytes = [attribs objectForKey:NSFileSystemFreeSize];
    unsigned long long ubytes = [bytes unsignedLongLongValue];
    int MBs = (int) (ubytes/1048576);
    return MBs;
}

int agk::GetStorageTotal( const char *path )
{
    uString sPath;
    if ( strncmp( path, "raw:", 4 ) == 0 )
    {
        sPath.SetStrUTF8( path+4 );
    }
    else
    {
        sPath.SetStr( path );
        PlatformGetFullPathWrite( sPath );
    }
    
    NSDictionary *attribs = [[NSFileManager defaultManager] attributesOfFileSystemForPath:[NSString stringWithUTF8String:sPath.GetStr()] error:nil];
    if ( !attribs ) return -1;
    
    NSNumber *bytes = [attribs objectForKey:NSFileSystemSize];
    unsigned long long ubytes = [bytes unsignedLongLongValue];
    int MBs = (int) (ubytes/1048576);
    return MBs;
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
	const char* szLang = [language cStringUsingEncoding:NSUTF8StringEncoding];

	// todo
	char *str = new char[ strlen(szLang)+1 ];
	strcpy(str, szLang);
	return str;
}

void agk::SetSleepMode( int mode )
//****
{
	[UIApplication sharedApplication].idleTimerDisabled = (mode == 0 ? YES : NO);
}

void agk::SetExpansionFileKey( const char *key )
//****
{
	// do nothing
}

void agk::SetExpansionFileVersion(int version)
{
	// do nothing on ios
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
	// do nothing on ios
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
	// do nothing
}

bool agk::GetDeviceCanRotate()
{
	return true;
}

bool agk::GetAGKShouldRotate()
{
	return false;
}

void agk::PlatformSetOrientationAllowed( int portrait, int portrait2, int landscape, int landscape2 )
{
    bool bAllowed = true;
    switch( m_iOrientation )
    {
        case 1: if ( portrait == 0 ) bAllowed=false; break;
        case 2: if ( portrait2 == 0 ) bAllowed=false; break;
        case 3: if ( landscape == 0 ) bAllowed=false; break;
        case 4: if ( landscape2 == 0 ) bAllowed=false; break;
    }
    
    if ( !bAllowed )
    {
        UIInterfaceOrientation orien = UIInterfaceOrientationUnknown;
        if ( portrait != 0 ) orien = UIInterfaceOrientationPortrait;
        else if ( portrait2 != 0 ) orien = UIInterfaceOrientationPortraitUpsideDown;
        else if ( landscape != 0 ) orien = UIInterfaceOrientationLandscapeRight;
        else if ( landscape2 != 0 ) orien = UIInterfaceOrientationLandscapeLeft;
        else return;
        
        [[UIDevice currentDevice] setValue:@(orien) forKey:@"orientation"];
        [UINavigationController attemptRotationToDeviceOrientation];
    }
}

bool agk::PlatformGetDeviceID( uString &out )
{
    NSString *UUID = [[NSUserDefaults standardUserDefaults] objectForKey:@"uniqueID"];
    if (!UUID)
    {
        CFUUIDRef theUUID = CFUUIDCreate(NULL);
        CFStringRef string = CFUUIDCreateString(NULL, theUUID);
        CFRelease(theUUID);
        UUID = [(__bridge NSString*)string stringByReplacingOccurrencesOfString:@"-" withString:@""];
        [[NSUserDefaults standardUserDefaults] setValue:UUID forKey:@"uniqueID"];
    }
    
	out.SetStr( [UUID cStringUsingEncoding:NSUTF8StringEncoding] );
	return true;
}

float agk::PlatformDeviceScale()
{
    float realWidth = m_iRenderWidth;
    float realHeight = m_iRenderHeight;
    
    struct utsname systemInfo;
    uname(&systemInfo);
    // iPad 1 and iPod touch 4th gen have 256MB RAM but high res screens, force them to lower res.
    if ( strncmp( systemInfo.machine, "iPad1", 5 ) == 0 || strncmp( systemInfo.machine, "iPod4", 5 ) == 0 )
	{
        if ( realWidth > 700 || realHeight > 700 )
        {
            realWidth *= 0.5;
            realHeight *= 0.5;
        }
	}
    
	float max1 = m_iIntendedWidth;
	if ( m_iIntendedHeight > max1 ) max1 = m_iIntendedHeight;
	if ( max1 <= 0 ) return  1.0f;

	float max2 = realWidth;
	if ( realHeight > max2 ) max2 = realHeight;
	if ( max2 <= 0 ) return  1.0f;

	float scale = 1.0f;
	while ( max1 / max2 >= 1.6 )
	{
		scale /= 2.0f;
		max2 *= 2;
	}

	if ( scale < 0.25f ) scale = 0.25f;
	return scale;
}

int agk::PlatformGetNumProcessors()
{
	NSUInteger a = [[NSProcessInfo processInfo] processorCount];
    return (int)a;
}

void agk::PlatformInitFilePaths()
{
	// file paths
	g_pBundle = [NSBundle mainBundle];
	NSString *appName = [[g_pBundle infoDictionary] objectForKey:@"CFBundleName"];
	
	if ( appName )
	{
		m_sAppName.SetStr( [appName cStringUsingEncoding:NSUTF8StringEncoding] );
	}
	
	NSString *path = [g_pBundle resourcePath];
	strcpy( szRootDir, [path UTF8String] );
	strcat( szRootDir, "/" );
	
	g_pFileManager = [[NSFileManager alloc] init];
	
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); 
	NSString *documentsDirectoryPath = [paths objectAtIndex:0];
	const char* szDocuments = [ documentsDirectoryPath UTF8String ];
	strcpy( szWriteDir, szDocuments );
	strcat( szWriteDir, "/" );

	//ParseCurrentDirectory();
    m_bUpdateFileLists = true;
}

void agk::PlatformUpdateWritePath()
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); 
	NSString *documentsDirectoryPath = [paths objectAtIndex:0];
	const char* szDocuments = [ documentsDirectoryPath UTF8String ];
	strcpy( szWriteDir, szDocuments );
	strcat( szWriteDir, "/" );
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

void agk::InitJoysticks()
{
	
}

void agk::PlatformSetDevicePtr( void* ptr )
{
    g_pViewController = (UIViewController*) ptr;
	g_pMainWindow = g_pViewController.view;
}

void agk::PlatformInitCommon()
{
	// todo
}

void agk::PlatformInitExternal( void* ptr, int width, int height, void(*swap)(void*), void* swapParam )
{
	// todo
}

void agk::PlatformInitGL( void* ptr )
{
	// iPhone setup
	[UIApplication sharedApplication].idleTimerDisabled = YES;
	g_pViewController = (UIViewController*) ptr;
	g_pMainWindow = g_pViewController.view;

	float scale = [[UIScreen mainScreen] scale];
	if ( scale == 0 ) scale = 1;
	m_iRealDeviceWidth = [UIScreen mainScreen].bounds.size.width * scale;
	m_iRealDeviceHeight = [UIScreen mainScreen].bounds.size.height * scale;
    m_fDeviceScale = scale;
    
    //NSLog( @"Width: %d, Height: %d, Scale: %f", m_iRealDeviceWidth, m_iRealDeviceHeight, m_fDeviceScale );
    
    UIInterfaceOrientation orien = [UIApplication sharedApplication].statusBarOrientation;
	
	switch ( orien )
	{
        case UIInterfaceOrientationPortrait:
        {
            agk::SetOrientationAllowed(1,1,0,0);
            agk::m_bOrientationSet = false;
            break;
        }
        case UIInterfaceOrientationPortraitUpsideDown:
        {
            agk::SetOrientationAllowed(1,1,0,0);
            agk::m_bOrientationSet = false;
            break;
        }
        case UIInterfaceOrientationLandscapeLeft:
        {
            if ( m_iRealDeviceHeight > m_iRealDeviceWidth )
            {
                int temp = m_iRealDeviceWidth;
                m_iRealDeviceWidth = m_iRealDeviceHeight;
                m_iRealDeviceHeight = temp;
            }
            agk::SetOrientationAllowed(0,0,1,1);
            agk::m_bOrientationSet = false;
            break;
        }
        case UIInterfaceOrientationLandscapeRight:
        {
            if ( m_iRealDeviceHeight > m_iRealDeviceWidth )
            {
                int temp = m_iRealDeviceWidth;
                m_iRealDeviceWidth = m_iRealDeviceHeight;
                m_iRealDeviceHeight = temp;
            }
            agk::SetOrientationAllowed(0,0,1,1);
            agk::m_bOrientationSet = false;
            break;
        }
	}
     
	if ( !CreateRendererWindow( ptr, m_iResolutionMode, m_iRealDeviceWidth, m_iRealDeviceHeight ) ) return;
	
	agk::PlatformDisableScissor();
    
    //NSLog( @"Device: %dx%dx%f", m_iRealDeviceWidth, m_iRealDeviceHeight, scale );
    //NSLog( @"Render: %dx%d", deviceWidth, deviceHeight );
    
    cImage::GetMaxTextureSize(); // preloads the value so that any threads don't call the GL function behind it
    agk::PlatformSetAlignment( 1 ); // not using memory alignment
	
	m_iRenderWidth = GetSurfaceWidth();
	m_iRenderHeight = GetSurfaceHeight();
	cCamera::UpdateAllAspectRatio( m_iRenderWidth/(float)m_iRenderHeight );
	
	m_iRealDeviceWidth /= scale;
	m_iRealDeviceHeight /= scale;
	
	dStartTime = CACurrentMediaTime();
	SetRandomSeed(agk::Round(dStartTime));
	
	// sound setup
	cSoundMgr::Init();
	
	// accelerometer
	//UIAccelerometer* theAccelerometer = [ UIAccelerometer sharedAccelerometer ];
	//theAccelerometer.updateInterval = 1 / 10;
	
	// Warning about app delegate not conforming is because app delegate does not exist until app creation.
	// As long as the final app delegate extends UIAccelerometerDelegate this will work.
	//theAccelerometer.delegate = (id) [ [UIApplication sharedApplication] delegate ];
    
    // motion setup
    g_pMotionManager = [[CMMotionManager alloc] init];
    if ( g_pMotionManager.accelerometerAvailable )
    {
        m_iAccelerometerExists = 1;
        [ g_pMotionManager startAccelerometerUpdates ];
    }
    if ( g_pMotionManager.gyroAvailable )
    {
        m_iGyroSensorExists = 1;
        [ g_pMotionManager startGyroUpdates ];
    }
    if ( g_pMotionManager.deviceMotionAvailable )
    {
        m_iRotationSensorExists = 1;
        [ g_pMotionManager startDeviceMotionUpdatesUsingReferenceFrame:CMAttitudeReferenceFrameXMagneticNorthZVertical ];
    }
    if ( g_pMotionManager.magnetometerAvailable )
    {
        m_iMagneticSensorExists = 1;
        [ g_pMotionManager startMagnetometerUpdates ];
    }
    
    // location setup
    g_pLocationManager = [[CLLocationManager alloc] init];
    g_pLocationListener = [[AGKLocationListener alloc] init];
    switch( [CLLocationManager authorizationStatus] )
    {
        case kCLAuthorizationStatusDenied:
        case kCLAuthorizationStatusRestricted: m_iGPSSensorExists = 0; break;
        case kCLAuthorizationStatusAuthorized:
        case kCLAuthorizationStatusAuthorizedWhenInUse:
        case kCLAuthorizationStatusNotDetermined: m_iGPSSensorExists = 1; break;
    }
    g_pLocationManager.delegate = g_pLocationListener;
    
    AGKShader::CreateDefaultShaders();

	// text input setup
	float width = 250;
	float height = 25;
	if ( width > m_iRealDeviceWidth ) width = (float) m_iRealDeviceWidth;
	float x = (m_iRealDeviceWidth - width) / 2.0f;
	float y = m_iRealDeviceHeight/4.0f;
    
    /*
    if ( orien == UIInterfaceOrientationLandscapeLeft || orien == UIInterfaceOrientationLandscapeRight )
    {
        float temp = x;
        x = y;
        y = temp;
    }
     */
    
	g_EventListener = [EventListener alloc];
    g_EventListener->m_bSkipResign = false;
	
	pTextField = [[UITextField alloc] initWithFrame:CGRectMake(x,y,width,height) ];
	[ pTextField setText:@"" ];
	[ pTextField setUserInteractionEnabled:YES ];
	[ pTextField setBorderStyle:UITextBorderStyleRoundedRect];
	pTextField.autocorrectionType = UITextAutocorrectionTypeNo;
	pTextField.delegate = g_EventListener;
	pTextField.returnKeyType = UIReturnKeyDone;
    pTextField.autocapitalizationType = UITextAutocapitalizationTypeNone;
    pTextField.autocorrectionType = UITextAutocorrectionTypeNo;
    
    pTextView = [[UITextView alloc] initWithFrame:CGRectMake(x,y,width,height) ];
	[ pTextView setText:@"" ];
	[ pTextView setUserInteractionEnabled:YES ];
	//[ pTextView setBorderStyle:UITextBorderStyleRoundedRect];
	pTextView.autocorrectionType = UITextAutocorrectionTypeNo;
	pTextView.delegate = g_EventListener;
	pTextView.returnKeyType = UIReturnKeyDefault;
    pTextView.autocapitalizationType = UITextAutocapitalizationTypeNone;
    pTextView.autocorrectionType = UITextAutocorrectionTypeNo;
    
    g_bMultiline = false;
    g_iInputType = 0;
    
	pTextBackground = new cSprite();
	pTextBackground->SetColor( 0,0,0, 128 );
	pTextBackground->SetPosition( -m_iDisplayExtraX, -m_iDisplayExtraY );
	pTextBackground->SetSize( m_iDisplayWidth+m_iDisplayExtraX*2, m_iDisplayHeight+m_iDisplayExtraY*2 );
	pTextBackground->FixToScreen(1);
    
    SocialPluginsSetup();
}

void agk::PlatformInitConsole()
{
	dStartTime = CACurrentMediaTime();
	SetRandomSeed(agk::Round(dStartTime));
	
	// sound setup
	cSoundMgr::Init();
	
	// motion setup
    g_pMotionManager = [[CMMotionManager alloc] init];
    if ( g_pMotionManager.accelerometerAvailable )
    {
        m_iAccelerometerExists = 1;
        [ g_pMotionManager startAccelerometerUpdates ];
    }
    if ( g_pMotionManager.gyroAvailable )
    {
        m_iGyroSensorExists = 1;
        [ g_pMotionManager startGyroUpdates ];
    }
    if ( g_pMotionManager.deviceMotionAvailable )
    {
        m_iRotationSensorExists = 1;
        m_iMagneticSensorExists = 1;
        [ g_pMotionManager startDeviceMotionUpdates ];
    }
    else if ( g_pMotionManager.magnetometerAvailable )
    {
        m_iMagneticSensorExists = 1;
        [ g_pMotionManager startMagnetometerUpdates ];
    }
    
    SocialPluginsSetup();
}

void agk::UpdatePtr( void *ptr )
{    
    g_pViewController = (UIViewController*) ptr;
	g_pMainWindow = g_pViewController.view;
		
	float scale = [[UIScreen mainScreen] scale];
	if ( scale == 0 ) scale = 1;
	m_iRealDeviceWidth = [UIScreen mainScreen].bounds.size.width * scale;
	m_iRealDeviceHeight = [UIScreen mainScreen].bounds.size.height * scale;
    m_fDeviceScale = scale;
	
	UIInterfaceOrientation orien = [UIApplication sharedApplication].statusBarOrientation;
	
	switch ( orien )
	{
		case UIInterfaceOrientationPortrait: 
		case UIInterfaceOrientationPortraitUpsideDown: break;
		case UIInterfaceOrientationLandscapeLeft: 
        {
            if ( m_iRealDeviceHeight > m_iRealDeviceWidth )
            {
                int temp = m_iRealDeviceWidth;
                m_iRealDeviceWidth = m_iRealDeviceHeight;
                m_iRealDeviceHeight = temp;
            }
            break;
        }
		case UIInterfaceOrientationLandscapeRight: 
		{
            if ( m_iRealDeviceHeight > m_iRealDeviceWidth )
            {
                int temp = m_iRealDeviceWidth;
                m_iRealDeviceWidth = m_iRealDeviceHeight;
                m_iRealDeviceHeight = temp;
            }
            break;
		}
	}
	
	SetRendererPointers( ptr, m_iResolutionMode, m_iRealDeviceWidth, m_iRealDeviceHeight );
    
    //NSLog( @"Device: %dx%dx%f", m_iRealDeviceWidth, m_iRealDeviceHeight, scale );
    //NSLog( @"Render: %dx%d", deviceWidth, deviceHeight );
	
	m_iRenderWidth = GetSurfaceWidth();
	m_iRenderHeight = GetSurfaceHeight();
	cCamera::UpdateAllAspectRatio( m_iRenderWidth/(float)m_iRenderHeight );
	
	m_iRealDeviceWidth /= scale;
	m_iRealDeviceHeight /= scale;
    
    if ( m_bUsingDeviceAspect ) m_fDisplayAspect = m_iRenderWidth / (float) m_iRenderHeight;
    
    RecalculateDisplay();
    
    if ( g_pSocialPlugins && g_pSocialPlugins->bannerView_ )
	{
        agk::PlatformAdMobPosition(m_iAdHorz, m_iAdVert, m_fAdOffsetX, m_fAdOffsetY);
    }
    
    agk::SetVideoDimensions(m_fVideoX, m_fVideoY, m_fVideoWidth, m_fVideoHeight);
    
    if( youtubeView )
    {
        int width = g_pViewController.view.frame.size.width;
        int height = g_pViewController.view.frame.size.height;
        [youtubeView setFrame:CGRectMake(0,0,width,height)];
        [youtubeView evaluateJavaScript:@"player.setSize(window.innerWidth, window.innerHeight-50);" completionHandler:nil];
    }
    
    agk::ClearScreen();
}

void agk::UpdatePtr2( void *ptr )
{
    g_pViewController = (UIViewController*) ptr;
	g_pMainWindow = g_pViewController.view;
}

int agk::GetInternalDataI( int index )
{
	if ( index == 0 ) return g_iImmersiveMode;
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
	if ( mode > 0 )
	{
		m_iSyncMode = 0;
		m_fSyncTime = 1 / 60.0f;
	}
}

void agk::Sleep( UINT milliseconds )
//****
{
	// sleeping on iPhone may crash the app?
	float seconds = milliseconds/1000.0f;
	[NSThread sleepForTimeInterval:seconds];
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
	return m_iRealDeviceWidth * m_fDeviceScale;
}

int agk::GetMaxDeviceHeight()
//****
{
	return m_iRealDeviceHeight * m_fDeviceScale;
}

int agk::GetDeviceDPI()
//****
{
	int result = 0;
	char *szType = agk::GetDeviceType();
	if ( !szType ) return 0;
	
	if ( strncmp( szType, "iPhone", 6 ) == 0 )
	{
		char *szNum = szType + 6;
		switch( *szNum )
		{
			case '1':						// iPhone 1, 3G
			case '2': result = 163; break;	// iPhone 3GS
			case '3':						// iPhone 4
			case '4':						// iPhone 4S
			case '5': 						// iPhone 5, 5C
			case '6': result = 326; break;	// iPhone 5S
			case '7': 
			{
				switch( *(szNum+2) ) 
				{
					case '1': result = 401; break;	// iPhone 6 Plus
					case '2': result = 326; break;	// iPhone 6
				}
			}
			case '8': 
			{
				switch( *(szNum+2) ) 
				{
					case '1': result = 326; break;	// iPhone 6S
					case '2': result = 401; break;	// iPhone 6S Plus
					case '4': result = 326; break;	// iPhone SE
				}
			}
			case '9': 
			{
				switch( *(szNum+2) ) 
				{
					case '1': 
					case '3': result = 326; break;	// iPhone 7
					case '2': 
					case '4': result = 401; break;	// iPhone 7 Plus
				}
			}
		}
	}
	else if ( strncmp( szType, "iPad", 4 ) == 0 )
	{
		char *szNum = szType + 4;
		
		switch( *szNum )
		{
			case '1': result = 132; break;	// iPad 1
			case '2': 
			{
				switch( *(szNum+2) ) 
				{
					case '1': 
					case '2': 
					case '3': 
					case '4': result = 132; break;	// iPad 2
					case '5': 
					case '6': 
					case '7': result = 163; break;	// iPad Mini
				}
			}
			case '3': result = 264; break;	// iPad 3, 4
			case '4':
			{
				switch( *(szNum+2) ) 
				{
					case '1': 
					case '2': 
					case '3': result = 264; break;	// iPad Air
					case '4': 
					case '5': 
					case '6':
					case '7':
					case '8': 
					case '9': result = 326; break;	// iPad Mini 2, Mini 3
				}
			}
			case '5': 
			{
				switch( *(szNum+2) ) 
				{
					case '1': 
					case '2': result = 326; break;	// iPad Mini 4
					case '3': 
					case '4': result = 264; break;	// iPad Air 2
				}
			}
			case '6': result = 264; break;	// iPad Pro 9.7, Pro 12.9
		}
	}
	else if ( strncmp( szType, "iPod", 4 ) == 0 )
	{
		char *szNum = szType + 4;
		
		switch( *szNum )
		{
			case '1':						// iPod 1
			case '2': 						// iPod 2
			case '3': result = 163; break;	// iPod 3
			case '4':						// iPod 4
			case '5': 						// iPod 5
			case '7': result = 326; break;	// iPod 6
		}
	}
	
	// if not found, use an estimated value
	if ( result == 0 )
	{
		float scale = 1;
		if ([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) scale = [[UIScreen mainScreen] scale];
		
		if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) result = 132 * scale;
		else if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPhone) result = 163 * scale;
		else result = 160 * scale;
	}
	
	delete [] szType;
	return result;
}

char* agk::GetAppPackageName()
//****
{
	NSString *bundleID = [[NSBundle mainBundle] bundleIdentifier];
	const char* packageName = [bundleID UTF8String];

	char *str = new char[ strlen(packageName)+1 ];
	strcpy( str, packageName );
	return str;
}

int agk::GetDevicePlatform()
//****
{
	return 0;
}

void agk::PlatformUpdateDeviceSize()
{
    float scale = [[UIScreen mainScreen] scale];
	if ( scale == 0 ) scale = 1;
	m_iRealDeviceWidth = [UIScreen mainScreen].bounds.size.width * scale;
	m_iRealDeviceHeight = [UIScreen mainScreen].bounds.size.height * scale;
    m_fDeviceScale = scale;
	
	m_iRenderWidth = GetSurfaceWidth();
	m_iRenderHeight = GetSurfaceHeight();
	cCamera::UpdateAllAspectRatio( m_iRenderWidth/(float)m_iRenderHeight );
}

void agk::PlatformUpdateTime ( void )
{
	m_fTimeCurr = CACurrentMediaTime() - dStartTime;
    m_iTimeMilliseconds = agk::Round(m_fTimeCurr*1000);
}

void agk::PlatformResetTime ( void )
{
    dStartTime = CACurrentMediaTime();
    m_fTimeCurr = 0;
	m_iTimeMilliseconds = 0;
}

double agk::PlatformGetRawTime ( void )
{
	return CACurrentMediaTime();
}

void agk::CompositionChanged()
{

}

void agk::PlatformSync()
{
	agk::PlatformSwap();
    
    // update motion
    if ( g_pMotionManager )
    {
        if ( g_pMotionManager.accelerometerActive )
        {
            agk::Accelerometer( (float) g_pMotionManager.accelerometerData.acceleration.x,
                                (float) -g_pMotionManager.accelerometerData.acceleration.y,
                                (float) -g_pMotionManager.accelerometerData.acceleration.z );
        }
        
        if ( g_pMotionManager.gyroActive )
        {
            agk::Gyro( (float) g_pMotionManager.gyroData.rotationRate.x,
                       (float) g_pMotionManager.gyroData.rotationRate.y,
                       (float) g_pMotionManager.gyroData.rotationRate.z );
        }
        
        if ( g_pMotionManager.deviceMotionActive )
        {
            agk::RotationSensor( (float) g_pMotionManager.deviceMotion.attitude.quaternion.x,
                                 (float) g_pMotionManager.deviceMotion.attitude.quaternion.y,
                                 (float) g_pMotionManager.deviceMotion.attitude.quaternion.z,
                                 (float) g_pMotionManager.deviceMotion.attitude.quaternion.w );
            
            agk::Magnetic( (float) g_pMotionManager.deviceMotion.magneticField.field.x,
                           (float) g_pMotionManager.deviceMotion.magneticField.field.y,
                           (float) g_pMotionManager.deviceMotion.magneticField.field.z );
        }
        else if ( g_pMotionManager.magnetometerActive )
        {
            agk::Magnetic( (float) g_pMotionManager.magnetometerData.magneticField.x,
                           (float) g_pMotionManager.magnetometerData.magneticField.y,
                           (float) g_pMotionManager.magnetometerData.magneticField.z );
        }
    }
    
    if ( g_pSpeechDelegate ) [g_pSpeechDelegate update];
}

void agk::PlatformCompleteInputInit()
{
	
}

bool bUpdateEditBox = false;

void agk::KeyboardMode( int mode )
{
	m_iKeyboardMode = mode;
}

int agk::PlatformInputPointerPressed(float x, float y)
{
    if ( g_bEditBoxHack )
	{
		// check if pressed on an edit box
		if ( m_cEditBoxList.GetCount() > 0 )
		{
			cEditBox *pFound = 0;
			cEditBox *pEditBox = m_cEditBoxList.GetFirst();
			while ( pEditBox )
			{
				float fWorldX = x;
				float fWorldY = y;
                
				if ( !pEditBox->GetFixed() )
				{
					fWorldX = agk::ScreenToWorldX( x );
					fWorldY = agk::ScreenToWorldY( y );
				}
                
				if ( pEditBox->GetHitTest( fWorldX, fWorldY ) && pEditBox->GetVisible() && pEditBox->GetActive() )
				{
					pEditBox->SetCursorLocation( fWorldX, fWorldY );
					pFound = pEditBox;
                    bUpdateEditBox = true;
				}
				
				pEditBox = m_cEditBoxList.GetNext();
			}
            
			if ( pFound )
			{
				pFound->GetText( m_sCurrInput );
                if ( g_bMultiline != pFound->GetMultiLine() || g_iInputType != pFound->GetInputType() )
                {
                    g_EventListener->m_bSkipResign = true;
                    if ( g_bMultiline )
                    {
                        [pTextView resignFirstResponder];
                    }
                    else
                    {
                        [pTextField resignFirstResponder];
                    }
                    
                    m_bInputStarted = false;
                }
                else
                {
                    SetSystemTextBox( m_sCurrInput.GetStr() );
                }
                pFound->SetFocus( true );
                
				pEditBox = m_cEditBoxList.GetFirst();
				while ( pEditBox )
				{
					if ( pEditBox != pFound ) pEditBox->SetFocus( false );
					pEditBox = m_cEditBoxList.GetNext();
				}
			}
            else
            {
                if( g_bMultiline ) [ pTextView resignFirstResponder ];
                else [ pTextField resignFirstResponder ];
				return 0;
            }
		}
	}
    else
    {
        if( g_bMultiline )
        {
            [ pTextView resignFirstResponder ];
        }
        else
        {
            [ pTextField resignFirstResponder ];
            
        }
    }

	return 1;
}

void agk::PlatformStartTextInput( const char* sInitial )
{
	if ( m_bInputStarted ) return;

	NSString* pString = [ [ NSString alloc ] initWithCString: sInitial encoding:NSUTF8StringEncoding ];
    
    if ( g_bMultiline )
    {
        pTextView.secureTextEntry = g_bPasswordMode ? YES : NO;
        [ pTextView setText:pString ];
        
        m_bInputCancelled = false;
        g_EventListener->m_bCancelled = true;
        g_EventListener->m_bFinished = false;
        [ g_pMainWindow addSubview:pTextView ];
        [ pTextView becomeFirstResponder ];
    }
    else
    {
        if ( g_iInputType == 1 ) pTextField.keyboardType = UIKeyboardTypeDecimalPad;
        else pTextField.keyboardType = UIKeyboardTypeDefault;
        pTextField.secureTextEntry = g_bPasswordMode ? YES : NO;
        [ pTextField setText:pString ];
        
        m_bInputCancelled = false;
        g_EventListener->m_bCancelled = true;
        g_EventListener->m_bFinished = false;
        [ g_pMainWindow addSubview:pTextField ];
        [ pTextField becomeFirstResponder ];
    }
    
    [ pString release ];
    
    g_bEditBoxHack = false;
}

void agk::PlatformStopTextInput() 
{
	if ( m_bInputStarted )
	{
		if( g_bMultiline ) [ pTextView resignFirstResponder ];
		else [ pTextField resignFirstResponder ];
	}
    
    g_iInputType = 0;
}

void agk::PlatformChangeTextInput( const char* str )
{
    m_sCurrInput.SetStr(str);
	SetSystemTextBox(str);
}

void agk::PlatformUpdateTextInput()
{
	if ( m_bInputStarted )
	{
		if ( g_EventListener->m_bFinished )
		{
			m_sCurrInput.SetStr( g_EventListener->m_sText );
			m_bInputStarted = false;
			m_bInputCancelled = g_EventListener->m_bCancelled;
            int editbox = agk::GetCurrentEditBox();
            if ( editbox ) agk::SetEditBoxFocus(editbox, 0);
            g_bPasswordMode = false;
		}
		else
		{
			if ( agk::GetPointerPressed() && !m_bInputCancelled )
			{
				//[ g_EventListener textFieldDidEndEditing:pTextField  ];
				if( g_bMultiline ) [ pTextView resignFirstResponder ];
                else [ pTextField resignFirstResponder ];
				//m_bInputCancelled = true;
				//m_sCurrInput.SetStr( g_EventListener->m_sText );
				//m_bInputStarted = false;
			}
			else
			{
				if ( g_bMultiline ) m_sCurrInput.SetStr( [pTextView.text cStringUsingEncoding:NSUTF8StringEncoding ] );
                else m_sCurrInput.SetStr( [pTextField.text cStringUsingEncoding:NSUTF8StringEncoding ] );
			}
		}
	}
}

void agk::PlatformDrawTextInput()
{
	// text input setup
	float width = 2 * agk::GetDeviceDPI();
	float height = 0.2f * agk::GetDeviceDPI();
	if ( width > m_iRealDeviceWidth ) width = (float) m_iRealDeviceWidth;
	float x = (m_iRealDeviceWidth - width) / 2.0f;
	float y = m_iRealDeviceHeight/4.0f;
    
    if ( g_bEditBoxHack ) y += m_iRealDeviceHeight;
    
    if ( g_bMultiline ) 
	{
		[pTextView setFrame:CGRectMake(x,y,width,height*3)];
	}
    else 
	{
		[pTextField setFrame:CGRectMake(x,y,width,height)];
	}
    
	if ( m_bInputStarted )
	{
		agk::PlatformSetDepthTest( 0 );
		pTextBackground->SetPosition( -m_iDisplayExtraX, -m_iDisplayExtraY );
		pTextBackground->SetSize( m_iDisplayWidth+m_iDisplayExtraX*2, m_iDisplayHeight+m_iDisplayExtraY*2 );
		if ( !g_bEditBoxHack ) pTextBackground->Draw();
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
	return (int) lrintf( floor( a ) );
}

int agk::Ceil( float a )
{
	return (int) lrintf( ceil( a ) );
}

int agk::Round( float a )
{
	return (int) lrintf( a );
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

// local function
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
    // do nothing on iOS
    return true;
}

void agk::PlatformShowChooseScreen()
{
    m_pUncollectedChosenImage = new cImage();
    
    if ( g_ImagePicker ) [ g_ImagePicker release ];
    g_ImagePicker = [[AGKImagePicker alloc] init];
    g_ImagePicker->m_bCancelled = false;
    g_ImagePicker->imagePickerController = 0;
    g_ImagePicker->m_bFinished = 0;
    g_ImagePicker->m_pImage = 0;
    
    [ g_ImagePicker pickImage:m_pUncollectedChosenImage ];
}

bool agk::PlatformShowCaptureScreen()
{
    if ( ![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera] )
    {
        return false;
    }
    
    UINT iID = m_cImageList.GetFreeID( 0x7fffffff );
	
	if ( iID == 0 )
	{
#ifdef _AGK_ERROR_CHECK
		uString errStr( "Failed to choose image - no free image IDs found" );
		agk::Error( errStr );
#endif
		return false;
	}
    
	cImage* pImage = new cImage();
    m_cImageList.AddItem( pImage, iID );
    m_iUncollectedCaptureImage = iID;
    
    if ( g_CameraPicker ) [ g_CameraPicker release ];
    g_CameraPicker = [[AGKCameraPicker alloc] init];
    g_CameraPicker->m_bCancelled = false;
    g_CameraPicker->imagePickerController = 0;
    g_CameraPicker->m_bFinished = 0;
    g_CameraPicker->m_pImage = 0;
    
    [ g_CameraPicker pickImage:pImage ];
    
    return true;
}

void agk::PlatformHideCaptureScreen()
{
	if ( g_CameraPicker )
    {
        [ g_CameraPicker release ];
        g_CameraPicker = 0;
    }
    
    if ( m_iUncollectedCaptureImage ) 
    {
        agk::DeleteImage( m_iUncollectedCaptureImage );
        m_iUncollectedCaptureImage = 0;
    }
}

bool cImage::CaptureFromCamera()
{
	// do nothing on iOS
    return true;
}

int agk::GetNumDeviceCameras()
//****
{
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
	return (int) [devices count];
}

int agk::SetDeviceCameraToImage( UINT cameraID, UINT imageID )
{
    cImage *pImage = m_cImageList.GetItem( imageID );
    if ( pImage )
    {
        if ( pImage != m_pCameraTexture )
        {
            agk::Error( "Failed to set device camera to image, image already exists" );
            return 0;
        }
        else
        {
            agk::Warning( "Device camera is already streaming to that image" );
            return 1;
        }
    }
    
    if ( imageID == 0 )
    {
        m_iDeviceCameraID = -1;
        
        if ( cameraSession ) [cameraSession stopRunning];
        
        if ( m_pCameraTexture )
        {
            // delete everything
            m_cImageList.RemoveItem( m_pCameraTexture->m_iID );
            m_pCameraTexture->OverrideTexture(0, 0, 0);
            delete m_pCameraTexture;
            m_pCameraTexture = 0;
        }
        
        ReleaseCameraImageData();
        
        if ( cameraOutput )
        {
            [cameraOutput release];
            cameraOutput = nil;
        }
        
        if ( cameraSession )
        {
            [ cameraSession release ];
            cameraSession = 0;
        }
        
        return 1;
    }
    
    m_iDeviceCameraID = cameraID;
    
    if ( m_pCameraTexture )
    {
        agk::Error( "Cannot change image ID whilst the camera is streaming, use an ID of 0 first to clear the existing image, then use the new ID" );
        return 0;
    }
    
    if ( !g_pVideoCaptureListener ) g_pVideoCaptureListener = [[AGKVideoCaptureDelegate alloc] init];
    
    if ( !CreateCameraImageData() ) return 0;
    
    cameraSession = [[AVCaptureSession alloc] init];
    [cameraSession beginConfiguration];
    
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    if ( [devices count] == 0 )
    {
        agk::Warning( "Failed to set device camera to image - no device cameras found" );
        return 0;
    }
    if ( cameraID >= [devices count] ) cameraID = 0;
    
    AVCaptureDevice *device = [devices objectAtIndex:cameraID];
    m_iDeviceCameraType = GetDeviceCameraType(cameraID);
    
    NSError *error = 0;
    AVCaptureDeviceInput *input = [AVCaptureDeviceInput deviceInputWithDevice:device error:&error];
    if ( error )
    {
        agk::Warning( "Failed to set device camera to image - could not get AVCaptureDeviceInput" );
        return 0;
    }
    [cameraSession addInput:input];
    
    cameraOutput = [[AVCaptureVideoDataOutput alloc] init];
    [cameraOutput setAlwaysDiscardsLateVideoFrames:TRUE];
    NSDictionary* settings = @{ (id)kCVPixelBufferPixelFormatTypeKey : [NSNumber numberWithInt:kCVPixelFormatType_32BGRA] };
    [cameraOutput setVideoSettings:settings];
    [cameraOutput setSampleBufferDelegate:g_pVideoCaptureListener queue:dispatch_get_main_queue()];
    
    [cameraSession addOutput:cameraOutput];
    [cameraSession commitConfiguration];
    
    CMVideoDimensions dim = CMVideoFormatDescriptionGetDimensions( device.activeFormat.formatDescription );
    
    m_pCameraTexture = new cImage();
    m_pCameraTexture->CreateBlankImage( dim.width, dim.height, 0, 0 );
    m_pCameraTexture->m_iID = imageID;
    m_cImageList.AddItem( m_pCameraTexture, imageID );
    
    AVCaptureConnection *con = [cameraOutput connectionWithMediaType:AVMediaTypeVideo];
    int orien = GetOrientation();
    switch( orien )
    {
        case 1: [con setVideoOrientation:AVCaptureVideoOrientationPortrait]; break;
        case 2: [con setVideoOrientation:AVCaptureVideoOrientationPortraitUpsideDown]; break;
        case 3: [con setVideoOrientation:AVCaptureVideoOrientationLandscapeRight]; break;
        case 4: [con setVideoOrientation:AVCaptureVideoOrientationLandscapeLeft]; break;
    }
    
    if ( m_iDeviceCameraType == 2 ) [con setVideoMirrored:TRUE];
    
    [cameraSession startRunning];
    
    return 1;
}

void agk::DeviceCameraUpdate()
{
    // iOS uses callbacks for the image
    
    if ( !cameraOutput ) return;
    
    AVCaptureConnection *con = [cameraOutput connectionWithMediaType:AVMediaTypeVideo];
    int orien = GetOrientation();
    switch( orien )
    {
        case 1: [con setVideoOrientation:AVCaptureVideoOrientationPortrait]; break;
        case 2: [con setVideoOrientation:AVCaptureVideoOrientationPortraitUpsideDown]; break;
        case 3: [con setVideoOrientation:AVCaptureVideoOrientationLandscapeRight]; break;
        case 4: [con setVideoOrientation:AVCaptureVideoOrientationLandscapeLeft]; break;
    }
}

void agk::DeviceCameraResumed()
{
	// do nothing
}

int agk::GetDeviceCameraType( UINT cameraID )
//****
{
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    if ( cameraID >= [devices count] ) return 0;
    
    AVCaptureDevice *device = [devices objectAtIndex:cameraID ];
    
    if ([device position] == AVCaptureDevicePositionBack) return 1;
    else if ([device position] == AVCaptureDevicePositionFront) return 2;
    else return 0;
}

void cImage::Print( float size )
{
	// save to photos folder
    
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
    
    CGDataProviderRetain( provider );
    
    CGImageRef iOSImage = CGImageCreate(m_iOrigWidth, m_iOrigHeight, 8, 32, m_iOrigWidth*4, colorSpaceRef, bitmapInfo, provider, NULL, NO, renderingIntent);
    
    CGImageRetain(iOSImage);
    
    UIImage *finalImage = [UIImage imageWithCGImage:iOSImage];
    [finalImage retain];
    
    UIImageWriteToSavedPhotosAlbum( finalImage, nil, nil, nil );
    
	//agk::Message( "Image has been saved to your photos folder" );
    
    //delete [] bits;
    // this is a deliberate memory leak as UIImageWriteToSavedPhotosAlbum does work in the background
    // need to detect when it is done and clean up
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
	AudioServicesPlaySystemSound( kSystemSoundID_Vibrate );
}

void agk::SetClipboardText( const char* szText )
//****
{
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    if ( !pasteboard ) return;
    pasteboard.string = [NSString stringWithUTF8String:szText];
}

char* agk::GetClipboardText()
//****
{
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    if ( pasteboard && pasteboard.string )
    {
        const char* text = [pasteboard.string UTF8String];
        if ( text && *text )
        {
            char *str = new char[ strlen(text) + 1 ];
            strcpy( str, text );
            return str;
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
	
	// Warning about app delegate not conforming is because app delegate does not exist until app creation.
	// As long as the final app delegate extends AVAudioPlayerDelgate this will work.
	musicPlayer.delegate = (id) [[UIApplication sharedApplication] delegate ];
    
    return true;
}

void cMusicMgr::Play( UINT iID, bool bLoop, UINT iStartID, UINT iEndID )
{
	// don't play music if ipod is playing
	if ( audioIsAlreadyPlaying ) return;
	
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
    
    if ( !PlatformPrepare( iID ) ) return;
	
	cMusic *pMusic = m_pMusicFiles[ iID ];

	[ musicPlayer setNumberOfLoops:0 ];
	musicPlayer.volume = (pMusic->m_iVolume * m_iMasterVolume) / 10000.0f;
	[ musicPlayer play ];
	
	m_pCurrentlyPlaying = pMusic;
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
}

void cMusicMgr::SetMasterVolume( int vol )
{
	if ( vol < 0 ) vol = 0;
	if ( vol > 100 ) vol = 100;
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
		AVAudioSession *audioSession = [AVAudioSession sharedInstance];
		[audioSession setCategory:AVAudioSessionCategoryPlayback error:nil];
		[audioSession setActive:YES error:nil];
		
		audioDevice = alcOpenDevice(NULL); // select the "preferred device"
		if(!audioDevice) agk::Error( "Failed to setup audio device" );
		else 
		{
			audioContext = alcCreateContext(audioDevice, NULL);
			if(!audioContext) agk::Error( "Failed to create audio context" );
			alcMakeContextCurrent(audioContext);
		}
	}
    
    m_fMinPlaybackRate = 0.5f;
    m_fMaxPlaybackRate = 2.0f;
    m_fStepPlaybackRate = 0.01f; //unknown
}

void cSoundMgr::AppPaused()
{
    // don't modify sound settings, it breaks on some iOS devices
}

void cSoundMgr::AppResumed()
{
    // don't try to re-initialise sound, it breaks on some iOS devices
}

void cSoundMgr::PlatformAddFile( cSoundFile *pSound )
{
	// no further processing needed on iOS
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
	
	// setup iphone specific variables
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
@interface YoutubeDelegate : NSObject <WKNavigationDelegate>
{
@public
    
}
- (void)notifyDelegateOfYouTubeCallbackUrl: (NSURL *) url;
@end

@implementation YoutubeDelegate

- (void)notifyDelegateOfYouTubeCallbackUrl: (NSURL *) url {
    NSString *action = url.host;
    
    if ([action isEqual:@"back"])
    {
        if ( youtubeView )
        {
            [youtubeView removeFromSuperview];
            [youtubeView release];
            youtubeView = nil;
        }
        [g_pViewController setActive];
    }
}

- (void)webView:(WKWebView *)webView
decidePolicyForNavigationAction:(WKNavigationAction *)navigationAction
decisionHandler:(void (^)(WKNavigationActionPolicy))decisionHandler
{
    NSURL *url = [navigationAction request].URL;
    if ( [url.scheme isEqual:@"ytplayer"] )
    {
        [self notifyDelegateOfYouTubeCallbackUrl:url];
        decisionHandler( WKNavigationActionPolicyCancel );
    }
    else
    {
        decisionHandler( WKNavigationActionPolicyAllow );
    }
}

@end

namespace AGK
{
    YoutubeDelegate *youtubeDelegate = nil;
    
    const char* szYoutubeFormat = "<html><head><style>\n\
    body { margin: 0; width:100%%; height:100%%;  background-color:#000000; }\n\
    html { width:100%%; height:100%%; background-color:#000000; }\n\
    </style>\n\
    <meta name=\"viewport\" content=\"initial-scale=1.0\"/>\n\
    </head>\n\
    <body>\n\
    <div style=\"text-align:center;width:100%%;height:50px;background-color:#2D2D2D;color:#969696;font-family:Arial,sans-serif;font-size:34px;padding-top:8px\" onclick=\"window.location.href='ytplayer://back'\">Back</div>\n\
    <div id=\"player\"></div>\n\
    \n\
    <script src=\"https://www.youtube.com/iframe_api\"></script>\n\
    <script>\n\
    var player;\n\
    function onYouTubeIframeAPIReady() {\n\
    player = new YT.Player('player', {height:'100%%',\n\
    width:'100%%',\n\
    videoId:'%s',\n\
    playerVars:{\n\
    autoplay: 0,\n\
    playsinline: 0,\n\
    rel: 0,\n\
    start: %d \n\
    },\n\
    events:{'onReady': onReady, 'onStateChange': onStateChange}\n\
    });\n\
    player.setSize(window.innerWidth, window.innerHeight-50);\n\
    }\n\
    \n\
    function onReady(event) {\n\
    //event.target.playVideo();\n\
    }\n\
    \n\
    function onStateChange(event) {\n\
    //event.data\n\
    }\n\
    \n\
    window.onresize = function() {\n\
    player.setSize(window.innerWidth, window.innerHeight-50);\n\
    }\n\
    </script>\n\
    </body>\n\
    </html>";
}

void agk::PlayYoutubeVideo( const char* developerKey, const char* videoID, float startTime )
//****
{
    if ( !youtubeDelegate ) youtubeDelegate = [[YoutubeDelegate alloc] init];
    
    NSString *sYoutubeHTML = [NSString stringWithFormat:[NSString stringWithUTF8String:szYoutubeFormat], videoID, (int)startTime];
    
    WKWebViewConfiguration *config = [[WKWebViewConfiguration alloc] init];
    config.allowsInlineMediaPlayback = NO;
    config.mediaPlaybackRequiresUserAction = NO;
    config.requiresUserActionForMediaPlayback = NO;
    config.mediaTypesRequiringUserActionForPlayback = WKAudiovisualMediaTypeNone;
    int width = g_pViewController.view.frame.size.width;
    int height = g_pViewController.view.frame.size.height;
    youtubeView = [[WKWebView alloc] initWithFrame:CGRectMake(0, 0, width,height) configuration: config];
    youtubeView.navigationDelegate = youtubeDelegate;
    youtubeView.scrollView.scrollEnabled = NO;
    youtubeView.scrollView.bounces = NO;
    youtubeView.autoresizingMask = (UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight);
    [ youtubeView loadHTMLString:sYoutubeHTML baseURL:[NSURL URLWithString:@"about:blank"] ];
    [ g_pViewController setInactive ];
    [ g_pViewController.view addSubview: youtubeView ];
}

// video commands

int agk::LoadVideo( const char *szFilename )
//****
{
    g_iHasVideoStarted = 0;
    
    if ( !g_pAGKMovieListener ) g_pAGKMovieListener = [[AGKMovieListener alloc] init];
    
    if ( videoplayer )
    {
        StopVideo();
        [[NSNotificationCenter defaultCenter] removeObserver:g_pAGKMovieListener name:AVPlayerItemDidPlayToEndTimeNotification object:videoplayer.currentItem];
        [videoplayer release];
        videoplayer = 0;
    }
    
    /*
    if ( player )
    {
        StopVideo();
        [[NSNotificationCenter defaultCenter] removeObserver:g_pAGKMovieListener name:MPMoviePlayerPlaybackDidFinishNotification object:player];
        [ player release ];
        player = 0;
    }
     */
    
    g_sVideoFile.SetStr(szFilename);
    uString sPath( g_sVideoFile );
    
    if ( !GetRealPath( sPath ) )
	{
		agk::Error( "Failed to load video file - file not found" );
		return 0;
	}
    
    m_fVideoX = 0;
	m_fVideoY = 0;
	m_fVideoWidth = (float) agk::GetVirtualWidth();
	m_fVideoHeight = (float) agk::GetVirtualHeight();
    
    NSString* pString = [ NSString stringWithUTF8String:sPath ];
    NSURL *fileURL = [NSURL fileURLWithPath:pString];
    
    // new video code
    videoplayer = [[AVPlayer alloc] initWithURL:fileURL];
    [[NSNotificationCenter defaultCenter] addObserver:g_pAGKMovieListener selector:@selector(playbackFinished:) name:AVPlayerItemDidPlayToEndTimeNotification object:videoplayer.currentItem];
    
    // old video code
    /*
	
	player = [[MPMoviePlayerController alloc] initWithContentURL:fileURL];
    player.controlStyle = MPMovieControlStyleNone;
    //[player prepareToPlay];
    [player.view setFrame: CGRectMake(0,0,GetDeviceWidth(),GetDeviceHeight())];
    player.scalingMode = MPMovieScalingModeFill;
    //[player prepareToPlay];
    
    [[NSNotificationCenter defaultCenter] addObserver:g_pAGKMovieListener selector:@selector(playbackFinished:) name:MPMoviePlayerPlaybackDidFinishNotification object:player];
     */
    
    return 1;
}

void agk::HandleVideoEvents()
{
	// do nothing on ios
}

void agk::ChangeVideoPointer( void *ptr )
{
	
}

void agk::DeleteVideo()
//****
{
    if ( videoplayer )
    {
        StopVideo();
        [[NSNotificationCenter defaultCenter] removeObserver:g_pAGKMovieListener name:AVPlayerItemDidPlayToEndTimeNotification object:videoplayer.currentItem];
        [ videoplayer release ];
        videoplayer = 0;
    }
    
    ReleaseVideoImageData();
    
    if ( m_pVideoTexture )
    {
		m_cImageList.RemoveItem( m_pVideoTexture->m_iID );
        m_pVideoTexture->OverrideTexture(0, 0, 0);
        delete m_pVideoTexture;
        m_pVideoTexture = 0;
    }
    else
    {
		// clear touch events in case the video captured some, they won't get released 
		cTouch::ClearAll();
	}
}

void agk::SetVideoDimensions( float x, float y, float width, float height )
//****
{
    m_fVideoX = x;
	m_fVideoY = y;
	m_fVideoWidth = width;
	m_fVideoHeight = height;
    
    if ( !videoplayer ) return;
    if ( !videoLayer ) return;
    
    int iX = agk::ScreenToDeviceX( x );
    int iY = agk::ScreenToDeviceY( y );
    int iWidth = agk::ScreenToDeviceX( width+x ) - iX;
    int iHeight = agk::ScreenToDeviceY( height+y ) - iY;
    
    [ videoLayer setFrame: CGRectMake(iX,iY,iWidth,iHeight) ];
}

void agk::VideoUpdate()
{
    if ( !videoplayer ) return;
    if ( !videoOutput ) return;
    if ( !HasVideoImageData() ) return;
    
    if ( !g_iVideoOutputAttached )
    {
        if ( videoplayer.currentItem.status != AVPlayerItemStatusReadyToPlay )
        {
            return;
        }
        else
        {
            g_iVideoOutputAttached = 1;
            [videoplayer.currentItem addOutput:videoOutput];
        }
    }

    CMTime currTime = videoplayer.currentTime;
    if ( ![videoOutput hasNewPixelBufferForItemTime:currTime] )
    {
        return;
    }
    
    CVPixelBufferRef buffer = [videoOutput copyPixelBufferForItemTime:currTime itemTimeForDisplay:nil];
    
    if ( buffer == nil )
    {
        return;
    }
    
    CopyVideoImageToAGKImage( buffer, m_pVideoTexture );
    CVPixelBufferRelease( buffer );
}

void agk::PlayVideoToImage( UINT imageID )
//****
{
    if ( m_iVideoPlayMode == 1 )
    {
        agk::Error( "Cannot use PlayVideoToImage whilst the video is playing to the screen, use PlayVideo to resume" );
        return;
    }
    
    if ( !videoplayer ) return;
    
    CreateVideoImageData();
    
    cImage *pImage = m_cImageList.GetItem( imageID );
    if ( pImage && pImage != m_pVideoTexture )
    {
        agk::Error( "Failed to play video to image, image already exists" );
        return;
    }
    
    if ( !m_pVideoTexture )
    {
        m_pVideoTexture = new cImage();
        m_pVideoTexture->CreateBlankImage( GetVideoWidth(), GetVideoHeight(), 0, 0 );
        m_pVideoTexture->m_iID = imageID;
        m_cImageList.AddItem( m_pVideoTexture, imageID );
    }
    
    [ videoplayer play ];

    if ( !videoOutput )
    {
        NSDictionary* settings = @{ (id)kCVPixelBufferPixelFormatTypeKey : [NSNumber numberWithInt:kCVPixelFormatType_32BGRA] };
        videoOutput = [[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:settings];
        if ( videoplayer.currentItem.status == AVPlayerItemStatusReadyToPlay )
        {
            g_iVideoOutputAttached = 1;
            [videoplayer.currentItem addOutput:videoOutput];
        }
    }
    
    g_iHasVideoStarted = 1;
	m_iVideoPlayMode = 2;
}

void agk::PlayVideo()
//****
{
    if ( m_iVideoPlayMode == 2 )
    {
        agk::Error( "Cannot use PlayVideo whilst the video is playing to an image, use PlayVideoToImage to resume" );
        return;
    }
    
    if ( !videoplayer ) return;
    [ videoplayer play ];
    
    if ( !videoLayer )
    {
        videoLayer = [[AVPlayerLayer playerLayerWithPlayer:videoplayer] retain];
        videoLayer.videoGravity = AVLayerVideoGravityResize;
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)g_pMainWindow.layer;
        [ eaglLayer addSublayer:videoLayer];
        
        agk::SetVideoDimensions(m_fVideoX, m_fVideoY, m_fVideoWidth, m_fVideoHeight);
    }
    
    g_iHasVideoStarted = 1;
    m_iVideoPlayMode = 1;
}

void agk::PauseVideo()
//****
{
    if ( !videoplayer ) return;
    [ videoplayer pause ];
    
    g_iHasVideoStarted = 0;
}

void agk::StopVideo()
//****
{
    if ( !videoplayer ) return;
    [videoplayer pause];
    
    if ( videoLayer )
    {
        [videoLayer removeFromSuperlayer];
        [videoLayer release];
        videoLayer = 0;
    }
    
    if ( videoOutput )
    {
        if ( g_iVideoOutputAttached ) [videoplayer.currentItem removeOutput:videoOutput];
        [videoOutput release];
        videoOutput = 0;
        g_iVideoOutputAttached = 0;
    }
    
    [videoplayer seekToTime:CMTimeMake(0, 1)];
    
    g_iHasVideoStarted = 0;
    m_iVideoPlayMode = 0;
}

int agk::GetVideoPlaying()
//****
{
    if ( !videoplayer ) return 0;
    return g_iHasVideoStarted;
}

float agk::GetVideoPosition()
//****
{
    if ( !videoplayer ) return 0;
    return (videoplayer.currentTime.value/(float)videoplayer.currentTime.timescale);
}

float agk::GetVideoDuration()
//****
{
    if ( !videoplayer ) return 0;
    return CMTimeGetSeconds(videoplayer.currentItem.asset.duration);
}

void agk::SetVideoVolume( float volume )
//****
{
    if ( !videoplayer ) return;
    
    if ( volume < 0 ) volume = 0;
    if ( volume > 100 ) volume = 100;
    volume /= 100.0f;
    
    //videoplayer.volume = volume / 100.0; // iOS 7.0 and above only
    
    NSArray *audioTracks = videoplayer.currentItem.asset.tracks;
    NSMutableArray *allAudioParams = [NSMutableArray array];
    for (AVAssetTrack *track in audioTracks) {
        AVMutableAudioMixInputParameters *audioInputParams =[AVMutableAudioMixInputParameters audioMixInputParameters];
        [audioInputParams setVolume:volume atTime:kCMTimeZero];
        [audioInputParams setTrackID:[track trackID]];
        [allAudioParams addObject:audioInputParams];
    }
    AVMutableAudioMix *audioMix = [AVMutableAudioMix audioMix];
    [audioMix setInputParameters:allAudioParams];
    [videoplayer.currentItem setAudioMix:audioMix];
}

float agk::GetVideoWidth()
//****
{
    if ( !videoplayer ) return -1;
    
	CGSize size = [[[videoplayer.currentItem.asset tracksWithMediaType:AVMediaTypeVideo] objectAtIndex:0] naturalSize];
    return size.width;
}

float agk::GetVideoHeight()
//****
{
    if ( !videoplayer ) return -1;
    
    CGSize size = [[[videoplayer.currentItem.asset tracksWithMediaType:AVMediaTypeVideo] objectAtIndex:0] naturalSize];
    return size.height;
}

void agk::SetVideoPosition( float seconds )
//****
{
	if ( !videoplayer ) return;
    
    int32_t timeScale = videoplayer.currentItem.asset.duration.timescale;
    [videoplayer seekToTime:CMTimeMakeWithSeconds(seconds, timeScale) toleranceBefore:kCMTimeZero toleranceAfter:kCMTimeZero];
}

// Screen recording

void agk::StartScreenRecording( const char *szFilename, int microphone )
//****
{
	if ( [[[UIDevice currentDevice] systemVersion] compare:@"9.0" options:NSNumericSearch] != NSOrderedAscending )
	{
		if ( g_pScreenRecorderDelegate ) return;
		g_pScreenRecorderDelegate = [[AGKScreenRecorderDelegate alloc] init];
		[g_pScreenRecorderDelegate StartRecording:microphone];
	}
	else
	{
		agk::Warning( "Screen recording requires iOS 9.0 or above" );
	}
}

void agk::StopScreenRecording()
//****
{
	if ( !g_pScreenRecorderDelegate ) return;
	[g_pScreenRecorderDelegate StopRecording];
}

int agk::IsScreenRecording()
//****
{
	return g_pScreenRecorderDelegate ? 1 : 0;
}

// Smart Watch

void agk::ActivateSmartWatch( const char *szReserved )
//****
{
    if ( [[[UIDevice currentDevice] systemVersion] compare:@"9.0" options:NSNumericSearch] != NSOrderedAscending )
    {
        if ( !g_pWatchDelegate ) g_pWatchDelegate = [[AGKWatchDelegate alloc] init];
        [g_pWatchDelegate connect];
    }
    else
    {
        agk::Warning( "Apple smart watch requires iOS 9.0 or above" );
    }
}

int agk::GetSmartWatchState()
//****
{
    if ( !g_pWatchDelegate ) return 0;
    return [g_pWatchDelegate getState];
}

void agk::SendSmartWatchData( const char* szJson )
//****
{
    if ( !g_pWatchDelegate ) return;
    [g_pWatchDelegate sendJSON:szJson];
}

char* agk::ReceiveSmartWatchData()
//****
{
    if ( !g_pWatchDelegate )
    {
        char* str = new char[1]; *str = 0;
        return str;
    }
    
    char* szMsg = [g_pWatchDelegate getMessage];
    if ( szMsg ) return szMsg;
    else
    {
        char* str = new char[1]; *str = 0;
        return str;
    }
}

// Text to speech
void agk::TextToSpeechSetup()
//****
{
    if ( g_pTextToSpeech ) return;
    g_pSpeechDelegate = [[AGKSpeechDelegate alloc] init];
    g_pTextToSpeech = [[AVSpeechSynthesizer alloc] init];
    g_pTextToSpeech.delegate = g_pSpeechDelegate;
}

int agk::GetTextToSpeechReady()
//****
{
    return g_pTextToSpeech ? 1 : 0;
}

void agk::Speak( const char *text )
//****
{
    if ( !g_pTextToSpeech ) return;
    AVSpeechUtterance *utterance = [AVSpeechUtterance speechUtteranceWithString:[NSString stringWithUTF8String:text]];
    [utterance setRate:g_fSpeechRate];
    [utterance setVoice:g_pSpeechVoice];
    [utterance retain];
    AGKUtterance *newUtterance = new AGKUtterance();
    newUtterance->utterance = utterance; 
    newUtterance->delay = 0;
    [g_pSpeechDelegate addUtterance:newUtterance];
}

void agk::Speak( const char *text, int delay )
//****
{
    // using the AVSpeechUtterance delay feature causes a bug if the utterance is stopped whilst in the delay phase
    // which will cause speech to never be spoken again, so use our own delay method instead
    
    if ( !g_pTextToSpeech ) return;
    AVSpeechUtterance *utterance = [AVSpeechUtterance speechUtteranceWithString:[NSString stringWithUTF8String:text]];
    [utterance setRate:g_fSpeechRate];
    [utterance setVoice:g_pSpeechVoice];
    [utterance retain];
    AGKUtterance *newUtterance = new AGKUtterance();
    newUtterance->utterance = utterance;
    newUtterance->delay = delay / 1000.0;
    [g_pSpeechDelegate addUtterance:newUtterance];
}

void agk::SetSpeechRate( float rate )
//****
{
    if ( !g_pTextToSpeech ) return;
    g_fSpeechRate = rate * AVSpeechUtteranceDefaultSpeechRate;
}

int agk::GetSpeechNumVoices()
//****
{
    return (int) [[AVSpeechSynthesisVoice speechVoices] count];
}

char* agk::GetSpeechVoiceLanguage( int index )
//****
{
    if ( index < 0 || index >= [[AVSpeechSynthesisVoice speechVoices] count] )
    {
        char *str = new char[1]; *str = 0;
        return str;
    }

    AVSpeechSynthesisVoice* voice = [[AVSpeechSynthesisVoice speechVoices] objectAtIndex:index];
	if ( !voice )
	{
        char *str = new char[1]; *str = 0;
        return str;
    }
    
    const char* lang = [[voice language] UTF8String];
	if ( !lang )
	{
        char *str = new char[1]; *str = 0;
        return str;
    }

    char* str = new char[ strlen(lang)+1 ];
    strcpy( str, lang );
    return str;
}

char* agk::GetSpeechVoiceName( int index )
//****
{
    if ( index < 0 || index >= [[AVSpeechSynthesisVoice speechVoices] count] )
    {
        char *str = new char[1]; *str = 0;
        return str;
    }
    
    AVSpeechSynthesisVoice* voice = [[AVSpeechSynthesisVoice speechVoices] objectAtIndex:index];
	if ( !voice || ![voice respondsToSelector:@selector(name)] || ![voice name] )
	{
        char *str = new char[1]; *str = 0;
        return str;
    }
    
    const char* name = [[voice name] UTF8String];
	if ( !name )
	{
        char *str = new char[1]; *str = 0;
        return str;
    }
    
    char* str = new char[ strlen(name)+1 ];
    strcpy( str, name );
    return str;
}

char* agk::GetSpeechVoiceID( int index )
//****
{
    if ( index < 0 || index >= [[AVSpeechSynthesisVoice speechVoices] count] )
    {
        char *str = new char[1]; *str = 0;
        return str;
    }
    
    AVSpeechSynthesisVoice* voice = [[AVSpeechSynthesisVoice speechVoices] objectAtIndex:index];
    if ( !voice || ![voice respondsToSelector:@selector(identifier)] || ![voice identifier] )
    {
        char *str = new char[1]; *str = 0;
        return str;
    }
    
    const char* sID = [[voice identifier] UTF8String];
    char* str = new char[ strlen(sID)+1 ];
    strcpy( str, sID );
    return str;
}

void agk::SetSpeechLanguage( const char* lang )
//****
{
    if ( !g_pTextToSpeech ) return;
    if ( g_pSpeechVoice ) [g_pSpeechVoice release];
    uString sLang(lang);
    sLang.Replace('_','-');
    g_pSpeechVoice = [AVSpeechSynthesisVoice voiceWithLanguage:[NSString stringWithUTF8String:sLang.GetStr()]];
    [g_pSpeechVoice retain];
}

void agk::SetSpeechLanguageByID( const char* sID )
//****
{
	if ( !g_pTextToSpeech ) return;
    if ( g_pSpeechVoice ) [g_pSpeechVoice release];
    g_pSpeechVoice = nil;
    if ( ![AVSpeechSynthesisVoice respondsToSelector:@selector(voiceWithIdentifier:)] ) return;
    g_pSpeechVoice = [AVSpeechSynthesisVoice voiceWithIdentifier:[NSString stringWithUTF8String:sID]];
    [g_pSpeechVoice retain];
}

int agk::IsSpeaking()
//****
{
    if ( !g_pTextToSpeech ) return 0;
    if ( !g_pSpeechDelegate ) return 0;
    return g_pSpeechDelegate->m_iIsSpeaking;
}

void agk::StopSpeaking()
//****
{
	if ( !g_pSpeechDelegate ) return;
    [g_pSpeechDelegate stopAll];
}

int uString::ToInt() const
{
	//NSString* pString = [ [ NSString alloc ] initWithCString: m_pData.GetStr() length: m_pData.GetLength() * sizeof ( char ) ];
	//return [string intValue];
	return atoi(m_pData);
}

float uString::ToFloat() const
{
	return (float)atof(m_pData);
}

void agk::PlatformReportError( const uString &sMsg )
{
	NSLog(@"%s\n", sMsg.GetStr());
}

void agk::PlatformMessage( const char* msg )
{
	NSString* pString = [ [ NSString alloc ] initWithUTF8String: msg ];
/*
    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"Message" message:pString preferredStyle:UIAlertControllerStyleAlert];
    UIAlertAction *ok = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault handler:nil];
    [alert addAction:ok];
    [g_pViewController presentViewController:alert animated:YES completion:nil];
*/
	UIAlertView *alert = [[UIAlertView alloc] init];
	[alert setTitle:@"Message"];
	[alert setMessage:pString];
	[alert setDelegate:nil];
	[alert addButtonWithTitle:@"Ok"];
	[alert show];
	[alert release];
    [ pString release ];
}

// Thread functions

void AGKThread::PlatformInit( )
{
	pThread = [[ iOSThread alloc ] initWithThread:this ];
	m_pStop = [[NSConditionLock alloc] initWithCondition: 0];
}

void AGKThread::PlatformStart( )
{
	if ( pThread ) 
	{
		//[ ((iOSThread*)pThread) free ]; 
		[ ((iOSThread*)pThread) release ];
	}
	
	pThread = [[ iOSThread alloc ] initWithThread:this ];
	
	[ (NSConditionLock*)m_pStop lock ];
	[ (NSConditionLock*)m_pStop unlockWithCondition: 0 ];
	[ ((iOSThread*)pThread)->finished lock ];
	[ ((iOSThread*)pThread)->finished unlockWithCondition: 0 ];
	[ (iOSThread*)pThread start ];
}

void AGKThread::PlatformStop( )
{
	[ (NSConditionLock*)m_pStop lock ];
	[ (NSConditionLock*)m_pStop unlockWithCondition: 1 ];
}

void AGKThread::PlatformTerminate( )
{
	if ( pThread )
	{
		if ( m_bRunning ) 
		{
			agk::Error("Forcing a thread to terminate, this may cause a crash...");
			//TerminateThread( pThread, 0 );
		}
		//CloseHandle( pThread );
	}
}

void AGKThread::PlatformCleanUp( )
{
	if ( m_pStop ) [ (NSConditionLock*)m_pStop release ];
	if ( pThread ) 
	{
		//[ (iOSThread*)pThread free ];
		[ (iOSThread*)pThread release ];
	}
}

void AGKThread::PlatformJoin( )
{
	[ ((iOSThread*)pThread)->finished lockWhenCondition:1 ];
	[ ((iOSThread*)pThread)->finished unlockWithCondition:1 ];
}

void AGKThread::PlatformSleepSafe( UINT milliseconds )
{
	NSTimeInterval interval = milliseconds / 1000.0f;
	BOOL result = [ (NSConditionLock*)m_pStop lockWhenCondition:1 beforeDate:[NSDate dateWithTimeIntervalSinceNow:interval] ];
	if ( result ) [ (NSConditionLock*)m_pStop unlockWithCondition:1 ];
}


// file system commands

void agk::SetRawWritePath( const char* str )
{
    // do nothing on iOS
    agk::Warning( "SetRawWritePath is deprecated, the preferred method is now OpenRawFolder and \"raw:\" file paths" );
}

char* agk::GetWritePath()
{
	// allow write directory to be determined
	char *szStr = new char[ strlen(szWriteDir)+1 ];
	strcpy( szStr, szWriteDir );
	return szStr;
	//return 0;
}

char* agk::GetReadPath()
{
	char *szStr = new char[ strlen(szRootDir)+1 ];
	strcpy( szStr, szRootDir );
	return szStr;
}

char* agk::GetDocumentsPath()
{
	char *szStr = new char[ strlen(szWriteDir)+1 ];
	strcpy( szStr, szWriteDir );
	return szStr;
}

bool agk::PlatformChooseFile( uString &out, const char *ext, int fullPath )
{
    // do nothing on iOS
    return false;
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

bool cFile::ExistsWrite( const char *szFilename )
{
	if ( !szFilename || !*szFilename ) return false;
	if ( strncmp(szFilename, "raw:", 4) == 0 ) return false;
	
	if ( strchr( szFilename, ':' ) ) return false;
	if ( strstr(szFilename, "..\\") || strstr(szFilename, "../") ) return false;
	
	size_t length = strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' ) return false;
	
    uString sPath( szFilename );
    agk::PlatformGetFullPathWrite(sPath);
    	
	struct stat buf;
	if ( stat( sPath.GetStr(), &buf ) != 0 ) return false;
	
	//FILE *pFile = AGKfopen( sPath.GetStr(), "rb" );
	//if ( !pFile ) return false;
	//fclose( pFile );

	return true;
}

bool cFile::ExistsRead( const char *szFilename, int *mode )
{
	if ( !szFilename || !*szFilename ) return false;
	if ( strncmp(szFilename, "raw:", 4) == 0 ) return false;
	
	if ( strchr( szFilename, ':' ) ) return false;
	if ( strstr(szFilename, "..\\") || strstr(szFilename, "../") ) return false;
	
	size_t length = strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' ) return false;
	
	// try resources folder with path
	uString sPath(szFilename);
    agk::PlatformGetFullPathRead(sPath);
	
	if ( mode ) *mode = 0;
	struct stat buf;
	if ( stat( sPath.GetStr(), &buf ) != 0 ) 	
	{
		// try resource folder without path
		uString str( szFilename );
		RemovePath( str );
		sPath.SetStr( szRootDir );
		sPath.Append( str );
		cFileEntry::ConvertToReal( sPath );
		
		if ( mode ) *mode = 1;
		if ( stat( sPath.GetStr(), &buf ) != 0 ) return false;
	}
	
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

bool cFile::Exists( const char *szFilename )
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

bool cFile::GetModified( const char *szFilename, int &time )
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

UINT cFile::GetFileSize( const char *szFilename )
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
	
	NSString* pPath = [ [ NSString alloc ] initWithUTF8String:sPath ];
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
	return (UINT)ftell( pFile );
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
	size_t size = ftell( pFile );
	fsetpos( pFile, &pos );
	return (UINT) size;
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
	size_t length = strlen( str );
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
	size_t length = strlen( str );
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

	while ( !IsEOF() && str.ByteAt( str.GetLength()-1 ) != '\n' )
	{
		if ( !fgets( tempstr, 1024, pFile ) ) break;
		str.Append( tempstr );
	}

	str.Trim( "\r\n" );
	return str.GetLength();
}
*/
int AGK::cFile::ReadData( char *str, UINT length )
{
	if ( !pFile ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "cFile::ReadData() cannot not read from file, file not open" );
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
	
	return (UINT)fread( str, 1, length, pFile );
}

// directory commands

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
	
	NSString* pPathString = [ [ NSString alloc ] initWithUTF8String:sPath.GetStr() ];
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
				pNewItem->m_sItem.SetStr( [ pString cStringUsingEncoding:NSUTF8StringEncoding ] );
				pNewItem->m_pNext = m_pCurrentDirectories;
				m_pCurrentDirectories = pNewItem;
			}
			else
			{
				// file
				cDirectoryItem *pNewItem = new cDirectoryItem();
				pNewItem->iFlags |= AGK_FILE_TYPE_READ;
				pNewItem->m_sItem.SetStr( [ pString cStringUsingEncoding:NSUTF8StringEncoding ] );
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
	
	pPathString = [ [ NSString alloc ] initWithUTF8String:sPath.GetStr() ];
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
					if ( pItem->m_sItem.CompareTo( [ pString cStringUsingEncoding:NSUTF8StringEncoding ] ) == 0 )
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
					pNewItem->m_sItem.SetStr( [ pString cStringUsingEncoding:NSUTF8StringEncoding ] );
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
					if ( pItem->m_sItem.CompareTo( [ pString cStringUsingEncoding:NSUTF8StringEncoding ] ) == 0 )
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
					pNewItem->m_sItem.SetStr( [ pString cStringUsingEncoding:NSUTF8StringEncoding ] );
					pNewItem->m_pNext = m_pCurrentFiles;
					m_pCurrentFiles = pNewItem;
				}
			}
		}
	}
	
	[ pPathString release ];
}

//****f* File/Directory/SetCurrentDir
// FUNCTION
//   Sets the current working directory for the current app. All media loading will be done relative to the 
//   current directory. e.g. setting the current directory to "images" and then using LoadImage with "background.png"
//   would attempt to load "images/background.png". This applies to both the read and write directory, so using
//   OpenToWrite() with the previous example would also write to the "images" folder.
//   You can not use absolute paths like "c:\myfolder", as the application has a self contained read folder containing
//   its media, and a self contained write folder for new files. To reset the current directory back to the application's
//   default path use SetCurrentDir with an empty string.
//   If the given directory does not exist this command will return 0 and remain in its current directory. Otherwise it
//   will return 1. The directory only has to exist in one of the locations (read and write folders) for this to succeed.
// INPUTS
//   szPath -- The directory to use as the new current directory.
// SOURCE
int agk::SetCurrentDir( const char* szPath )
//****
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
//#ifdef _AGK_ERROR_CHECK
//		agk::Error( "Invalid path for SetCurrentDir, must not start with a forward or backward slash" );
//#endif
//		return 0;
        
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
	//BAG Hack
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
	size_t length = strlen( szPath );
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
	return 1;
}

int agk::GetMouseExists()
{
	return 0;
}

int agk::GetKeyboardExists()
{
	return 2;
}

int agk::GetCameraExists()
{
    if ( ![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera] )
    {
        return 0;
    }
    else 
    {
        return 1;
    }
}

int agk::GetGPSSensorExists()
{
	return m_iGPSSensorExists;
}

void cJoystick::DetectJoysticks()
{

}

void cJoystick::PlatformUpdate()
{
	// do nothing
}

void agk::SetRawMouseVisible( int visible )
{
	// do nothing on iOS
}

void agk::SetRawMousePosition( float x, float y )
{
	// do nothing on iOS
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

// advert commands
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
	// if the edit box is in the lower half of the screen use the normal text entry, unless alternate methods are turned off
    
    g_bMultiline = GetMultiLine();
    g_iInputType = GetInputType();

	float topY = m_fY+m_fHeight;
	if ( !m_bFixed ) topY = agk::WorldToScreenY( topY );
    
	if ( !m_bUseAlternateInput || topY < agk::GetVirtualHeight()/2 )
	{
		g_bEditBoxHack = true;
		agk::PlatformDrawTextInput();
	}
    else g_bPasswordMode = m_bIsPassword;

    agk::StartTextInput( m_sCurrInput );
    
	// StartTextInput resets this flag
	if ( !m_bUseAlternateInput || topY < agk::GetVirtualHeight()/2 ) g_bEditBoxHack = true;
    
    m_iCursorPos = m_sCurrInput.GetNumChars();
    m_iLastLength = m_sCurrInput.GetNumChars();
    
    bUpdateEditBox = true;
}

void cEditBox::PlatformEndText()
{
	// do nothing on iOS
}

void cEditBox::PlatformUpdateExternal()
{
	bUpdateEditBox = true;
}

bool cEditBox::PlatformUpdateText()
{
    if ( agk::GetTextInputCompleted() ) SetFocus(false); //m_bActive = false;
    
    agk::GetTextInput( m_sCurrInput );
    
    bool bChanged = false;
    
    m_pInputText->SetString( m_sCurrInput );
	while ( (m_iMaxLines > 0 && m_pInputText->GetLines() > m_iMaxLines) || (m_iMaxChars > 0 && m_sCurrInput.GetNumChars() > m_iMaxChars) )
	{
		m_sCurrInput.Trunc2( 1 );
        m_pInputText->SetString( m_sCurrInput );
		bChanged = true;
	}
    
    if ( !m_bSupportsExtendedAscii && (m_bFlags & AGK_EDITBOX_USING_NEW_FONT) == 0 )
    {
        int length = m_sCurrInput.GetNumChars();
        m_sCurrInput.StripUTF8();
        if ( length != m_sCurrInput.GetNumChars() )
        {
            if ( !m_bSupportWarned )
            {
                m_bSupportWarned = true;
                agk::Message("This edit box does not support extended characters");
            }
            m_pInputText->SetString( m_sCurrInput );
            bChanged = true;
        }
    }
    
    if ( bChanged )
	{
		agk::PlatformChangeTextInput( m_sCurrInput );
	}
    
    if ( !bUpdateEditBox && m_iLastLength != m_sCurrInput.GetNumChars() )
    {
        m_iCursorPos += m_sCurrInput.GetNumChars() - m_iLastLength;
        if ( m_iCursorPos < 0 ) m_iCursorPos = 0;
        
        if ( g_bMultiline ) pTextView.selectedRange = NSMakeRange(m_iCursorPos,0);
        else [ pTextField setValue:[NSValue valueWithRange:NSMakeRange(m_iCursorPos,0)] forKey:@"selectionRange" ];
    }
    m_iLastLength = m_sCurrInput.GetNumChars();
    
    if ( bUpdateEditBox )
    {
        if ( g_bMultiline ) pTextView.selectedRange = NSMakeRange(m_iCursorPos,0);
        else [ pTextField setValue:[NSValue valueWithRange:NSMakeRange(m_iCursorPos,0)] forKey:@"selectionRange" ];
    }
    bUpdateEditBox = false;
    
    return true;
}

void cEditBox::PlatformUpdateTextEnd()
{
	// do nothing
}

void agk::OpenBrowser( const char* url )
{
    uString sURL( url );
	if ( sURL.FindStr( ":" ) < 0 ) sURL.Prepend( "http://" );

	sURL.ReplaceStr( " ", "%20" ); // iOS doesn't like unencoded spaces
    
    NSURL *uURL = [NSURL URLWithString:[NSString stringWithCString:sURL.GetStr() encoding:NSUTF8StringEncoding]];
    [[UIApplication sharedApplication] openURL:uURL];
}

UINT agk::RunApp( const char *szFilename, const char *szParameters )
{
    return 0;
}

UINT agk::GetAppRunning( UINT appID )
{
    return 0;
}

void agk::TerminateApp( UINT appID )
{

}

@interface AGKDocumentInteractionDelegate : NSObject <UIDocumentInteractionControllerDelegate>{}@end
@implementation AGKDocumentInteractionDelegate
- (UIViewController *)documentInteractionControllerViewControllerForPreview:(UIDocumentInteractionController *)controller
{
    return g_pViewController;
}
- (void)documentInteractionControllerDidEndPreview:(UIDocumentInteractionController *)controller
{
    //NSLog(@"Did end preview");
    [controller autorelease];
}
@end

AGKDocumentInteractionDelegate *g_pDocumentDelegate = 0;

void agk::ViewFile( const char* szFilename )
{
    uString sPath( szFilename );
    if ( !GetRealPath( sPath ) )
    {
		uString err;
		err.Format( "Could not find file at path: %s", szFilename );
		agk::Error( err );
		return;
    }
    
    if ( !g_pDocumentDelegate ) g_pDocumentDelegate = [[AGKDocumentInteractionDelegate alloc] init];
    
    NSURL *resourceToOpen = [NSURL fileURLWithPath:[NSString stringWithUTF8String:sPath.GetStr()]];
    UIDocumentInteractionController *documentController = [UIDocumentInteractionController interactionControllerWithURL:resourceToOpen];
    documentController.delegate = g_pDocumentDelegate;
    [documentController presentPreviewAnimated:YES];
    [documentController retain];
}

void agk::ShareText( const char* szText )
{
    NSString *message   = [NSString stringWithUTF8String:szText];
    NSArray *postItems  = @[message];
    
    UIActivityViewController *activityVc = [[UIActivityViewController alloc] initWithActivityItems:postItems applicationActivities:nil];
    
    [activityVc setCompletionHandler:^(NSString *actType, BOOL completed){ [g_pViewController setActive]; }];
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad && [activityVc respondsToSelector:@selector(popoverPresentationController)] )
    {
        UIPopoverController *popup = [[UIPopoverController alloc] initWithContentViewController:activityVc];
        
        [popup presentPopoverFromRect:CGRectMake(g_pViewController.view.frame.size.width/2, g_pViewController.view.frame.size.height/4, 0, 0)
                               inView:[UIApplication sharedApplication].keyWindow.rootViewController.view permittedArrowDirections:UIPopoverArrowDirectionUnknown animated:YES];
        [g_pViewController setInactive];
    }
    else
    {
        [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:activityVc animated:YES completion:nil];
        [g_pViewController setInactive];
    }
    
    [activityVc release];
}

void agk::ShareImage( const char* szFilename )
{
    uString sPath( szFilename );
    if ( !GetRealPath( sPath ) )
	{
		uString err; err.Format( "Could not find file at path: %s", szFilename );
		agk::Error( err );
		return;
	}
    
    NSString *imagePath = [NSString stringWithUTF8String:sPath.GetStr()];
    UIImage *image = [UIImage imageWithContentsOfFile:imagePath];
    NSArray *postItems  = @[image];
    
    UIActivityViewController *activityVc = [[UIActivityViewController alloc] initWithActivityItems:postItems applicationActivities:nil];
    
    [activityVc setCompletionHandler:^(NSString *actType, BOOL completed){ [g_pViewController setActive]; }];
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad && [activityVc respondsToSelector:@selector(popoverPresentationController)] )
    {
        UIPopoverController *popup = [[UIPopoverController alloc] initWithContentViewController:activityVc];
        
        [popup presentPopoverFromRect:CGRectMake(g_pViewController.view.frame.size.width/2, g_pViewController.view.frame.size.height/4, 0, 0)
                               inView:[UIApplication sharedApplication].keyWindow.rootViewController.view permittedArrowDirections:UIPopoverArrowDirectionUnknown animated:YES];
        [g_pViewController setInactive];
    }
    else
    {
        [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:activityVc animated:YES completion:nil];
        [g_pViewController setInactive];
    }
    
    [activityVc release];
}

void agk::ShareImageAndText( const char* szFilename, const char* szText )
{
    uString sPath( szFilename );
    if ( !GetRealPath( sPath ) )
	{
		uString err; err.Format( "Could not find file at path: %s", szFilename );
		agk::Error( err );
		return;
	}
    
    NSString *imagePath = [NSString stringWithUTF8String:sPath.GetStr()];
    UIImage *image = [UIImage imageWithContentsOfFile:imagePath];

	NSString *message   = [NSString stringWithUTF8String:szText];
    NSArray *postItems  = @[image,message];
    
    UIActivityViewController *activityVc = [[UIActivityViewController alloc] initWithActivityItems:postItems applicationActivities:nil];
    
    [activityVc setCompletionHandler:^(NSString *actType, BOOL completed){ [g_pViewController setActive]; }];
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad && [activityVc respondsToSelector:@selector(popoverPresentationController)] )
    {
        UIPopoverController *popup = [[UIPopoverController alloc] initWithContentViewController:activityVc];
        
        [popup presentPopoverFromRect:CGRectMake(g_pViewController.view.frame.size.width/2, g_pViewController.view.frame.size.height/4, 0, 0)
                               inView:[UIApplication sharedApplication].keyWindow.rootViewController.view permittedArrowDirections:UIPopoverArrowDirectionUnknown animated:YES];
        [g_pViewController setInactive];
    }
    else
    {
        [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:activityVc animated:YES completion:nil];
        [g_pViewController setInactive];
    }
    
    [activityVc release];
}

void agk::ShareFile( const char* szFilename )
//****
{
	uString sPath( szFilename );
    if ( !GetRealPath( sPath ) )
	{
		uString err; err.Format( "Could not find file at path: %s", szFilename );
		agk::Error( err );
		return;
	}
    
    NSString *filePath = [NSString stringWithUTF8String:sPath.GetStr()];
    NSURL *file = [NSURL fileURLWithPath:filePath];
	NSMutableArray *postItems = [NSMutableArray array];
	[postItems addObject:file];
    
    UIActivityViewController *activityVc = [[UIActivityViewController alloc] initWithActivityItems:postItems applicationActivities:nil];
    
    [activityVc setCompletionHandler:^(NSString *actType, BOOL completed){ [g_pViewController setActive]; }];
    
    if (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad && [activityVc respondsToSelector:@selector(popoverPresentationController)] )
    {
        UIPopoverController *popup = [[UIPopoverController alloc] initWithContentViewController:activityVc];
        
        [popup presentPopoverFromRect:CGRectMake(g_pViewController.view.frame.size.width/2, g_pViewController.view.frame.size.height/4, 0, 0)
                               inView:[UIApplication sharedApplication].keyWindow.rootViewController.view permittedArrowDirections:UIPopoverArrowDirectionUnknown animated:YES];
        [g_pViewController setInactive];
    }
    else
    {
        [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:activityVc animated:YES completion:nil];
        [g_pViewController setInactive];
    }
    
    [activityVc release];
}

void agk::FacebookActivateAppTracking()
//****
{
#ifndef LITEVERSION
    [FBAppEvents activateApp];
#endif
}

int agk::GetInternetState()
//****
{
	struct ifaddrs *interfaces = NULL;
    struct ifaddrs *temp_addr = NULL;
    int success = 0;
    
    uString sIP;
    
    // retrieve the current interfaces - returns 0 on success
    success = getifaddrs(&interfaces);
    if (success == 0) {
        // Loop through linked list of interfaces
        temp_addr = interfaces;
        while (temp_addr != NULL) {
            if( temp_addr->ifa_addr->sa_family == AF_INET) {
                sIP.SetStr( inet_ntoa(((struct sockaddr_in *)temp_addr->ifa_addr)->sin_addr) );
				if (sIP.CompareTo( "127.0.0.1" ) != 0 && sIP.CompareTo( "0.0.0.0" ) != 0 )
                {
					freeifaddrs(interfaces);
					return 1;
                }
            }
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
                                
                if ( level > 2 )
                {
                    freeifaddrs(interfaces);
					return 1;
                }
            }
            
            temp_addr = temp_addr->ifa_next;
        }
    }
    
    // Free memory
    freeifaddrs(interfaces);
    return 0;
}

void agk::SetPushNotificationKeys( const char* data1, const char* reserved )
//****
{
	// do nothing on ios
}

int agk_PushNotificationsSetup = 0;
int agk_LocalNotificationsSetup = 0;

//****f* Extras/PushNotifications/PushNotificationSetup
// FUNCTION
//   Returns 1 if setup was successful, 0 if it failed or the device does not support push notifications.
// SOURCE
int agk::PushNotificationSetup()
//****
{
#ifndef LITEVERSION
    
    if ([[UIApplication sharedApplication] respondsToSelector:@selector(registerUserNotificationSettings:)])
    {
        agk_PushNotificationsSetup = 1;
        UIUserNotificationType notifTypes = UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert;
        if ( agk_LocalNotificationsSetup ) notifTypes |= (UIUserNotificationTypeAlert | UIUserNotificationTypeSound);
        
        UIUserNotificationSettings *settings = [UIUserNotificationSettings settingsForTypes:notifTypes categories:nil];
        
        [[UIApplication sharedApplication] registerUserNotificationSettings:settings];
    } else {
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:(UIRemoteNotificationType)(UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound | UIRemoteNotificationTypeAlert)];
    }
    
    return 1;
#else
    agk::Message("Push notifications are not available in the lite build of AGK, please use the full version");
	return 0;
#endif
}

//****f* Extras/PushNotifications/GetPushNotificationToken
// FUNCTION
//   Returns the push notification token for this device, this must be sent to your server that sends out the 
//   notifications so that it can send notifications to this device.
//   If PushNotificationSetup returned 1 then you should keep calling this command until it returns a non-empty
//   string. If PushNotificationSetup returned 0 then this command will always return an empty string.
// SOURCE
char* agk::GetPushNotificationToken()
//****
{
	char *str = new char[ m_sPNToken.GetLength()+1 ];
	strcpy( str, m_sPNToken.GetStr() );
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
        
        if ([g_pLocationManager respondsToSelector:@selector(requestWhenInUseAuthorization)])
        {
            [g_pLocationManager requestWhenInUseAuthorization];
        }
        
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

#ifndef LITEVERSION

@interface AGKGameCenterDelegate : NSObject <GKGameCenterControllerDelegate>
{
@public
	
}

@end

@implementation AGKGameCenterDelegate
- (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)viewController
{
    //[g_pViewController dismissModalViewControllerAnimated: YES];
    //[viewController release];

    [g_pViewController dismissViewControllerAnimated:YES completion:^{
        viewController.gameCenterDelegate = nil;
        [viewController release];
    }];
    
    [g_pViewController setActive];
    agk::Resumed();
}

@end

AGKGameCenterDelegate *g_pGameCenterDelegate = 0;

#endif

int agk::GetGameCenterExists()
{
	return 1;
}

void agk::GameCenterSetup()
{
#ifndef LITEVERSION
    m_iGameCenterLogin = 0;
	if ( !g_pGameCenterDelegate ) g_pGameCenterDelegate = [[AGKGameCenterDelegate alloc] init];
#else
    agk::Message("Game Center is not available in the lite build of AGK, please use the full version");
#endif
}

void agk::GameCenterLogin()
{
#ifndef LITEVERSION
    if ([GKLocalPlayer localPlayer].authenticated == YES)
    {
        m_iGameCenterLogin = 1;
        return;
    }
    
    if ( m_iGameCenterLogin < 0 )
    {
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString:@"gamecenter:"]];
        return;
    }
    
    m_iGameCenterLogin = 0;
    
    [[GKLocalPlayer localPlayer] setAuthenticateHandler:(^(UIViewController* viewcontroller, NSError *error) {
        if (viewcontroller)
        {
            if ( m_iGameCenterLogin < 0 ) return; // if the user cancelled then we send them to the GameCenter app instead
            [g_pViewController presentViewController:viewcontroller animated:YES completion:nil];
            [g_pViewController setInactive];
        }
        else
        {
            if ([GKLocalPlayer localPlayer].authenticated == YES) m_iGameCenterLogin = 1;
            else m_iGameCenterLogin = -1;
            [g_pViewController setActive];
        }
    })];
    
    /*
     // old way for iOS 5.1.1 and below
	[[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error){
        if (error ==nil) {
            NSLog(@"GameCenter Success");
            m_iGameCenterLogin = 1;
        } else {
            NSLog(@"GameCenter Fail");
            m_iGameCenterLogin = -1;
        }
        
    }];
     */
#endif
}

void agk::GameCenterLogout()
{
	
}

int agk::GetGameCenterLoggedIn()
{
#ifndef LITEVERSION
	return m_iGameCenterLogin;
#else
    return 0;
#endif
}

char* agk::GetGameCenterPlayerID()
{
#ifndef LITEVERSION
	NSString *playerID = [[GKLocalPlayer localPlayer] playerID];
	const char *str = [playerID UTF8String];
    if ( !str )
    {
        char *str = new char[1]; *str = 0;
        return str;
    }
	char *szPlayerID = new char[ strlen(str)+1 ];
	strcpy( szPlayerID, str );
	return szPlayerID;
#else
	char *str = new char[1]; *str = 0;
	return str;
#endif
}

char* agk::GetGameCenterPlayerDisplayName()
{
#ifndef LITEVERSION
	NSString *playerName = [[GKLocalPlayer localPlayer] displayName];
	const char *str = [playerName UTF8String];
	char *szPlayerName = new char[ strlen(str)+1 ];
	strcpy( szPlayerName, str );
	return szPlayerName;
#else
	char *str = new char[1]; *str = 0;
	return str;
#endif
}

void agk::GameCenterSubmitScore( int iScore, const char* szBoardID )
{
#ifndef LITEVERSION
    NSString *pString = [NSString stringWithCString:szBoardID encoding:NSUTF8StringEncoding];
	GKScore *myScoreValue = [[[GKScore alloc] initWithCategory:pString] autorelease];
    myScoreValue.value = iScore;
    
    [myScoreValue reportScoreWithCompletionHandler:^(NSError *error){
        if(error != nil){
            NSLog(@"Score Submission Failed");
        } else {
            NSLog(@"Score Submitted");
        }
        
    }];
#endif
}

void agk::GameCenterShowLeaderBoard ( const char* szBoardID )
{
#ifndef LITEVERSION
	GKGameCenterViewController *gcViewController = [[GKGameCenterViewController alloc] init];
    
    gcViewController.gameCenterDelegate = g_pGameCenterDelegate;
    
    gcViewController.viewState = GKGameCenterViewControllerStateLeaderboards;
    if ( !szBoardID || strlen(szBoardID) == 0 ) gcViewController.leaderboardCategory = nil;
    else
    {
        NSString *pString = [NSString stringWithCString:szBoardID encoding:NSUTF8StringEncoding];
        gcViewController.leaderboardCategory = pString;
    }
    
    [g_pViewController presentViewController:gcViewController animated:YES completion:nil];
    [g_pViewController setInactive];
#endif
}

void agk::GameCenterSubmitAchievement ( const char* szAchievementID, int iPercentageComplete )
{
#ifndef LITEVERSION
    NSString *pString = [NSString stringWithCString:szAchievementID encoding:NSUTF8StringEncoding];
	GKAchievement *achievement= [[[GKAchievement alloc] initWithIdentifier:pString] autorelease];
    if(achievement!= NULL)
    {
        achievement.percentComplete = iPercentageComplete;
        [achievement reportAchievementWithCompletionHandler: ^(NSError *error){
            if(error != nil){
                NSLog(@"Achievement failed");
            } else {
                NSLog(@"Achievement Success");
            }
            
        }];
    }
#endif
}

void agk::GameCenterAchievementsShow ( void )
{
#ifndef LITEVERSION
	GKGameCenterViewController *gcViewController = [[GKGameCenterViewController alloc] init];
    
    gcViewController.gameCenterDelegate = g_pGameCenterDelegate;
    gcViewController.viewState = GKGameCenterViewControllerStateAchievements;
    
    [g_pViewController presentViewController:gcViewController animated:YES completion:nil];
    [g_pViewController setInactive];
#endif
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
                    if ( sIP.GetLength() == 0 || strcmp(temp_addr->ifa_name, "en0") == 0 )
                    {
                        sIP.SetStr( szIP );
                    }
                }
            }
            
            temp_addr = temp_addr->ifa_next;
        }
    }
    
    // Free memory
    freeifaddrs(interfaces);
    
    if ( sIP.GetLength() == 0 ) return 0;
	
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
    
    if ( sIP.GetLength() == 0 ) return 0;
    
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
    if ( g_iCloudSetup ) return;
    g_iCloudSetup = 1;
    
    g_iCloudDataChanged = 0;
    [[NSNotificationCenter defaultCenter]
     addObserverForName:
     NSUbiquitousKeyValueStoreDidChangeExternallyNotification
     object:[NSUbiquitousKeyValueStore defaultStore]
     queue:nil
     usingBlock:^(NSNotification __strong *notification) {
         NSDictionary *userInfo = [notification userInfo];
         NSUInteger reason = [[userInfo objectForKey:
                               NSUbiquitousKeyValueStoreChangeReasonKey] intValue];
         NSArray *keys = [userInfo objectForKey:
                          NSUbiquitousKeyValueStoreChangedKeysKey];
         
         if (reason == NSUbiquitousKeyValueStoreServerChange
          || reason == NSUbiquitousKeyValueStoreAccountChange
          || reason == NSUbiquitousKeyValueStoreInitialSyncChange)
         {
             if (keys.count > 0) g_iCloudDataChanged = 1;
         }
     }];
    [[NSUbiquitousKeyValueStore defaultStore] synchronize];
}

int agk::GetCloudDataAllowed()
{
    NSFileManager* fileManager = [NSFileManager defaultManager];
    if( fileManager.ubiquityIdentityToken == nil ) return -2;
	return 1;
}

int agk::GetCloudDataChanged()
{
	return g_iCloudDataChanged;
}

char* agk::GetCloudDataVariable( const char* varName, const char* defaultValue )
{
    g_iCloudDataChanged = 0;
    NSUbiquitousKeyValueStore *keystore = [NSUbiquitousKeyValueStore defaultStore];
    NSString *value = [keystore stringForKey:[NSString stringWithUTF8String:varName]];
    if ( !value )
    {
        if ( !defaultValue || !*defaultValue )
        {
            char *str = new char[1]; *str = 0;
            return str;
        }
        else
        {
            char *str = new char[strlen(defaultValue)+1];
            strcpy( str, defaultValue );
            return str;
        }
    }
    
    const char *valueStr = [value UTF8String];
    char *str = new char[strlen(valueStr)+1];
    strcpy( str, valueStr );
    return str;
}

void agk::SetCloudDataVariable( const char* varName, const char* varValue )
{
	NSUbiquitousKeyValueStore *keystore = [NSUbiquitousKeyValueStore defaultStore];
    [keystore setString:[NSString stringWithUTF8String:varValue] forKey:[NSString stringWithUTF8String:varName]];
    if ( [keystore synchronize] == NO ) NSLog( @"Failed to synchronize cloud data" );
}

void agk::DeleteCloudDataVariable( const char* varName )
{
	NSUbiquitousKeyValueStore *keystore = [NSUbiquitousKeyValueStore defaultStore];
    [keystore removeObjectForKey:[NSString stringWithUTF8String:varName]];
    [keystore synchronize];
}

// Shared variables

void agk::SetSharedVariableAppGroup( const char* group )
{
	m_sSharedAppGroup.SetStr( group );
}

void agk::SaveSharedVariable( const char *varName, const char *varValue )
{
    /*
     UIPasteBoards are no longer suitable in iOS 10
    NSString *bundleID = [[NSBundle mainBundle] bundleIdentifier];
    bundleID = [bundleID lowercaseString];
    
    if ( [bundleID compare:@"com.thegamecreators.agk2player"] != NSOrderedSame )
    {
        NSRange range = [bundleID rangeOfString:@"." options:NSBackwardsSearch];
        if ( range.location != NSNotFound )
        {
            bundleID = [bundleID substringToIndex:range.location];
        }
    }
    
    bundleID = [bundleID stringByAppendingString:@".vars."];
    bundleID = [bundleID stringByAppendingString:[NSString stringWithUTF8String:varName]];
    bundleID = [bundleID lowercaseString];
    
    if ( [bundleID length] > 64 )
    {
        agk::Error("Could not save shared variable, variable name is too long");
        return;
    }
    
    UIPasteboard* board = [UIPasteboard pasteboardWithName:bundleID create:YES];
    board.persistent=YES;
    [board setValue:[NSString stringWithUTF8String:varValue] forPasteboardType:@"com.thegamecreators.agkvariable"];
    */
    
    // delete any existing UIPasteboard for this variable as we will upgrade it to the new method
    NSString *bundleID = [[NSBundle mainBundle] bundleIdentifier];
    bundleID = [bundleID lowercaseString];
    
    if ( [bundleID compare:@"com.thegamecreators.agk2player"] != NSOrderedSame )
    {
        NSRange range = [bundleID rangeOfString:@"." options:NSBackwardsSearch];
        if ( range.location != NSNotFound )
        {
            bundleID = [bundleID substringToIndex:range.location];
        }
    }
    
    bundleID = [bundleID stringByAppendingString:@".vars."];
    bundleID = [bundleID stringByAppendingString:[NSString stringWithUTF8String:varName]];
    bundleID = [bundleID lowercaseString];
    
    [UIPasteboard removePasteboardWithName:bundleID];
    
    // save the variable with the new method
    NSString *pGroup = [NSString stringWithUTF8String:m_sSharedAppGroup.GetStr()];
    NSURL *pURL = [g_pFileManager containerURLForSecurityApplicationGroupIdentifier:pGroup ];
    if ( !pURL )
    {
        agk::Error( "Invalid app group, make sure you use SetSharedVariableAppGroup on iOS" );
        return;
    }

    uString filePath;
    filePath.SetStr( [[pURL absoluteString] UTF8String] );
    filePath.ReplaceStr( "file://", "" );
    uString varPath( varName );
    varPath.Replace( '/', '_' );
    varPath.Replace( '\\', '_' );
    varPath.Replace( '*', '_' );
    varPath.Replace( '?', '_' );
    varPath.Replace( '\"', '_' );
    varPath.Replace( '|', '_' );
    varPath.Replace( '<', '_' );
    varPath.Replace( '>', '_' );
    varPath.Replace( ':', '_' );
    filePath.Append( varPath );
    
    FILE *pFile = AGKfopen( filePath.GetStr(), "wb" );
    if ( !pFile )
    {
        agk::Error( "Failed to write to variable file" );
        return;
    }
    
    int length = strlen(varValue);
    fwrite( &length, 4, 1, pFile );
    fwrite( varValue, 1, length, pFile );
    fclose( pFile );
}

char* agk::LoadSharedVariable( const char *varName, const char *defaultValue )
{
    // try new method first
    NSString *pGroup = [NSString stringWithUTF8String:m_sSharedAppGroup.GetStr()];
    NSURL *pURL = [g_pFileManager containerURLForSecurityApplicationGroupIdentifier:pGroup ];
    if ( !pURL )
    {
        agk::Error( "Invalid app group, make sure you use SetSharedVariableAppGroup on iOS" );
    }
    else
    {
        uString filePath;
        filePath.SetStr( [[pURL absoluteString] UTF8String] );
        filePath.ReplaceStr( "file://", "" );
        uString varPath( varName );
        varPath.Replace( '/', '_' );
        varPath.Replace( '\\', '_' );
        varPath.Replace( '*', '_' );
        varPath.Replace( '?', '_' );
        varPath.Replace( '\"', '_' );
        varPath.Replace( '|', '_' );
        varPath.Replace( '<', '_' );
        varPath.Replace( '>', '_' );
        varPath.Replace( ':', '_' );
        filePath.Append( varPath );
        
        FILE *pFile = AGKfopen( filePath.GetStr(), "rb" );
        if ( !pFile )
        {
            //agk::Error( "Failed to read from variable file" );
            // fall through to old method
        }
        else
        {
            int length = 0;
            fread( &length, 4, 1, pFile );
            if ( length > 0 )
            {
                char *value = new char[ length+1 ];
                fread( value, 1, length, pFile );
                value[length] = 0;
                fclose( pFile );
                return value;
            }
            else
            {
                fclose( pFile );
                // fall through to old method
            }
        }
    }
    
    // try old method in case variable was saved with a previous version
    NSString *bundleID = [[NSBundle mainBundle] bundleIdentifier];
    bundleID = [bundleID lowercaseString];
    
    if ( [bundleID compare:@"com.thegamecreators.agk2player"] != NSOrderedSame )
    {
        NSRange range = [bundleID rangeOfString:@"." options:NSBackwardsSearch];
        if ( range.location != NSNotFound )
        {
            bundleID = [bundleID substringToIndex:range.location];
        }
    }
    
    bundleID = [bundleID stringByAppendingString:@".vars."];
    bundleID = [bundleID stringByAppendingString:[NSString stringWithUTF8String:varName]];
    bundleID = [bundleID lowercaseString];
    
    UIPasteboard* board = [UIPasteboard pasteboardWithName:bundleID create:NO];
    
    if ( !board )
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
    else
    {
        NSData* result = nil;
        NSString* resultStr = nil;
        result = [board valueForPasteboardType:@"com.thegamecreators.agkvariable"];
        if ( !result )
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
        else
        {
            resultStr = [[NSString alloc] initWithData:result encoding:NSUTF8StringEncoding];
            char *str = new char[ [resultStr length]+1 ];
            strcpy( str, [resultStr UTF8String] );
            [resultStr release];
            return str;
        }
    }
}

void agk::DeleteSharedVariable( const char *varName )
{
    // must delete both old and new
    
    // delete old method
    NSString *bundleID = [[NSBundle mainBundle] bundleIdentifier];
    bundleID = [bundleID lowercaseString];
    
    if ( [bundleID compare:@"com.thegamecreators.agk2player"] != NSOrderedSame )
    {
        NSRange range = [bundleID rangeOfString:@"." options:NSBackwardsSearch];
        if ( range.location != NSNotFound )
        {
            bundleID = [bundleID substringToIndex:range.location];
        }
    }
    
    bundleID = [bundleID stringByAppendingString:@".vars."];
    bundleID = [bundleID stringByAppendingString:[NSString stringWithUTF8String:varName]];
    bundleID = [bundleID lowercaseString];
    
    //NSLog( @"Deleting: %@", bundleID );
    
    [UIPasteboard removePasteboardWithName:bundleID];
    
    // delete new method
    NSString *pGroup = [NSString stringWithUTF8String:m_sSharedAppGroup.GetStr()];
    NSURL *pURL = [g_pFileManager containerURLForSecurityApplicationGroupIdentifier:pGroup ];
    if ( !pURL ) return;
    
    uString filePath;
    filePath.SetStr( [[pURL absoluteString] UTF8String] );
    filePath.ReplaceStr( "file://", "" );
    uString varPath( varName );
    varPath.Replace( '/', '_' );
    varPath.Replace( '\\', '_' );
    varPath.Replace( '*', '_' );
    varPath.Replace( '?', '_' );
    varPath.Replace( '\"', '_' );
    varPath.Replace( '|', '_' );
    varPath.Replace( '<', '_' );
    varPath.Replace( '>', '_' );
    varPath.Replace( ':', '_' );
    filePath.Append( varPath );
    
    NSString* pPath = [ [ NSString alloc ] initWithUTF8String:filePath.GetStr() ];
    [ g_pFileManager removeItemAtPath:pPath error:nil ];
    [ pPath release ];
}

void agk::FirebaseSetup()
{
#ifndef LITEVERSION
    static int configured = 0;
    if ( configured ) return;
    configured = 1;
    
    [FIRApp configure];
#else
	agk::Message("Firebase is not supported in the template_ios_lite project, use the template_ios project instead");
#endif
}

void agk::FirebaseLogEvent( const char *event_name )
{
#ifndef LITEVERSION
    [FIRAnalytics logEventWithName:[NSString stringWithUTF8String:event_name] parameters: nil];
#endif
}

int AGKFont::PlatformGetSystemFontPath( const uString &sFontName, uString &sOut )
{
	return 0;
}


// AR commands

namespace AGK
{
    ARSession *g_pARSession = 0;
    int g_iARStatus = 0;
    cImage *g_pARTextureY = 0;
    cImage *g_pARTextureUV = 0;
    cObject3D *g_pARQuad = 0;
    AGKShader *g_pARShader = 0;
    int g_iARPlaneDetection = 1;
    int g_iARLightEstimation = 1;
    ARPlaneAnchor **g_ppARPlanes = 0;
    int g_iARPlanesSize = 0;
    ARHitTestResult **g_ppARHitTestResults = 0;
    int g_iARHitTestResultsSize = 0;
    
    class AGKiOSARAnchor : public cNode
    {
    public:
        ARAnchor *m_pARAnchor;
        
        AGKiOSARAnchor() { m_pARAnchor = 0; }
        ~AGKiOSARAnchor() { if ( m_pARAnchor ) [m_pARAnchor release]; }
        
        void Update()
        {
            if ( !m_pARAnchor ) return;
            
            float x = m_pARAnchor.transform.columns[3][0];
            float y = m_pARAnchor.transform.columns[3][1];
            float z = -m_pARAnchor.transform.columns[3][2];
            
            float ax = agk::ASin(m_pARAnchor.transform.columns[2][1]);
            float ay = agk::ACos(-m_pARAnchor.transform.columns[2][2] / agk::Cos(ax));
            float az = agk::ASin(m_pARAnchor.transform.columns[0][1] / agk::Cos(ax));
            AGKQuaternion quat; quat.MakeFromEulerYXZ( ax, ay, az );
            
            SetNodePosition( x, y, z );
            SetNodeRotation( quat.w, quat.x, quat.y, quat.z );
        }
    };
    
    cHashedList<AGKiOSARAnchor> g_pARAnchorList(256);
}

void agk::ARSetup()
//****
{
    g_iARStatus = -1;
    if (@available(iOS 11.0, *))
    {
        if ( !ARWorldTrackingConfiguration.isSupported )
        {
            agk::Warning( "AR is not supported on this device" );
            return;
        }
        
        if ( !g_pARSession )
        {
            ARWorldTrackingConfiguration *pConfig = [[ARWorldTrackingConfiguration alloc] init];
            pConfig.lightEstimationEnabled = g_iARLightEstimation ? TRUE : FALSE;
            pConfig.worldAlignment = ARWorldAlignmentGravity;
            if ( g_iARPlaneDetection )
            {
                [pConfig setPlaneDetection:ARPlaneDetectionHorizontal /*| ARPlaneDetectionVertical*/];
            }
            else
            {
                [pConfig setPlaneDetection:ARPlaneDetectionNone];
            }
            g_pARSession = [[ARSession alloc] init];
            [g_pARSession runWithConfiguration:pConfig options:0];
            [pConfig release];
        }
        
        CreateARImageData();
        
        if ( !g_pARTextureY )
        {
            g_pARTextureY = new cImage();
            g_pARTextureY->CreateBlankImage( 1,1, 0, 0 );
        }
        
        if ( !g_pARTextureUV )
        {
            g_pARTextureUV = new cImage();
            g_pARTextureUV->CreateBlankImage( 1,1, 0, 0 );
        }
        
        if ( !g_pARShader )
        {
            g_pARShader = new AGKShader();
            g_pARShader->SetARTextureShaderiOS();
        }
        
        if ( !g_pARQuad )
        {
            g_pARQuad = new cObject3D();
            g_pARQuad->CreateQuad();
            g_pARQuad->SetImage( g_pARTextureY, 0 );
            g_pARQuad->SetImage( g_pARTextureUV, 1 );
            g_pARQuad->SetShader( g_pARShader );
        }
    }
    else
    {
        agk::Warning( "AR requires iOS 11 or greater" );
        return;
    }
    
    g_iARStatus = 2;
}

int agk::ARGetStatus()
//****
{
    return g_iARStatus;
}

// not a Tier 1 command
void agk::ARUpdateInternal()
{
    if ( !g_pARSession ) return;
    
    ARFrame* pARFrame = g_pARSession.currentFrame;
    CopyARImageToAGKImage( pARFrame.capturedImage, g_pARTextureY, g_pARTextureUV );
    
    AGKiOSARAnchor *pAnchor = g_pARAnchorList.GetFirst();
    while ( pAnchor )
    {
        pAnchor->Update();
        pAnchor = g_pARAnchorList.GetNext();
    }
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
    if ( g_pARQuad ) delete g_pARQuad;
    g_pARQuad = 0;
    
    if ( g_pARShader ) delete g_pARShader;
    g_pARShader = 0;
    
    ReleaseARImageData();
    
    if ( g_pARTextureY )
    {
        g_pARTextureY->OverrideTexture(0, 0, 0);
        delete g_pARTextureY;
        g_pARTextureY = 0;
    }
    
    if ( g_pARTextureUV )
    {
        g_pARTextureUV->OverrideTexture(0, 0, 0);
        delete g_pARTextureUV;
        g_pARTextureUV = 0;
    }
    
    ARHitTestFinish();
    ARGetPlanesFinish();
    
    if ( g_pARSession ) [g_pARSession release];
    g_pARSession = 0;
    g_iARStatus = 0;
}

void agk::ARControlCamera()
//****
{
	if ( !g_pARSession ) return;
    ARFrame *pARFrame = g_pARSession.currentFrame;
    
    cCamera *pAGKCamera = agk::GetCurrentCamera();
    float fNear = pAGKCamera->GetNearRange();
    float fFar = pAGKCamera->GetFarRange();
    
    ARCamera *pARCamera = pARFrame.camera;
    if ( !pARCamera ) return;
    if ( [pARCamera trackingState] == ARTrackingStateNotAvailable ) return;
    
    float x = pARCamera.transform.columns[3][0];
    float y = pARCamera.transform.columns[3][1];
    float z = -pARCamera.transform.columns[3][2];
    pAGKCamera->SetPosition( x, y, z );
    float radtodeg = 180.0f / 3.14159265f;
    pAGKCamera->SetRotationEuler(-pARCamera.eulerAngles[0]*radtodeg, -pARCamera.eulerAngles[1]*radtodeg, pARCamera.eulerAngles[2]*radtodeg);
    
    UIInterfaceOrientation orien = UIInterfaceOrientationUnknown;
    switch( GetOrientation() )
    {
        case 1:
        {
            pAGKCamera->RotateLocalZ(90);
            orien = UIInterfaceOrientationPortrait;
            break;
        }
        case 2:
        {
            pAGKCamera->RotateLocalZ(270);
            orien = UIInterfaceOrientationPortraitUpsideDown;
            break;
        }
        case 3: orien = UIInterfaceOrientationLandscapeLeft; break;
        case 4:
        {
            pAGKCamera->RotateLocalZ(180);
            orien = UIInterfaceOrientationLandscapeRight;
            break;
        }
    }
    
    matrix_float4x4 proj_mat = [pARCamera projectionMatrixForOrientation:orien viewportSize:CGSizeMake(GetDeviceWidth(), GetDeviceHeight()) zNear:fNear zFar:fFar];
    
    // off center proj matrix
    float diffX = (proj_mat.columns[2][0]-1) / (proj_mat.columns[2][0]+1); // l*diffX = r
    float tempX = (2*fNear) / proj_mat.columns[0][0]; // r-l = tempX
    float left = tempX / (diffX-1);
    float right = left*diffX;
    
    float diffY = (proj_mat.columns[2][1]-1) / (proj_mat.columns[2][1]+1); // b*diffY = t
    float tempY = (2*fNear) / proj_mat.columns[1][1]; // t-b = tempY
    float bottom = tempY / (diffY-1);
    float top = bottom*diffY;
    pAGKCamera->SetBounds( left, right, top, bottom );
    pAGKCamera->SetOffCenter(1);
}

void agk::ARDrawBackground()
//****
{
    if ( !g_pARSession ) return;
    if ( !g_pARTextureY ) return;
    if ( !g_pARTextureUV ) return;
    if ( !g_pARQuad ) return;
    
    int orien = GetOrientation();
    switch( orien )
    {
        case 1: g_pARShader->SetConstantByName( "orientation", 0, 1, -1, 0 ); break;
        case 2: g_pARShader->SetConstantByName( "orientation", 0, -1, 1, 0 ); break;
        case 3: g_pARShader->SetConstantByName( "orientation", 1, 0, 0, 1 ); break;
        case 4: g_pARShader->SetConstantByName( "orientation", -1, 0, 0, -1 ); break;
    }
    
    g_pARQuad->Draw();
}

void agk::ARSetPlaneDetectionMode( int mode )
//****
{
    g_iARPlaneDetection = mode==1 ? 1 : 0;
    if ( !g_pARSession ) return;
    
    ARWorldTrackingConfiguration *pConfig = [[ARWorldTrackingConfiguration alloc] init];
    pConfig.lightEstimationEnabled = g_iARLightEstimation ? TRUE : FALSE;
    pConfig.worldAlignment = ARWorldAlignmentGravity;
    if ( g_iARPlaneDetection )
    {
        [pConfig setPlaneDetection:ARPlaneDetectionHorizontal /*| ARPlaneDetectionVertical*/];
    }
    else
    {
        [pConfig setPlaneDetection:ARPlaneDetectionNone];
    }
    [g_pARSession runWithConfiguration:pConfig options:0];
    [pConfig release];
}

void agk::ARSetLightEstimationMode( int mode )
//****
{
    g_iARLightEstimation = mode==1 ? 1 : 0;
    if ( !g_pARSession ) return;
    
    ARWorldTrackingConfiguration *pConfig = [[ARWorldTrackingConfiguration alloc] init];
    pConfig.lightEstimationEnabled = g_iARLightEstimation ? TRUE : FALSE;
    pConfig.worldAlignment = ARWorldAlignmentGravity;
    if ( g_iARPlaneDetection )
    {
        [pConfig setPlaneDetection:ARPlaneDetectionHorizontal /*| ARPlaneDetectionVertical*/];
    }
    else
    {
        [pConfig setPlaneDetection:ARPlaneDetectionNone];
    }
    [g_pARSession runWithConfiguration:pConfig options:0];
    [pConfig release];
}

float agk::ARGetLightEstimate()
//****
{
    if ( !g_pARSession ) return 0.5f;
    
    ARFrame *pARFrame = g_pARSession.currentFrame;
    ARLightEstimate *pARLight = pARFrame.lightEstimate;
    return pARLight.ambientIntensity / 2000.0f;
}

int agk::ARHitTest( float screenX, float screenY )
//****
{
    if ( !g_pARSession ) return 0;
    ARHitTestFinish();
    
    float fX = ScreenToDeviceX( screenX );
    float fY = ScreenToDeviceY( screenY );
    fX /= m_iRealDeviceWidth;
    fY /= m_iRealDeviceHeight;
    
    float fX2, fY2;
    
    switch( GetOrientation() )
    {
        case 1: fX2 = fY; fY2 = 1-fX; break;
        case 2: fX2 = 1-fY; fY2 = fX; break;
        case 3: fX2 = fX; fY2 = fY; break;
        case 4: fX2 = 1.0-fX; fY2 = 1-fY; break;
    }
    
    ARFrame *pARFrame = g_pARSession.currentFrame;
    NSArray<ARHitTestResult*>* results = [pARFrame hitTest:CGPointMake(fX2,fY2) types:/*ARHitTestResultTypeFeaturePoint |*/ ARHitTestResultTypeExistingPlaneUsingExtent];
    
    g_ppARHitTestResults = new ARHitTestResult*[ [results count] ];
    int count = 0;
    for( int i = 0; i < [results count]; i++ )
    {
        g_ppARHitTestResults[count] = [results objectAtIndex:i];
        [g_ppARHitTestResults[count] retain];
        count++;
    }
    
    g_iARHitTestResultsSize = count;
    return g_iARHitTestResultsSize;
}

float agk::ARGetHitTestX( int index )
//****
{
    if ( !g_ppARHitTestResults ) return 0;
    if ( index < 1 || index > g_iARHitTestResultsSize ) return 0;
    index--;
    
    return g_ppARHitTestResults[index].worldTransform.columns[3][0];
}

float agk::ARGetHitTestY( int index )
//****
{
    if ( !g_ppARHitTestResults ) return 0;
    if ( index < 1 || index > g_iARHitTestResultsSize ) return 0;
    index--;
    
    return g_ppARHitTestResults[index].worldTransform.columns[3][1];
}

float agk::ARGetHitTestZ( int index )
//****
{
    if ( !g_ppARHitTestResults ) return 0;
    if ( index < 1 || index > g_iARHitTestResultsSize ) return 0;
    index--;
    
    return -g_ppARHitTestResults[index].worldTransform.columns[3][2];
}

float agk::ARGetHitTestNormalX( int index )
//****
{
    if ( !g_ppARHitTestResults ) return 0;
    if ( index < 1 || index > g_iARHitTestResultsSize ) return 0;
    index--;
    
    return g_ppARHitTestResults[index].worldTransform.columns[1][0];
}

float agk::ARGetHitTestNormalY( int index )
//****
{
    if ( !g_ppARHitTestResults ) return 0;
    if ( index < 1 || index > g_iARHitTestResultsSize ) return 0;
    index--;
    
    return g_ppARHitTestResults[index].worldTransform.columns[1][1];
}

float agk::ARGetHitTestNormalZ( int index )
//****
{
    if ( !g_ppARHitTestResults ) return 0;
    if ( index < 1 || index > g_iARHitTestResultsSize ) return 0;
    index--;
    
    return g_ppARHitTestResults[index].worldTransform.columns[1][2];
}

int agk::ARGetHitTestType( int index )
//****
{
    if ( !g_ppARHitTestResults ) return 0;
    if ( index < 1 || index > g_iARHitTestResultsSize ) return 0;
    index--;
    
    return g_ppARHitTestResults[index].type == ARHitTestResultTypeFeaturePoint ? 0 : 1;
}

void agk::ARHitTestFinish()
//****
{
	if ( g_ppARHitTestResults )
    {
        for( int i = 0; i < g_iARHitTestResultsSize; i++ )
        {
            if ( g_ppARHitTestResults[i] ) [g_ppARHitTestResults[i] release];
        }
        delete [] g_ppARHitTestResults;
        g_ppARHitTestResults = 0;
    }
    g_iARHitTestResultsSize = 0;
}

int agk::ARGetPlanes( int reserved )
//****
{
    if ( !g_pARSession ) return 0;
    ARGetPlanesFinish();
    
    ARFrame *pARFrame = g_pARSession.currentFrame;
    NSArray<ARAnchor*> *ppAnchors = pARFrame.anchors;
    g_ppARPlanes = new ARPlaneAnchor*[ [ppAnchors count] ];
    int count = 0;
    for( int i = 0; i < [ppAnchors count]; i++ )
    {
        if( ![ppAnchors[i] isKindOfClass:[ARPlaneAnchor class]] ) continue;
        
        g_ppARPlanes[count] = (ARPlaneAnchor*)([ppAnchors objectAtIndex:i]);
        [g_ppARPlanes[count] retain];
        count++;
    }
    
    g_iARPlanesSize = count;
    return g_iARPlanesSize;
}

float agk::ARGetPlaneX( int index )
//****
{
    if ( !g_ppARPlanes ) return 0;
    if ( index < 1 || index > g_iARPlanesSize ) return 0;
    index--;
    
    return g_ppARPlanes[index].transform.columns[3][0] + g_ppARPlanes[index].center[0];
}

float agk::ARGetPlaneY( int index )
//****
{
    if ( !g_ppARPlanes ) return 0;
    if ( index < 1 || index > g_iARPlanesSize ) return 0;
    index--;
    
    return g_ppARPlanes[index].transform.columns[3][1] + g_ppARPlanes[index].center[1];
}

float agk::ARGetPlaneZ( int index )
//****
{
    if ( !g_ppARPlanes ) return 0;
    if ( index < 1 || index > g_iARPlanesSize ) return 0;
    index--;
    
    return -g_ppARPlanes[index].transform.columns[3][2] - g_ppARPlanes[index].center[2];
}

float agk::ARGetPlaneAngleX( int index )
//****
{
    if ( !g_ppARPlanes ) return 0;
    if ( index < 1 || index > g_iARPlanesSize ) return 0;
    index--;
    
    return ASin(g_ppARPlanes[index].transform.columns[2][1]);
}

float agk::ARGetPlaneAngleY( int index )
//****
{
    if ( !g_ppARPlanes ) return 0;
    if ( index < 1 || index > g_iARPlanesSize ) return 0;
    index--;
    
    float x = ASin(g_ppARPlanes[index].transform.columns[2][1]);
    return ACos(-g_ppARPlanes[index].transform.columns[2][2] / Cos(x));
}

float agk::ARGetPlaneAngleZ( int index )
//****
{
    if ( !g_ppARPlanes ) return 0;
    if ( index < 1 || index > g_iARPlanesSize ) return 0;
    index--;
    
    float x = ASin(g_ppARPlanes[index].transform.columns[2][1]);
    return ASin(g_ppARPlanes[index].transform.columns[0][1] / Cos(x));
}

float agk::ARGetPlaneSizeX( int index )
//****
{
    if ( !g_ppARPlanes ) return 0;
    if ( index < 1 || index > g_iARPlanesSize ) return 0;
    index--;
    
    return g_ppARPlanes[index].extent[0];
}

float agk::ARGetPlaneSizeZ( int index )
//****
{
    if ( !g_ppARPlanes ) return 0;
    if ( index < 1 || index > g_iARPlanesSize ) return 0;
    index--;
    
    return g_ppARPlanes[index].extent[2];
}

void agk::ARGetPlanesFinish()
//****
{
	if ( g_ppARPlanes )
    {
        for( int i = 0; i < g_iARPlanesSize; i++ )
        {
            if ( g_ppARPlanes[i] ) [g_ppARPlanes[i] release];
        }
        delete [] g_ppARPlanes;
        g_ppARPlanes = 0;
    }
    g_iARPlanesSize = 0;
}

int agk::ARCreateAnchorFromHitTest( int index )
//****
{
    if ( !g_ppARHitTestResults ) return 0;
    if ( index < 1 || index > g_iARHitTestResultsSize ) return 0;
    index--;
    
    UINT anchorID = g_pARAnchorList.GetFreeID();
    if ( !anchorID )
    {
        agk::Warning( "Failed to create hit test anchor, no free IDs found" );
        return 0;
    }
    
    AGKiOSARAnchor *pNewAnchor = new AGKiOSARAnchor();
    pNewAnchor->m_pARAnchor = [[ARAnchor alloc] initWithTransform:g_ppARHitTestResults[index].worldTransform];
    pNewAnchor->Update();
    
    g_pARAnchorList.AddItem( pNewAnchor, anchorID );
    return anchorID;
}

int agk::ARCreateAnchorFromPlane( int index )
//****
{
    if ( !g_ppARPlanes ) return 0;
    if ( index < 1 || index > g_iARPlanesSize ) return 0;
    index--;
    
    UINT anchorID = g_pARAnchorList.GetFreeID();
    if ( !anchorID )
    {
        agk::Warning( "Failed to create plane anchor, no free IDs found" );
        return 0;
    }
    
    [g_ppARPlanes[index] retain];
    AGKiOSARAnchor *pNewAnchor = new AGKiOSARAnchor();
    pNewAnchor->m_pARAnchor = g_ppARPlanes[index];
    pNewAnchor->Update();
    
    g_pARAnchorList.AddItem( pNewAnchor, anchorID );
    return anchorID;
}

void agk::ARFixObjectToAnchor( int objID, int anchorID )
//****
{
    cObject3D *pObject = m_cObject3DList.GetItem( objID );
    if ( !pObject )
    {
        uString errStr; errStr.Format( "Failed to fix object %d - object does not exist", objID );
        Error( errStr );
        return;
    }
    
    if ( anchorID == 0 )
    {
        pObject->RemoveFromParent();
    }
    else
    {
        AGKiOSARAnchor *pToAnchor = g_pARAnchorList.GetItem( anchorID );
        if ( !pToAnchor )
        {
            uString errStr; errStr.Format( "Failed to fix object to anchor %d - anchor does not exist", anchorID );
            Error( errStr );
            return;
        }
        
        pToAnchor->AddChild( pObject );
    }
}

int agk::ARGetAnchorStatus( int anchorID )
//****
{
	return 2; // always tracking
}

void agk::ARDeleteAnchor( int anchorID )
//****
{
    AGKiOSARAnchor *pAnchor = g_pARAnchorList.RemoveItem( anchorID );
    if ( pAnchor ) delete pAnchor;
}

int agk::GetAppInstalled( const char *packageName )
//****
{
	// not possible since iOS 9?
	return 0;
}

// SnapChat

#ifndef LITEVERSION
#if !defined(__i386__) && !defined(__x86_64__)
namespace AGK
{
    SCSDKSnapAPI *g_pSnapChatAPI = 0;
    float g_fSnapChatStickerX = 0.5f;
    float g_fSnapChatStickerY = 0.5f;
    float g_fSnapChatStickerAngle = 0;
    UIImage *g_pSnapChatImage = 0;
    UIImage *g_pSnapChatStickerImage = 0;
    SCSDKSnapPhoto *g_pSnapChatPhoto = 0;
    SCSDKPhotoSnapContent *g_pSnapChatContent = 0;
    SCSDKSnapSticker *g_pSnapChatSticker = 0;
}
#endif
#endif

void agk::SetSnapChatStickerSettings( float x, float y, int width, int height, float angle )
//****
{
#ifndef LITEVERSION
  #if !defined(__i386__) && !defined(__x86_64__)
    g_fSnapChatStickerX = x;
    g_fSnapChatStickerY = y;
    g_fSnapChatStickerAngle = angle;
  #endif
#endif
}

void agk::ShareSnapChatImage( const char* imageFile, const char* stickerFile, const char* caption, const char* url )
//****
{
#ifndef LITEVERSION
  #if !defined(__i386__) && !defined(__x86_64__)
    uString sPath( imageFile );
    if ( !GetRealPath( sPath ) )
    {
        uString err; err.Format( "Could not find image at path: %s", imageFile );
        agk::Error( err );
        return;
    }
    
    if ( !g_pSnapChatAPI ) g_pSnapChatAPI = [[SCSDKSnapAPI alloc] init];
    
    if ( g_pSnapChatImage ) return;
    
    NSString *imagePath = [NSString stringWithUTF8String:sPath.GetStr()];
    g_pSnapChatImage = [UIImage imageWithContentsOfFile:imagePath];
    [g_pSnapChatImage retain];
    g_pSnapChatPhoto = [[SCSDKSnapPhoto alloc] initWithImage:g_pSnapChatImage];
    g_pSnapChatContent = [[SCSDKPhotoSnapContent alloc] initWithSnapPhoto:g_pSnapChatPhoto];
    
    if ( stickerFile && *stickerFile )
    {
        sPath.SetStr( stickerFile );
        if ( !GetRealPath( sPath ) )
        {
            uString err; err.Format( "Could not find sticker image at path: %s", stickerFile );
            agk::Error( err );
            return;
        }
        
        NSString *stickerPath = [NSString stringWithUTF8String:sPath.GetStr()];
        g_pSnapChatStickerImage = [UIImage imageWithContentsOfFile:stickerPath];
        [g_pSnapChatStickerImage retain];
        g_pSnapChatSticker = [[SCSDKSnapSticker alloc] initWithStickerImage:g_pSnapChatStickerImage];
        [g_pSnapChatSticker setPosX:g_fSnapChatStickerX];
        [g_pSnapChatSticker setPosY:g_fSnapChatStickerY];
        [g_pSnapChatSticker setRotation:g_fSnapChatStickerAngle];
        [g_pSnapChatContent setSticker:g_pSnapChatSticker];
    }
    
    if ( caption && *caption ) g_pSnapChatContent.caption = [NSString stringWithUTF8String:caption];
    else g_pSnapChatContent.caption = nil;
    if ( url && *url ) g_pSnapChatContent.attachmentUrl = [NSString stringWithUTF8String:url];
    else g_pSnapChatContent.attachmentUrl = nil;
    
    [g_pSnapChatAPI startSendingContent:g_pSnapChatContent completionHandler:^(NSError *error) {
        if ( error ) NSLog( @"%@", [error debugDescription] );
        else NSLog( @"Success" );
        
        if ( g_pSnapChatImage ) [g_pSnapChatImage release];
        if ( g_pSnapChatPhoto ) [g_pSnapChatPhoto release];
        if ( g_pSnapChatContent ) [g_pSnapChatContent release];
        if ( g_pSnapChatStickerImage ) [g_pSnapChatStickerImage release];
        if ( g_pSnapChatSticker ) [g_pSnapChatSticker release];
        
        g_pSnapChatImage = 0;
        g_pSnapChatPhoto = 0;
        g_pSnapChatContent = 0;
        g_pSnapChatStickerImage = 0;
        g_pSnapChatSticker = 0;
    }];
  #endif
#endif
}
