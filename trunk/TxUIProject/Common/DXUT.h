//--------------------------------------------------------------------------------------
// File: DXUT.h
//
// DirectX SDK Direct3D sample framework
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#pragma once
#ifndef DXUT_H
#define DXUT_H

//--------------------------------------------------------------------------------------
// Helper macros to build member functions that access member variables with thread safety
//--------------------------------------------------------------------------------------
#define SET_ACCESSOR( x, y )       inline void Set##y( x t )  { DXUTLock l; m_state.m_##y = t; };
#define GET_ACCESSOR( x, y )       inline x Get##y() { DXUTLock l; return m_state.m_##y; };
#define GET_SET_ACCESSOR( x, y )   SET_ACCESSOR( x, y ) GET_ACCESSOR( x, y )


#define SETP_ACCESSOR( x, y )      inline void Set##y( x* t )  { DXUTLock l; m_state.m_##y = *t; };
#define GETP_ACCESSOR( x, y )      inline x* Get##y() { DXUTLock l; return &m_state.m_##y; };
#define GETP_SETP_ACCESSOR( x, y ) SETP_ACCESSOR( x, y ) GETP_ACCESSOR( x, y )




#ifndef UNICODE
#error "The sample framework requires a Unicode build. If you are using Microsoft Visual C++ .NET, under the General tab of the project properties change the Character Set to 'Use Unicode Character Set'."
#endif
#include "DXUTmisc.h"
#include "DXUTgui.h"


extern CRITICAL_SECTION g_cs;  
extern bool g_bThreadSafe;

//--------------------------------------------------------------------------------------
// Automatically enters & leaves the CS upon object creation/deletion
//--------------------------------------------------------------------------------------
class DXUTLock
{
public:
	inline DXUTLock()  { if( g_bThreadSafe ) EnterCriticalSection( &g_cs ); }
	inline ~DXUTLock() { if( g_bThreadSafe ) LeaveCriticalSection( &g_cs ); }
};



//--------------------------------------------------------------------------------------
// Structs
//--------------------------------------------------------------------------------------

class CD3DEnumeration;
class CD3DSettingsDlg;

struct DXUTDeviceSettings
{
    UINT AdapterOrdinal;
    D3DDEVTYPE DeviceType;
    D3DFORMAT AdapterFormat;
    DWORD BehaviorFlags;
    D3DPRESENT_PARAMETERS pp;
};


//--------------------------------------------------------------------------------------
// Error codes
//--------------------------------------------------------------------------------------
#define DXUTERR_NODIRECT3D              MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0901)
#define DXUTERR_NOCOMPATIBLEDEVICES     MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0902)
#define DXUTERR_MEDIANOTFOUND           MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0903)
#define DXUTERR_NONZEROREFCOUNT         MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0904)
#define DXUTERR_CREATINGDEVICE          MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0905)
#define DXUTERR_RESETTINGDEVICE         MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0906)
#define DXUTERR_CREATINGDEVICEOBJECTS   MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0907)
#define DXUTERR_RESETTINGDEVICEOBJECTS  MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0908)
#define DXUTERR_INCORRECTVERSION        MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x0909)


//--------------------------------------------------------------------------------------
// Callback registration 
//--------------------------------------------------------------------------------------
typedef bool    (CALLBACK *LPDXUTCALLBACKISDEVICEACCEPTABLE)( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed );
typedef void    (CALLBACK *LPDXUTCALLBACKMODIFYDEVICESETTINGS)( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps );
typedef HRESULT (CALLBACK *LPDXUTCALLBACKDEVICECREATED)( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
typedef HRESULT (CALLBACK *LPDXUTCALLBACKDEVICERESET)( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
typedef void    (CALLBACK *LPDXUTCALLBACKDEVICEDESTROYED)();
typedef void    (CALLBACK *LPDXUTCALLBACKDEVICELOST)();
typedef void    (CALLBACK *LPDXUTCALLBACKFRAMEMOVE)( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime );
typedef void    (CALLBACK *LPDXUTCALLBACKFRAMERENDER)( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, WPARAM wparam);
typedef void    (CALLBACK *LPDXUTCALLBACKKEYBOARD)( UINT nChar, bool bKeyDown, bool bAltDown );
typedef void    (CALLBACK *LPDXUTCALLBACKMOUSE)( bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown, bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta, int xPos, int yPos );
typedef LRESULT (CALLBACK *LPDXUTCALLBACKMSGPROC)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing );
typedef void    (CALLBACK *LPDXUTCALLBACKTIMER)( UINT idEvent );

// Device callbacks
// void DXUTSetCallbackDeviceCreated( LPDXUTCALLBACKDEVICECREATED pCallbackDeviceCreated );
// void DXUTSetCallbackDeviceReset( LPDXUTCALLBACKDEVICERESET pCallbackDeviceReset );
// void DXUTSetCallbackDeviceLost( LPDXUTCALLBACKDEVICELOST pCallbackDeviceLost );
// void DXUTSetCallbackDeviceDestroyed( LPDXUTCALLBACKDEVICEDESTROYED pCallbackDeviceDestroyed );
// 
// // Frame callbacks
// void DXUTSetCallbackFrameMove( LPDXUTCALLBACKFRAMEMOVE pCallbackFrameMove );
// void DXUTSetCallbackFrameRender( LPDXUTCALLBACKFRAMERENDER pCallbackFrameRender , WPARAM wparam );
// 
// // Message callbacks
// void DXUTSetCallbackKeyboard( LPDXUTCALLBACKKEYBOARD pCallbackKeyboard );
// void DXUTSetCallbackMouse( LPDXUTCALLBACKMOUSE pCallbackMouse, bool bIncludeMouseMove = false );
// void DXUTSetCallbackMsgProc( LPDXUTCALLBACKMSGPROC pCallbackMsgProc );


//--------------------------------------------------------------------------------------
// Stores timer callback info
//--------------------------------------------------------------------------------------
struct DXUT_TIMER
{
	LPDXUTCALLBACKTIMER pCallbackTimer;
	float fTimeoutInSecs;
	float fCountdown;
	bool  bEnabled;
};



//--------------------------------------------------------------------------------------
//Initialization
//--------------------------------------------------------------------------------------
HRESULT DXUTInit( bool bParseCommandLine = true, bool bHandleDefaultHotkeys = true, bool bShowMsgBoxOnError = true );

// Choose either DXUTCreateWindow or DXUTSetWindow.  If using DXUTSetWindow, consider using DXUTStaticWndProc
HRESULT DXUTCreateWindow( const WCHAR* strWindowTitle = L"Direct3D Window", 
                          HINSTANCE hInstance = NULL, HICON hIcon = NULL, HMENU hMenu = NULL,
                          int x = CW_USEDEFAULT, int y = CW_USEDEFAULT );
HRESULT DXUTSetWindow( HWND hWndFocus, HWND hWndDeviceFullScreen, HWND hWndDeviceWindowed, bool bHandleMessages = true );
LRESULT CALLBACK DXUTStaticWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

// Choose either DXUTCreateDevice or DXUTSetDevice or DXUTCreateDeviceFromSettings
HRESULT DXUTCreateDevice( UINT AdapterOrdinal = D3DADAPTER_DEFAULT, bool bWindowed = true, 
                          int nSuggestedWidth = 640, int nSuggestedHeight = 480,
                          LPDXUTCALLBACKISDEVICEACCEPTABLE pCallbackIsDeviceAcceptable = NULL,
                          LPDXUTCALLBACKMODIFYDEVICESETTINGS pCallbackModifyDeviceSettings = NULL );
HRESULT DXUTCreateDeviceFromSettings( DXUTDeviceSettings* pDeviceSettings, bool bPreserveInput = false );
HRESULT DXUTSetDevice( IDirect3DDevice9* pd3dDevice );

// Choose either DXUTMainLoop or implement your own main loop 
HRESULT DXUTMainLoop( HACCEL hAccel = NULL );

// If not using DXUTMainLoop consider using DXUTRender3DEnvironment
void DXUTRender3DEnvironment(); 


//--------------------------------------------------------------------------------------
// Finding valid device settings
//--------------------------------------------------------------------------------------
enum DXUT_MATCH_TYPE
{
    DXUTMT_IGNORE_INPUT = 0,  // Use the closest valid value to a default 
    DXUTMT_PRESERVE_INPUT,    // Use input without change, but may cause no valid device to be found
    DXUTMT_CLOSEST_TO_INPUT   // Use the closest valid value to the input 
};

struct DXUTMatchOptions
{
    DXUT_MATCH_TYPE eAdapterOrdinal;
    DXUT_MATCH_TYPE eDeviceType;
    DXUT_MATCH_TYPE eWindowed;
    DXUT_MATCH_TYPE eAdapterFormat;
    DXUT_MATCH_TYPE eVertexProcessing;
    DXUT_MATCH_TYPE eResolution;
    DXUT_MATCH_TYPE eBackBufferFormat;
    DXUT_MATCH_TYPE eBackBufferCount;
    DXUT_MATCH_TYPE eMultiSample;
    DXUT_MATCH_TYPE eSwapEffect;
    DXUT_MATCH_TYPE eDepthFormat;
    DXUT_MATCH_TYPE eStencilFormat;
    DXUT_MATCH_TYPE ePresentFlags;
    DXUT_MATCH_TYPE eRefreshRate;
    DXUT_MATCH_TYPE ePresentInterval;
};

HRESULT DXUTFindValidDeviceSettings( DXUTDeviceSettings* pOut, DXUTDeviceSettings* pIn = NULL, DXUTMatchOptions* pMatchOptions = NULL );


//--------------------------------------------------------------------------------------
// Common Tasks 
//--------------------------------------------------------------------------------------
void    DXUTSetCursorSettings( bool bShowCursorWhenFullScreen, bool bClipCursorWhenFullScreen );
void    DXUTSetMultimonSettings( bool bAutoChangeAdapter );
void    DXUTSetShortcutKeySettings( bool bAllowWhenFullscreen = false, bool bAllowWhenWindowed = true ); // Controls the Windows key, and accessibility shortcut keys
void    DXUTSetConstantFrameTime( bool bConstantFrameTime, float fTimePerFrame = 0.0333f );
void    DXUTSetShowSettingsDialog( bool bShow ); 
HRESULT DXUTSetTimer( LPDXUTCALLBACKTIMER pCallbackTimer, float fTimeoutInSecs = 1.0f, UINT* pnIDEvent = NULL );
HRESULT DXUTKillTimer( UINT nIDEvent );
HRESULT DXUTToggleFullScreen();
HRESULT DXUTToggleREF();
void    DXUTPause( bool bPauseTime, bool bPauseRendering );
void    DXUTResetFrameworkState();
void    DXUTShutdown();
void DXUTCleanup3DEnvironment( bool bReleaseSettings );


//--------------------------------------------------------------------------------------
// State Retrieval  
//--------------------------------------------------------------------------------------
IDirect3D9*             DXUTGetD3DObject(); // Does not addref unlike typical Get* APIs
IDirect3DDevice9*       DXUTGetD3DDevice(); // Does not addref unlike typical Get* APIs
DXUTDeviceSettings      DXUTGetDeviceSettings(); 
D3DPRESENT_PARAMETERS   DXUTGetPresentParameters();
const D3DSURFACE_DESC*  DXUTGetBackBufferSurfaceDesc();
const D3DCAPS9*         DXUTGetDeviceCaps();
HWND                    DXUTGetHWND();
HWND                    DXUTGetHWNDFocus();
HWND                    DXUTGetHWNDDeviceFullScreen();
HWND                    DXUTGetHWNDDeviceWindowed();
const RECT&             DXUTGetWindowClientRect();
double                  DXUTGetTime();
float                   DXUTGetElapsedTime();
bool                    DXUTIsWindowed();
float                   DXUTGetFPS();
LPCWSTR                 DXUTGetWindowTitle();
LPCWSTR                 DXUTGetFrameStats();
LPCWSTR                 DXUTGetDeviceStats();
bool                    DXUTGetShowSettingsDialog(); 
bool                    DXUTIsRenderingPaused();
bool                    DXUTIsTimePaused();
int                     DXUTGetExitCode();
bool                    DXUTIsKeyDown( BYTE vKey ); // Pass a virtual-key code, ex. VK_F1, 'A', VK_RETURN, VK_LSHIFT, etc
bool                    DXUTIsMouseButtonDown( BYTE vButton ); // Pass a virtual-key code: VK_LBUTTON, VK_RBUTTON, VK_MBUTTON, VK_XBUTTON1, VK_XBUTTON2


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class DXUTState
{
protected:
	struct STATE
	{
		IDirect3D9*          m_D3D;                     // the main D3D object

		IDirect3DDevice9*    m_D3DDevice;               // the D3D rendering device
		CD3DEnumeration*     m_D3DEnumeration;          // CD3DEnumeration object

		DXUTDeviceSettings*  m_CurrentDeviceSettings;   // current device settings
		D3DSURFACE_DESC      m_BackBufferSurfaceDesc;   // back buffer surface description
		D3DCAPS9             m_Caps;                    // D3D caps for current device

		HWND  m_HWNDFocus;                  // the main app focus window
		HWND  m_HWNDDeviceFullScreen;       // the main app device window in fullscreen mode
		HWND  m_HWNDDeviceWindowed;         // the main app device window in windowed mode
		HMONITOR m_AdapterMonitor;          // the monitor of the adapter 
		double m_Time;                      // current time in seconds
		float m_ElapsedTime;                // time elapsed since last frame

		DWORD m_WinStyle;                   // window style
		RECT  m_WindowClientRect;           // client rect of HWND
		RECT  m_FullScreenClientRect;       // client rect of HWND when fullscreen
		RECT  m_WindowBoundsRect;           // window rect of HWND
		HMENU m_Menu;                       // handle to menu
		double m_LastStatsUpdateTime;       // last time the stats were updated
		DWORD m_LastStatsUpdateFrames;      // frames count since last time the stats were updated
		float m_FPS;                        // frames per second
		int   m_CurrentFrameNumber;         // the current frame number
		HHOOK m_KeyboardHook;               // handle to keyboard hook
		bool  m_AllowShortcutKeysWhenFullscreen; // if true, when fullscreen enable shortcut keys (Windows keys, StickyKeys shortcut, ToggleKeys shortcut, FilterKeys shortcut) 
		bool  m_AllowShortcutKeysWhenWindowed;   // if true, when windowed enable shortcut keys (Windows keys, StickyKeys shortcut, ToggleKeys shortcut, FilterKeys shortcut) 
		bool  m_AllowShortcutKeys;          // if true, then shortcut keys are currently disabled (Windows key, etc)
		STICKYKEYS m_StartupStickyKeys;     // StickyKey settings upon startup so they can be restored later
		TOGGLEKEYS m_StartupToggleKeys;     // ToggleKey settings upon startup so they can be restored later
		FILTERKEYS m_StartupFilterKeys;     // FilterKey settings upon startup so they can be restored later

		bool  m_HandleDefaultHotkeys;       // if true, the sample framework will handle some default hotkeys
		bool  m_ShowMsgBoxOnError;          // if true, then msgboxes are displayed upon errors
		bool  m_ClipCursorWhenFullScreen;   // if true, then the sample framework will keep the cursor from going outside the window when full screen
		bool  m_ShowCursorWhenFullScreen;   // if true, then the sample framework will show a cursor when full screen
		bool  m_ConstantFrameTime;          // if true, then elapsed frame time will always be 0.05f seconds which is good for debugging or automated capture
		float m_TimePerFrame;               // the constant time per frame in seconds, only valid if m_ConstantFrameTime==true
		bool  m_WireframeMode;              // if true, then D3DRS_FILLMODE==D3DFILL_WIREFRAME else D3DRS_FILLMODE==D3DFILL_SOLID 
		bool  m_AutoChangeAdapter;          // if true, then the adapter will automatically change if the window is different monitor
		bool  m_WindowCreatedWithDefaultPositions; // if true, then CW_USEDEFAULT was used and the window should be moved to the right adapter
		int   m_ExitCode;                   // the exit code to be returned to the command line

		bool  m_DXUTInited;                 // if true, then DXUTInit() has succeeded
		bool  m_WindowCreated;              // if true, then DXUTCreateWindow() or DXUTSetWindow() has succeeded
		bool  m_DeviceCreated;              // if true, then DXUTCreateDevice*() or DXUTSetDevice() has succeeded

		bool  m_DXUTInitCalled;             // if true, then DXUTInit() was called
		bool  m_WindowCreateCalled;         // if true, then DXUTCreateWindow() or DXUTSetWindow() was called
		bool  m_DeviceCreateCalled;         // if true, then DXUTCreateDevice*() or DXUTSetDevice() was called

		bool  m_DeviceObjectsCreated;       // if true, then DeviceCreated callback has been called (if non-NULL)
		bool  m_DeviceObjectsReset;         // if true, then DeviceReset callback has been called (if non-NULL)
		bool  m_InsideDeviceCallback;       // if true, then the framework is inside an app device callback
		bool  m_InsideMainloop;             // if true, then the framework is inside the main loop
		bool  m_Active;                     // if true, then the app is the active top level window
		bool  m_TimePaused;                 // if true, then time is paused
		bool  m_RenderingPaused;            // if true, then rendering is paused
		int   m_PauseRenderingCount;        // pause rendering ref count
		int   m_PauseTimeCount;             // pause time ref count
		bool  m_DeviceLost;                 // if true, then the device is lost and needs to be reset
		bool  m_Minimized;                  // if true, then the HWND is minimized
		bool  m_Maximized;                  // if true, then the HWND is maximized
		bool  m_IgnoreSizeChange;           // if true, the sample framework won't reset the device upon HWND size change (for internal use only)
		bool  m_NotifyOnMouseMove;          // if true, include WM_MOUSEMOVE in mousecallback

		int   m_OverrideAdapterOrdinal;     // if != -1, then override to use this adapter ordinal
		bool  m_OverrideWindowed;           // if true, then force to start windowed
		bool  m_OverrideFullScreen;         // if true, then force to start full screen
		int   m_OverrideStartX;             // if != -1, then override to this X position of the window
		int   m_OverrideStartY;             // if != -1, then override to this Y position of the window
		int   m_OverrideWidth;              // if != 0, then override to this width
		int   m_OverrideHeight;             // if != 0, then override to this height
		bool  m_OverrideForceHAL;           // if true, then force to HAL device (failing if one doesn't exist)
		bool  m_OverrideForceREF;           // if true, then force to REF device (failing if one doesn't exist)
		bool  m_OverrideForcePureHWVP;      // if true, then force to use pure HWVP (failing if device doesn't support it)
		bool  m_OverrideForceHWVP;          // if true, then force to use HWVP (failing if device doesn't support it)
		bool  m_OverrideForceSWVP;          // if true, then force to use SWVP 
		bool  m_OverrideConstantFrameTime;  // if true, then force to constant frame time
		float m_OverrideConstantTimePerFrame; // the constant time per frame in seconds if m_OverrideConstantFrameTime==true
		int   m_OverrideQuitAfterFrame;     // if != 0, then it will force the app to quit after that frame

		LPDXUTCALLBACKISDEVICEACCEPTABLE    m_IsDeviceAcceptableFunc;   // is device acceptable callback
		LPDXUTCALLBACKMODIFYDEVICESETTINGS  m_ModifyDeviceSettingsFunc; // modify device settings callback
		LPDXUTCALLBACKDEVICECREATED         m_DeviceCreatedFunc;        // device created callback
		LPDXUTCALLBACKDEVICERESET           m_DeviceResetFunc;          // device reset callback
		LPDXUTCALLBACKDEVICELOST            m_DeviceLostFunc;           // device lost callback
		LPDXUTCALLBACKDEVICEDESTROYED       m_DeviceDestroyedFunc;      // device destroyed callback
		LPDXUTCALLBACKFRAMEMOVE             m_FrameMoveFunc;            // frame move callback
		LPDXUTCALLBACKFRAMERENDER           m_FrameRenderFunc;          // frame render callback
		LPDXUTCALLBACKKEYBOARD              m_KeyboardFunc;             // keyboard callback
		LPDXUTCALLBACKMOUSE                 m_MouseFunc;                // mouse callback
		LPDXUTCALLBACKMSGPROC               m_WindowMsgFunc;            // window messages callback

		CD3DSettingsDlg*             m_D3DSettingsDlg;                  // CD3DSettings object
		bool                         m_ShowD3DSettingsDlg;              // if true, then show the D3DSettingsDlg
		bool                         m_Keys[256];                       // array of key state
		bool                         m_MouseButtons[5];                 // array of mouse states

		CGrowableArray<DXUT_TIMER>*  m_TimerList;                       // list of DXUT_TIMER structs
		WCHAR                        m_StaticFrameStats[256];           // static part of frames stats 
		WCHAR                        m_FrameStats[256];                 // frame stats (fps, width, etc)
		WCHAR                        m_DeviceStats[256];                // device stats (description, device type, etc)
		WCHAR                        m_WindowTitle[256];                // window title

		WPARAM						 m_FrameRenderWparam;
	};

	STATE m_state;

public:
	DXUTState()  { Create(); }
	~DXUTState() { Destroy(); }

	void Create()
	{
		// Make sure these are created before DXUTState so they 
		// destroyed last because DXUTState cleanup needs them
		DXUTGetGlobalDialogResourceManager();
		DXUTGetGlobalResourceCache();

		ZeroMemory( &m_state, sizeof(STATE) ); 
		g_bThreadSafe = true; 
		InitializeCriticalSection( &g_cs ); 
		m_state.m_OverrideStartX = -1; 
		m_state.m_OverrideStartY = -1; 
		m_state.m_OverrideAdapterOrdinal = -1; 
		m_state.m_AutoChangeAdapter = true; 
		m_state.m_ShowMsgBoxOnError = true;
		m_state.m_AllowShortcutKeysWhenWindowed = true;
		m_state.m_Active = true;
		m_state.m_FrameRenderWparam = NULL;
	}

	void Destroy()
	{
		DeleteCriticalSection( &g_cs ); 
	}

	// Macros to define access functions for thread safe access into m_state 
	GET_SET_ACCESSOR( IDirect3D9*, D3D );

	GET_SET_ACCESSOR( IDirect3DDevice9*, D3DDevice );
	GET_SET_ACCESSOR( CD3DEnumeration*, D3DEnumeration );   
	GET_SET_ACCESSOR( DXUTDeviceSettings*, CurrentDeviceSettings );   
	GETP_SETP_ACCESSOR( D3DSURFACE_DESC, BackBufferSurfaceDesc );
	GETP_SETP_ACCESSOR( D3DCAPS9, Caps );

	GET_SET_ACCESSOR( HWND, HWNDFocus );
	GET_SET_ACCESSOR( HWND, HWNDDeviceFullScreen );
	GET_SET_ACCESSOR( HWND, HWNDDeviceWindowed );
	GET_SET_ACCESSOR( HMONITOR, AdapterMonitor );
	GET_SET_ACCESSOR( double, Time );
	GET_SET_ACCESSOR( float, ElapsedTime );

	GET_SET_ACCESSOR( DWORD, WinStyle );
	GET_SET_ACCESSOR( const RECT &, WindowClientRect );
	GET_SET_ACCESSOR( const RECT &, FullScreenClientRect );
	GET_SET_ACCESSOR( const RECT &, WindowBoundsRect );   
	GET_SET_ACCESSOR( HMENU, Menu );   
	GET_SET_ACCESSOR( double, LastStatsUpdateTime );   
	GET_SET_ACCESSOR( DWORD, LastStatsUpdateFrames );   
	GET_SET_ACCESSOR( float, FPS );    
	GET_SET_ACCESSOR( int, CurrentFrameNumber );
	GET_SET_ACCESSOR( HHOOK, KeyboardHook );
	GET_SET_ACCESSOR( bool, AllowShortcutKeysWhenFullscreen );
	GET_SET_ACCESSOR( bool, AllowShortcutKeysWhenWindowed );
	GET_SET_ACCESSOR( bool, AllowShortcutKeys );
	GET_SET_ACCESSOR( STICKYKEYS, StartupStickyKeys );
	GET_SET_ACCESSOR( TOGGLEKEYS, StartupToggleKeys );
	GET_SET_ACCESSOR( FILTERKEYS, StartupFilterKeys );

	GET_SET_ACCESSOR( bool, HandleDefaultHotkeys );
	GET_SET_ACCESSOR( bool, ShowMsgBoxOnError );
	GET_SET_ACCESSOR( bool, ClipCursorWhenFullScreen );   
	GET_SET_ACCESSOR( bool, ShowCursorWhenFullScreen );
	GET_SET_ACCESSOR( bool, ConstantFrameTime );
	GET_SET_ACCESSOR( float, TimePerFrame );
	GET_SET_ACCESSOR( bool, WireframeMode );   
	GET_SET_ACCESSOR( bool, AutoChangeAdapter );
	GET_SET_ACCESSOR( bool, WindowCreatedWithDefaultPositions );
	GET_SET_ACCESSOR( int, ExitCode );

	GET_SET_ACCESSOR( bool, DXUTInited );
	GET_SET_ACCESSOR( bool, WindowCreated );
	GET_SET_ACCESSOR( bool, DeviceCreated );
	GET_SET_ACCESSOR( bool, DXUTInitCalled );
	GET_SET_ACCESSOR( bool, WindowCreateCalled );
	GET_SET_ACCESSOR( bool, DeviceCreateCalled );
	GET_SET_ACCESSOR( bool, InsideDeviceCallback );
	GET_SET_ACCESSOR( bool, InsideMainloop );
	GET_SET_ACCESSOR( bool, DeviceObjectsCreated );
	GET_SET_ACCESSOR( bool, DeviceObjectsReset );
	GET_SET_ACCESSOR( bool, Active );
	GET_SET_ACCESSOR( bool, RenderingPaused );
	GET_SET_ACCESSOR( bool, TimePaused );
	GET_SET_ACCESSOR( int, PauseRenderingCount );
	GET_SET_ACCESSOR( int, PauseTimeCount );
	GET_SET_ACCESSOR( bool, DeviceLost );
	GET_SET_ACCESSOR( bool, Minimized );
	GET_SET_ACCESSOR( bool, Maximized );
	GET_SET_ACCESSOR( bool, IgnoreSizeChange );   
	GET_SET_ACCESSOR( bool, NotifyOnMouseMove );

	GET_SET_ACCESSOR( int, OverrideAdapterOrdinal );
	GET_SET_ACCESSOR( bool, OverrideWindowed );
	GET_SET_ACCESSOR( bool, OverrideFullScreen );
	GET_SET_ACCESSOR( int, OverrideStartX );
	GET_SET_ACCESSOR( int, OverrideStartY );
	GET_SET_ACCESSOR( int, OverrideWidth );
	GET_SET_ACCESSOR( int, OverrideHeight );
	GET_SET_ACCESSOR( bool, OverrideForceHAL );
	GET_SET_ACCESSOR( bool, OverrideForceREF );
	GET_SET_ACCESSOR( bool, OverrideForcePureHWVP );
	GET_SET_ACCESSOR( bool, OverrideForceHWVP );
	GET_SET_ACCESSOR( bool, OverrideForceSWVP );
	GET_SET_ACCESSOR( bool, OverrideConstantFrameTime );
	GET_SET_ACCESSOR( float, OverrideConstantTimePerFrame );
	GET_SET_ACCESSOR( int, OverrideQuitAfterFrame );
	GET_SET_ACCESSOR(WPARAM, FrameRenderWparam);

	GET_SET_ACCESSOR( LPDXUTCALLBACKISDEVICEACCEPTABLE, IsDeviceAcceptableFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKMODIFYDEVICESETTINGS, ModifyDeviceSettingsFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKDEVICECREATED, DeviceCreatedFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKDEVICERESET, DeviceResetFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKDEVICELOST, DeviceLostFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKDEVICEDESTROYED, DeviceDestroyedFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKFRAMEMOVE, FrameMoveFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKFRAMERENDER, FrameRenderFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKKEYBOARD, KeyboardFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKMOUSE, MouseFunc );
	GET_SET_ACCESSOR( LPDXUTCALLBACKMSGPROC, WindowMsgFunc );

	GET_SET_ACCESSOR( CD3DSettingsDlg*, D3DSettingsDlg );   
	GET_SET_ACCESSOR( bool, ShowD3DSettingsDlg );   

	GET_SET_ACCESSOR( CGrowableArray<DXUT_TIMER>*, TimerList );   
	GET_ACCESSOR( bool*, Keys );
	GET_ACCESSOR( bool*, MouseButtons );
	GET_ACCESSOR( WCHAR*, StaticFrameStats );
	GET_ACCESSOR( WCHAR*, FrameStats );
	GET_ACCESSOR( WCHAR*, DeviceStats );    
	GET_ACCESSOR( WCHAR*, WindowTitle );
};



// DXUTState& GetDXUTState();
// //////////////////////////////////////////////////////////////////////////
// void DXUTHandlePossibleSizeChange();
// //////////////////////////////////////////////////////////////////////////



#endif




