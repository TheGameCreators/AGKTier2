// Includes (for Win32)
/*
#define WINVER 0x0501
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
*/

// Windows 7 for touch commands
#define _WIN32_WINNT 0x0601

#include "winsock2.h"

#include <shlobj.h>
#define DIRECTINPUT_VERSION 0x0800
#define INITGUID

#include <Guiddef.h>
#include <mfapi.h>
#include <mfidl.h>
#include <Shlwapi.h>

//#include <mmsystem.h>
//#include <DSound.h>
#include <DInput.h>

//#include <atlbase.h>
#include <windef.h>
#include <XAudio2.h>
#include <dshow.h>

// XAudio2 hack to support both DirectX SDK and Windows 8+
namespace XAudio2Win8
{
	typedef enum _AUDIO_STREAM_CATEGORY
	{
		AudioCategory_Other = 0,
		AudioCategory_ForegroundOnlyMedia,
		AudioCategory_BackgroundCapableMedia,
		AudioCategory_Communications,
		AudioCategory_Alerts,
		AudioCategory_SoundEffects,
		AudioCategory_GameEffects,
		AudioCategory_GameMedia,
	} AUDIO_STREAM_CATEGORY;

	#undef INTERFACE
	#define INTERFACE IXAudio2SourceVoice
	DECLARE_INTERFACE_(IXAudio2SourceVoice, IXAudio2Voice)
	{
		Declare_IXAudio2Voice_Methods();

		STDMETHOD(Start) (THIS_ UINT32 Flags X2DEFAULT(0), UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE;

		STDMETHOD(Stop) (THIS_ UINT32 Flags X2DEFAULT(0), UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE;

		STDMETHOD(SubmitSourceBuffer) (THIS_ _In_ const XAUDIO2_BUFFER* pBuffer, _In_opt_ const XAUDIO2_BUFFER_WMA* pBufferWMA X2DEFAULT(NULL)) PURE;

		STDMETHOD(FlushSourceBuffers) (THIS) PURE;

		STDMETHOD(Discontinuity) (THIS) PURE;

		STDMETHOD(ExitLoop) (THIS_ UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE;

		STDMETHOD_(void, GetState) (THIS_ _Out_ XAUDIO2_VOICE_STATE* pVoiceState, UINT32 Flags X2DEFAULT(0)) PURE;

		STDMETHOD(SetFrequencyRatio) (THIS_ float Ratio,
									  UINT32 OperationSet X2DEFAULT(XAUDIO2_COMMIT_NOW)) PURE;

		STDMETHOD_(void, GetFrequencyRatio) (THIS_ _Out_ float* pRatio) PURE;

		STDMETHOD(SetSourceSampleRate) (THIS_ UINT32 NewSourceSampleRate) PURE;
	};

	interface IXAudio2MasteringVoice : public IXAudio2Voice
	{
		virtual void STDMETHODCALLTYPE GetChannelMask(
			DWORD *pChannelMask) = 0;

	};

	#undef INTERFACE
	#define INTERFACE IXAudio2
	DECLARE_INTERFACE_(IXAudio2, IUnknown)
	{
		virtual HRESULT STDMETHODCALLTYPE RegisterForCallbacks(
			IXAudio2EngineCallback *pCallback) = 0;

		virtual void STDMETHODCALLTYPE UnregisterForCallbacks(
			IXAudio2EngineCallback *pCallback) = 0;

		virtual HRESULT STDMETHODCALLTYPE CreateSourceVoice(
			IXAudio2SourceVoice **ppSourceVoice,
			const WAVEFORMATEX *pSourceFormat,
			UINT32 Flags = 0,
			float MaxFrequencyRatio = XAUDIO2_DEFAULT_FREQ_RATIO,
			IXAudio2VoiceCallback *pCallback = 0,
			const XAUDIO2_VOICE_SENDS *pSendList = 0,
			const XAUDIO2_EFFECT_CHAIN *pEffectChain = 0) = 0;

		virtual HRESULT STDMETHODCALLTYPE CreateSubmixVoice(
			IXAudio2SubmixVoice **ppSubmixVoice,
			UINT32 InputChannels,
			UINT32 InputSampleRate,
			UINT32 Flags = 0,
			UINT32 ProcessingStage = 0,
			const XAUDIO2_VOICE_SENDS *pSendList = 0,
			const XAUDIO2_EFFECT_CHAIN *pEffectChain = 0) = 0;

		virtual HRESULT STDMETHODCALLTYPE CreateMasteringVoice(
			IXAudio2MasteringVoice **ppMasteringVoice,
			UINT32 InputChannels = XAUDIO2_DEFAULT_CHANNELS,
			UINT32 InputSampleRate = XAUDIO2_DEFAULT_SAMPLERATE,
			UINT32 Flags = 0,
			LPCWSTR DeviceId = 0,
			const XAUDIO2_EFFECT_CHAIN *pEffectChain = 0,
			AUDIO_STREAM_CATEGORY StreamCategory = AudioCategory_GameEffects) = 0;

		virtual HRESULT STDMETHODCALLTYPE StartEngine(
			) = 0;

		virtual void STDMETHODCALLTYPE StopEngine(
			) = 0;

		virtual HRESULT STDMETHODCALLTYPE CommitChanges(
			UINT32 OperationSet) = 0;

		virtual void STDMETHODCALLTYPE GetPerformanceData(
			XAUDIO2_PERFORMANCE_DATA *pPerfData) = 0;

		virtual void STDMETHODCALLTYPE SetDebugConfiguration(
			const XAUDIO2_DEBUG_CONFIGURATION *pDebugConfiguration,
			void *pReserved = 0) = 0;

	};
}

// For grabbing WebCam samples (added qedit.h to windows src)
#include <QEdit.h> 

// V108 Ultra commands for sensor access
#include <InitGuid.h>
#include <Sensors.h>
#include <SensorsApi.h>
#include <LocationApi.h>
#include <propkeydef.h>
#include <process.h>

// AGK header
#include "AGK.h"
#include "../renderer/OpenGL2/OpenGL2.h"

#include "Dwmapi.h"

#include "zlib.h"

// XInput
#include <wbemidl.h>
#include <XInput.h>

#pragma comment(lib,"Strmiids.lib")

#define WM_SOUND_EVENT 0x8001
#define WM_VIDEO_EVENT 0x8002

// perform a fast conversion from float to int
__inline int agklrintf (float flt)
{	
#ifdef _WIN64
	if (flt < 0 ) return (int) ( ceil(flt-0.5f ) );
	else return (int) ( floor(flt+0.5f ) );
#else
	int intgr;

	_asm
	{	
		fld flt
		fistp intgr
	};
		
	return intgr;
#endif
}

int agk::m_iKeyboardMode = 1; // physical keyboard

// delay loaded functions
BOOL (__stdcall *IsTouchWindowDelayed)(HWND, PULONG) = 0;

// ULTRA COMMAND globals A
ISensor* g_pAccelerometerSensor = NULL;
ILocation* g_pGeolocationSensor = NULL;; //ISensor* g_pGeolocationSensor = NULL; using LocationAPI now
ISensor* g_pMagnetSensor = NULL;
ISensor* g_pGyroSensor = NULL;
ISensor* g_pInclinometerSensor = NULL;
ISensor* g_pLightSensor = NULL;
ISensor* g_pOrientationSensor = NULL;
int g_iIsGPSTracking = 0;

// V108 Sensor class for Ultra commands
class cSensorEventSink : public ISensorEvents
{
public:
	//ISensorEvents
	HRESULT __stdcall OnEvent(ISensor *pSensor,REFGUID eventID,IPortableDeviceValues *pEventData)
	{
		return S_OK;
	}
	HRESULT __stdcall OnDataUpdated(ISensor *pSensor,ISensorDataReport *pNewData)
	{
        PROPVARIANT var = {};
        HRESULT hr = S_OK;
		if ( g_pAccelerometerSensor==pSensor )
		{
			hr = pNewData->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_X_G, &var);
			if (SUCCEEDED(hr))
			{
				// Update Accelerometer
				float x, y, z;
				x = (float)var.dblVal;
				pNewData->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_Y_G, &var);
				y = (float)var.dblVal;
				pNewData->GetSensorValue(SENSOR_DATA_TYPE_ACCELERATION_Z_G, &var);
				z = (float)var.dblVal;
				agk::Accelerometer(x,y,z);
			}
		}
		/* Using Location API instead
		if ( g_pGeolocationSensor==pSensor )
		{
			hr = pNewData->GetSensorValue(SENSOR_DATA_TYPE_LATITUDE_DEGREES, &var);
			if (SUCCEEDED(hr))
			{
				// Update GetGeolocation
				float latitude = (float)var.dblVal;
				pNewData->GetSensorValue(SENSOR_DATA_TYPE_LONGITUDE_DEGREES, &var);
				float longitude = (float)var.dblVal;
				agk::GPS( longitude, latitude );
			}
		}
		*/
		if ( g_pMagnetSensor==pSensor )
		{
			hr = pNewData->GetSensorValue(SENSOR_DATA_TYPE_MAGNETIC_FIELD_STRENGTH_X_MILLIGAUSS, &var);
			if (SUCCEEDED(hr))
			{
				float magX = (float)var.dblVal;
				pNewData->GetSensorValue(SENSOR_DATA_TYPE_MAGNETIC_FIELD_STRENGTH_Y_MILLIGAUSS, &var);
				float magY = (float)var.dblVal;
				pNewData->GetSensorValue(SENSOR_DATA_TYPE_MAGNETIC_FIELD_STRENGTH_Z_MILLIGAUSS, &var);
				float magZ = (float)var.dblVal;
				agk::Magnetic( magX, magY, magZ );
			}
		}
		if ( g_pGyroSensor==pSensor )
		{
			hr = pNewData->GetSensorValue(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_X_DEGREES_PER_SECOND, &var);
			if (SUCCEEDED(hr))
			{
				// Update GetGyrometer
				float gyroX = (float)var.dblVal;
				pNewData->GetSensorValue(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_Y_DEGREES_PER_SECOND, &var);
				float gyroY = (float)var.dblVal;
				pNewData->GetSensorValue(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_Z_DEGREES_PER_SECOND, &var);
				float gyroZ = (float)var.dblVal;
				agk::Gyro( gyroX, gyroY, gyroZ );
			}
		}
		if ( g_pLightSensor==pSensor )
		{
			hr = pNewData->GetSensorValue(SENSOR_DATA_TYPE_LIGHT_LEVEL_LUX, &var);
			if (SUCCEEDED(hr))
			{
				// Update GetLightSensor
				agk::LightSensor( var.fltVal );
			}
		}
        PropVariantClear(&var);
		return S_OK;
	}
	HRESULT __stdcall OnLeave(REFSENSOR_ID sensorID)
	{
		//cout << "Onleave" << endl;
		return S_OK;
	}
	HRESULT __stdcall OnStateChanged(ISensor *pSensor,SensorState state)
	{
		return S_OK;
	}
	// IUnknown
	ULONG __stdcall AddRef()
	{
		return ++ref;
	}
	ULONG __stdcall Release()
	{
		return --ref;
	}
	HRESULT __stdcall QueryInterface(const IID& id,void**p)
	{
		if (id == IID_IUnknown || id == __uuidof(ISensorEvents))
		{
			*p = this;
			AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	cSensorEventSink() { ref = 0; }

private:
	unsigned long ref;
};
HRESULT SetCurrentReportInterval(ISensor* pSensor, ULONG ulNewInterval)
{
    HRESULT hr = S_OK;
    IPortableDeviceValues* pPropsToSet = NULL; // Input
    IPortableDeviceValues* pPropsReturn = NULL; // Output
    hr = CoCreateInstance(__uuidof(PortableDeviceValues),
                            NULL,
                            CLSCTX_INPROC_SERVER,                           
                            IID_PPV_ARGS(&pPropsToSet));
    if(SUCCEEDED(hr))
    {
        // Add the current report interval property.
        hr = pPropsToSet->SetUnsignedIntegerValue(SENSOR_PROPERTY_CURRENT_REPORT_INTERVAL, ulNewInterval);
    }
    if(SUCCEEDED(hr))
    {
        // Only setting a single property, here.
        hr = pSensor->SetProperties(pPropsToSet, &pPropsReturn);
    }
    if(hr == S_FALSE)
    {
        HRESULT hrError = S_OK;
        hr = pPropsReturn->GetErrorValue(SENSOR_PROPERTY_CURRENT_REPORT_INTERVAL, &hrError);
        if(SUCCEEDED(hr))
        {
            //wprintf_s(L"\nSetting current report interval failed with error 0x%X\n", hrError);
            hr = hrError;
        }
    }
    else if(hr == E_ACCESSDENIED)
    {
        // No permission. Take appropriate action.
    }
	if ( pPropsToSet!=NULL ) { pPropsToSet->Release(); pPropsToSet=NULL; }
	if ( pPropsReturn!=NULL ) { pPropsReturn->Release(); pPropsReturn=NULL; }
    return hr;
}

// ULTRA COMMAND globals B
ISensorManager* g_pSensorManager = NULL;
ISensorCollection* g_pSensorCollection = NULL;
cSensorEventSink* g_pSensorEventClass = NULL;
ISensorEvents* g_pSensorEvents = NULL;

void GetLocationUpdate()
{
	// don't setup the geolocation until it is called
	static int first = 1;
	if ( first )
	{
		// only request permission once
		first = 0;

		/* any attempt to mess with AGK window here crashes Windows 8 
		// send app to back in case permission dialog is used
		//ShowWindow(g_hWnd,SW_HIDE);
		//::SetWindowPos( g_hWnd, HWND_BOTTOM, 0,0, 0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE );
		bool bIsFullscreen = false;
		LONG lStyle = GetWindowLong( g_hWnd, GWL_STYLE );
		if ( lStyle & WS_POPUP )
		{
			bIsFullscreen = true;
			::ShowWindow( g_hWnd, SW_MINIMIZE );
			//MessageBox( g_hWnd, "Got here without crashing", "Message", MB_SETFOREGROUND | MB_TOPMOST );
			::ShowWindow( g_hWnd, SW_SHOWNORMAL );
			//::SetWindowLongPtr( g_hWnd, GWL_STYLE, (lStyle & ~WS_POPUP) );
			//::SetWindowPos( g_hWnd, 0, 0,0, 0,0, SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE| SWP_FRAMECHANGED );
		}
		else 
		{
			bIsFullscreen = false;
		}
		*/

		// Geolocation through sensors not 100% reliable, so use Location API instead
		//if (SUCCEEDED(g_pGeolocationSensor.CoCreateInstance(CLSID_Location))) // Create the Location object
		if ( SUCCEEDED(CoCreateInstance(CLSID_Location, NULL, CLSCTX_INPROC, IID_ILocation, (void **) &g_pGeolocationSensor)) )
		{
			// Array of report types of interest. Other ones include IID_ICivicAddressReport
			IID REPORT_TYPES[] = { IID_ILatLongReport };

			// Request permissions for this user account to receive location data for all the
			// types defined in REPORT_TYPES (which is currently just one report)
			if (SUCCEEDED(g_pGeolocationSensor->RequestPermissions(NULL, REPORT_TYPES, ARRAYSIZE(REPORT_TYPES), TRUE))) // TRUE means a synchronous request
			{
				// we have permission
			}
			else
			{
				// don't have permission, disable GEO
				g_pGeolocationSensor->Release();
				g_pGeolocationSensor = NULL;
			}
		}

		/* as above
		// restore app window after permission sought
		//ShowWindow(g_hWnd,SW_SHOW);
		//::SetWindowPos( g_hWnd, HWND_TOP, 0,0, 0,0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE );
		if ( bIsFullscreen )
		{
			//::ShowWindow( g_hWnd, SW_SHOWNORMAL );
			//LONG lStyle = GetWindowLong( g_hWnd, GWL_STYLE );
			//::SetWindowLongPtr( g_hWnd, GWL_STYLE, lStyle | WS_POPUP );
			//::SetWindowPos( g_hWnd, 0, 0,0, 0,0, SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE| SWP_FRAMECHANGED );
		}
		*/
	}

	if ( g_pGeolocationSensor!=NULL )
	{
		LOCATION_REPORT_STATUS status = REPORT_NOT_SUPPORTED; // The LOCATION_REPORT_STATUS enumeration is defined in LocApi.h in the SDK
		if (SUCCEEDED(g_pGeolocationSensor->GetReportStatus(IID_ILatLongReport, &status))) // Get the status of this report type
		{
			bool fCachedData = true;
			switch (status) // If there is an error, print the error
			{
				case REPORT_RUNNING:
					// If the status for the current report is running, then don't print any additional message
					fCachedData = false;
					break;
				case REPORT_NOT_SUPPORTED:
					break;
				case REPORT_ERROR:
					break;
				case REPORT_ACCESS_DENIED:
					break;
				case REPORT_INITIALIZING:
					break;
			}
		}
		ILocationReport* spLocationReport = 0; // This is our location report object
		ILatLongReport* spLatLongReport = 0; // This is our LatLong report object

		// Get the current location report,
		// then get the ILatLongReport interface from ILocationReport,
		// then ensure it isn't NULL
		if ((SUCCEEDED(g_pGeolocationSensor->GetReport(IID_ILatLongReport, &spLocationReport))) &&
			(SUCCEEDED(spLocationReport->QueryInterface(&spLatLongReport))))
		{
			DOUBLE latitude = 0, longitude = 0, altitude = 0;
			if (SUCCEEDED(spLatLongReport->GetLatitude(&latitude)) && SUCCEEDED(spLatLongReport->GetLongitude(&longitude)) && SUCCEEDED(spLatLongReport->GetAltitude(&altitude)))
			{
				agk::GPS( (float)longitude, (float)latitude, (float) altitude );
			}
		}

		if ( spLocationReport ) spLocationReport->Release();
		if ( spLatLongReport ) spLatLongReport->Release();
	}
}

// Macros
#ifndef SAFE_RELEASE
#define SAFE_RELEASE( x )  \
    if ( NULL != x )       \
    {                      \
        x->Release( );     \
        x = NULL;          \
	}
#endif

extern "C" {
FILE* AGKfopen( const char *szPath, const char *mode )
{
	int size = MultiByteToWideChar( CP_UTF8, 0, szPath, -1, 0, 0 );
	wchar_t *wzPath = new wchar_t[ size ];
	MultiByteToWideChar( CP_UTF8, 0, szPath, -1, wzPath, size );

	size = MultiByteToWideChar( CP_UTF8, 0, mode, -1, 0, 0 );
	wchar_t *wzMode = new wchar_t[ size ];
	MultiByteToWideChar( CP_UTF8, 0, mode, -1, wzMode, size );

	FILE* pFile = _wfopen( wzPath, wzMode );
	delete [] wzPath;
	return pFile;
}
}

inline int GetPathExistsUTF8( const char *szPath )
{
	int size = MultiByteToWideChar( CP_UTF8, 0, szPath, -1, 0, 0 );
	wchar_t *wzPath = new wchar_t[ size ];
	MultiByteToWideChar( CP_UTF8, 0, szPath, -1, wzPath, size );

	UINT result = GetFileAttributesW( wzPath );
	delete [] wzPath;

	if ( result == INVALID_FILE_ATTRIBUTES ) return 0;
	else if ( result & FILE_ATTRIBUTE_DIRECTORY ) return 1;
	else return 2;
}

inline BOOL SetCurrentDirectoryUTF8( const char* szPath )
{
	int size = MultiByteToWideChar( CP_UTF8, 0, szPath, -1, 0, 0 );
	wchar_t *wzPath = new wchar_t[ size ];
	MultiByteToWideChar( CP_UTF8, 0, szPath, -1, wzPath, size );

	BOOL result = SetCurrentDirectoryW( wzPath );
	delete [] wzPath;

	return result;
}

inline BOOL CreateDirectoryUTF8( const char* szPath, LPSECURITY_ATTRIBUTES sec )
{
	int size = MultiByteToWideChar( CP_UTF8, 0, szPath, -1, 0, 0 );
	wchar_t *wzPath = new wchar_t[ size ];
	MultiByteToWideChar( CP_UTF8, 0, szPath, -1, wzPath, size );

	BOOL result = CreateDirectoryW( wzPath, sec );
	delete [] wzPath;
	return result;
}

inline void RemoveDirectoryUTF8( const char* szPath )
{
	int size = MultiByteToWideChar( CP_UTF8, 0, szPath, -1, 0, 0 );
	wchar_t *wzPath = new wchar_t[ size ];
	MultiByteToWideChar( CP_UTF8, 0, szPath, -1, wzPath, size );

	RemoveDirectoryW( wzPath );
	delete [] wzPath;
}

namespace AGK
{
	class SleepControl
	{
		public:
			SleepControl() { timeBeginPeriod( 1 ); }
			~SleepControl() { timeEndPeriod( 1 ); }
	};

	SleepControl g_SleepControl();

	// ****************
	// webcam functions
	// ****************
	HRESULT IsPinConnected(IPin *pPin, BOOL *pResult)
	{
		IPin *pTmp = NULL;
		HRESULT hr = pPin->ConnectedTo(&pTmp);
		if (SUCCEEDED(hr))
		{
			*pResult = TRUE;
		}
		else if (hr == VFW_E_NOT_CONNECTED)
		{
			// The pin is not connected. This is not an error for our purposes.
			*pResult = FALSE;
			hr = S_OK;
		}

		SAFE_RELEASE(pTmp);
		return hr;
	}

	HRESULT IsPinDirection(IPin *pPin, PIN_DIRECTION dir, BOOL *pResult)
	{
		PIN_DIRECTION pinDir;
		HRESULT hr = pPin->QueryDirection(&pinDir);
		if (SUCCEEDED(hr))
		{
			*pResult = (pinDir == dir);
		}
		return hr;
	}

	HRESULT MatchPin(IPin *pPin, PIN_DIRECTION direction, BOOL bShouldBeConnected, BOOL *pResult)
	{
		BOOL bMatch = FALSE;
		BOOL bIsConnected = FALSE;

		HRESULT hr = IsPinConnected(pPin, &bIsConnected);
		if (SUCCEEDED(hr))
		{
			if (bIsConnected == bShouldBeConnected)
			{
				hr = IsPinDirection(pPin, direction, &bMatch);
			}
		}

		if (SUCCEEDED(hr))
		{
			*pResult = bMatch;
		}
		return hr;
	}

	HRESULT FindUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin)
	{
		IEnumPins *pEnum = NULL;
		IPin *pPin = NULL;
		BOOL bFound = FALSE;

		HRESULT hr = pFilter->EnumPins(&pEnum);
		if (FAILED(hr))
		{
			SAFE_RELEASE(pPin);
			SAFE_RELEASE(pEnum);
			return hr;
		}

		while (S_OK == pEnum->Next(1, &pPin, NULL))
		{
			hr = MatchPin(pPin, PinDir, FALSE, &bFound);
			if (FAILED(hr))
			{
				SAFE_RELEASE(pPin);
				SAFE_RELEASE(pEnum);
				return hr;
			}
			if (bFound)
			{
				*ppPin = pPin;
				(*ppPin)->AddRef();
				break;
			}
			SAFE_RELEASE(pPin);
		}

		if (!bFound)
		{
			hr = VFW_E_NOT_FOUND;
		}

		SAFE_RELEASE(pPin);
		SAFE_RELEASE(pEnum);
		return hr;
	}

	HRESULT ConnectFilters( IGraphBuilder *pGraph, IPin *pOut, IBaseFilter *pDest )
	{
		IPin *pIn = NULL;
	        
		// Find an input pin on the downstream filter.
		HRESULT hr = FindUnconnectedPin(pDest, PINDIR_INPUT, &pIn);
		if (SUCCEEDED(hr))
		{
			// Try to connect them.
			hr = pGraph->Connect(pOut, pIn);
			pIn->Release();
		}
		return hr;
	}

	HRESULT ConnectFilters(IGraphBuilder *pGraph, IBaseFilter *pSrc, IBaseFilter *pDest)
	{
		IPin *pOut = NULL;

		// Find an output pin on the first filter.
		HRESULT hr = FindUnconnectedPin(pSrc, PINDIR_OUTPUT, &pOut);
		if (SUCCEEDED(hr))
		{
			hr = ConnectFilters(pGraph, pOut, pDest);
			pOut->Release();
		}
		return hr;
	}

	HRESULT FindUnconnectedFilterPin(IBaseFilter *pSrc, IPin **pOut)
	{
		IEnumPins *pEnum = 0;
		IPin *pPin = 0;
		pSrc->EnumPins( &pEnum );
		pEnum->Reset();
		PIN_INFO info = {0};

		// first look for an unconnected pin
		while ( pEnum->Next( 1, &pPin, NULL ) == S_OK )
		{
			// only interested in output pins
			pPin->QueryPinInfo( &info );
			if ( info.dir == PINDIR_INPUT ) 
			{
				pPin->Release();
				continue;
			}

			// check if it is connected
			IPin *pOther = 0;
			if ( FAILED(pPin->ConnectedTo( &pOther )) )
			{
				*pOut = pPin;
				pEnum->Release();
				return S_OK;
			}
			else
			{
				pOther->Release();
				pPin->Release();
			}
		}

		pEnum->Reset();

		while ( pEnum->Next( 1, &pPin, NULL ) == S_OK )
		{
			// only interested in output pins
			pPin->QueryPinInfo( &info );
			if ( info.dir == PINDIR_INPUT ) 
			{
				pPin->Release();
				continue;
			}

			// find the next filter and check its pins
			IPin *pOther = 0;
			if ( SUCCEEDED(pPin->ConnectedTo( &pOther )) )
			{
				pOther->QueryPinInfo( &info );
				pOther->Release();
								
				if ( FindUnconnectedFilterPin( info.pFilter, pOut ) == S_OK ) 
				{
					pPin->Release();
					pEnum->Release();
					return S_OK;
				}
			}
			pPin->Release();
		}
		pEnum->Release();

		return VFW_E_NOT_FOUND;
	}

	// ****************
	// end webcam functions
	// ****************

	HWND g_hWnd = 0;
	bool g_bGLInit = false;
	bool g_bDwmExists = false;
	bool g_bMFPlatExists = false;
	bool g_bCoInit = false;
	bool g_bIsExternal = false;
	void (*SwapExternal)(void*) = 0;
	void *g_pSwapParam = 0;
	HMODULE g_hUser32 = 0;

	// Timer variables
	__int64 i64TimeFreq = 0;
	__int64 i64CurrentTime = 0;
	__int64 i64StartTime = 0;

	// webcam variables
	IVideoWindow* m_pVWMoniker = 0;
	IMediaControl* m_pMCMoniker = 0;
	ICaptureGraphBuilder2* m_pCaptureMoniker = 0; 
	IFilterGraph2* m_pFilterGraph = 0;
	HWND g_hVideoWnd = 0;
	HWND g_hVideoButtonOK = 0;
	HWND g_hVideoButtonCancel = 0;
	bool g_bIsFullscreen = false;
	ISampleGrabber *pSampleGrabber = 0;

	// default screen size
	int g_iDefaultWidth = 0;
	int g_iDefaultHeight = 0;

	class cAGKAdvert
	{
		protected:
			float m_fX;
			float m_fY;
			float m_fWidth;
			float m_fHeight;
			bool m_bVisible;

		public:
			cAGKAdvert() { m_fX=0; m_fY=0; m_fWidth=320; m_fHeight=50; }
			~cAGKAdvert() {}

			void SetVisible( bool visible )
			{
				
			}

			void SetPosition( float x, float y )
			{
				
			}
	};

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

			XAUDIO2_BUFFER m_XABuffer;
			IXAudio2SourceVoice* m_pSourceVoice;
			XAudio2Win8::IXAudio2SourceVoice* m_pSourceVoiceWin8;
			
			cSoundInst *m_pPrevInst;
			cSoundInst *m_pNextInst;

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
				
				memset( &m_XABuffer, 0, sizeof(XAUDIO2_BUFFER) );
				m_pSourceVoice = 0;
				m_pSourceVoiceWin8 = 0;

				//m_pDSBuffer = NULL;
				
				m_pPrevInst = UNDEF;
				m_pNextInst = UNDEF;
			}

			~cSoundInst()
			{
				if ( m_pSourceVoice ) m_pSourceVoice->DestroyVoice();
				if ( m_pSourceVoiceWin8 ) m_pSourceVoiceWin8->DestroyVoice();
				m_pSourceVoice = 0;
				m_pSourceVoiceWin8 = 0;
			}

			void Reset()
			{
				if ( m_pSourceVoice ) m_pSourceVoice->DestroyVoice();
				if ( m_pSourceVoiceWin8 ) m_pSourceVoiceWin8->DestroyVoice();
				m_pSourceVoice = 0;
				m_pSourceVoiceWin8 = 0;
				memset( &m_XABuffer, 0, sizeof(XAUDIO2_BUFFER) );
				
				m_iParent = 0;
				m_iLoop = 0;
				m_fRate = 1.0f;
				m_fBalance = 0;
				m_iLoopCount = 0;
			}
	};

	cHashedList<cSoundInst> m_cSoundInstances(256);

	// windows music variables, using DirectShow
	IGraphBuilder *g_pGraphBuilder = NULL;
	IMediaControl *g_pMediaControl = NULL;
	IMediaSeeking *g_pMediaSeeking = NULL;
	IMediaEventEx *g_pMediaEventEx = NULL;
	IBasicAudio *g_pBasicAudio = NULL;
	IBaseFilter *pSource = NULL;

	// windows sound variables, using XAudio2
	IXAudio2* pXAudio2 = NULL;
	IXAudio2MasteringVoice* pMasterVoice = NULL;

	XAudio2Win8::IXAudio2* pXAudio2Win8 = NULL;
	XAudio2Win8::IXAudio2MasteringVoice* pMasterVoiceWin8 = NULL;
	int g_iXAudio2Mode = 0; // 0 = not loaded, 1 = DirectX SDK, 2 = Windows 8+
	HMODULE g_hXAudioDLL = 0;

	// DirectSound variables
	//IDirectSound8 *g_pDSDevice = NULL;

	// windows video variables, using DirectShow
	IGraphBuilder *g_pVideoGraphBuilder = NULL;
	IMediaControl *g_pVideoMediaControl = NULL;
	IMediaSeeking *g_pVideoMediaSeeking = NULL;
	IMediaEventEx *g_pVideoMediaEventEx = NULL;
	IBasicAudio *g_pVideoBasicAudio = NULL;
	IBaseFilter *pVideoSource = NULL;
	IBaseFilter *pVideoGrabberF = NULL;
	ISampleGrabber *pVideoSampleGrabber = NULL;
	IBaseFilter *pVideoNullF = NULL;

	// video variables using MediaFoundation
	IMFMediaSession *g_pVideoSession = NULL;
	IMFMediaSource *g_pVideoSource = NULL;
	IMFAudioStreamVolume *g_pVideoVolume = NULL;
	IMFPresentationClock *g_pVideoClock = NULL;
	cSprite *g_pVideoSprite = 0;
	int g_iVideoImageID = 0;
	BOOL g_bVideoPlaying = FALSE;
	uString sVideoPath;
	float fVideoDuration = 0;
	int iVideoWidth = -1;
	int iVideoHeight = -1;
	float fVideoPos = -1;
	float fVideoVolume = 100.0f;
	volatile unsigned char *pVideoFrame = 0;
	unsigned int *pVideoFrameFinal = 0;
	cSpinLock cVideoLock;
	volatile int iVideoChanged = 0;

	class SampleHandler : public ISampleGrabberCB 
	{
		public:
			SampleHandler() {}
			virtual ~SampleHandler() {}

			STDMETHOD(QueryInterface)(REFIID InterfaceIdentifier, VOID** ppvObject) throw() { return E_NOINTERFACE; } 
			STDMETHOD_(ULONG, AddRef)() throw() { return 2; } 
			STDMETHOD_(ULONG, Release)() throw() { return 1; } 

			STDMETHODIMP SampleCB( double SampleTime, IMediaSample *pSample ) { return S_OK; }

			STDMETHODIMP BufferCB( double SampleTime, BYTE *pBuffer, long BufferLen ) 
			{
				if ( BufferLen == 0 ) return S_OK;
				if ( !pVideoFrame ) return S_OK;

				cAutoSLock autolock( &cVideoLock );
				memcpy( (void*)pVideoFrame, pBuffer, BufferLen );
				iVideoChanged = 1;

				return S_OK;
			}
	};

	SampleHandler *pVideoCallback = 0;

	// Media Foundation sample grabber
	class SampleGrabberCB : public IMFSampleGrabberSinkCallback 
	{
		long m_cRef;

		SampleGrabberCB() : m_cRef(1) {}

		public:
			static HRESULT CreateInstance(SampleGrabberCB **ppCB) { *ppCB = new SampleGrabberCB(); return S_OK; }

			// IUnknown methods
			STDMETHODIMP QueryInterface(REFIID iid, void** ppv)
			{
				static const QITAB qit[] = 
				{
					QITABENT(SampleGrabberCB, IMFSampleGrabberSinkCallback),
					QITABENT(SampleGrabberCB, IMFClockStateSink),
					{ 0 }
				};
				return QISearch(this, qit, iid, ppv);
			}
			STDMETHODIMP_(ULONG) AddRef() { return InterlockedIncrement(&m_cRef); }
			STDMETHODIMP_(ULONG) Release()
			{
				ULONG cRef = InterlockedDecrement(&m_cRef);
				if (cRef == 0) delete this;
				return cRef;
			}

			// IMFClockStateSink methods
			STDMETHODIMP OnClockStart(MFTIME hnsSystemTime, LONGLONG llClockStartOffset) { return S_OK; }
			STDMETHODIMP OnClockStop(MFTIME hnsSystemTime) { return S_OK; }
			STDMETHODIMP OnClockPause(MFTIME hnsSystemTime) { return S_OK; }
			STDMETHODIMP OnClockRestart(MFTIME hnsSystemTime) { return S_OK; }
			STDMETHODIMP OnClockSetRate(MFTIME hnsSystemTime, float flRate) { return S_OK; }

			// IMFSampleGrabberSinkCallback methods
			STDMETHODIMP OnSetPresentationClock(IMFPresentationClock* pClock) { return S_OK; }
			STDMETHODIMP OnProcessSample(REFGUID guidMajorMediaType, DWORD dwSampleFlags,
				LONGLONG llSampleTime, LONGLONG llSampleDuration, const BYTE * pSampleBuffer,
				DWORD dwSampleSize)
			{
				//uString info; info.Format("Sample: start = %I64d, duration = %I64d, bytes = %d", llSampleTime, llSampleDuration, dwSampleSize); 
				//agk::Warning( info );
				//return S_OK;

				if ( dwSampleSize == 0 ) return S_OK;
				if ( !pVideoFrame ) return S_OK;

				cAutoSLock autolock( &cVideoLock );
				memcpy( (void*)pVideoFrame, pSampleBuffer, dwSampleSize );
				iVideoChanged = 1;

				return S_OK;
			}
			STDMETHODIMP OnShutdown() { return S_OK; }
	};

	SampleGrabberCB *g_pVideoSampleGrabber = 0;
	// end of video variables

	// camera streaming
	IFilterGraph2 *g_pCameraGraph = NULL;
	IMediaControl *pCameraMediaControl = 0;
	ISampleGrabber *pCameraSampleGrabber = NULL;
	volatile unsigned char *pCameraFrame = 0;
	unsigned int *pCameraFrameFinal = 0;
	cSpinLock cCameraLock;
	int iCameraWidth = 0;
	int iCameraHeight = 0;
	volatile int iCameraFrameChanged = 0;

	class CameraSampleHandler : public ISampleGrabberCB 
	{
		public:
			CameraSampleHandler() {}
			virtual ~CameraSampleHandler() {}

			STDMETHOD(QueryInterface)(REFIID InterfaceIdentifier, VOID** ppvObject) throw() { return E_NOINTERFACE; } 
			STDMETHOD_(ULONG, AddRef)() throw() { return 2; } 
			STDMETHOD_(ULONG, Release)() throw() { return 1; } 

			STDMETHODIMP SampleCB( double SampleTime, IMediaSample *pSample ) { return S_OK; }

			STDMETHODIMP BufferCB( double SampleTime, BYTE *pBuffer, long BufferLen ) 
			{
				if ( BufferLen == 0 ) return S_OK;
				if ( !pCameraFrame ) return S_OK;

				cAutoSLock autolock( &cCameraLock );
				memcpy( (void*)pCameraFrame, pBuffer, BufferLen );
				iCameraFrameChanged = 1;

				return S_OK;
			}
	};

	CameraSampleHandler *pCameraFrameCallback = 0;
	// end of camera streaming variables

	// DirectInput variables
	IDirectInput8 *g_pDIDevice = NULL;
	HANDLE g_pDirectInputThread = 0;

	// file system variables
	char szAppFolderName[ MAX_PATH ] = "";
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

	cSprite *pTextBackground = 0;

	// fullscreen resolution intermediate FBO
	FrameBuffer *g_pScreenFBO = 0;
}

// Namespace
using namespace AGK;

// file entry class

// build a list of all files in the read and write folders to detect incoorect case sensitive file lookups
// in windows this is not used, but defined anyway to satisfy the compiler.
void cFileEntry::TraverseDirectory( const char* dir )
{
	SetCurrentDirectory( dir );
	
	WIN32_FIND_DATA	FindFileData;
	HANDLE hFind = FindFirstFile ( "*.*", &FindFileData );
	if (hFind == INVALID_HANDLE_VALUE) return;

	do
	{
		if ( strcmp ( FindFileData.cFileName, "." ) != 0 && strcmp ( FindFileData.cFileName, ".." ) != 0 )
		{
			if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				// directory
				char str[ 1024 ];
				GetCurrentDirectory( 1024, str );
				strcat( str, "/" );
				strcat( str, FindFileData.cFileName );
				TraverseDirectory( str );
				SetCurrentDirectory( dir );
				AddNewFile( str );
			}
			else
			{
				// file
				char str[ 1024 ];
				GetCurrentDirectory( 1024, str );
				strcat( str, "/" );
				strcat( str, FindFileData.cFileName );
				AddNewFile( str );
			}
		}
	} while ( FindNextFile ( hFind, &FindFileData ) );

	FindClose( hFind );
}

void cFileEntry::InitFileList()
{
	// this won't work with UTF8 filenames
	/*
	// Windows doesn't need case sensitive help with files, but do it anyway to trap potential errors and provide warnings
	char szRootMedia[ MAX_PATH ];
	strcpy( szRootMedia, szRootDir );
	strcat( szRootMedia, "media/" );

	TraverseDirectory( szRootMedia );
	TraverseDirectory( szWriteDir );
	*/
}

// Setup

int g_iIsAGKFullscreen = 0;

//****f* Core/Display/SetWindowPosition
// FUNCTION
//   This sets the window position for platforms that support it, such as Windows and Mac.
//   If the window is currently full screen then this will have no affect.
//   This will not affect mobile devices.
// INPUTS
//   x -- The desired X position of the window in pixels.
//   y -- The desired Y position of the window in pixels.
// SOURCE
void agk::SetWindowPosition( int x, int y )
//****
{
	if ( !g_bGLInit ) return;
	if ( !g_hWnd ) return;

	// don't set position on a fullscreen window
	if ( GetWindowLong( g_hWnd, GWL_STYLE ) & WS_POPUP ) return;

	::SetWindowPos( g_hWnd, 0, x, y, 0,0, SWP_NOZORDER | SWP_NOSIZE );
}

//****f* Core/Display/SetWindowSize
// FUNCTION
//   This sets the window size for platforms that support it, such as Windows and Mac.
//   This will not affect mobile devices.
//   In fullscreen mode the width and height will be ignored and the desktop resolution will be used.
//   By default the window size will be adjusted so it always fits on screen, including making room
//   for the taskbar on Windows. This means that your final window size may be slightly smaller than
//   your chosen size. You can override this by setting the allowOverSized parameter to 1, which will 
//   remove all bounds checks and allow you to create windows larger than the current screen.
//   When the fullscreen parameter is set to 1 then the allowOverSized parameter has no effect.
// INPUTS
//   width -- The desired width of the window in pixels.
//   height -- The desired height of the window in pixels.
//   fullscreen -- 1 to use a full screen mode, 0 for a normal window.
//   allowOverSized -- 1 to allow the window size to exceed the visible screen, 0 to shrink it to fit where necessary
// SOURCE
void agk::SetWindowSize( int width, int height, int fullscreen )
//****
{
	SetWindowSize( width, height, fullscreen, 0 );
}

//****f* Core/Display/SetWindowSize
// FUNCTION
//   This sets the window size for platforms that support it, such as Windows and Mac.
//   This will not affect mobile devices.
//   In fullscreen mode the width and height will be ignored and the desktop resolution will be used.
//   By default the window size will be adjusted so it always fits on screen, including making room
//   for the taskbar on Windows. This means that your final window size may be slightly smaller than
//   your chosen size. You can override this by setting the allowOverSized parameter to 1, which will 
//   remove all bounds checks and allow you to create windows larger than the current screen.
//   When the fullscreen parameter is set to 1 then the allowOverSized parameter has no effect.
// INPUTS
//   width -- The desired width of the window in pixels.
//   height -- The desired height of the window in pixels.
//   fullscreen -- 1 to use a full screen mode, 0 for a normal window.
//   allowOverSized -- 1 to allow the window size to exceed the visible screen, 0 to shrink it to fit where necessary
// SOURCE
void agk::SetWindowSize( int width, int height, int fullscreen, int allowOverSized )
//****
{
	if ( !g_bGLInit ) return;
	if ( !g_hWnd ) return;

	if ( g_iDefaultWidth == 0 ) g_iDefaultWidth = agk::GetDeviceWidth();
	if ( g_iDefaultHeight == 0 ) g_iDefaultHeight = agk::GetDeviceHeight();

	static int oldX = 0;
	static int oldY = 0;

	if ( fullscreen == 0 )
	{
		RECT rc;
		GetWindowRect(GetDesktopWindow(), &rc);

		RECT currRect;
		GetWindowRect(g_hWnd, &currRect);

		if ( width < 0 ) width = 0;
		if ( height < 0 ) height = 0;

		if ( width == 0 ) width = g_iDefaultWidth;
		if ( height == 0 ) height = g_iDefaultHeight;

		if ( width == 0 ) width = agk::GetMaxDeviceWidth();
		if ( height == 0 ) height = agk::GetMaxDeviceHeight();

		if ( allowOverSized == 0 )
		{
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
		}

		// center window
		/*
		int x = ((rc.right-rc.left)-width)/2 + rc.left;
		int y = ((rc.bottom-rc.top-50)-height)/2 + rc.top;
		*/

		// keep window position the same if possible
		int x = currRect.left;
		int y = currRect.top;

		if ( g_iIsAGKFullscreen )
		{
			x = oldX;
			y = oldY;
		}

		int vLeft = GetSystemMetrics( SM_XVIRTUALSCREEN );
		int vTop = GetSystemMetrics( SM_YVIRTUALSCREEN );
		int vRight = GetSystemMetrics( SM_CXVIRTUALSCREEN ) + vLeft;
		int vBottom = GetSystemMetrics( SM_CYVIRTUALSCREEN ) + vTop;

		if ( x+width > vRight ) x = vRight - width;
		if ( y+height > vBottom ) x = vBottom - height;

		if ( x < vLeft ) x = vLeft;
		if ( y < vTop ) y = vTop;

		LONG lStyle = WS_OVERLAPPEDWINDOW; // must always have WS_SIZEBOX for the title bar to look right
				
		RECT WindowRect;							
		WindowRect.left=(long)0;					
		WindowRect.right=(long)width;				
		WindowRect.top=(long)0;						
		WindowRect.bottom=(long)height;	
		AdjustWindowRectEx(&WindowRect, lStyle, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

		if ( GetWindowLong( g_hWnd, GWL_STYLE ) & WS_POPUP )
		{
			// currently fullscreen
			SetWindowLongPtr( g_hWnd, GWL_STYLE, lStyle | WS_VISIBLE);
			SetWindowLongPtr( g_hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE );
			CompositionChanged();
		}

		::SetWindowPos( g_hWnd, 0, x, y, WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top, SWP_NOZORDER );

		g_iIsAGKFullscreen = 0;

		if ( m_bUsingVSync ) agk::SetVSync( 1 );
	}
	else
	{
		if ( g_iIsAGKFullscreen ) return;
		g_iIsAGKFullscreen = 1;

		RECT rc;
		GetWindowRect(GetDesktopWindow(), &rc);

		RECT currRect;
		GetWindowRect(g_hWnd, &currRect);

		oldX = currRect.left;
		oldY = currRect.top;

		RECT WindowRect;							
		WindowRect.left=(long)0;					
		WindowRect.right=(long)rc.right-rc.left;				
		WindowRect.top=(long)0;						
		WindowRect.bottom=(long)rc.bottom-rc.top;

		int changed = 0;
		if ( !(GetWindowLong( g_hWnd, GWL_STYLE ) & WS_POPUP) )
		{
			// currently not fullscreen
			SetWindowLongPtr( g_hWnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE );
			SetWindowLongPtr( g_hWnd, GWL_EXSTYLE, WS_EX_APPWINDOW );
			CompositionChanged();
			changed = 1;
		}

		if ( changed 
		  && WindowRect.right-WindowRect.left == currRect.right-currRect.left
		  && WindowRect.bottom-WindowRect.top == currRect.bottom-currRect.top )
		{
			// force resize message
			::SetWindowPos( g_hWnd, 0, WindowRect.left, WindowRect.top, WindowRect.right-WindowRect.left-1, WindowRect.bottom-WindowRect.top-1, SWP_NOZORDER | SWP_FRAMECHANGED );
			::SetWindowPos( g_hWnd, 0, WindowRect.left, WindowRect.top, WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top, SWP_NOZORDER | SWP_FRAMECHANGED );
		}
		else
		{
			::SetWindowPos( g_hWnd, 0, WindowRect.left, WindowRect.top, WindowRect.right-WindowRect.left, WindowRect.bottom-WindowRect.top, SWP_NOZORDER | SWP_FRAMECHANGED );
		}

		if ( m_bUsingVSync ) agk::SetVSync( 1 );
	}
}

//****f* Core/Display/SetWindowAllowResize
// FUNCTION
//   This sets the window sets whether the user is allowed to resize the window, by default this is set to 0
// INPUTS
//   mode -- 0 to disable resizing, 1 to allow resizing
// SOURCE
void agk::SetWindowAllowResize( int mode )
//****
{
	if ( !g_hWnd ) return;
	if ( mode == 0 ) m_bAGKFlags &= ~AGK_FLAG_CAN_RESIZE;
	else m_bAGKFlags |= AGK_FLAG_CAN_RESIZE;

	LONG lStyle = GetWindowLong( g_hWnd, GWL_STYLE );
	if ( lStyle & WS_POPUP ) return;

	if ( mode == 0 ) ::SetWindowLongPtr( g_hWnd, GWL_STYLE, (lStyle & ~(WS_MAXIMIZEBOX)) );  // must always have WS_SIZEBOX for the title bar to look right
	else ::SetWindowLongPtr( g_hWnd, GWL_STYLE, (lStyle | WS_MAXIMIZEBOX) );
}

//****f* Core/Display/MaximizeWindow
// FUNCTION
//   Maximizes the window so that it is the largest possible size in the current visible space without going full screen.
//   Does nothing on mobile devices.
// SOURCE
void agk::MaximizeWindow()
//****
{
	if ( !g_hWnd ) return;

	LONG lStyle = GetWindowLong( g_hWnd, GWL_STYLE );
	if ( lStyle & WS_POPUP ) return;

	ShowWindow( g_hWnd, SW_MAXIMIZE );
}

//****f* Core/Display/MinimizeApp
// FUNCTION
//   Sends the app to the background. On desktop platforms like Windows the app will continue to run when 
//   minimized. On Android the app will pause. On iOS it will do nothing as a user must minimize the app
//   by pressing the home button. On desktop platforms the window can be restored with <i>RestoreApp</i>.
// SOURCE
void agk::MinimizeApp()
//****
{
	if ( !g_hWnd ) return;

	ShowWindow( g_hWnd, SW_SHOWMINIMIZED );
}

//****f* Core/Display/RestoreApp
// FUNCTION
//   Restores the app from a minimized state to a normal window.
// SOURCE
void agk::RestoreApp()
//****
{
	if ( !g_hWnd ) return;

	ShowWindow( g_hWnd, SW_SHOWNORMAL );
}

//****f* Core/Display/SetImmersiveMode
// FUNCTION
//   Removes any on screen controls that may have been placed on screen by the OS, for example the home and 
//   back buttons, allowing your app to use the entire screen. Your app may be resized after calling this 
//   command so if you have used GetDeviceWidth or GetDeviceHeight you should wait a few frames then call 
//   them again to get the new values. The values may not change immediately after calling this command as 
//   it takes some time for the navigation bar to change. Sometimes the OS will turn off immersive mode 
//   without warning, for example if the volume buttons are pressed. When this happens your app will return 
//   to its non-immersive mode size to prevent any part of the app being hidden by the nav bar, which could
//   make some parts of the app inaccessible to touch events. You can detect this change by checking 
//   <i>GetDeviceWidth</i> and <i>GetDeviceHeight</i> at regular intervals, and use SetImmersiveMode again 
//   to request a return to immersive mode.<br/><br/>
//   Currently this only applies to Android devices running 4.4 (API 19) or higher. On other platforms this 
//   command does nothing.
// INPUTS
//   mode -- 1 to turn on immersive mode, 0 to turn it off
// SOURCE
void agk::SetImmersiveMode( int mode )
//****
{
	
}

//****f* Core/Display/SetScreenResolution
// FUNCTION
//   Sets the resolution of the backbuffer where possible. If your chosen resolution is greater than the 
//   device can display than the device will use its highest possible resolution, you can use 
//   <i>GetDeviceWidth</i> and <i>GetDeviceHeight</i> to find out what was used.
//   On mobile devices using a screen resolution with a different aspect ratio than the device will result 
//   in stretching. You can check the aspect ratio of the device with GetMaxDeviceWidth and GetMaxDeviceHeight.
//   It is not recommended that you allow both portrait and landscape orientations when using this command
//   as AGK cannot prevent stretching without undoing your desired screen resolution so it will not do so. 
//   If you want all orientations without stretching you will need to monitor GetMaxDeviceWidth and 
//   GetMaxDeviceHeight for changes and set an appropriate replacement resolution with SetScreenResolution
//   to match the new aspect ratio.
//   Using a resolution of 0 in either direction will reset the screen resolution to the maximum resolution 
//   supported on the device.
//   Note that on Windows and Mac the backbuffer size is changed by modifying the size of the window, so it
//   will overwrite any settings from <i>SetWindowSize</i>. This may not be the case in the future and AGK 
//   may be able to have separate window sizes and backbuffer sizes in a future version.
// INPUTS
//   width -- The desired width of the screen in pixels.
//   height -- The desired height of the screen in pixels.
// SOURCE
void agk::SetScreenResolution( int width, int height )
//****
{
	agk::SetWindowSize( width, height, 0 );
}

//****f* Core/Misc/GetURLSchemeText
// FUNCTION
//   On Android and iOS this returns the full URL that was used to open this app if a URL scheme was used. For example if you 
//   have set the URL scheme "myapp" for this app and the user clicks on a link such as "myapp://sometext", then the OS will
//   open your app and GetURLSchemeText will return "myapp://sometext" until the app is next opened. If a URL was not used
//   to open the app, or the platform doesn't support URL schemes, then an empty string will be returned.<br/><br/>
//   When choosing a URL scheme you must make sure it is unique to your app, as iOS will not allow two apps to have the same
//   scheme.
// SOURCE
char* agk::GetURLSchemeText()
//****
{
	char* str = new char[1]; *str = 0;
	return str;
}

//****f* Core/Misc/ClearURLSchemeText
// FUNCTION
//   Clears the currently stored URL scheme text so you can signal that you have acted upon it. This is not necessary but
//   can make your code easier by not having to remember that you have dealt with a URL scheme event.
// SOURCE
void agk::ClearURLSchemeText()
//****
{

}

void agk::GetDeviceName( uString &outString )
{
	outString.SetStr( "windows" );
}

//****f* Core/Misc/GetDeviceName
// FUNCTION
//   This command is deprecated, please use GetDeviceBaseName and GetDeviceType instead.
//
//   Returns a string containing the name of the current platform. e.g. "windows" or "bada".
//   When dealing with iOS more information is returned e.g. on iPad 1 "ios|iPad1,1" and on
//   iPod 2nd gen: "ios|iPod2,1" and on iPhone 3G: "ios|iPhone1,2". The first number is the major
//   model number, the second is the minor model number.
// SOURCE
char* agk::GetDeviceName( )
//****
{
	char *str = new char[ 256 ];
	strcpy( str, "windows" );
	return str;
}

//****f* Core/Misc/GetDeviceBaseName
// FUNCTION
//   Returns a string containing the name of the current platform. This will only refer to the base
//   platform, i.e. "windows", "ios", "android", "mac", or "linux". Use <i>GetDeviceType</i> to 
//   get more information about the specific device. This string will always be lower case.
// SOURCE
char* agk::GetDeviceBaseName()
//****
{
	char *str = new char[ 20 ];
	strcpy( str, "windows" );
	return str;
}

//****f* Core/Misc/GetDeviceType
// FUNCTION
//   Returns a string containing a device specific string, this will be different for each platform,
//   for example on Windows it will return the OS version "vista, "7", "xp", etc. On iOS it will return 
//   the iOS device name "ipad1,1", "ipod2,1", "iphone1,2", etc.  For Android it will return the device
//   model "nexus 7", etc. For Mac it will return the OS version "10.7", "10.8", etc.
//   This string will always be lower case
// SOURCE
char* agk::GetDeviceType()
//****
{
	char *str = new char[ 30 ];
	strcpy( str, "unknown" );

	RTL_OSVERSIONINFOEXW osvi;
    ZeroMemory(&osvi, sizeof(RTL_OSVERSIONINFOEXW));
    osvi.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);

    //GetVersionEx(&osvi); // doesn't work on Windows 8.1 and Windows 10

	typedef LONG (WINAPI* tRtlGetVersion)(RTL_OSVERSIONINFOEXW*);
    HMODULE h_NtDll = GetModuleHandle("ntdll.dll");
    tRtlGetVersion f_RtlGetVersion = (tRtlGetVersion)GetProcAddress(h_NtDll, "RtlGetVersion");
 
    if (!f_RtlGetVersion) return str; // This will never happen (all processes load ntdll.dll)
    
	LONG Status = f_RtlGetVersion(&osvi);

	if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 ) strcpy( str, "xp" );
	else if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 ) strcpy( str, "2003" );
	else if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0 ) strcpy( str, "vista" );
	else if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1 ) strcpy( str, "7" );
	else if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2 ) strcpy( str, "8" );
	else if ( osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3 ) strcpy( str, "8.1" );
	else if ( osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0 ) strcpy( str, "10" );

	return str;
}

//****f* Core/Misc/GetDeviceNetworkType
// FUNCTION
//   Returns the type of network connection the device has to the internet. Returns 0 if a mobile connection
//   is being used, 1 if a Wifi or Ethernet connection is being used, or -1 if the network type could not be
//   determined. Currently only implemented on iOS and Android, other platforms will return -1.
// SOURCE
int agk::GetDeviceNetworkType()
//****
{
	return -1;
}

//****f* Core/Misc/GetStorageRemaining
// FUNCTION
//   Returns the number of MB available to the app for data storage at the specified path. For example using a 
//   path that points to a location on the sdcard on Android will return how many MBs can be stored there, 
//   whereas using a path that points to the AGK write folder will return the number of MB the app can store
//   in its internal storage. In some cases these may be the same. This command returns -1 if the space 
//   available could not be determined. Currently only implemented on iOS and Android, other platforms will 
//   return -1.
// INPUTS
//   path -- The path to check
// SOURCE
int agk::GetStorageRemaining( const char *path )
//****
{
	return -1;
}

//****f* Core/Misc/GetStorageTotal
// FUNCTION
//   Returns the total number of MB of data storage at the specified path, used and unused. For example using a 
//   path that points to a location on the sdcard on Android will return the size of the sdcard, whereas using 
//   a path that points to the AGK write folder will return the number of MB in the internal storage. In 
//   some cases these may be the same. This command returns -1 if the storage size could not be determined. 
//   Currently only implemented on iOS and Android, other platforms will return -1.
// INPUTS
//   path -- The path to check
// SOURCE
int agk::GetStorageTotal( const char *path )
//****
{
	return -1;
}

void agk::GetAppName( uString &outString )
{
	outString.SetStr( m_sAppName );
}

//****f* Core/Misc/GetAppName
// FUNCTION
//   Returns the name of the exe.
// SOURCE
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

//****f* Core/Misc/GetDeviceLanguage
// FUNCTION
//   Returns a string containing the language of the current device in the form "en", "fr", "de", etc.
// SOURCE
char* agk::GetDeviceLanguage( )
//****
{
	char *str = new char[ 5 ];
	GetLocaleInfo( LOCALE_SYSTEM_DEFAULT, LOCALE_SISO639LANGNAME, str, 5 );
	return str;
}

//****f* Core/Misc/SetSleepMode
// FUNCTION
//   Sets whether the device should sleep when your app is idle. By default this is turned off,
//   but if activated it will use the device settings to determine how long it should idle before 
//   sleeping.
// INPUTS
//   mode -- 1 to enable sleep mode, 0 to turn it off (default)
// SOURCE
void agk::SetSleepMode( int mode )
//****
{
	// do nothing
}



//****f* Core/Misc/SetExpansionFileKey
// FUNCTION
//   Sets the public key to use in the other expansion file commands. Currently only applicable to
//   Android. The public key can be found in the Google Play Developer Console and is different for 
//   each app. Click on the app and then click on the Services and APIs section, it is the long 
//   string that starts MIIB.
// INPUTS
//   key -- The public key to use when checking for and downloading an expansion file.
// SOURCE
void agk::SetExpansionFileKey( const char *key )
//****
{
	// do nothing on windows
}

//****f* Core/Misc/SetExpansionFileVersion
// FUNCTION
//   Sets the version number to use in the other expansion file commands. Currently only applicable to
//   Android. The version number of the expansion file is determined by the version number of the APK
//   that was uploaded with it. For example the APK may be updated to a new version number but the 
//   expansion file could still be the original version number.
// INPUTS
//   version -- The version number to use when checking for and downloading an expansion file.
// SOURCE
void agk::SetExpansionFileVersion(int version)
//****
{
	// do nothing on windows
}

//****f* Core/Misc/GetExpansionFileState
// FUNCTION
//   Returns the state of any expansion file used by this platform, currently only applicable to Android.
//   Returns -1 if an error occurred during download, 0 if the expansion file is not used on this platform, 
//   1 if it should exist but doesn't (you call DownloadExpansionFile() in this case), 2 if it is currently 
//   being downloaded, and 3 if everything has completed and the file exists.
// SOURCE
int agk::GetExpansionFileState()
//****
{
	return 0;
}

//****f* Core/Misc/GetExpansionFileError
// FUNCTION
//   Returns the error code of the most recent error that occurred when downloading the expansion file. Possible errors
//   include:<br/>
//      15 = Unlicensed<br/>
//      16 = Failed fetching URL<br/>
//      17 = SDcard full<br/>
//      18 = Cancelled<br/>
//      19 = Unknown error
// SOURCE
int agk::GetExpansionFileError()
//****
{
	return 0;
}

//****f* Core/Misc/DownloadExpansionFile
// FUNCTION
//   Starts the download of any expansion file that this platform and app needs, currently only applicable
//   to Android. If the download has already started this has no effect. You can use <i>GetExpansionFileProgress</i>
//   to check on the progress of the download and <i>GetExpanionFileState</i> to check when it has completed.
//   After calling this command you should wait for <i>GetExpanionFileState</i> to return -1 (error) or 3 (complete)
//   whilst displaying a progress bar.
// SOURCE
void agk::DownloadExpansionFile()
//****
{
	// do nothing on windows
}

//****f* Core/Misc/GetExpansionFileProgress
// FUNCTION
//   Checks the progress of a download started by DownloadExpansionFile, returns a float value between 0 and 100
//   Use GetExpansionFileState to check if the download has completed.
// SOURCE
float agk::GetExpansionFileProgress()
//****
{
	return 0;
}

bool agk::ExtractExpansionFile( const char* localFile, const char* expansionFile )
{
	return false;
}

//****f* Core/Misc/SetWindowTitle
// FUNCTION
//   Sets the window title for those platforms that run in a window (e.g. Windows, Mac).
// INPUTS
//   szTitle -- The text to use for the window title.
// SOURCE
void agk::SetWindowTitle( const char *szTitle )
//****
{
	if ( !g_bGLInit ) return;
	if ( !g_hWnd ) return;

	int iLength = MultiByteToWideChar( CP_UTF8, 0, szTitle, -1, 0, 0 );
	WCHAR *wstr = new WCHAR[ iLength + 1 ];
	MultiByteToWideChar( CP_UTF8, 0, szTitle, -1, wstr, iLength+1 );

	SetWindowTextW( g_hWnd, wstr );

	delete [] wstr;
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
	// do nothing on Windows
}

bool agk::PlatformGetDeviceID( uString &out )
{
	DWORD serial;
	char *drive = GetWindowsDrive( 0 );
	if ( !drive ) GetVolumeInformation( "C:\\", NULL, 0, &serial, NULL, NULL, NULL, 0 );
	else 
	{
		uString sDrive( drive );
		sDrive.AppendAscii( '\\' );
		GetVolumeInformation( sDrive.GetStr(), NULL, 0, &serial, NULL, NULL, NULL, 0 );
		delete [] drive;
	}

	unsigned int result[ 5 ];

	SHA1 sha;
	sha.Input( (unsigned char*)&serial, 4 );
	sha.Result( &(result[0]) );
	out.Format( "%08X%08X%08X%08X%08X", result[0], result[1], result[2], result[3], result[4] );

	return true;
}

float agk::PlatformDeviceScale()
{
	return 1.0f;
}

int agk::PlatformGetNumProcessors()
{
	//return 1;
	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );

	return sysinfo.dwNumberOfProcessors;
}

BOOL IsXInputDevice( const GUID* pGuidProductFromDirectInput )
{
    IWbemLocator*           pIWbemLocator  = NULL;
    IEnumWbemClassObject*   pEnumDevices   = NULL;
    IWbemClassObject*       pDevices[20]   = {0};
    IWbemServices*          pIWbemServices = NULL;
    BSTR                    bstrNamespace  = NULL;
    BSTR                    bstrDeviceID   = NULL;
    BSTR                    bstrClassName  = NULL;
    DWORD                   uReturned      = 0;
    bool                    bIsXinputDevice= false;
    UINT                    iDevice        = 0;
    VARIANT                 var;
    HRESULT                 hr;

    // CoInit if needed
	if ( !g_bCoInit ) 
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		g_bCoInit = true;
	}

    // Create WMI
    hr = CoCreateInstance( __uuidof(WbemLocator),
                           NULL,
                           CLSCTX_INPROC_SERVER,
                           __uuidof(IWbemLocator),
                           (LPVOID*) &pIWbemLocator);
    if( FAILED(hr) || pIWbemLocator == NULL )
        goto LCleanup;

    bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" );if( bstrNamespace == NULL ) goto LCleanup;        
    bstrClassName = SysAllocString( L"Win32_PNPEntity" );   if( bstrClassName == NULL ) goto LCleanup;        
    bstrDeviceID  = SysAllocString( L"DeviceID" );          if( bstrDeviceID == NULL )  goto LCleanup;        
    
    // Connect to WMI 
    hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L, 
                                       0L, NULL, NULL, &pIWbemServices );
    if( FAILED(hr) || pIWbemServices == NULL )
        goto LCleanup;

    // Switch security level to IMPERSONATE. 
    CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, 
                       RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE );                    

    hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices ); 
    if( FAILED(hr) || pEnumDevices == NULL )
        goto LCleanup;

    // Loop over all devices
    for( ;; )
    {
        // Get 20 at a time
        hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
        if( FAILED(hr) )
            goto LCleanup;
        if( uReturned == 0 )
            break;

        for( iDevice=0; iDevice<uReturned; iDevice++ )
        {
            // For each device, get its device ID
            hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );
            if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL )
            {
                // Check if the device ID contains "IG_".  If it does, then it's an XInput device
				    // This information can not be found from DirectInput 
                if( wcsstr( var.bstrVal, L"IG_" ) )
                {
                    // If it does, then get the VID/PID from var.bstrVal
                    DWORD dwPid = 0, dwVid = 0;
                    WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
                    if( strVid && swscanf( strVid, L"VID_%4X", &dwVid ) != 1 )
                        dwVid = 0;
                    WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
                    if( strPid && swscanf( strPid, L"PID_%4X", &dwPid ) != 1 )
                        dwPid = 0;

                    // Compare the VID/PID to the DInput device
                    DWORD dwVidPid = MAKELONG( dwVid, dwPid );
                    if( dwVidPid == pGuidProductFromDirectInput->Data1 )
                    {
                        bIsXinputDevice = true;
                        goto LCleanup;
                    }
                }
            }   
            SAFE_RELEASE( pDevices[iDevice] );
        }
    }

LCleanup:
    if(bstrNamespace)
        SysFreeString(bstrNamespace);
    if(bstrDeviceID)
        SysFreeString(bstrDeviceID);
    if(bstrClassName)
        SysFreeString(bstrClassName);
    for( iDevice=0; iDevice<20; iDevice++ )
        SAFE_RELEASE( pDevices[iDevice] );
    SAFE_RELEASE( pEnumDevices );
    SAFE_RELEASE( pIWbemLocator );
    SAFE_RELEASE( pIWbemServices );
	
    return bIsXinputDevice;
}

BOOL __stdcall InputEnumObjectsCallback( LPCDIDEVICEOBJECTINSTANCE pObject, void* pRef )
{
	IDirectInputDevice8 *pDevice = (IDirectInputDevice8*) pRef;

	if ( (pObject->dwType & DIDFT_ABSAXIS) != 0 )
    {
    	DIPROPRANGE range;
    	range.diph.dwSize = sizeof(DIPROPRANGE);
    	range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    	range.diph.dwHow = DIPH_BYID;
    	range.diph.dwObj = pObject->dwType;
		range.lMax = 32768;
		range.lMin = -32767;
    	if ( pDevice->SetProperty(DIPROP_RANGE, &range.diph) != DI_OK )
    	{
			agk::Warning( "Failed to set DirectInput axis range" );
    	}
    }
		
	return DIENUM_CONTINUE;
}

BOOL __stdcall InputEnumCallback( LPCDIDEVICEINSTANCE device, void* ptr )
{
	if( IsXInputDevice( &device->guidProduct ) )
        return DIENUM_CONTINUE;

	//agk::Warning( device->tszProductName );
	cJoystick **pJoysticks = (cJoystick**) ptr;

	int type = device->dwDevType & 0xff;
	if ( type == DI8DEVTYPE_JOYSTICK || type == DI8DEVTYPE_GAMEPAD || type == DI8DEVTYPE_FLIGHT || type == DI8DEVTYPE_DRIVING || type == DI8DEVTYPE_1STPERSON )
	{
		UINT iIndex = 0;
		while ( iIndex < AGK_NUM_JOYSTICKS && pJoysticks[ iIndex ] ) iIndex++;
		if ( iIndex >= AGK_NUM_JOYSTICKS ) 
		{
			return DIENUM_STOP;
		}

		IDirectInputDevice8* pDevice = 0;
		HRESULT hr = g_pDIDevice->CreateDevice( device->guidInstance, &pDevice, NULL );
		if ( hr != DI_OK )
		{
			uString err( "Failed to create DirectInput device: " );
			err += device->tszProductName;
			agk::Warning( err );
			return DIENUM_CONTINUE;
		}

		cJoystick *pJoystick = new cJoystick( pDevice );
		pJoystick->SetName( device->tszProductName );

		pDevice->EnumObjects( InputEnumObjectsCallback, pDevice, DIDFT_ALL );
		pDevice->SetDataFormat( &c_dfDIJoystick2 );
		if ( g_bGLInit && g_hWnd ) pDevice->SetCooperativeLevel( g_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND );

		pDevice->Acquire();

		pJoysticks[ iIndex ] = pJoystick;
	}

	return DIENUM_CONTINUE;
}

UINT STDCALL DirectInputEnumThread( void *pParams )
{
	HRESULT hr = g_pDIDevice->EnumDevices( DI8DEVCLASS_GAMECTRL, InputEnumCallback, pParams, DIEDFL_ATTACHEDONLY ); 
	if ( hr != DI_OK )
	{
		agk::Warning( "Failed to enumerate DirectInput devices" );
	}

	cJoystick **pJoysticks = (cJoystick**) pParams;

	for (DWORD i = 0; i < 4; i++)
	{
		XINPUT_STATE state;
		DWORD result = XInputGetState( i, &state );
		if ( result == ERROR_SUCCESS )
		{
			UINT iIndex = 0;
			while ( iIndex < AGK_NUM_JOYSTICKS && pJoysticks[ iIndex ] ) iIndex++;
			if ( iIndex >= AGK_NUM_JOYSTICKS ) return 0;

			cJoystick *pJoystick = new cJoystick( (void*) i, 1 );
			pJoystick->SetName( "XInput Device" );
			
			pJoysticks[ iIndex ] = pJoystick;
		}
	}

	return 0;
}

void agk::PlatformInitFilePaths()
{
	wchar_t wzString[ MAX_PATH ];

	// generate app folder using exe name
	HMODULE hModule = GetModuleHandle( NULL );
	char szModule[ MAX_PATH ] = "";
	char szDrive[ 10 ] = "";
	char szDir[ MAX_PATH ] = "";
	char szEXE[ MAX_PATH ] = "";
	GetModuleFileNameW( hModule, wzString, MAX_PATH );
	WideCharToMultiByte( CP_UTF8, 0, wzString, -1, szModule, MAX_PATH, 0, 0 );

	_splitpath_s( szModule, szDrive, 10, szDir, MAX_PATH, szEXE, MAX_PATH, NULL, 0 );
	strcpy( szRootDir, szDrive );
	strcat( szRootDir, szDir ); 

	// 150811 - remove media as the root directory for windows.
	// strcat( szRootDir, "media\\" );
	// the decision is that T2 (C++) has no contrived media folder, but T1 (BASIC) uses a media folder
	// and this media folder is added to the root when you first run the AGK Player application

	if ( m_sAppName.GetLength() == 0 ) m_sAppName.SetStr( szEXE );

	// get my documents folder
	if ( !SHGetSpecialFolderPathW( NULL, wzString, /*CSIDL_MYDOCUMENTS*/ CSIDL_LOCAL_APPDATA, TRUE ) )
	{
		// if no my documents folder use exe location
		strcpy( szWriteDir, szRootDir );
	}

	WideCharToMultiByte( CP_UTF8, 0, wzString, -1, szWriteDir, MAX_PATH, 0, 0 );

	//uString sModule( szModule );
	uString sModule( szEXE ); // make documents path simpler
	sModule.Replace( '\\', '_' );
	sModule.Replace( '/', '_' );
	sModule.Strip( "\\/.: \t*" );

	if ( sModule.GetNumChars() > 30 ) sModule.Trunc2( sModule.GetNumChars()-30 );

	strcpy( szAppFolderName, sModule.GetStr() );

	// construct full write folder from my documents folder
	strcat_s( szWriteDir, MAX_PATH, "/AGKApps/" );
	if ( m_sCompanyName.GetLength() > 0 )
	{
		strcat_s( szWriteDir, MAX_PATH, m_sCompanyName.GetStr() );
		strcat_s( szWriteDir, MAX_PATH, "/" );
	}
	strcat_s( szWriteDir, MAX_PATH, szAppFolderName );
	strcat_s( szWriteDir, MAX_PATH, "/" );

	if ( !PlatformCreateRawPath( szWriteDir ) ) 
	{
		uString err; err.Format( "Failed to create write path: %s", szWriteDir );
		agk::Error( err );
	}

	char *str = szWriteDir;
	while ( *str )
	{
		if ( *str == '\\' ) *str = '/';
		str++;
	}

	str = szRootDir;
	while ( *str )
	{
		if ( *str == '\\' ) *str = '/';
		str++;
	}

	SetCurrentDirectoryUTF8( szRootDir );

	// make sure this value is set here incase restore is called without changing path
	strcpy_s( szOriginalWriteDir, MAX_PATH, szWriteDir );

	//ParseCurrentDirectory();
	m_bUpdateFileLists = true;
}

void agk::PlatformUpdateWritePath()
{
	// get my documents folder
	wchar_t wstr[ 1024 ];
	if ( !SHGetSpecialFolderPathW( NULL, wstr, /*CSIDL_MYDOCUMENTS*/ CSIDL_LOCAL_APPDATA, TRUE ) )
	{
		return;
	}

	char str[ 1024 ];
	WideCharToMultiByte( CP_UTF8, 0, wstr, -1, str, 1024, 0, 0 );

	strcpy_s( szWriteDir, MAX_PATH, str );
	strcat_s( szWriteDir, MAX_PATH, "/AGKApps/" );
	if ( m_sCompanyName.GetLength() > 0 )
	{
		strcat_s( szWriteDir, MAX_PATH, m_sCompanyName.GetStr() );
		strcat_s( szWriteDir, MAX_PATH, "/" );
	}
	strcat_s( szWriteDir, MAX_PATH, szAppFolderName );
	strcat_s( szWriteDir, MAX_PATH, "/" );
	if ( m_sAppName.GetLength() > 0 )
	{
		uString sModule( m_sAppName ); // make documents path simpler
		sModule.Replace( '\\', '_' );
		sModule.Replace( '/', '_' );
		sModule.Strip( "\\/.: \t*" );
		strcat_s( szWriteDir, MAX_PATH, sModule );
		strcat_s( szWriteDir, MAX_PATH, "/" );
	}

	if ( !PlatformCreateRawPath( szWriteDir ) ) 
	{
		uString err; err.Format( "Failed to create write path: %s", szWriteDir );
		agk::Error( err );
	}

	char *str2 = szWriteDir;
	while ( *str2 )
	{
		if ( *str2 == '\\' ) *str2 = '/';
		str2++;
	}

	SetCurrentDirectoryUTF8( szRootDir );
	m_bUpdateFileLists = true;
}

void agk::PlatformRestoreWritePath()
{
	if ( strlen ( szOriginalWriteDir ) > 0 ) strcpy_s( szWriteDir, MAX_PATH, szOriginalWriteDir );

	//ParseCurrentDirectory();
	m_bUpdateFileLists = true;
}

void agk::OverrideDirectories( const char *szWrite, int useRead )
{
	if ( szWrite && *szWrite )
	{
		strcpy_s( szWriteDir, MAX_PATH, szWrite );
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
	g_hWnd = (HWND) ptr;
}

void agk::InitJoysticks()
{
	
}

void agk::PlatformInitCommon()
{
	g_bMFPlatExists = false;

	char* szType = GetDeviceType();
	if ( strcmp(szType, "7") != 0 && strcmp(szType, "vista") != 0 && strcmp(szType, "2003") != 0 && strcmp(szType, "xp") != 0 )
	{
		HMODULE hMFPlat = LoadLibrary( "mfplat.dll" );
		if ( hMFPlat ) 
		{
			FreeLibrary( hMFPlat );
			g_bMFPlatExists = true;
		}
	}

	if ( !g_bCoInit ) 
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		g_bCoInit = true;
	}

	if ( g_bMFPlatExists ) MFStartup( MF_VERSION );

	// update timer var
	QueryPerformanceCounter ( (LARGE_INTEGER*) &i64StartTime );
	QueryPerformanceFrequency ( (LARGE_INTEGER*) &i64TimeFreq );
	if ( i64TimeFreq == 0 ) i64TimeFreq = 1;

	SetRandomSeed((unsigned int) i64StartTime);
	
	// network setup
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
#ifdef _AGK_ERROR_CHECK
		agk::Error("WSAStartup failed");
#endif
		return;
	}

	// load delayed functions
	if ( !g_hUser32 )
	{
		g_hUser32 = LoadLibrary( "USER32.dll" );
		IsTouchWindowDelayed = (BOOL(__stdcall *)(HWND, PULONG )) GetProcAddress( g_hUser32, "IsTouchWindow" );
	}

	// direct input setup
	HINSTANCE hi = GetModuleHandle(0);
	HRESULT hr = DirectInput8Create( hi, DIRECTINPUT_VERSION, IID_IDirectInput8A, (void**)&g_pDIDevice, NULL );
	if ( hr != DI_OK )
	{
		agk::Warning( "Failed to initialise DirectInput" );
	}
	else
	{
		UINT iThreadID = 0;
		g_pDirectInputThread = (HANDLE) _beginthreadex( NULL, 0, DirectInputEnumThread, m_pJoystick, 0, &iThreadID );
		/*
		hr = g_pDIDevice->EnumDevices( DI8DEVCLASS_GAMECTRL, InputEnumCallback, m_pJoystick, DIEDFL_ATTACHEDONLY ); 
		if ( hr != DI_OK )
		{
			agk::Warning( "Failed to enumerate DirectInput devices" );
		}
		*/
	}

	AGKShader::CreateDefaultShaders();

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
	pTextBackground->SetSize( m_iDisplayWidth+m_iDisplayExtraX*2, m_iDisplayHeight+m_iDisplayExtraY*2 );
	pTextBackground->SetPosition( -m_iDisplayExtraX, -m_iDisplayExtraY );
	pTextBackground->FixToScreen(1);

	// Initialise Ultrabook command code
	InitialiseSensorCommands();
}

void agk::PlatformInitExternal( void* ptr, int width, int height, void(*swap)(void*), void* swapParam )
{
	if ( !swap ) agk::Warning( "AGK was initialised with InitExternal but no swap function was given" );

	g_hWnd = 0;
	g_bIsExternal = true;
	SwapExternal = swap;
	g_pSwapParam = swapParam;

	// Windows location services produces a dialog 
	// which must happen BEFORE OpenGL owns window
	// so call dummy GeoLocation command now to
	// trigger one time permission request
	// 24-05-13 people complaining that all Windows apps will ask for a default location to be set which is suspicious for apps that don't use it
	//float fIgnoreValue = GetRawGeoLatitude();

	if ( g_bGLInit ) return;
	g_bGLInit = true;

	if ( !m_pMouse[ 0 ] ) m_pMouse[ 0 ] = new cMouse();

	if ( !LoadRendererExtensions() )
	{
		agk::Message( "Your graphics card does not support one or more OpenGL features required by this program. Please check if there are any driver updates available for your card." );
		exit(0);
	}

	cImage::GetMaxTextureSize(); // preloads the value so that any threads don't call the GL function behind it
	agk::PlatformSetAlignment( 1 ); // not using memory alignment

	// store device and display width and height and aspect
	m_iRenderWidth = width;
	m_iRenderHeight = height;
	cCamera::UpdateAllAspectRatio( m_iRenderWidth/(float)m_iRenderHeight );
	
	m_iRealDeviceWidth = m_iRenderWidth;
	m_iRealDeviceHeight = m_iRenderHeight;

	PlatformInitCommon();
	
	// set vsync on, now guaranteed to have an effect on all Windows devices
	if( wglSwapIntervalEXT ) wglSwapIntervalEXT(0);
	SetVSync(1);
}

// has grown to be a lot more than just OpenGL setup, but difficult to change the function name now
void agk::PlatformInitGL ( void* ptr )
{
	//timeBeginPeriod( 5 );

	HWND hWnd = (HWND) ptr;
	g_hWnd = hWnd;
	g_bIsExternal = false;

	// Windows location services produces a dialog 
	// which must happen BEFORE OpenGL owns window
	// so call dummy GeoLocation command now to
	// trigger one time permission request
	// 24-05-13 people complaining that all Windows apps will ask for a default location to be set which is suspicious for apps that don't use it
	//float fIgnoreValue = GetRawGeoLatitude();

	if ( g_bGLInit ) return;
	g_bGLInit = true;

	if ( !CreateRendererWindow( g_hWnd ) ) return;

	if ( !m_pMouse[ 0 ] ) m_pMouse[ 0 ] = new cMouse();

	if ( !LoadRendererExtensions() )
	{
		agk::Message( "Your graphics card does not support one or more OpenGL features required by this program. Please check if there are any driver updates available for your card." );
		exit(0);
	}

	cImage::GetMaxTextureSize(); // preloads the value so that any threads don't call the GL function behind it
	agk::PlatformSetAlignment( 1 ); // not using memory alignment

	// store device and display width and height and aspect
	RECT rectDevice;
	GetClientRect( hWnd, &rectDevice );
	m_iRenderWidth = rectDevice.right-rectDevice.left;
	m_iRenderHeight = rectDevice.bottom-rectDevice.top;
	cCamera::UpdateAllAspectRatio( m_iRenderWidth/(float)m_iRenderHeight );
	
	m_iRealDeviceWidth = m_iRenderWidth;
	m_iRealDeviceHeight = m_iRenderHeight;

	// this DLL does not exist on Windows XP so we must check
	if ( GetModuleHandle( "Dwmapi.dll" ) ) g_bDwmExists = true;
	else g_bDwmExists = false;

	PlatformInitCommon();

	// set vsync on, now guaranteed to have an effect on all Windows devices
	if( wglSwapIntervalEXT ) wglSwapIntervalEXT(0);
	SetVSync(1);

	/*
	// PNG data extractor
	cFile oFile, oFile2;
	oFile.OpenToRead( "AGK-Help-Icon.png" );
	oFile2.OpenToWrite( "AGK-Help-Icon.txt" );
	int count = 0;
	int count2 = 0;
	char str[ 30 ];
	while ( !oFile.IsEOF() )
	{
		UINT i = oFile.ReadInteger();
		sprintf( str, "0x%x, ", i );
		oFile2.WriteData( str, strlen(str) );
		count++;
		count2++;
		if ( count > 30 ) 
		{
			oFile2.WriteData( "\\\n", 2 );
			count = 0;
		}
	}
	sprintf( str, "\nsize = %d", count2 );
	oFile2.WriteData( str, strlen(str) );
	oFile2.Close();
	oFile.Close();
	*/
}

void agk::PlatformInitConsole()
{
	//timeBeginPeriod( 5 );

	if ( g_bGLInit ) return;

	g_hWnd = 0;

	if ( !m_pMouse[ 0 ] ) m_pMouse[ 0 ] = new cMouse();

	// update timer var
	QueryPerformanceCounter ( (LARGE_INTEGER*) &i64StartTime );
	QueryPerformanceFrequency ( (LARGE_INTEGER*) &i64TimeFreq );
	if ( i64TimeFreq == 0 ) i64TimeFreq = 1;

	SetRandomSeed((unsigned int) i64StartTime);

	// Windows XP needs this otherwise CoCreateInstance on Music INPROC fails!
	if ( !g_bCoInit ) 
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		g_bCoInit = true;
	}

	// network setup
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
#ifdef _AGK_ERROR_CHECK
		agk::Error("WSAStartup failed");
#endif
		return;
	}

	// direct input setup
	HINSTANCE hi = GetModuleHandle(0);
	HRESULT hr = DirectInput8Create( hi, DIRECTINPUT_VERSION, IID_IDirectInput8A, (void**)&g_pDIDevice, NULL );
	if ( hr != DI_OK )
	{
		agk::Warning( "Failed to initialise DirectInput" );
	}
	else
	{
		UINT iThreadID = 0;
		g_pDirectInputThread = (HANDLE) _beginthreadex( NULL, 0, DirectInputEnumThread, m_pJoystick, 0, &iThreadID );
		/*
		hr = g_pDIDevice->EnumDevices( DI8DEVCLASS_GAMECTRL, InputEnumCallback, m_pJoystick, DIEDFL_ATTACHEDONLY ); 
		if ( hr != DI_OK )
		{
			agk::Warning( "Failed to enumerate DirectInput devices" );
		}
		*/
	}
}

void agk::PlatformCleanUp()
{
	if ( g_hUser32 ) FreeLibrary( g_hUser32 );
	g_hUser32 = 0;

	// Free Ultrabook command code
	if ( g_bGLInit ) FreeSensorCommands();

	/*
	uString sPath( szWriteDir );
	sPath.Trunc2( 1 );
	sPath.Trunc( '/' );
	if ( SetCurrentDirectoryUTF8( sPath.GetStr() ) )
	{
		sPath.SetStr( szWriteDir );
		sPath.Trunc2( 1 );
		int index = sPath.RevFind( '/' );
		if ( index >= 0 )
		{
			uString sFolder;
			sPath.SubString( sFolder, index+1 );
			RemoveDirectoryUTF8( sFolder );
		}
	}
	*/

	if ( g_bMFPlatExists ) 
	{
		if ( g_pVideoSession ) DeleteVideo();
		MFShutdown();
	}

	agk::StopSound(0); // stop all
	AGKMusicOGG::DeleteAll();
	cSoundMgr::Destroy();

	WSACleanup();

	if ( g_bCoInit ) CoUninitialize();
	g_bCoInit = false;

	PlatformDestroyGL();
	g_bGLInit = false;
}

void agk::UpdatePtr( void *ptr )
{
	HWND hWnd = (HWND) ptr;
	g_hWnd = hWnd;
}

void agk::UpdatePtr2( void *ptr )
{
	HWND hWnd = (HWND) ptr;
	g_hWnd = hWnd;
}

int agk::GetInternalDataI( int index )
{
	switch( index )
	{	
		case 1: return 0; // does surface need updating
		default: return 0;
	}
}

void agk::WindowMoved()
{
	agk::SetVideoDimensions( m_fVideoX, m_fVideoY, m_fVideoWidth, m_fVideoHeight );
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

//****f* Core/Misc/Sleep
// FUNCTION
//   Suspend the app for a specified number of milliseconds. It is not recommended that you use this command
//   to suspend an app for more than the time of one frame (roughly 17 milliseconds).
// INPUTS
//   milliseconds -- The number of milliseconds to sleep.
// SOURCE
void agk::Sleep( UINT milliseconds )
//****
{
	::Sleep( milliseconds );
}

//****f* Core/Display/SetVSync
// FUNCTION
//   Sets whether vertical sync should be turned on or off. This is the preferred method of limiting the frame
//   rate as it will link the frame rate to the monitor refresh rate and avoid tearing or stuttering.
//   Using this command with mode 1 will override any currently set refresh rate from SetSyncRate, otherwise 
//   these two commands would fight each other for control of the frame rate, instead AGK will use the 
//   VSync refresh rate which is typically 60 frames per second, but depends on the monitor.<br/><br/>
//   On Linux, Mac, and HTML5 you can use a value greater than 1 to skip refreshes, for example if the monitor 
//   refresh rate is 60Hz then a mode value of 1 would limit the app to 60fps, but a mode value of 2 would skip 
//   every other refresh which would limit the app to 30fps.
// INPUTS
//   mode -- 1 to turn vsync on, 0 to turn it off.
// SOURCE
void agk::SetVSync( int mode )
//****
{
	if ( !g_bGLInit ) return;

	m_bUsingVSync = false;

	BOOL DwmActive = FALSE;
	if ( g_bDwmExists && !g_iIsAGKFullscreen ) DwmIsCompositionEnabled( &DwmActive );
	
	if ( DwmActive )
	{
		// Windows desktop window manager is the preferred method of VSync in Vista and Windows 7
		m_bUsingVSync = mode > 0;
	}
	else
	{
		// Windows XP does not suport desktop window manager so must use OpenGL VSync
		if( wglSwapIntervalEXT ) 
		{
			wglSwapIntervalEXT(mode);
			m_bUsingVSync = mode > 0;
		}
	}

	if ( m_bUsingVSync ) m_iSyncMode = 2;
	else m_iSyncMode = 0;
}

//****f* Core/Display/GetMaxDeviceWidth
// FUNCTION
//   Gets the maximum width that your app window can be. For platforms that support windowed mode such as Windows and Mac
//   your app can only achieve this size in full screen mode as windowed mode has a border around your app.
//   For HTML5 apps this will return the size of the current HTML document, and is not guaranteed to work if the HTML5 app 
//   is running in full screen mode. A better name for this command would be GetMaxWindowWidth, but it is now set in stone.
// SOURCE
int agk::GetMaxDeviceWidth()
//****
{
	RECT rc;
	GetWindowRect(GetDesktopWindow(), &rc);
	return rc.right-rc.left;
}

//****f* Core/Display/GetMaxDeviceHeight
// FUNCTION
//   Gets the maximum height that your app window can be. For platforms that support windowed mode such as Windows and Mac
//   your app can only achieve this size in full screen mode as windowed mode has a border around your app.
//   For HTML5 apps this will return the size of the current HTML document, and is not guaranteed to work if the HTML5 app 
//   is running in full screen mode. A better name for this command would be GetMaxWindowHeight, but it is now set in stone.
// SOURCE
int agk::GetMaxDeviceHeight()
//****
{
	RECT rc;
	GetWindowRect(GetDesktopWindow(), &rc);
	return rc.bottom-rc.top;
}

//****f* Core/Display/GetDeviceDPI
// FUNCTION
//   Gets the Dots Per Inch (also called Pixels Per Inch) of the device screen.
//   This only works on iOS and Android, other platforms will return 0.
//   On iOS this value is hardcoded by device, so if a new device is released then an AGK update will be required to
//   get an accurate value. In the meantime an estimated value will be generated.
// SOURCE
int agk::GetDeviceDPI()
//****
{
	return 0;
}

//****f* Core/Misc/GetAppPackageName
// FUNCTION
//   On Android this returns the package name used when exporting the app, for example for the AGK Player this would
//   be com.thegamecreators.agk_player2. On iOS it returns the Bundle ID used for the app, for example for the AGK 
//   Player this would be com.thegamecreators.agk2player. On all other platforms this currently returns an empty 
//   string. If calling this from tier 2 then the returned string must be deleted when it is no longer needed.
// SOURCE
char* agk::GetAppPackageName()
//****
{
	char *str = new char[1];
	*str = 0;
	return str;
}

//****f* Core/Misc/GetDevicePlatform
// FUNCTION
//   On Android this returns 0 if this app is running on a Google device, or 1 if it is running on an Amazon device.
//   On all other platforms this returns 0.
// SOURCE
int agk::GetDevicePlatform()
//****
{
	return 0;
}

void agk::PlatformUpdateDeviceSize()
{
	if ( !g_bGLInit ) return;
	if ( !g_hWnd ) return;

	RECT rectDevice;
	GetClientRect( g_hWnd, &rectDevice );

	/*
	if ( m_bUsingFBO )
	{
		m_iRealDeviceWidth = rectDevice.right-rectDevice.left;
		m_iRealDeviceHeight = rectDevice.bottom-rectDevice.top;

		m_iRenderWidth = m_iFBOWidth;
		m_iRenderHeight = m_iFBOHeight;
	}
	else*/
	{
		m_iRenderWidth = rectDevice.right-rectDevice.left;
		m_iRenderHeight = rectDevice.bottom-rectDevice.top;

		m_iRealDeviceWidth = m_iRenderWidth;
		m_iRealDeviceHeight = m_iRenderHeight;
	}
}

// Core

void agk::PlatformUpdateTime ( void )
{
	QueryPerformanceCounter ( (LARGE_INTEGER*) &i64CurrentTime );
	m_fTimeCurr = (i64CurrentTime-i64StartTime) / ( (double) i64TimeFreq );
	m_iTimeMilliseconds = (UINT) ((i64CurrentTime-i64StartTime)*1000 / i64TimeFreq);
}

void agk::PlatformResetTime ( void )
{
	QueryPerformanceCounter ( (LARGE_INTEGER*) &i64CurrentTime );
	i64StartTime = i64CurrentTime;

	// 100212 - added as compiler/apps can use network but not initgl (and timer is needed in network but inited in initgl)
	QueryPerformanceFrequency ( (LARGE_INTEGER*) &i64TimeFreq );
	if ( i64TimeFreq == 0 ) i64TimeFreq = 1;

	m_fTimeCurr = 0;
	m_iTimeMilliseconds = 0;
}

double agk::PlatformGetRawTime ( void )
{
	__int64 i64CurrentTime2;
	QueryPerformanceCounter ( (LARGE_INTEGER*) &i64CurrentTime2 );
	return i64CurrentTime2 / (double) i64TimeFreq;
}

void agk::CompositionChanged()
{
	if ( !g_bGLInit ) return;
	if ( !g_bDwmExists ) return;
	if ( !m_bUsingVSync ) return;

	LONG lStyle = 0;
	if ( g_hWnd ) GetWindowLong( g_hWnd, GWL_STYLE );
	BOOL DwmActive = FALSE;
	if ( g_bDwmExists && !(lStyle & WS_POPUP) ) DwmIsCompositionEnabled( &DwmActive );

	if ( DwmActive ) 
	{
		// turn off OpenGL vsync and rely on DWM VSync
		if ( wglSwapIntervalEXT ) wglSwapIntervalEXT( 0 );
	}
	else
	{
		// turn on OpenGL vsync as DWM VSync is no longer active
		if ( wglSwapIntervalEXT ) wglSwapIntervalEXT( 1 );
	}
}

void agk::PlatformSync ( void )
{
	if ( !g_bGLInit ) return;

	if ( !g_bIsExternal ) PlatformSwap();
	else 
	{
		if ( SwapExternal ) SwapExternal( g_pSwapParam );
	}

	LONG lStyle = 0;
	if ( g_hWnd ) GetWindowLong( g_hWnd, GWL_STYLE );
	BOOL DwmActive = FALSE;
	if ( g_bDwmExists && !(lStyle & WS_POPUP) ) DwmIsCompositionEnabled( &DwmActive );
	if ( DwmActive && m_bUsingVSync ) 
	{
		DwmFlush();
	}

	if ( (GetAsyncKeyState( VK_LSHIFT ) & 0x8000) == 0 ) agk::KeyUp( 257 );
	if ( (GetAsyncKeyState( VK_RSHIFT ) & 0x8000) == 0 ) agk::KeyUp( 258 );
	if ( (GetAsyncKeyState( VK_LCONTROL ) & 0x8000) == 0 ) agk::KeyUp( 259 );
	if ( (GetAsyncKeyState( VK_RCONTROL ) & 0x8000) == 0 ) agk::KeyUp( 260 );
	if ( (GetAsyncKeyState( VK_LMENU ) & 0x8000) == 0 ) agk::KeyUp( 261 );
	if ( (GetAsyncKeyState( VK_RMENU ) & 0x8000) == 0 ) agk::KeyUp( 262 );
}

void agk::PlatformCompleteInputInit()
{
	if ( g_pDirectInputThread == UNDEF ) return;
	WaitForSingleObject( g_pDirectInputThread, INFINITE );
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

	float DevToVirX = agk::GetVirtualWidth() / agk::m_fTargetViewportWidth;
	float DevToVirY = agk::GetVirtualHeight() / agk::m_fTargetViewportHeight;

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
		int editbox = agk::GetCurrentEditBox();
		if ( editbox > 0 ) agk::SetEditBoxFocus( editbox, 0 );
	}

	// backspace repeat
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

	// backspace pressed
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
	// windows doesn't require app control of the device volume
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

/*
int agk::Random( int from, int to )
{
	int diff = (to-from)+1;
	return (rand() % diff) + from;
}
*/

//****f* Core/Maths/Sin
// FUNCTION
//   Returns the sine of a value in degrees.
// INPUTS
//   a -- The value to pass into the sine function.
// SOURCE
float agk::Sin( float a )
//****
{
	return sin( a * PI / 180 );
}

//****f* Core/Maths/Cos
// FUNCTION
//   Returns the cosine of a value in degrees.
// INPUTS
//   a -- The value to pass into the cosine function.
// SOURCE
float agk::Cos( float a )
//****
{
	return cos( a * PI / 180 );
}

//****f* Core/Maths/Tan
// FUNCTION
//   Returns the tangent of a value in degrees.
// INPUTS
//   a -- The value to pass into the tangent function.
// SOURCE
float agk::Tan( float a )
//****
{
	return tan( a * PI / 180 );
}

//****f* Core/Maths/SinRad
// FUNCTION
//   Returns the sine of a value in radians.
// INPUTS
//   a -- The value to pass into the sine function.
// SOURCE
float agk::SinRad( float a )
//****
{
	return sin( a );
}

//****f* Core/Maths/CosRad
// FUNCTION
//   Returns the cosine of a value in radians.
// INPUTS
//   a -- The value to pass into the cosine function.
// SOURCE
float agk::CosRad( float a )
//****
{
	return cos( a );
}

//****f* Core/Maths/TanRad
// FUNCTION
//   Returns the tangent of a value in radians.
// INPUTS
//   a -- The value to pass into the tangent function.
// SOURCE
float agk::TanRad( float a )
//****
{
	return tan( a );
}

//****f* Core/Maths/ASin
// FUNCTION
//   Returns the arcsine of a value in degrees.
// INPUTS
//   a -- The value to pass into the arcsine function.
// SOURCE
float agk::ASin( float a )
//****
{
	return asin( a ) * 180 / PI;
}

//****f* Core/Maths/ACos
// FUNCTION
//   Returns the arccosine of a value in degrees.
// INPUTS
//   a -- The value to pass into the arccosine function.
// SOURCE
float agk::ACos( float a )
//****
{
	return acos( a ) * 180 / PI;
}

//****f* Core/Maths/ATan
// FUNCTION
//   Returns the arctangent of a value in degrees in the range -90 to 90.
// INPUTS
//   a -- The value to pass into the arctangent function.
// SOURCE
float agk::ATan( float a )
//****
{
	return atan( a ) * 180 / PI;
}

//****f* Core/Maths/ATan2
// FUNCTION
//   Returns the angle of an x,y vector in degrees in the range -180 to 180.
//   This exactly matches the C++ atan2 function with degrees.
// INPUTS
//   y -- The y value to pass to atan2
//   x -- The x value to pass to atan2
// SOURCE
float agk::ATan2( float y, float x )
//****
{
	return atan2( y, x ) * 180 / PI;
}

//****f* Core/Maths/ASinRad
// FUNCTION
//   Returns the arcsine of a value in radians.
// INPUTS
//   a -- The value to pass into the arcsine function.
// SOURCE
float agk::ASinRad( float a )
//****
{
	return asin( a );
}

//****f* Core/Maths/ACosRad
// FUNCTION
//   Returns the arccosine of a value in radians.
// INPUTS
//   a -- The value to pass into the arccosine function.
// SOURCE
float agk::ACosRad( float a )
//****
{
	return acos( a );
}

//****f* Core/Maths/ATanRad
// FUNCTION
//   Returns the arctangent of a value in radians in the range -pi/2 to pi/2.
// INPUTS
//   a -- The value to pass into the arctangent function.
// SOURCE
float agk::ATanRad( float a )
//****
{
	return atan( a );
}

//****f* Core/Maths/ATan2Rad
// FUNCTION
//   Returns the angle of an x,y vector in radians in the range -pi to pi.
//   This exactly matches the C++ atan2 function.
// INPUTS
//   y -- The y value to pass to atan2
//   x -- The x value to pass to atan2
// SOURCE
float agk::ATan2Rad( float y, float x )
//****
{
	return atan2( y, x );
}


//****f* Core/Maths/Trunc
// FUNCTION
//   Returns the integer part of a float value with no rounding either way.
// INPUTS
//   a -- The value to truncate.
// SOURCE
int agk::Trunc( float a )
//****
{
	if ( a > 0 ) return Floor( a );
	else return Ceil( a );
}

//****f* Core/Maths/Floor
// FUNCTION
//   Rounds a float to the next lowest integer. This differs from Trunc when using negative numbers,
//   Trunc( -1.6 ) is -1 but Floor( -1.6 ) is -2.
// INPUTS
//   a -- The value to floor.
// SOURCE
int agk::Floor( float a )
//****
{
	return agklrintf(floor(a));
}

//****f* Core/Maths/Ceil
// FUNCTION
//   Rounds a float to the next highest integer. This differs from Trunc when using positive numbers,
//   Trunc( 1.6 ) is 1 but Ceil( 1.6 ) is 2.
// INPUTS
//   a -- The value to round up.
// SOURCE
int agk::Ceil( float a )
//****
{
	return agklrintf(ceil(a));
}

//****f* Core/Maths/Round
// FUNCTION
//   Rounds a float to the nearest integer (positive or negative). 
// INPUTS
//   a -- The value to round.
// SOURCE
int agk::Round( float a )
//****
{
	return agklrintf(a);
}

//****f* Core/Maths/Sqrt
// FUNCTION
//   Returns the square root of a float value.
// INPUTS
//   a -- The value to square root.
// SOURCE
float agk::Sqrt( float a )
//****
{
	if ( a > 0 ) return sqrt( a );
	else return 0;
}

//****f* Core/Maths/Abs
// FUNCTION
//   Returns absolute of a value, that is the positive version of the number.
// INPUTS
//   a -- The value to convert.
// SOURCE
float agk::Abs( float a )
//****
{
	if ( a < 0 ) a = -a;
	return a;
}

//****f* Core/Maths/Mod
// FUNCTION
//   Returns the remainder of the integer division a/b.
// INPUTS
//   a -- The numerator.
//   b -- The denominator.
// SOURCE
int agk::Mod( int a, int b )
//****
{
	return a % b;
}

//****f* Core/Maths/FMod
// FUNCTION
//   Returns the remainder of the float division a/b.
// INPUTS
//   a -- The numerator.
//   b -- The denominator.
// SOURCE
float agk::FMod( float a, float b )
//****
{
	return fmod( a, b );
}

//****f* Core/Maths/Pow
// FUNCTION
//   Returns a raised to the power of b.
// INPUTS
//   a -- The base.
//   b -- The power.
// SOURCE
float agk::Pow( float a, float b )
//****
{
	return pow( a, b );
}

//****f* Core/Maths/Log
// FUNCTION
//   Returns the natural logarithm of the given value.
// INPUTS
//   a -- The input value
// SOURCE
float agk::Log( float a )
//****
{
	return log( a );
}

/*
void agk::ConvertMediaPath( uString &sPath )
{
	sPath.Prepend( "media\\" );
}
*/

// Image

bool cImage::ChooseFromSystem()
{
	char szFile[ MAX_PATH ];
	szFile[0] = '\0';
	
	OPENFILENAME ofn;
	memset( &ofn, 0, sizeof(OPENFILENAME) );

	char szPicturesFolder[ MAX_PATH ];
	if ( !SHGetSpecialFolderPath( NULL, szPicturesFolder, CSIDL_MYPICTURES, TRUE ) )
	{
		*szPicturesFolder = 0;
	}

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = g_hWnd;
	ofn.lpstrFilter = "Image Files\0*.jpg;*.png;*.bmp\0\0";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_NONETWORKBUTTON;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = szPicturesFolder;

	LONG lStyle = 0;
	if ( g_hWnd ) lStyle = GetWindowLong( g_hWnd, GWL_STYLE );
	if ( lStyle & WS_POPUP ) ::ShowWindow( g_hWnd, SW_MINIMIZE );

	if ( GetOpenFileName( &ofn ) == 0 ) 
	{
		DWORD error = CommDlgExtendedError();
		uString err; err.Format( "Open File Dialog Error: %d", error );
		agk::Warning( err );
		if ( lStyle & WS_POPUP ) ::ShowWindow( g_hWnd, SW_NORMAL );
		return false;
	}

	if ( lStyle & WS_POPUP ) ::ShowWindow( g_hWnd, SW_NORMAL );

	uString sPath( "raw:" ); sPath.Append( ofn.lpstrFile );
	return Load( sPath, false );
}

void agk::PlatformShowChooseScreen()
{
	char szFile[ MAX_PATH ];
	szFile[0] = '\0';
	
	OPENFILENAME ofn;
	memset( &ofn, 0, sizeof(OPENFILENAME) );

	char szPicturesFolder[ MAX_PATH ];
	if ( !SHGetSpecialFolderPath( NULL, szPicturesFolder, CSIDL_MYPICTURES, TRUE ) )
	{
		*szPicturesFolder = 0;
	}

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = "Image Files\0*.jpg;*.png;*.bmp\0\0";
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_NONETWORKBUTTON;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrInitialDir = szPicturesFolder;

	LONG lStyle = 0;
	if ( g_hWnd ) lStyle = GetWindowLong( g_hWnd, GWL_STYLE );
	if ( lStyle & WS_POPUP ) ::ShowWindow( g_hWnd, SW_MINIMIZE );

	if ( GetOpenFileName( &ofn ) == 0 ) 
	{
		DWORD error = CommDlgExtendedError();
		uString err; err.Format( "Open File Dialog Error: %d", error );
		agk::Warning( err );
		m_bIsChoosing = false;
		if ( lStyle & WS_POPUP ) ::ShowWindow( g_hWnd, SW_NORMAL );
		return;
	}

	if ( lStyle & WS_POPUP ) ::ShowWindow( g_hWnd, SW_NORMAL );

	m_pUncollectedChosenImage = new cImage();
	uString sPath( "raw:" ); sPath.Append( ofn.lpstrFile );
	m_pUncollectedChosenImage->Load( sPath, false );

	m_bIsChoosing = false;
}

bool agk::PlatformShowCaptureScreen()
{
	if ( !g_bGLInit ) return false;
	if ( !g_hWnd ) return false;

	HRESULT hr;

	if( !m_pFilterGraph )
	{
		// Create the filter graph
		hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IFilterGraph2, (void **) &m_pFilterGraph);
		if (FAILED(hr))	return false;
	}

	if( !m_pCaptureMoniker )
	{
		// Create the capture graph builder
		hr = CoCreateInstance(CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void **) &m_pCaptureMoniker);
		if (FAILED(hr)) return false;
    }

	// Obtain interfaces for media control and Video Window
    hr = m_pFilterGraph->QueryInterface(IID_IMediaControl,(LPVOID *) &m_pMCMoniker);
    if (FAILED(hr)) return false;
	
    hr = m_pFilterGraph->QueryInterface(IID_IVideoWindow, (LPVOID *) &m_pVWMoniker);
    if (FAILED(hr)) return false;


	// Create the system device enumerator
	IMoniker *pMoniker = NULL;
    ICreateDevEnum* pDevEnum = NULL;
		
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void ** ) &pDevEnum);
	if (FAILED(hr))
    {
		if ( pDevEnum ) pDevEnum->Release();
		agk::Warning( "Failed to get camera stream - Couldn't create system enumerator" );
        return false;
    }

	// Create an enumerator for the video capture devices
    IEnumMoniker* pClassEnum = NULL;

    hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pClassEnum, 0);
    if (FAILED(hr))
    {
		if ( pDevEnum ) pDevEnum->Release();
		if ( pClassEnum ) pClassEnum->Release();
        agk::Warning( "Failed to get camera stream - Couldn't create class enumerator" );
        return false;
    }

	if ( !pClassEnum )
    {
		if ( pDevEnum ) pDevEnum->Release();
		agk::Warning( "Failed to get camera stream - No video capture device was detected." );
        return false;
    }

	BOOL        Found = FALSE ;
	IPin        *pP = 0;
	IEnumPins   *pins=0;
	ULONG       n;
	PIN_INFO    pinInfo;
	IKsPropertySet *pKs = 0;
	//GUID		guid;
	//DWORD		dw;
	IBaseFilter *pSrc;
	ULONG		cFetched;
	
	while (!Found && S_OK == (pClassEnum->Next (1, &pMoniker, &cFetched)))
	{	
		hr = pMoniker->BindToObject(0,0,IID_IBaseFilter, (void**)&pSrc);
		if (FAILED(hr))
		{
			if ( pDevEnum ) pDevEnum->Release();
			if ( pClassEnum ) pClassEnum->Release();
			agk::Warning("Failed to get camera stream - Couldn't bind moniker to filter object");
			return false;
		}
		
		if(SUCCEEDED(pSrc->EnumPins(&pins)))
		{     
			while(!Found && (S_OK == pins->Next(1, &pP, &n)))
			{
				if(S_OK == pP->QueryPinInfo(&pinInfo))
				{
					if(pinInfo.dir == PINDIR_INPUT)
					{
						Found = TRUE;
						/*
						if(pP->QueryInterface(IID_IKsPropertySet, (void **)&pKs) == S_OK)
						{
							// for some reason this crashes with stack poitner corruption?
							if(pKs->Get(AMPROPSETID_Pin, AMPROPERTY_PIN_CATEGORY, NULL, 0, &guid, sizeof(GUID), &dw) == S_OK)
							{
								if(guid == PIN_CATEGORY_ANALOGVIDEOIN) Found = TRUE;
							}
							pKs->Release();
						}
						*/
					}
					pinInfo.pFilter->Release();
				}
				pP->Release();
			}
			pins->Release();
		}		
	} 

	if ( !Found )
	{
		if ( pDevEnum ) pDevEnum->Release();
		if ( pClassEnum ) pClassEnum->Release();
		agk::Warning("Failed to get camera stream - No suitable video capture device found");
		return false;
	}

	IBaseFilter *pBaseFilter = 0;

	IBindCtx *pContext=0;
    hr = CreateBindCtx(0, &pContext);
    if (FAILED(hr))
    {
		if ( pDevEnum ) pDevEnum->Release();
		if ( pClassEnum ) pClassEnum->Release();
        agk::Warning("Failed to get camera stream - Couldn't create a bind context for moniker" );
        return false;
    }
	
    hr = m_pFilterGraph->AddSourceFilterForMoniker(pMoniker, pContext, L"Camera", &pBaseFilter);
    if (FAILED(hr))
    {
		if ( pDevEnum ) pDevEnum->Release();
		if ( pClassEnum ) pClassEnum->Release();
        agk::Warning("Failed to get camera stream - Failed in AddSourceFilterForMoniker" );
        return false;
    }

	IBaseFilter *pGrabberF = NULL;
	hr = CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pGrabberF );
	if (FAILED(hr))
	{
		if ( pDevEnum ) pDevEnum->Release();
		if ( pClassEnum ) pClassEnum->Release();
		agk::Warning("Failed to get camera stream - Failed in create grabber filter" );
        return false;
	}

	hr = m_pFilterGraph->AddFilter( pGrabberF, L"Sample Grabber" );
    if (FAILED(hr))
    {
		if ( pDevEnum ) pDevEnum->Release();
		if ( pClassEnum ) pClassEnum->Release();
        agk::Warning("Failed to get camera stream - Failed in add grabber filter" );
        return false;
    }

	pGrabberF->QueryInterface(IID_ISampleGrabber, (void**)&pSampleGrabber);

	AM_MEDIA_TYPE amtype;
	memset( &amtype, 0, sizeof(AM_MEDIA_TYPE) );
	amtype.majortype = MEDIATYPE_Video;
	amtype.subtype = MEDIASUBTYPE_RGB24;

	pSampleGrabber->SetMediaType(&amtype);
	pSampleGrabber->SetBufferSamples( TRUE );

	hr = ConnectFilters(m_pFilterGraph, pBaseFilter, pGrabberF);
	if (FAILED(hr))
    {
		if ( pDevEnum ) pDevEnum->Release();
		if ( pClassEnum ) pClassEnum->Release();
        agk::Warning("Failed to get camera stream - Failed to connect sample grabber" );
        return false;
    }

	// Attach the filter graph to the capture graph
    hr = m_pCaptureMoniker->SetFiltergraph(m_pFilterGraph);
    if (FAILED(hr))
    {
		if ( pDevEnum ) pDevEnum->Release();
		if ( pClassEnum ) pClassEnum->Release();
        agk::Warning("Failed to get camera stream - Failed to set capture filter graph" );
        return false;
    }

    // Render the preview pin on the video capture filter
    // Use this instead of m_pGraph->RenderFile
    hr = m_pCaptureMoniker->RenderStream (NULL, NULL, pGrabberF, NULL, NULL);
	if (FAILED(hr))
	{
		if ( pDevEnum ) pDevEnum->Release();
		if ( pClassEnum ) pClassEnum->Release();
	    agk::Warning("Failed to get camera stream - The device may already be in use" );
		return false;
	}
	
    SAFE_RELEASE(pContext);
    SAFE_RELEASE(pBaseFilter);

	pMoniker->Release();

	LONG lStyle = GetWindowLong( g_hWnd, GWL_STYLE );
	if ( lStyle & WS_POPUP )
	{
		g_bIsFullscreen = true;
		::SetWindowLongPtr( g_hWnd, GWL_STYLE, (lStyle & ~WS_POPUP) );
		::SetWindowPos( g_hWnd, 0, 0,0, 0,0, SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE| SWP_FRAMECHANGED );
	}
	else 
	{
		g_bIsFullscreen = false;
	}

	RECT rc;
    ::GetClientRect(g_hWnd,&rc);
	int center = rc.left + (rc.right-rc.left)/2;
	if ( g_hVideoWnd ) ::DestroyWindow( g_hVideoWnd );
	g_hVideoWnd = ::CreateWindowEx( 0, "Static", "", WS_CHILD | WS_VISIBLE, 0, 0, rc.right, rc.bottom - 48, g_hWnd, (HMENU)123, (HINSTANCE)GetWindowLongPtr(g_hWnd, GWLP_HINSTANCE), NULL );

	g_hVideoButtonOK = ::CreateWindow( "Button", "Capture", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, center-110, rc.bottom-38, 100, 28, g_hWnd, (HMENU)124, (HINSTANCE)GetWindowLongPtr(g_hWnd, GWLP_HINSTANCE), NULL );
	g_hVideoButtonCancel = ::CreateWindow( "Button", "Cancel", WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, center+10, rc.bottom-38, 100, 28, g_hWnd, (HMENU)125, (HINSTANCE)GetWindowLongPtr(g_hWnd, GWLP_HINSTANCE), NULL );

	// Set the video window to be a child of the main window
    hr = m_pVWMoniker->put_Owner((OAHWND)g_hVideoWnd);
    if (FAILED(hr)) return false;
    
    // Set video window style
    hr = m_pVWMoniker->put_WindowStyle(WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    if (FAILED(hr)) return false;
	
    m_pVWMoniker->SetWindowPosition(0, 0, rc.right, rc.bottom);
	
    // Make the video window visible, now that it is properly positioned
    hr = m_pVWMoniker->put_Visible(OATRUE);
    if (FAILED(hr)) return false;
	
	hr = m_pVWMoniker->put_MessageDrain((OAHWND)g_hWnd);

	// start the stream
	hr = m_pMCMoniker->Run();
    if (FAILED(hr))
    {
		if ( pDevEnum ) pDevEnum->Release();
		if ( pClassEnum ) pClassEnum->Release();
        agk::Warning("Failed to get camera stream - Couldn't run the graph" );
        return false;
    }

	if ( pDevEnum ) pDevEnum->Release();
	if ( pClassEnum ) pClassEnum->Release();
	return true;
}

void agk::PlatformHideCaptureScreen()
{
	if ( !g_bGLInit ) return;
	if ( !g_hWnd ) return;
	if (m_pMCMoniker) m_pMCMoniker->StopWhenReady();

    if(m_pVWMoniker)
    {
        m_pVWMoniker->put_Visible(OAFALSE);
        m_pVWMoniker->put_Owner(NULL);
    }
	
    // Release DirectShow interfaces
    SAFE_RELEASE(m_pMCMoniker);
    SAFE_RELEASE(m_pVWMoniker);
    SAFE_RELEASE(m_pCaptureMoniker);
    SAFE_RELEASE(m_pFilterGraph);
	SAFE_RELEASE(pSampleGrabber);

	if ( g_hVideoWnd ) ::DestroyWindow( g_hVideoWnd );
	if ( g_hVideoButtonOK ) ::DestroyWindow( g_hVideoButtonOK );
	if ( g_hVideoButtonCancel ) ::DestroyWindow( g_hVideoButtonCancel );

	g_hVideoWnd = 0;
	g_hVideoButtonOK = 0;
	g_hVideoButtonCancel = 0;

	if ( g_bIsFullscreen )
	{
		LONG lStyle = GetWindowLong( g_hWnd, GWL_STYLE );
		::SetWindowLongPtr( g_hWnd, GWL_STYLE, lStyle | WS_POPUP );
		::SetWindowPos( g_hWnd, 0, 0,0, 0,0, SWP_NOZORDER|SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE| SWP_FRAMECHANGED );
	}
}

bool cImage::CaptureFromCamera()
{
	if ( !pSampleGrabber ) return false;

	AM_MEDIA_TYPE amtype;
	pSampleGrabber->GetConnectedMediaType( &amtype );

	VIDEOINFOHEADER *pVih;
	if ((amtype.formattype == FORMAT_VideoInfo) && 
		(amtype.cbFormat >= sizeof(VIDEOINFOHEADER)) &&
		(amtype.pbFormat != NULL) ) 
	{
		pVih = (VIDEOINFOHEADER*)amtype.pbFormat;
	}

	int width = 640;
	int height = 480;
	if ( pVih )
	{
		width = pVih->bmiHeader.biWidth;
		height = pVih->bmiHeader.biHeight;
	}

	long size;
	pSampleGrabber->GetCurrentBuffer( &size, NULL );

	unsigned char *data = new unsigned char[size];
	pSampleGrabber->GetCurrentBuffer(&size, (long*)data);

	UINT *bits = new UINT[ width*height ];
	for ( int y = 0; y < height; y++ )
	{
		UINT index = y*m_iWidth;
		UINT index2 = (height-y-1)*width;

		for ( int x = 0; x < width; x++ )
		{
			unsigned char byte1 = 0xff; //alpha
			unsigned char byte2 = data[index2*3 + 0];
			unsigned char byte3 = data[index2*3 + 1];
			unsigned char byte4 = data[index2*3 + 2];

			bits[ index ] = (((((byte1 << 8) | byte2) << 8) | byte3) << 8) | byte4;

			index++;
			index2++;
		}
	}

	delete [] data;

	LoadFromData( width, height, bits );

	delete [] bits;

	return true;
}

//****f* Image/Capture/GetNumDeviceCameras
// FUNCTION
//   Returns the number of cameras available for SetDeviceCameraToImage.
//   Currently only works on Android, iOS, and Windows
// SOURCE
int agk::GetNumDeviceCameras()
//****
{
	// Find a device camera
	ICreateDevEnum* pDevEnum = NULL;
		
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void ** ) &pDevEnum);
	if (FAILED(hr))
    {
		if ( pDevEnum ) pDevEnum->Release();
		agk::Warning( "Failed to get num cameras - Couldn't create system enumerator" );
        return 0;
    }

	// Create an enumerator for the video capture devices
    IEnumMoniker* pClassEnum = NULL;

    hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pClassEnum, 0);
    if (FAILED(hr))
    {
		if ( pDevEnum ) pDevEnum->Release();
		if ( pClassEnum ) pClassEnum->Release();
        agk::Warning( "Failed to get num cameras - Couldn't create class enumerator" );
        return 0;
    }

	if ( !pClassEnum )
    {
		if ( pDevEnum ) pDevEnum->Release();
		return 0;
    }

	BOOL        Found = FALSE ;
	IPin        *pP = 0;
	IEnumPins   *pins = 0;
	PIN_INFO    pinInfo;
	IBaseFilter *pSrc = 0;
	IMoniker *pMoniker = 0;
	
	while (!Found && S_OK == (pClassEnum->Next (1, &pMoniker, NULL)))
	{	
		hr = pMoniker->BindToObject(0,0,IID_IBaseFilter, (void**)&pSrc);
		if (FAILED(hr))
		{
			pMoniker->Release();
			pClassEnum->Release();
			pDevEnum->Release();
			agk::Warning("Failed to get num cameras - Couldn't bind moniker to filter object");
			return 0;
		}
		
		if(SUCCEEDED(pSrc->EnumPins(&pins)))
		{     
			while(!Found && (S_OK == pins->Next(1, &pP, NULL)))
			{
				if(S_OK == pP->QueryPinInfo(&pinInfo))
				{
					if(pinInfo.dir == PINDIR_INPUT)
					{
						Found = TRUE;
					}
				}
				pP->Release();
			}
			pins->Release();
		}
		pSrc->Release();
		pMoniker->Release();
	} 

	if ( pClassEnum ) pClassEnum->Release();
	if ( pDevEnum ) pDevEnum->Release();

	if ( Found ) return 1;
	else return 0;
}

//****f* Image/Capture/SetDeviceCameraToImage
// FUNCTION
//   Using an image ID greater than zero will stream the device camera to the chosen image. The image 
//   must not currently exist, it will be created by this command. Use an imageID of 0 to stop 
//   streaming, this will also delete the image.<br/>
//   The cameraID parameter is used when the device has multiple cameras, for example front and back
//   facing cameras. Typically 0 refers the back facing camera. Use <i>GetNumDeviceCameras</i> to see
//   how many cameras this device has and <i>GetDeviceCameraType</i> to check whether it is front or 
//   back facing. This parameter is ignored if the imageID is 0.<br/>
//   You can only have one device camera active at once, so to switch from back to front facing cameras
//   you must first call this command with imageID set to 0 to stop the capture, then start it again 
//   with the new cameraID.<br/>
//   Returns 1 if it was successful, 0 if there was a problem or the current platform doesn't support 
//   this command. This is currently supported on iOS, Android 4.0.3 and above, and Windows.
// INPUTS
//   cameraID -- The ID of the device camera you want to use, could be back or front facing camera
//   imageID -- The imageID you want to use for this image, must not exist
// SOURCE
int agk::SetDeviceCameraToImage( UINT cameraID, UINT imageID )
//****
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

		if ( m_pCameraTexture )
		{
			if ( pCameraMediaControl ) 
			{ 
				pCameraMediaControl->Stop(); 
				pCameraMediaControl->Release(); 
				pCameraMediaControl = 0; 
			}

			// delete everything
			m_cImageList.RemoveItem( m_pCameraTexture->m_iID );
			delete m_pCameraTexture; m_pCameraTexture = 0;
			SAFE_RELEASE( g_pCameraGraph );
			SAFE_RELEASE( pCameraSampleGrabber );
			if ( pCameraFrame ) delete [] pCameraFrame; pCameraFrame = 0;
			if ( pCameraFrameFinal ) delete [] pCameraFrameFinal; pCameraFrameFinal = 0;
			if ( pCameraFrameCallback ) delete pCameraFrameCallback; pCameraFrameCallback = 0;
			iCameraFrameChanged = 0;
		}

		return 1;
	}

	m_iDeviceCameraID = cameraID;
	
	if ( m_pCameraTexture )
	{
		agk::Error( "Cannot change image ID whilst the camera is streaming, use an ID of 0 first to clear the existing image, then use the new ID" );
		return 0;
	}

	// Find a device camera
	ICreateDevEnum* pDevEnum = NULL;
		
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void ** ) &pDevEnum);
	if (FAILED(hr))
    {
		if ( pDevEnum ) pDevEnum->Release();
		agk::Warning( "Failed to get camera stream - Couldn't create system enumerator" );
        return 0;
    }

	// Create an enumerator for the video capture devices
    IEnumMoniker* pClassEnum = NULL;

    hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pClassEnum, 0);
    if (FAILED(hr))
    {
		if ( pDevEnum ) pDevEnum->Release();
		if ( pClassEnum ) pClassEnum->Release();
        agk::Warning( "Failed to get camera stream - Couldn't create class enumerator" );
        return 0;
    }

	if ( !pClassEnum )
    {
		if ( pDevEnum ) pDevEnum->Release();
		agk::Warning( "Failed to get camera stream - No video capture device was detected." );
        return 0;
    }

	BOOL        Found = FALSE ;
	IPin        *pP = 0;
	IEnumPins   *pins = 0;
	PIN_INFO    pinInfo;
	IBaseFilter *pSrc = 0;
	IMoniker *pMoniker = 0;
	
	while (!Found && S_OK == (pClassEnum->Next (1, &pMoniker, NULL)))
	{	
		hr = pMoniker->BindToObject(0,0,IID_IBaseFilter, (void**)&pSrc);
		if (FAILED(hr))
		{
			pDevEnum->Release();
			pClassEnum->Release();
			pMoniker->Release();
			agk::Warning("Failed to get camera stream - Couldn't bind moniker to filter object");
			return 0;
		}
		
		if(SUCCEEDED(pSrc->EnumPins(&pins)))
		{     
			while(!Found && (S_OK == pins->Next(1, &pP, NULL)))
			{
				if(S_OK == pP->QueryPinInfo(&pinInfo))
				{
					if(pinInfo.dir == PINDIR_INPUT)
					{
						Found = TRUE;
					}
				}
				pP->Release();
			}
			pins->Release();
		}
		pSrc->Release();
		if ( !Found ) pMoniker->Release();
	} 

	if ( pDevEnum ) pDevEnum->Release();
	if ( pClassEnum ) pClassEnum->Release();

	if ( !Found )
	{
		agk::Warning("Failed to get camera stream - No suitable video capture device found");
		return 0;
	}

	IBindCtx *pContext = 0;
    hr = CreateBindCtx(0, &pContext);
    if (FAILED(hr))
    {
		pMoniker->Release();
		agk::Warning("Failed to get camera stream - Couldn't create a bind context for moniker" );
        return false;
    }
	
    // create filter graph
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IFilterGraph2,(void **)&g_pCameraGraph);
	if (FAILED(hr))
	{
		pContext->Release();
		pMoniker->Release();
		agk::Warning( "DirectShow could not be initialised. Failed to CoCreateInstance" );
		return 0;
	}

	hr = g_pCameraGraph->QueryInterface(IID_IMediaControl,(void **)&pCameraMediaControl);
	if (FAILED(hr))
	{
		pContext->Release();
		pMoniker->Release();
		SAFE_RELEASE(g_pCameraGraph);
		agk::Warning( "Failed to set camera to image - unable to get media controller" );
		return 0;
	}

	// add camera as source
	IBaseFilter *pSourceFilter = 0;
	hr = g_pCameraGraph->AddSourceFilterForMoniker(pMoniker, pContext, L"Camera", &pSourceFilter);
    if (FAILED(hr))
    {
		pMoniker->Release();
		pContext->Release();
		SAFE_RELEASE(pCameraMediaControl);
		SAFE_RELEASE(g_pCameraGraph);
		agk::Warning("Failed to get camera stream - Failed in AddSourceFilterForMoniker" );
        return 0;
    }

	pContext->Release();
	pMoniker->Release();
	
	// add sample grabber
	IBaseFilter *pCameraGrabberF = 0;
	hr = CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pCameraGrabberF );
	if (FAILED(hr))
	{
		pSourceFilter->Release();
		SAFE_RELEASE(pCameraMediaControl);
		SAFE_RELEASE(g_pCameraGraph);
		agk::Warning("Failed to create grabber filter" );
		return 0;
	}

	hr = g_pCameraGraph->AddFilter( pCameraGrabberF, L"Sample Grabber" );
	if (FAILED(hr))
	{
		pCameraGrabberF->Release();
		pSourceFilter->Release();
		SAFE_RELEASE(pCameraMediaControl);
		SAFE_RELEASE(g_pCameraGraph);
		agk::Warning("Failed to add grabber filter" );
		return 0;
	}

	hr = pCameraGrabberF->QueryInterface(IID_ISampleGrabber, (void**)&pCameraSampleGrabber);
	if (FAILED(hr))
	{
		pCameraGrabberF->Release();
		pSourceFilter->Release();
		SAFE_RELEASE(pCameraMediaControl);
		SAFE_RELEASE(g_pCameraGraph);
		agk::Warning("Failed to get sample grabber" );
		return 0;
	}

	AM_MEDIA_TYPE amtype;
	memset( &amtype, 0, sizeof(AM_MEDIA_TYPE) );
	amtype.majortype = MEDIATYPE_Video;
	amtype.subtype = MEDIASUBTYPE_RGB24;

	pCameraFrameCallback = new CameraSampleHandler();

	pCameraSampleGrabber->SetMediaType(&amtype);
	//pCameraSampleGrabber->SetBufferSamples( FALSE );
	pCameraSampleGrabber->SetCallback( pCameraFrameCallback, 1 );

	hr = ConnectFilters(g_pCameraGraph, pSourceFilter, pCameraGrabberF);
	if (FAILED(hr))
	{
		pCameraGrabberF->Release();
		pSourceFilter->Release();
		SAFE_RELEASE(pCameraSampleGrabber);
		SAFE_RELEASE(pCameraMediaControl);
		SAFE_RELEASE(g_pCameraGraph);
		agk::Warning("Failed to connect source and sample grabber filters" );
		return 0;
	}

	pSourceFilter->Release();

	// add null renderer
	IBaseFilter *pCameraNullF = 0;
	hr = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pCameraNullF));
	if (FAILED(hr))
	{
		pCameraGrabberF->Release();
		SAFE_RELEASE(pCameraSampleGrabber);
		SAFE_RELEASE(pCameraMediaControl);
		SAFE_RELEASE(g_pCameraGraph);
		agk::Warning("Failed to create null renderer" );
		return 0;
	}

	hr = g_pCameraGraph->AddFilter(pCameraNullF, L"Null Filter");
	if (FAILED(hr))
	{
		pCameraNullF->Release();
		pCameraGrabberF->Release();
		SAFE_RELEASE(pCameraSampleGrabber);
		SAFE_RELEASE(pCameraMediaControl);
		SAFE_RELEASE(g_pCameraGraph);
		agk::Warning("Failed to add null renderer" );
		return 0;
	}

	hr = ConnectFilters(g_pCameraGraph, pCameraGrabberF, pCameraNullF);
	if (FAILED(hr))
	{
		pCameraNullF->Release();
		pCameraGrabberF->Release();
		SAFE_RELEASE(pCameraSampleGrabber);
		SAFE_RELEASE(pCameraMediaControl);
		SAFE_RELEASE(g_pCameraGraph);
		agk::Warning("Failed to connect source and sample grabber filters" );
		return 0;
	}

	pCameraNullF->Release();
	pCameraGrabberF->Release();

	pCameraSampleGrabber->GetConnectedMediaType( &amtype );
	VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER*)amtype.pbFormat;
	
	iCameraWidth = 640;
	iCameraHeight = 480;
	if ( pVih )
	{
		iCameraWidth = pVih->bmiHeader.biWidth;
		iCameraHeight = pVih->bmiHeader.biHeight;
	}
	else
	{
		agk::Warning( "Failed to get camera width and height" );
	}

	// create destination image
	m_pCameraTexture = new cImage();
	m_pCameraTexture->CreateBlankImage( iCameraWidth, iCameraHeight, 0, 0 );
	m_pCameraTexture->m_iID = imageID;
	m_cImageList.AddItem( m_pCameraTexture, imageID );

	pCameraFrame = new unsigned char[ iCameraWidth*iCameraHeight*3 ];
	pCameraFrameFinal = new unsigned int[ iCameraWidth*iCameraHeight ];

	hr = pCameraMediaControl->Run();
	if ( FAILED(hr) )
	{
		m_cImageList.RemoveItem( m_pCameraTexture->m_iID );
		delete m_pCameraTexture; m_pCameraTexture = 0;
		SAFE_RELEASE(pCameraSampleGrabber);
		SAFE_RELEASE(pCameraMediaControl);
		SAFE_RELEASE(g_pCameraGraph);
		agk::Error("Failed to run camera stream");
		return 0;
	}

	return 1;
}

void agk::DeviceCameraUpdate()
{
	if ( !pCameraSampleGrabber ) return;
	if ( !pCameraFrame ) return;
	if ( !pCameraFrameFinal ) return;
	if ( !iCameraFrameChanged ) return;
	
	// scope the auto lock
	{
		cAutoSLock autolock( &cCameraLock );
		if ( !pCameraSampleGrabber ) return;
		if ( !pCameraFrame ) return;
		if ( !pCameraFrameFinal ) return;
		if ( !iCameraFrameChanged ) return;

		for ( int y = 0; y < iCameraHeight; y++ )
		{
			UINT index = y*iCameraWidth;
			UINT index2 = (iCameraHeight-y-1)*iCameraWidth;

			for ( int x = 0; x < iCameraWidth; x++ )
			{
				unsigned char byte1 = 0xff; //alpha
				unsigned char byte2 = pCameraFrame[index2*3 + 0];
				unsigned char byte3 = pCameraFrame[index2*3 + 1];
				unsigned char byte4 = pCameraFrame[index2*3 + 2];

				pCameraFrameFinal[ index ] = (((((byte1 << 8) | byte2) << 8) | byte3) << 8) | byte4;

				index++;
				index2++;
			}
		}

		iCameraFrameChanged = 0;
	}

	m_pCameraTexture->LoadFromData( iCameraWidth, iCameraHeight, pCameraFrameFinal );
}

void agk::DeviceCameraResumed()
{
	// do nothing
}

//****f* Image/Capture/GetDeviceCameraType
// FUNCTION
//   Returns 1 if the given camera ID is a back facing camera, 2 if it is a front facing camera, or
//   0 if this is unknown.
// INPUTS
//   cameraID -- The ID of the device camera you want to check
// SOURCE
int agk::GetDeviceCameraType( UINT cameraID )
//****
{
	// do nothing
	return 0;
}

void cImage::Print( float size )
{
	if ( !g_hWnd ) return;
	if ( size < 0 ) size = 0;
	if ( size > 100 ) size = 100;
	size = size * 0.0095f;
	
	PRINTDLG lppd;
	memset( &lppd, 0, sizeof(PRINTDLG) );
	lppd.lStructSize = sizeof(PRINTDLG);
	lppd.Flags = PD_NOSELECTION | PD_NOPAGENUMS | PD_NOWARNING | PD_RETURNDC | PD_USEDEVMODECOPIESANDCOLLATE;

	BOOL result = PrintDlg( &lppd );
	if ( !result ) return;

	if ( !lppd.hDC ) return;

	DOCINFO di;
	memset( &di, 0, sizeof(DOCINFO) );
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = "Image";

	int sizeX = GetDeviceCaps (lppd.hDC, HORZRES);
	int sizeY = GetDeviceCaps (lppd.hDC, VERTRES);
	int borderX = agk::Round( sizeX * 0.025f );
	int borderY = agk::Round( sizeY * 0.025f );
	if ( borderY < borderX ) borderX = borderY;
	else borderY = borderX;

	if (StartDoc( lppd.hDC, &di ) > 0)
	{
		if (StartPage( lppd.hDC ) > 0)
		{
			if ( GetTextureID() == 0 ) return;

			// get src image size
			UINT iTrueWidthSrc = GetTotalWidth();
			UINT iTrueHeightSrc = GetTotalHeight();
			UINT iOrigWidthSrc = GetWidth();
			UINT iOrigHeightSrc = GetHeight();

			// get the offsets for the src image
			int startXSrc = agk::Floor( GetU1() * iTrueWidthSrc );
			int startYSrc = agk::Floor( GetV1() * iTrueHeightSrc );

			// get src pixels from OpenGL
			unsigned char *bits = 0;
			GetRawData( &bits );
			if ( !bits ) 
			{
				agk::Warning("Failed to get image data");
				return;
			}

			// swap red and blue
			for( UINT y = 0; y < iOrigHeightSrc; y++ )
			{
				for( UINT x = 0; x < iOrigWidthSrc; x++ )
				{
					UINT index = y*iOrigWidthSrc + x;
					unsigned char temp = bits[ index*4 ];
					bits[ index*4 ] = bits[ index*4 + 2 ];
					bits[ index*4 + 2 ] = temp;
				}
			}

			HDC dc = CreateCompatibleDC( GetDC( g_hWnd ) );
			HBITMAP bitmap = CreateBitmap( m_iOrigWidth, m_iOrigHeight, 1, 32, bits );
			HGDIOBJ hObj = SelectObject( dc, bitmap );

			float printerAspect = sizeY / (float)sizeX;
			float aspect = m_iOrigHeight / (float)m_iOrigWidth;
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
			
			StretchBlt( lppd.hDC, borderX, borderY, sizeX, sizeY, dc, 0, 0, m_iOrigWidth, m_iOrigHeight, SRCCOPY );

			if ( EndPage( lppd.hDC ) > 0 ) EndDoc( lppd.hDC );

			DeleteDC( dc );
			DeleteObject( bitmap );
			delete [] bits;
		}
	}
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
		uString err; err.Format( "Failed to load image: %s", szFile );
		agk::Error( err );
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

//****f* Sound/Vibration/VibrateDevice
// FUNCTION
//   Vibrates the device for the specified number of seconds. Works on iOS and Android. Note that on iOS the length 
//   value is ignored and only a short vibration will occur, this is due to Apple restrictions.
// INPUTS
//   seconds -- The time in seconds to vibrate
// SOURCE
void agk::VibrateDevice( float seconds )
//****
{
	// do nothing
}

//****f* Extras/Clipboard/SetClipboardText
// FUNCTION
//   Sets the device clipboard to the specified text, this overwrites anything that was previously in the device clipboard. 
//   The clipboard is the same as that used by the copy/paste functionality of the device.
// INPUTS
//   szText -- The text to copy
// SOURCE
void agk::SetClipboardText( const char* szText )
//****
{
	if ( !OpenClipboard( 0 ) ) return;
	EmptyClipboard();
	int length = strlen(szText) + 1;
	HGLOBAL hg = GlobalAlloc( GMEM_MOVEABLE, length );
	if ( !hg ) 
	{
		CloseClipboard();
		return;
	}
	memcpy( GlobalLock(hg), szText, length );
	GlobalUnlock( hg );
	SetClipboardData( CF_TEXT, hg );
	CloseClipboard();
	GlobalFree(hg);
}

//****f* Extras/Clipboard/GetClipboardText
// FUNCTION
//   Gets any text currently held in the device clipboard, the text remains in the clipboard so it can still be used by other apps. 
//   The clipboard is the same as that used by the copy/paste functionality of the device.
// SOURCE
char* agk::GetClipboardText()
//****
{
	if ( !OpenClipboard( 0 ) ) goto error;
	HANDLE hData = GetClipboardData( CF_TEXT );
	if ( !hData ) 
	{
		CloseClipboard();
		goto error;
	}

	char* szData = (char*)GlobalLock( hData );
	if ( !szData ) goto error;

	int length = strlen( szData ) + 1;

	char *str = new char[ length ]; 
	strcpy( str, szData );

	GlobalUnlock( hData );
	CloseClipboard();

	return str;

error:
	char *str2 = new char[1]; *str2 = 0;
	return str2;
}

// Music

void cMusicMgr::PlatformAddFile( cMusic *pMusic )
{
	/*
	uString sFilename = pMusic->m_sFile;
	sFilename.Lower();
	if ( sFilename.FindStr(".mp3") >= 0 && !cFile::Exists( pMusic->m_sFile ) )
	{
		pMusic->m_sFile.Trunc( '.' );
		pMusic->m_sFile.Append( ".ogg" );
	}
	*/
}

bool cMusicMgr::PlatformPrepare( UINT iID )
{
	if ( !g_hWnd )
	{
		agk::Error( "Music commands are not supported without a window, try the OGG music commands instead" );
		return false;
	}

	cMusic *pMusic = m_pMusicFiles[ iID ];

	m_pCurrentlyPlaying = UNDEF;

	HRESULT h;
	IPin            *pPin = NULL;
    IEnumFilters    *pFilterEnum = NULL;
    IBaseFilter     **ppFilters;
    int             iFiltCount;
    int             iPos;	

	if ( !g_pGraphBuilder )
	{
//		h = CoCreateInstance(CLSID_FilterGraph, NULL,CLSCTX_INPROC, IID_IGraphBuilder,(void **)&g_pGraphBuilder);
		h = CoCreateInstance(CLSID_FilterGraph, NULL,CLSCTX_INPROC_SERVER, IID_IGraphBuilder,(void **)&g_pGraphBuilder);
		if (FAILED(h))
		{
#ifdef _AGK_ERROR_CHECK
			agk::Error( uString("DirectShow could not be initialised. Failed to CoCreateInstance") );
#endif
			return false;
		}

		// Get the IMediaControl Interface
		h = g_pGraphBuilder->QueryInterface(IID_IMediaControl,(void **)&g_pMediaControl);
		if (FAILED(h))
		{
#ifdef _AGK_ERROR_CHECK
			agk::Error( uString("DirectShow could not be initialised. Failed to get the IMediaControl Interface") );
#endif
			return false;
		}

		// Get the IMediaSeeking Interface
		h = g_pGraphBuilder->QueryInterface(IID_IMediaSeeking,(void **)&g_pMediaSeeking);
		if (FAILED(h))
		{
#ifdef _AGK_ERROR_CHECK
			agk::Error( uString("DirectShow could not be initialised. Failed to get the IMediaSeeking Interface") );
#endif
			return false;
		}

		// Get the IMediaEventEx Interface
		h = g_pGraphBuilder->QueryInterface(IID_IMediaEventEx,(void **)&g_pMediaEventEx);
		if (FAILED(h))
		{
#ifdef _AGK_ERROR_CHECK
			agk::Error( uString("DirectShow could not be initialised. Failed to get the IMediaEventEx Interface") );
#endif
			return false;
		}

		// Get the IBasicAudio Interface
		h = g_pGraphBuilder->QueryInterface(IID_IBasicAudio, (void **)&g_pBasicAudio);
		if (FAILED(h))
		{
#ifdef _AGK_ERROR_CHECK
			agk::Error( uString("DirectShow could not be initialised. Failed to get the IBasicAudio Interface") );
#endif
			return false;
		}

		if ( g_bGLInit ) g_pMediaEventEx->SetNotifyWindow((OAHWND)g_hWnd, WM_SOUND_EVENT, (LONG_PTR) this );
	}
	
	g_pMediaControl->Stop();

	// clean up any existing filters
	if ( pSource ) 
	{
		g_pGraphBuilder->RemoveFilter( pSource );
		pSource->Release();

		g_pGraphBuilder->EnumFilters(&pFilterEnum);
    	for (iFiltCount = 0; pFilterEnum->Skip(1) == S_OK; iFiltCount++)
        ;

		// Allocate space, then pull out all of the 
		ppFilters = (IBaseFilter **)malloc(sizeof(IBaseFilter *) * iFiltCount);

		pFilterEnum->Reset();

		for (iPos = 0; pFilterEnum->Next(1, &ppFilters[iPos], NULL) == S_OK; iPos++)
			;
	    
		pFilterEnum->Release();
		
		for (iPos = 0; iPos < iFiltCount; iPos++) 
		{
			g_pGraphBuilder->RemoveFilter(ppFilters[iPos]);
			ppFilters[iPos]->Release();
		}

		free(ppFilters);
	}

	pSource = 0;
    
	// add new source filter
	int iLength = MultiByteToWideChar( CP_UTF8, 0, pMusic->m_sFile.GetStr(), -1, 0, 0 );
	WCHAR *wstr = new WCHAR[ iLength + 1 ];
	MultiByteToWideChar( CP_UTF8, 0, pMusic->m_sFile.GetStr(), -1, wstr, iLength+1 );
	
	h = g_pGraphBuilder->AddSourceFilter( wstr, wstr, &pSource );
	delete [] wstr;
    if (FAILED(h)) 
	{
		agk::Error( uString("Failed to load music source filter") );
		return false;
    }

	// Get the first output pin of the new source filter. Audio sources 
    // typically have only one output pin, so for most audio cases finding 
    // any output pin is sufficient.
    HRESULT hr = pSource->FindPin(L"Output", &pPin);  
	if ( FAILED(hr) )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( uString("Failed load get music source pin, are the necessary codecs installed?") );
#endif
		return false;		
	}

    // Break all connections on the filters. You can do this by adding 
    // and removing each filter in the graph

    hr = g_pGraphBuilder->EnumFilters(&pFilterEnum);
	if ( FAILED(hr) )
	{
		agk::Error( uString("Failed to enum graph filters") );
		return false;		
	}
    
    // Need to know how many filters. If we add/remove filters during the
    // enumeration we'll invalidate the enumerator
    for (iFiltCount = 0; pFilterEnum->Skip(1) == S_OK; iFiltCount++)
		;

	// Allocate space, then pull out all of the 
	ppFilters = (IBaseFilter **)malloc(sizeof(IBaseFilter *) * iFiltCount);

	pFilterEnum->Reset();

	for (iPos = 0; pFilterEnum->Next(1, &ppFilters[iPos], NULL) == S_OK; iPos++)
		;
    
	pFilterEnum->Release();
    
    for (iPos = 0; iPos < iFiltCount; iPos++) 
    {
		g_pGraphBuilder->RemoveFilter(ppFilters[iPos]);
        
        // Put the filter back
        hr = g_pGraphBuilder->AddFilter(ppFilters[iPos], NULL);
		if ( FAILED(hr) )
		{
			free(ppFilters);
			agk::Error( uString("Failed to add graph filter") );
			return false;		
		}

        ppFilters[iPos]->Release();
    }

	free(ppFilters);

    // We have the new output pin. Render it
    hr = g_pGraphBuilder->Render(pPin);
	pPin->Release();
	if ( FAILED(hr) )
	{
		uString err; err.Format( "Failed to render music graph: %x", hr );
		agk::Error( err );
		return false;		
	}

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

	if ( m_pCurrentlyPlaying && m_pCurrentlyPlaying->m_sFile.CompareTo( m_pMusicFiles[ iID ]->m_sFile.GetStr() ) == 0 )
	{
		m_pCurrentlyPlaying = m_pMusicFiles[ iID ];
		LONGLONG llPos = 0;
		g_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning, &llPos, AM_SEEKING_NoPositioning);
		g_pMediaControl->Run();
		return;
	}

	if ( !PlatformPrepare( iID ) ) return;

	cMusic *pMusic = m_pMusicFiles[ iID ];
	
	float volume = (m_iMasterVolume * pMusic->m_iVolume) / 10000.0f;

	if ( volume > 1 ) volume = 1;
	if ( volume < 0.00001f ) volume = -10000;
	else volume = (log10(volume)*20) * 100;
	g_pBasicAudio->put_Volume( (long) volume );

    // Re-seek the graph to the beginning
    LONGLONG llPos = 0;
    g_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning, &llPos, AM_SEEKING_NoPositioning);

	m_pCurrentlyPlaying = pMusic;

	HRESULT hr = g_pMediaControl->Run();
	if ( FAILED(hr) )
	{
		agk::Error( "Failed to play music file" );
	}
}

void cMusicMgr::Pause()
{
	if ( m_pCurrentlyPlaying == UNDEF ) return;
	
	for ( UINT i = 0; i < MAX_MUSIC_FILES; i++ )
	{
		if ( m_pMusicFiles[ i ] == m_pCurrentlyPlaying )
		{
			if ( g_pMediaControl ) g_pMediaControl->Pause();
		}
	}
}

void cMusicMgr::Resume()
{
	if ( m_pCurrentlyPlaying == UNDEF ) return;

	for ( UINT i = 0; i < MAX_MUSIC_FILES; i++ )
	{
		if ( m_pMusicFiles[ i ] == m_pCurrentlyPlaying )
		{
			if ( g_pMediaControl ) g_pMediaControl->Run();
		}
	}
}

void cMusicMgr::Stop()
{
	if ( m_pCurrentlyPlaying == UNDEF ) return;

	for ( UINT i = 0; i < MAX_MUSIC_FILES; i++ )
	{
		if ( m_pMusicFiles[ i ] == m_pCurrentlyPlaying )
		{
			if ( g_pMediaControl ) g_pMediaControl->Stop();
			m_pCurrentlyPlaying = UNDEF;
		}
	}
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

	if ( volume > 1 ) volume = 1;
	if ( volume < 0.00001f ) volume = -10000;
	else volume = (log10(volume)*20) * 100;

	if ( g_pBasicAudio ) g_pBasicAudio->put_Volume( (long) volume );
}

float cMusicMgr::GetDuration( UINT ID )
{
	if ( m_pCurrentlyPlaying )
	{
		if ( m_pCurrentlyPlaying->m_iID != ID )
		{
#ifdef _AGK_ERROR_CHECK
			agk::Error( "Cannot get music file duration whilst another file is playing" );
#endif
			return 0;
		}
	}
	else
	{
		if ( !PlatformPrepare( ID ) ) return 0;
	}

	LONGLONG dur = 0;
	g_pMediaSeeking->GetDuration( &dur );
	float seconds = dur / 10000000.0f;
	return seconds;
}

void cMusicMgr::Seek( float seconds, int mode )
{
	if ( !m_pCurrentlyPlaying )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot seek as no music is playing" );
#endif
		return;
	}

	LONGLONG pos = (LONGLONG) (seconds*10000000);
	if ( mode == 0 )
	{
		g_pMediaSeeking->SetPositions( &pos, AM_SEEKING_AbsolutePositioning, &pos, AM_SEEKING_NoPositioning );
	}
	else
	{
		LONGLONG currpos = 0;
		g_pMediaSeeking->GetPositions( &currpos, NULL );
		currpos += pos;
		g_pMediaSeeking->SetPositions( &currpos, AM_SEEKING_AbsolutePositioning, &pos, AM_SEEKING_NoPositioning );
	}
}

float cMusicMgr::GetPosition()
{
	if ( !m_pCurrentlyPlaying ) return 0;
	
	LONGLONG pos = 0;
	LONGLONG stop = 0;
	g_pMediaSeeking->GetPositions( &pos, &stop );
	float seconds = pos / 10000000.0f;
	return seconds;
}

void cMusicMgr::HandleEvent()
{
	long evCode;
	LONG_PTR param1, param2;
    HRESULT h;

	while ( !FAILED(h = g_pMediaEventEx->GetEvent(&evCode, &param1, &param2, 0)) )
	{
#ifdef _AGK_ERROR_CHECK
        //uString str( "Music Event, Code: ", 100 ); str.AppendUInt( evCode ); str.Append( ", Param1: " ); str.AppendUInt( param1 ); str.Append( ", Param2: " ); str.AppendUInt( param2 );
		//agk::Error( str );
#endif

		if ( evCode == EC_COMPLETE && m_pCurrentlyPlaying != UNDEF ) 
		{
			UINT iCurrID = m_pCurrentlyPlaying->m_iID;
			UINT iNextID = 0;

			if ( iCurrID >= m_iEndID )
			{
				if ( m_bLoop ) 
				{
					if ( m_iStartID == m_iEndID )
					{
						LONGLONG llPos = 0;
						g_pMediaSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning, &llPos, AM_SEEKING_NoPositioning);
						g_pMediaEventEx->FreeEventParams(evCode, param1, param2);
						continue;
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

		g_pMediaEventEx->FreeEventParams(evCode, param1, param2);
	}
}

// OGG Music

struct AGKOGGSoundData
{
	XAUDIO2_BUFFER m_XABuffer;
	IXAudio2SourceVoice* m_pSourceVoice;
	XAudio2Win8::IXAudio2SourceVoice* m_pSourceVoiceWin8;
	unsigned char *m_pBuffers[ 3 ];
	unsigned int m_iResetFlag[ 3 ];
	unsigned int m_iCurrBuffer;
};

void AGKMusicOGG::PlatformInit()
{
	if ( !(g_iXAudio2Mode == 1 && pXAudio2) && !(g_iXAudio2Mode == 2 && pXAudio2Win8) ) return;
	AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;

	if ( !m_pSoundData ) 
	{
		m_pSoundData = (void*) new AGKOGGSoundData();
		pTemp = (AGKOGGSoundData*)m_pSoundData;
		
		memset( &(pTemp->m_XABuffer), 0, sizeof(XAUDIO2_BUFFER) );
		pTemp->m_pSourceVoice = 0;
		pTemp->m_pSourceVoiceWin8 = 0;
		pTemp->m_iCurrBuffer = 0;
		pTemp->m_iResetFlag[ 0 ] = 0;
		pTemp->m_iResetFlag[ 1 ] = 0;
		pTemp->m_iResetFlag[ 2 ] = 0;
		pTemp->m_pBuffers[ 0 ] = new unsigned char[ AGK_OGG_BUFFER_SIZE ];
		pTemp->m_pBuffers[ 1 ] = new unsigned char[ AGK_OGG_BUFFER_SIZE ];
		pTemp->m_pBuffers[ 2 ] = new unsigned char[ AGK_OGG_BUFFER_SIZE ];
	}

	HRESULT hr = -1;
	if ( g_iXAudio2Mode == 1 ) hr = pXAudio2->CreateSourceVoice( &(pTemp->m_pSourceVoice), (::WAVEFORMATEX*)&m_fmt, 0, XAUDIO2_MAX_FREQ_RATIO, NULL, NULL, NULL );
	if ( g_iXAudio2Mode == 2 ) hr = pXAudio2Win8->CreateSourceVoice( &(pTemp->m_pSourceVoiceWin8), (::WAVEFORMATEX*)&m_fmt, 0, XAUDIO2_MAX_FREQ_RATIO, NULL, NULL, NULL );
	if( FAILED(hr) ) 
	{
		agk::Error( "Failed to create music source voice" );
		return;
	}

	float fVol = m_iVolume / 100.0f;
	if ( pTemp->m_pSourceVoice ) pTemp->m_pSourceVoice->SetVolume( fVol );
	else if ( pTemp->m_pSourceVoiceWin8 ) pTemp->m_pSourceVoiceWin8->SetVolume( fVol );
}

void AGKMusicOGG::PlatformCleanUp()
{
	if ( m_pSoundData ) 
	{
		AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;

		if ( pTemp->m_pSourceVoice ) pTemp->m_pSourceVoice->DestroyVoice();
		if ( pTemp->m_pSourceVoiceWin8 ) pTemp->m_pSourceVoiceWin8->DestroyVoice();
		pTemp->m_pSourceVoice = 0;
		pTemp->m_pSourceVoiceWin8 = 0;

		if ( pTemp->m_pBuffers[ 0 ] ) delete [] pTemp->m_pBuffers[ 0 ];
		if ( pTemp->m_pBuffers[ 1 ] ) delete [] pTemp->m_pBuffers[ 1 ];
		if ( pTemp->m_pBuffers[ 2 ] ) delete [] pTemp->m_pBuffers[ 2 ];

		delete pTemp;
		m_pSoundData = 0;
	}
}

int AGKMusicOGG::PlatformPlay()
{
	if ( !(g_iXAudio2Mode == 1 && pXAudio2) && !(g_iXAudio2Mode == 2 && pXAudio2Win8) ) return 0;
	if ( !m_pSoundData ) return 0;
	AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;

	HRESULT hr = -1;
	if ( pTemp->m_pSourceVoice ) hr = pTemp->m_pSourceVoice->Start( 0, XAUDIO2_COMMIT_NOW );
	else if ( pTemp->m_pSourceVoiceWin8 ) hr = pTemp->m_pSourceVoiceWin8->Start( 0, XAUDIO2_COMMIT_NOW );
	if ( FAILED(hr) )
	{
		agk::Error( "Failed to start sound" );
		return 0;
	}

	return 1;
}

void AGKMusicOGG::PlatformSetVolume()
{
	if ( !(g_iXAudio2Mode == 1 && pXAudio2) && !(g_iXAudio2Mode == 2 && pXAudio2Win8) ) return;
	if ( !m_pSoundData ) return;
	AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;

	float fVol = (m_iVolume * g_iMasterVolume) / 10000.0f;
	if ( pTemp->m_pSourceVoice ) pTemp->m_pSourceVoice->SetVolume( fVol );
	else if ( pTemp->m_pSourceVoiceWin8 ) pTemp->m_pSourceVoiceWin8->SetVolume( fVol );
}

void AGKMusicOGG::PlatformPause()
{
	if ( !(g_iXAudio2Mode == 1 && pXAudio2) && !(g_iXAudio2Mode == 2 && pXAudio2Win8) ) return;
	if ( !m_pSoundData ) return;
	AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;

	if ( pTemp->m_pSourceVoice ) pTemp->m_pSourceVoice->Stop();
	if ( pTemp->m_pSourceVoiceWin8 ) pTemp->m_pSourceVoiceWin8->Stop();
}

void AGKMusicOGG::PlatformResume()
{
	if ( !(g_iXAudio2Mode == 1 && pXAudio2) && !(g_iXAudio2Mode == 2 && pXAudio2Win8) ) return;
	if ( !m_pSoundData ) return;
	AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;

	if ( pTemp->m_pSourceVoice ) pTemp->m_pSourceVoice->Start();
	if ( pTemp->m_pSourceVoiceWin8 ) pTemp->m_pSourceVoiceWin8->Start();
}

void AGKMusicOGG::PlatformStop()
{
	if ( !(g_iXAudio2Mode == 1 && pXAudio2) && !(g_iXAudio2Mode == 2 && pXAudio2Win8) ) return;
	if ( !m_pSoundData ) return;
	AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;

	if ( pTemp->m_pSourceVoice ) 
	{
		pTemp->m_pSourceVoice->Stop();
		pTemp->m_pSourceVoice->FlushSourceBuffers();
	}
	if ( pTemp->m_pSourceVoiceWin8 ) 
	{
		pTemp->m_pSourceVoiceWin8->Stop();
		pTemp->m_pSourceVoiceWin8->FlushSourceBuffers();
	}

	pTemp->m_iResetFlag[ 0 ] = 0;
	pTemp->m_iResetFlag[ 1 ] = 0;
	pTemp->m_iResetFlag[ 2 ] = 0;
}

void AGKMusicOGG::PlatformClearBuffers()
{
	if ( !(g_iXAudio2Mode == 1 && pXAudio2) && !(g_iXAudio2Mode == 2 && pXAudio2Win8) ) return;
	if ( !m_pSoundData ) return;
	AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;
		
	if ( !pTemp->m_pSourceVoice && !pTemp->m_pSourceVoiceWin8 ) return;

	if ( pTemp->m_pSourceVoice ) 
	{
		pTemp->m_pSourceVoice->Stop();
		pTemp->m_pSourceVoice->FlushSourceBuffers();
	}
	else if ( pTemp->m_pSourceVoiceWin8 ) 
	{
		pTemp->m_pSourceVoiceWin8->Stop();
		pTemp->m_pSourceVoiceWin8->FlushSourceBuffers();
	}

	pTemp->m_iResetFlag[ 0 ] = 0;
	pTemp->m_iResetFlag[ 1 ] = 0;
	pTemp->m_iResetFlag[ 2 ] = 0;
}

void AGKMusicOGG::PlatformReset()
{
	if ( !(g_iXAudio2Mode == 1 && pXAudio2) && !(g_iXAudio2Mode == 2 && pXAudio2Win8) ) return;
	if ( !m_pSoundData ) return;
	AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;

	if ( pTemp->m_pSourceVoice ) pTemp->m_pSourceVoice->DestroyVoice();
	if ( pTemp->m_pSourceVoiceWin8 ) pTemp->m_pSourceVoiceWin8->DestroyVoice();
	pTemp->m_pSourceVoice = 0;
	pTemp->m_pSourceVoiceWin8 = 0;

	pTemp->m_iResetFlag[ 0 ] = 0;
	pTemp->m_iResetFlag[ 1 ] = 0;
	pTemp->m_iResetFlag[ 2 ] = 0;
}

int AGKMusicOGG::PlatformGetNumBuffers()
{
	if ( !(g_iXAudio2Mode == 1 && pXAudio2) && !(g_iXAudio2Mode == 2 && pXAudio2Win8) ) return 0;
	if ( !m_pSoundData ) return 0;
	AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;
		
	if ( !pTemp->m_pSourceVoice && !pTemp->m_pSourceVoiceWin8 ) return 0;

	XAUDIO2_VOICE_STATE state;
	if ( pTemp->m_pSourceVoice ) pTemp->m_pSourceVoice->GetState( &state );
	else pTemp->m_pSourceVoiceWin8->GetState( &state, /*XAUDIO2_VOICE_NOSAMPLESPLAYED*/ 0x100 );

	return state.BuffersQueued;
}

int AGKMusicOGG::PlatformGetMaxBuffers() { return 3; }

int AGKMusicOGG::PlatformAddBuffer( int *reset )
{
	if ( !(g_iXAudio2Mode == 1 && pXAudio2) && !(g_iXAudio2Mode == 2 && pXAudio2Win8) ) return 0;
	if ( !m_pSoundData ) return 0;
	AGKOGGSoundData *pTemp = (AGKOGGSoundData*)m_pSoundData;
		
	if ( !pTemp->m_pSourceVoice && !pTemp->m_pSourceVoiceWin8 ) return 0;

	//uString info;
	//info.Format( "Adding Buffer: %d", m_iBufferSize );
	//agk::Warning( info );

	int currBuf = pTemp->m_iCurrBuffer;
	if ( reset ) *reset = pTemp->m_iResetFlag[ currBuf ];
	memcpy( pTemp->m_pBuffers[currBuf], m_pDecodeBuffer, m_iBufferSize );
	pTemp->m_iResetFlag[ currBuf ] = 0;

	pTemp->m_XABuffer.AudioBytes = m_iBufferSize;
	pTemp->m_XABuffer.pAudioData = pTemp->m_pBuffers[ currBuf ];

	if ( m_iFlags & AGK_MUSIC_OGG_BUFFER_END ) 
	{
		// check if we will be looping
		int shouldLoop = 0;
		if ( m_iLoop == 1 ) shouldLoop = 1;
		else if ( m_iLoopCount+1 < m_iLoop ) shouldLoop = 1;

		if ( !shouldLoop ) pTemp->m_XABuffer.Flags = XAUDIO2_END_OF_STREAM;

		pTemp->m_iResetFlag[ currBuf ] = 1;
	}

	pTemp->m_iCurrBuffer++;
	if ( (int)pTemp->m_iCurrBuffer >= PlatformGetMaxBuffers() ) pTemp->m_iCurrBuffer = 0;

	HRESULT hr = -1;
	if ( pTemp->m_pSourceVoice ) hr = pTemp->m_pSourceVoice->SubmitSourceBuffer( &(pTemp->m_XABuffer) );
	if ( pTemp->m_pSourceVoiceWin8 ) hr = pTemp->m_pSourceVoiceWin8->SubmitSourceBuffer( &(pTemp->m_XABuffer) );
	if( FAILED(hr) )
	{
		agk::Error( "Failed to submit music source voice" );
		return 0;
	}

	return 1;
}

// Sound

void cSoundMgr::PlatformInit()
{
	HRESULT hr;

	if ( g_iXAudio2Mode == 0 )
	{
		// needed for sound setup
		if ( !g_bCoInit ) 
		{
			CoInitializeEx(NULL, COINIT_MULTITHREADED);
			g_bCoInit = true;
		}

		// attempt to load the Windows 8/10 version of XAudio2
		if ( g_hXAudioDLL == 0 ) g_hXAudioDLL = LoadLibrary( "XAudio2_9.dll" ); // Windows 10
		if ( g_hXAudioDLL == 0 ) g_hXAudioDLL = LoadLibrary( "XAudio2_8.dll" ); // Windows 8

		if ( g_hXAudioDLL == 0 )
		{
			// use DirectX SDK version of XAudio2
			g_iXAudio2Mode = 1; 
			g_hXAudioDLL = LoadLibrary( "XAudio2_7.dll" ); // must still load it to prevent it being unloaded by accident (Windows bug)

			UINT32 flags = 0;
			#ifdef _DEBUG
				//flags |= XAUDIO2_DEBUG_ENGINE;
			#endif

			if ( FAILED(hr = XAudio2Create( &pXAudio2, flags, XAUDIO2_DEFAULT_PROCESSOR ) ) ) 
			{
				agk::Warning( "Failed to create XAudio2, make sure you have the \"DirectX End-User Runtime\" installed." );
				agk::Message( "Failed to create XAudio2, make sure you have the \"DirectX End-User Runtime\" installed." );
				pXAudio2 = NULL;
				return;
			}

			if ( FAILED(hr = pXAudio2->CreateMasteringVoice( &pMasterVoice, 2, 44100, 0, 0, NULL ) ) )
			{
				agk::Warning( "Failed to create XAudio2 Mastering Voice" );
				pXAudio2 = NULL;
				return;
			}
		}
		else
		{
			// Use Windows 8/10 version of XAudio2
			g_iXAudio2Mode = 2;
			typedef HRESULT (__stdcall *XAudio2CreateFunc)(XAudio2Win8::IXAudio2** ppXAudio2, UINT32 Flags, XAUDIO2_PROCESSOR XAudio2Processor);
			XAudio2CreateFunc XAudio2Create_dll = (XAudio2CreateFunc) GetProcAddress( g_hXAudioDLL, "XAudio2Create" );
			if ( !XAudio2Create_dll || FAILED(XAudio2Create_dll(&pXAudio2Win8, 0, XAUDIO2_DEFAULT_PROCESSOR)) )
			{
				agk::Warning( "Failed to create XAudio2 from DLL" );
				pXAudio2Win8 = 0;
				return;
			}

			if ( FAILED(pXAudio2Win8->CreateMasteringVoice(&pMasterVoiceWin8, 2, 44100, 0, 0, NULL)) )
			{
				agk::Warning( "Failed to create XAudio2 Mastering Voice from DLL" );
				pXAudio2Win8 = NULL;
				return;
			}
		}
	}

	m_fMinPlaybackRate = 1/1024.0f;
	m_fMaxPlaybackRate = 1024/1.0f;
	m_fStepPlaybackRate = 0;
	
/*
	// DirectSound Setup
	hr = DirectSoundCreate8( NULL, &g_pDSDevice, NULL );
	if ( FAILED(hr) )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to create DirectSound device, make sure you have the latest version of DirectX installed" );
#endif
		return;
	}

	g_pDSDevice->SetCooperativeLevel( g_hWnd, DSSCL_PRIORITY );
*/
}

void cSoundMgr::AppPaused()
{
    
}

void cSoundMgr::AppResumed()
{
    
}

void cSoundMgr::PlatformAddFile( cSoundFile *pSound )
{
	// no further processing needed on windows
	return;
}

void cSoundMgr::PlatformUpdate()
{
	XAUDIO2_VOICE_STATE state;
	cSoundInst *pSound = m_pSounds;
	cSoundInst *pNext = UNDEF;
	while ( pSound )
	{
		pNext = pSound->m_pNextInst;

		if ( pSound->m_pSourceVoice || pSound->m_pSourceVoiceWin8 )
		{
			if ( pSound->m_pSourceVoice ) pSound->m_pSourceVoice->GetState( &state );
			else pSound->m_pSourceVoiceWin8->GetState( &state, /*XAUDIO2_VOICE_NOSAMPLESPLAYED*/ 0x100 );
			
			if ( state.BuffersQueued <= 1 && (pSound->m_iLoop == 1 || pSound->m_iLoopCount+1 < pSound->m_iLoop) )
			{
				pSound->m_iLoopCount++;
				if ( pSound->m_iLoop == 1 || pSound->m_iLoopCount+1 < pSound->m_iLoop ) 
				{
					if ( pSound->m_pSourceVoice ) pSound->m_pSourceVoice->SubmitSourceBuffer( &(pSound->m_XABuffer) );
					else pSound->m_pSourceVoiceWin8->SubmitSourceBuffer( &(pSound->m_XABuffer) );
				}
			}
			else if ( state.BuffersQueued == 0 )
			{
				pSound->m_iLoopCount++;

				// sound is finished, cleanup and move to used list
				if ( m_pSoundFiles[ pSound->m_iParent ] ) m_pSoundFiles[ pSound->m_iParent ]->m_iInstances--;
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
		if ( pSound->m_pSourceVoice ) pSound->m_pSourceVoice->SetVolume( fVol );
		else if ( pSound->m_pSourceVoiceWin8 ) pSound->m_pSourceVoiceWin8->SetVolume( fVol );

		//float iVol = ( m_iGlobalVolume * pSound->m_iVolume ) / 10000.0f;
		//if ( iVol > 1 ) iVol = 1;
		//if ( iVol < 0.00001f ) iVol = -10000;
		//else iVol = (log10(iVol)*20) * 100;
		//if ( pSound->m_pDSBuffer ) pSound->m_pDSBuffer->SetVolume( (long) iVol );

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

	if ( pMasterVoice ) pMasterVoice->DestroyVoice();
	if ( pXAudio2 ) pXAudio2->Release();
	
	if ( pMasterVoiceWin8 ) pMasterVoiceWin8->DestroyVoice();
	if ( pXAudio2Win8 ) pXAudio2Win8->Release();	

	if ( g_hXAudioDLL ) 
	{
		FreeLibrary( g_hXAudioDLL );
		g_hXAudioDLL = 0;
	}
}

UINT cSoundMgr::PlatformCreateInstance( cSoundMgr::cSoundFile *pSound, int iVol, int iLoop, int iPriority )
{
	if ( !(g_iXAudio2Mode == 1 && pXAudio2) && !(g_iXAudio2Mode == 2 && pXAudio2Win8) ) return 0;

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
		
	// xaudio
	pSoundInst->m_XABuffer.AudioBytes = pSound->m_uDataSize;  // buffer containing audio data
	pSoundInst->m_XABuffer.pAudioData = pSound->m_pRawData;  // size of the audio buffer in bytes
	if ( iLoop == 0 ) pSoundInst->m_XABuffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
	//if ( iLoop > 0 ) pSoundInst->m_XABuffer.LoopCount = XAUDIO2_LOOP_INFINITE;

	HRESULT hr = -1;
	if ( g_iXAudio2Mode == 1 ) hr = pXAudio2->CreateSourceVoice( &(pSoundInst->m_pSourceVoice), (::WAVEFORMATEX*)&(pSound->m_fmt), 0,  XAUDIO2_MAX_FREQ_RATIO, NULL, NULL, NULL );
	else if ( g_iXAudio2Mode == 2 ) hr = pXAudio2Win8->CreateSourceVoice( &(pSoundInst->m_pSourceVoiceWin8), (::WAVEFORMATEX*)&(pSound->m_fmt), 0,  XAUDIO2_MAX_FREQ_RATIO, NULL, NULL, NULL );
	if( FAILED(hr) ) 
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to create instance" );
#endif
		delete pSoundInst;
		return 0;
	}

	hr = -1;
	if ( g_iXAudio2Mode == 1 ) hr = pSoundInst->m_pSourceVoice->SubmitSourceBuffer( &(pSoundInst->m_XABuffer) );
	else if ( g_iXAudio2Mode == 2 ) hr = pSoundInst->m_pSourceVoiceWin8->SubmitSourceBuffer( &(pSoundInst->m_XABuffer) );
	if( FAILED(hr) )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to submit source voice" );
#endif
		delete pSoundInst;
		return 0;
	}

	// if looping queue up the buffer again
	if ( iLoop > 0 ) 
	{
		if ( pSoundInst->m_pSourceVoice ) pSoundInst->m_pSourceVoice->SubmitSourceBuffer( &(pSoundInst->m_XABuffer) );
		else if ( pSoundInst->m_pSourceVoiceWin8 ) pSoundInst->m_pSourceVoiceWin8->SubmitSourceBuffer( &(pSoundInst->m_XABuffer) );
	}

	float fVol = m_iGlobalVolume / 100.0f;
	fVol *= ( pSoundInst->m_iVolume / 100.0f );
	if ( pSoundInst->m_pSourceVoice ) pSoundInst->m_pSourceVoice->SetVolume( fVol );
	else if ( pSoundInst->m_pSourceVoiceWin8 ) pSoundInst->m_pSourceVoiceWin8->SetVolume( fVol );

	hr = -1;
	if ( g_iXAudio2Mode == 1 ) hr = pSoundInst->m_pSourceVoice->Start( 0, XAUDIO2_COMMIT_NOW );
	else if ( g_iXAudio2Mode == 2 ) hr = pSoundInst->m_pSourceVoiceWin8->Start( 0, XAUDIO2_COMMIT_NOW );
	if ( FAILED(hr) )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Failed to start sound" );
#endif
		delete pSoundInst;
		return 0;
	}

	// add it to the running list
	pSoundInst->m_pPrevInst = UNDEF;
	pSoundInst->m_pNextInst = m_pSounds;
	m_pSounds = pSoundInst;
	if ( pSoundInst->m_pNextInst ) pSoundInst->m_pNextInst->m_pPrevInst = pSoundInst;

	m_cSoundInstances.AddItem( pSoundInst, pSoundInst->m_iID );

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
	if ( pSound->m_pSourceVoice ) pSound->m_pSourceVoice->SetVolume( fVol );
	else if ( pSound->m_pSourceVoiceWin8 ) pSound->m_pSourceVoiceWin8->SetVolume( fVol );
}

void cSoundMgr::SetInstanceRate( UINT instance, float rate )
{
	cSoundInst *pSound = m_cSoundInstances.GetItem( instance );
	if ( !pSound ) return;

	if ( rate < m_fMinPlaybackRate ) rate = m_fMinPlaybackRate;
	if ( rate > m_fMaxPlaybackRate ) rate = m_fMaxPlaybackRate;

	pSound->m_fRate = rate;

	if ( pSound->m_pSourceVoice ) pSound->m_pSourceVoice->SetFrequencyRatio( rate );
	else if ( pSound->m_pSourceVoiceWin8 ) pSound->m_pSourceVoiceWin8->SetFrequencyRatio( rate );
}

void cSoundMgr::SetInstanceBalance( UINT instance, float balance )
{
	cSoundInst *pSound = m_cSoundInstances.GetItem( instance );
	if ( !pSound ) return;

	if ( balance < -1 ) balance = -1;
	if ( balance > 1 ) balance = 1;

	pSound->m_fBalance = balance;

	float matrix[4];
	if ( m_pSoundFiles[ pSound->m_iParent ]->m_fmt.nChannels == 2 )
	{
		matrix[ 0 ] = balance < 0 ? 1 : 1-balance;
		matrix[ 2 ] = balance < 0 ? 0 : balance;

		matrix[ 1 ] = balance > 0 ? 0 : -balance;
		matrix[ 3 ] = balance > 0 ? 1 : 1+balance;
	}
	else
	{
		matrix[ 0 ] = balance < 0 ? 1 : 1-balance;
		matrix[ 1 ] = balance > 0 ? 1 : 1+balance;
	}

	if ( pSound->m_pSourceVoice ) pSound->m_pSourceVoice->SetOutputMatrix( NULL, m_pSoundFiles[ pSound->m_iParent ]->m_fmt.nChannels, 2,  matrix );
	else if ( pSound->m_pSourceVoiceWin8 ) pSound->m_pSourceVoiceWin8->SetOutputMatrix( NULL, m_pSoundFiles[ pSound->m_iParent ]->m_fmt.nChannels, 2,  matrix );
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


/*
void cSoundMgr::PlatformPauseInstances( UINT iID )
{
	// pausing won't work as it will look like a finished sound and get deleted by the system.

	cSoundInst *pNext = UNDEF;
	cSoundInst *pSound = m_pSounds;
	while ( pSound )
	{
		pNext = pSound->m_pNextInst;

		if ( iID == 0 || pSound->m_iParent == iID )
		{
			// stop sound, keep on playing list
			//if ( pSound->m_pSourceVoice ) pSound->m_pSourceVoice->Stop();
			if ( pSound->m_pDSBuffer ) pSound->m_pDSBuffer->Stop();
		}

		pSound = pNext;
	}
}

void cSoundMgr::PlatformResumeInstances( UINT iID )
{
	// no Pause function so no resume
	cSoundInst *pNext = UNDEF;
	cSoundInst *pSound = m_pSounds;
	while ( pSound )
	{
		pNext = pSound->m_pNextInst;

		if ( iID == 0 || pSound->m_iParent == iID )
		{
			// resume sounds on playing list
			//if ( pSound->m_pSourceVoice ) pSound->m_pSourceVoice->Start();
			if ( pSound->m_pDSBuffer ) pSound->m_pDSBuffer->Play();
		}

		pSound = pNext;
	}
}
*/

//****f* Video/Youtube/PlayYoutubeVideo
// FUNCTION
//   Plays the specified Youtube video in a separate window above your app. For Android you must enable the Youtube 
//   Data API v3 in the Google Cloud Console for your app, and create an API key for it in the credentials section. 
//   You can create a key specifically for use with the Youtube API, or you can use an unrestricted key that can be 
//   used by multiple APIs. The videoID is the string that follows the ?v= part of a Youtube URL, e.g. "eLIgxYHCgWA". 
//   You can also specify a start time to start playing the video from part way through instead of starting from the 
//   beginning. The time is specified in seconds and accepts decimal values to represent fractions of a second.<br/>
//   <br/>
//   On Windows, Mac, and Linux this command will open the default browser to play the Youtube video. The developer 
//   key field is only required on Android.
// INPUTS
//   developerKey -- The API key credential created in the Google Cloud Console for the Youtube Data API v3.
//   videoID -- The ID of the video, e.g. eLIgxYHCgWA
//   startTime -- The seek time in seconds from which to start playing the video
// SOURCE
void agk::PlayYoutubeVideo( const char* developerKey, const char* videoID, float startTime )
//****
{
	uString sURL;
	sURL.Format( "https://www.youtube.com/watch?v=%s&t=%d", videoID, (int)startTime );
	OpenBrowser( sURL );
}

// video commands 

//****f* Video/General/LoadVideo
// FUNCTION
//   This command loads a video file ready to be played. Only one video file can be loaded at a time
//   so loading a video deletes any existing video. On iOS, Android, and Mac it is recommended that 
//   you use .mp4 files with the H264 (MPEG-4 AVC) codec for video. For audio it must be encoded in MP3 
//   or AAC. On Windows it depends on the codecs installed and although in some cases it will play mp4
//   files it is recommended that you use wmv on Windows for maximum support.
//   Returns 1 if successful, 0 if there was an error, -1 if this platform does not support videos.
// INPUTS
//   szFilename -- The video file to load.
// SOURCE
int agk::LoadVideo( const char *szFilename )
//****
{
	if ( !g_bGLInit ) return 0;

	if ( g_bMFPlatExists )
	{
		if ( g_pVideoSession ) DeleteVideo();

		sVideoPath.SetStr( szFilename );

		if ( !GetRealPath( sVideoPath ) )
		{
			agk::Error( "Failed to load video file - file not found" );
			return 0;
		}

		m_fVideoX = 0;
		m_fVideoY = 0;
		m_fVideoWidth = (float) agk::GetVirtualWidth();
		m_fVideoHeight = (float) agk::GetVirtualHeight();

		IMFSourceResolver* pSourceResolver = NULL;
		IMFPresentationDescriptor *pPD = NULL;
		IMFMediaTypeHandler *pHandler = NULL;
		DWORD cStreams = 0;
		UINT64 duration;
		BOOL bFound = FALSE;
		BOOL fSelected = FALSE;
		GUID majorType;
		IMFStreamDescriptor *pSD = NULL;
		IMFMediaType *pType2;
		UINT32 iWidth, iHeight;

		HRESULT hr = MFCreateSourceResolver(&pSourceResolver);
		if ( FAILED(hr) ) { agk::Error( "Failed to create source resolver" ); return 0; }

		MF_OBJECT_TYPE ObjectType;
		IUnknown* pUnknownSource = NULL;
		int iLength = MultiByteToWideChar( CP_UTF8, 0, sVideoPath.GetStr(), -1, 0, 0 );
		WCHAR *wstr = new WCHAR[ iLength + 1 ];
		MultiByteToWideChar( CP_UTF8, 0, sVideoPath.GetStr(), -1, wstr, iLength+1 );
		hr = pSourceResolver->CreateObjectFromURL(wstr, MF_RESOLUTION_MEDIASOURCE, NULL, &ObjectType, &pUnknownSource);
		delete [] wstr;
		if ( FAILED(hr) ) { agk::Error( "Failed to create object from URL" ); goto failed; }

		hr = pUnknownSource->QueryInterface(IID_PPV_ARGS(&g_pVideoSource));
		pUnknownSource->Release();
		if ( FAILED(hr) ) { agk::Error( "Failed to query source interface" ); goto failed; }

		hr = g_pVideoSource->CreatePresentationDescriptor(&pPD);
		if ( FAILED(hr) ) { agk::Error( "Failed to create presentation description" ); goto failed; }
		hr = pPD->GetStreamDescriptorCount(&cStreams);
		if ( FAILED(hr) ) { agk::Error( "Failed to get stream description count" ); goto failed; }

		hr = pPD->GetUINT64(MF_PD_DURATION, (UINT64*)&duration);
		if ( FAILED(hr) ) { agk::Error( "Failed to get video duration" ); goto failed; }
		fVideoDuration = duration / 10000000.0f;
		//uString info; info.Format( "duration: %f", fVideoDuration ); agk::Warning( info );

		for (DWORD i = 0; i < cStreams; i++)
		{
			hr = pPD->GetStreamDescriptorByIndex(i, &fSelected, &pSD);
			if ( FAILED(hr) ) { agk::Error( "Failed to get stream descriptor by index" ); goto failed; }
			hr = pSD->GetMediaTypeHandler(&pHandler);
			if ( FAILED(hr) ) { agk::Error( "Failed to get media type handler" ); goto failed; }
			hr = pHandler->GetMajorType(&majorType);
			if ( FAILED(hr) ) { agk::Error( "Failed to get media major type" ); goto failed; }

			if (majorType == MFMediaType_Video && fSelected)
			{
				hr = pHandler->GetCurrentMediaType( &pType2 );
				if ( FAILED(hr) ) { agk::Error( "Failed to get media type" ); goto failed; }

				MFGetAttributeSize( pType2, MF_MT_FRAME_SIZE, &iWidth, &iHeight );
				//uString info; info.Format( "Width: %d, Height: %d", iWidth, iHeight ); agk::Warning( info );
				iVideoWidth = iWidth;
				iVideoHeight = iHeight;

				SAFE_RELEASE( pType2 )
				bFound = TRUE;
				break;
			}

			SAFE_RELEASE( pSD )
			SAFE_RELEASE( pHandler )
		}

		SAFE_RELEASE( pType2 )
		SAFE_RELEASE( pHandler )
		SAFE_RELEASE( pSD )
		SAFE_RELEASE( pPD )
		SAFE_RELEASE( pSourceResolver )
		if ( g_pVideoSource ) g_pVideoSource->Shutdown();
		SAFE_RELEASE( g_pVideoSource )
		goto success;

	failed:
		
		SAFE_RELEASE( pType2 )
		SAFE_RELEASE( pHandler )
		SAFE_RELEASE( pSD )
		SAFE_RELEASE( pPD )
		SAFE_RELEASE( pSourceResolver )
		if ( g_pVideoSource ) g_pVideoSource->Shutdown();
		SAFE_RELEASE( g_pVideoSource )
		return 0;

	success:
		
		if ( !bFound ) agk::Error( "Failed to get video width and height" );
	}
	else
	{
		if ( g_pVideoGraphBuilder ) DeleteVideo();

		sVideoPath.SetStr( szFilename );

		if ( !GetRealPath( sVideoPath ) )
		{
			agk::Error( "Failed to load video file - file not found" );
			return 0;
		}

		m_fVideoX = 0;
		m_fVideoY = 0;
		m_fVideoWidth = (float) agk::GetVirtualWidth();
		m_fVideoHeight = (float) agk::GetVirtualHeight();

		// temporarily construct a filter graph to get the width/height/duration
		HRESULT h = 0;
		h = CoCreateInstance(CLSID_FilterGraph, NULL,CLSCTX_INPROC_SERVER, IID_IGraphBuilder,(void **)&g_pVideoGraphBuilder);
		if (FAILED(h))
		{
			agk::Error( "DirectShow could not be initialised. Failed to CoCreateInstance" );
			return 0;
		}

		h = g_pVideoGraphBuilder->QueryInterface(IID_IMediaSeeking,(void **)&g_pVideoMediaSeeking);
		if (FAILED(h)) 
		{
			agk::Error( "Failed to load DirectShow interface." );
			return 0;			
		}

		int iLength = MultiByteToWideChar( CP_UTF8, 0, sVideoPath.GetStr(), -1, 0, 0 );
		WCHAR *wstr = new WCHAR[ iLength + 1 ];
		MultiByteToWideChar( CP_UTF8, 0, sVideoPath.GetStr(), -1, wstr, iLength+1 );
    
		h = g_pVideoGraphBuilder->RenderFile( wstr, NULL );
		delete [] wstr;
		if ( FAILED(h) )
		{
			//agk::Error( "Failed to load video file" );
			switch( h )
			{
				case VFW_S_VIDEO_NOT_RENDERED: agk::Error("Failed to load video - unsupported video codec"); break;
				case VFW_E_INVALID_FILE_FORMAT: agk::Error("Failed to load video - invalid file format"); break;
				case VFW_E_UNKNOWN_FILE_TYPE: agk::Error("Failed to load video - unsupported media type"); break;
				case VFW_E_UNSUPPORTED_STREAM: agk::Error("Failed to load video - unsupported codec"); break;
				default: agk::Error( "Failed to load video file" );
			}

			return 0;
		}

		IBaseFilter *pVmr = NULL;
		h = g_pVideoGraphBuilder->FindFilterByName( L"Video Renderer", &pVmr );
		if ( FAILED(h) ) 
		{
			agk::Warning("Failed to get video width and height");
			iVideoWidth = -1;
			iVideoHeight = -1;
		}
		else
		{
			IBasicVideo *pBV = NULL;
			h = pVmr->QueryInterface(IID_PPV_ARGS(&pBV));
			if ( FAILED(h) ) 
			{
				agk::Warning("Failed to get video interface width and height");
				iVideoWidth = -1;
				iVideoHeight = -1;
			}
			else
			{
				long width = 0;
				long height = 0;
				pBV->get_SourceWidth( &width );
				pBV->get_SourceHeight( &height );
				iVideoWidth = (int) width;
				iVideoHeight = (int) height;
			}

			if ( pBV ) pBV->Release();
		}
	
		if ( pVmr ) pVmr->Release();

		long long pos;
		g_pVideoMediaSeeking->GetDuration( &pos );
		fVideoDuration = pos / 10000000.0f;

		// delete temporary graph, PlayVideo will construct its own
		g_pVideoMediaSeeking->Release();
		g_pVideoMediaSeeking = 0;

		g_pVideoGraphBuilder->Release();
		g_pVideoGraphBuilder = 0;
	}

	fVideoPos = -1;
	return 1;
}

void agk::HandleVideoEvents()
{
	if ( !g_pVideoMediaEventEx ) return;

	HRESULT h;
	long evCode;
	LONG_PTR param1, param2;
	while ( !FAILED(h = g_pVideoMediaEventEx->GetEvent(&evCode, &param1, &param2, 0)) )
	{
		if ( evCode == EC_COMPLETE )
		{
			StopVideo();
		}

		g_pVideoMediaEventEx->FreeEventParams( evCode, param1, param2 );
	}
}

void agk::ChangeVideoPointer( void *ptr )
{
	
}

//****f* Video/General/DeleteVideo
// FUNCTION
//   Stops any playing video and deletes any loaded video from memory.
// SOURCE
void agk::DeleteVideo()
//****
{
	StopVideo();

	// DirectShow
	if ( g_pVideoMediaControl ) g_pVideoMediaControl->Release();
	if ( g_pVideoMediaSeeking ) g_pVideoMediaSeeking->Release();
	if ( g_pVideoMediaEventEx ) g_pVideoMediaEventEx->Release();
	if ( g_pVideoBasicAudio ) g_pVideoBasicAudio->Release();
	if ( g_pVideoGraphBuilder ) g_pVideoGraphBuilder->Release();
	if ( pVideoSampleGrabber ) pVideoSampleGrabber->Release();
	if ( pVideoGrabberF ) pVideoGrabberF->Release();
	if ( pVideoNullF ) pVideoNullF->Release();
	if ( pVideoCallback ) delete pVideoCallback;

	g_pVideoMediaControl = 0;
	g_pVideoMediaSeeking = 0;
	g_pVideoMediaEventEx = 0;
	g_pVideoBasicAudio = 0;
	g_pVideoGraphBuilder = 0;
	pVideoSampleGrabber = 0;
	pVideoGrabberF = 0;
	pVideoNullF = 0;
	pVideoCallback = 0;

	// Media Foundation
	if ( g_pVideoClock ) g_pVideoClock->Release();
	if ( g_pVideoSampleGrabber ) g_pVideoSampleGrabber->Release();
	if ( g_pVideoSource ) { g_pVideoSource->Shutdown(); g_pVideoSource->Release(); }
	if ( g_pVideoSession ) { g_pVideoSession->Shutdown(); g_pVideoSession->Release(); }
	if ( g_pVideoVolume ) g_pVideoVolume->Release();
	
	g_pVideoClock = 0;
	g_pVideoSession = 0;
	g_pVideoSource = 0;
	g_pVideoSampleGrabber = 0;
	g_pVideoVolume = 0;

	if ( g_pVideoSprite ) delete g_pVideoSprite;
	
	g_pVideoSprite = 0;
	g_iVideoImageID = 0; // deleted by m_pVideoTexture below

	sVideoPath.SetStr("");
	fVideoDuration = 0;
	iVideoWidth = -1;
	iVideoHeight = -1;
	fVideoPos = -1;
	fVideoVolume = 100;
	if ( pVideoFrame ) { delete [] pVideoFrame; pVideoFrame = 0; }
	if ( pVideoFrameFinal ) { delete [] pVideoFrameFinal; pVideoFrameFinal = 0; }
	iVideoChanged = 0;

	if ( m_pVideoTexture ) 
	{
		m_cImageList.RemoveItem( m_pVideoTexture->m_iID );
		delete m_pVideoTexture; 
		m_pVideoTexture = 0;
	}
}

//****f* Video/General/SetVideoDimensions
// FUNCTION
//   Sets the width, height, and position of the video whilst it is playing. Can be called before or 
//   during playback. The coordinates are in AGK screen coordinates based on your chosen virtual 
//   resolution. The aspect ratio is not preserved and will be stretched to fit your chosen dimensions.
//   To find the actual size of the video in pixels use <i>GetVideoWidth</i> and <i>GetVideoHeight</i>.
//   An exception to the stretching is Windows which will always keep the aspect ratio of the original
//   video and add black borders where necessary.
// INPUTS
//   x -- The x coordinate to position the top left corner of the video
//   y -- The y coordinate to position the top left corner of the video
//   width -- The width to display the video
//   height -- The height to display the video
// SOURCE
void agk::SetVideoDimensions( float x, float y, float width, float height )
//****
{
	if ( !g_bGLInit ) return;
	if ( !g_hWnd ) return;

	m_fVideoX = x;
	m_fVideoY = y;
	m_fVideoWidth = width;
	m_fVideoHeight = height;
}

void agk::VideoUpdate()
{
	if ( g_pVideoSprite )
	{
		g_pVideoSprite->SetPosition( m_fVideoX, m_fVideoY );
		g_pVideoSprite->SetSize( m_fVideoWidth, m_fVideoHeight, false );
		g_pVideoSprite->Draw();
	}
	
	if ( g_bMFPlatExists && g_pVideoSession )
	{
		IMFMediaEvent *pEvent;
		while( g_pVideoSession->GetEvent( MF_EVENT_FLAG_NO_WAIT, &pEvent ) == S_OK )
		{
			MediaEventType eType;
			pEvent->GetType( &eType );
			if ( eType == MEEndOfPresentation )
			{
				StopVideo();
			}
			//uString info; info.Format( "Event: %d", eType ); agk::Warning( info );
			pEvent->Release();
		}
	}

	if ( m_iVideoPlayMode != 2 ) return;
	if ( g_bMFPlatExists && !g_pVideoSampleGrabber ) return;
	if ( !g_bMFPlatExists && !pVideoSampleGrabber ) return;
	if ( !pVideoFrame ) return;
	if ( !pVideoFrameFinal ) return;
	if ( !iVideoChanged ) return;
	
	// scope the auto lock
	{
		cAutoSLock autolock( &cVideoLock );
		
		
		if ( g_bMFPlatExists ) 
		{
			//memcpy( pVideoFrameFinal, (const void*)pVideoFrame, iVideoWidth*iVideoHeight*4 );

			/*
			// Ideal - MFVideoFormat_RGB24
			for ( int y = 0; y < iVideoHeight; y++ )
			{
				UINT index = y*iVideoWidth;
				
				for ( int x = 0; x < iVideoWidth; x++ )
				{
					unsigned char red = pVideoFrame[ index*3 + 2 ];
					unsigned char green = pVideoFrame[ index*3 + 1 ];
					unsigned char blue = pVideoFrame[ index*3 + 0 ];
								
					pVideoFrameFinal[ index ] = 0xff000000 | (blue << 16) | (green << 8) | red;
					index++;
				}
			}
			*/

			/*
			// MFVideoFormat_YV12
			int extra = iVideoHeight % 16;
			if ( extra > 0 ) extra = 16 - extra;

			UINT halfWidth = iVideoWidth/2;
			UINT halfHeight = iVideoHeight/2;
			UINT offsetV = iVideoWidth*iVideoHeight + iVideoWidth*extra;
			UINT offsetU = offsetV + halfWidth*halfHeight + halfWidth*(extra/2);
			
			for ( int y = 0; y < iVideoHeight; y++ )
			{
				UINT index = y*iVideoWidth;
				UINT indexU = (y>>1)*halfWidth + offsetU;
				UINT indexV = (y>>1)*halfWidth + offsetV;
				int flag = 1;

				for ( int x = 0; x < iVideoWidth; x++ )
				{
					int iY = pVideoFrame[ index ];
					int iU = pVideoFrame[ indexU ];
					int iV = pVideoFrame[ indexV ];
					iU -= 128;
					iV -= 128;

					// 0-255 conversion
					//int iRed = iY + iV*7/5;
					//int iGreen = iY - iU*43/125 - iV*357/500;
					//int iBlue = iY + iU*177/100;

					// 16-235 conversion
					iY -= 16;
					iY *= 298;
					int iRed = (iY + iV*409) >> 8;
					int iGreen = (iY - iU*100 - iV*208) >> 8;
					int iBlue = (iY + iU*516) >> 8;

					if( iRed < 0 ) iRed = 0;
					if( iGreen < 0 ) iGreen = 0;
					if( iBlue < 0 ) iBlue = 0;

					if( iRed > 255 ) iRed = 255;
					if( iGreen > 255 ) iGreen = 255;
					if( iBlue > 255 ) iBlue = 255;

					pVideoFrameFinal[ index ] = 0xff000000 | (iBlue << 16) | (iGreen << 8) | iRed;

					index++;
					flag = 1 - flag;
					indexU += flag;
					indexV += flag;
				}
			}
			*/

			// MFVideoFormat_YUY2
			unsigned int* pVideoFrameInt = (unsigned int*)pVideoFrame;
			for ( int y = 0; y < iVideoHeight; y++ )
			{
				UINT index = y*iVideoWidth/2;
				UINT index2 = y*iVideoWidth;
				
				for ( int x = 0; x < iVideoWidth/2; x++ )
				{
					unsigned int value = pVideoFrameInt[ index ];
					int iY0 = value & 0xff;
					int iU = (value >> 8) & 0xff;
					int iY1 = (value >> 16) & 0xff;
					int iV = value >> 24;
					iU -= 128;
					iV -= 128;

					// 0-255 conversion
					//int iRed = iY + iV*7/5;
					//int iGreen = iY - iU*43/125 - iV*357/500;
					//int iBlue = iY + iU*177/100;

					// 16-235 conversion
					iY0 -= 16;
					iY0 *= 298;
					iY0 += 128;
					int iRed = (iY0 + iV*409) >> 8;
					int iGreen = (iY0 - iU*100 - iV*208) >> 8;
					int iBlue = (iY0 + iU*516) >> 8;

					if( iRed < 0 ) iRed = 0;
					if( iGreen < 0 ) iGreen = 0;
					if( iBlue < 0 ) iBlue = 0;

					if( iRed > 255 ) iRed = 255;
					if( iGreen > 255 ) iGreen = 255;
					if( iBlue > 255 ) iBlue = 255;

					pVideoFrameFinal[ index2 ] = 0xff000000 | (iBlue << 16) | (iGreen << 8) | iRed;

					iY1 -= 16;
					iY1 *= 298;
					iY1 += 128;
					int iRed2 = (iY1 + iV*409) >> 8;
					int iGreen2 = (iY1 - iU*100 - iV*208) >> 8;
					int iBlue2 = (iY1 + iU*516) >> 8;

					if( iRed2 < 0 ) iRed2 = 0;
					if( iGreen2 < 0 ) iGreen2 = 0;
					if( iBlue2 < 0 ) iBlue2 = 0;

					if( iRed2 > 255 ) iRed2 = 255;
					if( iGreen2 > 255 ) iGreen2 = 255;
					if( iBlue2 > 255 ) iBlue2 = 255;

					pVideoFrameFinal[ index2 + 1 ] = 0xff000000 | (iBlue2 << 16) | (iGreen2 << 8) | iRed2;

					index++;
					index2 += 2;
				}
			}
		}
		else
		{
			for ( int y = 0; y < iVideoHeight; y++ )
			{
				UINT index = y*iVideoWidth;
				UINT index2 = (iVideoHeight-y-1)*iVideoWidth;

				for ( int x = 0; x < iVideoWidth; x++ )
				{
					unsigned char red = pVideoFrame[ index2*3 + 2 ];
					unsigned char green = pVideoFrame[ index2*3 + 1 ];
					unsigned char blue = pVideoFrame[ index2*3 + 0 ];
					unsigned char alpha = 0xff;
								
					pVideoFrameFinal[ index ] = (((((alpha << 8) | blue) << 8) | green) << 8) | red;

					index++;
					index2++;
				}
			}
		}

		iVideoChanged = 0;
	}

	m_pVideoTexture->LoadFromData( iVideoWidth, iVideoHeight, pVideoFrameFinal );
}

//****f* Video/General/PlayVideoToImage
// FUNCTION
//   Plays the video or resumes it from pausing. Plays the video to an image instead of to the screen, the image 
//   must not already exist. The image will be deleted when you call <i>DeleteVideo</i>, you must not delete it
//   manually.<br/>
//   You can either use this command or <i>PlayVideo</i>, but not both. If you pause the video you must resume 
//   it using the same command you used to start it, with the same parameters.<br/>
//   Currently this is only supported on iOS 7.0 or above, Android 4.0.3 or above, and Windows.
// INPUTS
//   imageID -- An ID to use to reference the new image, must not already exist
// SOURCE
void agk::PlayVideoToImage( UINT imageID )
//****
{
	if ( sVideoPath.GetLength() == 0 ) return;
	if ( m_iVideoPlayMode == 1 )
	{
		agk::Error( "Cannot use PlayVideo on a video started with PlayVideoToImage, use PlayVideoToImage to resume" );
		return;
	}
	m_iVideoPlayMode = 2;

	if( g_bMFPlatExists )
	{
		if ( !g_pVideoSession )
		{
			IMFActivate *pSinkActivate = NULL;
			IMFTopology *pTopology = NULL;
			IMFMediaType *pType = NULL;
			IUnknown* pUnknownSource = NULL;
			IMFSourceResolver* pSourceResolver = NULL;
			MF_OBJECT_TYPE ObjectType;
			IMFPresentationDescriptor *pPD = NULL;
			IMFStreamDescriptor *pSD = NULL;
			IMFMediaTypeHandler *pHandler = NULL;
			IMFTopologyNode *pNode1 = NULL;
			IMFTopologyNode *pNode2 = NULL;
			DWORD cStreams = 0;
			bool bVideoDone = false;
			bool bAudioDone = false;
			BOOL fSelected = FALSE;
			GUID majorType;
			IMFActivate *pAudioActivate = NULL;

			HRESULT hr = MFCreateMediaType(&pType);
			if ( FAILED(hr) ) { agk::Error( "Failed to create media type" ); goto failed; }
			hr = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
			if ( FAILED(hr) ) { agk::Error( "Failed to set media major type" ); goto failed; }
			hr = pType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_YUY2); // Using an RGB format generates an exception when decoding H264 video, for some reason
			if ( FAILED(hr) ) { agk::Error( "Failed to set media minor type" ); goto failed; }


			hr = SampleGrabberCB::CreateInstance(&g_pVideoSampleGrabber);
			if ( FAILED(hr) ) { agk::Error( "Failed to create sample grabber" ); goto failed; }
			hr = MFCreateSampleGrabberSinkActivate(pType, g_pVideoSampleGrabber, &pSinkActivate);
			if ( FAILED(hr) ) { agk::Error( "Failed to create sample grabber 2" ); goto failed; }

			hr = MFCreateMediaSession(NULL, &g_pVideoSession);
			if ( FAILED(hr) ) { agk::Error( "Failed to create media session" ); goto failed; }

			hr = MFCreateSourceResolver(&pSourceResolver);
			if ( FAILED(hr) ) { agk::Error( "Failed to create source resolver" ); goto failed; }
			
			int iLength = MultiByteToWideChar( CP_UTF8, 0, sVideoPath.GetStr(), -1, 0, 0 );
			WCHAR *wstr = new WCHAR[ iLength + 1 ];
			MultiByteToWideChar( CP_UTF8, 0, sVideoPath.GetStr(), -1, wstr, iLength+1 );
			hr = pSourceResolver->CreateObjectFromURL(wstr, MF_RESOLUTION_MEDIASOURCE | MF_RESOLUTION_CONTENT_DOES_NOT_HAVE_TO_MATCH_EXTENSION_OR_MIME_TYPE, NULL, &ObjectType, &pUnknownSource);
			delete [] wstr;
			if ( FAILED(hr) ) { agk::Error( "Failed to create load video from file" ); goto failed; }

			hr = pUnknownSource->QueryInterface(IID_PPV_ARGS(&g_pVideoSource));
			if ( FAILED(hr) ) { agk::Error( "Failed to query source interface" ); goto failed; }

			// Create Topology
			hr = MFCreateTopology(&pTopology);
			if ( FAILED(hr) ) { agk::Error( "Failed to create topology" ); goto failed; }
			hr = g_pVideoSource->CreatePresentationDescriptor(&pPD);
			if ( FAILED(hr) ) { agk::Error( "Failed to create presentation description" ); goto failed; }
			hr = pPD->GetStreamDescriptorCount(&cStreams);
			if ( FAILED(hr) ) { agk::Error( "Failed to get stream description count" ); goto failed; }

			for (DWORD i = 0; i < cStreams; i++)
			{
				hr = pPD->GetStreamDescriptorByIndex(i, &fSelected, &pSD);
				if ( FAILED(hr) ) { agk::Error( "Failed to get stream descriptor by index" ); goto failed; }
				hr = pSD->GetMediaTypeHandler(&pHandler);
				if ( FAILED(hr) ) { agk::Error( "Failed to get media type handler" ); goto failed; }
				hr = pHandler->GetMajorType(&majorType);
				if ( FAILED(hr) ) { agk::Error( "Failed to get media major type" ); goto failed; }

				if (majorType == MFMediaType_Video && fSelected && !bVideoDone)
				{
					hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pNode1);
					if ( FAILED(hr) ) { agk::Error( "Failed to create source node" ); goto failed; }
					hr = pNode1->SetUnknown(MF_TOPONODE_SOURCE, g_pVideoSource);
					if ( FAILED(hr) ) { agk::Error( "Failed to set node source" ); goto failed; }
					hr = pNode1->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPD);
					if ( FAILED(hr) ) { agk::Error( "Failed to set node presentation descriptor" ); goto failed; }
					hr = pNode1->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSD);
					if ( FAILED(hr) ) { agk::Error( "Failed to set node stream descriptor" ); goto failed; }
					hr = pTopology->AddNode(pNode1);
					if ( FAILED(hr) ) { agk::Error( "Failed to add topology node" ); goto failed; }

					hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pNode2);
					if ( FAILED(hr) ) { agk::Error( "Failed to create output node" ); goto failed; }
					hr = pNode2->SetObject(pSinkActivate);
					if ( FAILED(hr) ) { agk::Error( "Failed to set node sink" ); goto failed; }
					hr = pNode2->SetUINT32(MF_TOPONODE_STREAMID, 0);
					if ( FAILED(hr) ) { agk::Error( "Failed to set node2 stream ID" ); goto failed; }
					hr = pNode2->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE);
					if ( FAILED(hr) ) { agk::Error( "Failed to set node shutdown mode" ); goto failed; }
					hr = pTopology->AddNode(pNode2);
					if ( FAILED(hr) ) { agk::Error( "Failed to add output node" ); goto failed; }

					hr = pNode1->ConnectOutput(0, pNode2, 0);
					if ( FAILED(hr) ) { agk::Error( "Failed to connect nodes" ); goto failed; }
					
					bVideoDone = true;
				}
				else if (majorType == MFMediaType_Audio && fSelected && !bAudioDone)
				{
					hr = MFCreateTopologyNode(MF_TOPOLOGY_SOURCESTREAM_NODE, &pNode1);
					if ( FAILED(hr) ) { agk::Error( "Failed to create audio source node" ); goto failed; }
					hr = pNode1->SetUnknown(MF_TOPONODE_SOURCE, g_pVideoSource);
					if ( FAILED(hr) ) { agk::Error( "Failed to set audio node source" ); goto failed; }
					hr = pNode1->SetUnknown(MF_TOPONODE_PRESENTATION_DESCRIPTOR, pPD);
					if ( FAILED(hr) ) { agk::Error( "Failed to set audio node presentation descriptor" ); goto failed; }
					hr = pNode1->SetUnknown(MF_TOPONODE_STREAM_DESCRIPTOR, pSD);
					if ( FAILED(hr) ) { agk::Error( "Failed to set audio node stream descriptor" ); goto failed; }
					hr = pTopology->AddNode(pNode1);
					if ( FAILED(hr) ) { agk::Error( "Failed to add audio topology node" ); goto failed; }

					hr = MFCreateAudioRendererActivate( &pAudioActivate );
					if ( FAILED(hr) ) { agk::Error( "Failed to create audio output node" ); goto failed; }

					hr = MFCreateTopologyNode(MF_TOPOLOGY_OUTPUT_NODE, &pNode2);
					if ( FAILED(hr) ) { agk::Error( "Failed to create output node" ); goto failed; }
					hr = pNode2->SetObject(pAudioActivate);
					if ( FAILED(hr) ) { agk::Error( "Failed to set audio node sink" ); goto failed; }
					hr = pNode2->SetUINT32(MF_TOPONODE_STREAMID, 0);
					if ( FAILED(hr) ) { agk::Error( "Failed to set audio node2 stream ID" ); goto failed; }
					hr = pNode2->SetUINT32(MF_TOPONODE_NOSHUTDOWN_ON_REMOVE, FALSE);
					if ( FAILED(hr) ) { agk::Error( "Failed to set audio node shutdown mode" ); goto failed; }
					hr = pTopology->AddNode(pNode2);
					if ( FAILED(hr) ) { agk::Error( "Failed to add audio output node" ); goto failed; }

					hr = pNode1->ConnectOutput(0, pNode2, 0);
					if ( FAILED(hr) ) { agk::Error( "Failed to connect audio nodes" ); goto failed; }

					bAudioDone = true;
				}
				else
				{
					hr = pPD->DeselectStream(i);
					if ( FAILED(hr) ) { agk::Error( "Failed to deselect stream" ); goto failed; }
				}

				SAFE_RELEASE( pNode1 )
				SAFE_RELEASE( pNode2 )
				SAFE_RELEASE( pSD )
				SAFE_RELEASE( pHandler )
			}
			
			hr = g_pVideoSession->SetTopology( 0, pTopology );
			if ( FAILED(hr) ) { agk::Error( "Failed to set session topology" ); goto failed; }

			// must wait for topology to finish setting before we can get the volume interface
			float startTime = agk::Timer();
			IMFMediaEvent *pEvent = 0;
			do
			{
				agk::Sleep( 100 );
				if ( agk::Timer() - startTime > 4 )
				{
					agk::Error( "Failed to load video, format may not be supported" );
					goto failed;
				}
				hr = g_pVideoSession->GetEvent( MF_EVENT_FLAG_NO_WAIT, &pEvent );
				if ( hr == S_OK )
				{
					MediaEventType eType;
					pEvent->GetType( &eType );
					pEvent->Release();
					if ( eType == MESessionTopologyStatus ) break;
				}
			} while( hr == S_OK || hr == (0xC00D3E80L) ); // MF_E_NO_EVENTS_AVAILABLE

			if ( hr != S_OK )
			{
				agk::Error( "Failed to check video session topology" );
				goto failed;
			}
			
			hr = MFGetService( g_pVideoSession, MR_STREAM_VOLUME_SERVICE, IID_PPV_ARGS(&g_pVideoVolume) );
			//if ( FAILED(hr) ) { agk::Error( "Failed to get video volume interface" ); goto failed; }
			if ( FAILED(hr) ) { agk::Warning( "Failed to get video volume interface" ); g_pVideoVolume = 0; }
						
			SAFE_RELEASE( pNode2 )
			SAFE_RELEASE( pNode1 )
			SAFE_RELEASE( pHandler )
			SAFE_RELEASE( pSD )
			SAFE_RELEASE( pPD )
			SAFE_RELEASE( pUnknownSource )
			SAFE_RELEASE( pSourceResolver )
			SAFE_RELEASE( pSinkActivate )
			SAFE_RELEASE( pTopology )
			SAFE_RELEASE( pType )
			SAFE_RELEASE( pAudioActivate )
			goto success;

		failed:
			SAFE_RELEASE( g_pVideoVolume )
			SAFE_RELEASE( pNode2 )
			SAFE_RELEASE( pNode1 )
			SAFE_RELEASE( pHandler )
			SAFE_RELEASE( pSD )
			SAFE_RELEASE( pPD )
			SAFE_RELEASE( pUnknownSource )
			SAFE_RELEASE( pSourceResolver )
			SAFE_RELEASE( pSinkActivate )
			SAFE_RELEASE( pTopology )
			SAFE_RELEASE( pType )
			SAFE_RELEASE( pAudioActivate )
			if ( g_pVideoSource ) g_pVideoSource->Shutdown();
			SAFE_RELEASE( g_pVideoSource );
			if ( g_pVideoSession ) g_pVideoSession->Shutdown();
			SAFE_RELEASE( g_pVideoSession )
			return;

		success:
		
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
				m_pVideoTexture = new cImage();
				m_pVideoTexture->CreateBlankImage( iVideoWidth, iVideoHeight, 0, 0 );
				m_pVideoTexture->m_iID = imageID;
				m_cImageList.AddItem( m_pVideoTexture, imageID );
			}

			pVideoFrame = new unsigned char[ iVideoWidth*iVideoHeight*4 ];
			pVideoFrameFinal = new unsigned int[ iVideoWidth*iVideoHeight ];

			SetVideoVolume( fVideoVolume );

			g_pVideoClock = 0;
			IMFClock *pClock = 0;
			hr = g_pVideoSession->GetClock(&pClock);
			if ( SUCCEEDED(hr) && pClock )
			{
				hr = pClock->QueryInterface(IID_PPV_ARGS(&g_pVideoClock));
				pClock->Release();
			}
		}

		PROPVARIANT var;
		PropVariantInit(&var); 
		var.vt = VT_EMPTY;

		if ( fVideoPos > fVideoDuration ) fVideoPos = 0;
		if ( fVideoPos >= 0 ) 
		{
			long long pos = (long long) (fVideoPos * 10000000);
			var.vt = VT_I8;
			var.hVal.QuadPart = pos;
			fVideoPos = -1;
		}

		HRESULT hr = g_pVideoSession->Start( NULL, &var );
		if ( FAILED(hr) ) { agk::Error( "Failed to start session" ); return; }
		g_bVideoPlaying = TRUE;
	}
	else
	{
		if ( !g_pVideoGraphBuilder )
		{
			HRESULT h = 0;
			h = CoCreateInstance(CLSID_FilterGraph, NULL,CLSCTX_INPROC_SERVER, IID_IGraphBuilder,(void **)&g_pVideoGraphBuilder);
			if (FAILED(h))
			{
				agk::Error( "DirectShow could not be initialised. Failed to CoCreateInstance" );
				return;
			}
		
			h = g_pVideoGraphBuilder->QueryInterface(IID_IMediaControl,(void **)&g_pVideoMediaControl);
			if (!FAILED(h)) h = g_pVideoGraphBuilder->QueryInterface(IID_IMediaSeeking,(void **)&g_pVideoMediaSeeking);
			if (!FAILED(h)) h = g_pVideoGraphBuilder->QueryInterface(IID_IMediaEventEx,(void **)&g_pVideoMediaEventEx);
			if (!FAILED(h)) h = g_pVideoGraphBuilder->QueryInterface(IID_IBasicAudio, (void **)&g_pVideoBasicAudio);
			if (FAILED(h)) 
			{
				agk::Error( "Failed to load DirectShow interface" );
				return;			
			}

			// add source 
			int iLength = MultiByteToWideChar( CP_UTF8, 0, sVideoPath.GetStr(), -1, 0, 0 );
			WCHAR *wstr = new WCHAR[ iLength + 1 ];
			MultiByteToWideChar( CP_UTF8, 0, sVideoPath.GetStr(), -1, wstr, iLength+1 );

			IBaseFilter *pSourceFilter = 0;
			h = g_pVideoGraphBuilder->AddSourceFilter( wstr, wstr, &pSourceFilter );
			delete [] wstr;
			if ( FAILED(h) )
			{
				agk::Error( "Failed to create source filter for video file" );
				return;
			}

			// add sample grabber
			h = CoCreateInstance( CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER, IID_IBaseFilter, (void**)&pVideoGrabberF );
			if (FAILED(h))
			{
				agk::Warning("Failed to create grabber filter" );
				return;
			}

			h = g_pVideoGraphBuilder->AddFilter( pVideoGrabberF, L"Sample Grabber" );
			if (FAILED(h))
			{
				agk::Warning("Failed to add grabber filter" );
				return;
			}

			h = pVideoGrabberF->QueryInterface(IID_ISampleGrabber, (void**)&pVideoSampleGrabber);
			if (FAILED(h))
			{
				agk::Warning("Failed to get sample grabber" );
				return;
			}

			AM_MEDIA_TYPE amtype;
			memset( &amtype, 0, sizeof(AM_MEDIA_TYPE) );
			amtype.majortype = MEDIATYPE_Video;
			amtype.subtype = MEDIASUBTYPE_RGB24;

			pVideoCallback = new SampleHandler();

			pVideoSampleGrabber->SetMediaType(&amtype);
			//pVideoSampleGrabber->SetBufferSamples( FALSE );
			pVideoSampleGrabber->SetCallback( pVideoCallback, 1 );

			/*
			h = ConnectFilters(g_pVideoGraphBuilder, pSourceFilter, pVideoGrabberF);
			if (FAILED(h))
			{
				agk::Warning("Failed to connect source and sample grabber filters" );
				return;
			}
			*/

			IEnumPins *pEnum = 0;
			h = pSourceFilter->EnumPins( &pEnum );
			if ( FAILED(h) )
			{
				agk::Warning("Failed to enum source pins" );
				return;
			}

			IPin *pSourcePin = 0;
			while( S_OK == pEnum->Next(1, &pSourcePin, NULL) )
			{
				h = ConnectFilters(g_pVideoGraphBuilder, pSourcePin, pVideoGrabberF);
				if ( pSourcePin ) pSourcePin->Release();
				if ( SUCCEEDED(h) )
				{
					break;
				}
			}

			if ( FAILED(h) )
			{
				agk::Warning("Failed to connect source pin to sample grabber" );
				return;
			}

			// add null renderer
			h = CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pVideoNullF));
			if (FAILED(h))
			{
				agk::Warning("Failed to create null renderer" );
				return;
			}

			h = g_pVideoGraphBuilder->AddFilter(pVideoNullF, L"Null Filter");
			if (FAILED(h))
			{
				agk::Warning("Failed to add null renderer" );
				return;
			}

			h = ConnectFilters(g_pVideoGraphBuilder, pVideoGrabberF, pVideoNullF);
			if (FAILED(h))
			{
				agk::Warning("Failed to connect source and sample grabber filters" );
				return;
			}

			// look for audio pin to construct the rest automatically
			IPin *pPin = 0;
			FindUnconnectedFilterPin( pSourceFilter, &pPin );

			// if audio pin found render it
			if ( pPin )
			{
				h = g_pVideoGraphBuilder->Render( pPin );
				if (FAILED(h))
				{
					agk::Warning("Failed to render audio" );
				}
				pPin->Release();
				pPin = 0;
			}

			pSourceFilter->Release();
			pSourceFilter = 0;

			if ( g_hWnd ) g_pVideoMediaEventEx->SetNotifyWindow((OAHWND)g_hWnd, WM_VIDEO_EVENT, 0 );
			//g_pVideoMediaEventEx->CancelDefaultHandling( EC_COMPLETE );

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
				m_pVideoTexture = new cImage();
				m_pVideoTexture->CreateBlankImage( iVideoWidth, iVideoHeight, 0, 0 );
				m_pVideoTexture->m_iID = imageID;
				m_cImageList.AddItem( m_pVideoTexture, imageID );
			}

			if ( fVideoPos >= 0 ) 
			{
				long long pos = (long long) (fVideoPos * 10000000);
				h = g_pVideoMediaSeeking->SetPositions( &pos, AM_SEEKING_AbsolutePositioning, 0, AM_SEEKING_NoPositioning );
				fVideoPos = -1;
			}

			pVideoFrame = new unsigned char[ iVideoWidth*iVideoHeight*3 ];
			pVideoFrameFinal = new unsigned int[ iVideoWidth*iVideoHeight ];

			SetVideoVolume( fVideoVolume );
		}

		g_pVideoMediaControl->Run();
	}
}

//****f* Video/General/PlayVideo
// FUNCTION
//   Plays the video on screen or resumes it from pausing.<br/>
//   You can either use this command or <i>PlayVideoToImage</i>, but not both. If you pause the video you must resume 
//   it using the same command you used to start it.
// SOURCE
void agk::PlayVideo()
//****
{
	if ( sVideoPath.GetLength() == 0 ) return;
	
	if ( !g_iVideoImageID ) g_iVideoImageID = m_cImageList.GetFreeID();
	agk::PlayVideoToImage( g_iVideoImageID );
	
	if ( !g_pVideoSprite )
	{
		cImage *pImage = agk::GetImagePtr( g_iVideoImageID );
		if ( !pImage ) return;

		g_pVideoSprite = new cSprite( pImage );
		g_pVideoSprite->FixToScreen( 1 );
	}
	else 
	{
		g_pVideoSprite->SetVisible( true );
	}
}

//****f* Video/General/PauseVideo
// FUNCTION
//   Pauses the video and keeps it on screen.
// SOURCE
void agk::PauseVideo()
//****
{
	if ( g_bMFPlatExists )
	{
		if ( !g_pVideoSession ) return;
		g_pVideoSession->Pause();
		g_bVideoPlaying = FALSE;
	}
	else
	{
		if ( !g_pVideoMediaControl ) return;
		g_pVideoMediaControl->Pause();
	}
}

//****f* Video/General/StopVideo
// FUNCTION
//   Stops the video and removes it from view, using PlayVideo will start it from the beginning.
// SOURCE
void agk::StopVideo()
//****
{
	if ( g_pVideoSprite ) g_pVideoSprite->SetVisible( false );

	if ( g_bMFPlatExists )
	{
		if ( !g_pVideoSession ) return;
		g_pVideoSession->Stop();
		fVideoPos = 0;
		g_bVideoPlaying = FALSE;
	}
	else
	{
		if ( !g_pVideoMediaControl ) return;
		long long pos = 0;
		HRESULT hr = g_pVideoMediaSeeking->SetPositions( &pos, AM_SEEKING_AbsolutePositioning, 0, AM_SEEKING_NoPositioning );
		g_pVideoMediaControl->Stop();
	}
	
	m_iVideoPlayMode = 0;
}

//****f* Video/General/GetVideoPlaying
// FUNCTION
//   Returns 1 if the video is currently playing, 0 if it has finished or been stopped with 
//   StopVideo() or PauseVideo().
// SOURCE
int agk::GetVideoPlaying()
//****
{
	if ( g_bMFPlatExists )
	{
		if ( !g_pVideoSession ) return 0;
		return g_bVideoPlaying != 0 ? 1 : 0;
	}
	else
	{
		if ( !g_pVideoMediaControl ) return 0;
		OAFilterState state;
		g_pVideoMediaControl->GetState( INFINITE, &state );
		if ( state == State_Running ) return 1;
		return 0;
	}
}

//****f* Video/General/GetVideoPosition
// FUNCTION
//   Returns the number of seconds played so far.
// SOURCE
float agk::GetVideoPosition()
//****
{
	if ( g_bMFPlatExists )
	{
		if ( !g_pVideoSession ) return 0;
		if ( !g_pVideoClock ) return 0;		

		MFTIME currTime;
		g_pVideoClock->GetTime( &currTime );
		float fPos = currTime / 10000000.0f;
		return fPos;
	}
	else
	{
		if ( !g_pVideoMediaSeeking ) return 0;
		long long pos;
		g_pVideoMediaSeeking->GetCurrentPosition( &pos );
		float fPos = pos / 10000000.0f;
		return fPos;
	}
}

//****f* Video/General/GetVideoDuration
// FUNCTION
//   Returns the number of seconds the video lasts from start to finish. If this returns 0 
//   then call it in a loop with Sync() to wait for the video to finish loading the meta data.
//   If it returns less than 0 then this platform does not support this command or an error occurred.
//
//   example:
//   duration = GetVideoDuration()
//   if ( duration = 0 )
//       repeat
//           Sync()
//           duration = GetVideoDuration()
//       until duration != 0
//   endif
// SOURCE
float agk::GetVideoDuration()
//****
{
	return fVideoDuration;
}

//****f* Video/General/SetVideoVolume
// FUNCTION
//   Sets the volume of the video from 0 (muted) to 100 (full volume)
// INPUTS
//   volume -- The volume of the video in the range 0-100.
// SOURCE
void agk::SetVideoVolume( float volume )
//****
{
	fVideoVolume = volume;

	if ( g_bMFPlatExists )
	{
		if ( !g_pVideoVolume ) return;
		
		volume /= 100.0f;
		UINT32 count = 0;
		g_pVideoVolume->GetChannelCount( &count );
		for( UINT i = 0; i < count; i++ )
		{
			g_pVideoVolume->SetChannelVolume( i, volume );
		}
	}
	else
	{
		if ( !g_pVideoBasicAudio ) return;

		volume /= 100.0f;

		if ( volume > 1 ) volume = 1;
		if ( volume < 0.00001f ) volume = -10000;
		else volume = (log10(volume)*20) * 100;
		g_pVideoBasicAudio->put_Volume( (long) volume );
	}
}

//****f* Video/General/GetVideoWidth
// FUNCTION
//   Returns the width in pixels of the currently loaded video, if this returns 0 
//   then call it in a loop with Sync() to wait for the video to finish loading the meta data.
//   If it returns less than 0 then this platform does not support this command or an error occurred.
//
//   example:
//   width = GetVideoWidth()
//   if ( width = 0 )
//       repeat
//           Sync()
//           width = GetVideoWidth()
//       until width != 0
//   endif
// SOURCE
float agk::GetVideoWidth()
//****
{
	return (float) iVideoWidth;
}

//****f* Video/General/GetVideoHeight
// FUNCTION
//   Returns the height in pixels of the currently loaded video, if this returns 0  
//   then call it in a loop with Sync() to wait for the video to finish loading the meta data.
//   If it returns less than 0 then this platform does not support this command or an error occurred.
//
//   example:
//   height = GetVideoHeight()
//   if ( height = 0 )
//       repeat
//           Sync()
//           height = GetVideoHeight()
//       until height != 0
//   endif
// SOURCE
float agk::GetVideoHeight()
//****
{
	return (float) iVideoHeight;
}

//****f* Video/General/SetVideoPosition
// FUNCTION
//   Seeks the video to the given position. This is not guaranteed to be 100% accurate, the device may 
//   choose to seek to the nearest keyframe, which may be up to 10 seconds before or after your chosen 
//   time.
// INPUTS
//   seconds -- The position in the video to seek to
// SOURCE
void agk::SetVideoPosition( float seconds )
//****
{
	if ( g_bMFPlatExists )
	{
		if ( !g_pVideoSession ) fVideoPos = seconds;
		else if ( !GetVideoPlaying() ) 
		{
			if ( seconds >= fVideoDuration ) fVideoPos = 0;
			else fVideoPos = seconds;
		}
		else
		{
			if ( seconds >= fVideoDuration ) StopVideo();
			else
			{
				PROPVARIANT var; PropVariantInit(&var); var.vt = VT_EMPTY;

				long long pos = (long long) (seconds * 10000000);
				var.vt = VT_I8;
				var.hVal.QuadPart = pos;
				fVideoPos = -1;
			
				HRESULT hr = g_pVideoSession->Start( NULL, &var );
				if ( FAILED(hr) ) { agk::Error( "Failed to set video position" ); return; }
			}
		}
	}
	else
	{
		if ( !g_pVideoMediaSeeking ) 
		{
			fVideoPos = seconds;
		}
		else
		{
			long long pos = (long long) (seconds * 10000000);
			HRESULT hr = g_pVideoMediaSeeking->SetPositions( &pos, AM_SEEKING_AbsolutePositioning, 0, AM_SEEKING_NoPositioning );
			fVideoPos = -1;
		}
	}
}

// screen recording

//****f* Video/Recording/StartScreenRecording
// FUNCTION
//   Starts recording the screen and saving it to a file. Currently only supported on iOS 9.0 and above 
//   and Android 5.0 and above.
//   On iOS the filename is ignored as the device will always save the recording in the camera roll.
//   The microphone parameter should be set to 1 to record audio from the device microphone during the 
//   recording. Unfortunately Android does not support recording audio directly from the app, so using 
//   a value of 0 will produce a silent video. The audio output from the app may be audible through the 
//   microphone recording though. Recording through the microphone requires that you enable the 
//   "RecordAudio" permission when exporting your APK. On iOS a microphone value of 1 will record both
//   the app audio output and the microphone, a value of 0 will only record the app audio.<br/><br/>
//   On Android 6 and below the recording may stop at any time, for example if the app is sent to the 
//   background, or if another activity is activated such as an In App Purchase, or using the 
//   <i>FacebookLogin</i> command. On Android 7 and above the recording will pause when the app is in the
//   background and resume when the app resumes. 
//   On iOS the recording will continue after such interruptions, until <i>StopScreenRecording</i> is called.
// INPUTS
//   szFilename -- The path to save the video, should end in .mp4, can be a "raw:" file path
//   microphone -- 1 to record from the microphone, 0 to record from the device
// SOURCE
void agk::StartScreenRecording( const char *szFilename, int microphone )
//****
{
	
}

//****f* Video/Recording/StopScreenRecording
// FUNCTION
//   Stops any current screen recording. If the recording has already stopped then this has no effect.
//   On iOS this will prompt the user to edit and/or save the video, they can also choose to discard it.
//   On Android the user sees no change and the video is silently saved in the background.
// SOURCE
void agk::StopScreenRecording()
//****
{

}

//****f* Video/Recording/IsScreenRecording
// FUNCTION
//   Returns 1 if the screen recording is currently active, 0 if it has stopped. On Android it could have stopped
//   because the app was sent to the background, or another activity such as an In App Purchase was activated.
// SOURCE
int agk::IsScreenRecording()
//****
{
	return 0;
}

// Smart Watch

//****f* Extras/SmartWatch/ActivateSmartWatch
// FUNCTION
//   Attempts to connect to the smart watch connected to the current device. It will attempt to maintain that
//   connection for the life of the app, for example if the watch is unpaired and then repaired then your app
//   will reconnect to the watch automatically. So you only need to call this command once at the beginning 
//   of your app.
//   </br></br>
//   Currently only works on iOS. To create a watch app you will need to use the Tier 2 project interpreter_ios
//   and add a watch app to it.
// INPUTS
//   szReserved -- Reserved for future use, must be an empty string
// SOURCE
void agk::ActivateSmartWatch( const char *szReserved )
//****
{
    
}

//****f* Extras/SmartWatch/GetSmartWatchState
// FUNCTION
//   Returns the current state of the smart watch connection: 0=initial state, <i>ActivateSmartWatch</i> has 
//   not been called. 1=connecting, will transition to a more detailed state in a moment. 2=connection 
//   successful, you may send and receive data. -1=watch commands not supported on this device. -2=connection 
//   failed or no watch found. -3=watch found but it is not currently paired with the device, -4=watch found 
//   but our watch app is not currently installed.
//   </br></br>
//   Currently only works on iOS.
// SOURCE
int agk::GetSmartWatchState()
//****
{
    return -1;
}

//****f* Extras/SmartWatch/SendSmartWatchData
// FUNCTION
//   Send data to the smart watch in the background, if watch app is not currently open the message will be 
//   queued and received by the watch app when it is next opened. The message must be in the form of a JSON
//   string, for example "{"label":"my message"}". You can also use a type with the .toJSON() function to
//   create a JSON string. The watch app will receive it as an NSDictionary with the labels as keys.
//   </br></br>
//   Currently only works on iOS.
// INPUTS
//   szJson -- A JSON string containing the message to send to the watch app.
// SOURCE
void agk::SendSmartWatchData( const char* szJson )
//****
{
    
}

//****f* Extras/SmartWatch/ReceiveSmartWatchData
// FUNCTION
//   Receives a queued message from the watch app. This will be in the form of a JSON string. If you are 
//   calling this command from Tier 2 you must delete the returned string when you are done with it by 
//   calling agk::DeleteString.
//   </br></br>
//   Currently only works on iOS.
// SOURCE
char* agk::ReceiveSmartWatchData()
//****
{
    char* str = new char[1]; *str = 0;
    return str;
}

// Text to speech

//****f* Sound/TextToSpeech/TextToSpeechSetup
// FUNCTION
//   Initialises the Text To Speech engine on the device so that it can produce audio. Note that this may
//   not be immediate, you should check <i>GetTextToSpeechReady</i> to see if it has finished setting up.
//   Calling other text to speech commands when it is not ready will be ignored.
//   </br></br>
//   Currently only works on iOS and Android.
// SOURCE
void agk::TextToSpeechSetup()
//****
{
	
}

//****f* Sound/TextToSpeech/GetTextToSpeechReady
// FUNCTION
//   Returns 1 when the text to speech engine is ready to start converting text to speech. Calling other text
//   to speech commands when it is not ready will be ignored.
//   </br></br>
//   Currently only works on iOS and Android.
// SOURCE
int agk::GetTextToSpeechReady()
//****
{
	return 0;
}

//****f* Sound/TextToSpeech/Speak
// FUNCTION
//   Queues the given text for speaking. If some text is already being spoken then the given text will be spoken
//   after the current speech has finished. This command can be called multiple times in quick succession to 
//   queue up a lot of text. You can call <i>IsSpeaking</i> to check when the queue is empty and all text has been 
//   spoken, or cancelled. Note that there may be a delay on the first call to this command when an unfamiliar
//   language has been set with <i>SetSpeechLanguage</i>. For example using French on a device setup for English 
//   will attempt to download the French language engine before starting to speak the text.
//   </br></br>
//   If the app is sent to the background then speech will stop and all text will be removed from the queue.
//   </br></br>
//   You can specify an optional delay in milliseconds to make the device wait before starting to say the text.
//   </br></br>
//   Currently only works on iOS and Android.
// INPUTS
//   text -- The text to speak
// SOURCE
void agk::Speak( const char *text )
//****
{
	
}

//****f* Sound/TextToSpeech/Speak
// FUNCTION
//   Queues the given text for speaking. If some text is already being spoken then the given text will be spoken
//   after the current speech has finished. This command can be called multiple times in quick succession to 
//   queue up a lot of text. You can call <i>IsSpeaking</i> to check when the queue is empty and all text has been 
//   spoken, or cancelled. Note that there may be a delay on the first call to this command when an unfamiliar
//   language has been set with <i>SetSpeechLanguage</i>. For example using French on a device setup for English 
//   will attempt to download the French language engine before starting to speak the text.
//   </br></br>
//   If the app is sent to the background then speech will stop and all text will be removed from the queue.
//   </br></br>
//   You can specify an optional delay in milliseconds to make the device wait before starting to say the text.
//   </br></br>
//   Currently only works on iOS and Android.
// INPUTS
//   text -- The text to speak
//   delay -- The amount of time in milliseconds to wait before starting the speech output
// SOURCE
void agk::Speak( const char *text, int delay )
//****
{
	
}

//****f* Sound/TextToSpeech/SetSpeechRate
// FUNCTION
//   Sets the speaking rate, where 1.0 is normal speed, 2.0 is double speed, 0.5 is half speed, and so on.
//   </br></br>
//   Currently only works on iOS and Android.
// INPUTS
//   rate -- The rate to speak text, default is 1.0
// SOURCE
void agk::SetSpeechRate( float rate )
//****
{

}

//****f* Sound/TextToSpeech/GetSpeechNumVoices
// FUNCTION
//   Returns the number of voices that can be selected with the <i>SetSpeechLanguage</i> command. You can use
//   the <i>GetSpeechVoiceName</i> and <i>GetSpeechVoiceLanguage</i> commands to get details about the different
//   voices.
//   </br></br>
//   Currently only works on iOS and Android.
// SOURCE
int agk::GetSpeechNumVoices()
//****
{
    return 0;
}

//****f* Sound/TextToSpeech/GetSpeechVoiceLanguage
// FUNCTION
//   Returns the language string of the given voice. This can be used directly with the <i>SetSpeechLanguage</i> 
//   command to use this voice for any future speech.
//   </br></br>
//   Currently only works on iOS and Android.
// INPUTS
//   index -- The index of the voice to check, starts at 0 for the first voice
// SOURCE
char* agk::GetSpeechVoiceLanguage( int index )
//****
{
    char *str = new char[1]; *str = 0;
    return str;
}

//****f* Sound/TextToSpeech/GetSpeechVoiceName
// FUNCTION
//   Returns the name of the given voice. On iOS this is a human readable first name, but does not give any details 
//   about the accent or language of that voice. On Android the name gives the language and accent information but 
//   is not as easy to read.
//   </br></br>
//   Currently only works on iOS and Android.
// INPUTS
//   index -- The index of the voice to check, starts at 0 for the first voice
// SOURCE
char* agk::GetSpeechVoiceName( int index )
//****
{
    char *str = new char[1]; *str = 0;
    return str;
}

//****f* Sound/TextToSpeech/GetSpeechVoiceID
// FUNCTION
//   Returns the ID of the given voice. The ID can be used with <i>SetSpeechLanguageByID</i> to select a 
//   specific voice.
// INPUTS
//   index -- The index of the voice to check, starts at 0 for the first voice
// SOURCE
char* agk::GetSpeechVoiceID( int index )
//****
{
    char *str = new char[1]; *str = 0;
    return str;
}

//****f* Sound/TextToSpeech/SetSpeechLanguage
// FUNCTION
//   Sets the language to use when speaking text, for example "en_GB" for British English, "en_US" for
//   American English, "fr_FR" for French, and so on. If the language engine is not currently on the device
//   then the device will attempt to download it when <i>Speak</i> is next called, this may introduce a 
//   delay before that text is spoken. If the download fails, or no internet connection is available then
//   the default language engine for the current device will be used.
//   </br></br>
//   Currently only works on iOS and Android.
// INPUTS
//   lang -- The language to use for speaking text, default is the device's current language.
// SOURCE
void agk::SetSpeechLanguage( const char* lang )
//****
{

}

//****f* Sound/TextToSpeech/SetSpeechLanguageByID
// FUNCTION
//   Sets the language to use when speaking text by voice ID. The ID can be found with <i>GetSpeechVoiceID</i>, 
//   and is necessary when multiple voices have the same language but with different accents.
// INPUTS
//   sID -- The language to use for speaking text, default is the device's current language.
// SOURCE
void agk::SetSpeechLanguageByID( const char* sID )
//****
{

}

//****f* Sound/TextToSpeech/IsSpeaking
// FUNCTION
//   Returns 1 if <i>Speak</i> has been called and the text has not yet finished. If multiple items of text
//   have been queued up then this command will continue to return 1 until all of them have been spoken, or
//   <i>StopSpeaking</i> is called.
//   </br></br>
//   Currently only works on iOS and Android.
// SOURCE
int agk::IsSpeaking()
//****
{
	return 0;
}

//****f* Sound/TextToSpeech/StopSpeaking
// FUNCTION
//   Immediately stops any text that is being spoken and removes all items from the queue.
//   </br></br>
//   Currently only works on iOS and Android.
// SOURCE
void agk::StopSpeaking()
//****
{
	
}

// String

int uString::ToInt() const
{
	return atoi(m_pData);
}

float uString::ToFloat() const
{
	return (float)atof(m_pData);
}

// Error handling

void agk::PlatformReportError( const uString &sMsg )
{
	//simple messagebox error handler for now
	//MessageBox( NULL, sMsg.GetStr(), "Error", 0 );

	OutputDebugString( "    > " );
	OutputDebugString( sMsg.GetStr() );
	OutputDebugString( "\n" );

	/*
	static HANDLE hDebugConsoleOut = NULL;
	if ( !hDebugConsoleOut ) 
	{
		AllocConsole ( );
		hDebugConsoleOut = GetStdHandle ( STD_OUTPUT_HANDLE );
	}
	
	DWORD dwWritten = 0;
	WriteConsole ( hDebugConsoleOut, "> ", 2, &dwWritten, NULL );
	WriteConsole ( hDebugConsoleOut, sMsg.GetStr(), (DWORD) sMsg.GetLength(), &dwWritten, NULL );
	WriteConsole ( hDebugConsoleOut, "\n", 1, &dwWritten, NULL );
	*/
}

void agk::PlatformMessage( const char* msg )
{
	if ( !g_bGLInit || !g_hWnd )
	{
		MessageBox( g_hWnd, msg, "Message", MB_SETFOREGROUND | MB_TOPMOST );
	}
	else
	{
		LONG lStyle = GetWindowLong( g_hWnd, GWL_STYLE );
		if ( lStyle & WS_POPUP )
		{
			//::SetWindowLong( g_hWnd, GWL_STYLE, lStyle & ~WS_POPUP );
			//::SetWindowPos( g_hWnd, HWND_BOTTOM, 0,0, 0,0, SWP_NOMOVE | SWP_NOSIZE );
			::ShowWindow( g_hWnd, SW_MINIMIZE );
			MessageBox( g_hWnd, msg, "Message", MB_SETFOREGROUND | MB_TOPMOST );
			//::SetWindowLong( g_hWnd, GWL_STYLE, lStyle );
			//::SetWindowPos( g_hWnd, HWND_NOTOPMOST, 0,0, 0,0, SWP_NOMOVE | SWP_NOSIZE );
			::ShowWindow( g_hWnd, SW_SHOWNORMAL );
		}
		else
		{
			MessageBox( g_hWnd, msg, "Message", MB_SETFOREGROUND | MB_TOPMOST );
		}
	}
}

// Thread functions
void AGKThread::PlatformInit( )
{
	m_pStop = CreateEvent( NULL, FALSE, FALSE, NULL );
	pThread = 0;
}

void AGKThread::PlatformStart( )
{
	ResetEvent( m_pStop );
	if ( pThread ) CloseHandle( pThread );
	pThread = (HANDLE)_beginthreadex( NULL, 0, EntryPoint, (void*) this, 0, &iThreadID );
}

void AGKThread::PlatformStop( )
{
	SetEvent( m_pStop );
}

void AGKThread::PlatformTerminate( )
{
	if ( pThread )
	{
		if ( m_bRunning ) 
		{
#ifdef _AGK_ERROR_CHECK
			agk::Warning("Forcing a thread to terminate, this may cause a crash...");
#endif
			//TerminateThread( pThread, 0 );
			//m_bRunning = false;
		}
		//CloseHandle( pThread );
	}
}

void AGKThread::PlatformCleanUp( )
{
	if ( m_pStop ) CloseHandle( m_pStop );
	if ( pThread ) CloseHandle( pThread );
	pThread = 0;
}

void AGKThread::PlatformJoin( )
{
	WaitForSingleObject( pThread, INFINITE );
}

void AGKThread::PlatformSleepSafe( UINT milliseconds )
{
	WaitForSingleObject( m_pStop, milliseconds );
}

// file system commands

//****f* File/Access/SetRawWritePath
// FUNCTION
//   Sets the folder where files created by AGK will be stored. This has no effect on iOS since it cannot
//   write outside its app folder. You can set this to any absolute path on the device, however some 
//   locations, such as the Program Files folder on Windows, will require administrator privileges
//   to write to them. Some locations will be protected by the operating system, SetRawWritePath will let 
//   you set the path to these locations but any attempt to write to them will fail. You can use 
//   <i>GetDocumentsPath</i> to get the system defined documents folder for the current user, or sdcard 
//   folder on Android, which is guaranteed to allow write access.<br/><br/>
//   This command is now deprecated. The preferred method of accessing files outside of the usual write
//   folder is to use the <i>OpenRawFolder</i> commands and "raw:" file paths.
// INPUTS
//   str -- The path to use as the write folder
// SOURCE
void agk::SetRawWritePath( const char* str )
//****
{
	if ( !str || !*str ) return;
	agk::Warning( "SetRawWritePath is deprecated, the preferred method is now OpenRawFolder and \"raw:\" file paths" );

	strcpy( szWriteDir, str );
	UINT length = (UINT) strlen( str );
	if ( str[ length-1 ] != '/' && str[ length-1 ] != '\\' ) strcat( szWriteDir, "/" );

	char *str2 = szWriteDir;
	while ( *str2 )
	{
		if ( *str2 == '\\' ) *str2 = '/';
		str2++;
	}

	//ParseCurrentDirectory();
	m_bUpdateFileLists = true;
}

//****f* File/Access/GetWritePath
// FUNCTION
//   Returns the directory that new files will be written to on the current platform that the app is running on.
// SOURCE
char* agk::GetWritePath()
//****
{
	// allow write directory to be determined
	char *szStr = new char[ strlen(szWriteDir)+1 ];
	strcpy( szStr, szWriteDir );
	return szStr;
}

//****f* File/Access/GetReadPath
// FUNCTION
//   Returns the directory that contains the application executable on the current platform that the app is running on.
//   Note that on Android this path is inaccessible in normal use and will return an empty string.
// SOURCE
char* agk::GetReadPath()
//****
{
	char *szStr = new char[ strlen(szRootDir)+1 ];
	strcpy( szStr, szRootDir );
	return szStr;
}

//****f* File/Access/GetDocumentsPath
// FUNCTION
//   Returns the directory that contains the current users documents. On Windows this will be something like 
//   "C:\Users\Me\My Documents", on iOS this will just be the app write directory.
//   By default all apps write to the system specified app settings folder, for example "C:\Users\Me\AppData" 
//   on Windows, "/home/user/.config" on Linux, and a protected app data folder on Android. The documents path
//   is guaranteed to be visible to the user, for example on Android it will be the sdcard folder. Only iOS 
//   can't provide a publicly accessible location to write to.
// SOURCE
char* agk::GetDocumentsPath()
//****
{
	wchar_t szTemp[ MAX_PATH ];
	szTemp[ 0 ] = 0;
	if ( !SHGetSpecialFolderPathW( NULL, szTemp, CSIDL_MYDOCUMENTS, TRUE ) )
	{
		// if no my documents folder use write location
		char *szStr = new char[ strlen(szWriteDir)+1 ];
		strcpy( szStr, szWriteDir );
		return szStr;
	}

	int size = WideCharToMultiByte( CP_UTF8, 0, szTemp, -1, 0, 0, 0, 0 );
	char *szStr = new char[ size ];
	WideCharToMultiByte( CP_UTF8, 0, szTemp, -1, szStr, size, 0, 0 );
	return szStr;
}

bool agk::PlatformChooseFile( uString &out, const char *ext, int fullPath )
{
	char *ext2 = new char[ 100 ];
	if ( !ext || !*ext ) 
	{
		strcpy( ext2, "All Files" );
		ext2[ 9 ] = 0;
		ext2[ 10 ] = '*';
		ext2[ 11 ] = '.';
		ext2[ 12 ] = '*';
		ext2[ 13 ] = 0;
		ext2[ 14 ] = 0;
	}
	else
	{
		strcpy( ext2, "File" );
		ext2[ 4 ] = 0;
		char* temp = ext2 + 5;
		int length = (int) strlen( ext );
		if ( length > 90 ) 
		{
			strncpy( temp, ext, 90 );
			temp += 90;
		}
		else 
		{
			strcpy( temp, ext );
			temp += length;
		}
		
		temp[ 0 ] = 0;
		temp[ 1 ] = 0;
	}

	char szFile[ MAX_PATH ];
	szFile[0] = '\0';
	
	OPENFILENAME ofn;
	memset( &ofn, 0, sizeof(OPENFILENAME) );

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = g_hWnd;
	ofn.lpstrFilter = ext2;
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_NONETWORKBUTTON;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = MAX_PATH;

	LONG lStyle = 0;
	if ( g_hWnd ) lStyle = GetWindowLong( g_hWnd, GWL_STYLE );
	if ( lStyle & WS_POPUP ) ::ShowWindow( g_hWnd, SW_MINIMIZE );
	
	if ( GetOpenFileName( &ofn ) == 0 ) 
	{
		DWORD error = CommDlgExtendedError();
		if ( error > 0 ) 
		{
			uString err; err.Format( "Open File Dialog Error: %d", error );
			agk::Warning( err );
		}
		delete [] ext2;
		if ( lStyle & WS_POPUP ) ::ShowWindow( g_hWnd, SW_NORMAL );
		return false;
	}

	if ( lStyle & WS_POPUP ) ::ShowWindow( g_hWnd, SW_NORMAL );

	if ( 1 == fullPath )
	{
		out.SetStr( ofn.lpstrFile );
	}
	else
	{
		uString sFile( ofn.lpstrFile );
		sFile.Replace( '\\', '/' );
		if ( sFile.Find( '/' ) >= 0 )
		{
			uString temp;
			sFile.SubString( temp, sFile.RevFind( '/' )+1 );
			sFile.SetStr( temp );
		}

		uString sPath( szWriteDir );
		sPath += m_sCurrentDir;
		sPath += sFile;

		CopyFile( ofn.lpstrFile, sPath.GetStr(), FALSE );

		out.SetStr( sFile );
	}

	delete [] ext2;
	return true;
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

int agk::PlatformCreateRawPath( const char *path )
{
	if ( !path || !*path ) return 0;
	if ( !IsAbsolutePath( path ) || path[0] == '/' ) 
	{
		uString err; err.Format( "Invalid path \"%s\", must be absolute", path );
		agk::Error( err );
		return 0;
	}

	char *newPath = new char[ strlen(path) + 1 ];
	strcpy( newPath, path );
	char *origPath = newPath;

	if ( newPath[0] == '\\' && newPath[1] == '\\' )
	{
		const char *ptr = newPath+2;
		while( *ptr && *ptr != '/' && *ptr != '\\' ) ptr++;
		if ( !*ptr ) 
		{
			// server name only, assume it exists
			delete [] origPath;
			return 1;
		}

		UINT length = (UINT)(ptr-newPath);
		if ( length == 0 )
		{
			uString err; err.Format( "Invalid path \"%s\", two back slashes must be followed by a server name", origPath );
			delete [] origPath;
			agk::Error( err );
			return 0;
		}

		// can't check if a server exists

		newPath += length+1;
	}
	else
	{
		newPath += 3;
	}

	char *ptr = newPath;
	while( *ptr ) 
	{
		if ( *ptr == '\\' ) *ptr = '/';
		ptr++;
	}

	char *szPrev = newPath;
	char *szSlash = 0;
	while( (szSlash = strchr( szPrev, '/' )) )
	{
		UINT length = (UINT)(szSlash-szPrev);
		if ( length == 0 )
		{
			uString err; err.Format( "Invalid path \"%s\", folder names must have at least one character", origPath );
			delete [] origPath;
			agk::Error( err );
			return 0;
		}

		*szSlash = 0;

		if ( !GetPathExistsUTF8( origPath ) && !CreateDirectoryUTF8( origPath, NULL ) )
		{
			uString err; err.Format( "Failed to create path \"%s\", the app may not have permission to create it", origPath );
			delete [] origPath;
			agk::Error( err );
			return 0;
		}

		*szSlash = '/';
		
		szPrev = szSlash+1;
	}

	delete [] origPath;
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

	/*
	if ( !SetCurrentDirectoryUTF8( szPath ) )
	{
		uString err; err.Format( "Failed to open folder \"%s\", it might not exist", szPath );
		agk::Error( err );
		return 0;
	}
	*/

	UINT length = (UINT) strlen(szPath);
	int size = MultiByteToWideChar( CP_UTF8, 0, szPath, -1, 0, 0 );
	wchar_t *wzPath = new wchar_t[ size+5 ];
	MultiByteToWideChar( CP_UTF8, 0, szPath, -1, wzPath, size );
	if ( szPath[length-1] == '/' || szPath[length-1] == '\\' ) wcscat( wzPath, L"*.*" );
	else wcscat( wzPath, L"/*.*" );
	
	WIN32_FIND_DATAW FindFileData;
	HANDLE hFind = FindFirstFileW( wzPath, &FindFileData );
	if (hFind == INVALID_HANDLE_VALUE)
	{
		uString err; err.Format( "Failed to find files in folder \"%s\", the app may not have permission to access it", szPath );
		agk::Error( err );
		return 0;
	}

	do
	{
		if ( wcscmp( FindFileData.cFileName, L"." ) != 0 && wcscmp( FindFileData.cFileName, L".." ) != 0 )
		{
			if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ) m_iNumFolders++;
			else m_iNumFiles++;
		}
	} while ( FindNextFileW( hFind, &FindFileData ) );
	FindClose( hFind );

	m_pFiles = new uString[ m_iNumFiles ];
	m_pFolders = new uString[ m_iNumFolders ];

	int fileCount = 0;
	int folderCount = 0;
	char szTemp[ MAX_PATH ];

	hFind = FindFirstFileW( wzPath, &FindFileData );
	do
	{
		if ( wcscmp( FindFileData.cFileName, L"." ) != 0 && wcscmp( FindFileData.cFileName, L".." ) != 0 )
		{
			if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			{
				// directory
				WideCharToMultiByte( CP_UTF8, 0, FindFileData.cFileName, -1, szTemp, MAX_PATH, 0, 0 );
				m_pFolders[ folderCount++ ].SetStr( szTemp );
			}
			else
			{
				// file
				WideCharToMultiByte( CP_UTF8, 0, FindFileData.cFileName, -1, szTemp, MAX_PATH, 0, 0 );
				m_pFiles[ fileCount++ ].SetStr( szTemp );
			}
		}
	} while ( FindNextFileW( hFind, &FindFileData ) );
	FindClose( hFind );

	return 1;
}

bool cFile::ExistsWrite( const char *szFilename )
{
	if ( !szFilename || !*szFilename ) return false;
	if ( strncmp(szFilename, "raw:", 4) == 0 ) return false;

	if ( strchr( szFilename, ':' ) ) return false;
	if ( strstr(szFilename, "..\\") || strstr(szFilename, "../") ) return false;

	UINT length = (UINT)strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' ) return false;

	uString sPath( szFilename, MAX_PATH );
	agk::PlatformGetFullPathWrite( sPath );

	// doesn't work on Windows XP due to Visual Studio bug
	//struct stat buf;
	//if ( stat( sPath.GetStr(), &buf ) != 0 ) return false;
	
	FILE *pFile = AGKfopen( sPath.GetStr(), "rb" );
	if ( !pFile ) return false;
	fclose( pFile );

	return true;
}

bool cFile::ExistsRead( const char *szFilename, int *mode )
{
	if ( !szFilename || !*szFilename ) return false;
	if ( strncmp(szFilename, "raw:", 4) == 0 ) return false;

	if ( strchr( szFilename, ':' ) ) return false;
	if ( strstr(szFilename, "..\\") || strstr(szFilename, "../") ) return false;

	UINT length = (UINT) strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' ) return false;

	uString sPath( szFilename, MAX_PATH );
	agk::PlatformGetFullPathRead( sPath );
	
	// doesn't work on Windows XP due to Visual Studio bug
	//struct stat buf;
	//if ( stat( sPath.GetStr(), &buf ) != 0 ) return false;

	FILE *pFile = AGKfopen( sPath.GetStr(), "rb" );
	if ( !pFile ) return false;
	fclose( pFile );

	return true;
}

bool cFile::ExistsRaw( const char *szFilename )
{
	if ( !szFilename || !*szFilename ) return false;
	UINT length = (UINT) strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' ) return false;

	if ( strncmp(szFilename, "raw:", 4) != 0 ) return false;
	if ( !agk::IsAbsolutePath( szFilename ) ) return false;
	
	// absolute path to anywhere allowed
	FILE *pFile = AGKfopen( szFilename+4, "rb" );
	if ( !pFile ) return false;
	fclose( pFile );
	return true;
}

bool cFile::Exists( const char *szFilename )
{
	if ( !szFilename || !*szFilename ) return false;

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

	UINT length = (UINT) strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' ) return false;

	uString sPath( szFilename );
	if ( !agk::GetRealPath( sPath ) ) return false;

	// doesn't work in Windows XP due to Visual Studio bug
	//struct stat fileInfo;
	//int result = stat( sPath.GetStr(), &fileInfo );

	WIN32_FILE_ATTRIBUTE_DATA fileInfo;
	int result = GetFileAttributesEx( sPath.GetStr(), GetFileExInfoStandard, (void*) &fileInfo );
	if ( result == 0 ) return false;
	
	long long modified = fileInfo.ftLastWriteTime.dwHighDateTime;
	modified <<= 32;
	modified |= fileInfo.ftLastWriteTime.dwLowDateTime;
	modified = modified / 10000000 - 11644473600LL;

	time = (int) modified;
	return true;
}

void cFile::SetModified( const char *szFilename, int time )
{
	if ( !szFilename || !*szFilename ) return;

	UINT length = (UINT) strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' ) return;

	uString sPath( szFilename );
	if ( cFile::ExistsRaw( szFilename ) ) sPath.SetStr( szFilename+4 );
	else if ( cFile::ExistsWrite( szFilename ) ) agk::PlatformGetFullPathWrite(sPath);
	else return;

	HANDLE hFile = CreateFile( sPath.GetStr(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( hFile == INVALID_HANDLE_VALUE ) return;

	long long modified64 = time;
	modified64 += 11644473600LL;
	modified64 *= 10000000;

	FILETIME modified;
	modified.dwHighDateTime = (int) (modified64 >> 32);
	modified.dwLowDateTime = (int) modified64;
	SetFileTime( hFile, NULL, NULL, &modified );

	CloseHandle( hFile );
	
	/*
	struct utimbuf fileInfo;
	fileInfo.actime = time;
	fileInfo.modtime = time;
	utime( sPath.GetStr(), &fileInfo );
	*/
}

UINT cFile::GetFileSize( const char *szFilename )
{
	cFile pFile;
	if ( !pFile.OpenToRead( szFilename ) ) return 0;
	UINT size = pFile.GetSize();
	pFile.Close();
	
	return size;
}

void cFile::DeleteFile( const char *szFilename )
{
	if ( !szFilename || !*szFilename ) return;

	UINT length = (UINT) strlen(szFilename);
	if ( szFilename[length-1] == '/' || szFilename[length-1] == '\\' )
	{
		agk::Error( "Invalid path for DeleteFile file, must not end in a forward or backward slash" );
		return;
	}

	uString sPath( szFilename );
	if ( cFile::ExistsRaw( szFilename ) ) sPath.SetStr( szFilename+4 );
	else if ( cFile::ExistsWrite( szFilename ) ) agk::PlatformGetFullPathWrite(sPath);
	else return;

	int size = MultiByteToWideChar( CP_UTF8, 0, sPath.GetStr(), -1, 0, 0 );
	wchar_t *wzPath = new wchar_t[ size ];
	MultiByteToWideChar( CP_UTF8, 0, sPath.GetStr(), -1, wzPath, size );

	::DeleteFileW( wzPath );

	agk::m_bUpdateFileLists = true;
}

bool cFile::OpenToWrite( const char *szFilename, bool append )
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
		uString err = "Failed to open file for writing ";
		err += sPath.GetStr();
		agk::Error( err );
		return false;
	}

	// refresh any stored directory details for the new file
	if ( 0 == raw ) cFileEntry::AddNewFile( sPath );
	agk::m_bUpdateFileLists = true;

	return true;
}

bool cFile::OpenToRead( const char *szFilename )
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
	
	pFile = AGKfopen( sPath, "rb" );
	if ( !pFile )
	{
		uString err = "Failed to open file for reading ";
		err += sPath.GetStr();
		agk::Error( err );
		return false;
	}

	return true;
}

void cFile::Close()
{
	if ( pFile ) fclose( pFile );
	pFile = UNDEF;
}

UINT cFile::GetSize()
{
	if ( !pFile ) return 0;
	fpos_t pos;
	fgetpos( pFile, &pos );
	fseek( pFile, 0, SEEK_END );
	UINT size = ftell( pFile );
	fsetpos( pFile, &pos );
	return size;
}

UINT cFile::GetPos()
{
	if ( !pFile ) return 0;
	return ftell( pFile );
}

void cFile::Seek( UINT pos )
{
	if ( !pFile ) return;
	fseek( pFile, pos, SEEK_SET );
}

void cFile::Flush()
{
	if ( !pFile ) return;
	fflush( pFile );
}

void cFile::Rewind()
{
	if ( !pFile ) return;
	rewind( pFile );
}

bool cFile::IsEOF()
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

void cFile::WriteInteger( int i )
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

void cFile::WriteFloat( float f )
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

void cFile::WriteString( const char *str )
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
	UINT length = (UINT) strlen( str );
	fwrite( str, 1, length + 1, pFile );
}

// writes the string length first to allow faster reading
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
	UINT length = (UINT) strlen( str );
	UINT l = agk::PlatformLittleEndian( length );
	fwrite( &l, 4, 1, pFile );
	fwrite( str, 1, length, pFile );
}

void cFile::WriteData( const char *str, UINT bytes )
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

void cFile::WriteLine( const char *str )
{
	if ( !pFile ) return;
	if ( mode != 1 )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Cannot write to file opened for reading" );
#endif
		return;
	}
	UINT length = (UINT) strlen( str );
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

int cFile::ReadInteger( )
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
	return agk::PlatformLocalEndian( i );
}

float cFile::ReadFloat( )
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

int cFile::ReadString( uString &str )
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

// uses the string length to read faster
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

int cFile::ReadLine( uString &str )
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
		UINT written = (UINT) fread( buffer+pos, 1, diff, pFile ); // if written is less than diff and eof not reached, then this could cause a problem
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

	str.Trim( "\n\r" );
	return str.GetLength();
}
*/

int cFile::ReadData( char *str, UINT length )
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

	return (int) fread( str, 1, length, pFile );
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

	char szTemp[ MAX_PATH ];

	if ( SetCurrentDirectoryUTF8( sPath.GetStr() ) ) 
	{	
		WIN32_FIND_DATAW FindFileData;
		HANDLE hFind = FindFirstFileW( L"*.*", &FindFileData );
		if (hFind == INVALID_HANDLE_VALUE) return;

		do
		{
			if ( wcscmp( FindFileData.cFileName, L"." ) != 0 && wcscmp( FindFileData.cFileName, L".." ) != 0 )
			{
				if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					// directory
					cDirectoryItem *pNewItem = new cDirectoryItem();
					pNewItem->iFlags |= AGK_FILE_TYPE_READ;
					WideCharToMultiByte( CP_UTF8, 0, FindFileData.cFileName, -1, szTemp, MAX_PATH, 0, 0 );
					pNewItem->m_sItem.SetStr( szTemp );
					pNewItem->m_pNext = m_pCurrentDirectories;
					m_pCurrentDirectories = pNewItem;
				}
				else
				{
					// file
					cDirectoryItem *pNewItem = new cDirectoryItem();
					pNewItem->iFlags |= AGK_FILE_TYPE_READ;
					WideCharToMultiByte( CP_UTF8, 0, FindFileData.cFileName, -1, szTemp, MAX_PATH, 0, 0 );
					pNewItem->m_sItem.SetStr( szTemp );
					pNewItem->m_pNext = m_pCurrentFiles;
					m_pCurrentFiles = pNewItem;
				}
			}
		} while ( FindNextFileW( hFind, &FindFileData ) );

		FindClose( hFind );
	}

	// check write directory for files and directories (non-recursive)
	sPath.SetStr( szWriteDir );
	sPath.Append( m_sCurrentDir );
	cFileEntry::ConvertToReal( sPath );

	if ( SetCurrentDirectoryUTF8( sPath.GetStr() ) ) 
	{	
		WIN32_FIND_DATAW FindFileData;
		HANDLE hFind = FindFirstFileW( L"*.*", &FindFileData );
		if (hFind == INVALID_HANDLE_VALUE) return;

		do
		{
			if ( wcscmp( FindFileData.cFileName, L"." ) != 0 && wcscmp( FindFileData.cFileName, L".." ) != 0 )
			{
				if ( (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
				{
					// directory
					bool bExists = false;
					cDirectoryItem *pItem = m_pCurrentDirectories;
					WideCharToMultiByte( CP_UTF8, 0, FindFileData.cFileName, -1, szTemp, MAX_PATH, 0, 0 );
					while ( pItem )
					{
						if ( pItem->m_sItem.CompareTo( szTemp ) == 0 )
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
						pNewItem->m_sItem.SetStr( szTemp );
						pNewItem->m_pNext = m_pCurrentDirectories;
						m_pCurrentDirectories = pNewItem;
					}
				}
				else
				{
					// file
					bool bExists = false;
					cDirectoryItem *pItem = m_pCurrentFiles;
					WideCharToMultiByte( CP_UTF8, 0, FindFileData.cFileName, -1, szTemp, MAX_PATH, 0, 0 );
					while ( pItem )
					{
						if ( pItem->m_sItem.CompareTo( szTemp ) == 0 )
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
						pNewItem->m_sItem.SetStr( szTemp );
						pNewItem->m_pNext = m_pCurrentFiles;
						m_pCurrentFiles = pNewItem;
					}
				}
			}
		} while ( FindNextFileW( hFind, &FindFileData ) );

		FindClose( hFind );
	}

	SetCurrentDirectoryUTF8( szRootDir );
}

//****f* File/Directory/SetCurrentDir
// FUNCTION
//   This command is deprecated and has been replaced with <i>SetFolder</i>.
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
		if ( pos >= 0 && pos < ((int)m_sCurrentDir.GetLength())-1 )
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

	// set to a relative path only, no full disk access
	if ( strchr( szPath, ':' ) )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Invalid path for SetFolder, must be a relative path" );
#endif
		return 0;
	}

	if ( strstr(szPath, "..") )
	{
#ifdef _AGK_ERROR_CHECK
		agk::Error( "Invalid path for SetFolder, must not traverse backwards up the directory tree using ../" );
#endif
		return 0;
	}

	uString sPath( szPath );
	
	if ( szPath[0] == '\\' || szPath[0] == '/' )
	{
		m_sCurrentDir.ClearTemp();
		sPath.SetStr( szPath+1 );
	}

	sPath.Replace( '\\', '/' );

	// add a final slash
	UINT length = (UINT) strlen( szPath );
	//if ( szPath[ length-1 ] != '/' && szPath[ length-1 ] != '\\' ) sPath.Append( '/' ); // 280811 - needs to be backslash for debugging
	if ( szPath[ length-1 ] != '/' && szPath[ length-1 ] != '\\' ) sPath.AppendAscii( '/' );

	m_sCurrentDir.Append( sPath );
	m_sCurrentDir.Replace( '\\', '/' );

	//ParseCurrentDirectory();
	m_bUpdateFileLists = true;

	return 1;
}

//****f* File/Directory/MakeFolder
// FUNCTION
//   Creates a folder at the current path. By default this is determined by the command <i>SetFolder</i>.
//   You can also use a filename beginning with "raw:" to create a folder anywhere on the file system, for example 
//   MakeFolder("raw:C:\Temp\MyFolder")
// INPUTS
//   szName -- The name of the folder create.
// SOURCE
int agk::MakeFolder( const char* szName )
//****
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

//****f* File/Directory/DeleteFolder
// FUNCTION
//   Deletes a folder at the current path. By default this is determined by the command <i>SetFolder</i>.
//   You can also use a filename beginning with "raw:" to delete a folder anywhere on the file system, for example 
//   DeleteFolder("raw:C:\Temp\MyFolder"). 
//   The folder must be empty for this command to succeed.
// INPUTS
//   szName -- The name of the folder create.
// SOURCE
void agk::DeleteFolder( const char* szName )
//****
{
	if ( !szName || strlen( szName ) == 0 )
	{
		return;
	}

	if ( strncmp(szName, "raw:", 4) == 0 )
	{
		RemoveDirectoryUTF8( szName+4 );
	}
	else
	{
		// no special characters
		if ( strchr( szName, ':' ) || strchr( szName, '/' ) || strchr( szName, '\\' ) || strstr( szName, ".." )  )
		{
	#ifdef _AGK_ERROR_CHECK
			agk::Error( "Invalid folder name for DeleteFolder, it must not contain the special characters / : \\ .." );
	#endif
			return;
		}

		uString sDirPath( szWriteDir );
		sDirPath.Append( m_sCurrentDir );
		if ( !SetCurrentDirectoryUTF8( sDirPath ) ) return;

		RemoveDirectoryUTF8( szName );
		SetCurrentDirectoryUTF8( szRootDir );

		m_bUpdateFileLists = true;
	}

	
}

// input commands

//****f* Input-Raw/Existence/GetMultiTouchExists
// FUNCTION
//   Returns 1 if the current device supports multitouch, 0 if not.
// SOURCE
int agk::GetMultiTouchExists()
//****
{
	if ( IsTouchWindowDelayed && g_bGLInit )
	{
		if ( g_hWnd && IsTouchWindowDelayed( g_hWnd, 0 ) ) return 1;
	}

	return 0;
}

//****f* Input-Raw/Existence/GetMouseExists
// FUNCTION
//   Returns 1 if the current device has a mouse, 0 if not.
// SOURCE
int agk::GetMouseExists()
//****
{
	return 1;
}

//****f* Input-Raw/Existence/GetKeyboardExists
// FUNCTION
//   Returns 1 if the current device has a full sized keyboard, 2 if the device has a virtual or mobile phone keyboard
//   and 0 for no keyboard at all.
// SOURCE
int agk::GetKeyboardExists()
//****
{
	return 1;
}

//****f* Input-Raw/Existence/GetCameraExists
// FUNCTION
//   Returns 1 if the current device has a camera, 0 if not. On some platforms this command enumerates the hardware
//   devices to check for a camera which may be slow. It is not recommended that you call this every frame.
//   On Windows this command will successfully detect a new camera being plugged in whilst the app is running.
// SOURCE
int agk::GetCameraExists()
//****
{
	if ( g_hVideoWnd ) return 1;

	ICreateDevEnum* pDevEnum = NULL;

    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void ** ) &pDevEnum);
	if (FAILED(hr))
    {
		if ( pDevEnum ) pDevEnum->Release();
		agk::Warning( "Failed to check camera status - Couldn't create system enumerator" );
        return 0;
    }

	// Create an enumerator for the video capture devices
    IEnumMoniker* pClassEnum = NULL;

    hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pClassEnum, 0);
    if (FAILED(hr))
    {
		if ( pDevEnum ) pDevEnum->Release();
		if ( pClassEnum ) pClassEnum->Release();
        agk::Warning( "Failed to check camera status - Couldn't create class enumerator" );
        return 0;
    }

	if ( !pClassEnum )
    {
		if ( pDevEnum ) pDevEnum->Release();
        return 0;
    }

	if ( pDevEnum ) pDevEnum->Release();
	if ( pClassEnum ) pClassEnum->Release();
	return 1;
}

//****f* Input-Raw/Existence/GetGPSSensorExists
// FUNCTION
//   Returns 1 if the current device has a GPS receiver that is currently supported by AGK, 0 if not.
// SOURCE
int agk::GetGPSSensorExists()
//****
{
	GetLocationUpdate();
	m_iGPSSensorExists = (g_pGeolocationSensor != NULL) ? 1 : 0;
	return m_iGPSSensorExists;
}

//****f* Input-Raw/Mouse/SetRawMouseVisible
// FUNCTION
//   Attempts to hide or show any mouse pointer used on this platform. Not guaranteed to work
//   on all platforms.
// INPUTS
//   visible -- 1 to make it visible, 0 to hide it.
// SOURCE
void agk::SetRawMouseVisible( int visible )
//****
{
	::ShowCursor( visible != 0 ? TRUE : FALSE );
}

//****f* Input-Raw/Mouse/SetRawMousePosition
// FUNCTION
//   Attempts to position any mouse pointer used on this platform. Not guaranteed to work
//   on all platforms. Will only work whilst your app window has focus.
// INPUTS
//   x -- The X component of the new mouse position.
//   y -- The Y component of the new mouse position.
// SOURCE
void agk::SetRawMousePosition( float x, float y )
//****
{
	if ( !g_bGLInit ) return;
	if ( !g_hWnd ) return;
	if ( GetFocus() != g_hWnd ) return;

	int realX = agk::ScreenToDeviceX( x );
	int realY = agk::ScreenToDeviceY( y );

	POINT pt;
	pt.x = realX;
	pt.y = realY;
	ClientToScreen( g_hWnd, &pt );
	SetCursorPos( pt.x, pt.y );

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

void cJoystick::DetectJoysticks()
{
	
}

void cJoystick::PlatformUpdate()
{
	if ( m_iDeviceType == 0 )
	{
		// Direct Input
		IDirectInputDevice8 *pDevice = (IDirectInputDevice8*) m_pDevice;
		HRESULT hr = pDevice->Poll(); 
		if ( FAILED(hr) ) 
		{
			hr = pDevice->Acquire();
			while(hr == DIERR_INPUTLOST) hr = pDevice->Acquire();
			if ( FAILED(hr) ) 
			{
				m_iConnected = 0;
				return;
			}
		}

		DIJOYSTATE2 js;
		hr = pDevice->GetDeviceState(sizeof(DIJOYSTATE2), &js);
		if ( FAILED(hr) ) 
		{
			m_iConnected = 0;
			return;
		}

		m_iConnected = 1;

		m_fX = (js.lX / 32768.0f);
		m_fY = (js.lY / 32768.0f);
		m_fZ = (js.lZ / 32768.0f);
		
		m_fRX = (js.lRx / 32768.0f);
		m_fRY = (js.lRy / 32768.0f);
		m_fRZ = (js.lRz / 32768.0f);

		m_iSlider[ 0 ] = js.rglSlider[ 0 ];
		m_iSlider[ 1 ] = js.rglSlider[ 1 ];

		m_iPOV[ 0 ] = js.rgdwPOV[ 0 ];
		m_iPOV[ 1 ] = js.rgdwPOV[ 1 ];
		m_iPOV[ 2 ] = js.rgdwPOV[ 2 ];
		m_iPOV[ 3 ] = js.rgdwPOV[ 3 ];

		for ( UINT j = 0; j < AGK_MAX_JOYSTICK_BUTTONS; j++ ) m_iButtons[ j ] = js.rgbButtons[ j ] != 0 ? 1 : 0;
	}
	else if ( m_iDeviceType == 1 )
	{
		// XInput
		// XInputGetState takes over a millisecond when disconnected, only check every so often
		if ( m_iConnected == 0 )
		{
			if ( agk::Random( 0, 100 ) != 0 ) return;
		}

		int iID = (int) m_pDevice;
		XINPUT_STATE state;
		DWORD dwResult = XInputGetState( iID, &state );
		if( dwResult != ERROR_SUCCESS )
		{
			m_iConnected = 0;
			return;
		}

		m_iConnected = 1;

		m_fZ = state.Gamepad.bLeftTrigger / 255.0f;
		m_fRZ = state.Gamepad.bRightTrigger / 255.0f;

		m_fX = state.Gamepad.sThumbLX / 32768.0f;
		m_fY = state.Gamepad.sThumbLY / -32768.0f;
					
		m_fRX = state.Gamepad.sThumbRX / 32768.0f;
		m_fRY = state.Gamepad.sThumbRY / -32768.0f;
		
		m_iButtons[ 0 ] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_A) ? 1 : 0;
		m_iButtons[ 1 ] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_B) ? 1 : 0;
		m_iButtons[ 2 ] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_X) ? 1 : 0;
		m_iButtons[ 3 ] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) ? 1 : 0;

		m_iButtons[ 4 ] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) ? 1 : 0;
		m_iButtons[ 5 ] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) ? 1 : 0;

		m_iButtons[ 6 ] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) ? 1 : 0;
		m_iButtons[ 7 ] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_START) ? 1 : 0;

		m_iButtons[ 8 ] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB) ? 1 : 0;
		m_iButtons[ 9 ] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) ? 1 : 0;

		m_iButtons[ 12 ] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) ? 1 : 0;
		m_iButtons[ 13 ] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) ? 1 : 0;
		m_iButtons[ 14 ] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) ? 1 : 0;
		m_iButtons[ 15 ] = (state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) ? 1 : 0;
	}
}

// date/time commands

INT64 agk::GetUnixTime64()
{
	time_t unix = time(NULL);
	return unix;
}

//****f* Time/General/GetUnixTime
// FUNCTION
//   Returns the current date and time in unix time format, which is measured as seconds from 1st Jan 1970.
//   It can be negative for dates before 1970.
// SOURCE
int agk::GetUnixTime()
//****
{
	time_t unix = time(NULL);
	if ( unix > 2147483646 ) unix = 2147483647;
	if ( unix < -2147483647 ) unix = -2147483647;
	return (int) unix;
}

//****f* Time/General/GetDayOfWeek
// FUNCTION
//   Returns an integer representing the current day of the week in the range 0-6 with 0 as Sunday, 1 as Monday, etc.
// SOURCE
int agk::GetDayOfWeek()
//****
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

//****f* Advert/AdMob/LoadConsentStatusAdMob
// FUNCTION
//   Loads the current user consent status from the AdMob server, this must be done before calling 
//   <i>RequestConsentAdMob</i>. You should wait for <i>GetConsentStatusAdMob</i> to return
//   a non-negative value before using the other AdMob consent commands.
// INPUTS
//   szPubID -- The publisher ID for you AdMob account
//   privacyPolicy -- A URL to your privacy policy, this will be shown to the user as a link on the consent dialog
// SOURCE
void agk::LoadConsentStatusAdMob( const char* szPubID, const char* privacyPolicy )
//****
{

}

//****f* Advert/AdMob/GetConsentStatusAdMob
// FUNCTION
//   Returns the current user consent status, it returns one of the following values:<br/>
//   -2 = LoadConsentStatusAdMob has not yet been called, you must call it before proceeding further<br/>
//   -1 = LoadConsentStatusAdMob is in the process of loading the user consent status, please wait<br/>
//   0 = User has not yet been asked, AdMob ads will default to not using personal information, call <i>RequestConsentAdMob</i> to prompt the user for consent<br/>
//   1 = User has refused consent, any AdMob ads displayed will not use personal information<br/>
//   2 = User has given consent, any AdMob ads displayed will use personal information<br/>
// SOURCE
int agk::GetConsentStatusAdMob()
//****
{
	return 0;
}

//****f* Advert/AdMob/RequestConsentAdMob
// FUNCTION
//   Displays a popup dialog asking the user for consent to show personalised ads. The user's response will automatically 
//   be used to modify the AdMob settings so that ads are displayed in accordance with the user's preferences. The consent
//   status will also be saved on the AdMob server so that a future call to <i>LoadConsentStatusAdMob</i> can retrieve the 
//   value. You can call this command at any time during your app so that the user can change their preference, the user 
//   must be able to do this somewhere within your app.<br/>
//   <br/>
//   This consent does not apply to Chartboost or Amazon, it only asks for consent to show personalised ads from AdMob. You 
//   will need your own method of requesting consent for Chartboost and Amazon.
// SOURCE
void agk::RequestConsentAdMob()
//****
{

}

//****f* Advert/AdMob/OverrideConsentAdMob
// FUNCTION
//   Forces AdMob to use the given consent value when showing ads, this is not saved to the AdMob server and overrides 
//   the normal consent process. This is used if you have you own method of aquiring user consent to show personalised ads.
//   Note that the GDPR requires you by law to have the consent from EU users before showing them personalised ads.
// INPUTS
//   consent -- The consent value to use, 1=non-personalised, 2=personalised
// SOURCE
void agk::OverrideConsentAdMob( int consent )
//****
{
	
}

//****f* Advert/Chartboost/OverrideConsentChartboost
// FUNCTION
//   Forces Chartboost to use the given consent value when showing ads, by default AGK shows non-personalised ads from Chartboost. 
//   This command can be used if you have you own method of aquiring user consent to show personalised ads.
//   Note that the GDPR requires you by law to have the consent from EU users before showing them personalised ads.
// INPUTS
//   consent -- The consent value to use, 1=non-personalised, 2=personalised
// SOURCE
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
	//return 641; // SonyEricsson 
	//return 661; // Bada
	//return 738; // Palm
	//return 818; // Samsung
	//return 947; // iPad, doesn't seem to produce ads
}

void cEditBox::PlatformStartText()
{
	// do nothing on windows
}

void cEditBox::PlatformEndText()
{
	// do nothing on windows
}

void cEditBox::PlatformUpdateExternal()
{
	
}

bool cEditBox::PlatformUpdateText()
{
	// do nothing on windows, must return false if not doing anything.
	return false;
}

void cEditBox::PlatformUpdateTextEnd()
{
	// do nothing on windows
}

//****f* HTTP/General/OpenBrowser
// FUNCTION
//   Opens the default browser of the current platform and points it to the page given.
// INPUTS
//   url -- THe url to open
// SOURCE
void agk::OpenBrowser( const char *url )
//****
{
	if ( !url || !*url ) return;

	uString sURL( url );
	if ( sURL.FindStr( ":" ) < 0 ) sURL.Prepend( "http://" );
	ShellExecute( NULL, "open", sURL.GetStr(), NULL, NULL, SW_SHOWNORMAL );

	/*
	HKEY hKey = 0;
	DWORD dwDataSize = MAX_PATH;
	DWORD dwType = REG_SZ;
	char szBrowser [ MAX_PATH ];
	bool bFound = true;

	try
	{
		LSTATUS ls = RegOpenKeyEx( HKEY_CURRENT_USER, "SOFTWARE\\Clients\\StartMenuInternet", NULL, KEY_QUERY_VALUE, &hKey );
		if ( ls != ERROR_SUCCESS ) throw 0;

		ls = RegQueryValueEx( hKey, NULL, NULL, &dwType, (BYTE*)szBrowser, &dwDataSize );
		if ( ls != ERROR_SUCCESS ) throw 1;

		RegCloseKey( hKey );
	}
	catch (int)
	{
		RegCloseKey( hKey );

		bFound = false;
	}

	if ( bFound )
	{
		hKey = 0;
		dwDataSize = MAX_PATH;
		dwType = REG_SZ;
		uString sPath( "SOFTWARE\\Clients\\StartMenuInternet\\" );
		sPath += szBrowser;
		sPath += "\\shell\\open\\command";

		try
		{
			LSTATUS ls = RegOpenKeyEx( HKEY_LOCAL_MACHINE, sPath, NULL, KEY_QUERY_VALUE, &hKey );
			if ( ls != ERROR_SUCCESS ) throw 0;

			ls = RegQueryValueEx( hKey, NULL, NULL, &dwType, (BYTE*)szBrowser, &dwDataSize );
			if ( ls != ERROR_SUCCESS ) throw 1;

			RegCloseKey( hKey );
		}
		catch (int)
		{
			RegCloseKey( hKey );

			bFound = false;
		}
	}

	if ( !bFound ) strcpy_s( szBrowser, MAX_PATH, "iexplore.exe" );
	ShellExecute( NULL, "open", szBrowser, url, NULL, SW_SHOWNORMAL );
	*/
}

//****f* Core/External Apps/RunApp
// FUNCTION
//   Runs an external executable that is independent of this app. It will return an ID that you can 
//   use to check if the app is still running or in extreme cases to terminate the app.
//   If this fails it will return 0.
//   Currently only works on Windows, Mac, Linux, and Raspberry Pi.
// INPUTS
//   szFilename -- The filename of the new app, this is accessed just like any other file in AGK
//   szParameters -- Optional command line parameters to pass to the app, or an empty string
// SOURCE
UINT agk::RunApp( const char *szFilename, const char *szParameters )
//****
{
	uString sPath( szFilename );
	GetRealPath( sPath );

	uString sCmdLine;
	sCmdLine.Format( "\"%s\" %s", sPath.GetStr(), szParameters ? szParameters : "" );
	
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;

	ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));

	siStartInfo.cb         = sizeof(STARTUPINFO);

	char *str = new char[ sCmdLine.GetLength()+1 ];
	strcpy( str, sCmdLine.GetStr() );
	BOOL result = CreateProcess( NULL, str, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo );
	delete [] str;
	if ( !result )
	{
		agk::Warning( "Failed to create process" );
		return 0;
	}

	CloseHandle( piProcInfo.hProcess );
	CloseHandle( piProcInfo.hThread );

	return piProcInfo.dwProcessId;
}

//****f* Core/External Apps/GetAppRunning
// FUNCTION
//   Returns 1 if the specified app ID is still running, 0 if it is not.
// INPUTS
//   appID -- The app ID to check, returned from RunApp()
// SOURCE
UINT agk::GetAppRunning( UINT appID )
//****
{
	HANDLE hProc = OpenProcess( SYNCHRONIZE, FALSE, appID );
	if ( !hProc ) return 0;

	DWORD ret = WaitForSingleObject( hProc, 0 );
	CloseHandle( hProc );

	return (ret == WAIT_TIMEOUT) ? 1 : 0;
}

//****f* Core/External Apps/TerminateApp
// FUNCTION
//   Immediately terminates the specified app. This should only be used in extreme cases as the app is not
//   given a chance to clean up or finish what it is doing.
// INPUTS
//   appID -- The app ID to terminate, returned from RunApp()
// SOURCE
void agk::TerminateApp( UINT appID )
//****
{
	HANDLE hProc = OpenProcess( PROCESS_TERMINATE, TRUE, appID );
	if ( !hProc ) return;

	TerminateProcess( hProc, 0 );
}

//****f* Core/External Apps/ViewFile
// FUNCTION
//   Opens the given file in an the external application suitable for its file type. Typically this is a 
//   default app chosen by the user. The file should be in your read or write folder, you should provide 
//   the filename as if you were loading the file. Currently this only works on Windows, iOS, and Android.
// INPUTS
//   szFilename -- The path to the file to open
// SOURCE
void agk::ViewFile( const char* szFilename )
//****
{
	uString sPath( szFilename );
	if ( !GetRealPath( sPath ) )
	{
		agk::Error( "Could not find specified file" );
		return;
	}

	ShellExecute( NULL, "open", sPath.GetStr(), NULL, NULL, SW_SHOWNORMAL );
}

//****f* Core/External Apps/ShareText
// FUNCTION
//   Sends the given text to the operating system which will then ask the user how they want to share the 
//   text, e.g. through Facebook, Twitter, etc. This only works on iOS and Android.
// INPUTS
//   szText -- The text to share
// SOURCE
void agk::ShareText( const char* szText )
//****
{
	
}

//****f* Core/External Apps/ShareImage
// FUNCTION
//   Sends the given image to the operating system which will then ask the user how they want to share the 
//   image, e.g. through Facebook, Twitter, etc. The file should be in your read or write folder, or you can 
//   use a "raw:" path to load from anywhere, you should provide the filename as if you were loading the 
//   file. This only works on iOS and Android.
// INPUTS
//   szFilename -- The filename of the image to share
// SOURCE
void agk::ShareImage( const char* szFilename )
//****
{
	
}

//****f* Core/External Apps/ShareImageAndText
// FUNCTION
//   Sends the given image and text to the operating system which will then ask the user how they want to share the 
//   text, e.g. through Facebook, Twitter, etc. The image file should be in your read or write folder, or you can 
//   use a "raw:" path to load from anywhere, you should provide the filename as if you were loading the 
//   file. This only works on iOS and Android.
// INPUTS
//   szFilename -- The filename of the image to share
//   szText -- The text to share
// SOURCE
void agk::ShareImageAndText( const char* szFilename, const char* szText )
//****
{

}

//****f* Core/External Apps/ShareFile
// FUNCTION
//   Sends the given file to the operating system which will then ask the user how they want to share it, 
//   e.g. through email, NFC, etc. The file can be in your read or write folder, or you can 
//   use a "raw:" path to load from anywhere, you should provide the filename as if you were loading the 
//   file. This only works on iOS and Android.
// INPUTS
//   szFilename -- The path to the file to share
// SOURCE
void agk::ShareFile( const char* szFilename )
//****
{

}

//****f* Extras/Facebook/FacebookActivateAppTracking
// FUNCTION
//   Activates tracking in the Facebook SDK, this is useful if you are using Facebook Ads as this will 
//   associate installs to adverts previously seen and clicked on Facebook. Note that on iOS this 
//   functionality requires the IDFA (Advertising Identifier) and you must declare this if you submit 
//   the app to Apple. Apple will ask if you use the IDFA and what you use it for. For install tracking 
//   with Facebook Ads you MUST tick "Attribute this app installation to a previously served Advertisement" 
//   and "Attribute an action taken within this app to a previously served advertisement". If you are also 
//   using AdMob, Amazon Ads, or Chartboost in your app then you must also tick "Serve advertisements 
//   within the app".<br/><br/>
//   This command does not require the user to be logged in.
// SOURCE
void agk::FacebookActivateAppTracking()
//****
{
	
}

//****f* HTTP/General/GetInternetState
// FUNCTION
//   Returns 1 if the device is connected to the internet, 0 if not.
// SOURCE
int agk::GetInternetState()
//****
{
	DWORD dwState = 0;
	BOOL bOnline = InternetGetConnectedState( &dwState, 0 );
	if ( !bOnline )
	{
		return 0;
	}

	return 1;
}

//****f* Extras/PushNotifications/SetPushNotificationKeys
// FUNCTION
//   This command is used on Android to set the SenderID used by the Firebase project. Currently keyName must be 
//   set to "SenderID" (case sensitive), and the keyValue must be set to the SenderID value that can be found in
//   your Firebase project settings, in the Cloud Messaging tab.
// INPUTS
//   keyName -- The key to set
//   keyValue -- The key value
// SOURCE
void agk::SetPushNotificationKeys( const char* keyName, const char* keyValue )
//****
{
	// do nothing on windows
}

//****f* Extras/PushNotifications/PushNotificationSetup
// FUNCTION
//   Returns 1 if setup was successful, 0 if it failed or the device does not support push notifications.
// SOURCE
int agk::PushNotificationSetup()
//****
{
	return 0;
}

//****f* Extras/PushNotifications/GetPushNotificationToken
// FUNCTION
//   Returns the push notification token for this device, this must be sent to your server that sends out the 
//   notifications so that it can send notifications to this device.
//   If PushNotificationSetup returned 1 then you should keep calling this command until it returns a non-empty
//   string. If PushNotificationSetup returned 0 then this command will always return an empty string.<br/>
//   <br/>
//   Please see the guide on Android Push Notifications for more details on how to use the device token to send 
//   push notifications to this device.
// SOURCE
char* agk::GetPushNotificationToken()
//****
{
	char *str = new char[ 1 ];
	*str = 0;
	return str;
}

// V108 ULTRABOOK COMMANDS

// Internal support functions

void agk::InitialiseSensorCommands()
{
	// construct and initialise any code used in the Ultra command set
	g_pSensorManager = NULL;
	g_pSensorCollection = NULL;

	// Light Sensor
	g_pSensorEventClass = NULL;
	g_pSensorEvents = NULL;
	g_pGeolocationSensor = NULL;
	g_pMagnetSensor = NULL;
	g_pGyroSensor = NULL;
	g_pLightSensor = NULL;

	// Prepare all sensors for usage
    HRESULT hr = ::CoCreateInstance(CLSID_SensorManager, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pSensorManager));
    if (SUCCEEDED(hr))
	{
		// Get a list of all motion sensors on the computer, but if required you can find specific sensor type e.g. SENSOR_TYPE_GYROMETER_3D. Don't forget to check the result, because the system may not have any sensors or there might be driver problem.  
	    hr = g_pSensorManager->GetSensorsByCategory(SENSOR_CATEGORY_ALL, &g_pSensorCollection); 
		if ( FAILED(hr) ) return;

		// Now find out the required sensor. To get accelerometer you can provide SENSOR_DATA_TYPE_ACCELERATION_Z_G as argument, for Gyroscope use SENSOR_DATA_TYPE_TILT_Z_DEGREES and so on. Following example is for Ambient Light sensor.    
		ISensor* pAnySensor = NULL;
	    ULONG ulCount = 0;
		hr = g_pSensorCollection->GetCount(&ulCount);
	    if (SUCCEEDED(hr))
		{
			for (int i = 0; i < (int)ulCount; i++)
			{
				ISensor* pSensor;
				hr = g_pSensorCollection->GetAt(i, &pSensor);
				if (SUCCEEDED(hr))
				{
					// check for the different sensors
					VARIANT_BOOL bSupported = VARIANT_FALSE;

					// AccelerometerSensor
					bSupported = VARIANT_FALSE;
					hr = pSensor->SupportsDataField(SENSOR_DATA_TYPE_ACCELERATION_X_G, &bSupported);
					if (SUCCEEDED(hr) && (bSupported == VARIANT_TRUE))
					{
						g_pAccelerometerSensor = pSensor;
						pAnySensor = pSensor;
					}

					// GetGeolocation
					/* using locationAPI instead
					bSupported = VARIANT_FALSE;
					hr = pSensor->SupportsDataField(SENSOR_DATA_TYPE_LATITUDE_DEGREES, &bSupported);
					if (SUCCEEDED(hr) && (bSupported == VARIANT_TRUE))
					{
						g_pGeolocationSensor = pSensor;
						pAnySensor = pSensor;
					}
					*/

					// GetMagnet
					bSupported = VARIANT_FALSE;
					hr = pSensor->SupportsDataField(SENSOR_DATA_TYPE_MAGNETIC_FIELD_STRENGTH_X_MILLIGAUSS, &bSupported);
					if (SUCCEEDED(hr) && (bSupported == VARIANT_TRUE))
					{
						g_pMagnetSensor = pSensor;
						pAnySensor = pSensor;
					}

					// GetGyrometer
					bSupported = VARIANT_FALSE;
					hr = pSensor->SupportsDataField(SENSOR_DATA_TYPE_ANGULAR_VELOCITY_X_DEGREES_PER_SECOND, &bSupported);
					if (SUCCEEDED(hr) && (bSupported == VARIANT_TRUE))
					{
						g_pGyroSensor = pSensor;
						pAnySensor = pSensor;
					}

					// Ambient light
					bSupported = VARIANT_FALSE;
					hr = pSensor->SupportsDataField(SENSOR_DATA_TYPE_LIGHT_LEVEL_LUX, &bSupported);
					if (SUCCEEDED(hr) && (bSupported == VARIANT_TRUE))
					{
						g_pLightSensor = pSensor;
						pAnySensor = pSensor;
					}
				}
			}
		}

		// Check for sensor permissions
		if ( pAnySensor!=NULL )
		{
			SensorState state = SENSOR_STATE_ERROR;
			hr = pAnySensor->GetState(&state);
			if (state == SENSOR_STATE_ACCESS_DENIED)
			{
				// Ask permission for ALL sensors available
				hr = g_pSensorManager->RequestPermissions(NULL, g_pSensorCollection, TRUE);
			} 
		}

		// Create event sink hook to event handler
		if ( pAnySensor!=NULL )
		{
			// Now you need to hook the Sensor for any DataUpdated, Leave, or StateChanged events.
			g_pSensorEventClass = new cSensorEventSink(); // create class instance
			g_pSensorEvents = NULL;
			hr = g_pSensorEventClass->QueryInterface(IID_PPV_ARGS(&g_pSensorEvents)); 
			if (SUCCEEDED(hr))
			{
				// required events
				GUID pguid[2];
				pguid[0] = SENSOR_EVENT_DATA_UPDATED;
				pguid[1] = SENSOR_EVENT_STATE_CHANGED;

				// required sensitivity
				IPortableDeviceValues* pInDeviceValues;
				hr = ::CoCreateInstance(CLSID_PortableDeviceValues, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pInDeviceValues));
				if (FAILED(hr))
				{
					::MessageBox(NULL, "Unable to create a PortableDeviceValues collection", "AGK", MB_OK | MB_ICONERROR);
					return;
				}
				PROPVARIANT pv;
				PropVariantInit(&pv);
				IPortableDeviceValues* pInValues = NULL;
				IPortableDeviceValues* pOutValues = NULL;

				// AccelerometerSensor to event handler
				if ( g_pAccelerometerSensor!=NULL )
				{
					hr = g_pAccelerometerSensor->SetEventSink(g_pSensorEvents);
					if (SUCCEEDED(hr))
					{
						g_pAccelerometerSensor->SetEventInterest(pguid, 2);
					}
					else
					{
						g_pAccelerometerSensor->Release();
						g_pAccelerometerSensor = NULL;
					}
				}

				// GeolocationSensor to event handler
				/* using locationAPI instead
				if ( g_pGeolocationSensor!=NULL )
				{
					hr = g_pGeolocationSensor->SetEventSink(g_pSensorEvents);
					if (SUCCEEDED(hr))
					{
						g_pGeolocationSensor->SetEventInterest(pguid, 2);
					}
					else
					{
						g_pGeolocationSensor->Release();
						g_pGeolocationSensor = NULL;
					}
				}
				*/

				// MagnetSensor to event handler
				if ( g_pMagnetSensor!=NULL )
				{
					hr = g_pMagnetSensor->SetEventSink(g_pSensorEvents);
					if (SUCCEEDED(hr))
					{
						g_pMagnetSensor->SetEventInterest(pguid, 2);
					}
					else
					{
						g_pMagnetSensor->Release();
						g_pMagnetSensor = NULL;
					}
				}

				// GyroSensor to event handler
				if ( g_pGyroSensor!=NULL )
				{
					hr = g_pGyroSensor->SetEventSink(g_pSensorEvents);
					if (SUCCEEDED(hr))
					{
						g_pGyroSensor->SetEventInterest(pguid, 2);
					}
					else
					{
						g_pGyroSensor->Release();
						g_pGyroSensor = NULL;
					}
				}

				// Ambient Light sensor to event handler
				if ( g_pLightSensor!=NULL )
				{
					hr = g_pLightSensor->SetEventSink(g_pSensorEvents);
					if (SUCCEEDED(hr))
					{
						g_pLightSensor->SetEventInterest(pguid, 2);
						SetCurrentReportInterval(g_pLightSensor,50);
						pv.vt = VT_R8; // COM double
						pv.dblVal = (double)1.0;
						pInDeviceValues->SetValue(SENSOR_DATA_TYPE_LIGHT_LEVEL_LUX, &pv);
						hr = ::CoCreateInstance(CLSID_PortableDeviceValues, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pInValues));
						if (SUCCEEDED(hr))
						{
							pInValues->SetIPortableDeviceValuesValue(SENSOR_PROPERTY_CHANGE_SENSITIVITY, pInDeviceValues);
							g_pLightSensor->SetProperties(pInValues, &pOutValues);
						}
					}
					else
					{
						g_pLightSensor->Release();
						g_pLightSensor = NULL;
					}
				}

				// free usages
				PropVariantClear(&pv);
			}
		}
	}

	m_iAccelerometerExists = (g_pAccelerometerSensor != NULL) ? 1 : 0;
	m_iGyroSensorExists = (g_pGyroSensor != NULL) ? 1 : 0;
	m_iGPSSensorExists = (g_pGeolocationSensor != NULL) ? 1 : 0;
	m_iLightSensorExists = (g_pLightSensor != NULL) ? 1 : 0;
	m_iMagneticSensorExists = (g_pMagnetSensor != NULL) ? 1 : 0;
	//m_iRotationSensorExists = (g_pOrientationSensor != NULL) ? 1 : 0;
}

void agk::FreeSensorCommands()
{
	// free any resources used by the Ultra command set (but not sensors which could be duplicated)
	//if(g_pAccelerometerSensor!=NULL) { g_pAccelerometerSensor->Release(); g_pAccelerometerSensor=NULL; }
	if(g_pGeolocationSensor!=NULL) { g_pGeolocationSensor->Release(); g_pGeolocationSensor=NULL; }
	//if(g_pCompassSensor!=NULL) { g_pCompassSensor->Release(); g_pCompassSensor=NULL; }
	//if(g_pGyroSensor!=NULL) { g_pGyroSensor->Release(); g_pGyroSensor=NULL; }
	//if(g_pInclinometerSensor!=NULL) { g_pInclinometerSensor->Release(); g_pInclinometerSensor=NULL; }
	//if(g_pLightSensor!=NULL) { g_pLightSensor->Release(); g_pLightSensor=NULL; }
	//if(g_pOrientationSensor!=NULL) { g_pOrientationSensor->Release(); g_pOrientationSensor=NULL; }
	//if(g_pSensorEvents!=NULL) { g_pSensorEvents->Release(); g_pSensorEvents=NULL; }
	//if(g_pSensorEventClass!=NULL) { delete g_pSensorEventClass; g_pSensorEventClass=NULL; }
	//if(g_pSensorCollection!=NULL) { g_pSensorCollection->Release(); g_pSensorCollection=NULL; }
	if(g_pSensorManager!=NULL) { g_pSensorManager->Release(); g_pSensorManager=NULL; }
}

// Actual command functions

int agk::GetNotificationType()
//****
{
	return 0;
}

void agk::SetNotificationImage(int iImageIndex)
//****
{

}

void agk::SetNotificationText( const char* pText)
//****
{
}

//****f* Input-Raw/Existence/GetNFCExists
// FUNCTION
//   Returns 1 if the current device has an NFC device built-in, otherwise 0 is returned.
//   This command is not yet functional.
// SOURCE
int agk::GetNFCExists()
//****
{
	return 0;
}

// FUNCTION
//   Returns the number of NFC devices detected within range of the device.
//   This command is not yet functional.
// SOURCE
UINT agk::GetRawNFCCount()
{
	return 0;
}

// FUNCTION
//   Returns the device ID of the first NFC device currently in range. Use <i>GetRawNextNFCDevice</i> to iterate through all NFC devices.
//   This command is not yet functional.
// SOURCE
UINT agk::GetRawFirstNFCDevice()
{
	return 0;
}

// FUNCTION
//   Returns the device ID of the next NFC device currently in range. Use <i>GetRawFirstNFCDevice</i> to initiate the iteration through all NFC devices.
//   This command is not yet functional.
// SOURCE
UINT agk::GetRawNextNFCDevice()
{
	return 0;
}

// FUNCTION
//   Get the name of the NFC device specified by the index.
//   This command is not yet functional.
// INPUTS
//   Index -- the index returned from the <i>GetRawFirstNFCDevice</i> and <i>GetRawNextNFCDevice</i> commands.
// SOURCE
char* agk::GetRawNFCName(UINT iIndex)
{
	return 0;
}

// FUNCTION
//   Send raw text data to the NFC device specified by the index.
//   This command is not yet functional.
// INPUTS
//   Index -- the index returned from the <i>GetRawFirstNFCDevice</i> and <i>GetRawNextNFCDevice</i> commands.
//   MessageText -- the text to send to the NFC device.
// SOURCE
int agk::SendRawNFCData(UINT iIndex, const char* pMessageText)
{
	return 0;
}

// FUNCTION
//   Get the state of the specified NFC device. A value of 0 means there is no data waiting. A value of 1 means the NFC device has data to send to this app.
//   This command is not yet functional.
// INPUTS
//   Index -- the index returned from the <i>GetRawFirstNFCDevice</i> and <i>GetRawNextNFCDevice</i> commands.
// SOURCE
int agk::GetRawNFCDataState(UINT iIndex)
{
	return 0;
}

// FUNCTION
//   Get the data being sent from the specified NFC device. The data is a text string no larger than 4K in size.
//   This command is not yet functional.
// INPUTS
//   Index -- the index returned from the <i>GetRawFirstNFCDevice</i> and <i>GetRawNextNFCDevice</i> commands.
// SOURCE
char* agk::GetRawNFCData(UINT iIndex)
{
	return 0;
}

//****f* Input-Raw/Sensors/StartGPSTracking
// FUNCTION
//   Starts the process of getting a GPS location if possible on the current device. This may produce a permission request
//   on some platforms the first time you call it. If the user denies permission then <i>GetGPSSensorExists</i> may return 
//   0 from then on. You should only call this command when the user is doing an activity they know will require a location, 
//   otherwise they may deny it and never get asked again (they will have to go into their device settings to enable it).
//   This command will consume a lot of battery power, so use <i>StopGPSTracking</i> when you no longer need a location.
//   This command works on iOS, Android, Mac, and Windows 7 and above.
//   It may take some time from calling this command to get an accurate location from <i>GetRawGPSLatitude</i> and
//   <i>GetRawGPSLongitude</i>.
// SOURCE
void agk::StartGPSTracking()
//****
{
	g_iIsGPSTracking = 1;
	GetLocationUpdate();
}

//****f* Input-Raw/Sensors/StopGPSTracking
// FUNCTION
//   Stops GPS location updates, use this to save battery power when you no longer need a location.
// SOURCE
void agk::StopGPSTracking()
//****
{
	g_iIsGPSTracking = 0;
}

//****f* Input-Raw/Sensors/GetRawGPSLatitude
// FUNCTION
//   Returns the most up to date latitude available.
// SOURCE
float agk::GetRawGPSLatitude()
//****
{
	if ( g_iIsGPSTracking ) GetLocationUpdate();
	return m_fGPSLat;
}

//****f* Input-Raw/Sensors/GetRawGPSLongitude
// FUNCTION
//   Returns the most up to date longitude available.
// SOURCE
float agk::GetRawGPSLongitude()
//****
{
	if ( g_iIsGPSTracking ) GetLocationUpdate();
	return m_fGPSLong;
}

//****f* Input-Raw/Sensors/GetRawGPSAltitude
// FUNCTION
//   Returns the most up to date altitude available in meters above sea level.
// SOURCE
float agk::GetRawGPSAltitude()
//****
{
	if ( g_iIsGPSTracking ) GetLocationUpdate();
	return m_fGPSAltitude;
}

//
// V108 GameCenter Commands
//

//****f* Extras/GameCenter/GetGameCenterExists
// FUNCTION
//   Returns 1 if the current platform supports Game Center or Google Play Games. 
// SOURCE
int agk::GetGameCenterExists()
//****
{
	return 0;
}

//****f* Extras/GameCenter/GameCenterSetup
// FUNCTION
//   Call this once to setup your app for further Game Center commands.
// SOURCE
void agk::GameCenterSetup()
//****
{
	
}

//****f* Extras/GameCenter/GameCenterLogin
// FUNCTION
//   Call this once to log the user in to Game Center (iOS) or Google Play Games (Android), if they have 
//   logged in before this happens in the background and does not interrupt the user, otherwise a popup will 
//   appear asking them to log in and grant permission to continue.
// SOURCE
void agk::GameCenterLogin()
//****
{
	
}

//****f* Extras/GameCenter/GameCenterLogout
// FUNCTION
//   Call this to log the user out of Google Play Games. After this you may call <i>GameCenterLogin</i>
//   again. On Game Center (iOS) this has no affect, the user must logout from the Game Center app.
// SOURCE
void agk::GameCenterLogout()
//****
{
	
}

//****f* Extras/GameCenter/GetGameCenterLoggedIn
// FUNCTION
//   Will return 1 if the user is logged in to Game Center or Google Play Games, 0 if not. The login process is Asynchronous 
//   so after calling <i>GameCenterLogin</i> it may take a few seconds for this command to return 1.
//   If the user fails to login or GameCenter is not available then this will return -1.
// SOURCE
int agk::GetGameCenterLoggedIn()
//****
{
	return 0;
}

//****f* Extras/GameCenter/GetGameCenterPlayerID
// FUNCTION
//   Returns the unique ID for the currently logged in player. If no one is logged in, or the platform
//   does not support GameCenter commands then this will return an empty string.
//   If calling this command from Tier 2 then the returned string must be deleted with agk::DeleteString
//   when you are done with it.
// SOURCE
char* agk::GetGameCenterPlayerID()
//****
{
	char *str = new char[1]; *str = 0;
	return str;
}

//****f* Extras/GameCenter/GetGameCenterPlayerDisplayName
// FUNCTION
//   Returns the display name for the currently logged in player. If no one is logged in, or the platform
//   does not support GameCenter commands then this will return an empty string.
//   If calling this command from Tier 2 then the returned string must be deleted with agk::DeleteString
//   when you are done with it.
// SOURCE
char* agk::GetGameCenterPlayerDisplayName()
//****
{
	char *str = new char[1]; *str = 0;
	return str;
}

//****f* Extras/GameCenter/GameCenterSubmitScore
// FUNCTION
//   Submits a score to a named leader board, this must match the Leaderboard ID given to your board in 
//   iTunes connect, Google Game Services, or GameCircle.
// INPUTS
//   iScore -- The score to submit
//   szBoardID -- The name of the leader board
// SOURCE
void agk::GameCenterSubmitScore( int iScore, const char* szBoardID )
//****
{
	
}

//****f* Extras/GameCenter/GameCenterShowLeaderBoard
// FUNCTION
//   Shows the user the named leaderboard, the name must match the Leaderboard ID given to your board in 
//   iTunes connect, Google Game Services, or GameCircle.
// INPUTS
//   szBoardID -- The name of the leader board
// SOURCE
void agk::GameCenterShowLeaderBoard ( const char* szBoardID )
//****
{
	
}

//****f* Extras/GameCenter/GameCenterSubmitAchievement
// FUNCTION
//   Change the users progress on reaching a named achievement, the name must match the Achievement ID given 
//   to the achievement in iTunes connect, Google Game Services, or GameCircle.
//   The iPercentageComplete values should be between 0 and 100. However, for incremental achievements this 
//   value is actually the number of steps completed and may be outside of this range. The value will overwrite 
//   the current percentage or steps completed of the achievement.
// INPUTS
//   szAchievementID -- The name of the achievement
//   iPercentageComplete -- The users progress towards getting this achievement, or the number of steps completed
// SOURCE
void agk::GameCenterSubmitAchievement ( const char* szAchievementID, int iPercentageComplete )
//****
{
	
}

//****f* Extras/GameCenter/GameCenterAchievementsShow
// FUNCTION
//   Shows the user their achievements and progress.
// SOURCE
void agk::GameCenterAchievementsShow ( )
//****
{
	
}

//****f* Extras/GameCenter/GameCenterAchievementsReset
// FUNCTION
//   Not yet functional
// SOURCE
void agk::GameCenterAchievementsReset ( )
//****
{
	
}

//****f* Extras/Permissions/CheckPermission
// FUNCTION
//   Only for Android, all other platforms will return 1 and do nothing. Checks if the user has granted your
//   app the specified permission to use certain Android commands. The permissions are as follows<br/>
//   "WriteExternal" - used by the <i>SaveSharedVariable</i> commands and any "raw:" paths that access the sdcard.<br/>
//   "Location" - used by the GPS commands<br/>
//   "Camera" - used by the <i>SetDeviceCameraToImage</i> command</br>
//   "RecordAudio" - used by the <i>StartScreenRecording</i> command and for the AR camera feed</br>
//   This command will return 0 if you don't have permission and the user hasn't been asked yet, you should call
//   <i>RequestPermission</i> if you need it. This command returns 1 if the user is in the process of being asked
//   for permission, you should wait for a reply by continuing to call this command. It will return -1 if the user 
//   rejected permission, or 2 if the user granted permission. If the user rejects your request then you can ask again
//   but you should explain why your app needs it first so they can make an informed choice. If they still reject it
//   you should not normally ask a third time. After the first attempt Android will give the user the option to
//   never allow the permission. In this case this command will always return -1 and <i>RequestPermission</i> will do 
//   nothing. The user will have to go into the device app settings to reverse this decision.<br/><br/>
//   Any permission not listed here does not require you to prompt the user for permission, it will be auto granted 
//   on install if your app requires it. All permissions that your app uses, even those that require prompting, must
//   be specified in the APK export dialog by ticking the appropriate check boxes.
// INPUTS
//   szPermission -- The permission to check
// SOURCE
int agk::CheckPermission( const char* szPermission )
//****
{
	return 2;
}

//****f* Extras/Permissions/RequestPermission
// FUNCTION
//   Only for Android, all other platforms will do nothing. Generates a system dialog asking the user for
//   the specified device permission, used by certain Android commands. The permissions are as follows<br/>
//   "WriteExternal" - used by the <i>SaveSharedVariable</i> commands and any "raw:" paths that access the sdcard.<br/>
//   "Location" - used by the GPS commands<br/>
//   "Camera" - used by the <i>SetDeviceCameraToImage</i> command and for the AR camera feed</br>
//   "RecordAudio" - used by the <i>StartScreenRecording</i> command</br>
//   You can check the result of the request by calling <i>CheckPermission</i>.
//   After the first request Android will give the user the option to stop displaying the request dialog. In this case 
//   this command will no longer generate a request dialog and instead do nothing. The user will have to go into 
//   the device app settings to reverse this decision.<br/><br/>
//   Any permission not listed here does not require you to prompt the user for permission, it will be auto granted 
//   on install if your app requires it. All permissions that your app uses, even those that require prompting, must
//   be specified in the APK export dialog by ticking the appropriate check boxes.
// INPUTS
//   szPermission -- The permission to request
// SOURCE
void agk::RequestPermission( const char* szPermission )
//****
{

}

// Cloud data

//****f* Extras/Cloud Data/SetupCloudData
// FUNCTION
//   Sets up the device to use cloud data on compatible platforms, currently only Android and iOS support this feature.
//   Cloud data can be used to share settings across multiple devices running the same app. For example saving some app
//   settings or progress on one device using the cloud data commands will make them available (after a short period of 
//   time) on all other devices on the same platform. Note that data does not cross platforms, so data saved on iOS is
//   not available on Android, and data saved on Android is not available on iOS.
//   <br/><br/>
//   On iOS this uses iCloud Drive to store the data, so the user must be logged in to iCloud and have iCloud Drive 
//   turned on, you can detect if these have been done with <i>GetCloudDataAllowed</i>. You must also add iCloud to your
//   provisioning profile, but you do not need to create any containers. You must use this provisioning profile when 
//   exporting your app. On iOS you are limited to 1MB of storage.
//   <br/><br/>
//   On Android this uses Android Drive AppData so the user must be logged in with a Google account on the device.
//   You must also add a Google API project for your app here <a href="https://console.developers.google.com">
//   https://console.developers.google.com</a> and create an OAuth client ID in the credentials section. Tell it that
//   the request is coming from Android, give it a name of your choice, and tell it your app package name. You will
//   need give it the SHA-1 fignerprint of the keystore file that you will use to sign your APK, this is so it can
//   check that only APKs signed by you can use the generated OAuth client ID. The "keytool" program is part of the 
//   Java Development Kit (JDK). Once created you do not need to do anything with the OAuth token, it will be used
//   automatically when you call the Cloud Data commands. On Android any data you store will count against a user's
//   Google Drive storage limit, they cannot access the files stored by your app but they can clear the app data in 
//   their Drive settings.
//   <br/><br/>
//   Broadcast apps will use the AGK Player data store and will share variables across all your broadcast apps, so 
//   be aware of overwriting values if two of your apps share the same variable name. Data you store in the AGK Player 
//   data store will only be visible to you and your devices.
//   <br/><br/>
//   This command must be called before any other cloud data commands, except <i>GetCloudDataAllowed</i> which can be 
//   called at any time. On Android you should ask the user if they want to store data in their Drive before calling 
//   setup, as this command may ask the user to login to their Google Account.
// INPUTS
//   reserved -- Reserved, must be an empty string
// SOURCE
void agk::SetupCloudData( const char* reserved )
//****
{

}

//****f* Extras/Cloud Data/GetCloudDataAllowed
// FUNCTION
//   Returns 1 if the cloud data commands are available to use. Returns 0 if the user hasn't decided whether to allow
//   or block access yet, this will be prompted in <i>SetupCloudData</i> if applicable. Returns -1 if the user has 
//   specifically denied access. Returns -2 if the user is not logged in or the device does not have iCloud or 
//   Google Drive. You can prompt the user to login and enable these when your app first starts, or when they
//   choose to enable cloud backup in your own app settings. If this command returns -3 on Android then the Google 
//   cloud data has become corrupted and must be cleared in the Drive settings.
// SOURCE
int agk::GetCloudDataAllowed()
//****
{
	return -2;
}

//****f* Extras/Cloud Data/GetCloudDataChanged
// FUNCTION
//   Returns 1 if the cloud data has changed due to another device changing a value, this will remain 1 until you
//   call <i>GetCloudDataVariable</i>. Otherwise returns 0. This will not become 1 when you change the value locally.
// SOURCE
int agk::GetCloudDataChanged()
//****
{
	return 0;
}

//****f* Extras/Cloud Data/GetCloudDataVariable
// FUNCTION
//   Returns the value of a cloud data variable by name. If the variable does not exist then the default value passed 
//   in will be returned instead. If you are calling this command from Tier 2 then you must delete the returned string
//   when you are done with it by calling agk::DeleteString.
// INPUTS
//   varName -- The name of the variable to check, must be less than 64 bytes
//   defaultValue -- The value to return if the variable doesn't exist
// SOURCE
char* agk::GetCloudDataVariable( const char* varName, const char* defaultValue )
//****
{
	char *str = new char[1]; *str = 0;
	return str;
}

//****f* Extras/Cloud Data/SetCloudDataVariable
// FUNCTION
//   Sets a cloud data variable to the specified value. If multiple devices write a value at the same time 
//   then the cloud provider chooses one and sends it to all devices. The variable name must be less than 64
//   bytes, a single UTF8 character may be multiple bytes but typically 1 character equals 1 byte. It is 
//   recommended that variable names incorporate the app name, for example "myapp.myvariablename", since 
//   when broadcasting your apps they will all share a single data store in the AGK PLayer app. Varaible data 
//   is stored locally so <i>GetCloudDataVariable</i> will immediately return the new value, however syncing
//   with the cloud is done in the background and may take several minutes. If the device has no internet 
//   connection then the data will be synced at the next available opportunity.
//   On iOS you are limited to 1024 variables with 1MB total storage. On Android any data you store counts
//   against the user's Google Drive storage limit, the user can clear your app data to free up space, but 
//   they cannot read the data you store.
//   <br/></br>
//   You should not store sensitive information like passwords in plain text using these commands.
// INPUTS
//   varName -- The name of the variable to change, must be less than 64 bytes
//   varValue -- The value to set the variable 
// SOURCE
void agk::SetCloudDataVariable( const char* varName, const char* varValue )
//****
{
	
}

//****f* Extras/Cloud Data/DeleteCloudDataVariable
// FUNCTION
//   Deletes the variable with the specified name and removes any data stored in the cloud for that variable.
//   If the variable doesn't exist then this does nothing.
// INPUTS
//   varName -- The name of the variable to delete, must be less than 64 bytes
// SOURCE
void agk::DeleteCloudDataVariable( const char* varName )
//****
{
	
}

// shared variables

//****f* Extras/Shared Variables/SetSharedVariableAppGroup
// FUNCTION
//   For iOS only. Tells AGK what App Group you are using to share data between apps. For example
//   group.com.mycompany.mygroup.myvariables, this is created on the Apple developer portal and
//   must be added to every App ID you want to access these variables. You will need to regenerate 
//   the provisioning profile for each app after adding an App Group to its App ID.
// INPUTS
//   group -- The name to use to identify this variable
// SOURCE
void agk::SetSharedVariableAppGroup( const char* group )
//****
{
	
}

//****f* Extras/Shared Variables/SaveSharedVariable
// FUNCTION
//   Saves a variable so it can be accessed from other apps. This only works on iOS, Android, and HTML5 platforms, 
//   and apps can only share a variable if they meet certain requirements based on the platform.<br/><br/>
//
//   On iOS the apps must be created by the same Apple developer account, and have the same App Group added 
//   to their App IDs on the Apple developer portal. You will need to regenerate the provisioning profile 
//   after doing this. On iOS you must tell AGK what the App Group is by using <i>SetSharedVariableAppGroup</i>.<br/><br/>
//
//   On Android the apps must have the WRITE_EXTERNAL_STORAGE permission and have the same package name up 
//   to the last dot. For example com.mycompany.mygroup.myapp1 and com.mycompany.mygroup.myapp2 would be able 
//   to share variables. On Android shared variables are written to a user accessible location, so be aware 
//   that users may be able to read and/or edit the variables that you save.<br/><br/>
//
//   On HTML the apps must be hosted on the same domain, the values are stored as cookies.<br/><br/>
//
//   Variables are identified by name, so using SaveSharedVariable("username", "Alice") will set the 
//   variable called "username" to the value "Alice". This can then be read by other apps by using
//   LoadSharedVariable( "username", "" )
//   If two apps save different values to the same variable name then the value will be overwritten and 
//   only the most recent value will be accessible.
//   Variable values will persist even if the app is deleted and re-installed.
//   You should not use this command to store any sensitive information such as passwords in plain text.
//   On iOS the length of the variable name plus the length of your App ID up to the last dot 
//   (e.g. com.mycompany.mygroup) must be less than 58.
// INPUTS
//   varName -- The name to use to identify this variable
//   varValue -- The value to save in this variable
// SOURCE
void agk::SaveSharedVariable( const char *varName, const char *varValue )
//****
{
	
}

//****f* Extras/Shared Variables/LoadSharedVariable
// FUNCTION
//   Loads a variable that was saved by this app, or another app. This only works on iOS, Android, and HTML5 platforms, 
//   and apps can only share a variable if they meet certain requirements based on the platform.<br/><br/>
//
//   On iOS the apps must have the same Bundle Seed ID (also called App ID Prefix) and have the same 
//   explicit App ID up to the last dot. For example com.mycompany.mygroup.myapp1 and 
//   com.mycompany.mygroup.myapp2 would be able to share variables.<br/><br/>
//
//   On Android the apps must have the WRITE_EXTERNAL_STORAGE permission and have the same package name up 
//   to the last dot. For example com.mycompany.mygroup.myapp1 and com.mycompany.mygroup.myapp2 would be able 
//   to share variables. On Android shared variables are written to a user accessible location, so be aware 
//   that users may be able to read and/or edit the variables that you save.<br/><br/>
//
//   On HTML the apps must be hosted on the same domain, the values are stored as cookies.<br/><br/>
//
//   Variables are identified by name, so using SaveSharedVariable("username", "Alice") will set the 
//   variable called "username" to the value "Alice". This can then be read by other apps by using
//   LoadSharedVariable( "username", "" ).
//   If a variable with the given name doesn't exist then the specified default value is returned instead
// INPUTS
//   varName -- The name of the variable to retrieve
//   defaultValue -- The value to return if the variable doesn't exist
// SOURCE
char* agk::LoadSharedVariable( const char *varName, const char *defaultValue )
//****
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

//****f* Extras/Shared Variables/DeleteSharedVariable
// FUNCTION
//   Deletes a shared variable by name. If the variable doesn't exist then this will do nothing. 
//   Once the variable is deleted then <i>LoadSharedVariable</i> will return the default value for 
//   any requests to load it.
// INPUTS
//   varName -- The name of the variable to delete
// SOURCE
void agk::DeleteSharedVariable( const char *varName )
//****
{
	
}

//****f* Extras/Firebase/FirebaseSetup
// FUNCTION
//   Starts the Firebase analytics system and the tracking of some automated events. Additional events can be 
//   tracked by using <i>FirebaseLogEvent</i>.<br/>
//   You must include the google services config file that was created when you setup your Firebase project,
//   this can be added during export for Android or iOS. If you do not specify your own google services config
//   file during export then all your tracking data will go to our AGK Firebase project and be ignored. 
//   Similarly if you use these commands during broadcasting then the tracking data will go to the AGK project
//   and be ignored.
// SOURCE
void agk::FirebaseSetup()
//****
{
	
}

//****f* Extras/Firebase/FirebaseLogEvent
// FUNCTION
//   Tells Firebase to record an event that happened in your app, for example unlocking an achievement. 
//   These will appear in your Firebase project analytics page. Note that event names must not contain
//   dashes or spaces, otherwise they will fail to register.
// INPUTS
//   event_name -- The name of the event to pass to Firebase, some of these are reserved for automated events e.g. "first_open"
// SOURCE
void agk::FirebaseLogEvent( const char *event_name )
//****
{
	
}

int AGKFont::PlatformGetSystemFontPath( const uString &sFontName, uString &sOut )
{
	uString sFontNameShort = sFontName;
	sFontNameShort.Trunc( '.' );

	static const char* fontRegistryPath = "Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
	HKEY hKey;
	LONG result;
	
	// Open Windows font registry key
	result = RegOpenKeyEx( HKEY_LOCAL_MACHINE, fontRegistryPath, 0, KEY_READ, &hKey );
	if (result != ERROR_SUCCESS) return 0;

	DWORD maxValueNameSize, maxValueDataSize;
	result = RegQueryInfoKey( hKey, 0, 0, 0, 0, 0, 0, 0, &maxValueNameSize, &maxValueDataSize, 0, 0 );
	if (result != ERROR_SUCCESS) return 0;

	maxValueNameSize *= 2; // max length doesn't account for escaped characters taking up 2 bytes, so increase size to be safe
	maxValueDataSize *= 2;

	DWORD valueIndex = 0;
	char* valueName = new char[maxValueNameSize];
	unsigned char* valueData = new unsigned char[maxValueDataSize];
	DWORD valueNameSize, valueDataSize, valueType;
	*valueName = 0;

	// Look for a matching font name
	do 
	{
		valueDataSize = maxValueDataSize;
		valueNameSize = maxValueNameSize;

		result = RegEnumValue(hKey, valueIndex, valueName, &valueNameSize, 0, &valueType, valueData, &valueDataSize);

		valueIndex++;

		if (result != ERROR_SUCCESS || valueType != REG_SZ) continue;

		char *bracket = strchr( valueName, '(' );
		if ( bracket && bracket != valueName ) *(bracket-1) = 0;

		// Found a match
		if ( strcmp( sFontNameShort.GetStr(), valueName ) == 0 ) 
		{
			break;
		}
		*valueName = 0;
	} while (result != ERROR_NO_MORE_ITEMS);

	

	RegCloseKey(hKey);

	if ( *valueName == 0 ) 
	{
		delete[] valueName;
		delete[] valueData;
		return 0;
	}

	// Build full font file path
	char winDir[MAX_PATH];
	GetWindowsDirectory(winDir, MAX_PATH);
	strcat( winDir, "\\Fonts\\" );
	strcat( winDir, (char*)valueData );

	delete[] valueName;
	delete[] valueData;

	sOut.SetStr( winDir );
	return 1;
}


// AR commands

//****f* Extras/AR/ARSetup
// FUNCTION
//   Only available on iOS and Android. This command sets up the AR features of the device, if available. Use the
//   <i>ARGetStatus</i> command to check if this was successful. This command must be called before any other AR
//   commands, if the setup fails you may call this command again to attempt setup again.<br/>
//   <br/>
//   Setting up AR will take control of the 3D camera, including the projection matrix, so any changes you make 
//   to the camera position, rotation, FOV, will be overwritten by the AR system when <i>Render</i> or Sync are called. 
//   However you can still control the camera near and far values with <i>SetCameraRange</i> which will persist 
//   through the AR system.<br/>
//   <br/>
//   When using AR and placing or sizing 3D objects then the coordinate system is 1 AGK unit equals 1 meter.
// SOURCE
void agk::ARSetup()
//****
{

}

//****f* Extras/AR/ARGetStatus
// FUNCTION
//   Returns the current state of the AR setup process. 0=ARSetup not yet called,
//   -1=AR not available on this device, or setup failed, -2=User rejected ARCore install, 1=ARCore install in 
//   progress, 2=AR setup successful.
// SOURCE
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

//****f* Extras/AR/ARDestroy
// FUNCTION
//   Destroys the AR session and any resources allocated during setup. After you 
//   can not call any AR commands, except <i>ARGetStatus</i> and <i>ARSetup</i>. You may call <i>ARSetup</i> to 
//   recreate an AR session. There is no limit on the number of times you can create and destroy the AR session.</br>
//   </br>
//   Currently on Android this command may take up to 5 seconds, apparently this will be improved ina future version 
//   of Google's ARCore.
// SOURCE
void agk::ARDestroy()
//****
{
	
}

//****f* Extras/AR/ARControlCamera
// FUNCTION
//   This is an optional command that will reset the camera to the AR assigned values. This could be useful if you
//   change the camera position or rotation but then want to know the AR values for these settings. You do not need
//   to call this command as the values will be reset to the AR values by <i>Render</i> or <i>Sync</i> anyway.
// SOURCE
void agk::ARControlCamera()
//****
{
	
}

//****f* Extras/AR/ARDrawBackground
// FUNCTION
//   You must call this command if you want the AR camera feed to be drawn to the screen. If you do not then the 
//   camera will still track as if it were moving around the real world but the camera feed will not be seen inside 
//   the app. This may be useful if you want to render a purely virtual reality but still use the AR camera tracking
//   functionality. The best time to call this command is just before calling <i>Render</i> or <i>Sync</i>.
// SOURCE
void agk::ARDrawBackground()
//****
{
	
}

//****f* Extras/AR/ARSetPlaneDetectionMode
// FUNCTION
//   Sets the plane detection mode for AR, by default this is turned on. Detected planes can be discovered with 
//   the <i>ARGetPlanes</i> command.
// INPUTS
//   mode -- 1=Turn plane detection on, 0=Turn plane detection off
// SOURCE
void agk::ARSetPlaneDetectionMode( int mode )
//****
{
	
}

//****f* Extras/AR/ARSetLightEstimationMode
// FUNCTION
//   Sets the light estimation mode for AR, by default this is turned on. This attempts to work out the ambient 
//   light in the real world so you can light your virtual objects correctly.
// INPUTS
//   mode -- 1=Turn light estimation on, 0=Turn light estimation off
// SOURCE
void agk::ARSetLightEstimationMode( int mode )
//****
{

}

//****f* Extras/AR/ARGetLightEstimate
// FUNCTION
//   Returns the estimated light value for the current frame, this will be a value between 0.0 and 1.0.
//   The best use of this value is to set the ambient light color with <i>SetAmbientColor</i> with a 
//   value of 1.0 corresponding to a color value of 255,255,255.
// SOURCE
float agk::ARGetLightEstimate()
//****
{
	return 0;
}

//****f* Extras/AR/ARHitTest
// FUNCTION
//   Casts a ray into the real world scene and detects any hits with planes or tracking points detected
//   in the world. Returns the number of hits detected, the hits will be ordered with the closest first.
//   The coordinates should be in screen space, for example the values returned from <i>GetPointerX</i>
//   and <i>GetPointerY</i>. When you are finished checking the results of this hit test you should call 
//   <i>ARHitTestFinish</i> to release any used resources. If not then this will be done automatically 
//   when you next call ARHitTest.
// INPUTS
//   screenX -- The X component of the screen coordinate
//   screenY -- The Y component of the screen coordinate
// SOURCE
int agk::ARHitTest( float screenX, float screenY )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetHitTestX
// FUNCTION
//   Returns the X component of the given hit test result in 3D space. Hit test results are ordered with 
//   the closest first, starting at index 1. This point should only be used for a short time, for example
//   to display a particle effect, as it will drift relative to the real world. If you plan to use this 
//   3D point to position a long lived object in the world then you should create an anchor from this hit 
//   test result instead so that it doesn't drift.
// INPUTS
//   index -- The index of the hit test result to check, starting with index 1.
// SOURCE
float agk::ARGetHitTestX( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetHitTestY
// FUNCTION
//   Returns the Y component of the given hit test result in 3D space. Hit test results are ordered with 
//   the closest first, starting at index 1. This point should only be used for a short time, for example
//   to display a particle effect, as it will drift relative to the real world. If you plan to use this 
//   3D point to position a long lived object in the world then you should create an anchor from this hit 
//   test result instead so that it doesn't drift.
// INPUTS
//   index -- The index of the hit test result to check, starting with index 1.
// SOURCE
float agk::ARGetHitTestY( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetHitTestZ
// FUNCTION
//   Returns the Z component of the given hit test result in 3D space. Hit test results are ordered with 
//   the closest first, starting at index 1. This point should only be used for a short time, for example
//   to display a particle effect, as it will drift relative to the real world. If you plan to use this 
//   3D point to position a long lived object in the world then you should create an anchor from this hit 
//   test result instead so that it doesn't drift.
// INPUTS
//   index -- The index of the hit test result to check, starting with index 1.
// SOURCE
float agk::ARGetHitTestZ( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetHitTestNormalX
// FUNCTION
//   Returns the X component of the normal of the given hit test result in 3D space. Hit test results are 
//   ordered with the closest first, starting at index 1. For planes on the ground the normal will point 
//   straight up in 3D space, for points detected on walls or other objects the normal will point away 
//   from the detected surface.
// INPUTS
//   index -- The index of the hit test result to check, starting with index 1.
// SOURCE
float agk::ARGetHitTestNormalX( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetHitTestNormalY
// FUNCTION
//   Returns the Y component of the normal of the given hit test result in 3D space. Hit test results are 
//   ordered with the closest first, starting at index 1. For planes on the ground the normal will point 
//   straight up in 3D space, for points detected on walls or other objects the normal will point away 
//   from the detected surface.
// INPUTS
//   index -- The index of the hit test result to check, starting with index 1.
// SOURCE
float agk::ARGetHitTestNormalY( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetHitTestNormalZ
// FUNCTION
//   Returns the Z component of the normal of the given hit test result in 3D space. Hit test results are 
//   ordered with the closest first, starting at index 1. For planes on the ground the normal will point 
//   straight up in 3D space, for points detected on walls or other objects the normal will point away 
//   from the detected surface.
// INPUTS
//   index -- The index of the hit test result to check, starting with index 1.
// SOURCE
float agk::ARGetHitTestNormalZ( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetHitTestType
// FUNCTION
//   Returns the type of surface detected, 1=plane, usually horizontal, 0=point, usually a wall or other 
//   object, -1=unknown.
// INPUTS
//   index -- The index of the hit test result to check, starting with index 1.
// SOURCE
int agk::ARGetHitTestType( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARHitTestFinish
// FUNCTION
//   Cleans up the resources used during hit testing, this should be called when you are done checking 
//   the hit test results. If you do not call it then it will be called automatically when you perform
//   another hit test or destroy the AR session.
// SOURCE
void agk::ARHitTestFinish()
//****
{
	
}

//****f* Extras/AR/ARGetPlanes
// FUNCTION
//   Queries the AR session for all the planes currently being tracked in the world, these are detected
//   over time if <i>ARSetPlaneDetectionMode</i> is turned on. Returns the number of planes found. This
//   can be used to show the detected planes to the user by creating some plane objects with 
//   <i>CreateObjectPlane</i> and placing them at the given position, with the given rotation, and size.
//   When you are done checking the results you should call <i>ARGetPlanesFinish</i> to clean up any 
//   resources used in the check.
// INPUTS
//   reserved -- reserved for future use, must be 0
// SOURCE
int agk::ARGetPlanes( int reserved )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetPlaneX
// FUNCTION
//   Returns the X component of the center of the given plane. This point may drift over time relative to 
//   the real world scene, so it should be checked and updated regularly if you are using it to show a 
//   visible representation of the plane.
// INPUTS
//   index -- The index of the plane to check, starting with index 1
// SOURCE
float agk::ARGetPlaneX( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetPlaneY
// FUNCTION
//   Returns the Y component of the center of the given plane. This point may drift over time relative to 
//   the real world scene, so it should be checked and updated regularly if you are using it to show a 
//   visible representation of the plane.
// INPUTS
//   index -- The index of the plane to check, starting with index 1
// SOURCE
float agk::ARGetPlaneY( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetPlaneZ
// FUNCTION
//   Returns the Z component of the center of the given plane. This point may drift over time relative to 
//   the real world scene, so it should be checked and updated regularly if you are using it to show a 
//   visible representation of the plane.
// INPUTS
//   index -- The index of the plane to check, starting with index 1
// SOURCE
float agk::ARGetPlaneZ( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetPlaneAngleX
// FUNCTION
//   Returns the X component of the rotation of the given plane. This can be used to rotate a 3D object
//   to match a visual representation of the detected plane.
// INPUTS
//   index -- The index of the plane to check, starting with index 1
// SOURCE
float agk::ARGetPlaneAngleX( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetPlaneAngleY
// FUNCTION
//   Returns the Y component of the rotation of the given plane. This can be used to rotate a 3D object
//   to match a visual representation of the detected plane.
// INPUTS
//   index -- The index of the plane to check, starting with index 1
// SOURCE
float agk::ARGetPlaneAngleY( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetPlaneAngleZ
// FUNCTION
//   Returns the Z component of the rotation of the given plane. This can be used to rotate a 3D object
//   to match a visual representation of the detected plane.
// INPUTS
//   index -- The index of the plane to check, starting with index 1
// SOURCE
float agk::ARGetPlaneAngleZ( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetPlaneSizeX
// FUNCTION
//   Returns the X component of the size of the given plane. This can be used to scale a 3D object
//   to match a visual representation of the detected plane.
// INPUTS
//   index -- The index of the plane to check, starting with index 1
// SOURCE
float agk::ARGetPlaneSizeX( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetPlaneSizeZ
// FUNCTION
//   Returns the Z component of the size of the given plane. This can be used to scale a 3D object
//   to match a visual representation of the detected plane.
// INPUTS
//   index -- The index of the plane to check, starting with index 1
// SOURCE
float agk::ARGetPlaneSizeZ( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARGetPlanesFinish
// FUNCTION
//   Cleans up any resources used in the call to <i>ARGetPlanes</i>, you should call this when you 
//   are done checking the results. If not then it will be called automatically when you next call
//   <i>ARGetPlanes</i> or destroy the AR session.
// SOURCE
void agk::ARGetPlanesFinish()
//****
{
	
}

//****f* Extras/AR/ARCreateAnchorFromHitTest
// FUNCTION
//   Creates an anchor from the given hit test result which can be used to track a spot in the real 
//   world over time. Anchors automatically adjust their position to attempt to stay fixed to a point
//   detected in the real world, this allows you to place an object there without it drifting too 
//   much. Returns an ID you can use to reference this anchor later. The hit test result does not 
//   need to exist after creating an anchor from it, you can safely clean up the hit test results
//   afterwards. The anchor will persist and use some tracking resources until you specifically 
//   delete it with <i>ARDeleteAnchor</i> or you destroy the AR session.
// INPUTS
//   index -- The index of the hit test result to use, starting with index 1
// SOURCE
int agk::ARCreateAnchorFromHitTest( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARCreateAnchorFromPlane
// FUNCTION
//   Creates an anchor from the given plane's center which can be used to track a spot in the real 
//   world over time. Anchors automatically adjust their position to attempt to stay fixed to a point
//   detected in the real world, this allows you to place an object there without it drifting too 
//   much. Returns an ID you can use to reference this anchor later. The plane result does not 
//   need to exist after creating an anchor from it, you can safely call <i>ARGetPlanesFinish</i>
//   afterwards. The anchor will persist and use some tracking resources until you specifically 
//   delete it with <i>ARDeleteAnchor</i> or you destroy the AR session.
// INPUTS
//   index -- The index of the plane to use, starting with index 1
// SOURCE
int agk::ARCreateAnchorFromPlane( int index )
//****
{
	return 0;
}

//****f* Extras/AR/ARFixObjectToAnchor
// FUNCTION
//   Attaches an object to an anchor in the same way that <i>FixObjectToObject</i> works. The object's 
//   position and rotation will be added to the anchor's position and rotation. So for example an object 
//   at position 0,0,0 would be directly on top of the anchor's position, whereas if the object had a 
//   position of 0,2,0 then the object would be placed 2 meters above the anchor's position.
//   There is no limit on the number of objects that can be attached to an anchor, and objects can
//   still be attached to the object attached to the anchor. For example object 1 could be fixed
//   to the anchor, and object 2 could be fixed to object 1. To remove an object from an anchor call 
//   this command with an anchor ID of 0. The object will then be placed in the world at its given 
//   position, for example a position of 0,0,0 would place it at the origin of the world.
// INPUTS
//   objID -- The ID of the object to fix to the anchor
//   anchorID -- The ID of the anchor to use
// SOURCE
void agk::ARFixObjectToAnchor( int objID, int anchorID )
//****
{
	
}

//****f* Extras/AR/ARGetAnchorStatus
// FUNCTION
//   Returns the current tracking status of the given anchor. 0=stopped, 1=paused, 2=tracking, 
//   -1=don't know. Anchors can stop tracking if the camera loses tracking with the world, this can 
//   happen if the camera can't see enough features to work out how the scene is moving. For example
//   by looking at a blank wall. When an anchor stops or pauses tracking it maintains its current 
//   position and rotation, but this no longer correlates to the world position it was tracking, so
//   the anchor may appear to be drifting relative to the world. When the anchor resumes tracking
//   it should return to its correct world position.
// INPUTS
//   anchorID -- The ID of the anchor to check
// SOURCE
int agk::ARGetAnchorStatus( int anchorID )
//****
{
	return 0;
}

//****f* Extras/AR/ARDeleteAnchor
// FUNCTION
//   Deletes the specified anchor and frees up any resources that were being used by it. If any 
//   objects were attached to the anchor when it is deleted then they will return to an unattached
//   state and be placed at world coordinates
// INPUTS
//   anchorID -- The ID of the anchor to delete
// SOURCE
void agk::ARDeleteAnchor( int anchorID )
//****
{
	
}

//****f* Core/Misc/GetAppInstalled
// FUNCTION
//   Returns 1 if the specified app is installed and enabled on the device. For Android this should be the 
//   package name of the app, e.g. com.faceboook.katana for the Facebook App.
//   Currently this only works on Android
// INPUTS
//   packageName -- The package name of the app to check
// SOURCE
int agk::GetAppInstalled( const char *packageName )
//****
{
	return 0;
}

// SnapChat

//****f* Extras/SnapChat/SetSnapChatStickerSettings
// FUNCTION
//   Sets the properties for any sticker images that are added to future calls to <i>ShareSnapChatImage</i>.
//   The X and Y parameters should be in the range 0.0 to 1.0 for example in the X direction 1.0 is the far 
//   right of the image and 0.0 is the far left of the image.
// INPUTS
//   x -- The X position of the sticker, default 0.5
//   y -- The Y position of the sticker, default 0.5
//   width -- The width of the sticker, default 250
//   height -- The height of the sticker, default 250
//   angle -- The angle of the sticker, default 0
// SOURCE
void agk::SetSnapChatStickerSettings( float x, float y, int width, int height, float angle )
//****
{

}

//****f* Extras/SnapChat/ShareSnapChatImage
// FUNCTION
//   Sends the specified image to the SnapChat app on the current device, along with an optional sticker file, 
//   caption, and URL. The user can then modify the image using the SnapChat app and decide whether to share 
//   it with friends. If the user does not have the SnapChat app installed then the app store will be opened
//   instead on the SnapChat app page. This command will only work on Android and iOS 10 or above
// INPUTS
//   imageFile -- The path to the image you want to share
//   stickerFile -- The path to a sticker image to add to the image, can be an empty string
//   caption -- The caption to add to the image, can be an empty string
//   url -- The URL to add to the image, can be an empty string
// SOURCE
void agk::ShareSnapChatImage( const char* imageFile, const char* stickerFile, const char* caption, const char* url )
//****
{

}
