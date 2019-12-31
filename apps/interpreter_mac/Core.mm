// includes
#import <Cocoa/Cocoa.h>
#include "agk.h"
#include "interpreter.h"
#include "../../platform/mac/GLFW/glfw3.h"

void PlatformAppQuit()
{
    exit(0);
}

// platform specific
unsigned int TranslateKey( unsigned int key )
{
	switch( key )
	{
        case GLFW_KEY_BACKSPACE: key = 8; break;
        case GLFW_KEY_TAB: key = 9; break;
        case GLFW_KEY_ENTER: key = 13; break;
        case GLFW_KEY_KP_ENTER: key = 13; break; // Num pad enter
        case GLFW_KEY_LEFT_SHIFT: key = 257; break; // 16
        case GLFW_KEY_RIGHT_SHIFT: key = 258; break; // 16
        case GLFW_KEY_LEFT_CONTROL: key = 259; break; // 17
        case GLFW_KEY_RIGHT_CONTROL: key = 260; break; // 17
        case GLFW_KEY_LEFT_ALT: key = 261; break;  // 18
        case GLFW_KEY_RIGHT_ALT: key = 262; break;  // 18
        case GLFW_KEY_PAUSE: key = 19; break;
        case GLFW_KEY_CAPS_LOCK: key = 20; break;
        case GLFW_KEY_ESCAPE: key = 27; break;
            
        case GLFW_KEY_SPACE: key = 32; break;
        case GLFW_KEY_PAGE_UP: key = 33; break;
        case GLFW_KEY_PAGE_DOWN: key = 34; break;
        case GLFW_KEY_END: key = 35; break;
        case GLFW_KEY_HOME: key = 36; break;
            
        case GLFW_KEY_LEFT: key = 37; break;
        case GLFW_KEY_UP: key = 38; break;
        case GLFW_KEY_RIGHT: key = 39; break;
        case GLFW_KEY_DOWN: key = 40; break;
        case GLFW_KEY_INSERT: key = 45; break;
        case GLFW_KEY_DELETE: key = 46; break;
            
        case GLFW_KEY_LEFT_SUPER: key = 91; break;
        case GLFW_KEY_RIGHT_SUPER: key = 92; break;
            
            // Numpad
        case GLFW_KEY_KP_0: key = 96; break;
        case GLFW_KEY_KP_1: key = 97; break;
        case GLFW_KEY_KP_2: key = 98; break;
        case GLFW_KEY_KP_3: key = 99; break;
        case GLFW_KEY_KP_4: key = 100; break;
        case GLFW_KEY_KP_5: key = 101; break;
        case GLFW_KEY_KP_6: key = 102; break;
        case GLFW_KEY_KP_7: key = 103; break;
        case GLFW_KEY_KP_8: key = 104; break;
        case GLFW_KEY_KP_9: key = 105; break;
        case GLFW_KEY_KP_MULTIPLY: key = 106; break;
        case GLFW_KEY_KP_ADD: key = 107; break;
        case GLFW_KEY_KP_SUBTRACT: key = 109; break;
        case GLFW_KEY_KP_DECIMAL: key = 110; break;
        case GLFW_KEY_KP_DIVIDE: key = 111; break;
            
            // F1 - F12
        case GLFW_KEY_F1: key = 112; break;
        case GLFW_KEY_F2: key = 113; break;
        case GLFW_KEY_F3: key = 114; break;
        case GLFW_KEY_F4: key = 115; break;
        case GLFW_KEY_F5: key = 116; break;
        case GLFW_KEY_F6: key = 117; break;
        case GLFW_KEY_F7: key = 118; break;
        case GLFW_KEY_F8: key = 119; break;
        case GLFW_KEY_F9: key = 120; break;
        case GLFW_KEY_F10: key = 121; break;
        case GLFW_KEY_F11: key = 122; break;
        case GLFW_KEY_F12: key = 123; break;
            
        case GLFW_KEY_NUM_LOCK: key = 144; break;
        case GLFW_KEY_SCROLL_LOCK: key = 145; break;
            
        case GLFW_KEY_SEMICOLON: key = 186; break; // ;
        case GLFW_KEY_EQUAL: key = 187; break;
        case GLFW_KEY_COMMA: key = 188; break;
        case GLFW_KEY_MINUS: key = 189; break;
        case GLFW_KEY_PERIOD: key = 190; break;
        case GLFW_KEY_SLASH: key = 191; break;
        case GLFW_KEY_APOSTROPHE: key = 192; break; // '
        case GLFW_KEY_LEFT_BRACKET: key = 219; break; // [
        case GLFW_KEY_BACKSLASH: key = 220; break;
        case GLFW_KEY_RIGHT_BRACKET: key = 221; break; // ]
        case GLFW_KEY_WORLD_1: key = 222; break; // # (maybe)
        case GLFW_KEY_WORLD_2: key = 222; break; // # (maybe)
        case GLFW_KEY_GRAVE_ACCENT: key = 223; break; // `
	}
	
	return key;
}

static void keyboard_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
    if ( key < 0 ) return;
    
    if ( action == GLFW_PRESS )
    {
        int key2 = TranslateKey(key);
        agk::KeyDown( key2 );
        
        switch( key2 )
        {
                // Top row 0-9
            case 48: agk::KeyDown(263); break;
            case 49: agk::KeyDown(264); break;
            case 50: agk::KeyDown(265); break;
            case 51: agk::KeyDown(266); break;
            case 52: agk::KeyDown(267); break;
            case 53: agk::KeyDown(268); break;
            case 54: agk::KeyDown(269); break;
            case 55: agk::KeyDown(270); break;
            case 56: agk::KeyDown(271); break;
            case 57: agk::KeyDown(272); break;
                
                // Num pad 0-9
            case 96: agk::KeyDown(48); break;
            case 97: agk::KeyDown(49); break;
            case 98: agk::KeyDown(50); break;
            case 99: agk::KeyDown(51); break;
            case 100: agk::KeyDown(52); break;
            case 101: agk::KeyDown(53); break;
            case 102: agk::KeyDown(54); break;
            case 103: agk::KeyDown(55); break;
            case 104: agk::KeyDown(56); break;
            case 105: agk::KeyDown(57); break;
                
                // shift
            case 257:
            case 258: agk::KeyDown(16); break;
                
                // control
            case 259:
            case 260: agk::KeyDown(17); break;
                
                // alt
            case 261:
            case 262: agk::KeyDown(18); break;
        }
    }
    else if ( action == GLFW_RELEASE )
    {
        int key2 = TranslateKey(key);
        agk::KeyUp( key2 );
        
        switch( key2 )
        {
                // Top row 0-9
            case 48: agk::KeyUp(263); break;
            case 49: agk::KeyUp(264); break;
            case 50: agk::KeyUp(265); break;
            case 51: agk::KeyUp(266); break;
            case 52: agk::KeyUp(267); break;
            case 53: agk::KeyUp(268); break;
            case 54: agk::KeyUp(269); break;
            case 55: agk::KeyUp(270); break;
            case 56: agk::KeyUp(271); break;
            case 57: agk::KeyUp(272); break;
                
                // Num pad 0-9
            case 96: agk::KeyUp(48); break;
            case 97: agk::KeyUp(49); break;
            case 98: agk::KeyUp(50); break;
            case 99: agk::KeyUp(51); break;
            case 100: agk::KeyUp(52); break;
            case 101: agk::KeyUp(53); break;
            case 102: agk::KeyUp(54); break;
            case 103: agk::KeyUp(55); break;
            case 104: agk::KeyUp(56); break;
            case 105: agk::KeyUp(57); break;
                
                // shift
            case 257:
            case 258: agk::KeyUp(16); break;
                
                // control
            case 259:
            case 260: agk::KeyUp(17); break;
                
                // alt
            case 261:
            case 262: agk::KeyUp(18); break;
        }
    }
    
}

static void char_callback( GLFWwindow* window, unsigned int key )
{
    agk::CharDown( key );
}

static void mouse_pos_callback( GLFWwindow* window, double x, double y )
{
    agk::MouseMove( 0, agk::Round(x), agk::Round(y) );
}

static void mouse_button_callback( GLFWwindow* window, int button, int action, int mods )
{
    if ( button == GLFW_MOUSE_BUTTON_LEFT )
    {
        if ( action == GLFW_PRESS ) agk::MouseLeftButton( 0, 1 );
        else if ( action == GLFW_RELEASE ) agk::MouseLeftButton( 0, 0 );
    }
    else if ( button == GLFW_MOUSE_BUTTON_RIGHT )
    {
        if ( action == GLFW_PRESS ) agk::MouseRightButton( 0, 1 );
        else if ( action == GLFW_RELEASE ) agk::MouseRightButton( 0, 0 );
    }
    else if ( button == GLFW_MOUSE_BUTTON_MIDDLE )
    {
        if ( action == GLFW_PRESS ) agk::MouseMiddleButton( 0, 1 );
        else if ( action == GLFW_RELEASE ) agk::MouseMiddleButton( 0, 0 );
    }
}

static void mouse_wheel_callback( GLFWwindow* window, double x, double y )
{
    agk::MouseWheel(0, y);
}

static void window_size_callback( GLFWwindow* window, int width, int height)
{
    agk::UpdateDeviceSize();
}

static void close_callback( GLFWwindow* window )
{
    App.AppSplash();
    glfwSetWindowShouldClose( window, 1 );
}

static void focus_callback( GLFWwindow* window, int focus )
{
    if ( focus == GLFW_TRUE ) agk::Resumed();
    else agk::Paused();
}

void Output ( char const* debugtext )
{
	// report this text to the internal debug reporting system for MAC
}

struct egldata
{
    GLFWwindow* window;
};

int main (int argc, char **argv)
{
    // this must be done before setting up the window due to memory issues in IOCreatePlugInInterfaceForService
    agk::InitJoysticks();
   
    App.g_dwDeviceWidth = DEVICE_WIDTH;
	App.g_dwDeviceHeight = DEVICE_HEIGHT;
	App.g_dwFullScreen = FULLSCREEN ? 1 : 0;

	char* pSetupFile = (char*)"setup.agc";
	if ( agk::GetFileExists ( pSetupFile )==1 )
	{
		char* pField = (char*)"";
		strcpy ( App.g_pWindowTitle, "" );
		agk::OpenToRead ( 1, pSetupFile );
		while ( agk::FileEOF ( 1 )==false ) 
		{
			char* pLineToRead = agk::ReadLine ( 1 );
			pField=(char*)"title="; if ( strncmp ( pLineToRead, pField, strlen(pField) )==0 )	strcpy ( App.g_pWindowTitle, pLineToRead+strlen(pField) );
			pField=(char*)"width="; if ( strncmp ( pLineToRead, pField, strlen(pField) )==0 )	App.g_dwDeviceWidth = (unsigned int)atoi(pLineToRead+strlen(pField));
			pField=(char*)"height="; if ( strncmp ( pLineToRead, pField, strlen(pField) )==0 )	App.g_dwDeviceHeight = (unsigned int)atoi(pLineToRead+strlen(pField));
			pField=(char*)"fullscreen="; if ( strncmp ( pLineToRead, pField, strlen(pField) )==0 )	App.g_dwFullScreen = (unsigned int)atoi(pLineToRead+strlen(pField));
			pField=(char*)"resolutionmode="; if ( strncmp ( pLineToRead, pField, strlen(pField) )==0 )	App.g_dwResolutionMode = (unsigned int)atoi(pLineToRead+strlen(pField));
		}
		agk::CloseFile ( 1 );
	}
    
	// create a window for the app
	if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        exit(1);
    }
    
    NSScreen *mainScreen = [NSScreen mainScreen];
    NSRect screenRect = [mainScreen visibleFrame];
    
    int width = App.g_dwDeviceWidth;
    int height = App.g_dwDeviceHeight;
    
    float appAspect = width / (float) height;
	float windowAspect = (screenRect.size.width-15) / (float) (screenRect.size.height-80);
    
	if ( appAspect > windowAspect )
	{
		if ( width > screenRect.size.width-15 )
		{
			float ratio = (screenRect.size.width-15) / (float)width;
			width = screenRect.size.width-15;
			height = (int) height*ratio;
		}
	}
	else
	{
		if ( height > screenRect.size.height-80 )
		{
			float ratio = (screenRect.size.height-80) / (float)height;
			height = screenRect.size.height-80;
			width = (int) width*ratio;
		}
	}
    
    App.g_dwDeviceWidth = width;
    App.g_dwDeviceHeight = height;

	glfwWindowHint( GLFW_SAMPLES, 4 );
    glfwWindowHint( GLFW_RED_BITS, 8 );
    glfwWindowHint( GLFW_GREEN_BITS, 8 );
    glfwWindowHint( GLFW_BLUE_BITS, 8 );
    glfwWindowHint( GLFW_ALPHA_BITS, 8 );
    glfwWindowHint( GLFW_DEPTH_BITS, 24 );
    
    glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
    
    GLFWwindow *window = glfwCreateWindow(App.g_dwDeviceWidth, App.g_dwDeviceHeight, App.g_pWindowTitle, NULL, NULL);
    if ( !window )
    {
        glfwTerminate();
        fprintf(stderr, "Failed to open GLFW window\n");
        exit(1);
    }
    
    glfwMakeContextCurrent(window);
    
    glfwSetWindowSizeCallback( window, window_size_callback );
    glfwSetKeyCallback( window, keyboard_callback );
    glfwSetCursorPosCallback( window, mouse_pos_callback );
    glfwSetMouseButtonCallback( window, mouse_button_callback );
    glfwSetScrollCallback( window, mouse_wheel_callback );
    glfwSetCharCallback( window, char_callback );
    glfwSetWindowCloseCallback( window, close_callback );
    glfwSetWindowFocusCallback( window, focus_callback );
    
	// initialise graphics API for app
    agk::SetExtraAGKPlayerAssetsMode ( 2 );
    
    // call app begin
	int done = 0;
	try
	{
        egldata data;
        data.window = window;
        agk::InitGL(&data);
        
        if ( App.g_dwFullScreen==1 )
            agk::SetWindowSize( 0,0, 1 );

		[[[[[NSApp mainMenu] itemAtIndex:1] submenu] itemAtIndex:0] setTitle:@"Minimize"];

		App.Begin();
	}
	catch( ... )
	{
		uString err = agk::GetLastError();
		err.Prepend( "Uncaught exception: \n\n" );
		NSString* pString = [ [ NSString alloc ] initWithUTF8String:err.GetStr() ];
		NSAlert *alert = [[NSAlert alloc] init];
		[ alert setMessageText: pString ];
		[ pString release ];
		[ alert runModal ];
		[ alert release ];
		done = 1;
	}
    
    while (!done && !glfwWindowShouldClose(window))
	{
		// call app each frame
		try
		{
            App.Loop();
            glfwPollEvents();
		}
		catch( ... )
		{
			uString err = agk::GetLastError();
			err.Prepend( "Uncaught exception: \n\n" );
			NSString* pString = [ [ NSString alloc ] initWithUTF8String:err.GetStr() ];
			NSAlert *alert = [[NSAlert alloc] init];
			[ alert setMessageText: pString ];
			[ pString release ];
			[ alert runModal ];
			[ alert release ];
			done = 1;
		}
	}
	
	// close app
	App.End();
    agk::CleanUp();
	
	// close window system
    glfwDestroyWindow(window);
    glfwTerminate();
	
	// success    
    return 0;
}


