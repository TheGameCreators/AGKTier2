#include "agk.h"
#include <android/keycodes.h>
#include "interpreter.h"
#include <android/native_activity.h>
#include <jni.h>
#include <android/configuration.h>
#include <android_native_app_glue.h>
#include <android/log.h>
#include <EGL/egl.h>

#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

struct egldata {
    EGLDisplay display;
    EGLSurface surface;
    EGLContext context;
    struct ANativeActivity *activity;
    EGLint format;
    ANativeWindow* window;
};

ANativeActivity *g_pActivity = 0;

void PlatformAppQuit()
{
	//exit(0);

	jint lFlags = 0;
	JNIEnv* lJNIEnv = g_pActivity->env;
	JavaVM* vm = g_pActivity->vm;
	vm->AttachCurrentThread(&lJNIEnv, NULL);

	if ( !g_pActivity ) agk::Warning("Failed to get activity pointer");
	jobject lNativeActivity = g_pActivity->clazz;
	if ( !lNativeActivity ) agk::Warning("Failed to get native activity pointer");

	jclass classActivity = lJNIEnv->FindClass("android/app/NativeActivity");
	if ( !classActivity ) agk::Warning("Failed to get class NativeActivity");

	jmethodID getClassLoader = lJNIEnv->GetMethodID(classActivity, "getClassLoader", "()Ljava/lang/ClassLoader;");
	if( !getClassLoader ) agk::Warning("Exception occurred while getting getClassLoader methodId");

	jobject objClassLoader = lJNIEnv->CallObjectMethod(lNativeActivity, getClassLoader);
	if ( !objClassLoader ) agk::Warning("Exception occurred while getting class loader instance");

	jclass classLoader = lJNIEnv->FindClass("java/lang/ClassLoader");
	if ( !classLoader ) agk::Warning("Exception occurred while finding ClassLoader class definition");

	jmethodID methodFindClass = lJNIEnv->GetMethodID( classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
	if( !methodFindClass ) agk::Warning("Exception occurred while getting loadClass method id");

	// Get AGK Helper class
	jclass classHelper = (jclass)lJNIEnv->CallObjectMethod(objClassLoader, methodFindClass, lJNIEnv->NewStringUTF("com.thegamecreators.agk_player.AGKHelper"));
	if( !classHelper ) agk::Warning("Failed to get AGKHelper class");

	jmethodID method = lJNIEnv->GetStaticMethodID( classHelper, "QuitApp","(Landroid/app/Activity;)V");
	if ( !method ) agk::Warning("Failed to get method QuitApp");
	else lJNIEnv->CallStaticVoidMethod( classHelper, method, lNativeActivity );

	vm->DetachCurrentThread();
}

extern "C"
{
	bool g_bFirstLoop = true;
	int wasPlaying = 0;
	int wasVideoPlaying = 0;
	bool g_bIsTablet = false;

	// to skip calibration set g_bCalibrated to true and g_iCalibrationSetup to 2
	bool g_bCalibrated = true;
	int g_iCalibrationSetup = 2;

	// default to portrait accelerometer, detect landscape later, or override with calibration
	int g_iPortraitAxis = 1; // 0=X, 1=Y
	int g_iPortraitModifier = 1;
	int g_iLandscapeAxis = 0; // 0=X, 1=Y
	int g_iLandscapeModifier = -1;

	int getModel( ANativeActivity* pActivity, char *szOut )
	{
		if ( !szOut ) return 0;
		
		JNIEnv* lJNIEnv = pActivity->env;

		jint lFlags = 0;
		JavaVM* vm = pActivity->vm;
		bool attached = false;
		switch (vm->GetEnv((void**)&lJNIEnv, JNI_VERSION_1_6))
		{
			case JNI_OK: break;
			case JNI_EDETACHED:
			{
				vm->AttachCurrentThread(&lJNIEnv, NULL);
				break;
			}
			default: return 0;
		}

		jobject lNativeActivity = pActivity->clazz;
		jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);
		jboolean isCopy;

		// Retrieves Build class.
		jclass ClassContext = lJNIEnv->FindClass( "android/os/Build" );

		// Model
		jfieldID FieldMODEL = lJNIEnv->GetStaticFieldID( ClassContext, "MODEL", "Ljava/lang/String;" );
		jstring MODEL = (jstring) lJNIEnv->GetStaticObjectField( ClassContext, FieldMODEL );
		const char *szModel = lJNIEnv->GetStringUTFChars( MODEL, &isCopy );
		strcpy( szOut, szModel );
		lJNIEnv->ReleaseStringUTFChars( MODEL, szModel );

		vm->DetachCurrentThread();

		return 1;
	}

	int checkformat( ANativeActivity* pActivity )
	{
		int result = 0;

		JNIEnv* lJNIEnv = pActivity->env;

		jint lFlags = 0;
		JavaVM* vm = pActivity->vm;
		bool attached = false;
		switch (vm->GetEnv((void**)&lJNIEnv, JNI_VERSION_1_6))
		{
			case JNI_OK: break;
			case JNI_EDETACHED:
			{
				if (vm->AttachCurrentThread(&lJNIEnv, NULL)!=0)
				{
					agk::Warning("Could not attach current thread");
				}
				break;
			}
			case JNI_EVERSION: agk::Warning("Invalid java version");
		}

		jobject lNativeActivity = pActivity->clazz;
		jclass ClassNativeActivity = lJNIEnv->GetObjectClass(lNativeActivity);
		jboolean isCopy;

		// Retrieves Build class.
		jclass ClassContext = lJNIEnv->FindClass( "android/os/Build" );

		/*
		// Board
		jfieldID FieldBOARD = lJNIEnv->GetStaticFieldID( ClassContext, "BOARD", "Ljava/lang/String;" );
		jstring BOARD = (jstring) lJNIEnv->GetStaticObjectField( ClassContext, FieldBOARD );
		const char* szBoard = lJNIEnv->GetStringUTFChars( BOARD, &isCopy );
		agk::Warning( szBoard );

		// Brand
		FieldBOARD = lJNIEnv->GetStaticFieldID( ClassContext, "BRAND", "Ljava/lang/String;" );
		BOARD = (jstring) lJNIEnv->GetStaticObjectField( ClassContext, FieldBOARD );
		szBoard = lJNIEnv->GetStringUTFChars( BOARD, &isCopy );
		agk::Warning( szBoard );
		*/

		// Device
		jfieldID FieldDEVICE = lJNIEnv->GetStaticFieldID( ClassContext, "DEVICE", "Ljava/lang/String;" );
		jstring DEVICE = (jstring) lJNIEnv->GetStaticObjectField( ClassContext, FieldDEVICE );
		const char *szDevice = lJNIEnv->GetStringUTFChars( DEVICE, &isCopy );
		LOGW( "%s", szDevice );
		if ( strcmp( szDevice, "marvel" ) == 0 ) result = 1; //HTC Wildfire S
		lJNIEnv->ReleaseStringUTFChars( DEVICE, szDevice );

		/*
		// Display
		FieldBOARD = lJNIEnv->GetStaticFieldID( ClassContext, "DISPLAY", "Ljava/lang/String;" );
		BOARD = (jstring) lJNIEnv->GetStaticObjectField( ClassContext, FieldBOARD );
		szBoard = lJNIEnv->GetStringUTFChars( BOARD, &isCopy );
		agk::Warning( szBoard );

		// Hardware
		FieldBOARD = lJNIEnv->GetStaticFieldID( ClassContext, "HARDWARE", "Ljava/lang/String;" );
		BOARD = (jstring) lJNIEnv->GetStaticObjectField( ClassContext, FieldBOARD );
		szBoard = lJNIEnv->GetStringUTFChars( BOARD, &isCopy );
		agk::Warning( szBoard );
*/
		// Model
		jfieldID FieldMODEL = lJNIEnv->GetStaticFieldID( ClassContext, "MODEL", "Ljava/lang/String;" );
		jstring MODEL = (jstring) lJNIEnv->GetStaticObjectField( ClassContext, FieldMODEL );
		const char *szModel = lJNIEnv->GetStringUTFChars( MODEL, &isCopy );
		LOGW( "%s", szModel );
		lJNIEnv->ReleaseStringUTFChars( MODEL, szModel );
/*
		// PRODUCT
		FieldBOARD = lJNIEnv->GetStaticFieldID( ClassContext, "PRODUCT", "Ljava/lang/String;" );
		BOARD = (jstring) lJNIEnv->GetStaticObjectField( ClassContext, FieldBOARD );
		szBoard = lJNIEnv->GetStringUTFChars( BOARD, &isCopy );
		agk::Warning( szBoard );
		*/

		vm->DetachCurrentThread();

		// clear error message for interpreter
		agk::Warning( "" );
		return result;
	}

	void init( void *ptr )
	{
		agk::SetExtraAGKPlayerAssetsMode ( 2 );

		try
		{
			agk::InitGL(ptr);
		}
		catch(...)
		{
			uString err; agk::GetLastError(err);
			err.Prepend( "Error: " );
			agk::Message( err.GetStr() );
			PlatformAppQuit();
			return;
		}

		if ( App.g_dwDeviceWidth==0 )
		{
			App.g_dwDeviceWidth = agk::GetDeviceWidth();
			App.g_dwDeviceHeight = agk::GetDeviceHeight();
		}

		egldata *eptr = (egldata*)ptr;

		g_pActivity = eptr->activity;

		// (OLD) detect if this screen reports as a native landscape and modify accelerometer data to compensate
		/*
		AConfiguration *config = AConfiguration_new();
		AConfiguration_fromAssetManager( config, eptr->activity->assetManager);
		int screenlong = AConfiguration_getScreenLong( config );
		LOGW("Screen Long: %d", screenlong);
		if ( screenlong == ACONFIGURATION_SCREENLONG_NO )
		{
			g_iPortraitAxis = 0; // 0=X, 1=Y
			g_iPortraitModifier = -1;
			g_iLandscapeAxis = 1; // 0=X, 1=Y
			g_iLandscapeModifier = -1;
			g_bIsTablet = true;
		}
		AConfiguration_delete( config );
		*/

		// (NEW) detect native orientation code
		jint lFlags = 0;
		JNIEnv* lJNIEnv = g_pActivity->env;
		JavaVM* vm = g_pActivity->vm;
		vm->AttachCurrentThread(&lJNIEnv, NULL);

		if ( !g_pActivity ) agk::Warning("Failed to get activity pointer");
		jobject lNativeActivity = g_pActivity->clazz;
		if ( !lNativeActivity ) agk::Warning("Failed to get native activity pointer");

		jclass classActivity = lJNIEnv->FindClass("android/app/NativeActivity");
		if ( !classActivity ) agk::Warning("Failed to get class NativeActivity");

		jmethodID getClassLoader = lJNIEnv->GetMethodID(classActivity, "getClassLoader", "()Ljava/lang/ClassLoader;");
		if( !getClassLoader ) agk::Warning("Exception occurred while getting getClassLoader methodId");

		jobject objClassLoader = lJNIEnv->CallObjectMethod(lNativeActivity, getClassLoader);
		if ( !objClassLoader ) agk::Warning("Exception occurred while getting class loader instance");

		jclass classLoader = lJNIEnv->FindClass("java/lang/ClassLoader");
		if ( !classLoader ) agk::Warning("Exception occurred while finding ClassLoader class definition");

		jmethodID methodFindClass = lJNIEnv->GetMethodID( classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
		if( !methodFindClass ) agk::Warning("Exception occurred while getting loadClass method id");

		// Get AGK Helper class
		jclass classHelper = (jclass)lJNIEnv->CallObjectMethod(objClassLoader, methodFindClass, lJNIEnv->NewStringUTF("com.thegamecreators.agk_player.AGKHelper"));
		if( !classHelper ) agk::Warning("Failed to get AGKHelper class");

		jmethodID methodGetOrien = lJNIEnv->GetStaticMethodID( classHelper, "GetOrientation","(Landroid/app/Activity;)I");
		if ( !methodGetOrien ) agk::Warning("Failed to get method GetOrientation");

		int orien = lJNIEnv->CallStaticIntMethod( classHelper, methodGetOrien, lNativeActivity );

		vm->DetachCurrentThread();

		if ( agk::GetDeviceWidth() > agk::GetDeviceHeight() )
		{
			if (orien == 0 || orien == 2)
			{
				g_iPortraitAxis = 0; // 0=X, 1=Y
				g_iPortraitModifier = -1;
				g_iLandscapeAxis = 1; // 0=X, 1=Y
				g_iLandscapeModifier = -1;
				g_bIsTablet = true;
			}
		}
		else
		{
			if (orien == 1 || orien == 3)
			{
				g_iPortraitAxis = 0; // 0=X, 1=Y
				g_iPortraitModifier = -1;
				g_iLandscapeAxis = 1; // 0=X, 1=Y
				g_iLandscapeModifier = -1;
				g_bIsTablet = true;
			}
		}
	}

	void updateptr( void *ptr )
	{
		// fixes a crash on Ouya
		egldata *eptr = (egldata*)ptr;
		g_pActivity = eptr->activity;

		agk::UpdatePtr(ptr);
	}

	void updateptr2( void *ptr )
	{
		// fixes a crash on Ouya
		egldata *eptr = (egldata*)ptr;
		g_pActivity = eptr->activity;

		agk::UpdatePtr2(ptr);
	}

	void cleanup()
	{
		App.End();
		agk::CleanUp();
	}

	void devicerotate()
	{
		float aspect = agk::GetDisplayAspect();
		agk::SetVirtualResolution( agk::GetVirtualWidth(), agk::GetVirtualHeight() );
		agk::SetDisplayAspect( aspect );
	}

	void windowclosing()
	{
		agk::LosingContext();
	}

	void pauseapp()
	{
		agk::AppPausing();

		jint lFlags = 0;
		JNIEnv* lJNIEnv = g_pActivity->env;
		JavaVM* vm = g_pActivity->vm;
		vm->AttachCurrentThread(&lJNIEnv, NULL);

		if ( !g_pActivity ) agk::Warning("Failed to get activity pointer");
		jobject lNativeActivity = g_pActivity->clazz;
		if ( !lNativeActivity ) agk::Warning("Failed to get native activity pointer");

		jclass classActivity = lJNIEnv->FindClass("android/app/NativeActivity");
		if ( !classActivity ) agk::Warning("Failed to get class NativeActivity");

		jmethodID getClassLoader = lJNIEnv->GetMethodID(classActivity, "getClassLoader", "()Ljava/lang/ClassLoader;");
		if( !getClassLoader ) agk::Warning("Exception occurred while getting getClassLoader methodId");

		jobject objClassLoader = lJNIEnv->CallObjectMethod(lNativeActivity, getClassLoader);
		if ( !objClassLoader ) agk::Warning("Exception occurred while getting class loader instance");

		jclass classLoader = lJNIEnv->FindClass("java/lang/ClassLoader");
		if ( !classLoader ) agk::Warning("Exception occurred while finding ClassLoader class definition");

		jmethodID methodFindClass = lJNIEnv->GetMethodID( classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
		if( !methodFindClass ) agk::Warning("Exception occurred while getting loadClass method id");

		// Get AGK Helper class
		jclass classHelper = (jclass)lJNIEnv->CallObjectMethod(objClassLoader, methodFindClass, lJNIEnv->NewStringUTF("com.thegamecreators.agk_player.AGKHelper"));
		if( !classHelper ) agk::Warning("Failed to get AGKHelper class");

		jmethodID method = lJNIEnv->GetStaticMethodID( classHelper, "OnStop","(Landroid/app/Activity;)V");
		if ( !method ) agk::Warning("Failed to get method OnStop");
		else lJNIEnv->CallStaticVoidMethod( classHelper, method, lNativeActivity );

		vm->DetachCurrentThread();
	}

	void resumeapp()
	{
		agk::AppResuming();
		
		agk::Resumed();
	}

	void resumeapp2()
	{
		agk::ResumedOpenGL();
	}

	void onstart(void* ptr)
	{
		ANativeActivity *act = (ANativeActivity*) ptr;

		jint lFlags = 0;
		JNIEnv* lJNIEnv = act->env;
		JavaVM* vm = act->vm;
		vm->AttachCurrentThread(&lJNIEnv, NULL);

		if ( !act ) agk::Warning("Failed to get activity pointer");
		jobject lNativeActivity = act->clazz;
		if ( !lNativeActivity ) agk::Warning("Failed to get native activity pointer");

		jclass classActivity = lJNIEnv->FindClass("android/app/NativeActivity");
		if ( !classActivity ) agk::Warning("Failed to get class NativeActivity");

		jmethodID getClassLoader = lJNIEnv->GetMethodID(classActivity, "getClassLoader", "()Ljava/lang/ClassLoader;");
		if( !getClassLoader ) agk::Warning("Exception occurred while getting getClassLoader methodId");

		jobject objClassLoader = lJNIEnv->CallObjectMethod(lNativeActivity, getClassLoader);
		if ( !objClassLoader ) agk::Warning("Exception occurred while getting class loader instance");

		jclass classLoader = lJNIEnv->FindClass("java/lang/ClassLoader");
		if ( !classLoader ) agk::Warning("Exception occurred while finding ClassLoader class definition");

		jmethodID methodFindClass = lJNIEnv->GetMethodID( classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
		if( !methodFindClass ) agk::Warning("Exception occurred while getting loadClass method id");

		// Get AGK Helper class
		jclass classHelper = (jclass)lJNIEnv->CallObjectMethod(objClassLoader, methodFindClass, lJNIEnv->NewStringUTF("com.thegamecreators.agk_player.AGKHelper"));
		if( !classHelper ) agk::Warning("Failed to get AGKHelper class");

		jmethodID method = lJNIEnv->GetStaticMethodID( classHelper, "OnStart","(Landroid/app/Activity;)V");
		if ( !method ) agk::Warning("Failed to get method OnStart");
		else lJNIEnv->CallStaticVoidMethod( classHelper, method, lNativeActivity );

		jmethodID methodGetOrien = lJNIEnv->GetStaticMethodID( classHelper, "GetOrientation","(Landroid/app/Activity;)I");
		int orien = lJNIEnv->CallStaticIntMethod( classHelper, methodGetOrien, lNativeActivity );

		jmethodID GetDisplayWidth = lJNIEnv->GetStaticMethodID( classHelper, "GetDisplayWidth","(Landroid/app/Activity;)I" );
		int width = lJNIEnv->CallStaticIntMethod( classHelper, GetDisplayWidth, lNativeActivity );

		jmethodID GetDisplayHeight = lJNIEnv->GetStaticMethodID( classHelper, "GetDisplayHeight","(Landroid/app/Activity;)I" );
		int height = lJNIEnv->CallStaticIntMethod( classHelper, GetDisplayHeight, lNativeActivity );

		vm->DetachCurrentThread();
	}

	int gettextstate()
	{
		if ( agk::GetTextInputState() == 0 ) return 1;
		if ( agk::GetCurrentEditBox() ) return 1;
		return 0;
	}

	void begin()
	{
		static int image1 = 0;
		static int text1 = 0;
		g_bFirstLoop = false;
		
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
			return;
		}
	}

	void reset()
	{
		//g_bFirstLoop = true;
	}

	void loop()
	{
		if ( g_bFirstLoop )
		{
			begin();
		}
		else
		{
			static int lastorien = 0;
			static int orien = 0;
			static float orientime = 0;
			orientime += agk::GetFrameTime();
			if ( orientime > 1 )
			{
				orientime = 0;

				jint lFlags = 0;
				JNIEnv* lJNIEnv = g_pActivity->env;
				JavaVM* vm = g_pActivity->vm;
				vm->AttachCurrentThread(&lJNIEnv, NULL);

				if ( !g_pActivity ) agk::Warning("Failed to get activity pointer");
				jobject lNativeActivity = g_pActivity->clazz;
				if ( !lNativeActivity ) agk::Warning("Failed to get native activity pointer");

				jclass classActivity = lJNIEnv->FindClass("android/app/NativeActivity");
				if ( !classActivity ) agk::Warning("Failed to get class NativeActivity");

				jmethodID getClassLoader = lJNIEnv->GetMethodID(classActivity, "getClassLoader", "()Ljava/lang/ClassLoader;");
				if( !getClassLoader ) agk::Warning("Exception occurred while getting getClassLoader methodId");

				jobject objClassLoader = lJNIEnv->CallObjectMethod(lNativeActivity, getClassLoader);
				if ( !objClassLoader ) agk::Warning("Exception occurred while getting class loader instance");

				jclass classLoader = lJNIEnv->FindClass("java/lang/ClassLoader");
				if ( !classLoader ) agk::Warning("Exception occurred while finding ClassLoader class definition");

				jmethodID methodFindClass = lJNIEnv->GetMethodID( classLoader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
				if( !methodFindClass ) agk::Warning("Exception occurred while getting loadClass method id");

				// Get AGK Helper class
				jclass classHelper = (jclass)lJNIEnv->CallObjectMethod(objClassLoader, methodFindClass, lJNIEnv->NewStringUTF("com.thegamecreators.agk_player.AGKHelper"));
				if( !classHelper ) agk::Warning("Failed to get AGKHelper class");

				jmethodID methodGetOrien = lJNIEnv->GetStaticMethodID( classHelper, "GetOrientation","(Landroid/app/Activity;)I");
				if ( !methodGetOrien ) agk::Warning("Failed to get method GetOrientation");

				orien = lJNIEnv->CallStaticIntMethod( classHelper, methodGetOrien, lNativeActivity );

				vm->DetachCurrentThread();

				if ( g_bIsTablet )
				{
					switch( orien )
					{
						case 0: orien = 3; break;
						case 1: orien = 2; break;
						case 2: orien = 4; break;
						case 3: orien = 1; break;
						default: break;
					}
				}
				else
				{
					switch( orien )
					{
						case 0: orien = 1; break;
						case 1: orien = 3; break;
						case 2: orien = 2; break;
						case 3: orien = 4; break;
						default: break;
					}
				}

				if ( orien != lastorien ) agk::OrientationChanged( orien );
				lastorien = orien;
			}

			try
			{
				App.Loop();
			}
			catch(...)
			{
				uString err; agk::GetLastError(err);
				err.Prepend( "Error: " );
				agk::Message( err.GetStr() );
				PlatformAppQuit();
				return;
			}
		}
	}

	int getinternaldata(int index)
	{
		return agk::GetInternalDataI(index);
	}

	void updatesize()
	{
		agk::UpdateDeviceSize();
	}

	void setaccel( float x, float y, float z )
	{
		x /= 10.0f;
		y /= 10.0f;
		z /= 10.0f;

		//uString str;
		//str.Format( "X: %.2f, Y: %.2f, Z: %.2f", x, y, z );
		//agk::Print( str );

		if ( g_bCalibrated )
		{
			float x2 = 0;
			float y2 = 0;

			if ( g_iPortraitAxis == 1 ) y2 = y * g_iPortraitModifier;
			else y2 = x * g_iPortraitModifier;

			if ( g_iLandscapeAxis == 1 ) x2 = y * g_iLandscapeModifier;
			else x2 = x * g_iLandscapeModifier;

			x = x2;
			y = y2;
		}

		// check orientation
		static int lastmode = 0;
		int mode = 0;
		if ( agk::Abs(y) > agk::Abs(x) + 0.2 )
		{
			if ( y < -0.4 ) mode = 2;
			if ( y > 0.4 ) mode = 1;
		}
		else if ( agk::Abs(x) > agk::Abs(y) + 0.2 )
		{
			if ( x < -0.4 ) mode = 3;
			if ( x > 0.4 ) mode = 4;
		}

		//if ( mode > 0 && mode != lastmode ) agk::OrientationChanged( mode );
		//lastmode = mode;

		agk::Accelerometer( x, y, z );
	}

	void setgyro( float x, float y, float z )
	{
		agk::Gyro( x,y,z );
	}

	void setproximity( float dist )
	{
		agk::Proximity( dist );
	}

	void setlight( float light )
	{
		agk::LightSensor( light );
	}

	void setmagnetic( float x, float y, float z )
	{
		agk::Magnetic( x,y,z );
	}

	void setrotvector( float x, float y, float z, float w )
	{
		static int needsCorrecting = 1;
		if ( w != 0 ) needsCorrecting = 0;

		// w is optional on Android API <18
		//float length = x*x + y*y + z*z + w*w;
		//if ( length < 0.999f || length > 1.001f ) w = agk::Sqrt( 1 - (x*x + y*y + z*z) );
		if ( needsCorrecting ) w = agk::Sqrt( 1 - (x*x + y*y + z*z) );
		agk::RotationSensor( x,y,z,w );
	}

	void touchdown( int id, float x, float y )
	{
		agk::TouchPressed( id+1, x, y );
	}

	void touchmoved( int id, float x, float y )
	{
		agk::TouchMoved( id+1, x, y );
	}

	void touchup( int id, float x, float y )
	{
		agk::TouchReleased( id+1, x, y );
	}

	void mousedown( int id )
	{
		agk::MouseLeftButton( id, 1 );
	}

	void mouseup( int id )
	{
		agk::MouseLeftButton( id, 0 );
	}

	void mousemoved( int id, float x, float y )
	{
		agk::MouseMove( id, x, y );
	}

	void joystickaxis( int id, int stick, int axis, float value )
	{
		agk::JoystickAxis( (void*)id, stick, axis, value );
	}

	void joystickbutton( int id, int button, int state )
	{
		agk::JoystickButton( (void*)id, button, state );
	}

	void keyboardmode( int mode )
	{
		agk::KeyboardMode( mode );
	}

	int TranslateKey( int key )
	{
		switch( key )
		{
			case AKEYCODE_DEL: return AGK_KEY_BACK; // backspace
			case AKEYCODE_TAB: return AGK_KEY_TAB;
			case AKEYCODE_ENTER: return AGK_KEY_ENTER;
			case AKEYCODE_SHIFT_LEFT: return 257;
			case AKEYCODE_SHIFT_RIGHT: return 258;
			case AKEYCODE_CTRL_LEFT: return 259;
			case AKEYCODE_CTRL_RIGHT: return 260;
			case AKEYCODE_BACK: return AGK_KEY_ESCAPE;
			case AKEYCODE_SPACE: return AGK_KEY_SPACE;
			case AKEYCODE_PAGE_UP: return AGK_KEY_PAGEUP;
			case AKEYCODE_PAGE_DOWN: return AGK_KEY_PAGEDOWN;
			case AKEYCODE_MOVE_HOME: return AGK_KEY_END;
			case AKEYCODE_MOVE_END: return AGK_KEY_HOME;
			case AKEYCODE_DPAD_LEFT: return AGK_KEY_LEFT;
			case AKEYCODE_DPAD_UP: return AGK_KEY_UP;
			case AKEYCODE_DPAD_RIGHT: return AGK_KEY_RIGHT;
			case AKEYCODE_DPAD_DOWN: return AGK_KEY_DOWN;
			case AKEYCODE_DPAD_CENTER: return AGK_KEY_ENTER;
			case AKEYCODE_INSERT: return AGK_KEY_INSERT;
			
			case AKEYCODE_0: return AGK_KEY_0;
			case AKEYCODE_1: return AGK_KEY_1;
			case AKEYCODE_2: return AGK_KEY_2;
			case AKEYCODE_3: return AGK_KEY_3;
			case AKEYCODE_4: return AGK_KEY_4;
			case AKEYCODE_5: return AGK_KEY_5;
			case AKEYCODE_6: return AGK_KEY_6;
			case AKEYCODE_7: return AGK_KEY_7;
			case AKEYCODE_8: return AGK_KEY_8;
			case AKEYCODE_9: return AGK_KEY_9;

			case AKEYCODE_A: return AGK_KEY_A;
			case AKEYCODE_B: return AGK_KEY_B;
			case AKEYCODE_C: return AGK_KEY_C;
			case AKEYCODE_D: return AGK_KEY_D;
			case AKEYCODE_E: return AGK_KEY_E;
			case AKEYCODE_F: return AGK_KEY_F;
			case AKEYCODE_G: return AGK_KEY_G;
			case AKEYCODE_H: return AGK_KEY_H;
			case AKEYCODE_I: return AGK_KEY_I;
			case AKEYCODE_J: return AGK_KEY_J;
			case AKEYCODE_K: return AGK_KEY_K;
			case AKEYCODE_L: return AGK_KEY_L;
			case AKEYCODE_M: return AGK_KEY_M;
			case AKEYCODE_N: return AGK_KEY_N;
			case AKEYCODE_O: return AGK_KEY_O;
			case AKEYCODE_P: return AGK_KEY_P;
			case AKEYCODE_Q: return AGK_KEY_Q;
			case AKEYCODE_R: return AGK_KEY_R;
			case AKEYCODE_S: return AGK_KEY_S;
			case AKEYCODE_T: return AGK_KEY_T;
			case AKEYCODE_U: return AGK_KEY_U;
			case AKEYCODE_V: return AGK_KEY_V;
			case AKEYCODE_W: return AGK_KEY_W;
			case AKEYCODE_X: return AGK_KEY_X;
			case AKEYCODE_Y: return AGK_KEY_Y;
			case AKEYCODE_Z: return AGK_KEY_Z;

			case AKEYCODE_F1: return AGK_KEY_F1;
			case AKEYCODE_F2: return AGK_KEY_F2;
			case AKEYCODE_F3: return AGK_KEY_F3;
			case AKEYCODE_F4: return AGK_KEY_F4;
			case AKEYCODE_F5: return AGK_KEY_F5;
			case AKEYCODE_F6: return AGK_KEY_F6;
			case AKEYCODE_F7: return AGK_KEY_F7;
			case AKEYCODE_F8: return AGK_KEY_F8;
			case AKEYCODE_F9: return 120;
			case AKEYCODE_F10: return 121;
			case AKEYCODE_F11: return 122;
			case AKEYCODE_F12: return 123;
			
			case AKEYCODE_GRAVE: return 223; break; // `
			case AKEYCODE_MINUS: return 189; break; // -
			case AKEYCODE_EQUALS: return 187; break; // =
			case AKEYCODE_LEFT_BRACKET: return 219; break; // [
			case AKEYCODE_RIGHT_BRACKET: return 221; break; // ]
			case AKEYCODE_SEMICOLON: return 186; break; // ;
			case AKEYCODE_APOSTROPHE: return 192; break; // '
			case AKEYCODE_COMMA: return 188; break; // ,
			case AKEYCODE_PERIOD: return 190; break; // .
			case AKEYCODE_SLASH: return 191; break; // /
			case AKEYCODE_BACKSLASH: return 220; break; // \    /
			case AKEYCODE_POUND: return 222; break; // #

			case AKEYCODE_ALT_LEFT: return 261;
			case AKEYCODE_ALT_RIGHT: return 262;
			case AKEYCODE_ESCAPE: return AGK_KEY_ESCAPE;
			case AKEYCODE_FORWARD_DEL: return AGK_KEY_DELETE;

			/*
			// O-U-Y-A keys:
			case 96:  return AGK_KEY_O;
			case 99:  return AGK_KEY_U;
			case 100: return AGK_KEY_Y;
			case 97:  return AGK_KEY_A;

			case AKEYCODE_MENU: return 101; // Ouya home button
			case AKEYCODE_BUTTON_START: return 101; // XBox start button

			case 102: return 102; // L1
			case 104: return 104; // L2
			case 106: return 106; // L3
			case 103: return 103; // R1
			case 105: return 105; // R2
			case 107: return 107; // R3
			*/

			default: return 0;
		}

		return 0;
	}

	int AsciiKey( int key )
	{
		if ( agk::GetRawKeyState( AGK_KEY_SHIFT ) == 1 )
		{
			// shift pressed
			switch( key )
			{
				case AKEYCODE_SPACE: return ' ';

				case AKEYCODE_0: return ')';
				case AKEYCODE_1: return '!';
				case AKEYCODE_2: return '@';
				case AKEYCODE_3: return '#';
				case AKEYCODE_4: return '$';
				case AKEYCODE_5: return '%';
				case AKEYCODE_6: return '^';
				case AKEYCODE_7: return '&';
				case AKEYCODE_8: return '*';
				case AKEYCODE_9: return '(';

				case AKEYCODE_A: return 'A';
				case AKEYCODE_B: return 'B';
				case AKEYCODE_C: return 'C';
				case AKEYCODE_D: return 'D';
				case AKEYCODE_E: return 'E';
				case AKEYCODE_F: return 'F';
				case AKEYCODE_G: return 'G';
				case AKEYCODE_H: return 'H';
				case AKEYCODE_I: return 'I';
				case AKEYCODE_J: return 'J';
				case AKEYCODE_K: return 'K';
				case AKEYCODE_L: return 'L';
				case AKEYCODE_M: return 'M';
				case AKEYCODE_N: return 'N';
				case AKEYCODE_O: return 'O';
				case AKEYCODE_P: return 'P';
				case AKEYCODE_Q: return 'Q';
				case AKEYCODE_R: return 'R';
				case AKEYCODE_S: return 'S';
				case AKEYCODE_T: return 'T';
				case AKEYCODE_U: return 'U';
				case AKEYCODE_V: return 'V';
				case AKEYCODE_W: return 'W';
				case AKEYCODE_X: return 'X';
				case AKEYCODE_Y: return 'Y';
				case AKEYCODE_Z: return 'Z';

				case AKEYCODE_GRAVE: return '~';
				case AKEYCODE_COMMA: return '<';
				case AKEYCODE_PERIOD: return '>';
				case AKEYCODE_MINUS: return '_';
				case AKEYCODE_EQUALS: return '+';
				case AKEYCODE_LEFT_BRACKET: return '{';
				case AKEYCODE_RIGHT_BRACKET: return '}';
				case AKEYCODE_BACKSLASH: return '|';
				case AKEYCODE_SEMICOLON: return ':';
				case AKEYCODE_APOSTROPHE: return '"';
				case AKEYCODE_SLASH: return '?';

				default: return 0;
			}
		}
		else
		{
			// shift not pressed
			switch( key )
			{
				case AKEYCODE_SPACE: return ' ';

				case AKEYCODE_0: return '0';
				case AKEYCODE_1: return '1';
				case AKEYCODE_2: return '2';
				case AKEYCODE_3: return '3';
				case AKEYCODE_4: return '4';
				case AKEYCODE_5: return '5';
				case AKEYCODE_6: return '6';
				case AKEYCODE_7: return '7';
				case AKEYCODE_8: return '8';
				case AKEYCODE_9: return '9';

				case AKEYCODE_A: return 'a';
				case AKEYCODE_B: return 'b';
				case AKEYCODE_C: return 'c';
				case AKEYCODE_D: return 'd';
				case AKEYCODE_E: return 'e';
				case AKEYCODE_F: return 'f';
				case AKEYCODE_G: return 'g';
				case AKEYCODE_H: return 'h';
				case AKEYCODE_I: return 'i';
				case AKEYCODE_J: return 'j';
				case AKEYCODE_K: return 'k';
				case AKEYCODE_L: return 'l';
				case AKEYCODE_M: return 'm';
				case AKEYCODE_N: return 'n';
				case AKEYCODE_O: return 'o';
				case AKEYCODE_P: return 'p';
				case AKEYCODE_Q: return 'q';
				case AKEYCODE_R: return 'r';
				case AKEYCODE_S: return 's';
				case AKEYCODE_T: return 't';
				case AKEYCODE_U: return 'u';
				case AKEYCODE_V: return 'v';
				case AKEYCODE_W: return 'w';
				case AKEYCODE_X: return 'x';
				case AKEYCODE_Y: return 'y';
				case AKEYCODE_Z: return 'z';

				case AKEYCODE_GRAVE: return '`';
				case AKEYCODE_STAR: return '*';
				case AKEYCODE_POUND: return '#';
				case AKEYCODE_COMMA: return ',';
				case AKEYCODE_PERIOD: return '.';
				case AKEYCODE_MINUS: return '-';
				case AKEYCODE_EQUALS: return '=';
				case AKEYCODE_LEFT_BRACKET: return '[';
				case AKEYCODE_RIGHT_BRACKET: return ']';
				case AKEYCODE_BACKSLASH: return '\\';
				case AKEYCODE_SEMICOLON: return ';';
				case AKEYCODE_APOSTROPHE: return '\'';
				case AKEYCODE_SLASH: return '/';
				case AKEYCODE_AT: return '@';
				case AKEYCODE_PLUS: return '+';

				default: return 0;
			}
		}
	}

	void keydown( int key )
	{
		// stop ouya A button sending a back command
		if ( agk::GetRawKeyState(97) == 1 && key == AKEYCODE_BACK ) return;

		int newKey = TranslateKey(key);
		agk::KeyDown( newKey );
		if ( newKey == 257 || newKey == 258 ) agk::KeyDown( 16 );
		if ( newKey == 259 || newKey == 260 ) agk::KeyDown( 17 );
		if ( newKey == 261 || newKey == 262 ) agk::KeyDown( 18 );
		
		int ch = AsciiKey( key );
		if ( ch != 0 )
		{
			agk::CharDown( ch );
		}
	}

	void keyup( int key )
	{
		int newKey = TranslateKey(key);
		agk::KeyUp( newKey );
		if ( newKey == 257 || newKey == 258 ) agk::KeyUp( 16 );
		if ( newKey == 259 || newKey == 260 ) agk::KeyUp( 17 );
		if ( newKey == 261 || newKey == 262 ) agk::KeyUp( 18 );
	}

	/*
	void logInput( int action, int pointer, int source, int device, int pointers, int id, int flags, float x, float y )
	{
		char str[ 1024 ];
		sprintf( str, "Action: %d, pointer: %d, source: %d, device: %d, count: %d, id: %d, flags: %d, X: %.1f, Y: %.1f\n", action, pointer, source, device, pointers, id, flags, x, y );

		uString sPath = agk::GetDocumentsPath();
		sPath.Append( "/input_log.txt" );
		FILE *fp = fopen( sPath.GetStr(), "ab" );
		if ( !fp ) 
		{
			static int warned = 0;
			if ( !warned )
			{
				warned = 1;
				agk::Message( "Failed to log input" );
			}
			return;
		}
		fwrite( str, 1, strlen(str), fp );
		fclose( fp );
	}
	*/

	float getagktimer()
	{
		return agk::Timer();
	}
}
