#include "agk.h"
#include "OpenGLES2.h"

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#include <AL/al.h>
#include <AL/alc.h>

// for music
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include "png.h"

#include "jpeglib.h"
#include "jpegint.h"

#include "pwd.h"

#include "emscripten.h"
#include "html5.h"

extern unsigned char libImageAscii[];
extern unsigned char libImageAsciiExt[];

int agk::m_iKeyboardMode = 1; // physical keyboard

extern "C" {
FILE* AGKfopen( const char *szPath, const char* mode )
{
	FILE* pFile = fopen( szPath, mode );
	return pFile;
}
}

namespace AGK
{
 	INT64 uFixTime = 0;
	float fFixTime2 = 0;

	GLFWwindow *g_pWindow = 0;
	
	int g_iUpdateSurface = 0;
	int g_iNewSurfaceWidth = 0;
	int g_iNewSurfaceHeight = 0;

	struct egldata
	{
		GLFWwindow *window;
	};

	// sounds
	ALCcontext *audioContext = NULL;
	ALCdevice *audioDevice = NULL;
	
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
			ALuint sourceID;
		
			cSoundInst() 
			{ 
				m_iID = 0;
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
			
			~cSoundInst()
			{

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
	
	// music
	Mix_Music * g_pMusic = 0;	
	float g_fMusicStartTime = 0;
	float g_fMusicPauseTime = 0;
		
	// video
	// todo

	char szAppFolderName[ MAX_PATH ] = "";
	char szBaseDir[ MAX_PATH ] = "";
	char szRootDir[ MAX_PATH ] = "";
	char szWriteDir[ MAX_PATH ] = "";
	char szOriginalWriteDir[ MAX_PATH ] = "";
	
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
	bool g_bPasswordMode = false;
	
	cSprite *pTextBackground = 0;
	bool g_bEditBoxHack = false;

	// advert variables
	bool g_bAdvertActive = false;
	int g_iAdvertHorz = 0;
	int g_iAdvertVert = 0;
	float g_fAdvertOffsetX = 0;
	float g_fAdvertOffsetY = 0;

	// facebook variables
	uString m_sFBAppID;
	uString m_sAccessToken;
	cHTTPConnection *m_pFacebookConnection = 0;
	int m_iFacebookGettingFriends = 0;
	int m_iFBGettingPicture = 0;
	uString m_sFBLocalFile;
	uString m_sFBUserID;
	uString m_sFBName;
	int m_iFBUserIDStarted = 0;

	struct FacebookUser
	{
		uString userID;
		uString name;
	};

	FacebookUser *m_pFBFriends = 0;
	int m_iFBFriendCount = 0;
}

using namespace AGK;

void cFileEntry::TraverseDirectory( const char* dir )
{
	DIR *pDir = opendir( dir );
	if ( pDir )
	{
		dirent* item = readdir( pDir );
		while( item )
		{
			//uString sDir;
			//sDir.Format( "%s - %d", item->d_name, item->d_type );
			//agk::Warning( sDir );

			if ( item->d_type == DT_DIR )
			{
				// directory
				if ( strcmp( item->d_name, "." ) != 0 && strcmp( item->d_name, ".." ) != 0 )
				{
					char str[ 1024 ];
					strcpy( str, dir );
					strcat( str, item->d_name );
					AddNewFile( str );

					strcat( str, "/" );
					TraverseDirectory( str );
				}
			}
			else if ( item->d_type == DT_REG )
			{
				// file
				char str[ 1024 ];
				strcpy( str, dir );
				strcat( str, item->d_name );
				AddNewFile( str );
			}

			item = readdir( pDir );
		}

		closedir( pDir );
	}
}

void cFileEntry::InitFileList()
{
	//TraverseDirectory( szRootDir );
	//TraverseDirectory( szWriteDir );
}

void agk::SetWindowPosition( int x, int y )
{
	glfwSetWindowPos( g_pWindow, x, y );
}

void agk::SetWindowSize( int width, int height, int fullscreen )
{
	SetWindowSize( width, height, fullscreen, 0 );
}

void agk::SetWindowSize( int width, int height, int fullscreen, int allowOverSized )
{
	if ( width < 0 ) width = 0;
	if ( height < 0 ) height = 0;
	
	
	if ( fullscreen )
	{
		width = EM_ASM_INT_V({return screen.width});
		height = EM_ASM_INT_V({return screen.height});
	}
	else
	{
		float maxWidth = agk::GetMaxDeviceWidth();
		float maxHeight = agk::GetMaxDeviceHeight();
		
		if ( allowOverSized == 0 )
		{
			float aspect = width / (float)height;
			float winAspect = maxWidth / maxHeight;
			if ( aspect > winAspect )
			{
				if ( width > maxWidth ) 
				{
					float ratio = maxWidth / width;
					width = maxWidth;
					height = height*ratio;
				}
			}
			else
			{
				if ( height > maxHeight ) 
				{
					float ratio = maxHeight / height;
					height = maxHeight;
					width = width*ratio;
				}
			}
		}
	}

	glfwSetWindowSize( g_pWindow, width, height );
}

void agk::SetWindowAllowResize( int mode )
{
	// do nothing on linux
}

void agk::MaximizeWindow()
{
	
}

void agk::MinimizeApp()
{
	glfwIconifyWindow( g_pWindow );
}

void agk::RestoreApp()
{
	glfwRestoreWindow( g_pWindow );
}

void agk::SetImmersiveMode( int mode )
{
	
}

void agk::SetScreenResolution( int width, int height )
//****
{
	// do nothing on linux
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
	outString.SetStr( "html5" );
}

char* agk::GetDeviceName( )
//****
{
	char *str = new char[ 15 ];
	strcpy( str, "html5" );
	return str;
}

char* agk::GetDeviceBaseName( )
//****
{
	char *str = new char[ 15 ];
	strcpy( str, "html5" );
	return str;
}

char* agk::GetDeviceType( )
//****
{
	// todo
	char *str = new char[ 128 ];
	*str = 0;
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
	char *str = new char[ m_sAppName.GetLength()+1 ];
	strcpy( str, m_sAppName.GetStr() );
	return str;
}

bool agk::GetDeviceHasSplash()
{
	return false;
}

char* agk::GetDeviceLanguage( )
//****
{
	// todo
	char *str = new char[ 10 ];
	str[0] = 0;

	return str;
}

void agk::SetSleepMode( int mode )
//****
{
	// do nothing on linux
}

void agk::SetExpansionFileKey( const char *key )
//****
{
	// do nothing
}

void agk::SetExpansionFileVersion(int version)
{
	// do nothing on linux
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
	// do nothing on linux
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
	// do nothing on linux
}

bool agk::PlatformGetDeviceID( uString &out )
{
	// todo
	return false;
}

float agk::PlatformDeviceScale()
{
	return 1.0f;
}

int agk::PlatformGetNumProcessors()
{
	return 1;
}

struct AGKData
{
	int width;
	int height;
};

void agk::PlatformInitFilePaths()
{
	for ( int i = 0; i < MAX_PATH; i++ ) szBaseDir[ i ] = 0;

	// todo?

	strcpy( szRootDir, "" );
	strcpy( szWriteDir, "/" );
	
	// make sure this value is set here incase restore is called without changing path
	strcpy( szOriginalWriteDir, szWriteDir );

	m_bUpdateFileLists = true;
}

void agk::PlatformUpdateWritePath()
{
	strcpy( szRootDir, "" );
	strcat( szWriteDir, "/" );

	m_bUpdateFileLists = true;
}

void agk::PlatformRestoreWritePath()
{
	if ( strlen ( szOriginalWriteDir ) > 0 ) strcpy( szWriteDir, szOriginalWriteDir );
	
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
	egldata *ePtr = (egldata*)ptr;

	g_pWindow = ePtr->window;
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
	egldata *ePtr = (egldata*)ptr;

	g_pWindow = ePtr->window;

	glfwGetFramebufferSize( g_pWindow, &m_iRenderWidth, &m_iRenderHeight );
	glfwGetWindowSize( g_pWindow, &m_iRealDeviceWidth, &m_iRealDeviceHeight );
	cCamera::UpdateAllAspectRatio( m_iRenderWidth/(float)m_iRenderHeight );

	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	uFixTime = now.tv_sec;
	fFixTime2 = now.tv_nsec * 1e-9;
	SetRandomSeed( uFixTime + (now.tv_nsec % 1000) );
	
	CheckRendererExtensions();
	
	if ( !m_pMouse[ 0 ] ) m_pMouse[ 0 ] = new cMouse();

	agk::PlatformDisableScissor();
	agk::PlatformDisableStencil();
	agk::PlatformSetAlignment( 1 ); // not using memory alignment

	AGKShader::CreateDefaultShaders();

	// sensors
	agk::m_iAccelerometerExists = 0;
	agk::m_iGyroSensorExists = 0;
	agk::m_iMagneticSensorExists = 0;
	agk::m_iLightSensorExists = 0;
	agk::m_iProximitySensorExists = 0;
	agk::m_iRotationSensorExists = 0;

	m_iGPSSensorExists = 0;

	// text input setup
	float DevToVirX = 1.0f;
	float DevToVirY = 1.0f;
	if ( agk::m_fTargetViewportWidth > 0 ) DevToVirX = agk::GetVirtualWidth() / agk::m_fTargetViewportWidth;
	if ( agk::m_fTargetViewportHeight > 0 )  DevToVirY = agk::GetVirtualHeight() / agk::m_fTargetViewportHeight;
	
	float width = 250 * DevToVirX;
	float height = 22 * DevToVirY;
	
	if ( width > agk::GetVirtualWidth() ) width = (float) agk::GetVirtualWidth();
	
	g_fTextStartX = (agk::GetVirtualWidth() - width)/2.0f + 3*DevToVirX;
	g_fTextStartY = agk::GetVirtualHeight()/3.0f + 2*DevToVirY;
	
	g_pTextInputCursor = new cSprite();
	g_pTextInputCursor->SetSize( 2 * DevToVirX, 18 * DevToVirY );
	g_pTextInputCursor->SetColor( 102, 213, 255, 255 );
	g_pTextInputCursor->SetPosition( g_fTextStartX, g_fTextStartY );
	g_pTextInputCursor->SetOffset( 0,0 );
	g_pTextInputCursor->FixToScreen(1);
	
	g_pTextInputArea = new cSprite();
	g_pTextInputArea->SetSize( width, height );
	g_pTextInputArea->SetColor( 255,255,255,255 );
	g_pTextInputArea->SetPosition( (agk::GetVirtualWidth() - width) / 2.0f, agk::GetVirtualHeight()/3.0f );
	g_pTextInputArea->SetOffset( 0,0 );
	g_pTextInputArea->FixToScreen(1);
	
	width += 8 * DevToVirX;
	height += 8 * DevToVirY;
	
	g_pTextInputBack = new cSprite();
	g_pTextInputBack->SetSize( width, height );
	g_pTextInputBack->SetColor( 190,190,190,255 );
	g_pTextInputBack->SetPosition( (agk::GetVirtualWidth() - width) / 2.0f, agk::GetVirtualHeight()/3.0f - 4*DevToVirY );
	g_pTextInputBack->SetOffset( 0,0 );
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
	pTextBackground->SetPosition( -m_iDisplayExtraX, -m_iDisplayExtraY );
	pTextBackground->SetSize( m_iDisplayWidth+m_iDisplayExtraX*2, m_iDisplayHeight+m_iDisplayExtraY*2 );
	pTextBackground->FixToScreen(1);

	// joysticks 
	int numJoysticks = emscripten_get_num_gamepads();
	if ( numJoysticks == EMSCRIPTEN_RESULT_NOT_SUPPORTED ) return;
	if ( numJoysticks > AGK_NUM_JOYSTICKS ) numJoysticks = AGK_NUM_JOYSTICKS;
	
	for( int i = 0; i < AGK_NUM_JOYSTICKS; i++ )
	{
		if ( agk::m_pJoystick[i] ) agk::m_pJoystick[i]->m_iConnected = 0;
	}
	
	for( int i = 0; i < numJoysticks; i++ )
	{
		EmscriptenGamepadEvent pStatus ;
		emscripten_get_gamepad_status( i, &pStatus );
		int index = pStatus.index;
		if ( index >= AGK_NUM_JOYSTICKS ) continue;
		if ( !pStatus.connected ) continue;
		
		if ( agk::m_pJoystick[index] ) agk::m_pJoystick[index]->m_iConnected = 1;
		else
		{
			// new joystick
			agk::m_pJoystick[index] = new cJoystick( 0 );
			agk::m_pJoystick[index]->m_iDeviceType = index | 0x30000;
		}
	}
}

void agk::PlatformInitConsole()
{
	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	uFixTime = now.tv_sec;
	fFixTime2 = now.tv_nsec * 1e-9;
	SetRandomSeed( uFixTime + (now.tv_nsec % 1000) );
	
	// joysticks 
	int numJoysticks = emscripten_get_num_gamepads();
	if ( numJoysticks == EMSCRIPTEN_RESULT_NOT_SUPPORTED ) return;
	if ( numJoysticks > AGK_NUM_JOYSTICKS ) numJoysticks = AGK_NUM_JOYSTICKS;
	
	for( int i = 0; i < AGK_NUM_JOYSTICKS; i++ )
	{
		if ( agk::m_pJoystick[i] ) agk::m_pJoystick[i]->m_iConnected = 0;
	}
	
	for( int i = 0; i < numJoysticks; i++ )
	{
		EmscriptenGamepadEvent pStatus ;
		emscripten_get_gamepad_status( i, &pStatus );
		int index = pStatus.index;
		if ( index >= AGK_NUM_JOYSTICKS ) continue;
		if ( !pStatus.connected ) continue;
		
		if ( agk::m_pJoystick[index] ) agk::m_pJoystick[index]->m_iConnected = 1;
		else
		{
			// new joystick
			agk::m_pJoystick[index] = new cJoystick( 0 );
			agk::m_pJoystick[index]->m_iDeviceType = index | 0x30000;
		}
	}
}

void agk::UpdatePtr( void *ptr )
{
	egldata *ePtr = (egldata*)ptr;

	g_pWindow = ePtr->window;

	glfwGetFramebufferSize( g_pWindow, &m_iRenderWidth, &m_iRenderHeight );
	glfwGetWindowSize( g_pWindow, &m_iRealDeviceWidth, &m_iRealDeviceHeight );
	cCamera::UpdateAllAspectRatio( m_iRenderWidth/(float)m_iRenderHeight );

	RecalculateDisplay();

	agk::SetVideoDimensions( m_fVideoX, m_fVideoY, m_fVideoWidth, m_fVideoHeight );
}

void agk::UpdatePtr2( void *ptr )
{
	egldata *ePtr = (egldata*)ptr;

	g_pWindow = ePtr->window;
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
	if ( !g_pWindow ) return;
	if ( mode < 0 ) mode = 0;

	//glfwSwapInterval( mode );
	if ( mode > 0 ) 
	{
		emscripten_set_main_loop_timing( EM_TIMING_RAF, mode );
	}
	else emscripten_set_main_loop_timing( EM_TIMING_SETTIMEOUT, agk::Round(m_fSyncTime*1000) );
}

void agk::Sleep( UINT milliseconds )
//****
{
	// convert from milliseconds to microseconds
	usleep( milliseconds*1000 );
}

void agk::PlatformCleanUp()
{
	agk::StopSound(0); // stop all
	AGKMusicOGG::DeleteAll();
	cSoundMgr::Destroy();
}

int agk::GetMaxDeviceWidth()
//****
{
	int width = EM_ASM_INT_V({
		var width = document.documentElement.clientWidth;
		return width;
	});
	return width;
}

int agk::GetMaxDeviceHeight()
//****
{
	int height = EM_ASM_INT_V({
		var height = document.documentElement.clientHeight;
		return height;
	});
	return height;
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
	if ( g_pWindow == NULL ) return;

	//glfwGetFramebufferSize( g_pWindow, &m_iRenderWidth, &m_iRenderHeight );
	//glfwGetWindowSize( g_pWindow, &m_iRealDeviceWidth, &m_iRealDeviceHeight );
	int width = EM_ASM_INT_V({return Module["canvas"].width});
	int height = EM_ASM_INT_V({return Module["canvas"].height});
	m_iRenderWidth = width;
	m_iRenderHeight = height;
	m_iRealDeviceWidth = width;
	m_iRealDeviceHeight = height;
	cCamera::UpdateAllAspectRatio( m_iRenderWidth/(float)m_iRenderHeight );

	uString str;
	str.Format( "Width: %d, Height: %d", m_iRenderWidth, m_iRenderHeight );
	agk::Warning( str );
}

void agk::PlatformUpdateTime ( void )
{
	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	int time = now.tv_sec - uFixTime;
	m_fTimeCurr = time + (now.tv_nsec * 1e-9) - fFixTime2;
	m_iTimeMilliseconds = time*1000 + (now.tv_nsec / 1000000) - agk::Round(fFixTime2*1000);
}

void agk::PlatformResetTime ( void )
{
	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	uFixTime = now.tv_sec;
	fFixTime2 = now.tv_nsec * 1e-9;
	
	m_fTimeCurr = 0;
	m_iTimeMilliseconds = 0;
}

double agk::PlatformGetRawTime ( void )
{
	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	double dtime = now.tv_sec;
	dtime = dtime + (now.tv_nsec * 1e-9);
	return dtime;
}

void agk::CompositionChanged()
{

}

void agk::PlatformSync()
{
	// display backbuffer
	PlatformSwap();

	if ( g_pMusic ) m_cMusicMgr.HandleEvent();
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
	
	// enter released
	if ( m_iPrevKeyDown[ AGK_KEY_ENTER ] && !m_iKeyDown[ AGK_KEY_ENTER ] )
	{
		m_bInputStarted = false;
		m_bInputCancelled = false;
	}
	
	// escape released
	if ( m_iPrevKeyDown[ AGK_KEY_ESCAPE ] && !m_iKeyDown[ AGK_KEY_ESCAPE ] )
	{
		m_sCurrInput.SetStr( "" );
		m_bInputCancelled = true;
		m_bInputStarted = false;
		int editbox = agk::GetCurrentEditBox();
		if ( editbox > 0 ) agk::SetEditBoxFocus( editbox, 0 );
	}
	
	// backspace held
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
	
	if ( (m_iPrevKeyDown[ AGK_KEY_BACK ] && !m_iKeyDown[ AGK_KEY_BACK ]) || bBackspaceRepeat )
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
	
	// delete held
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
	
	// delete released
	if ( (m_iPrevKeyDown[ AGK_KEY_DELETE ] && !m_iKeyDown[ AGK_KEY_DELETE ]) || bDeleteRepeat )
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
	
	// left arrow released
	if ( (m_iPrevKeyDown[ AGK_KEY_LEFT ] && !m_iKeyDown[ AGK_KEY_LEFT ]) || bLeftRepeat )
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
	
	// right arrow released
	if ( (m_iPrevKeyDown[ AGK_KEY_RIGHT ] && !m_iKeyDown[ AGK_KEY_RIGHT ]) || bRightRepeat )
	{
		g_iTextCursorPos++;
		if ( g_iTextCursorPos > (int) m_sCurrInput.GetNumChars() ) g_iTextCursorPos = m_sCurrInput.GetNumChars();
		g_iShowCursor = 1;
		g_fCursorBlinkTime = m_fCursorBlinkDelay;
		
		if ( fRightTimer <= 0 ) fRightTimer = 0.5f;
	}
	
	// end released
	if ( m_iPrevKeyDown[ AGK_KEY_END ] && !m_iKeyDown[ AGK_KEY_END ] )
	{
		g_iTextCursorPos = m_sCurrInput.GetNumChars();
		g_iShowCursor = 1;
		g_fCursorBlinkTime = m_fCursorBlinkDelay;
	}
	
	// home released
	if ( m_iPrevKeyDown[ AGK_KEY_HOME ] && !m_iKeyDown[ AGK_KEY_HOME ] )
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
		
	pTextBackground->SetPosition( -m_iDisplayExtraX, -m_iDisplayExtraY );
	pTextBackground->SetSize( m_iDisplayWidth+m_iDisplayExtraX*2, m_iDisplayHeight+m_iDisplayExtraY*2 );
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

	agk::PlatformSetDepthTest( 1 );
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
	return (int) ( floor( a ) );
}

int agk::Ceil( float a )
{
	return (int) ( ceil( a ) );
}

int agk::Round( float a )
{
	if ( a < 0 ) return (int) ( ceil( a-0.5f ) );
	else return (int) ( floor( a+0.5f ) );
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
	// do nothing
	return false;
}

void agk::PlatformShowChooseScreen()
{
	m_bIsChoosing = false;

	// todo ?
}

bool agk::PlatformShowCaptureScreen()
{
	// todo
	// show capture image screen on blackberry?
	return false;
}

void agk::PlatformHideCaptureScreen()
{
	// do nothing
}

bool cImage::CaptureFromCamera()
{
	// do nothing, use ShowImageCaptureScreen() instead
	return false;
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
	agk::Message( "AGK does not currently support printing images from HTML5" );
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

}

char* agk::GetClipboardText()
//****
{
	char *str = new char[1]; *str = 0;
	return str;
}

// Music

void cMusicMgr::PlatformAddFile( cMusic *pMusic )
{
    
}

bool cMusicMgr::PlatformPrepare( UINT iID )
{
	return false;
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

	cMusic *pMusic = m_pMusicFiles[ iID ];
	
	if ( g_pMusic )
	{
		Mix_FreeMusic(g_pMusic);
		g_pMusic = 0;
	}
	
	// try write folder
	uString sPath( pMusic->m_sFile.GetStr() );
	if ( !agk::GetRealPath( sPath ) )
	{
		uString err;
		err.Format( "Could not find file at path: %s", pMusic->m_sFile.GetStr() );
		agk::Warning( err );
		return;
	}
	
	g_pMusic = Mix_LoadMUS(sPath.GetStr());
	if ( !g_pMusic )
	{
		uString err;
		err.Format( "Failed to load music file: %s", pMusic->m_sFile.GetStr() );
		agk::Warning( err );
		return;
	}
	
	int result = Mix_PlayMusic( g_pMusic, 0 ); // looping handled by HandleEvent
	if ( result != 0 )
	{
		uString err;
		err.Format( "Failed to play music file: %s", pMusic->m_sFile.GetStr() );
		agk::Warning( err );
		return;
	}
	
	//Mix_SetPanning( MIX_CHANNEL_POST, 255,255 );
	
	g_fMusicStartTime = agk::Timer();
	g_fMusicPauseTime = 0;
	
	m_pCurrentlyPlaying = pMusic;
	
	float volume = (m_iMasterVolume * pMusic->m_iVolume) / 10000.0f;
	if ( volume < 0 ) volume = 0;
	if ( volume > 1 ) volume = 1;
	
	int SDLVolume = (int) (volume * MIX_MAX_VOLUME);
	Mix_VolumeMusic( SDLVolume );
}

void cMusicMgr::Pause()
{
	if ( m_pCurrentlyPlaying == UNDEF ) return;
	
	Mix_PauseMusic();
	g_fMusicPauseTime = agk::Timer();
}

void cMusicMgr::Resume()
{
	if ( m_pCurrentlyPlaying == UNDEF ) return;
	
	Mix_ResumeMusic();
	
	if ( g_fMusicPauseTime > 0 )
	{
		g_fMusicStartTime += (agk::Timer() - g_fMusicPauseTime);
	}
	g_fMusicPauseTime = 0;
}

void cMusicMgr::Stop()
{
	if ( m_pCurrentlyPlaying == UNDEF ) return;
	
	Mix_HaltMusic();
	m_pCurrentlyPlaying = UNDEF;
}

void cMusicMgr::SetMasterVolume( int vol )
{
	if ( vol < 0 ) vol = 0;
	if ( vol > 100 ) vol = 100;
	m_iMasterVolume = vol;
	
	float volume = m_iMasterVolume / 100.0f;
	if ( m_pCurrentlyPlaying )
	{
		volume *= m_pCurrentlyPlaying->m_iVolume / 100.0f;
	}

	if ( volume < 0 ) volume = 0;
	if ( volume > 1 ) volume = 1;

	int SDLVolume = (int) (volume * MIX_MAX_VOLUME);
	Mix_VolumeMusic( SDLVolume );
}

float cMusicMgr::GetDuration( UINT ID )
{
	// todo
	return 0;
}

void cMusicMgr::Seek( float seconds, int mode )
{
	if ( !m_pCurrentlyPlaying ) return;
	
	static int warned = 0;
	if ( !warned )
	{
		warned = 1;
		agk::Warning( "HTML5 does not support seeking for music" );
	}
}

float cMusicMgr::GetPosition()
{
	if ( !m_pCurrentlyPlaying ) return 0;
	
	float mustime = agk::Timer() - g_fMusicStartTime;
	if ( g_fMusicPauseTime > 0 ) mustime -= (agk::Timer() - g_fMusicPauseTime);
	
	return mustime;
}

void cMusicMgr::HandleEvent()
{
	if ( !m_pCurrentlyPlaying ) return;
	
	if ( !Mix_PlayingMusic() )
	{
		UINT iCurrID = m_pCurrentlyPlaying->m_iID;
		UINT iNextID = 0;

		if ( iCurrID >= m_iEndID )
		{
			if ( m_bLoop ) 
			{
				if ( m_iStartID == m_iEndID )
				{
					iNextID = m_iStartID;
				}
				else
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
	int processed = 0;
    alGetSourcei( pTemp->sourceID, AL_BUFFERS_PROCESSED, &processed);
    
    while( processed > 0 )
    {
        processed--;
        ALuint buf;
        alSourceUnqueueBuffers( pTemp->sourceID, 1, &buf );
    }
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
	int processed = 0;
    alGetSourcei( pTemp->sourceID, AL_BUFFERS_PROCESSED, &processed);
    
    while( processed > 0 )
    {
        processed--;
        ALuint buf;
        alSourceUnqueueBuffers( pTemp->sourceID, 1, &buf );
    }
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
    
    int queued = 0;
	alGetSourcei( pTemp->sourceID, AL_BUFFERS_QUEUED, &queued);
    
    uString info;
    info.Format("Queued: %d, Processed: %d", queued, processed);
    //agk::Warning( info );
    
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
			if(!audioContext) agk::Error( "Failed to create audio context" );
			alcMakeContextCurrent(audioContext);
		}
	}

	m_fMinPlaybackRate = 1.0f;
	m_fMaxPlaybackRate = 1.0f;
	m_fStepPlaybackRate = 0.01f;
	
	// music
	if ( SDL_Init(SDL_INIT_AUDIO) < 0 )
	{
		agk::Warning( "Failed to init SDL audio" );
		return;
	}
	
	Mix_OpenAudio( 22050, AUDIO_S16, 2, 1024 );
}

void cSoundMgr::AppPaused()
{
    
}

void cSoundMgr::AppResumed()
{
    
}

void cSoundMgr::PlatformAddFile( cSoundFile *pSound )
{
	// no further processing needed on linux
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
				}
			}

			buffers = 0;
			alGetSourcei(pSound->sourceID, AL_BUFFERS_QUEUED, &buffers);

			//int state = 0;
			//alGetSourcei( pSound->sourceID, AL_SOURCE_STATE, &state );
			//if ( state != AL_PLAYING )
			if ( buffers == 0 )
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
			agk::Error( "Unsupported wav file bit size, only 8bit and 16bit supported." );
			return 0;
		}
	}
	else if ( pSound->m_fmt.nChannels == 1 )
	{
		if ( pSound->m_fmt.wBitsPerSample == 8 ) format = AL_FORMAT_MONO8;
		else if ( pSound->m_fmt.wBitsPerSample == 16 ) format = AL_FORMAT_MONO16;
		else
		{
			agk::Error( "Unsupported wav file bit size, only 8bit and 16bit supported." );
			return 0;
		}
	}
	else
	{
		agk::Error( "Unsupported wav file number of channels, only 1 (mono) and 2 (stereo) channels supported." );
		return 0;
	}

	m_cSoundInstances.AddItem( pSoundInst, pSoundInst->m_iID );

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
	ALenum result = alGetError();
	if ( result != AL_NO_ERROR )
	{
		agk::Warning( "AL Error" );
	}
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
int g_iVideoStarted = 0;
int agk::LoadVideo( const char *szFilename )
//****
{
	EM_ASM_({ 
		var video = document.getElementById("AGKVideo");
		if ( video == null ) console.log("AGKVideo tag is missing on this page, videos will not play");
		video.autoplay = false;
		video.loop = false;
		video.muted = false;
		video.src = Pointer_stringify($0); 
	}, szFilename);

	return 1;
}

void agk::ChangeVideoPointer( void *ptr )
{
	
}

void agk::HandleVideoEvents()
{
	
}

void agk::DeleteVideo()
//****
{
	EM_ASM({ 
		var video = document.getElementById("AGKVideo");
		if ( video != null ) 
		{
			video.pause();
			video.src = "";
		}
	});
	g_iVideoStarted = 0;

	if ( m_pVideoTexture ) 
	{
		m_cImageList.RemoveItem( m_pVideoTexture->m_iID );
		delete m_pVideoTexture; 
		m_pVideoTexture = 0;
	}
}

void agk::SetVideoDimensions( float x, float y, float width, float height )
//****
{
	m_fVideoX = x;
	m_fVideoY = y;
	m_fVideoWidth = width;
	m_fVideoHeight = height;

	//if ( g_videoContext )
	{
		int iX = agk::ScreenToDeviceX( x );
		int iY = agk::ScreenToDeviceY( y );
		int iWidth = agk::ScreenToDeviceX( x+width ) - iX;
		int iHeight = agk::ScreenToDeviceY( y+height ) - iY;
	}
}

void agk::VideoUpdate()
{
	if ( !m_pVideoTexture || !m_pVideoTexture->GetTextureID() ) return;
	if ( !g_iVideoStarted ) return;
	
	EM_ASM_({ 
		var video = document.getElementById("AGKVideo");
		if ( video != null ) 
		{
			if ( video.src != null && video.src != "" && !video.ended )
			{
				var canvas = document.getElementById('canvas');
				var gl = canvas.getContext('webgl');
				gl.bindTexture(gl.TEXTURE_2D, GL.textures[$0]);
				gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, video); 
			}
		}
	}, m_pVideoTexture->GetTextureID());
}

void agk::PlayVideoToImage( UINT imageID )
//****
{
	cImage *pImage = m_cImageList.GetItem( imageID );
	if ( pImage && pImage != m_pVideoTexture )
	{
		agk::Error( "Failed to play video to image, image already exists" );
		return;
	}

	// create video image if necessary
	if ( m_pVideoTexture ) 
	{
		if ( m_pVideoTexture->m_iID != imageID ) agk::Warning( "Cannot change video image ID during playback, call DeleteVideo first then PlayVideoToImage with the new ID" );
	}
	else
	{
		unsigned int color = 0;
		m_pVideoTexture = new cImage();
		m_pVideoTexture->CreateBlankImage( 1, 1, 0, 0 );
		m_pVideoTexture->LoadFromData( 1, 1, &color, 0 );
		m_pVideoTexture->m_iID = imageID;
		m_cImageList.AddItem( m_pVideoTexture, imageID );
	} 

	EM_ASM({ 
		var video = document.getElementById("AGKVideo");
		if ( video != null )
		{
			video.play();
		}
	});
	g_iVideoStarted = 1;
}

void agk::PlayVideo()
//****
{
	agk::Error( "PlayVideo is not supported on HTML5, use PlayVideoToImage instead" );
}

void agk::PauseVideo()
//****
{
	EM_ASM({ 
		var video = document.getElementById("AGKVideo");
		if ( video != null ) 
		{
			video.pause();
		}
	});
}

void agk::StopVideo()
//****
{
	EM_ASM({ 
		var video = document.getElementById("AGKVideo");
		if ( video != null ) 
		{
			video.pause();
			video.src = "";
		}
	});
	g_iVideoStarted = 0;
}

int agk::GetVideoPlaying()
//****
{
	if ( !g_iVideoStarted ) return 0;
	
	int res = EM_ASM_INT_V({ 
		var video = document.getElementById("AGKVideo");
		var result = 0;
		if ( video != null && video.src != null && video.src != "" && !video.ended ) result = 1;
		return result;
	});
	
	return res;
}

float agk::GetVideoPosition()
//****
{
	return EM_ASM_DOUBLE_V({ 
		var video = document.getElementById("AGKVideo");
		if ( video == null || video.src == null || video.src == "" ) return 0;
		else return video.currentTime;
	});
}

float agk::GetVideoDuration()
//****
{
	return EM_ASM_DOUBLE_V({ 
		var video = document.getElementById("AGKVideo");
		if ( video == null || video.src == null || video.src == "" ) return 0;
		else return video.duration;
	});
}

void agk::SetVideoVolume( float volume )
//****
{
	EM_ASM_({ 
		var video = document.getElementById("AGKVideo");
		if ( video != null ) video.volume = $0 / 100.0;
	}, volume);
}

float agk::GetVideoWidth()
//****
{
	return EM_ASM_INT_V({ 
		var video = document.getElementById("AGKVideo");
		if ( video == null || video.src == null || video.src == "" ) return 0;
		else return video.width;
	});
}

float agk::GetVideoHeight()
//****
{
	return EM_ASM_INT_V({ 
		var video = document.getElementById("AGKVideo");
		if ( video == null || video.src == null || video.src == "" ) return 0;
		else return video.height;
	});
}

void agk::SetVideoPosition( float seconds )
//****
{
	EM_ASM_({ 
		var video = document.getElementById("AGKVideo");
		if ( video != null && video.src != null && video.src != "" ) video.currentTime = $0;
	}, seconds);
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

// string

int uString::ToInt() const
{
	if ( m_iLength == 0 || !m_pData ) return 0;
	return atoi(m_pData);
}

float uString::ToFloat() const
{
	return (float)atof(m_pData);
}


void agk::PlatformReportError( const uString &sMsg )
{
	//fprintf( stderr, "%s\n", sMsg.GetStr());
	const char *szMsg = sMsg.GetStr();
	EM_ASM_({ 
		console.log(Pointer_stringify($0)); 
	}, szMsg);
}

void agk::DialogGroup( const char* group )
{
	
}

void agk::PlatformMessage( const char* msg )
{
	EM_ASM_({ alert(Pointer_stringify($0)); }, msg);
}

// Thread functions

void AGKThread::PlatformInit( )
{
	
}

/*
UINT STDCALL StartNewThread( void* param )
{
	UINT result = ((AGKThread*)param)->EntryPoint( param );
	return result;
}
*/

void AGKThread::PlatformStart( )
{
	m_bRunning = false;
}

void AGKThread::PlatformStop( )
{
	
}

void AGKThread::PlatformTerminate( )
{
	
}

void AGKThread::PlatformCleanUp( )
{
	
}

void AGKThread::PlatformJoin( )
{
	
}

void AGKThread::PlatformSleepSafe( UINT milliseconds )
{
	
}

// filesystem commands

void agk::SetRawWritePath( const char* str )
//****
{
	if ( !str || !*str ) return;
	agk::Warning( "SetRawWritePath is deprecated, the preferred method is now OpenRawFolder and \"raw:\" file paths" );

	strcpy( szWriteDir, str );
	UINT length = strlen( str );
	if ( str[ length-1 ] != '/' ) strcat( szWriteDir, "/" );

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
	char *szStr = new char[ strlen(szWriteDir)+1 ];
	strcpy( szStr, szWriteDir );
	return szStr;
}

bool agk::PlatformChooseFile( uString &out, const char *ext, int fullPath )
{
	return false;
}

void agk::PlatformGetFullPathWrite( uString &inout )
{
	inout.Replace( '\\', '/' );
	if ( inout.FindStr( "../" ) >= 0 )
	{
		uString sub;
		inout.SubString( sub, inout.RevFind( '/' )+1 );
		inout.SetStr( sub );
	}

	if ( inout.CharAt( 0 ) == '/' )
	{
		uString sub;
		inout.SubString( sub, 1 );
		inout.SetStr( sub );
		inout.Prepend( szWriteDir );
	}
	else
	{
		inout.Prepend( m_sCurrentDir );
		inout.Prepend( szWriteDir );
	}

	cFileEntry::ConvertToReal( inout );
}

void agk::PlatformGetFullPathRead( uString &inout, int mode )
{
	inout.Replace( '\\', '/' );
	if ( inout.FindStr( "../" ) >= 0 )
	{
		uString sub;
		inout.SubString( sub, inout.RevFind( '/' )+1 );
		inout.SetStr( sub );
	}

	if ( inout.CharAt( 0 ) == '/' )
	{
		uString sub;
		inout.SubString( sub, 1 );
		inout.SetStr( sub );
		inout.Prepend( szRootDir );
	}
	else
	{
		inout.Prepend( m_sCurrentDir );
		inout.Prepend( szRootDir );
	}

	cFileEntry::ConvertToReal( inout );
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

	uString sPath( path );
	sPath.Replace( '\\', '/' );
	sPath.Trunc( '/' );
	if ( sPath.GetLength() == 0 ) sPath.SetStr( "/" );

	int fd = open( sPath.GetStr(), O_RDONLY | O_CLOEXEC );
	if ( fd >= 0 ) 
	{
		close( fd );
		return 1; // already exists
	}

	int found = 0;
	do
	{
		sPath.Trunc( '/' );
		if ( sPath.GetLength() == 0 ) sPath.SetStr( "/" );
		fd = open( sPath.GetStr(), O_RDONLY | O_CLOEXEC );
		if ( fd >= 0 ) found = 1;
	} while( sPath.GetLength() > 1 && !found );
	
	if ( !found )
	{
		uString err; err.Format( "Failed to create path \"%s\", the app may not have permissions to create folders in the part that exists", path );
		agk::Error( err );
		return 0;
	}

	uString sPath2( path );
	sPath2.Replace( '\\', '/' );
	const char *szRemaining = sPath2.GetStr() + sPath.GetLength() + 1;
	const char *szSlash;
	char szFolder[ MAX_PATH ];
	while ( (szSlash = strchr( szRemaining, '/' )) )
	{
		UINT length = (UINT)(szSlash-szRemaining);
		if ( length == 0 )
		{
			uString err; err.Format( "Invalid path \"%s\", folder names must have at least one character", path );
			agk::Error( err );
			return 0;
		}
		
		strncpy( szFolder, szRemaining, length );
		szFolder[ length ] = '\0';

		int newFd = openat( fd, szFolder, O_RDONLY | O_CLOEXEC );
		if ( newFd < 0 )
		{
			mkdirat( fd, szFolder, 0777 );
			newFd = openat( fd, szFolder, O_RDONLY | O_CLOEXEC );
			if ( newFd < 0 )
			{
				uString err; err.Format( "Failed to create folder \"%s\" in path \"%s\", the app may not have permission to create it", szFolder, path );
				agk::Error( err );
				return 0;
			}
		}

		close( fd );
		fd = newFd;

		szRemaining = szSlash+1;
	}

	close( fd );

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

	DIR *pDir = opendir( szPath );
	if ( !pDir )
	{
		uString err; err.Format( "Failed to open folder \"%s\", it may not exist or this app doesn't have permission to accesss it", szPath );
		agk::Error( err );
		return 0;
	}

	dirent* item = readdir( pDir );
	while( item )
	{
		if ( item->d_type == DT_DIR )
		{
			if ( strcmp( item->d_name, "." ) != 0 && strcmp( item->d_name, ".." ) != 0 )
			{
				m_iNumFolders++;
			}
		}
		else if ( item->d_type == DT_REG )
		{
			m_iNumFiles++;
		}

		item = readdir( pDir );
	}
	closedir( pDir );

	m_pFiles = new uString[ m_iNumFiles ];
	m_pFolders = new uString[ m_iNumFolders ];

	int fileCount = 0;
	int folderCount = 0;

	pDir = opendir( szPath );
	item = readdir( pDir );
	while( item )
	{
		if ( item->d_type == DT_DIR )
		{
			// directory
			if ( strcmp( item->d_name, "." ) != 0 && strcmp( item->d_name, ".." ) != 0 )
			{
				m_pFolders[ folderCount++ ].SetStr( item->d_name );
			}
		}
		else if ( item->d_type == DT_REG )
		{
			// file
			m_pFiles[ fileCount++ ].SetStr( item->d_name );
		}

		item = readdir( pDir );
	}
	closedir( pDir );

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
	agk::PlatformGetFullPathWrite( sPath );

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

	uString sPath( szFilename );
	agk::PlatformGetFullPathRead( sPath );

	struct stat buf;
	if ( stat( sPath.GetStr(), &buf ) != 0 ) return false;

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
	time = 0;
	if ( !szFilename || !*szFilename ) return false;

	UINT length = strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' ) return false;
	
	uString sPath( szFilename );
	if ( !agk::GetRealPath( sPath ) ) return false;
	
	struct stat fileInfo;
	int result = stat( sPath, &fileInfo );
	if ( result != 0 )
	{
		return false;
	}
	
	time = (int) fileInfo.st_mtime;
	return true;
}

void cFile::SetModified( const char *szFilename, int time )
{
	if ( !szFilename || !*szFilename ) return;
	UINT length = strlen(szFilename);
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
	
	remove( sPath.GetStr() );
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
	
	pFilePtr = 0;
	if ( append ) pFile = AGKfopen( sPath.GetStr(), "ab" );
	else pFile = AGKfopen( sPath.GetStr(), "wb" );
	
	if ( !pFile )
	{
		uString err = "Failed to open file for writing ";
		err += szFilename;
		agk::Error( err );
		return false;
	}
	
	// refresh any stored directory details for the new file
	if ( 0 == raw ) cFileEntry::AddNewFile( sPath.GetStr() );
	agk::m_bUpdateFileLists = true;
	
	return true;
}

bool AGK::cFile::OpenToRead( const char *szFilename )
{
	if ( !szFilename || !*szFilename ) return false;
	if ( pFile ) Close();
	mode = 0;

	uString sPath ( szFilename );
	if ( !agk::GetRealPath( sPath ) )
	{
		uString err = "Could not find file ";
		err += szFilename;
		agk::Error( err );
		return false;
	}
	
	pFilePtr = 0;
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
		agk::Error( "Cannot write to file opened for reading" );
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
		agk::Error( "Cannot write to file opened for reading" );
		return;
	}
	fwrite( &f, 4, 1, pFile );
}

void AGK::cFile::WriteString( const char *str )
{
	if ( !pFile ) return;
	if ( mode != 1 )
	{
		agk::Error( "Cannot write to file opened for reading" );
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
		agk::Error( "Cannot write to file opened for reading" );
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

unsigned char cFile::ReadByte( )
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
		agk::Error( "Cannot read from file opened for writing" );
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
		agk::Error( "Cannot read from file opened for writing" );
		return 0;
	}
	float f;
	fread( &f, 4, 1, pFile );
	return f;
}

int AGK::cFile::ReadString( uString &str )
{
	if ( !pFile ) return 0;
	if ( mode != 0 )
	{
		agk::Error( "Cannot read from file opened for writing" );
		return 0;
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
	if ( !pFile ) return 0;
	if ( mode != 0 )
	{
		agk::Error( "Cannot read from file opened for writing" );
		return 0;
	}
	
	return fread( str, 1, length, pFile );
}

void agk::ParseCurrentDirectory()
{
	
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

	// add a final slash
	UINT length = strlen( szPath );
	if ( szPath[ length-1 ] != '/' && szPath[ length-1 ] != '\\' ) sPath.AppendAscii( '/' );

	m_sCurrentDir.Append( sPath );
	m_sCurrentDir.Replace( '\\', '/' );
	
	//ParseCurrentDirectory();
	m_bUpdateFileLists = true;
	
	return 1;
}

int agk::MakeFolder( const char* szName )
{
	if ( !szName || strlen( szName ) == 0 )
	{
		return 0;
	}

	if ( strncmp(szName, "raw:", 4) == 0 )
	{
		uString sPath( szName+4 );
		sPath.AppendAscii( '/' );
		PlatformCreateRawPath( sPath );
	}
	else
	{
		// no special characters
		if ( strchr( szName, ':' ) || strchr( szName, '/' ) || strchr( szName, '\\' ) || strstr( szName, ".." )  )
		{
			agk::Error( "Invalid folder name for MakeFolder, it must not contain the special characters / : \\ .." );
			return 0;
		}

		uString sPath( szName );
		PlatformGetFullPathWrite( sPath );
		sPath.AppendAscii( '/' );
		PlatformCreateRawPath( sPath );

		m_bUpdateFileLists = true;
	}
	
	return true;
}

void agk::DeleteFolder( const char* szName )
{
	if ( !szName || strlen( szName ) == 0 )
	{
		return;
	}

	if ( strncmp(szName, "raw:", 4) == 0 )
	{
		uString sPath( szName+4 );
		sPath.Replace( '\\', '/' );
		int pos = sPath.RevFind( '/' );
		if ( pos < 0 ) return;
		uString sFolder;
		sPath.SubString( sFolder, pos+1 );
		sPath.Trunc( '/' );
		
		if ( chdir( sPath.GetStr() ) < 0 ) return;
		rmdir( sFolder.GetStr() );
		chdir( szWriteDir );
	}
	else
	{
		// no special characters
		if ( strchr( szName, ':' ) || strchr( szName, '/' ) || strchr( szName, '\\' ) || strstr( szName, ".." )  )
		{
			agk::Error( "Invalid folder name for DeleteFolder, it must not contain the special characters / : \\ .." );
			return;
		}

		uString sDirPath( szWriteDir );
		sDirPath.Append( m_sCurrentDir );
		if ( chdir( sDirPath.GetStr() ) < 0 ) return;

		rmdir( szName );
		chdir( szWriteDir );
	
		m_bUpdateFileLists = true;
	}
}

// input commands
int agk::GetMultiTouchExists()
{
	return 1;
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
	return 0; // todo
}

void agk::SetRawMouseVisible( int visible )
//****
{
	if ( !g_pWindow ) return;
	glfwSetInputMode( g_pWindow, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN );
}

void agk::SetRawMousePosition( float x, float y )
//****
{
	if ( !g_pWindow ) return;
	
	int realX = agk::ScreenToDeviceX( x );
	int realY = agk::ScreenToDeviceY( y );
	
	glfwSetCursorPos( g_pWindow, realX, realY );
	
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

void cJoystick::DetectJoysticks()
{
	int numJoysticks = emscripten_get_num_gamepads();
	if ( numJoysticks == EMSCRIPTEN_RESULT_NOT_SUPPORTED ) return;
	if ( numJoysticks > AGK_NUM_JOYSTICKS ) numJoysticks = AGK_NUM_JOYSTICKS;
	
	for( int i = 0; i < AGK_NUM_JOYSTICKS; i++ )
	{
		if ( agk::m_pJoystick[i] ) agk::m_pJoystick[i]->m_iConnected = 0;
	}
	
	for( int i = 0; i < numJoysticks; i++ )
	{
		EmscriptenGamepadEvent pStatus ;
		emscripten_get_gamepad_status( i, &pStatus );
		int index = pStatus.index;
		if ( index >= AGK_NUM_JOYSTICKS ) continue;
		if ( !pStatus.connected ) continue;
		
		if ( agk::m_pJoystick[index] ) agk::m_pJoystick[index]->m_iConnected = 1;
		else
		{
			// new joystick
			agk::m_pJoystick[index] = new cJoystick( 0 );
			agk::m_pJoystick[index]->m_iDeviceType = index | 0x30000;
		}
	}

	/*
	if (MON != NULL) {
		fd_set r;
		struct timeval tv;
		int fd = udev_monitor_get_fd(MON);

		// set up a poll on the udev device
		FD_ZERO(&r);
		FD_SET(fd, &r);

		tv.tv_sec = 0;
		tv.tv_usec = 0;

		select(fd + 1, &r, 0, 0, &tv);

		// test if we have a device change
		if (FD_ISSET(fd, &r)) {
			struct udev_device* dev = udev_monitor_receive_device(MON);
			if (dev) {
				const char* devNode = udev_device_get_devnode(dev);
				const char* sysPath = udev_device_get_syspath(dev);
				const char* action = udev_device_get_action(dev);
				sysPath = udev_device_get_syspath(dev);
				action = udev_device_get_action(dev);

				if (strstr(sysPath, "/js") != 0) {
					if (strcmp(action, "remove") == 0) 
					{
						for ( int i = 0; i < AGK_NUM_JOYSTICKS; i++ )
						{
							if ( strcmp((char*)(agk::m_pJoystick[ i ]->m_pDevice), devNode ) == 0 )
							{
								agk::m_pJoystick[ i ]->m_iConnected = 0;
								break;							
							}
						}
					} 
					else if (strcmp(action, "add") == 0) 
					{
						int index = 0;
						while ( index < AGK_NUM_JOYSTICKS && agk::m_pJoystick[ index ] && agk::m_pJoystick[ index ]->GetConnected() ) index++;
						if ( index < AGK_NUM_JOYSTICKS )
						{
							if ( agk::m_pJoystick[ index ] )
							{
								if ( agk::m_pJoystick[ index ]->m_pDevice ) delete [] (char*)(agk::m_pJoystick[ index ]->m_pDevice);
								delete agk::m_pJoystick[ index ];				
							}
							agk::m_pJoystick[ index ] = new cJoystick( strdup(devNode) );
				
							agk::m_pJoystick[ index ]->m_iDeviceType = open(devNode, O_RDWR|O_NONBLOCK);
							if (agk::m_pJoystick[ index ]->m_iDeviceType == -1) {
								// attempt to open in read-only mode if access was denied
								if (errno == EACCES) {
									agk::m_pJoystick[ index ]->m_iDeviceType = open(devNode, O_RDONLY|O_NONBLOCK);
									if (agk::m_pJoystick[ index ]->m_iDeviceType == -1) {
										delete [] (char*)(agk::m_pJoystick[ index ]->m_pDevice);
										delete agk::m_pJoystick[ index ];
										agk::m_pJoystick[ index ] = 0;
									}
								}
							}
						}
					}
				}

				udev_device_unref(dev);
			}
		}
	}
*/
}

void cJoystick::PlatformUpdate()
{
	int numJoysticks = emscripten_get_num_gamepads();
	if ( numJoysticks == EMSCRIPTEN_RESULT_NOT_SUPPORTED ) return;
	if ( numJoysticks > AGK_NUM_JOYSTICKS ) numJoysticks = AGK_NUM_JOYSTICKS;
	
	for( int i = 0; i < numJoysticks; i++ )
	{
		EmscriptenGamepadEvent pStatus ;
		emscripten_get_gamepad_status( i, &pStatus );
		if ( pStatus.index != (m_iDeviceType & 0xffff) ) continue;
		
		for( int j = 0; j < 32; j++ ) m_iButtons[ j ] = pStatus.digitalButton[ j ];

		m_fX = pStatus.axis[0];
		m_fY = pStatus.axis[1];
		
		if ( pStatus.numAxes > 4 )
		{
			m_fZ = pStatus.axis[2]/2.0f + 0.5f;
			if ( m_fZ != 0.5f ) m_iDeviceType &= 0x2ffff; // trigger has been pressed
			if ( m_iDeviceType & 0x10000 ) m_fZ = 0; // trigger starts at 0.5 until it is pressed, so reset it to 0
			
			m_fRX = pStatus.axis[3];
			m_fRY = pStatus.axis[4];
			
			m_fRZ = pStatus.axis[5]/2.0f + 0.5f;
			if ( m_fRZ != 0.5f ) m_iDeviceType &= 0x1ffff; // trigger has been pressed
			if ( m_iDeviceType & 0x20000 ) m_fRZ = 0; // trigger starts at 0.5 until it is pressed, so reset it to 0
			
			m_iButtons[ 8 ] = pStatus.digitalButton[9];
			m_iButtons[ 9 ] = pStatus.digitalButton[10];
			m_iButtons[ 10 ] = 0;
			m_iButtons[ 11 ] = 0;
			
			// D-Pad
			if ( pStatus.numAxes > 6 )
			{
				m_iButtons[ 12 ] = (pStatus.axis[6] < -0.9f) ? 1 : 0;
				m_iButtons[ 14 ] = (pStatus.axis[6] > 0.9f) ? 1 : 0;
				
				m_iButtons[ 13 ] = (pStatus.axis[7] < -0.9f) ? 1: 0;
				m_iButtons[ 15 ] = (pStatus.axis[7] > 0.9f) ? 1: 0;
			}
		}
		else 
		{
			m_fRX = pStatus.axis[2];
			m_fRY = pStatus.axis[3];
			
			m_fZ = pStatus.analogButton[6];
			m_fRZ = pStatus.analogButton[7];
			
			m_iButtons[ 6 ] = pStatus.digitalButton[8];
			m_iButtons[ 7 ] = pStatus.digitalButton[9];
			m_iButtons[ 8 ] = pStatus.digitalButton[10];
			m_iButtons[ 9 ] = pStatus.digitalButton[11];
			
			m_iButtons[ 10 ] = 0;
			m_iButtons[ 11 ] = 0;
		
			// D-Pad
			m_iButtons[ 12 ] = pStatus.digitalButton[14];
			m_iButtons[ 14 ] = pStatus.digitalButton[15];
				
			m_iButtons[ 13 ] = pStatus.digitalButton[12];
			m_iButtons[ 15 ] = pStatus.digitalButton[13];
		}
		
		break;
	}
		
	/*
	if ( m_iDeviceType == -1 ) return;
	
	struct js_event je;
	
	while (read(m_iDeviceType, &je, sizeof(je)) > 0) 
	{
		switch (je.type) {
			case JS_EVENT_BUTTON:
				// determine which button the event is for
				switch (je.number) {
					case 0: m_iButtons[ 0 ] = je.value ? 1 : 0; break;
					case 1: m_iButtons[ 1 ] = je.value ? 1 : 0; break;
					case 2: m_iButtons[ 2 ] = je.value ? 1 : 0; break;
					case 3: m_iButtons[ 3 ] = je.value ? 1 : 0; break;
					case 4: m_iButtons[ 4 ] = je.value ? 1 : 0; break;
					case 5: m_iButtons[ 5 ] = je.value ? 1 : 0; break;
					case 6: m_iButtons[ 6 ] = je.value ? 1 : 0; break;
					case 7: m_iButtons[ 7 ] = je.value ? 1 : 0; break;
					case 8: break; // XBOX button 
					case 9: m_iButtons[ 8 ] = je.value ? 1 : 0; break;
					case 10: m_iButtons[ 9 ] = je.value ? 1 : 0; break;
					default: break;
				}
				break;
				
			case JS_EVENT_AXIS:
				// normalize and store the axis
				switch (je.number) {
					case 0:	m_fX = je.value / 32768.0f; break;
					case 1:	m_fY = je.value / 32768.0f; break;
					case 2:	m_fZ = (je.value + 32768) / 65535.0f; break;
					case 3:	m_fRX = je.value / 32768.0f; break;
					case 4:	m_fRY = je.value / 32768.0f; break;
					case 5:	m_fRZ = (je.value + 32768) / 65535.0f; break;
					case 6:
						if (je.value == -32767) {
							m_iButtons[ 12 ] = 1;
							m_iButtons[ 14 ] = 0;
						} else if (je.value == 32767) {
							m_iButtons[ 12 ] = 0;
							m_iButtons[ 14 ] = 1;
						} else {
							m_iButtons[ 12 ] = 0;
							m_iButtons[ 14 ] = 0;
						}
						break;
					case 7:
						if (je.value == -32767) {
							m_iButtons[ 13 ] = 1;
							m_iButtons[ 15 ] = 0;
						} else if (je.value == 32767) {
							m_iButtons[ 13 ] = 0;
							m_iButtons[ 15 ] = 1;
						} else {
							m_iButtons[ 13 ] = 0;
							m_iButtons[ 15 ] = 0;
						}
						break;
					default: break;
				}
				break;
				
			default:
				break;
		}
	}
*/
}

INT64 agk::GetUnixTime64()
{
	time_t unixtime = time(NULL);
	return unixtime;
}

int agk::GetUnixTime()
{
	time_t unixtime = time(NULL);
	if ( unixtime > 2147483646 ) unixtime = 2147483647;
	if ( unixtime < -2147483647 ) unixtime = -2147483647;
	return (int) unixtime;
}

int agk::GetDayOfWeek()
//****
{
	time_t a = time ( NULL );
	tm*    b = localtime ( &a );
	int    c = b->tm_wday;
	return c;
}

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
	//return 642; // iPhone
	//return 551; // Ovi
	return 559; // Android
	//return 635; // Blackberry
	//return 641; // SonyEricsson
	//return 661; // Bada
	//return 738; // Palm
	//return 818; // Samsung
	//return 947; // iPad, doesn't seem to produce ads
}

///

void cEditBox::PlatformStartText()
{
	// do nothing on linux
}

void cEditBox::PlatformEndText()
{
	// do nothing on linux
}

void cEditBox::PlatformUpdateExternal()
{
	
}

bool cEditBox::PlatformUpdateText()
{
	return false;
}

void cEditBox::PlatformUpdateTextEnd()
{
	// do nothing
}

void agk::OpenBrowser( const char *url )
//****
{
	if ( !url || strlen(url) == 0 ) return;

	EM_ASM_({ 
		window.open(Pointer_stringify($0),'_blank'); 
	}, url);
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
{
	// do nothing on linux
}

int agk::PushNotificationSetup()
//****
{
	return 0;
}

char* agk::GetPushNotificationToken()
//****
{
	char *str = new char[ 7 ];
	strcpy( str, "Error" );
	return str;
}

// ********************
// Social commands
// ********************

// todo all from here

// internally called by AGK
void agk::PlatformSocialPluginsSetup( void )
{
	// do nothing
}

void agk::PlatformSocialPluginsDestroy( void )
{
	// do nothing
}

// RATING COMMANDS
void agk::PlatformRateApp( const char* szID, const char* title, const char* message )
{
}

// IN APP PURCHASE COMMANDS

void agk::PlatformInAppPurchaseSetKeys( const char* szData1, const char* szData2 )
{
	// do nothing
}

void agk::PlatformInAppPurchaseSetTitle( const char* szTitle )
{
	// do nothing
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
	return 1;
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

// szID: publisher ID provided by AdMob
// horz: 0=left, 1=center, 2=right
// vert: 0=top, 1=center, 2=right
// offsetX: AGK distance to offset the horz position from the left or right edge. Ignored when centered
// offsetY: AGK distance to offset the vert position from the top or bottom edge. Ignored when centered
void agk::PlatformAdMobSetupRelative( const char* szID, int horz, int vert, float offsetX, float offsetY, int type )
{

}

void  agk::PlatformAdMobFullscreen ()
{
}

void  agk::PlatformAdMobCacheFullscreen ()
{
}

void  agk::PlatformSetAdMobVisible ( int iVisible )
{

}

void  agk::PlatformAdMobPosition ( int horz, int vert, float offsetX, float offsetY )
{

}

void agk::PlatformAdMobRequestNewAd( void )
{

}

void agk::PlatformAdMobDestroy( void )
{

}

bool agk::PlatformHasAdMob( void )
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

void agk::PlatformAmazonAdSetTesting( int testing )
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

void RefreshAccessToken()
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

int FacebookStringCompare( const void* a, const void* b )
{
	//FacebookUser* user1 = (FacebookUser*)a;
	//FacebookUser* user2 = (FacebookUser*)b;

	//return user1->name.CompareTo( user2->name );
	return 0;
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

char* agk::PlatformFacebookGetAccessToken ( void )
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


// V108 ULTRABOOK COMMANDS

int agk::GetNotificationType()
{
	return 0;
}

void agk::SetNotificationImage(int iImageIndex)
{
}

void agk::SetNotificationText( const char* pText)
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

// GPS
int agk::GetGPSSensorExists()
{
	return m_iGPSSensorExists;
}

void agk::StartGPSTracking()
{
	// todo
}

void agk::StopGPSTracking()
//****
{
	// todo
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
	EM_ASM_({
		var cookieName = UTF8ToString($0);
		var cookieValue = UTF8ToString($1);
		var d = new Date();
		d.setTime(d.getTime() + (5 * 365 * 24 * 60 * 60 * 1000)); // 5 years
		var expires = "expires="+d.toUTCString();
		document.cookie = cookieName + "=" + cookieValue + ";" + expires + ";path=/";
	}, varName, varValue);
}

char* agk::LoadSharedVariable( const char *varName, const char *defaultValue )
{
	char* jsString = (char*) EM_ASM_INT({
		var cookieName = UTF8ToString($0);
		var returnValue = UTF8ToString($1);
		var name = cookieName + "=";
		var ca = document.cookie.split(';');
		for(var i = 0; i < ca.length; i++) {
			var c = ca[i];
			while (c.charAt(0) == ' ') {
				c = c.substring(1);
			}
			if (c.indexOf(name) == 0) {
				returnValue = c.substring(name.length, c.length);
				break;
			}
		}
		var lengthBytes = lengthBytesUTF8(returnValue)+1;
		var heapString = _malloc(lengthBytes);
		stringToUTF8(returnValue, heapString, lengthBytes);
		return heapString;
	}, varName, defaultValue);

	char *str = new char[ strlen(jsString)+1 ];
	strcpy( str, jsString );
	free(jsString);
	return str;
}

void agk::DeleteSharedVariable( const char *varName )
{
	EM_ASM_({
		var cookieName = UTF8ToString($0);
		document.cookie = cookieName + "=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/";
	}, varName);
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
