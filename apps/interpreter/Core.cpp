// Common Includes (for Win32)

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// windows 7 for touch commands
#define _WIN32_WINNT 0x0601

#include "AGK.h"
#include <windows.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library
#include "direct.h"

// App specific include for Core (so core can be distributed between apps without editing inside it)
#include "interpreter.h"

#ifdef AGK_STEAM
	#include "steam/steam_api.h"
	#include "Shlwapi.h"
#endif

#define WM_SOUND_EVENT 0x8001
#define WM_VIDEO_EVENT 0x8002

// comment out to revert to single mouse
//#define MULTI_MOUSE

// Namespace
using namespace AGK;

// Globals for core
HANDLE hDebugConsoleOut = NULL;
HWND g_hWnd = NULL;
bool g_bAGKInitialised = false;
bool bExitLoop = false;

//
// Windows Application STUB requires entry function, create window and message pump
//

#define AGK_MOUSE_LEFTDOWN		0x00000001
#define AGK_MOUSE_RIGHTDOWN		0x00000002
#define AGK_MOUSE_MIDDLEDOWN	0x00000004

void PlatformAppQuit()
{
	PostQuitMessage(0);
}

class cMouseDevice
{
	protected:

	public:
		cMouseDevice() { ID = 0; hDevice = 0; x = 0; y = 0; pNext = 0; sprite = 0; m_bFlags = 0; }
		~cMouseDevice();

		UINT ID;
		HANDLE hDevice;
		int x;
		int y;
		UINT m_bFlags;
		cMouseDevice *pNext;
		UINT sprite;
};

cMouseDevice *pMouseList = 0;

void Output( const char *szMsg )
{
	if ( !hDebugConsoleOut ) 
	{
		AllocConsole ( );
		HWND hConsole = GetConsoleWindow();
		MoveWindow( hConsole, 32, 500, 800, 350, TRUE );
		hDebugConsoleOut = GetStdHandle ( STD_OUTPUT_HANDLE );
	}
	
	DWORD dwWritten = 0;
	WriteConsole ( hDebugConsoleOut, "> ", 2, &dwWritten, NULL );
	WriteConsole ( hDebugConsoleOut, szMsg, (DWORD) strlen( szMsg ), &dwWritten, NULL );
	WriteConsole ( hDebugConsoleOut, "\n", 1, &dwWritten, NULL );
}

void AdditionalKeyDown( unsigned int key )
{
	switch( key )
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

		case 16: // shift
		{
			if ( GetAsyncKeyState( VK_LSHIFT ) & 0x8000 ) agk::KeyDown( 257 );
			if ( GetAsyncKeyState( VK_RSHIFT ) & 0x8000 ) agk::KeyDown( 258 );
			break;
		}

		case 17: // ctrl
		{
			if ( GetAsyncKeyState( VK_LCONTROL ) & 0x8000 ) agk::KeyDown( 259 );
			if ( GetAsyncKeyState( VK_RCONTROL ) & 0x8000 ) agk::KeyDown( 260 );
			break;
		}

		case 18: // alt
		{
			if ( GetAsyncKeyState( VK_LMENU ) & 0x8000 ) agk::KeyDown( 261 );
			if ( GetAsyncKeyState( VK_RMENU ) & 0x8000 ) agk::KeyDown( 262 );
			break;
		}
	}
}

void AdditionalKeyUp( unsigned int key )
{
	switch( key )
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

		// note that shift keyup is not always triggered if both keys are down, so the lib does its own check
		case 16: // shift
		{
			if ( (GetAsyncKeyState( VK_LSHIFT ) & 0x8000) == 0 ) agk::KeyUp( 257 );
			if ( (GetAsyncKeyState( VK_RSHIFT ) & 0x8000) == 0 ) agk::KeyUp( 258 );
			break;
		}

		case 17: // ctrl
		{
			if ( (GetAsyncKeyState( VK_LCONTROL ) & 0x8000) == 0 ) agk::KeyUp( 259 );
			if ( (GetAsyncKeyState( VK_RCONTROL ) & 0x8000) == 0 ) agk::KeyUp( 260 );
			break;
		}

		case 18: // alt
		{
			if ( (GetAsyncKeyState( VK_LMENU ) & 0x8000) == 0 ) agk::KeyUp( 261 );
			if ( (GetAsyncKeyState( VK_RMENU ) & 0x8000) == 0 ) agk::KeyUp( 262 );
			break;
		}
	}
}

bool g_bShouldBeTopMost = false;

// delay the loading of these functions so Windows XP and Vista don't complain
HMODULE g_hUser32 = 0;
int (__stdcall *CloseTouchInputHandleDelayed)( HTOUCHINPUT ) = 0;
int (__stdcall *GetTouchInputInfoDelayed)( HTOUCHINPUT, unsigned int, PTOUCHINPUT, int ) = 0;
BOOL (__stdcall *RegisterTouchWindowDelay)(HWND, ULONG) = 0;

void LoadDelayedFunctions()
{
	// only checks once
	static int first = 1;
	if ( first == 1 )
	{
		first = 0;
		g_hUser32 = LoadLibraryA( "USER32.dll" );
		if ( !g_hUser32 ) return;
		CloseTouchInputHandleDelayed = (int(__stdcall *)(HTOUCHINPUT)) GetProcAddress( g_hUser32, "CloseTouchInputHandle" );
		GetTouchInputInfoDelayed = (int(__stdcall *)(HTOUCHINPUT, unsigned int, PTOUCHINPUT, int)) GetProcAddress( g_hUser32, "GetTouchInputInfo" );
		RegisterTouchWindowDelay = (BOOL(__stdcall *)(HWND, ULONG)) GetProcAddress( g_hUser32, "RegisterTouchWindow" );
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool bLeftDown = false;

	switch (message)
	{
#ifdef WM_TOUCH
		// only works on Windows 7 or above
		case WM_TOUCH:
		{
			LoadDelayedFunctions();

			if ( CloseTouchInputHandleDelayed )
			{
				// windows based touch device
				UINT cInputs = LOWORD(wParam);
				if ( cInputs > 0 )
				{
					PTOUCHINPUT pInputs = new TOUCHINPUT[cInputs];
					if (GetTouchInputInfoDelayed((HTOUCHINPUT)lParam, cInputs, pInputs, sizeof(TOUCHINPUT)))
					{
						// process pInputs
						for ( UINT i = 0; i < cInputs; i++ )
						{
							pInputs[i].x /= 100;
							pInputs[i].y /= 100;
							POINT p; p.x = pInputs[i].x; p.y = pInputs[i].y;
							ScreenToClient( hWnd, &p );
							
							if ( pInputs[i].dwFlags & TOUCHEVENTF_DOWN ) agk::TouchPressed( pInputs[i].dwID, p.x, p.y );
							if ( pInputs[i].dwFlags & TOUCHEVENTF_MOVE ) agk::TouchMoved( pInputs[i].dwID, p.x, p.y );
							if ( pInputs[i].dwFlags & TOUCHEVENTF_UP ) agk::TouchReleased( pInputs[i].dwID, p.x, p.y );
						}
						
						CloseTouchInputHandleDelayed((HTOUCHINPUT)lParam);
					}
					
					delete [] pInputs;
				}
			}

			break;
		}
		
#endif

#ifdef MULTI_MOUSE
		case WM_INPUT:
		{			
			if ( !bCapturePointer ) break;
			// raw input from registered devices (in this case mice)
			UINT dwSize;
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
			LPBYTE lpb = new BYTE[dwSize];
			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
			RAWINPUT* raw = (RAWINPUT*)lpb;

			// find mouse device
			int count = 0;
			cMouseDevice *pMouse = pMouseList;
			while ( pMouse )
			{
				count++;
				if ( pMouse->hDevice == raw->header.hDevice ) break;
				pMouse = pMouse->pNext;
			}

			// is this a new mouse device?
			if ( !pMouse )
			{
				pMouse = new cMouseDevice();
				pMouse->hDevice = raw->header.hDevice;
				pMouse->pNext = pMouseList;
				pMouse->ID = count+1;
				pMouse->x = lastX;
				pMouse->y = lastY;
				pMouse->sprite = agk::CreateSprite( 99 );
				agk::SetSpriteSize( pMouse->sprite, 4,-1 );
				agk::SetSpriteTransparency( pMouse->sprite, 2 );
				switch( count )
				{
					case 0: agk::SetSpriteColor( pMouse->sprite, 255,0,0,255 ); break;
					case 1: agk::SetSpriteColor( pMouse->sprite, 0,255,0,255 ); break;
					case 2: agk::SetSpriteColor( pMouse->sprite, 0,0,255,255 ); break;
					case 3: agk::SetSpriteColor( pMouse->sprite, 255,255,0,255 ); break;
				}
				pMouseList = pMouse;
			}

			// relative or absolute positioning?
			if ( raw->data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE )
			{
				pMouse->x = raw->data.mouse.lLastX;
				pMouse->y = raw->data.mouse.lLastY;
			}
			else
			{
				pMouse->x += raw->data.mouse.lLastX;
				pMouse->y += raw->data.mouse.lLastY;
			}

			// check buttons
			if ( raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN )
			{
				if ( (pMouse->m_bFlags & AGK_MOUSE_LEFTDOWN) == 0 )
				{
					agk::TouchPressed( pMouse->ID, pMouse->x, pMouse->y );
				}

				pMouse->m_bFlags |= AGK_MOUSE_LEFTDOWN;
			}

			if ( raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP )
			{
				if ( (pMouse->m_bFlags & AGK_MOUSE_LEFTDOWN) != 0 )
				{
					agk::TouchReleased( pMouse->ID, pMouse->x, pMouse->y );
				}

				pMouse->m_bFlags &= ~AGK_MOUSE_LEFTDOWN;
			}

			// if holding left button send drag event
			if ( (pMouse->m_bFlags & AGK_MOUSE_LEFTDOWN) != 0 )
			{
				if ( raw->data.mouse.lLastX != 0 && raw->data.mouse.lLastY != 0 )
				{
					agk::TouchMoved( pMouse->ID, pMouse->x, pMouse->y );
				}
			}

			if ( pMouse->x > DEVICE_WIDTH ) pMouse->x = DEVICE_WIDTH;
			if ( pMouse->y > DEVICE_HEIGHT ) pMouse->y = DEVICE_HEIGHT;
			if ( pMouse->x < 0 ) pMouse->x = 0;
			if ( pMouse->y < 0 ) pMouse->y = 0;

			agk::SetSpritePosition( pMouse->sprite, agk::DeviceToScreenX(pMouse->x), agk::DeviceToScreenY(pMouse->y) );

			//char str[ 256 ];
			//sprintf( str, "X: %d, Y: %d", pMouse->x, pMouse->y );
			//Output( str );
			if ( bCapturePointer )
			{
				RECT rect;
				::GetWindowRect( hWnd, &rect );
				int centerX = (rect.left + rect.right) / 2;
				int centerY = (rect.top + rect.bottom) / 2;
				SetCursorPos( centerX,centerY );
			}
			break;
		}
#endif

		case WM_LBUTTONDOWN:
		{
			bLeftDown = true;
			int x = lParam & 0x0000ffff;
			int y = (lParam >> 16) & 0x0000ffff;
			if ( x > 32767 ) x = x - 65536;
			if ( y > 32767 ) y = y - 65536;

			// detect pen touch
			//if ( (GetMessageExtraInfo() & 0xFFFFFF00) != 0xFF515700 ) 
				agk::MouseLeftButton( 0, 1 );

#ifdef MULTI_MOUSE
			if ( !bCapturePointer )
			{
				bCapturePointer = true;
				ShowCursor( FALSE );
				lastX = x;
				lastY = y;
			}
#else
			SetCapture(hWnd);
			//agk::TouchPressed( 1, x,y );
#endif
			break;
		}

		case WM_LBUTTONUP:
		{
			bLeftDown = false;
			int x = lParam & 0x0000ffff;
			int y = (lParam >> 16) & 0x0000ffff;
			if ( x > 32767 ) x = x - 65536;
			if ( y > 32767 ) y = y - 65536;

			// detect pen touch
			//if ( (GetMessageExtraInfo() & 0xFFFFFF00) != 0xFF515700 ) 
				agk::MouseLeftButton( 0, 0 );

#ifndef MULTI_MOUSE
			ReleaseCapture();
			//agk::TouchReleased( 1, x,y );
#endif
			break;
		}

		case WM_RBUTTONDOWN:
		{
			agk::MouseRightButton( 0, 1 );

#ifdef MULTI_MOUSE
			if ( bCapturePointer ) // Alt Gr
			{
				bCapturePointer = false;
				ShowCursor( TRUE );
			}
#else
			SetCapture(hWnd);
#endif
			break;
		}

		case WM_RBUTTONUP:
		{
			agk::MouseRightButton( 0, 0 );
			ReleaseCapture();
			break;
		}

		case WM_MBUTTONDOWN:
		{
			agk::MouseMiddleButton( 0, 1 );
			SetCapture(hWnd);
			break;
		}

		case WM_MBUTTONUP:
		{
			agk::MouseMiddleButton( 0, 0 );
			ReleaseCapture();
			break;
		}

		case WM_XBUTTONDOWN:
		{
			if ( wParam & 0x00010000 ) agk::MouseFourthButton( 0, 1 );
			else if ( wParam & 0x00020000 ) agk::MouseFifthButton( 0, 1 );
			else return DefWindowProc(hWnd, message, wParam, lParam);

			SetCapture(hWnd);
			return 1;
			break;
		}

		case WM_XBUTTONUP:
		{
			if ( wParam & 0x00010000 ) agk::MouseFourthButton( 0, 0 );
			else if ( wParam & 0x00020000 ) agk::MouseFifthButton( 0, 0 );
			else return DefWindowProc(hWnd, message, wParam, lParam);

			ReleaseCapture();
			return 1;
			break;
		}

		case WM_MOUSEWHEEL:
		{
			int w = (wParam >> 16) & 0x0000ffff;
			if ( w > 32767 ) w = w - 65536;

			float fW = w / 40.0f;

			agk::MouseWheel( 0, fW );
			break;
		}

		case WM_MOUSEMOVE:
		{
			int x = lParam & 0x0000ffff;
			int y = (lParam >> 16) & 0x0000ffff;
			if ( x > 32767 ) x = x - 65536;
			if ( y > 32767 ) y = y - 65536;

			agk::MouseMove( 0, x, y );

			if ( bLeftDown )
			{
#ifndef MULTI_MOUSE
				//agk::TouchMoved( 1, x,y );
#endif
			}
			break;
		}

		case WM_SIZE:
		{
			// if maximizing then set window to topmost to prevent it overlapping the start button when "use small icons" is ticked
			// which seems to cause image stutter
			g_bShouldBeTopMost = (wParam == SIZE_MAXIMIZED);
			if ( g_bShouldBeTopMost ) ::SetWindowPos( hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE );
			else ::SetWindowPos( hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE );

			agk::UpdateDeviceSize();
			agk::WindowMoved();
			break;
		}
		
		case WM_SIZING:
		{
			if ( g_bAGKInitialised )
			{
				try
				{
					if (!agk::IsCapturingImage())
					{
						App.Loop();
					}
					else
					{
						agk::Sleep(10);
					}
				}
				catch (...)
				{
					uString err = agk::GetLastError();
					err.Prepend("(WM_SIZING) Uncaught exception: \n\n");
					MessageBoxA(NULL, err.GetStr(), "Error", 0);
					bExitLoop = true;
				}
			}
			break;
		}
	
		case WM_NCHITTEST:
		{
			LRESULT result = DefWindowProc(hWnd, message, wParam, lParam);

			if ( agk::CanResize() ) return result;
			else
			{
				// filter out resize options
				if ( result == HTBOTTOM || result == HTBOTTOMLEFT || result == HTBOTTOMRIGHT 
				  || result == HTTOP || result == HTTOPLEFT || result == HTTOPRIGHT
				  || result == HTLEFT || result == HTRIGHT
				  || result == HTSIZE )
				{
					return HTBORDER;
				}
				else 
				{
					return result;
				}
			}
		}

		case WM_WINDOWPOSCHANGED:
		case WM_MOVE:
		{
			if ( g_bAGKInitialised )
			{
				agk::WindowMoved();

				// save window position for next opening
				RECT rc;
				GetWindowRect( hWnd, &rc );
				cFile posFile;
				posFile.OpenToWrite( "/window_position.dat" );
				posFile.WriteInteger( rc.left );
				posFile.WriteInteger( rc.right );
				posFile.WriteInteger( rc.top );
				posFile.WriteInteger( rc.bottom );
				posFile.Close();
			}

			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		case WM_ACTIVATE:
		{
			// if being made inactive and currently we are topmost temporarily remove topmost until we are active again.
			if ( LOWORD(wParam) != WA_INACTIVE ) 
			{
				if ( g_bShouldBeTopMost ) ::SetWindowPos( hWnd, HWND_TOPMOST, 0,0,0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE );
				agk::Resumed();
			}
			else
			{
				agk::MouseLeftButton( 0, 0 );
				agk::MouseRightButton( 0, 0 );
				agk::MouseMiddleButton( 0, 0 );
				if ( g_bShouldBeTopMost ) ::SetWindowPos( hWnd, HWND_NOTOPMOST, 0,0,0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSENDCHANGING | SWP_NOSIZE );
				agk::Paused();
				agk::KeyUp( 18 ); // Alt key up in case Alt-Tab was used
			}
			agk::WindowMoved();
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		case WM_NCACTIVATE:
		{
			if ( wParam ) agk::WindowMoved();
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		case WM_SETFOCUS:
		{
			agk::WindowMoved();
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		case WM_KEYDOWN:
		{
#ifdef MULTI_MOUSE
			if ( wParam == 17 && bCapturePointer ) // Alt Gr
			{
				bCapturePointer = false;
				ShowCursor( TRUE );
			}
#endif
			AdditionalKeyDown( wParam );
			if ( wParam > 0 ) agk::KeyDown( wParam );			
			
			/*
			uString sKey;
			sKey.Format( "Key: %d", wParam );
			agk::Warning( sKey );
			*/

			break;
		}

		case WM_KEYUP:
		{
			AdditionalKeyUp( wParam );
			if ( wParam > 0 ) agk::KeyUp( wParam );
			break;
		}

		case WM_CHAR:
		{
			agk::CharDown( wParam );
			break;
		}
		
		case WM_UNICHAR:
		{
			if ( wParam == UNICODE_NOCHAR ) return TRUE;
			agk::CharDown( wParam );
			break;
		}

		case WM_SYSKEYDOWN:
		{
			AdditionalKeyDown( wParam );
			if ( wParam > 0 ) agk::KeyDown( wParam );	
			break;
		}

		case WM_SYSKEYUP:
		{
			AdditionalKeyUp( wParam );
			if ( wParam > 0 ) agk::KeyUp( wParam );
			break;
		}

		case WM_SYSCOMMAND:
		{
			if ( wParam == SC_SIZE && !agk::CanResize() ) return 0;
			else return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}

		case WM_SOUND_EVENT:
		{
			agk::HandleMusicEvents( (void*) lParam );
			break;
		}

		case WM_VIDEO_EVENT:
		{
			agk::HandleVideoEvents();
			break;
		}
			
		case WM_COMMAND:
		{
			if ( (wParam & 0xffff) == 124 )
			{
				// capture
				agk::CaptureImage();
				agk::CancelCapture();
			}
			else if ( (wParam & 0xffff) == 125 )
			{
				// cancel
				agk::CancelCapture();
			}
			break;
		}

		case WM_CLOSE:
		{
			//PostQuitMessage(0);
			App.AppQuit();
			break;
		}

		case WM_ERASEBKGND:
		{
			break;
		}

		case WM_PAINT:
		{
			ValidateRect( hWnd, NULL );
			break;
		}

		case 0x031E: //WM_DWMCOMPOSITIONCHANGED
		{
			agk::CompositionChanged();
			break;
		}

		default:
		{
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	return 0;
}

HWND CreateWin32Window( HINSTANCE hInstance, int width, int height, uString &szTitle, int x, int y, bool fullscreen )
{
	//MessageBox( NULL, "Wait", "Info", 0 );

	HANDLE hIcon = LoadImageA(NULL, "icon.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);

	// register the class
	WNDCLASSEX wcex;

	// hardcoded resource IDs for icons
	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= hIcon ? (HICON)hIcon : LoadIcon(hInstance, MAKEINTRESOURCE(104)); 
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"OPENGLWINDOW";
	wcex.hIconSm		= NULL;
	RegisterClassEx(&wcex);

	// check the given values against the desktop resolution and center if requested (x=-1 or y=-1)
	RECT rc;
	GetWindowRect(GetDesktopWindow(), &rc);
	
	if ( width < 1 ) width = 1;
	if ( height < 1 ) height = 1;

	float appAspect = width / (float) height;
	float windowAspect = (rc.right-rc.left-15) / (float) (rc.bottom-rc.top-80);

	if ( appAspect > windowAspect )
	{
		if ( width > rc.right-rc.left-15 )
		{
			float ratio = (rc.right-rc.left-15) / (float)width;
			width = rc.right-rc.left-15;
			height = (int) (height*ratio);
		}
	}
	else
	{
		if ( height > rc.bottom-rc.top-80 )
		{
			float ratio = (rc.bottom-rc.top-80) / (float)height;
			height = rc.bottom-rc.top-80;
			width = (int) (width*ratio);
		}
	}
	
	if ( x == -1 ) x = ((rc.right-rc.left)-width)/2 + rc.left;
	if ( y == -1 ) y = ((rc.bottom-rc.top-50)-height)/2 + rc.top;
	if ( x < 0 ) x = 0;
	if ( y < 0 ) y = 0;
	if ( x+width > rc.right ) x = rc.right-width;
	if ( y+height > rc.bottom ) y = rc.bottom-height;

	// check if window has a previous position
	
	int loaded = 0;
	if ( agk::GetFileExists( "/window_position.dat" ) )
	{
		cFile posFile;
		posFile.OpenToRead( "/window_position.dat" );

		RECT rc2;
		rc2.left = posFile.ReadInteger();
		rc2.right = posFile.ReadInteger();
		rc2.top = posFile.ReadInteger();
		rc2.bottom = posFile.ReadInteger();
		posFile.Close();

		x = rc2.left;
		y = rc2.top;
		width = rc2.right - rc2.left;
		height = rc2.bottom - rc2.top;

		int vLeft = GetSystemMetrics( SM_XVIRTUALSCREEN );
		int vTop = GetSystemMetrics( SM_YVIRTUALSCREEN );
		int vRight = GetSystemMetrics( SM_CXVIRTUALSCREEN ) + vLeft;
		int vBottom = GetSystemMetrics( SM_CYVIRTUALSCREEN ) + vTop;

		if ( x+width > vRight ) x = vRight - width;
		if ( y+height > vBottom ) x = vBottom - height;

		if ( x < vLeft ) x = vLeft;
		if ( y < vTop ) y = vTop;

		loaded = 1;
	}

	if ( fullscreen )												
    {
		width = rc.right-rc.left;
		height = rc.bottom-rc.top;
		x = 0;
		y = 0;
		loaded = 0;

		/*
    	DEVMODE dmScreenSettings;								// Device Mode
    	memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
    	dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
    	dmScreenSettings.dmBitsPerPel	= 32;					// Selected Bits Per Pixel
    	dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    	// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
    	if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
    	{
    		fullscreen = false;
    	}
		*/
    }

	DWORD dwExStyle;
	DWORD dwStyle;

	if ( fullscreen )
    {
    	dwExStyle = WS_EX_APPWINDOW;							
    	dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	
    }
    else
    {
    	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	
    	dwStyle = WS_OVERLAPPEDWINDOW;// & (~WS_SIZEBOX);	// removing WS_SIZEBOX reduces the size of the title bar, which then increases in size itself on the first window resize
    }

	// adjust the window and make it bigger for borders, window title, etc
	RECT WindowRect;							
	WindowRect.left=(long)0;					
	WindowRect.right=(long)width;				
	WindowRect.top=(long)0;						
	WindowRect.bottom=(long)height;				
	if ( !loaded ) AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

	// initialise window
	HWND hWnd = CreateWindow(L"OPENGLWINDOW", L"AGK", dwStyle, x, y, 
			WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top, NULL, NULL, hInstance, NULL);

	// terminate if failed to create window
	if (!hWnd) return FALSE;
	g_hWnd = hWnd;
	
	// show window
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

#ifdef MULTI_MOUSE
	// Register that we want raw input from mice
	RAWINPUTDEVICE pDevices;
	pDevices.usUsage = 2;
	pDevices.usUsagePage = 1;
	pDevices.hwndTarget = hWnd;
	pDevices.dwFlags = 0;
	RegisterRawInputDevices( &pDevices, 1, sizeof(RAWINPUTDEVICE) );
#endif

	LoadDelayedFunctions();
	if ( RegisterTouchWindowDelay ) RegisterTouchWindowDelay(hWnd,TWF_WANTPALM);

	// success
	return hWnd;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
#ifdef AGK_STEAM
	// SteamAPI_Init() will produce a message box if steam_api.dll is not found, so check for it first
	if ( PathFileExistsA( "steam_api.dll" ) )
	{
		SteamAPI_Init();
	}
#endif

	agk::InitFilePaths();

	// load company name from bytecode file to make write folder path more unique, used in InitGL
	if ( agk::GetFileExists("media/bytecode.byc") && App.m_sProgram.LoadBytecodeHeader("media/bytecode.byc") )
	{
		agk::SetCompanyName( App.m_sProgram.GetCompanyName() );
		agk::InitFilePaths();
	}
	
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

	// create a win32 window for the app
	uString szTitle( App.g_pWindowTitle );
	HWND hWnd = CreateWin32Window( hInstance, App.g_dwDeviceWidth, App.g_dwDeviceHeight, szTitle, -1, -1, App.g_dwFullScreen==1 );

	agk::SetExtraAGKPlayerAssetsMode ( 2 ); // 0-no assets, 1-minimum assets, 2-extra AGK Player assets

	// call app begin
	try
	{
		// initialise graphics API (win32 openGL) for app
		agk::InitGL( (void*) hWnd );
		g_bAGKInitialised = true;

		App.Begin();
	}
	catch(...)
	{
		uString err; agk::GetLastError(err);
		err.Prepend( "Uncaught exception: \n\n" );
		MessageBoxA( NULL, err.GetStr(), "Error", 0 );
		bExitLoop = true;
	}

	int firstRun = 1;

	// message pump
	MSG msg;
	while ( !bExitLoop )
	{
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) ) 
		{	
			if ( msg.message == WM_QUIT ) bExitLoop = true;
			// message 799 is sent just after a change to fullscreen mode and for some reason sets it back to windowed mode.
			// update: but it doesn't seem to cause a problem after reinstating the WS_SIZEBOX style
			//if ( msg.message == 799 ) continue;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			// call app each frame
			try
			{
				if ( !agk::IsCapturingImage() ) 
				{
					App.Loop();
				}
				else
				{
					agk::Sleep( 10 );
				}
			}
			catch(...)
			{
				uString err; agk::GetLastError(err);
				err.Prepend( "Uncaught exception: \n\n" );
				MessageBoxA( NULL, err.GetStr(), "Error", 0 );
				bExitLoop = true;
			}
		}
	}
	
	// call app end 
	App.End();

	if ( g_hUser32 ) FreeLibrary( g_hUser32 );
	g_hUser32 = 0;

	agk::CleanUp();
	
	// finished
	return 0;
}

