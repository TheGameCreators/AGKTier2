// includes
#include "agk.h"
#include "interpreter.h"

#include "bcm_host.h"
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"

#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include "X11/extensions/xf86vmode.h"

#include <signal.h>

#define ELEMENT_CHANGE_LAYER  	      0x01
#define ELEMENT_CHANGE_OPACITY  	  0x02
#define ELEMENT_CHANGE_DEST_RECT  0x04
#define ELEMENT_CHANGE_SRC_RECT  	  0x08

volatile int done = 0;
void PlatformAppQuit()
{
    done = 1;
}

void sig_handler(int sig)
{
	done = 1;
}

// platform specific
unsigned int TranslateKey( unsigned int key )
{
	switch( key )
	{
		case 9: key = 27; break; // Esc
			
		// F1 - F8
		case 67: key = 112; break;
		case 68: key = 113; break;
		case 69: key = 114; break;
		case 70: key = 115; break;
		case 71: key = 116; break;
		case 72: key = 117; break;
		case 73: key = 118; break;
		case 74: key = 119; break;
			
		case 49: key = 223; break; // `
		case 20: key = 189; break; // -
		case 21: key = 187; break; // =
		case 110: key = 36; break; // Home
		case 112: key = 33; break; // PgUp
		case 117: key = 34; break; // PgDown
		case 115: key = 35; break; // End
		case 119: key = 46; break; // Del
		//case 127: key = 223; break;  // +-
		case 118: key = 45; break; // Insert
			
		case 50: key = 16; break; // Shift (left)
		case 62: key = 16; break; // Shift (right)
		case 37: key = 17; break; // Ctrl (left)
		//case : key = 17; break; // Ctrl (right)
		case 64: key = 18; break;  // Alt (left)
		case 135: key = 18; break;  // Alt Gr (right)
			
		case 23: key = 9; break; // Tab
		case 36: key = 13; break; // Enter
		case 22: key = 8; break; // Backspace
		case 65: key = 32; break; // Space
			
		case 34: key = 219; break; // [
		case 35: key = 221; break; // ]
		case 47: key = 186; break; // ;
		case 48: key = 192; break; // '
			
		case 113: key = 37; break; // Left
		case 111: key = 38; break; // Up
		case 114: key = 39; break; // Right
		case 116: key = 40; break; // Down
			
		case 59: key = 188; break; // ,
		case 60: key = 190; break; // .
		case 61: key = 191; break; // /
		case 94: key = 220; break; // \ 
		case 51: key = 222; break; // #
			
			/*
		case : key = 13; break; // Num pad enter
		case : key = 111; break; // Num pad divide 
		case : key = 106; break; // Num pad multiply
		case : key = 109; break; // Num pad subtract
		case : key = 107; break; // Num pad add
		case : key = 110; break; // Num pad decimcal
		case : key = 0; break; // Pause/Break
		case : key = 0; break; // Caps lock 
		case : key = 45; break; // Num pad 0
		case : key = 35; break; // Num pad 1
		case : key = 40; break; // Num pad 2
		case : key = 34; break; // Num pad 3
		case : key = 37; break; // Num pad 4
		case : key = 12; break; // Num pad 5
		case : key = 39; break; // Num pad 6
		case : key = 36; break; // Num pad 7
		case : key = 38; break; // Num pad 8
		case : key = 33; break; // Num pad 9
		*/
		
		case 19: key = 48; break; // 0
		case 10: key = 49; break; // 1
		case 11: key = 50; break; // 2
		case 12: key = 51; break; // 3
		case 13: key = 52; break; // 4
		case 14: key = 53; break; // 5
		case 15: key = 54; break; // 6
		case 16: key = 55; break; // 7
		case 17: key = 56; break; // 8
		case 18: key = 57; break; // 9
		
		case 38: key = 65; break; // A
		case 56: key = 66; break; // B
		case 54: key = 67; break; // C
		case 40: key = 68; break; // D
		case 26: key = 69; break; // E
		case 41: key = 70; break; // F
		case 42: key = 71; break; // G
		case 43: key = 72; break; // H
		case 31: key = 73; break; // I
		case 44: key = 74; break; // J
		case 45: key = 75; break; // K
		case 46: key = 76; break; // L
		case 58: key = 77; break; // M
		case 57: key = 78; break; // N
		case 32: key = 79; break; // O
		case 33: key = 80; break; // P
		case 24: key = 81; break; // Q
		case 27: key = 82; break; // R
		case 39: key = 83; break; // S
		case 28: key = 84; break; // T
		case 30: key = 85; break; // U
		case 55: key = 86; break; // V
		case 25: key = 87; break; // W
		case 53: key = 88; break; // X
		case 29: key = 89; break; // Y
		case 52: key = 90; break; // Z
	}
	
	return key;
}

void Output ( char const* debugtext )
{
	// report this text to the internal debug reporting system for MAC
}

struct egldata
{
    void* display;
    void* surface;
    void* context;
    Display *XDisplay;
    Window XWindow;
};

typedef struct
{
	unsigned long flags;
	unsigned long functions;
	unsigned long decorations;
	long inputMode;
	unsigned long status;
} Hints;

EGLDisplay display;
EGLSurface surface;
EGLContext context;
static volatile int terminate;

int main (int argc, char **argv)
{
	struct sigaction sig_struct;
	sig_struct.sa_handler = sig_handler;
	sig_struct.sa_flags = 0;
	sigemptyset( &sig_struct.sa_mask );
	
	if ( sigaction(SIGINT, &sig_struct, NULL) == -1 )
	{
		printf( "Failed to set signal handler\n" );
	}
	
	if ( sigaction(SIGTERM, &sig_struct, NULL) == -1 )
	{
		printf( "Failed to set term signal handler\n" );
	}

    bcm_host_init();

    App.g_dwDeviceWidth = 1024;
    App.g_dwDeviceHeight = 768;
    App.g_dwFullScreen = 0;
    
    int32_t success = 0;
    EGLBoolean result;
    EGLint num_config;

    static EGL_DISPMANX_WINDOW_T nativewindow;

    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;
   
    EGLConfig config;

    // get an EGL display connection
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(display!=EGL_NO_DISPLAY);

    // initialize the EGL display connection
    result = eglInitialize(display, NULL, NULL);
    assert(EGL_FALSE != result);

    static const EGLint attribute_list[] =
    {
       EGL_RED_SIZE, 8,
       EGL_GREEN_SIZE, 8,
       EGL_BLUE_SIZE, 8,
       EGL_ALPHA_SIZE, 8,
       EGL_DEPTH_SIZE, 24,
       EGL_CONFORMANT, EGL_OPENGL_ES2_BIT,
       EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
       EGL_NONE
    };

    static const EGLint context_attributes[] = 
    {
       EGL_CONTEXT_CLIENT_VERSION, 2,
       EGL_NONE
    };

    // get an appropriate EGL frame buffer configuration
    result = eglChooseConfig(display, attribute_list, &config, 1, &num_config);
    assert(EGL_FALSE != result);

    result = eglBindAPI(EGL_OPENGL_ES_API);
    assert(EGL_FALSE != result);

    // create an EGL rendering context
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);
    assert(context!=EGL_NO_CONTEXT);

    unsigned int mainWidth = 0;
    unsigned int mainHeight = 0;

    // check desired width and height against the screen bounds
    success = graphics_get_display_size(0 /* LCD */, &mainWidth, &mainHeight);
    assert( success >= 0 );
    
    int width = App.g_dwDeviceWidth;
    int height = App.g_dwDeviceHeight;
    
    // Get XWindow display
    Display *d = XOpenDisplay(NULL);
    int s = DefaultScreen(d);
    
    // Get desktop resolution
    XWindowAttributes xwAttr;
    XGetWindowAttributes( d, RootWindow(d,s), &xwAttr );
    printf( "%d, %d\n", xwAttr.width, xwAttr.height );
    
    int overscanX = (mainWidth - xwAttr.width) / 2;
    int overscanY = (mainHeight - xwAttr.height) / 2;
    
    mainWidth = xwAttr.width;
    mainHeight = xwAttr.height;
    
    float appAspect = width / (float) height;
    float windowAspect = (mainWidth-15) / (float) (mainHeight-80);
    
    if ( appAspect > windowAspect )
    {
        if ( width > mainWidth-15 )
        {
            float ratio = (mainWidth-15) / (float)width;
            width = mainWidth-15;
            height = (int) height*ratio;
        }
    }
    else
    {
        if ( height > mainHeight-80 )
        {
            float ratio = (mainHeight-80) / (float)height;
            height = mainHeight-80;
            width = (int) width*ratio;
        }
    }
    
    //create dummy X window
    Window w = XCreateSimpleWindow(d, RootWindow(d,s), (mainWidth-width) / 2,(mainHeight-height) / 2, width,height, 1, WhitePixel(d,s), BlackPixel(d,s));
    
    XEvent e;
    
    // Set X window message handing
    int eventMask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask |  StructureNotifyMask | FocusChangeMask;
    XSelectInput(d, w, eventMask);
    
    Atom wmDelete = XInternAtom(d, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(d, w, &wmDelete, 1);
    XMapWindow(d, w);
    XStoreName( d,w,"AGK" );
    
    // wait for the window to setup
    timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	double seconds = now.tv_sec + (now.tv_nsec * 1e-9);
	double seconds2;
    int count = 0;
    do
    { 
    	if ( XCheckMaskEvent(d, StructureNotifyMask, &e) ) count++;
    	else usleep( 20*1000 ); // 20 milliseconds
   		clock_gettime(CLOCK_MONOTONIC, &now);
    	seconds2 = now.tv_sec + (now.tv_nsec * 1e-9);
    } while ( count < 2 && seconds2 < seconds+4 );
    
    // get window position for the hardware surface
    int winX, winY;
    Window unused;
    XTranslateCoordinates( d, w, RootWindow(d,s), 0,0, &winX, &winY, &unused );
    winX += overscanX;
    winY += overscanY;
    
    App.g_dwDeviceWidth = width;
    App.g_dwDeviceHeight = height;

    dst_rect.x = winX;
    dst_rect.y = winY;
    dst_rect.width = width;
    dst_rect.height = height;
      
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = width << 16;
    src_rect.height = height << 16; 

    dispman_display = vc_dispmanx_display_open( 0 /* LCD */);
    dispman_update = vc_dispmanx_update_start( 0 );

    VC_DISPMANX_ALPHA_T alpha;
    alpha.flags = (DISPMANX_FLAGS_ALPHA_T) 1;
    alpha.opacity = 255;
    alpha.mask = 0;
         
    dispman_element = vc_dispmanx_element_add ( dispman_update, dispman_display, 0/*layer*/, &dst_rect, 0/*src*/,
      &src_rect, DISPMANX_PROTECTION_NONE, &alpha, 0/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);
    
    nativewindow.element = dispman_element;
    nativewindow.width = width;
    nativewindow.height = height;
    vc_dispmanx_update_submit_sync( dispman_update );
      
    surface = eglCreateWindowSurface( display, config, &nativewindow, NULL );
    assert(surface != EGL_NO_SURFACE);

    // connect the context to the surface
    result = eglMakeCurrent(display, surface, surface, context);
    assert(EGL_FALSE != result);
	
    // initialise graphics API for app
    agk::SetExtraAGKPlayerAssetsMode ( 2 );
		
    // call app begin
    try
    {
        egldata data;
        data.display = display;
        data.surface = surface;
        data.context = context;
        data.XDisplay = d;
        data.XWindow = w;
        agk::InitGL(&data);

        App.Begin();
    }
    catch( ... )
    {
        uString err = agk::GetLastError();
        err.Prepend( "Uncaught exception: \n\n" );
        done = 1;
    }

    float displayTimer = 0;
    int currWidth = width;
    int currHeight = height;
    int focussed = 1;
    while ( !done )
    {
    	// run AGK app loop
	    try
	    {
	        App.Loop();
	    }
	    catch( ... )
	    {
	        uString err = agk::GetLastError();
	        err.Prepend( "Uncaught exception: \n\n" );
	        done = 1;
	    }
        
        while( XCheckMaskEvent(d, eventMask, &e)
                || XCheckTypedEvent(d, ClientMessage, &e) )
        {
		    switch( e.type )
		    {
		        case Expose:
		        {
		        	XSetForeground( d, DefaultGC(d,s), (16<<11) | (32<<5) | 16 );
	            	XDrawString(d,w, DefaultGC(d,s), currWidth/2-75,currHeight/2, "Window not currently active", 27);
	            	XDrawString(d,w, DefaultGC(d,s), currWidth/2-52,currHeight/2+20, "App is still running", 20);
		            break;
		        }
	
		        case ClientMessage:
		        {
		        	if ( e.xclient.data.l[0] == XInternAtom(d, "WM_DELETE_WINDOW", False) ) App.AppQuit();
		        	break;
		        }

				case ConfigureNotify:
				{
					int newX, newY;
					Window unused;
					XTranslateCoordinates( d, w, RootWindow(d,s), 0,0, &newX, &newY, &unused );
					newX += overscanX;
					newY += overscanY;
					
					int surfaceChanged = 0;
					if ( currWidth != e.xconfigure.width || currHeight != e.xconfigure.height )
					{					
						currWidth = e.xconfigure.width;
						currHeight = e.xconfigure.height;
						surfaceChanged = 1;
					}
				
					dst_rect.x = newX;
					dst_rect.y = newY;
					dst_rect.width = e.xconfigure.width;
					dst_rect.height = e.xconfigure.height;
					  
					src_rect.x = 0;
					src_rect.y = 0;
					src_rect.width = e.xconfigure.width << 16;
					src_rect.height = e.xconfigure.height << 16; 
				
					dispman_update = vc_dispmanx_update_start( 0 );
		            		int failure = vc_dispmanx_element_change_attributes( dispman_update, dispman_element, ELEMENT_CHANGE_DEST_RECT | ELEMENT_CHANGE_SRC_RECT, 0, 255, &dst_rect, &src_rect, 0, (DISPMANX_TRANSFORM_T)0 ); 
		            vc_dispmanx_update_submit_sync( dispman_update );
					assert(failure == 0);
		            
		            if ( !surfaceChanged ) break;
		            
		            nativewindow.element = dispman_element;
					nativewindow.width = e.xconfigure.width;
					nativewindow.height = e.xconfigure.height;

					eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
					eglDestroySurface( display, surface );
					
  					surface = eglCreateWindowSurface( display, config, &nativewindow, NULL );
					assert(surface != EGL_NO_SURFACE);

					result = eglMakeCurrent(display, surface, surface, context);
				
					egldata data;
		    		data.display = display;
				    data.surface = surface;
			    	data.context = context;
			    	data.XDisplay = d;
			    	data.XWindow = w;
			    	agk::UpdatePtr( &data );
			    	break;
				}
			
				case FocusOut:
				{
					focussed = 0;
				    displayTimer = 0;
					dispman_update = vc_dispmanx_update_start( 0 );
		        	vc_dispmanx_element_change_attributes( dispman_update, dispman_element, 2, 0, 0, NULL, NULL, 0, (DISPMANX_TRANSFORM_T)0 ); 
		        	vc_dispmanx_update_submit_sync( dispman_update );
		        	
		        	//printf( "Out %d, %d\n", e.xfocus.mode, e.xfocus.detail );
		        	break;
				}
			
				case FocusIn:
				{
					focussed = 1;
					// don't display right away, wait to let other updates process, like position changes
				    displayTimer = 0.1f;
				    /*
					dispman_update = vc_dispmanx_update_start( 0 );
		        	vc_dispmanx_element_change_attributes( dispman_update, dispman_element, 2, 0, 255, NULL, NULL, 0, (DISPMANX_TRANSFORM_T)0 ); 
		        	vc_dispmanx_update_submit_sync( dispman_update );
		        	*/
		        
		        	//printf( "In %d, %d\n", e.xfocus.mode, e.xfocus.detail );
					break;
				}
				
				case ButtonPress:
				{	
					switch( e.xbutton.button )
					{
						case 1: agk::MouseLeftButton( 0, 1 ); break;
						case 2: agk::MouseMiddleButton( 0, 1 ); break;
						case 3: agk::MouseRightButton( 0, 1 ); break;
						case 4: agk::MouseWheel( 0, 3 ); break;
						case 5: agk::MouseWheel( 0, -3 ); break;
					}
					break;
				}
				
				case ButtonRelease:
				{
					switch( e.xbutton.button )
					{
						case 1: agk::MouseLeftButton( 0, 0 ); break;
						case 2: agk::MouseMiddleButton( 0, 0 ); break;
						case 3: agk::MouseRightButton( 0, 0 ); break;
					}
					break;
				}
				
				case KeyPress:
				{
					agk::KeyDown( TranslateKey(e.xkey.keycode) );
					
					if ( e.xkey.keycode ) agk::CharDown( 32 );
					else
					{
						int keysym = XKeycodeToKeysym( d, e.xkey.keycode, (e.xkey.state & ShiftMask) ? 1 : 0 );
						if ( keysym < 255 ) agk::CharDown( keysym );
						//printf("%d\n", keysym);
					}
					break;
				}
				
				case KeyRelease:
				{
					agk::KeyUp( TranslateKey(e.xkey.keycode) );
					break;
				}
		    } // switch(e.type)
		} // while( events to process )
		
		// if the focus in timer has expired then show the display
		if ( displayTimer > 0 )
		{
		    displayTimer -= agk::GetFrameTime();
		    if ( displayTimer < 0 )
		    {
		        displayTimer = 0;
    		    dispman_update = vc_dispmanx_update_start( 0 );
		        vc_dispmanx_element_change_attributes( dispman_update, dispman_element, 2, 0, 255, NULL, NULL, 0, (DISPMANX_TRANSFORM_T)0 ); 
		        vc_dispmanx_update_submit_sync( dispman_update );
		    }
		}
		
		// read mouse coords
		int mouseX, mouseY, rootMouseX, rootMouseY;
		unsigned int mask;
		XQueryPointer( d, w, &unused, &unused, &rootMouseX, &rootMouseY, &mouseX, &mouseY, &mask );
		agk::MouseMove( 0, mouseX, mouseY );
    } // while ( !done )
	
    // close app
    App.End();
    agk::CleanUp();

    XCloseDisplay(d);
	
    // close window system
    eglMakeCurrent( display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
    eglDestroySurface( display, surface );
    eglDestroyContext( display, context );
    eglTerminate( display );
	
    // success    
    return 0;
}
