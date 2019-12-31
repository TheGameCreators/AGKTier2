#include "agk.h"
#include "OpenGL2.h"
#include "zlib.h"

bool IsExtensionSupported(const char *extension)
{
	const size_t extlen = strlen(extension);
	const char *supported = NULL;
 
	supported = (char*)glGetString(GL_EXTENSIONS);
 
	// If That Failed Must Be No Extensions Supported
	if (supported == NULL) return false;
 
	// Begin Examination At Start Of String, Increment By 1 On False Match
	for (const char* p = supported; ; p++)
	{
		// Advance p Up To The Next Possible Match
		p = strstr(p, extension);
		if (p == NULL) return false;                       // No Match
 
		// Make Sure That Match Is At The Start Of The String Or That
		// The Previous Char Is A Space, Or Else We Could Accidentally
		// Match "wglFunkywglExtension" With "wglExtension"
 
		// Also, Make Sure That The Following Character Is Space Or NULL
		// Or Else "wglExtensionTwo" Might Match "wglExtension"
		if ((p==supported || p[-1]==' ') && (p[extlen]=='\0' || p[extlen]==' ')) return true; // Match
	}
}

#if defined(AGK_WINDOWS) || defined(AGK_LINUX)
	// OpenGL extensions
	PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebuffer = 0;
	PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffers = 0;
	PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2D = 0;
	PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbuffer = 0;
	PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatus = 0;
	PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffers = 0;
	PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbuffer = 0;
	PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffers = 0;
	PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffers = 0;
	PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorage = 0;

	PFNGLPOINTPARAMETERFARBPROC glPointParameterfARB = 0;
	PFNGLPOINTPARAMETERFVARBPROC glPointParameterfvARB = 0;

	// shaders
	PFNGLCREATESHADERPROC glCreateShader = 0;
	PFNGLSHADERSOURCEPROC glShaderSource = 0;
	PFNGLCOMPILESHADERPROC glCompileShader = 0;
	PFNGLGETSHADERIVPROC glGetShaderiv = 0;
	PFNGLDELETESHADERPROC glDeleteShader = 0;
	PFNGLATTACHSHADERPROC glAttachShader = 0;
	PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation = 0;
	PFNGLLINKPROGRAMPROC glLinkProgram = 0;
	PFNGLGETPROGRAMIVPROC glGetProgramiv = 0;
	PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = 0;
	PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = 0;
	PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = 0;
	PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv = 0;
	PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
	PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = 0;
	PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib = 0;
	PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = 0;
	PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
	PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;
	PFNGLUSEPROGRAMPROC glUseProgram = 0;

	// shader uniforms
	PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv = 0;
	PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv = 0;
	PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = 0;
	PFNGLUNIFORM1IPROC glUniform1i = 0;
	PFNGLUNIFORM1FPROC glUniform1f = 0;
	PFNGLUNIFORM2FPROC glUniform2f = 0;
	PFNGLUNIFORM3FPROC glUniform3f = 0;
	PFNGLUNIFORM4FPROC glUniform4f = 0;
	PFNGLUNIFORM1FVPROC glUniform1fv = 0;
	PFNGLUNIFORM2FVPROC glUniform2fv = 0;
	PFNGLUNIFORM3FVPROC glUniform3fv = 0;
	PFNGLUNIFORM4FVPROC glUniform4fv = 0;
	PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = 0;

	// texture
#ifndef AGK_LINUX
	PFNGLACTIVETEXTUREPROC glActiveTexture = 0;
#endif
	PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmap = 0;

	// vertex buffer objects
	PFNGLGENBUFFERSPROC glGenBuffers = 0;
	PFNGLBINDBUFFERPROC glBindBuffer = 0;
	PFNGLBUFFERDATAPROC glBufferData = 0;
	PFNGLDELETEBUFFERSPROC glDeleteBuffers = 0;

	PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate = 0;
#endif

#ifdef AGK_WINDOWS
	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
	PFNWGLCreateContextARB wglCreateContextAttribsARB = 0;

	bool LoadRendererExtensions()
	{
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

		// framebuffer/renderbuffer objects
		glBindFramebuffer = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress( "glBindFramebufferEXT" );
		if( !glBindFramebuffer ) agk::Message( "Failed to get OpenGL extension glBindFramebufferEXT" );

		glGenFramebuffers = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress( "glGenFramebuffersEXT" );
		if( !glGenFramebuffers ) agk::Message( "Failed to get OpenGL extension glGenFramebuffersEXT" );

		glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress( "glFramebufferTexture2DEXT" );
		if( !glFramebufferTexture2D ) agk::Message( "Failed to get OpenGL extension glFramebufferTexture2DEXT" );

		glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress( "glFramebufferRenderbufferEXT" );
		if( !glFramebufferRenderbuffer ) agk::Message( "Failed to get OpenGL extension glFramebufferRenderbufferEXT" );

		glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress( "glCheckFramebufferStatusEXT" );
		if( !glCheckFramebufferStatus ) agk::Message( "Failed to get OpenGL extension glCheckFramebufferStatusEXT" );

		glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress( "glDeleteFramebuffersEXT" );
		if( !glDeleteFramebuffers ) agk::Message( "Failed to get OpenGL extension glDeleteFramebuffersEXT" );

		glBindRenderbuffer = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress( "glBindRenderbufferEXT" );
		if( !glBindRenderbuffer ) agk::Message( "Failed to get OpenGL extension glBindRenderbufferEXT" );

		glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress( "glDeleteRenderbuffersEXT" );
		if( !glDeleteRenderbuffers ) agk::Message( "Failed to get OpenGL extension glDeleteRenderbuffersEXT" );

		glGenRenderbuffers = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress( "glGenRenderbuffersEXT" );
		if( !glGenRenderbuffers ) agk::Message( "Failed to get OpenGL extension glGenRenderbuffersEXT" );

		glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress( "glRenderbufferStorageEXT" );
		if( !glRenderbufferStorage ) agk::Message( "Failed to get OpenGL extension glRenderbufferStorageEXT" );

		// mipmaps (OpenGL 3.0+ only), if missing don't error just fall back to GL_GENERATE_MIPMAP
		glGenerateMipmap = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress( "glGenerateMipmap" );
		if ( !glGenerateMipmap ) glGenerateMipmap = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress( "glGenerateMipmapEXT" );
		//if ( !glGenerateMipmap ) agk::Message( "Failed to load OpenGL extension glGenerateMipmap" );

		// shader functions
		glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress( "glCreateShader" );
		if ( !glCreateShader ) { agk::Message( "Failed to load OpenGL extension glCreateShader" ); return false; }

		glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress( "glShaderSourceARB" );
		if ( !glShaderSource ) { agk::Message( "Failed to load OpenGL extension glShaderSource" ); return false; }

		glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress( "glCompileShaderARB" );
		if ( !glCompileShader ) { agk::Message( "Failed to load OpenGL extension glCompileShader" ); return false; }

		glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress( "glGetShaderiv" );
		if ( !glGetShaderiv ) { agk::Message( "Failed to load OpenGL extension glGetShaderiv" ); return false; }

		glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress( "glDeleteShader" );
		if ( !glDeleteShader ) { agk::Message( "Failed to load OpenGL extension glDeleteShader" ); return false; }

		glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress( "glAttachShader" );
		if ( !glAttachShader ) { agk::Message( "Failed to load OpenGL extension glAttachShader" ); return false; }

		glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress( "glBindAttribLocation" );
		if ( !glBindAttribLocation ) { agk::Message( "Failed to load OpenGL extension glBindAttribLocation" ); return false; }

		glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress( "glLinkProgramARB" );
		if ( !glLinkProgram ) { agk::Message( "Failed to load OpenGL extension glLinkProgram" ); return false; }

		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress( "glGetProgramiv" );
		if ( !glGetProgramiv ) { agk::Message( "Failed to load OpenGL extension glGetProgramiv" ); return false; }

		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress( "glGetUniformLocation" );
		if ( !glGetUniformLocation ) { agk::Message( "Failed to load OpenGL extension glGetUniformLocation" ); return false; }

		glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress( "glDisableVertexAttribArray" );
		if ( !glDisableVertexAttribArray ) { agk::Message( "Failed to load OpenGL extension glDisableVertexAttribArray" ); return false; }

		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress( "glEnableVertexAttribArray" );
		if ( !glEnableVertexAttribArray ) { agk::Message( "Failed to load OpenGL extension glEnableVertexAttribArray" ); return false; }

		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress( "glVertexAttribPointer" );
		if ( !glVertexAttribPointer ) { agk::Message( "Failed to load OpenGL extension glVertexAttribPointer" ); return false; }

		glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)wglGetProcAddress( "glVertexAttrib4fv" );
		if ( !glVertexAttrib4fv ) { agk::Message( "Failed to load OpenGL extension glVertexAttrib4fv" ); return false; }

		glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress( "glCreateProgram" );
		if ( !glCreateProgram ) { agk::Message( "Failed to load OpenGL extension glCreateProgram" ); return false; }

		glDeleteProgram = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress( "glDeleteProgram" );
		if ( !glDeleteProgram ) { agk::Message( "Failed to load OpenGL extension glDeleteProgram" ); return false; }

		glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress( "glUseProgram" );
		if ( !glUseProgram ) { agk::Message( "Failed to load OpenGL extension glUseProgram" ); return false; }

		glUniformMatrix2fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress( "glUniformMatrix2fv" );
		if ( !glUniformMatrix2fv ) { agk::Message( "Failed to load OpenGL extension glUniformMatrix2fv" ); return false; }

		glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)wglGetProcAddress( "glUniformMatrix3fv" );
		if ( !glUniformMatrix3fv ) { agk::Message( "Failed to load OpenGL extension glUniformMatrix3fv" ); return false; }

		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress( "glUniformMatrix4fv" );
		if ( !glUniformMatrix4fv ) { agk::Message( "Failed to load OpenGL extension glUniformMatrix4fv" ); return false; }

		glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress( "glUniform1i" );
		if ( !glUniform1i ) { agk::Message( "Failed to load OpenGL extension glUniform1i" ); return false; }

		glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress( "glUniform1f" );
		if ( !glUniform1f ) { agk::Message( "Failed to load OpenGL extension glUniform1f" ); return false; }

		glUniform2f = (PFNGLUNIFORM2FPROC)wglGetProcAddress( "glUniform2f" );
		if ( !glUniform2f ) { agk::Message( "Failed to load OpenGL extension glUniform2f" ); return false; }

		glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress( "glUniform3f" );
		if ( !glUniform3f ) { agk::Message( "Failed to load OpenGL extension glUniform3f" ); return false; }

		glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress( "glUniform4f" );
		if ( !glUniform4f ) { agk::Message( "Failed to load OpenGL extension glUniform4f" ); return false; }

		glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress( "glUniform1fv" );
		if ( !glUniform1fv ) { agk::Message( "Failed to load OpenGL extension glUniform1fv" ); return false; }

		glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress( "glUniform2fv" );
		if ( !glUniform2fv ) { agk::Message( "Failed to load OpenGL extension glUniform2fv" ); return false; }

		glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress( "glUniform3fv" );
		if ( !glUniform3fv ) { agk::Message( "Failed to load OpenGL extension glUniform3fv" ); return false; }

		glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress( "glUniform4fv" );
		if ( !glUniform4fv ) { agk::Message( "Failed to load OpenGL extension glUniform4fv" ); return false; }

		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress( "glGetShaderInfoLog" );
		if ( !glGetShaderInfoLog ) { agk::Message( "Failed to load OpenGL extension glGetShaderInfoLog" ); return false; }

		glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress( "glGetProgramInfoLog" );
		if ( !glGetProgramInfoLog ) { agk::Message( "Failed to load OpenGL extension glGetProgramInfoLog" ); return false; }

		glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress( "glGetActiveUniform" );
		if ( !glGetActiveUniform ) { agk::Message( "Failed to load OpenGL extension glGetActiveUniform" ); return false; }

		glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC) wglGetProcAddress( "glGetActiveAttrib" );
		if ( !glGetActiveAttrib ) { agk::Message( "Failed to load OpenGL extension glGetActiveAttrib" ); return false; }

		glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) wglGetProcAddress( "glGetAttribLocation" );
		if ( !glGetAttribLocation ) { agk::Message( "Failed to load OpenGL extension glGetAttribLocation" ); return false; }

		glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress( "glGenBuffers" );
		if ( !glGenBuffers ) { agk::Message( "Failed to load OpenGL extension glGenBuffers" ); return false; }

		glBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress( "glBindBuffer" );
		if ( !glBindBuffer ) { agk::Message( "Failed to load OpenGL extension glBindBuffer" ); return false; }

		glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress( "glBufferData" );
		if ( !glBufferData ) { agk::Message( "Failed to load OpenGL extension glBufferData" ); return false; }

		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) wglGetProcAddress( "glDeleteBuffers" );
		if ( !glDeleteBuffers ) { agk::Message( "Failed to load OpenGL extension glDeleteBuffers" ); return false; }

		glActiveTexture = (PFNGLACTIVETEXTUREPROC) wglGetProcAddress( "glActiveTexture" );
		if ( !glActiveTexture ) { agk::Message( "Failed to load OpenGL extension glActiveTexture" ); return false; }

		glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC) wglGetProcAddress( "glBlendFuncSeparate" );
		if ( !glBlendFuncSeparate ) 
		{ 
			glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC) wglGetProcAddress( "glBlendFuncSeparateExt" );
			if ( !glBlendFuncSeparate ) 
			{
				agk::Message( "Failed to load OpenGL extension glBlendFuncSeparate" ); 
				return false; 
			}
		}
				
		return true;
	}

	HWND g_hWndOpenGL = 0;
	HDC g_hDCOpenGL = 0;
	HGLRC g_hOpenGLRC = 0;

	int CreateRendererWindow( HWND hWnd )
	{
		g_hWndOpenGL = hWnd;
		g_hDCOpenGL = GetDC( g_hWndOpenGL );
		if ( g_hDCOpenGL == 0 )
		{
			agk::Error( uString("Failed to get DC") );
			return 0;
		}

		BOOL bResult = 0;
		int multisample = 1;
		int iFormat = 0;
		PIXELFORMATDESCRIPTOR pfd;
		
		if ( multisample )
		{
			// create a dummy window to get WGL extensions, this will be destroyed before we return
			WNDCLASSEX wcex;
			memset( &wcex, 0, sizeof(WNDCLASSEX) );
			// hardcoded resource IDs for icons
			wcex.cbSize			= sizeof(WNDCLASSEX);
			wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wcex.lpfnWndProc	= DefWindowProc;
			wcex.hInstance		= (HINSTANCE) GetModuleHandle(NULL);
			wcex.hIcon			= LoadIcon((HINSTANCE) GetModuleHandle(NULL), MAKEINTRESOURCE(104)); 
			wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
			wcex.lpszClassName	= "AGKOPENGLWINDOW2";
			RegisterClassEx(&wcex);

			HWND tempHWnd = CreateWindow("AGKOPENGLWINDOW2", "", WS_OVERLAPPEDWINDOW & (~WS_SIZEBOX), 0, 0, 1, 1, NULL, NULL, (HINSTANCE) GetModuleHandle(NULL), NULL);
			if ( tempHWnd == 0 )
			{
				agk::Error( uString("Failed to create temp window") );
				return 0;
			}

			HDC tempHDC = GetDC( tempHWnd );
			if ( tempHDC == 0 )
			{
				agk::Error( uString("Failed to get temp DC") );
				return 0;
			}
			
			// create temporary OpenGL context just to load extensions
			ZeroMemory( &pfd, sizeof( pfd ) );
			pfd.nSize = sizeof( pfd );
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SUPPORT_COMPOSITION;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cAlphaBits = 8;
			pfd.cDepthBits = 24;
			pfd.iLayerType = PFD_MAIN_PLANE;
			iFormat = ChoosePixelFormat( tempHDC, &pfd );
			if ( iFormat == 0 )
			{
				agk::Error( uString("Temp choose Format Failed") );
				return 0;
			}

			bResult = SetPixelFormat( tempHDC, iFormat, &pfd );
			if ( !bResult ) 
			{
				char str[256];
				sprintf_s( str, 256, "Temp set Pixel Format Failed: %d", GetLastError() );
				agk::Error( uString( str ) );
				return 0;
			}

			HGLRC tempHGLRC = wglCreateContext( tempHDC );
			bResult = wglMakeCurrent( tempHDC, tempHGLRC );
			if ( !bResult ) 
			{
				agk::Error( uString("Temp wglMakeCurrent Failed") );
				return 0;
			}

			// get what we came for and delete the temp window
			PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress( "wglChoosePixelFormatARB" );
			DestroyWindow( tempHWnd );

			if ( !wglChoosePixelFormatARB ) 
			{
				multisample = 0;
			}
			else
			{
				// start normal window setup
				UINT numFormats;
				float fAttributes[] = {0,0};
			 
				int iAttributes[] = { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
					WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
					WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
					WGL_COLOR_BITS_ARB, 24,
					WGL_ALPHA_BITS_ARB, 8,
					WGL_DEPTH_BITS_ARB, 24,
					WGL_STENCIL_BITS_ARB, 0,
					WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
					WGL_SAMPLE_BUFFERS_EXT, GL_TRUE,
					WGL_SAMPLES_EXT, 4,
					0,0 };
			 
				// first we check to see if we can get a pixel format for 4 samples
				int valid = wglChoosePixelFormatARB( g_hDCOpenGL, iAttributes, fAttributes, 1, &iFormat, &numFormats );
			  
				if ( !valid || numFormats < 1 )
				{
					// try without multisampling
					agk::Warning( uString("Multisample Choose Format Failed") );
					multisample = 0;
				}
			}
		}		
		
		if ( !multisample )
		{
			// old window setup code that doesn't use multisampling
			ZeroMemory( &pfd, sizeof( pfd ) );
			pfd.nSize = sizeof( pfd );
			pfd.nVersion = 1;
			pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_SUPPORT_COMPOSITION;
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 32;
			pfd.cAlphaBits = 8;
			pfd.cDepthBits = 24;
			pfd.iLayerType = PFD_MAIN_PLANE;
			iFormat = ChoosePixelFormat( g_hDCOpenGL, &pfd );
			if ( iFormat == 0 )
			{
				agk::Error( uString("Choose Format Failed") );
				return 0;
			}
		}

		bResult = SetPixelFormat( g_hDCOpenGL, iFormat, &pfd );
		if ( !bResult ) 
		{
	#ifdef _AGK_ERROR_CHECK
			char str[256];
			sprintf_s( str, 256, "Set Pixel Format Failed: %d", GetLastError() );
			//MessageBox( NULL, str, "Error", 0 );
			agk::Error( uString( str ) );
	#endif
			return 0;
		}

		g_hOpenGLRC = wglCreateContext( g_hDCOpenGL );
		bResult = wglMakeCurrent( g_hDCOpenGL, g_hOpenGLRC );
		if ( !bResult ) 
		{
	#ifdef _AGK_ERROR_CHECK
			//MessageBox( NULL, "Make Current Failed", "Error", 0 );
			agk::Error( uString("Make Current Failed") );
	#endif
			return 0;
		}

		return 1;
	}

	void agk::PlatformDestroyGL ( void )
	{
		if ( g_hDCOpenGL && g_hOpenGLRC )
		{
			wglMakeCurrent(g_hDCOpenGL,NULL);
			wglDeleteContext(g_hOpenGLRC);
		}
		g_hOpenGLRC = 0;

		if ( g_hWndOpenGL && g_hDCOpenGL ) ReleaseDC( g_hWndOpenGL, g_hDCOpenGL );
		g_hDCOpenGL = 0;
	}

	void agk::PlatformSwap()
	{
		GLenum err = glGetError();
		if ( err != GL_NO_ERROR )
		{
			if ( err == GL_OUT_OF_MEMORY )
			{
				agk::Error( "Ran out of GPU memory, try using smaller or fewer images" );
				throw 1;
			}
		}

		// double buffering
		SwapBuffers( g_hDCOpenGL );
		//wglSwapLayerBuffers( g_hDC, WGL_SWAP_MAIN_PLANE );
	}
#endif

#ifdef AGK_MACOS

    namespace AGK
    {
        extern GLFWwindow *g_pWindow;
    }

	void agk::PlatformDestroyGL ( void )
	{
		
	}

	void agk::PlatformSwap()
	{
		GLenum err = glGetError();
		if ( err != GL_NO_ERROR )
		{
			if ( err == GL_OUT_OF_MEMORY )
			{
				agk::Error( "Ran out of GPU memory, try using smaller or fewer images" );
				throw 1;
			}
		}
		
		// display backbuffer
		glfwSwapBuffers(g_pWindow);
	}
#endif

#ifdef AGK_LINUX
	namespace AGK
	{
		extern GLFWwindow *g_pWindow;
	}

	bool LoadRendererExtensions()
	{
		// framebuffer/renderbuffer objects
		glBindFramebuffer = (PFNGLBINDFRAMEBUFFEREXTPROC)glXGetProcAddress( (GLubyte*)"glBindFramebufferEXT" );
		if( !glBindFramebuffer ) agk::Message( "Failed to get OpenGL extension glBindFramebufferEXT" );

		glGenFramebuffers = (PFNGLGENFRAMEBUFFERSEXTPROC)glXGetProcAddress( (GLubyte*) "glGenFramebuffersEXT" );
		if( !glGenFramebuffers ) agk::Message( "Failed to get OpenGL extension glGenFramebuffersEXT" );

		glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)glXGetProcAddress( (GLubyte*) "glFramebufferTexture2DEXT" );
		if( !glFramebufferTexture2D ) agk::Message( "Failed to get OpenGL extension glFramebufferTexture2DEXT" );

		glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)glXGetProcAddress( (GLubyte*) "glFramebufferRenderbufferEXT" );
		if( !glFramebufferRenderbuffer ) agk::Message( "Failed to get OpenGL extension glFramebufferRenderbufferEXT" );

		glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)glXGetProcAddress( (GLubyte*) "glCheckFramebufferStatusEXT" );
		if( !glCheckFramebufferStatus ) agk::Message( "Failed to get OpenGL extension glCheckFramebufferStatusEXT" );

		glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSEXTPROC)glXGetProcAddress( (GLubyte*) "glDeleteFramebuffersEXT" );
		if( !glDeleteFramebuffers ) agk::Message( "Failed to get OpenGL extension glDeleteFramebuffersEXT" );

		glBindRenderbuffer = (PFNGLBINDRENDERBUFFEREXTPROC)glXGetProcAddress( (GLubyte*) "glBindRenderbufferEXT" );
		if( !glBindRenderbuffer ) agk::Message( "Failed to get OpenGL extension glBindRenderbufferEXT" );

		glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSEXTPROC)glXGetProcAddress( (GLubyte*) "glDeleteRenderbuffersEXT" );
		if( !glDeleteRenderbuffers ) agk::Message( "Failed to get OpenGL extension glDeleteRenderbuffersEXT" );

		glGenRenderbuffers = (PFNGLGENRENDERBUFFERSEXTPROC)glXGetProcAddress( (GLubyte*) "glGenRenderbuffersEXT" );
		if( !glGenRenderbuffers ) agk::Message( "Failed to get OpenGL extension glGenRenderbuffersEXT" );

		glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEEXTPROC)glXGetProcAddress( (GLubyte*) "glRenderbufferStorageEXT" );
		if( !glRenderbufferStorage ) agk::Message( "Failed to get OpenGL extension glRenderbufferStorageEXT" );

		// mipmaps (OpenGL 3.0+ only), if missing don't error just fall back to GL_GENERATE_MIPMAP
		glGenerateMipmap = (PFNGLGENERATEMIPMAPEXTPROC)glXGetProcAddress( (GLubyte*) "glGenerateMipmap" );
		if ( !glGenerateMipmap ) glGenerateMipmap = (PFNGLGENERATEMIPMAPEXTPROC)glXGetProcAddress( (GLubyte*) "glGenerateMipmapEXT" );
		//if ( !glGenerateMipmap ) agk::Message( "Failed to load OpenGL extension glGenerateMipmap" );

		// shader functions
		glCreateShader = (PFNGLCREATESHADERPROC)glXGetProcAddress( (GLubyte*) "glCreateShader" );
		if ( !glCreateShader ) { agk::Message( "Failed to load OpenGL extension glCreateShader" ); return false; }

		glShaderSource = (PFNGLSHADERSOURCEPROC)glXGetProcAddress( (GLubyte*) "glShaderSourceARB" );
		if ( !glShaderSource ) { agk::Message( "Failed to load OpenGL extension glShaderSource" ); return false; }

		glCompileShader = (PFNGLCOMPILESHADERPROC)glXGetProcAddress( (GLubyte*) "glCompileShaderARB" );
		if ( !glCompileShader ) { agk::Message( "Failed to load OpenGL extension glCompileShader" ); return false; }

		glGetShaderiv = (PFNGLGETSHADERIVPROC)glXGetProcAddress( (GLubyte*) "glGetShaderiv" );
		if ( !glGetShaderiv ) { agk::Message( "Failed to load OpenGL extension glGetShaderiv" ); return false; }

		glDeleteShader = (PFNGLDELETESHADERPROC)glXGetProcAddress( (GLubyte*) "glDeleteShader" );
		if ( !glDeleteShader ) { agk::Message( "Failed to load OpenGL extension glDeleteShader" ); return false; }

		glAttachShader = (PFNGLATTACHSHADERPROC)glXGetProcAddress( (GLubyte*) "glAttachShader" );
		if ( !glAttachShader ) { agk::Message( "Failed to load OpenGL extension glAttachShader" ); return false; }

		glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)glXGetProcAddress( (GLubyte*) "glBindAttribLocation" );
		if ( !glBindAttribLocation ) { agk::Message( "Failed to load OpenGL extension glBindAttribLocation" ); return false; }

		glLinkProgram = (PFNGLLINKPROGRAMPROC)glXGetProcAddress( (GLubyte*) "glLinkProgramARB" );
		if ( !glLinkProgram ) { agk::Message( "Failed to load OpenGL extension glLinkProgram" ); return false; }

		glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glXGetProcAddress( (GLubyte*) "glGetProgramiv" );
		if ( !glGetProgramiv ) { agk::Message( "Failed to load OpenGL extension glGetProgramiv" ); return false; }

		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddress( (GLubyte*) "glGetUniformLocation" );
		if ( !glGetUniformLocation ) { agk::Message( "Failed to load OpenGL extension glGetUniformLocation" ); return false; }

		glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress( (GLubyte*) "glDisableVertexAttribArray" );
		if ( !glDisableVertexAttribArray ) { agk::Message( "Failed to load OpenGL extension glDisableVertexAttribArray" ); return false; }

		glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress( (GLubyte*) "glEnableVertexAttribArray" );
		if ( !glEnableVertexAttribArray ) { agk::Message( "Failed to load OpenGL extension glEnableVertexAttribArray" ); return false; }

		glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress( (GLubyte*) "glVertexAttribPointer" );
		if ( !glVertexAttribPointer ) { agk::Message( "Failed to load OpenGL extension glVertexAttribPointer" ); return false; }

		glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)glXGetProcAddress( (GLubyte*) "glVertexAttrib4fv" );
		if ( !glVertexAttrib4fv ) { agk::Message( "Failed to load OpenGL extension glVertexAttrib4fv" ); return false; }

		glCreateProgram = (PFNGLCREATEPROGRAMPROC)glXGetProcAddress( (GLubyte*) "glCreateProgram" );
		if ( !glCreateProgram ) { agk::Message( "Failed to load OpenGL extension glCreateProgram" ); return false; }

		glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glXGetProcAddress( (GLubyte*) "glDeleteProgram" );
		if ( !glDeleteProgram ) { agk::Message( "Failed to load OpenGL extension glDeleteProgram" ); return false; }

		glUseProgram = (PFNGLUSEPROGRAMPROC)glXGetProcAddress( (GLubyte*) "glUseProgram" );
		if ( !glUseProgram ) { agk::Message( "Failed to load OpenGL extension glUseProgram" ); return false; }

		glUniformMatrix2fv = (PFNGLUNIFORMMATRIX3FVPROC)glXGetProcAddress( (GLubyte*) "glUniformMatrix2fv" );
		if ( !glUniformMatrix2fv ) { agk::Message( "Failed to load OpenGL extension glUniformMatrix2fv" ); return false; }

		glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)glXGetProcAddress( (GLubyte*) "glUniformMatrix3fv" );
		if ( !glUniformMatrix3fv ) { agk::Message( "Failed to load OpenGL extension glUniformMatrix3fv" ); return false; }

		glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glXGetProcAddress( (GLubyte*) "glUniformMatrix4fv" );
		if ( !glUniformMatrix4fv ) { agk::Message( "Failed to load OpenGL extension glUniformMatrix4fv" ); return false; }

		glUniform1i = (PFNGLUNIFORM1IPROC)glXGetProcAddress( (GLubyte*) "glUniform1i" );
		if ( !glUniform1i ) { agk::Message( "Failed to load OpenGL extension glUniform1i" ); return false; }

		glUniform1f = (PFNGLUNIFORM1FPROC)glXGetProcAddress( (GLubyte*) "glUniform1f" );
		if ( !glUniform1f ) { agk::Message( "Failed to load OpenGL extension glUniform1f" ); return false; }

		glUniform2f = (PFNGLUNIFORM2FPROC)glXGetProcAddress( (GLubyte*) "glUniform2f" );
		if ( !glUniform2f ) { agk::Message( "Failed to load OpenGL extension glUniform2f" ); return false; }

		glUniform3f = (PFNGLUNIFORM3FPROC)glXGetProcAddress( (GLubyte*) "glUniform3f" );
		if ( !glUniform3f ) { agk::Message( "Failed to load OpenGL extension glUniform3f" ); return false; }

		glUniform4f = (PFNGLUNIFORM4FPROC)glXGetProcAddress( (GLubyte*) "glUniform4f" );
		if ( !glUniform4f ) { agk::Message( "Failed to load OpenGL extension glUniform4f" ); return false; }

		glUniform1fv = (PFNGLUNIFORM1FVPROC)glXGetProcAddress( (GLubyte*) "glUniform1fv" );
		if ( !glUniform1fv ) { agk::Message( "Failed to load OpenGL extension glUniform1fv" ); return false; }

		glUniform2fv = (PFNGLUNIFORM2FVPROC)glXGetProcAddress( (GLubyte*) "glUniform2fv" );
		if ( !glUniform2fv ) { agk::Message( "Failed to load OpenGL extension glUniform2fv" ); return false; }

		glUniform3fv = (PFNGLUNIFORM3FVPROC)glXGetProcAddress( (GLubyte*) "glUniform3fv" );
		if ( !glUniform3fv ) { agk::Message( "Failed to load OpenGL extension glUniform3fv" ); return false; }

		glUniform4fv = (PFNGLUNIFORM4FVPROC)glXGetProcAddress( (GLubyte*) "glUniform4fv" );
		if ( !glUniform4fv ) { agk::Message( "Failed to load OpenGL extension glUniform4fv" ); return false; }

		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddress( (GLubyte*) "glGetShaderInfoLog" );
		if ( !glGetShaderInfoLog ) { agk::Message( "Failed to load OpenGL extension glGetShaderInfoLog" ); return false; }

		glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glXGetProcAddress( (GLubyte*) "glGetProgramInfoLog" );
		if ( !glGetProgramInfoLog ) { agk::Message( "Failed to load OpenGL extension glGetProgramInfoLog" ); return false; }

		glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)glXGetProcAddress( (GLubyte*) "glGetActiveUniform" );
		if ( !glGetActiveUniform ) { agk::Message( "Failed to load OpenGL extension glGetActiveUniform" ); return false; }

		glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC) glXGetProcAddress( (GLubyte*) "glGetActiveAttrib" );
		if ( !glGetActiveAttrib ) { agk::Message( "Failed to load OpenGL extension glGetActiveAttrib" ); return false; }

		glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) glXGetProcAddress( (GLubyte*) "glGetAttribLocation" );
		if ( !glGetAttribLocation ) { agk::Message( "Failed to load OpenGL extension glGetAttribLocation" ); return false; }

		glGenBuffers = (PFNGLGENBUFFERSPROC) glXGetProcAddress( (GLubyte*) "glGenBuffers" );
		if ( !glGenBuffers ) { agk::Message( "Failed to load OpenGL extension glGenBuffers" ); return false; }

		glBindBuffer = (PFNGLBINDBUFFERPROC) glXGetProcAddress( (GLubyte*) "glBindBuffer" );
		if ( !glBindBuffer ) { agk::Message( "Failed to load OpenGL extension glBindBuffer" ); return false; }

		glBufferData = (PFNGLBUFFERDATAPROC) glXGetProcAddress( (GLubyte*) "glBufferData" );
		if ( !glBufferData ) { agk::Message( "Failed to load OpenGL extension glBufferData" ); return false; }

		glDeleteBuffers = (PFNGLDELETEBUFFERSPROC) glXGetProcAddress( (GLubyte*) "glDeleteBuffers" );
		if ( !glDeleteBuffers ) { agk::Message( "Failed to load OpenGL extension glDeleteBuffers" ); return false; }

		//glActiveTexture = (PFNGLACTIVETEXTUREPROC) glXGetProcAddress( (GLubyte*) "glActiveTexture" );
		//if ( !glActiveTexture ) { agk::Message( "Failed to load OpenGL extension glActiveTexture" ); return false; }

		glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC) glXGetProcAddress( (GLubyte*) "glBlendFuncSeparate" );
		if ( !glBlendFuncSeparate ) 
		{ 
			glBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC) glXGetProcAddress( (GLubyte*) "glBlendFuncSeparateExt" );
			if ( !glBlendFuncSeparate ) 
			{
				agk::Message( "Failed to load OpenGL extension glBlendFuncSeparate" ); 
				return false; 
			}
		}
		
		return true;
	}

	void agk::PlatformDestroyGL ( void )
	{
		
	}

	void agk::PlatformSwap()
	{
		GLenum err = glGetError();
		if ( err != GL_NO_ERROR )
		{
			if ( err == GL_OUT_OF_MEMORY )
			{
				agk::Error( "Ran out of GPU memory, try using smaller or fewer images" );
				throw 1;
			}
		}

		glfwSwapBuffers(g_pWindow);
	}
#endif

//****f* Core/Display/IsSupportedDepthTexture
// FUNCTION
//   This command returns 1 if the current device supports using depth images with <i>SetRenderToImage</i>. If this
//   returns 0 then the only valid depth image IDs to SetRenderToImage are 0 for no depth, or -1 for a normal depth 
//   buffer.
// SOURCE
UINT agk::IsSupportedDepthTexture()
//****
{
	return 1; // OpenGL 2.0 guarantees support for depth textures
}

int agk::CanUseIntIndices() 
{ 
	return 1; // OpenGL 2.0 guarantees support for 32bit indices
}

int agk::CanUseShadowSamplers() 
{ 
	return 1; // OpenGL 2.0 guarantees support for shadow samplers
}

//****f* Core/Misc/SetAntialiasMode
// FUNCTION
//   Sets whether the device should use anti-aliasing when rendering to the back buffer. Currently 
//   this only applies to Windows, Mac, and Linux, and only 4x multi-sampling is available.
//   This does not apply to any objects drawn to an image with SetRenderToImage, only the back buffer 
//   is anti-aliased.
// INPUTS
//   mode -- 0=off, 1=4xMSAA
// SOURCE
void agk::SetAntialiasMode( int mode )
//****
{
	if ( mode == 1 ) glEnable( GL_MULTISAMPLE );
	else glDisable( GL_MULTISAMPLE );
}

int agk::PlatformGetMaxVSUniforms()
{
	int maxUniformVectors;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxUniformVectors);
	return maxUniformVectors/4;
}

int agk::PlatformGetMaxPSUniforms()
{
	int maxUniformVectors;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxUniformVectors);
	return maxUniformVectors/4;
}

#ifndef GL_MAX_VARYING_COMPONENTS
    #define GL_MAX_VARYING_COMPONENTS GL_MAX_VARYING_FLOATS
#endif
int agk::PlatformGetMaxVaryings()
{
	int maxUniformVectors;
	glGetIntegerv(GL_MAX_VARYING_COMPONENTS, &maxUniformVectors);
	return maxUniformVectors;
}

int agk::PlatformGetMaxVertexTextures()
{
	int maxVertexTextures;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &maxVertexTextures);
	return maxVertexTextures;
}

void agk::PlatformSetAlignment( int amount )
{
	glPixelStorei( GL_PACK_ALIGNMENT, amount );
	glPixelStorei( GL_UNPACK_ALIGNMENT, amount );
}

void agk::PlatformDisableScissor()
{
	glDisable( GL_SCISSOR_TEST );
}

void agk::PlatformDisableStencil()
{
	glDisable( GL_STENCIL_TEST );
}

void agk::PlatformPrepareDefaultDraw()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	agk::PlatformSetDepthTest( 1 );
	agk::PlatformSetBlendMode( 1 );
	agk::PlatformSetCullMode( 0 );
	agk::PlatformSetDepthWrite( 1 );
	agk::PlatformSetDepthRange( 0, 1 );
	agk::PlatformSetDepthBias( 0 );
	glFrontFace( GL_CCW );
}

void agk::PlatformSetBlendEnabled( int mode )
{
	if ( mode == m_iCurrentBlendEnabled ) return;

	if ( mode > 0 ) glEnable( GL_BLEND );
	else glDisable( GL_BLEND );

	m_iCurrentBlendEnabled = mode > 0 ? 1 : 0;
}

void agk::PlatformSetDepthTest( int mode )
{
	if ( mode == m_iCurrentDepthTest ) return;

	if ( mode > 0 ) glEnable( GL_DEPTH_TEST );
	else glDisable( GL_DEPTH_TEST );

	m_iCurrentDepthTest = mode > 0 ? 1 : 0;
}

void agk::PlatformSetBlendFunc( int mode1, int mode2 )
{
	if ( mode1 == m_iCurrentBlendFunc1 && mode2 == m_iCurrentBlendFunc2 ) return;

	int final1 = GL_ONE;
	switch ( mode1 )
	{
		case 0: final1 = GL_ZERO; break;
		case 1: final1 = GL_ONE; break;
		case 2: final1 = GL_SRC_ALPHA; break;
		case 3: final1 = GL_ONE_MINUS_SRC_ALPHA; break;
		case 4: final1 = GL_DST_ALPHA; break;
		case 5: final1 = GL_ONE_MINUS_DST_ALPHA; break;
		//case 6: final1 = GL_SRC_COLOR; break; // illegal
		//case 7: final1 = GL_ONE_MINUS_SRC_COLOR; break; // illegal
		case 8: final1 = GL_DST_COLOR; break;
		case 9: final1 = GL_ONE_MINUS_DST_COLOR; break;
		case 10: final1 = GL_SRC_ALPHA_SATURATE; break;
		default: mode1 = AGK_BLEND_ONE;
	}

	int final2 = GL_ZERO;
	switch ( mode2 )
	{
		case 0: final2 = GL_ZERO; break;
		case 1: final2 = GL_ONE; break;
		case 2: final2 = GL_SRC_ALPHA; break;
		case 3: final2 = GL_ONE_MINUS_SRC_ALPHA; break;
		case 4: final2 = GL_DST_ALPHA; break;
		case 5: final2 = GL_ONE_MINUS_DST_ALPHA; break;
		case 6: final2 = GL_SRC_COLOR; break; 
		case 7: final2 = GL_ONE_MINUS_SRC_COLOR; break; 
		//case 8: final2 = GL_DST_COLOR; break; // illegal
		//case 9: final2 = GL_ONE_MINUS_DST_COLOR; break; // illegal
		//case 10: final2 = GL_SRC_ALPHA_SATURATE; break; // illegal
		default: mode2 = AGK_BLEND_ZERO;
	}

	glBlendFuncSeparate( final1, final2, GL_ONE_MINUS_DST_ALPHA, GL_ONE );
	
	m_iCurrentBlendFunc1 = mode1;
	m_iCurrentBlendFunc2 = mode2;
}

void agk::PlatformSetDepthWrite( int mode )
{
	if ( mode == m_iCurrentDepthWrite ) return;

	if ( mode > 0 ) glDepthMask( GL_TRUE );
	else glDepthMask( GL_FALSE );

	m_iCurrentDepthWrite = mode > 0 ? 1 : 0;
}

void agk::PlatformSetDepthBias( float bias )
{
	if ( bias == m_fCurrentDepthBias ) return;

	if ( bias == 0 ) glDisable( GL_POLYGON_OFFSET_FILL );
	else 
	{
		if ( m_fCurrentDepthBias == 0 ) glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( 1, bias );
	}

	m_fCurrentDepthBias = bias;
}

void agk::PlatformSetDepthRange( float zNear, float zFar )
{
	if ( zNear < 0 ) zNear = 0;
	if ( zNear > 1 ) zNear = 1;
	if ( zFar < 0 ) zFar = 0;
	if ( zFar > 1 ) zFar = 1;

	if ( zNear == m_fCurrentDepthNear && zFar == m_fCurrentDepthFar ) return;

	glDepthRange( zNear, zFar );

	m_fCurrentDepthNear = zNear;
	m_fCurrentDepthFar = zFar;
}

void agk::PlatformSetDepthFunc( int mode )
{
	if ( mode == m_iCurrentDepthFunc ) return;

	switch ( mode )
	{
		case 0: glDepthFunc( GL_NEVER ); break;
		case 1: glDepthFunc( GL_LESS ); break;
		case 2: glDepthFunc( GL_EQUAL ); break;
		case 3: glDepthFunc( GL_LEQUAL ); break;
		case 4: glDepthFunc( GL_GREATER ); break;
		case 5: glDepthFunc( GL_NOTEQUAL ); break;
		case 6: glDepthFunc( GL_GEQUAL ); break;
		case 7: glDepthFunc( GL_ALWAYS ); break;
		default: return;
	}
	
	m_iCurrentDepthFunc = mode;
}

void agk::PlatformSetCullMode( int mode )
{
	if ( mode == m_iCurrentCullMode ) return;

	switch( mode )
	{
		case 0: if ( m_iCurrentCullMode != 0 ) glDisable( GL_CULL_FACE ); break; // both front and back
		case 1: // front faces
		{
			if ( m_iCurrentCullMode <= 0 ) glEnable( GL_CULL_FACE );
			glCullFace( GL_BACK );
			break;
		}
		case 2: // back faces
		{
			if ( m_iCurrentCullMode <= 0 ) glEnable( GL_CULL_FACE );
			glCullFace( GL_FRONT );
			break;
		}
		default: return;
	}

	m_iCurrentCullMode = mode;
}

bool agk::PlatformBindBuffer( UINT buffer )
{
	if ( m_iCurrentBoundVBO == buffer ) return false;

	m_iCurrentBoundVBO = buffer;
	glBindBuffer( GL_ARRAY_BUFFER, buffer );

	return true;
}

bool agk::PlatformBindIndexBuffer( UINT buffer )
{
	if ( m_iCurrentBoundIndexVBO == buffer ) return false;

	m_iCurrentBoundIndexVBO = buffer;
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer );

	return true;
}

void agk::PlatformSetViewport( int x, int y, int width, int height )
{
	glViewport( x, y, width, height );
}

void agk::PlatformBindDefaultFramebuffer()
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}

void agk::PlatformFlush()
{
	glFlush();
}

void agk::PlatformClearScreenRaw( float red, float green, float blue, float alpha )
{
	glClearColor( red, green, blue, alpha );
	glClear( GL_COLOR_BUFFER_BIT );
}

void agk::PlatformClearScreen()
{
	// this *must* be set back to true before clearing the depth buffer.
	agk::PlatformSetDepthWrite( 1 );

	if ( !m_bClearColor )
	{
		// no color to clear
		//glDisable( GL_SCISSOR_TEST );
		PlatformScissor( 0,0,0,0 );
		if ( m_bClearDepth ) glClear( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		PlatformScissor( m_iScissorX, m_iScissorY, m_iScissorWidth, m_iScissorHeight );
	}
	else
	{
		if ( m_fTargetViewportX == 0 && m_fTargetViewportY == 0 )
		{
			// no borders
			GLbitfield iClear = GL_COLOR_BUFFER_BIT;
			if ( m_bClearDepth ) iClear |= GL_DEPTH_BUFFER_BIT;
			
			PlatformScissor( 0,0,0,0 );
			float red = ((m_iClearColor >> 16) & 0xff) / 255.0f;
			float green = ((m_iClearColor >> 8) & 0xff) / 255.0f;
			float blue = (m_iClearColor & 0xff) / 255.0f;
			glClearColor( red, green, blue, 0 );
			glClear( iClear | GL_STENCIL_BUFFER_BIT );
			PlatformScissor( m_iScissorX, m_iScissorY, m_iScissorWidth, m_iScissorHeight );
		}
		else
		{
			// clear borders then clear main drawing area
			GLbitfield iClear = GL_COLOR_BUFFER_BIT;
			if ( m_bClearDepth ) iClear |= GL_DEPTH_BUFFER_BIT;

			float red = ((m_iBorderColor >> 16) & 0xff) / 255.0f; 
			float green = ((m_iBorderColor >> 8) & 0xff) / 255.0f; 
			float blue = (m_iBorderColor & 0xff) / 255.0f;
			PlatformScissor( 0,0,0,0 );
			glClearColor( red, green, blue, 0 );
			glClear( iClear | GL_STENCIL_BUFFER_BIT );

			// check if we need to clear again
			if ( m_iBorderColor != m_iClearColor )
			{
				int ired = ((m_iClearColor >> 16) & 0xff);
				int igreen = ((m_iClearColor >> 8) & 0xff);
				int iblue = (m_iClearColor & 0xff);
				static cSprite* g_pClearSprite = 0;
				if ( !g_pClearSprite ) g_pClearSprite = new cSprite();
				g_pClearSprite->SetSize( (float)agk::GetVirtualWidth(), (float)agk::GetVirtualHeight() );
				g_pClearSprite->SetColor( ired, igreen, iblue, 0 );
				g_pClearSprite->SetTransparency( 0 );
				agk::PlatformSetDepthTest(0);
				agk::PlatformSetDepthWrite(0);
				g_pClearSprite->Draw();
				agk::PlatformSetDepthTest(1);
			}

			PlatformScissor( m_iScissorX, m_iScissorY, m_iScissorWidth, m_iScissorHeight );
		}
	}
}

void agk::PlatformClearDepthBuffer()
{
	// this *must* be set back to true before clearing the depth buffer.
	agk::PlatformSetDepthWrite( 1 );

	PlatformScissor( 0,0,0,0 );
	glClear( GL_DEPTH_BUFFER_BIT );
	PlatformScissor( m_iScissorX, m_iScissorY, m_iScissorWidth, m_iScissorHeight );
}

void agk::PlatformScissor( int x, int y, int width, int height )
{
	if ( x == 0 && y == 0 && width == 0 && height == 0 )
	{
		if ( m_bScissorEnabled ) glDisable( GL_SCISSOR_TEST );
		m_bScissorEnabled = false;
	}
	else
	{
		if ( !m_bScissorEnabled )
		{
			glEnable( GL_SCISSOR_TEST );
		}
		glScissor( x, y, width, height );
		m_bScissorEnabled = true;
	}
}

// Image

void cImage::BindTexture( UINT iTex, UINT stage )
{
	if ( stage >= AGK_MAX_TEXTURES ) return;

	// check if we need to change texture
	if ( iTex == iCurrTexture[stage] ) return;

	glActiveTexture( GL_TEXTURE0 + stage );
	glBindTexture( GL_TEXTURE_2D, iTex );

	iCurrTexture[stage] = iTex;
}

void cImage::Bind( UINT stage )
{
	if ( stage > 7 ) return;
	
	UINT iTex = GetTextureID();

	// check if we need to change texture
	if ( iTex == iCurrTexture[stage] ) return;

	glActiveTexture( GL_TEXTURE0 + stage );
	if ( m_bIsCubeMap ) glBindTexture( GL_TEXTURE_CUBE_MAP, iTex );
	else glBindTexture( GL_TEXTURE_2D, iTex );

	iCurrTexture[stage] = iTex;
}

void cImage::UnBind()
{
	UINT iTex = GetTextureID();
    
	// check if we need to unbind texture
    for ( int i = 0; i < 8; i++ )
    {
        if ( iTex == iCurrTexture[i] )
        {
            glActiveTexture( GL_TEXTURE0 + i );
            if ( m_bIsCubeMap ) glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
            else glBindTexture( GL_TEXTURE_2D, 0 );
        
            iCurrTexture[i] = 0;
        }
    }
}

void cImage::PlatformDelete()
{
	if ( m_iTextureID != 0 ) glDeleteTextures( 1, &m_iTextureID );
	m_iTextureID = 0;
}

void cImage::PlatformSetSubData( int x, int y, int width, int height, unsigned char* pData )
{
	cImage::BindTexture( m_iTextureID );

	if( !glGenerateMipmap )
	{
		if ( m_bMipmapped ) glTexParameterf( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
		else glTexParameterf( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE );
	}

	UINT glformat = GL_RGBA;
	if ( m_iImageMode == 2 ) glformat = GL_ALPHA;
	glTexSubImage2D(GL_TEXTURE_2D, 0, x,y, width,height, glformat, GL_UNSIGNED_BYTE, pData );

	if ( glGenerateMipmap )
	{
		if ( m_bMipmapped ) glGenerateMipmap(GL_TEXTURE_2D);
	}
}

void cImage::PlatformGetDataFromScreen( unsigned int** pData, int x, int y, int width, int height )
{
	*pData = new UINT[ width*height ];
	glReadBuffer( GL_BACK );
	glReadPixels( x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, *pData );
}

int cImage::GetRawDataFull( unsigned char** pData )
{
	if ( !pData ) return 0;
	if ( HasParent() ) return m_pParentImage->GetRawDataFull( pData );
	
	int iTexID = GetTextureID();
	if ( iTexID == 0 ) return 0;

	UINT iTrueWidthSrc = GetTotalWidth();
	UINT iTrueHeightSrc = GetTotalHeight();
	cImage::BindTexture( iTexID );
	if ( m_iImageMode == 2 )
	{
		*pData = new unsigned char[ iTrueWidthSrc*iTrueHeightSrc ];
		glGetTexImage( GL_TEXTURE_2D, 0, GL_ALPHA, GL_UNSIGNED_BYTE, *pData );
		return iTrueWidthSrc*iTrueHeightSrc;
	}
	else
	{
		*pData = new unsigned char[ iTrueWidthSrc*iTrueHeightSrc*4 ];
		glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, *pData );
		return iTrueWidthSrc*iTrueHeightSrc*4;
	}
}

void cImage::PlatformLoadFromData( int width, int height, UINT *bits )
{
	UINT glformat = GL_RGBA;
	UINT glbytesize = GL_UNSIGNED_BYTE;

	switch( m_iImageMode )
	{
		case 2: 
		{
			glformat = GL_ALPHA;
			break;
		}
	}

	if ( m_iTextureID == 0 ) glGenTextures( 1, &m_iTextureID );
	cImage::BindTexture( m_iTextureID );
	
	if ( !glGenerateMipmap )
	{
		if ( m_bMipmapped ) glTexParameterf( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
		else glTexParameterf( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE );
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, glformat, width, height, 0, glformat, glbytesize, bits );

	if ( glGenerateMipmap )
	{
		if ( m_bMipmapped ) glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	if ( m_iWrapU == 0 ) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	else glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	if ( m_iWrapV == 0 ) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	else glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if ( m_iMagFilter == 0 ) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if ( m_bMipmapped )
	{
		if ( m_iMinFilter == 0 ) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		if ( m_iMinFilter == 0 ) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void cImage::LoadSubRegionFromData( int x, int y, int width, int height, unsigned char *bits, int format )
{
	if ( m_iTextureID == 0 ) return;
	cImage::BindTexture( m_iTextureID );

	UINT glformat = GL_RGBA;
	UINT glbytesize = GL_UNSIGNED_BYTE;

	switch( m_iImageMode )
	{
		case 2: 
		{
			glformat = GL_ALPHA;
			break;
		}
	}

	glTexSubImage2D( GL_TEXTURE_2D, 0, x, y, width, height, glformat, glbytesize, bits );
	SetCompressedPixelData( 0, 0 );
}

void cImage::PlatformCreateBlank( int width, int height, UINT format, UINT mipmap )
{
	m_iOrigWidth = width;
	m_iOrigHeight = height;
	m_iWidth = width;
	m_iHeight = height;
	m_bResized = false;

	m_fU1 = 0;
	m_fV1 = 0;
	m_fU2 = 1;
	m_fV2 = 1;

	if ( m_iTextureID == 0 ) glGenTextures( 1, &m_iTextureID );
	cImage::BindTexture( m_iTextureID );
	
	m_bMipmapped = mipmap > 0;
	
	if ( !glGenerateMipmap )
	{
		// can't use GL_GENERATE_MIPMAP on frame buffer images
		m_bMipmapped = false;
	}

	UINT glformat = GL_RGBA;
	UINT glformat2 = GL_RGBA;
	UINT glsize = GL_UNSIGNED_BYTE;
	switch( format )
	{
		case 1: 
		{
			glformat = GL_DEPTH_COMPONENT24; 
			glformat2 = GL_DEPTH_COMPONENT; 
			glsize = GL_UNSIGNED_INT;
			break;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, glformat, m_iWidth, m_iHeight, 0, glformat2, glsize, 0 );

	if ( format == 1 ) glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE );

	if ( glGenerateMipmap )
	{
		if ( m_bMipmapped ) glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	if ( agk::m_iDefaultMagFilter == 0 ) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if ( m_bMipmapped )
	{
		if ( agk::m_iDefaultMinFilter == 0 ) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		if ( agk::m_iDefaultMinFilter == 0 ) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void cImage::GenerateMipmaps()
{
	if ( m_bMipmapped && glGenerateMipmap )
	{
		Bind();
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

void cImage::PlatformSetMagFilter( UINT mode )
{
	Bind();
	if ( mode == 0 ) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void cImage::PlatformSetMinFilter( UINT mode )
{
	Bind();
	if ( m_bMipmapped )
	{
		if ( mode == 0 ) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		if ( mode == 0 ) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
}

void cImage::PlatformSetWrapU( UINT mode )
{
	Bind();
	if ( mode == 0 ) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	else glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}

void cImage::PlatformSetWrapV( UINT mode )
{
	Bind();
	if ( mode == 0 ) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	else glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

int cImage::GetSupportsNPOT() 
{ 
	return 1; 
}

int cImage::GetMaxTextureSize() 
{ 
	static GLint texMaxSize = 0;
	if ( texMaxSize == 0 ) glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texMaxSize);
	return texMaxSize;
}

void cImage::PlatformReloadFromData()
{
	//if ( !m_pCompressedPixelData ) return;  still want to create the image even if it doesn't have data
	if ( m_pParentImage ) return;

	UINT glformat = GL_RGBA;
	UINT glformat2 = GL_RGBA;
	UINT glbytesize = GL_UNSIGNED_BYTE;
	uLong size = m_iWidth*m_iHeight*4;

	switch( m_iImageMode )
	{
		case 1: 
		{
			glformat = GL_DEPTH_COMPONENT24;
			glformat2 = GL_DEPTH_COMPONENT; 
			glbytesize = GL_UNSIGNED_INT;
			break;
		}
		case 2: 
		{
			glformat = GL_ALPHA;
			glformat2 = GL_ALPHA; 
			glbytesize = GL_UNSIGNED_BYTE;
			size = m_iWidth*m_iHeight;
			break;
		}
	}
	
	unsigned char* tempbuf = new unsigned char[ size ];
	if ( m_pCompressedPixelData ) 
	{
		int err = uncompress( tempbuf, &size, m_pCompressedPixelData, m_iCompressedLength );
		if ( err != Z_OK )
		{
			delete [] tempbuf;
			agk::Warning( "Failed to extract compressed image data" );
			return;
		}
	}
	else
	{
		// if no data exists for the image create it as a blank image
		for( unsigned int i = 0; i < size; i++ )
		{
			tempbuf[ i ] = 0;
		}
	}

	// generate OepnGL texture
	if ( m_iTextureID == 0 ) glGenTextures( 1, &m_iTextureID );
	cImage::BindTexture( m_iTextureID );

	if ( !glGenerateMipmap )
	{
		if ( m_bMipmapped ) glTexParameterf( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
		else glTexParameterf( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE );
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, glformat, m_iWidth, m_iHeight, 0, glformat2, glbytesize, tempbuf );

	if ( m_iImageMode == 1 ) glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE );

	if ( glGenerateMipmap )
	{
		if ( m_bMipmapped ) glGenerateMipmap(GL_TEXTURE_2D);
	}
	
	if ( m_iWrapU == 0 ) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	else glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	if ( m_iWrapV == 0 ) glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	else glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if ( m_iMagFilter == 0 ) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if ( m_bMipmapped )
	{
		if ( m_iMinFilter == 0 ) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
	{
		if ( m_iMinFilter == 0 ) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		else glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	delete [] tempbuf;
}

//3D drawing
void agk::PlatformDeleteBuffer( UINT buffer )
{
	if ( m_iCurrentBoundVBO == buffer ) agk::PlatformBindBuffer( 0 );
	if ( m_iCurrentBoundIndexVBO == buffer ) agk::PlatformBindIndexBuffer( 0 );
	glDeleteBuffers( 1, &buffer );
}

void cObject3D::PlatformSetupDrawing()
{
	switch ( m_iTransparency )
	{
		case 0:	agk::PlatformSetBlendMode( 0 ); break;
		case 1:	agk::PlatformSetBlendMode( 1 ); break;
		case 2:	agk::PlatformSetBlendMode( 2 ); break;
		case 3: 
		{
			agk::PlatformSetBlendEnabled( 1 );
			agk::PlatformSetBlendFunc( m_iBlendModes & 0x0F, (m_iBlendModes >> 4) & 0x0F );
			break;
		}
		default: agk::PlatformSetBlendMode( 0 ); break;
	}

	agk::PlatformSetDepthTest( 1 );
	agk::PlatformSetDepthFunc( m_iZFunc );
	agk::PlatformSetDepthWrite( (m_iObjFlags & AGK_OBJECT_Z_WRITE) ? 1 : 0 );
	agk::PlatformSetCullMode( m_iCullMode );
	agk::PlatformSetDepthBias( m_fZBias );
	agk::PlatformSetDepthRange( m_fDepthNear, m_fDepthFar );
	
	AGKMatrix4 matWorld;
	
	matWorld.MakeWorld( rotFinal(), posFinal(), scaleFinal() );
	AGKShader::GetCurrentShader()->SetWorldMatrix( matWorld.GetFloatPtr() );

	// use inverse scale here in case the scale is non-uniform, also never invert the normal with negative scale,
	// using scale to turn the object inside out doesn't turn the polygons around, it only modifies their position
	AGKVector normScale = scaleFinal().GetInverse();
	if ( normScale.x < 0 ) normScale.x = -normScale.x;
	if ( normScale.y < 0 ) normScale.y = -normScale.y;
	if ( normScale.z < 0 ) normScale.z = -normScale.z;
	AGKMatrix3 matWorldNorm;
	matWorldNorm.MakeWorld( rotFinal(), normScale );
	AGKShader::GetCurrentShader()->SetNormalMatrix( matWorldNorm.GetFloatPtr() );
}

// mesh
void cMesh::PlatformGenBuffers()
{
	if ( m_iNumArrays == 0 ) return;
	
	if ( !m_iVBOVertices )
	{
		m_iVBOVertices = new UINT[ m_iNumArrays ];
	
		for ( UINT i = 0; i < m_iNumArrays; i++ )
		{
			glGenBuffers( 1, &(m_iVBOVertices[ i ]) );
			agk::PlatformBindBuffer( m_iVBOVertices[ i ] );
			glBufferData( GL_ARRAY_BUFFER, m_iNumVertices[ i ]*m_iVertexStride, m_ppVBOVertexData[ i ], GL_STATIC_DRAW );
		}
	}
	else
	{
		for ( UINT i = 0; i < m_iNumArrays; i++ )
		{
			agk::PlatformBindBuffer( m_iVBOVertices[ i ] );
			glBufferData( GL_ARRAY_BUFFER, m_iNumVertices[ i ]*m_iVertexStride, NULL, GL_STATIC_DRAW ); // discard the old contents in a GPU friendly manner
			glBufferData( GL_ARRAY_BUFFER, m_iNumVertices[ i ]*m_iVertexStride, m_ppVBOVertexData[ i ], GL_STATIC_DRAW );
		}
	}

	if ( m_ppIndices )
	{
		if ( !m_iVBOIndices )
		{
			m_iVBOIndices = new UINT[ m_iNumArrays ];

			for ( UINT i = 0; i < m_iNumArrays; i++ )
			{
				glGenBuffers( 1, &(m_iVBOIndices[ i ]) );
				agk::PlatformBindIndexBuffer( m_iVBOIndices[ i ] );
				if ( m_iFlags & AGK_MESH_UINT_INDICES ) glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_iNumIndices[ i ]*sizeof(UINT), m_ppIndices[ i ], GL_STATIC_DRAW );
				else glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_iNumIndices[ i ]*sizeof(unsigned short), m_ppIndices[ i ], GL_STATIC_DRAW );
			}
		}
		else
		{
			for ( UINT i = 0; i < m_iNumArrays; i++ )
			{
				agk::PlatformBindIndexBuffer( m_iVBOIndices[ i ] );
				if ( m_iFlags & AGK_MESH_UINT_INDICES ) 
				{
					glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_iNumIndices[ i ]*sizeof(UINT), NULL, GL_STATIC_DRAW ); // discard the old contents in a GPU friendly manner
					glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_iNumIndices[ i ]*sizeof(UINT), m_ppIndices[ i ], GL_STATIC_DRAW );
				}
				else 
				{
					glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_iNumIndices[ i ]*sizeof(unsigned short), NULL, GL_STATIC_DRAW ); // discard the old contents in a GPU friendly manner
					glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_iNumIndices[ i ]*sizeof(unsigned short), m_ppIndices[ i ], GL_STATIC_DRAW );
				}
			}
		}
	}
}

void cMesh::PlatformDraw( int shadow, int alphamask )
{
	cMesh *pVertexOwner = this;
	if ( m_pSharedVertices ) pVertexOwner = m_pSharedVertices;

	if ( pVertexOwner->m_iNumArrays == 0 || !pVertexOwner->m_iVBOVertices )
	{
		static bool bWarned = false;
		if ( !bWarned ) agk::Warning( "Tried to draw a mesh that has no vertices" );
		bWarned = true;
		return;
	}

	AGKShader *pShader = AGKShader::GetCurrentShader();

	for ( UINT i = 0; i < pVertexOwner->m_iNumArrays; i++ )
	{
		agk::m_iVerticesProcessed += pVertexOwner->m_iNumVertices[ i ];

		agk::PlatformBindBuffer( pVertexOwner->m_iVBOVertices[ i ] );

		for ( unsigned char a = 0; a < pVertexOwner->m_iNumAttribs; a++ )
		{
			cVertexAttrib *pAttrib = pVertexOwner->m_pVertexAttribs[ a ];

			int shaderLoc = pAttrib->m_iShaderLoc;
			if ( m_pSharedVertices ) shaderLoc = pShader->GetAttribByName( pAttrib->m_sName );
			if ( shadow )
			{
				if ( strcmp(pAttrib->m_sName.GetStr(),"position") == 0 ) shaderLoc = pShader->GetPositionLoc();
				else if ( strcmp(pAttrib->m_sName.GetStr(),"boneindices") == 0 ) shaderLoc = pShader->GetBoneIndicesLoc();
				else if ( strcmp(pAttrib->m_sName.GetStr(),"boneweights") == 0 ) shaderLoc = pShader->GetBoneWeightsLoc();
				else if ( alphamask && strcmp(pAttrib->m_sName.GetStr(),"uv") == 0 ) shaderLoc = pShader->GetUVLoc();
				else continue;
			}
			
			if ( shaderLoc >= 0 ) 
			{
				UINT stride = 0;
				void* ptr = pAttrib->m_pData;
				if ( pAttrib->m_iOffset >= 0 ) 
				{
					stride = pVertexOwner->m_iVertexStride;
					ptr = (void*)(pAttrib->m_iOffset);
				}

				if ( pAttrib->m_iType == 0 ) pShader->SetAttribFloat( shaderLoc, pAttrib->m_iComponents, stride, (float*)ptr );
				else pShader->SetAttribUByte( shaderLoc, pAttrib->m_iComponents, stride, pAttrib->m_bNormalize, (unsigned char*)ptr );
			}
		}

		if ( !shadow && pVertexOwner->m_pDummyAttribs && i == 0 )
		{
			agk::PlatformBindBuffer( 0 );
			cDummyAttrib *pAttrib = pVertexOwner->m_pDummyAttribs;
			while( pAttrib )
			{
				int shaderLoc = pAttrib->m_iShaderLoc;
				if ( m_pSharedVertices ) shaderLoc = pShader->GetAttribByName( pAttrib->m_sName );
				if ( shaderLoc >= 0 )
				{
					// ATI doesn't seem to like glVertexAttrib4fv as a substitute for vertex arrays
					//glVertexAttrib4fv( pAttrib->m_iShaderLoc, &(pAttrib->m_pData[0]) );
					pShader->SetAttribUByte( shaderLoc, 4, 0, false, pAttrib->m_pData );
				}
				pAttrib = pAttrib->m_pNextAttrib;
			}
		}

		// Draw
		if ( pVertexOwner->m_iVBOIndices && pVertexOwner->m_iVBOIndices[ i ] ) 
		{
			agk::PlatformBindIndexBuffer( pVertexOwner->m_iVBOIndices[ i ] );
			
			if ( !shadow )
			{
				if ( m_iPrimitiveType == AGK_TRIANGLES ) agk::m_iPolygonsDrawn += pVertexOwner->m_iNumIndices[ i ] / 3;
				else if ( m_iPrimitiveType == AGK_TRIANGLE_STRIP ) agk::m_iPolygonsDrawn += pVertexOwner->m_iNumIndices[ i ] - 2;
			}
			else
			{
				if ( m_iPrimitiveType == AGK_TRIANGLES ) agk::m_iShadowPolygonsDrawn += pVertexOwner->m_iNumIndices[ i ] / 3;
				else if ( m_iPrimitiveType == AGK_TRIANGLE_STRIP ) agk::m_iShadowPolygonsDrawn += pVertexOwner->m_iNumIndices[ i ] - 2;
			}

			if ( m_iFlags & AGK_MESH_UINT_INDICES )
			{
				//pShader->DrawIndicesInt( pVertexOwner->m_iNumIndices[ i ], (unsigned int*)(pVertexOwner->m_ppIndices[ i ]) );
				pShader->DrawIndicesInt( pVertexOwner->m_iNumIndices[ i ], 0, m_iPrimitiveType );
			}
			else
			{
				//pShader->DrawIndices( pVertexOwner->m_iNumIndices[ i ], (unsigned short*)(pVertexOwner->m_ppIndices[ i ]) );
				pShader->DrawIndices( pVertexOwner->m_iNumIndices[ i ], 0, m_iPrimitiveType );
			}

			int blend = 0;

			//for ( int j = 0; j < 20000; j++ )
			{
				// No changes - 280

				// Checks only - 250

				// VBO bind - 178
				//glBindBuffer( GL_ARRAY_BUFFER, 0 );
				//glBindBuffer( GL_ARRAY_BUFFER, pVertexOwner->m_iVBOVertices[ i ] );

				// Blend change - 104
				//if ( blend ) glEnable( GL_BLEND );
				//else glDisable( GL_BLEND );
				//blend = 1 - blend;

				// Single uniform bind - 102
				//pShader->m_bTextureChanged = 1;

				// Matrix uniform bind - 85
				
				// texture bind - 61
				//glActiveTexture( GL_TEXTURE0 + 0 );
				//glBindTexture( GL_TEXTURE_2D, 0 );
				//glBindTexture( GL_TEXTURE_2D, m_pImage[0]->GetTextureID() );

				// Shader bind - 27
				//glUseProgram( 0 );
				//glUseProgram( pShader->m_iShaderID );

				//glDrawElements( GL_TRIANGLES, pVertexOwner->m_iNumIndices[ i ], GL_UNSIGNED_SHORT, pVertexOwner->m_ppIndices[ i ] );
				//pShader->DrawIndices( pVertexOwner->m_iNumIndices[ i ], pVertexOwner->m_ppIndices[ i ] );
			}
		}
		else 
		{
			agk::PlatformBindIndexBuffer( 0 );

			if ( !shadow ) agk::m_iPolygonsDrawn += pVertexOwner->m_iNumVertices[ i ] / 3;
			else agk::m_iShadowPolygonsDrawn += pVertexOwner->m_iNumVertices[ i ] / 3; 

			pShader->DrawTriangles( 0, pVertexOwner->m_iNumVertices[ i ] );
		}
	}
}

// shaders
void AGKShader::PlatformInit() 
{ 
	m_iShaderID = glCreateProgram();
}

void AGKShader::PlatformDelete() 
{ 
	if ( m_iShaderID ) glDeleteProgram( m_iShaderID ); 
	m_iShaderID = 0;
}

void AGKShader::MakeActive()
{
	if ( g_pCurrentShader != this ) 
	{
		// record which attribs are currently on
		if ( g_pCurrentShader ) 
		{
			cShaderAttrib *pAttrib = g_pCurrentShader->m_cAttribList.GetFirst();
			while( pAttrib )
			{
				if ( pAttrib->m_iLocation >= 0 && g_iAttributeActive[ pAttrib->m_iLocation ] > 0 ) 
				{
					g_iAttributeActive[ pAttrib->m_iLocation ] = 2;
					m_bFlags |= AGK_SHADER_ATTRIBUTES_CHANGED;
				}
				pAttrib = g_pCurrentShader->m_cAttribList.GetNext();
			}
		}

		// change shader
		glUseProgram( m_iShaderID );
	}
	g_pCurrentShader = this;

	g_pCurrentShader->UpdateUniforms();

	UpdateMatrices();
	UpdateAGKUniforms();

	// set global light variables
	if ( agk::m_cDirectionalLight.m_active )
	{
		g_pCurrentShader->SetTempConstantByName( "agk_DLightDir", agk::m_cDirectionalLight.m_direction.x, agk::m_cDirectionalLight.m_direction.y, agk::m_cDirectionalLight.m_direction.z, 0 );
		g_pCurrentShader->SetTempConstantByName( "agk_DLightColor", agk::m_cDirectionalLight.m_color.x, agk::m_cDirectionalLight.m_color.y, agk::m_cDirectionalLight.m_color.z, 0 );
	}
	else
	{
		g_pCurrentShader->SetTempConstantByName( "agk_DLightColor", 0, 0, 0, 0 );
	}

	g_pCurrentShader->SetTempConstantByName( "agk_LightAmbient", agk::m_AmbientColor.x, agk::m_AmbientColor.y, agk::m_AmbientColor.z, 0 );
}

void AGKShader::NoShader()
{
	if ( g_pCurrentShader ) 
	{
		for ( int i = 0; i < AGK_MAX_ATTRIBUTES; i++ )
		{
			if ( g_iAttributeActive[ i ] > 0 ) glDisableVertexAttribArray( i );
			g_iAttributeActive[ i ] = 0;
		}
		glUseProgram( 0 );
	}
	g_pCurrentShader = 0;
}

void AGKShader::SetAttribFloat( UINT location, int size, int stride, const float* ptr )
{
	if ( g_pCurrentShader != this ) MakeActive();
	glVertexAttribPointer( location, size, GL_FLOAT, GL_FALSE, stride, (void*) ptr );
	if ( g_iAttributeActive[ location ] == 0 ) glEnableVertexAttribArray( location );
	g_iAttributeActive[ location ] = 1;
}

void AGKShader::SetAttribUByte( UINT location, int size, int stride, bool normalise, const unsigned char* ptr )
{
	if ( g_pCurrentShader != this ) MakeActive();
	glVertexAttribPointer( location, size, GL_UNSIGNED_BYTE, normalise ? GL_TRUE : GL_FALSE, stride, (void*) ptr );
	if ( g_iAttributeActive[ location ] == 0 ) glEnableVertexAttribArray( location );
	g_iAttributeActive[ location ] = 1;
}

void AGKShader::PlatformDrawPrimitives( UINT primitive, int first, UINT count )
{
	// set old attributes off if they are no longer used
	if ( (m_bFlags & AGK_SHADER_ATTRIBUTES_CHANGED) != 0 )
	{
		for ( int i = 0; i < AGK_MAX_ATTRIBUTES; i++ )
		{
			if ( g_iAttributeActive[ i ] == 2 ) 
			{
				glDisableVertexAttribArray( i );
				g_iAttributeActive[ i ] = 0;
			}
		}

		m_bFlags &= ~AGK_SHADER_ATTRIBUTES_CHANGED;
	}

	GLenum glprimitive = GL_TRIANGLES;
	switch( primitive )
	{
		case AGK_TRIANGLES: glprimitive = GL_TRIANGLES; break;
		case AGK_TRIANGLE_STRIP: glprimitive = GL_TRIANGLE_STRIP; break;
		case AGK_TRIANGLE_FAN: glprimitive = GL_TRIANGLE_FAN; break;
		case AGK_LINE_LOOP: glprimitive = GL_LINE_LOOP; break;
		case AGK_LINES: glprimitive = GL_LINES; break;
		case AGK_POINTS: glprimitive = GL_POINTS; break;
	}

	glDrawArrays( glprimitive, first, count ); 
}

void AGKShader::PlatformDrawIndices( UINT primitive, UINT count, unsigned short *pIndices )
{
	// set old attributes off if they are no longer used
	if ( (m_bFlags & AGK_SHADER_ATTRIBUTES_CHANGED) != 0 )
	{
		for ( int i = 0; i < AGK_MAX_ATTRIBUTES; i++ )
		{
			if ( g_iAttributeActive[ i ] == 2 ) 
			{
				glDisableVertexAttribArray( i );
				g_iAttributeActive[ i ] = 0;
			}
		}

		m_bFlags &= ~AGK_SHADER_ATTRIBUTES_CHANGED;
	}

	GLenum glprimitive = GL_TRIANGLES;
	switch( primitive )
	{
		case AGK_TRIANGLES: glprimitive = GL_TRIANGLES; break;
		case AGK_TRIANGLE_STRIP: glprimitive = GL_TRIANGLE_STRIP; break;
		case AGK_TRIANGLE_FAN: glprimitive = GL_TRIANGLE_FAN; break;
		case AGK_LINE_LOOP: glprimitive = GL_LINE_LOOP; break;
		case AGK_LINES: glprimitive = GL_LINES; break;
		case AGK_POINTS: glprimitive = GL_POINTS; break;
	}

	glDrawElements( glprimitive, count, GL_UNSIGNED_SHORT, pIndices );
}

void AGKShader::PlatformDrawIndicesInt( UINT primitive, UINT count, unsigned int *pIndices )
{
	// set old attributes off if they are no longer used
	if ( (m_bFlags & AGK_SHADER_ATTRIBUTES_CHANGED) != 0 )
	{
		for ( int i = 0; i < AGK_MAX_ATTRIBUTES; i++ )
		{
			if ( g_iAttributeActive[ i ] == 2 ) 
			{
				glDisableVertexAttribArray( i );
				g_iAttributeActive[ i ] = 0;
			}
		}

		m_bFlags &= ~AGK_SHADER_ATTRIBUTES_CHANGED;
	}

	GLenum glprimitive = GL_TRIANGLES;
	switch( primitive )
	{
		case AGK_TRIANGLES: glprimitive = GL_TRIANGLES; break;
		case AGK_TRIANGLE_STRIP: glprimitive = GL_TRIANGLE_STRIP; break;
		case AGK_TRIANGLE_FAN: glprimitive = GL_TRIANGLE_FAN; break;
		case AGK_LINE_LOOP: glprimitive = GL_LINE_LOOP; break;
		case AGK_LINES: glprimitive = GL_LINES; break;
		case AGK_POINTS: glprimitive = GL_POINTS; break;
	}

	glDrawElements( glprimitive, count, GL_UNSIGNED_INT, pIndices );
}

void AGKShader::SetTempConstantByName( const char* szName, float f1, float f2, float f3, float f4 )
{
	if ( !szName ) return;
	if ( g_pCurrentShader != this ) MakeActive(); // can only set temp variables whilst the shader is active

	cShaderUniform *pUniform = m_cUniformList.GetItem( szName );
	if ( !pUniform ) return;
	if ( pUniform->m_iType != 0 )
	{
		agk::Error( "Failed to set shader variable - tried to set vector values on a matrix" );
		return;
	}

	switch( pUniform->m_iComponents )
	{
		case 1: glUniform1f( pUniform->m_iLocation, f1 ); break;
		case 2: glUniform2f( pUniform->m_iLocation, f1, f2 ); break;
		case 3: glUniform3f( pUniform->m_iLocation, f1, f2, f3 ); break;
		case 4: glUniform4f( pUniform->m_iLocation, f1, f2, f3, f4 ); break;
	}

	// set it as changed so the default value gets put back later
	if ( !pUniform->m_bChanged )
	{
		pUniform->m_bChanged = true;
		pUniform->m_pNextUniform = m_pChangedUniforms;
		m_pChangedUniforms = pUniform;
	}
}

void AGKShader::SetTempConstantArrayByName( const char* szName, UINT index, float f1, float f2, float f3, float f4 )
{
	if ( !szName ) return;
	if ( g_pCurrentShader != this ) MakeActive(); // can only set temp variables whilst the shader is active

	cShaderUniform *pUniform = m_cUniformList.GetItem( szName );
	if ( !pUniform ) return;
	if ( pUniform->m_iType != 0 )
	{
		agk::Error( "Failed to set shader Constant - tried to set vector values on a matrix" );
		return;
	}

	if ( index >= pUniform->m_iArrayMembers ) return;

	switch( pUniform->m_iComponents )
	{
		case 1: glUniform1f( pUniform->m_iLocation + index, f1 ); break;
		case 2: glUniform2f( pUniform->m_iLocation + index, f1, f2 ); break;
		case 3: glUniform3f( pUniform->m_iLocation + index, f1, f2, f3 ); break;
		case 4: glUniform4f( pUniform->m_iLocation + index, f1, f2, f3, f4 ); break;
	}

	// set it as changed so the default value gets put back later
	if ( !pUniform->m_bChanged )
	{
		pUniform->m_bChanged = true;
		pUniform->m_pNextUniform = m_pChangedUniforms;
		m_pChangedUniforms = pUniform;
	}
}

void AGKShader::SetTempConstantArrayByName( const char* szName, const float* values )
{
	if ( !szName ) return;
	if ( g_pCurrentShader != this ) MakeActive(); // can only set temp variables whilst the shader is active

	cShaderUniform *pUniform = m_cUniformList.GetItem( szName );
	if ( !pUniform ) return;
	if ( pUniform->m_iType != 0 )
	{
		agk::Error( "Failed to set shader variable - tried to set vector values on a matrix" );
		return;
	}

	switch( pUniform->m_iComponents )
	{
		case 1: glUniform1fv( pUniform->m_iLocation, pUniform->m_iArrayMembers, values ); break;
		case 2: glUniform2fv( pUniform->m_iLocation, pUniform->m_iArrayMembers, values ); break;
		case 3: glUniform3fv( pUniform->m_iLocation, pUniform->m_iArrayMembers, values ); break;
		case 4: glUniform4fv( pUniform->m_iLocation, pUniform->m_iArrayMembers, values ); break;
	}

	// set it as changed so the default value gets put back later
	if ( !pUniform->m_bChanged )
	{
		pUniform->m_bChanged = true;
		pUniform->m_pNextUniform = m_pChangedUniforms;
		m_pChangedUniforms = pUniform;
	}
}

void AGKShader::SetTempConstantMatrixByName( const char* szName, const float* values )
{
	if ( !szName ) return;
	if ( g_pCurrentShader != this ) MakeActive(); // can only set temp variables whilst the shader is active

	cShaderUniform *pUniform = m_cUniformList.GetItem( szName );
	if ( !pUniform ) return;
	if ( pUniform->m_iType != 1 )
	{
		agk::Error( "Failed to set shader variable - tried to set matrix values on a vector" );
		return;
	}

	switch( pUniform->m_iComponents )
	{
		case 2: glUniformMatrix2fv( pUniform->m_iLocation, 1, GL_FALSE, values ); break;
		case 3: glUniformMatrix3fv( pUniform->m_iLocation, 1, GL_FALSE, values ); break;
		case 4: glUniformMatrix4fv( pUniform->m_iLocation, 1, GL_FALSE, values ); break;
	}

	// set it as changed so the default value gets put back later
	if ( !pUniform->m_bChanged )
	{
		pUniform->m_bChanged = true;
		pUniform->m_pNextUniform = m_pChangedUniforms;
		m_pChangedUniforms = pUniform;
	}
}

void AGKShader::SetTempConstantMatrixArrayByName( const char* szName, UINT index, const float* values )
{
	if ( !szName ) return;
	if ( g_pCurrentShader != this ) MakeActive(); // can only set temp variables whilst the shader is active

	cShaderUniform *pUniform = m_cUniformList.GetItem( szName );
	if ( !pUniform ) return;
	if ( pUniform->m_iType != 1 )
	{
		agk::Error( "Failed to set shader variable - tried to set matrix values on a vector" );
		return;
	}

	if ( index >= pUniform->m_iArrayMembers ) return;

	switch( pUniform->m_iComponents )
	{
		case 2: glUniformMatrix2fv( pUniform->m_iLocation + index, 1, GL_FALSE, values ); break;
		case 3: glUniformMatrix3fv( pUniform->m_iLocation + index, 1, GL_FALSE, values ); break;
		case 4: glUniformMatrix4fv( pUniform->m_iLocation + index, 1, GL_FALSE, values ); break;
	}

	// set it as changed so the default value gets put back later
	if ( !pUniform->m_bChanged )
	{
		pUniform->m_bChanged = true;
		pUniform->m_pNextUniform = m_pChangedUniforms;
		m_pChangedUniforms = pUniform;
	}
}

void AGKShader::UpdateUniforms()
{
	cShaderUniform *pLast = 0;
	cShaderUniform *pUniform = m_pChangedUniforms;
	while ( pUniform )
	{
		if ( pUniform->m_iType == 0 )
		{
			// scalar/vector
			switch( pUniform->m_iComponents )
			{
				case 1: glUniform1fv( pUniform->m_iLocation, pUniform->m_iArrayMembers, pUniform->m_pValues ); break;
				case 2: glUniform2fv( pUniform->m_iLocation, pUniform->m_iArrayMembers, pUniform->m_pValues ); break;
				case 3: glUniform3fv( pUniform->m_iLocation, pUniform->m_iArrayMembers, pUniform->m_pValues ); break;
				case 4: glUniform4fv( pUniform->m_iLocation, pUniform->m_iArrayMembers, pUniform->m_pValues ); break;
			}
		}
		else
		{
			// matrix
			switch( pUniform->m_iComponents )
			{
				case 2: glUniformMatrix2fv( pUniform->m_iLocation, pUniform->m_iArrayMembers, GL_FALSE, pUniform->m_pValues ); break;
				case 3: glUniformMatrix3fv( pUniform->m_iLocation, pUniform->m_iArrayMembers, GL_FALSE, pUniform->m_pValues ); break;
				case 4: glUniformMatrix4fv( pUniform->m_iLocation, pUniform->m_iArrayMembers, GL_FALSE, pUniform->m_pValues ); break;
			}
		}
		
		pLast = pUniform;
		pUniform = pUniform->m_pNextUniform;
		
		// remove it from the changed list
		pLast->m_pNextUniform = 0;
		pLast->m_bChanged = false;
	}

	// clear changed list
	m_pChangedUniforms = 0;
}

void AGKShader::UpdateAGKUniforms()
{
	if ( agk::m_bUsingFBO ) glFrontFace( GL_CW );
	else glFrontFace( GL_CCW );

	if ( m_iAGKTime >= 0 ) glUniform1f( m_iAGKTime, agk::Timer() );
	if ( m_iAGKSinTime >= 0 ) glUniform1f( m_iAGKSinTime, agk::SinRad(agk::Timer()) );
	if ( m_iAGKResolution >= 0 ) 
	{
		if ( agk::m_bUsingFBO ) glUniform2f( m_iAGKResolution, (float) agk::m_iFBOWidth, (float) agk::m_iFBOHeight );
		else glUniform2f( m_iAGKResolution, (float) agk::GetDeviceWidth(), (float) agk::GetDeviceHeight() );
	}
	if ( m_iAGKInvert >= 0 ) 
	{
		if ( agk::m_bUsingFBO ) glUniform1f( m_iAGKInvert, -1 );
		else glUniform1f( m_iAGKInvert, 1 );
	}
	if ( m_iAGKCameraPos >= 0 && agk::m_pCurrentCamera ) glUniform3f( m_iAGKCameraPos, agk::m_pCurrentCamera->GetX(), agk::m_pCurrentCamera->GetY(), agk::m_pCurrentCamera->GetZ() );
	if ( m_iAGKShadowParams >= 0 ) 
	{
		if ( agk::m_fShadowRange > 0 ) glUniform4f( m_iAGKShadowParams, agk::m_fShadowRange, agk::m_fShadowBias, 1.0f/agk::m_iShadowMapWidth, 1.0f/agk::m_iShadowMapHeight );
		else glUniform4f( m_iAGKShadowParams, agk::m_pCurrentCamera->GetFarRange(), agk::m_fShadowBias, 1.0f/agk::m_iShadowMapWidth, 1.0f/agk::m_iShadowMapHeight );
	}
	if ( m_iAGKShadowParams2 >= 0 ) glUniform4f( m_iAGKShadowParams2, g_fShadowParams2[0], g_fShadowParams2[1], g_fShadowParams2[2], g_fShadowParams2[3] );
}

void AGKShader::UpdateMatrices()
{
	if ( agk::m_pCurrentCamera && (agk::m_pCurrentCamera->HasUpdated() || agk::m_pCurrentCamera != m_pCurrentCamera) )
	{
		AGKMatrix4 view;
		agk::m_pCurrentCamera->GetViewMatrix( view );
		SetViewMatrix( view.GetFloatPtr() );
	}

	if ( agk::m_pCurrentCamera && (agk::m_pCurrentCamera->HasProjUpdated() || agk::m_pCurrentCamera != m_pCurrentCamera) )
	{
		const AGKMatrix4 *proj = agk::m_pCurrentCamera->GetProjMatrix();
		SetProjMatrix( &(proj->mat[0][0]) );

		m_pCurrentCamera = agk::m_pCurrentCamera;
	}

	// check if anything has changed
	bool bWorldChanged = (m_bFlags & AGK_SHADER_WORLD_CHANGED) != 0;
	bool bViewChanged = (m_bFlags & AGK_SHADER_VIEW_CHANGED) != 0;
	bool bProjChanged = (m_bFlags & AGK_SHADER_PROJ_CHANGED) != 0;
	bool bOrthoChanged = (m_bFlags & AGK_SHADER_ORTHO_CHANGED) != 0;
	bool bNormChanged = (m_bFlags & AGK_SHADER_WORLDNORMAL_CHANGED) != 0;
	bool bShadowChanged = (m_bFlags & AGK_SHADER_SHADOW_PROJ_CHANGED) != 0;
	
	// check if we should update everything anyway
	if ( (m_bFlags & AGK_SHADER_RELOAD_UNIFORMS) != 0 )
	{
		bWorldChanged = true;
		bViewChanged = true;
		bProjChanged = true;
		bOrthoChanged = true;
		bNormChanged = true;
		bShadowChanged = true;

		// texture samplers only need to be updated once, do it here for simplicity
		for ( int i = 0; i < AGK_MAX_TEXTURES; i++ )
		{
			glUniform1i( m_iTexture2D[ i ], i );
			glUniform1i( m_iTextureCube[ i ], i );
		}

		if ( m_iShadowMapTex >= 0 ) glUniform1i( m_iShadowMapTex, 7 );
		if ( m_iShadowMap2Tex >= 0 ) glUniform1i( m_iShadowMap2Tex, 6 );
		if ( m_iShadowMap3Tex >= 0 ) glUniform1i( m_iShadowMap3Tex, 5 );
		if ( m_iShadowMap4Tex >= 0 ) glUniform1i( m_iShadowMap4Tex, 4 );
		
		m_bTextureBoundsChanged = 0xffffffff;
		m_bUVBoundsChanged = 0xffffffff;

		m_bFlags &= ~AGK_SHADER_RELOAD_UNIFORMS;
	}

	// check if texture UV offsets changed
	for ( int i = 0; i < AGK_MAX_TEXTURES; i++ )
	{
		UINT index = 1 << i;
		if ( (m_bTextureBoundsChanged & index) && m_iTextureBounds[ i ] >= 0 )
		{
			glUniform4f( m_iTextureBounds[ i ], m_fTextureU2[ i ]-m_fTextureU1[ i ], m_fTextureV2[ i ]-m_fTextureV1[ i ], m_fTextureU1[ i ], m_fTextureV1[ i ] );
		}
		m_bTextureBoundsChanged &= ~index;

		if ( (m_bUVBoundsChanged & index) && m_iUVBounds[ i ] >= 0 )
		{
			glUniform4f( m_iUVBounds[ i ], m_fU2[ i ]-m_fU1[ i ], m_fV2[ i ]-m_fV1[ i ], m_fU1[ i ], m_fV1[ i ] );
		}
		m_bUVBoundsChanged &= ~index;
	}

	if ( m_iUniformWorldMat >= 0 && bWorldChanged ) glUniformMatrix4fv( m_iUniformWorldMat, 1, GL_FALSE, &m_matWorld.mat[0][0] );
	if ( m_iUniformViewMat >= 0 && bViewChanged ) glUniformMatrix4fv( m_iUniformViewMat, 1, GL_FALSE, &m_matView.mat[0][0] );
	if ( bProjChanged && m_iUniformProjMat >= 0 ) glUniformMatrix4fv( m_iUniformProjMat, 1, GL_FALSE, &m_matProj.mat[0][0] );
	if ( bOrthoChanged && m_iUniformOrthoMat >= 0 ) glUniformMatrix4fv( m_iUniformOrthoMat, 1, GL_FALSE, &g_matOrtho.mat[0][0] );
	if ( m_iUniformNormalMat >= 0 && bNormChanged ) glUniformMatrix3fv( m_iUniformNormalMat, 1, GL_FALSE, &m_matWorldNormal.mat[0][0] );
	if ( m_iUniformShadowProjMat >= 0 && bShadowChanged ) glUniformMatrix4fv( m_iUniformShadowProjMat, 1, GL_FALSE, &g_matShadowProj.mat[0][0] );
	if ( m_iUniformShadowProj2Mat >= 0 && bShadowChanged ) glUniformMatrix4fv( m_iUniformShadowProj2Mat, 1, GL_FALSE, &g_matShadow2Proj.mat[0][0] );
	if ( m_iUniformShadowProj3Mat >= 0 && bShadowChanged ) glUniformMatrix4fv( m_iUniformShadowProj3Mat, 1, GL_FALSE, &g_matShadow3Proj.mat[0][0] );
	if ( m_iUniformShadowProj4Mat >= 0 && bShadowChanged ) glUniformMatrix4fv( m_iUniformShadowProj4Mat, 1, GL_FALSE, &g_matShadow4Proj.mat[0][0] );

	if ( m_iUniformVPMat >= 0 && (bViewChanged || bProjChanged) ) 
	{
		AGKMatrix4 vp = m_matView;
		vp.Mult( m_matProj );
		glUniformMatrix4fv( m_iUniformVPMat, 1, GL_FALSE, vp.GetFloatPtr() );
	}

	if ( m_iUniformWVPMat >= 0 && (bWorldChanged || bViewChanged || bProjChanged) ) 
	{
		AGKMatrix4 wvp = m_matWorld;
		wvp.Mult( m_matView );
		wvp.Mult( m_matProj );
		glUniformMatrix4fv( m_iUniformWVPMat, 1, GL_FALSE, &wvp.mat[0][0] );
	}

	if ( m_iUniformWVOMat >= 0 && (bWorldChanged || bOrthoChanged) ) 
	{
		AGKMatrix4 wvo = m_matWorld;
		wvo.Mult( g_matOrtho );
		glUniformMatrix4fv( m_iUniformWVOMat, 1, GL_FALSE, &wvo.mat[0][0] );
	}

	UINT mask = AGK_SHADER_WORLD_CHANGED | AGK_SHADER_VIEW_CHANGED | AGK_SHADER_PROJ_CHANGED 
		      | AGK_SHADER_WORLDNORMAL_CHANGED | AGK_SHADER_ORTHO_CHANGED | AGK_SHADER_SHADOW_PROJ_CHANGED;
	m_bFlags &= ~mask;
}

int agk::PlatformSupportsPSHighp()
{
	// assume desktop platforms support highp
	return 1;
}

void AGKShader::SetShaderSource( const char* vertex, const char* fragment )
{
	GLint logLength=0, status=0;
	GLuint vertShader=0, fragShader=0;

	m_sVSSource.SetStr( vertex );
	m_sPSSource.SetStr( fragment );

	// desktop OpenGL doesn't support precision modifiers
	m_sVSSource.ReplaceStr( "mediump ", "" );
	m_sVSSource.ReplaceStr( "highp ", "" );
	m_sVSSource.ReplaceStr( "lowp ", "" );
	m_sVSSource.ReplaceStr( "precision float;", "" );

	m_sPSSource.ReplaceStr( "mediump ", "" );
	m_sPSSource.ReplaceStr( "highp ", "" );
	m_sPSSource.ReplaceStr( "lowp ", "" );
	m_sPSSource.ReplaceStr( "precision float;", "" );

	m_sVSLog.SetStr( "" );
	m_sPSLog.SetStr( "" );

	char * vertex2 = new char[ m_sVSSource.GetLength() + 100 ];
	strcpy( vertex2, "" );
	if ( strstr( vertex, "#version" ) == 0 ) strcpy( vertex2, "#version 110\n" ); // for OpenGL 2.0
	strcat( vertex2, m_sVSSource.GetStr() );

	char* fragment2 = new char[ m_sPSSource.GetLength() + 100 ];
	strcpy( fragment2, "" );
	if ( strstr( fragment, "#version" ) == 0 ) strcpy( fragment2, "#version 110\n" ); // for OpenGL 2.0
	strcat( fragment2, m_sPSSource.GetStr() );

	// vertex shader
	vertShader = glCreateShader ( GL_VERTEX_SHADER );
	glShaderSource ( vertShader, 1, (const GLchar**)&vertex2, NULL );
	glCompileShader ( vertShader );

	delete [] vertex2;

	glGetShaderiv ( vertShader, GL_COMPILE_STATUS, &status );
	if ( status == 0 )
	{
		glGetShaderiv ( vertShader, GL_INFO_LOG_LENGTH, &logLength );
		if ( logLength > 1 )
		{
			GLchar *log = (GLchar*)malloc(logLength);
			glGetShaderInfoLog(vertShader, logLength, &logLength, log);
			m_sVSLog.SetStr( log );
			free(log);
		}

		glDeleteShader ( vertShader );
		uString info;
		info.Format( "Vertex shader %s failed to compile: %s", m_sVSFilename.GetStr(), m_sVSLog.GetStr() );
		agk::Error( info );
		agk::Message( info );
		m_bValid = false;
		return;
	}

	// fragment shader
	fragShader = glCreateShader ( GL_FRAGMENT_SHADER );
	glShaderSource ( fragShader, 1, (const GLchar**)&fragment2, NULL );
	glCompileShader ( fragShader );

	delete [] fragment2;

	glGetShaderiv ( fragShader, GL_COMPILE_STATUS, &status );
	if ( status == 0 )
	{
		glGetShaderiv ( fragShader, GL_INFO_LOG_LENGTH, &logLength );
		if ( logLength > 1 )
		{
			GLchar *log = (GLchar*)malloc(logLength);
			glGetShaderInfoLog(fragShader, logLength, &logLength, log);
			m_sPSLog.SetStr( log );
			free(log);
		}

		glDeleteShader ( vertShader );
		glDeleteShader ( fragShader );
		uString info;
		info.Format( "Pixel shader %s failed to compile: %s", m_sPSFilename.GetStr(), m_sPSLog.GetStr() );
		agk::Error( info );
		agk::Message( info );
		m_bValid = false;
		return;
	}

	// attach shaders to program
	glAttachShader ( m_iShaderID, vertShader );
	glAttachShader ( m_iShaderID, fragShader );

	// link program
	glLinkProgram ( m_iShaderID );
	
	glGetProgramiv ( m_iShaderID, GL_LINK_STATUS, &status);
	if ( status == 0 )
	{
		if ( vertShader ) { glDeleteShader ( vertShader ); vertShader=0; }
		if ( fragShader ) { glDeleteShader ( fragShader ); fragShader=0; }

		glGetProgramiv ( m_iShaderID, GL_INFO_LOG_LENGTH, &logLength);
		if ( logLength > 0 )
		{
			GLchar *log = (GLchar*)malloc(logLength);
			glGetProgramInfoLog ( m_iShaderID, logLength, &logLength, log);
			m_sLinkLog.SetStr( log );
			free(log);
		}
		uString info;
		info.Format( "Vertex shader %s and pixel shader %s failed to link: %s", m_sVSFilename.GetStr(), m_sPSFilename.GetStr(), m_sLinkLog.GetStr() );
		agk::Error( info );
		agk::Message( info );
		m_bValid = false;
		return;
	}

	if ( m_bReloading )
	{
		if ( vertShader ) glDeleteShader ( vertShader );
		if ( fragShader ) glDeleteShader ( fragShader );
		m_bValid = true;
		return;
	}

	m_pChangedUniforms = 0;
	cShaderUniform* pUniform = m_cUniformList.GetFirst();
	while ( pUniform )
	{
		delete pUniform;
		pUniform = m_cUniformList.GetNext();
	}
	m_cUniformList.ClearAll();

	cShaderAttrib* pAttrib = m_cAttribList.GetFirst();
	while ( pAttrib )
	{
		delete pAttrib;
		pAttrib = m_cAttribList.GetNext();
	}
	m_cAttribList.ClearAll();

	// get uniform locations
	m_iUniformWorldMat = glGetUniformLocation ( m_iShaderID, "agk_World" );
	m_iUniformNormalMat = glGetUniformLocation ( m_iShaderID, "agk_WorldNormal" );
	m_iUniformViewMat = glGetUniformLocation ( m_iShaderID, "agk_View" );
	m_iUniformProjMat = glGetUniformLocation ( m_iShaderID, "agk_Proj" );
	m_iUniformOrthoMat = glGetUniformLocation ( m_iShaderID, "agk_Ortho" );
	m_iUniformVPMat = glGetUniformLocation ( m_iShaderID, "agk_ViewProj" );
	m_iUniformWVPMat = glGetUniformLocation ( m_iShaderID, "agk_WorldViewProj" );
	m_iUniformWVOMat = glGetUniformLocation ( m_iShaderID, "agk_WorldOrtho" );
	m_iUniformShadowProjMat = glGetUniformLocation ( m_iShaderID, "agk_ShadowProj" );
	m_iUniformShadowProj2Mat = glGetUniformLocation ( m_iShaderID, "agk_Shadow2Proj" );
	m_iUniformShadowProj3Mat = glGetUniformLocation ( m_iShaderID, "agk_Shadow3Proj" );
	m_iUniformShadowProj4Mat = glGetUniformLocation ( m_iShaderID, "agk_Shadow4Proj" );

	int numUniforms = 0;
	int maxLength = 0;
	int length = 0;
	int size = 0;
	UINT type = 0;
	glGetProgramiv( m_iShaderID, GL_ACTIVE_UNIFORMS, &numUniforms );
	glGetProgramiv( m_iShaderID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength );
	char* szUniformName = new char[ maxLength+1 ];
	
	m_iNumUniforms = 0;
	for ( int i = 0; i < numUniforms; i++ )
	{
		glGetActiveUniform( m_iShaderID, i, maxLength, &length, &size, &type, szUniformName );
		int location = glGetUniformLocation( m_iShaderID, szUniformName );

		int newSize = 0;
		int newType = 0;
		switch( type )
		{
			case GL_FLOAT: newSize = 1; break;
			case GL_FLOAT_VEC2: newSize = 2; break;
			case GL_FLOAT_VEC3: newSize = 3; break;
			case GL_FLOAT_VEC4: newSize = 4; break;
			case GL_FLOAT_MAT2: newSize = 2; newType = 1; break;
			case GL_FLOAT_MAT3: newSize = 3; newType = 1; break;
			case GL_FLOAT_MAT4: newSize = 4; newType = 1; break;
			default:
			{
				/*
				uString info;
				info.Format( "Shader variable \"%s\" is not supported. Only float, vec2, vec3, vec4 are currently allowed.", szUniformName );
				agk::Message( info );
				*/
				continue;
				break;
			}
		}

		cShaderUniform *pNewUniform = new cShaderUniform();
		pNewUniform->m_iComponents = newSize;
		pNewUniform->m_iLocation = location;
		pNewUniform->m_iType = newType;
		pNewUniform->m_iArrayMembers = size;
		pNewUniform->m_sName.SetStr( szUniformName );
		pNewUniform->m_sName.Trunc( '[' );
		if ( newType == 0 ) 
		{
			pNewUniform->m_pValues = new float[ size*newSize ];
			for ( int i = 0; i < size*newSize; i++ ) pNewUniform->m_pValues[ i ] = 0;
		}
		else 
		{
			pNewUniform->m_pValues = new float[ size*newSize*newSize ];
			for ( int i = 0; i < size*newSize*newSize; i++ ) pNewUniform->m_pValues[ i ] = 0;
		}
		
		m_cUniformList.AddItem( pNewUniform, pNewUniform->m_sName );
		m_iNumUniforms++;
	}

	delete [] szUniformName;

	m_iAGKTime = glGetUniformLocation( m_iShaderID, "agk_time" );
	m_iAGKSinTime = glGetUniformLocation( m_iShaderID, "agk_sintime" );
	m_iAGKResolution = glGetUniformLocation( m_iShaderID, "agk_resolution" );
	m_iAGKInvert = glGetUniformLocation( m_iShaderID, "agk_invert" );
	m_iAGKCameraPos = glGetUniformLocation( m_iShaderID, "agk_CameraPos" );
	m_iAGKShadowParams = glGetUniformLocation( m_iShaderID, "agk_ShadowParams" );
	m_iAGKShadowParams2 = glGetUniformLocation( m_iShaderID, "agk_ShadowParams2" );

	if ( glGetUniformLocation( m_iShaderID, "agk_spritepos" ) >= 0 ) m_bFlags |= AGK_SHADER_PER_SPRITE_UNIFORM;
	if ( glGetUniformLocation( m_iShaderID, "agk_spritesize" ) >= 0 ) m_bFlags |= AGK_SHADER_PER_SPRITE_UNIFORM;
		
	uString name;
	for ( int i = 0; i < AGK_MAX_TEXTURES; i++ )
	{
		name.Format( "texture%d", i );
		m_iTexture2D[ i ] = glGetUniformLocation( m_iShaderID, name.GetStr() );
		name.Format( "CubeMap%d", i );
		m_iTextureCube[ i ] = glGetUniformLocation( m_iShaderID, name.GetStr() );
		name.Format( "uvBounds%d", i );
		m_iUVBounds[ i ] = glGetUniformLocation( m_iShaderID, name.GetStr() );
		name.Format( "textureBounds%d", i );
		m_iTextureBounds[ i ] = glGetUniformLocation( m_iShaderID, name.GetStr() );
	}

	m_iShadowMapTex = glGetUniformLocation( m_iShaderID, "shadowMap" );
	m_iShadowMap2Tex = glGetUniformLocation( m_iShaderID, "shadowMap2" );
	m_iShadowMap3Tex = glGetUniformLocation( m_iShaderID, "shadowMap3" );
	m_iShadowMap4Tex = glGetUniformLocation( m_iShaderID, "shadowMap4" );

	int numAttribs = 0;
	glGetProgramiv( m_iShaderID, GL_ACTIVE_ATTRIBUTES, &numAttribs );
	glGetProgramiv( m_iShaderID, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxLength );
	char* szAttribName = new char[ maxLength+1 ];
	for ( int i = 0; i < numAttribs; i++ )
	{
		glGetActiveAttrib( m_iShaderID, i, maxLength, &length, &size, &type, szAttribName );
		int location = glGetAttribLocation( m_iShaderID, szAttribName );
		int newSize = 0;
		int newType = 0;
		switch( type )
		{
			case GL_FLOAT: newSize = 1; break;
			case GL_FLOAT_VEC2: newSize = 2; break;
			case GL_FLOAT_VEC3: newSize = 3; break;
			case GL_FLOAT_VEC4: newSize = 4; break;
			case GL_FLOAT_MAT2: newSize = 2; newType = 1; break;
			case GL_FLOAT_MAT3: newSize = 3; newType = 1; break;
			case GL_FLOAT_MAT4: newSize = 4; newType = 1; break;
			default:
			{
				uString info;
				info.Format( "Shader attribute \"%s\" is not supported. Non-square matrices are a likely cause", szAttribName );
				agk::Message( info );
				continue;
				break;
			}
		}

		m_iNumAttribs++;

		if ( strcmp(szAttribName, "position") == 0 ) m_iPositionLoc = location;
		else if ( strcmp(szAttribName, "normal") == 0 ) m_iNormalLoc = location;
		else if ( strcmp(szAttribName, "tangent") == 0 ) m_iTangentLoc = location;
		else if ( strcmp(szAttribName, "binormal") == 0 ) m_iBiNormalLoc = location;
		else if ( strcmp(szAttribName, "color") == 0 ) m_iColorLoc = location;
		else if ( strcmp(szAttribName, "uv") == 0 ) m_iTexCoordLoc = location;
		else if ( strcmp(szAttribName, "boneweights") == 0 ) m_iBoneWeightsLoc = location;
		else if ( strcmp(szAttribName, "boneindices") == 0 ) m_iBoneIndicesLoc = location;

		cShaderAttrib *pNewAttrib = new cShaderAttrib();
		pNewAttrib->m_iComponents = newSize;
		pNewAttrib->m_iLocation = location;
		pNewAttrib->m_iType = newType;
		pNewAttrib->m_sName.SetStr( szAttribName );
		m_cAttribList.AddItem( pNewAttrib, szAttribName );
	}

	delete [] szAttribName;

	// release shaders now program created okay
	if ( vertShader ) glDeleteShader ( vertShader );
	if ( fragShader ) glDeleteShader ( fragShader );

	m_bValid = true;
}

// Frame Buffer Objects

void FrameBuffer::PlatformCreateFrameBuffer( cImage *pColor, cImage *pDepth, bool forceDepth )
{
	if ( m_iFBO != 0 ) PlatformDeleteFrameBuffer();
	
	glGenFramebuffers( 1, &m_iFBO );
	//glBindFramebuffer( GL_FRAMEBUFFER, m_iFBO );
	Bind();
	
	if ( pColor )
	{
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pColor->GetTextureID(), 0 );
	}
	else
    {
        glDrawBuffer( GL_NONE );
        glReadBuffer( GL_NONE );
    }

	if ( pDepth )
	{
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, pDepth->GetTextureID(), 0 );
	}
	else
	{
		if ( forceDepth )
		{
			glGenRenderbuffers( 1, &m_iRBODepth );
			glBindRenderbuffer( GL_RENDERBUFFER, m_iRBODepth );
			glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, pColor ? pColor->GetTotalWidth() : 32, pColor ? pColor->GetTotalHeight() : 32 );
			glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_iRBODepth );
		}
	}

	GLenum result = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if ( result != GL_FRAMEBUFFER_COMPLETE )
	{
		switch( result )
		{
			case GL_FRAMEBUFFER_UNSUPPORTED: agk::Message( "GL_FRAMEBUFFER_UNSUPPORTED" ); break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: agk::Message( "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" ); break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: agk::Message( "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" ); break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: agk::Message( "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" ); break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: agk::Message( "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" ); break;
			default: 
			{
				uString err = "Framebuffer error: ";
				err += result;
				agk::Message( err );
			}
		}
	}

	agk::BindDefaultFramebuffer();

	if ( !pColor )
	{
		glDrawBuffer( GL_BACK );
        glReadBuffer( GL_BACK );
	}
}

void FrameBuffer::PlatformDeleteFrameBuffer()
{
	if ( m_iFBO ) 
	{
		glDeleteFramebuffers( 1, &m_iFBO );
		m_iFBO = 0;
	}

	if ( m_iRBODepth )
	{
		glDeleteRenderbuffers( 1, &m_iRBODepth );
		m_iRBODepth = 0;
	}
}

void FrameBuffer::PlatformBind()
{
	glBindFramebuffer( GL_FRAMEBUFFER, m_iFBO );
}
