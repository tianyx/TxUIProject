#pragma once

#include "ITxUIAgent.h"
#include "TxDrawBaseImp.h"
#include "TxResourceMgr.h"

typedef LRESULT (CALLBACK *FUNTXAGENTWNDPROC)( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

using namespace namespace_TxDrawBase;
class CTxDxAgent :public ITxUIAgent
{
public:
	CTxDxAgent(void);
	virtual ~CTxDxAgent(void);

public:

	//interface
	virtual void Init(HWND hwnd, ITxD3DToDraw* pRender);
	virtual void Release();
	virtual ITxResourceMgr* GetResourceMgr();
	
	HWND GetHwnd(){return m_hwnd;}
	FUNTXAGENTWNDPROC GetOldWndProc(){return m_pOldWndProc;}
	void Render();
	void PreRender();
public:
	//get directx dev
	IDirect3DDevice9* GetD3dDevice();
	DXUTTextureNode* GetTexture(int nIdx);

private:
	BOOL InitDxAgent(HWND hwnd);
	BOOL UnInitDxAgent();
private:
	BOOL m_bInited;
	HWND m_hwnd;
	FUNTXAGENTWNDPROC m_pOldWndProc;
	ITxD3DToDraw* m_pRender;
	CTxDrawBaseImp* m_pIDraw;
	CMutex m_mutex;
	BOOL m_bNotifyDrawing;

private:
	DXUTState m_Dxstate;
	CTxResourceMgr m_resMgr;
	CDXUTResourceCache m_resCache;
	CDXUTDialogResourceManager m_dlgResMgr;

public:
	CTxResourceMgr& GetTxResourceMgr(){return m_resMgr;}

	//////////////////////////////////////////////////////////////////////////
public:
	//func from dxut.cpp
	DXUTState&  GetDXUTState();
	void DXUTRender3DEnvironment(); 
	HRESULT DXUTSetWindow( HWND hWndFocus, HWND hWndDeviceFullScreen, HWND hWndDeviceWindowed, bool bHandleMessages = true );
	void DXUTSetCallbackFrameRender( LPDXUTCALLBACKFRAMERENDER pCallbackFrameRender , WPARAM wparam );
	void DXUTCleanup3DEnvironment( bool bReleaseSettings= true );
 	CDXUTDialogResourceManager* DXUTGetGlobalDialogResourceManager(){return &m_dlgResMgr;}
	CDXUTResourceCache& DXUTGetGlobalResourceCache(){return m_resCache;};
	void DXUTDisplayErrorMessage( HRESULT hr );
	HRESULT DXUTChangeDevice( DXUTDeviceSettings* pNewDeviceSettings, IDirect3DDevice9* pd3dDeviceFromApp, bool bForceRecreate );
	void DXUTUpdateFrameStats();
	void DXUTHandleTimers();
	void DXUTUpdateDeviceSettingsWithOverrides( DXUTDeviceSettings* pDeviceSettings );
	void DXUTAllowShortcutKeys( bool bAllowKeys );
	CD3DEnumeration* DXUTPrepareEnumerationObject( bool bEnumerate = false );
	void DXUTUpdateDeviceStats( D3DDEVTYPE DeviceType, DWORD BehaviorFlags, D3DADAPTER_IDENTIFIER9* pAdapterIdentifier );


	//--------------------------------------------------------------------------------------
	// Common Tasks 
	//-----
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

	void DXUTHandlePossibleSizeChange();
	void DXUTCheckForWindowMonitorChange();
	HRESULT DXUTGetAdapterOrdinalFromMonitor( HMONITOR hMonitor, UINT* pAdapterOrdinal );
	HRESULT DXUTInit( bool bParseCommandLine = true, bool bHandleDefaultHotkeys = true, bool bShowMsgBoxOnError = true );
	void DXUTParseCommandLine();
	bool    DXUTGetCmdParam( WCHAR*& strCmdLine, WCHAR* strFlag, int nFlagLen );
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

	//Initialization
	//--------------------------------------------------------------------------------------
	// Choose either DXUTCreateWindow or DXUTSetWindow.  If using DXUTSetWindow, consider using DXUTStaticWndProc
// 	HRESULT DXUTCreateWindow( const WCHAR* strWindowTitle = L"Direct3D Window", 
// 		HINSTANCE hInstance = NULL, HICON hIcon = NULL, HMENU hMenu = NULL,
// 		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT );
	// Choose either DXUTCreateDevice or DXUTSetDevice or DXUTCreateDeviceFromSettings
	HRESULT DXUTCreateDevice( UINT AdapterOrdinal = D3DADAPTER_DEFAULT, bool bWindowed = true, 
		int nSuggestedWidth = 640, int nSuggestedHeight = 480,
		LPDXUTCALLBACKISDEVICEACCEPTABLE pCallbackIsDeviceAcceptable = NULL,
		LPDXUTCALLBACKMODIFYDEVICESETTINGS pCallbackModifyDeviceSettings = NULL );
// 	HRESULT DXUTCreateDeviceFromSettings( DXUTDeviceSettings* pDeviceSettings, bool bPreserveInput = false );
	HRESULT DXUTSetDevice( IDirect3DDevice9* pd3dDevice );
	//////////////////////////////////////////////////////////////////////////
	HRESULT DXUTInitialize3DEnvironment();
	void    DXUTAdjustWindowStyle( HWND hWnd, bool bWindowed );
	void DXUTPrepareDevice( IDirect3DDevice9* pd3dDevice );

	void DXUTUpdateStaticFrameStats();
	HRESULT DXUTReset3DEnvironment();
	HRESULT CTxDxAgent::DXUTFindValidDeviceSettings( DXUTDeviceSettings* pOut, DXUTDeviceSettings* pIn, 
		DXUTMatchOptions* pMatchOptions );
	void DXUTBuildOptimalDeviceSettings( DXUTDeviceSettings* pOptimalDeviceSettings, 
		DXUTDeviceSettings* pDeviceSettingsIn, 
		DXUTMatchOptions* pMatchOptions );

	HRESULT DXUTFindAdapterFormat( UINT AdapterOrdinal, D3DDEVTYPE DeviceType, D3DFORMAT BackBufferFormat, 
		BOOL Windowed, D3DFORMAT* pAdapterFormat );
	HRESULT DXUTFindValidResolution( CD3DEnumDeviceSettingsCombo* pBestDeviceSettingsCombo, 
		D3DDISPLAYMODE displayModeIn, D3DDISPLAYMODE* pBestDisplayMode );
	void DXUTBuildValidDeviceSettings( DXUTDeviceSettings* pValidDeviceSettings, 
		CD3DEnumDeviceSettingsCombo* pBestDeviceSettingsCombo, 
		DXUTDeviceSettings* pDeviceSettingsIn, 
		DXUTMatchOptions* pMatchOptions );
	float DXUTRankDeviceCombo( CD3DEnumDeviceSettingsCombo* pDeviceSettingsCombo, 
		DXUTDeviceSettings* pOptimalDeviceSettings,
		D3DDISPLAYMODE* pAdapterDesktopDisplayMode );
	bool DXUTDoesDeviceComboMatchPreserveOptions( CD3DEnumDeviceSettingsCombo* pDeviceSettingsCombo, 
		DXUTDeviceSettings* pDeviceSettingsIn, 
		DXUTMatchOptions* pMatchOptions );
	UINT DXUTColorChannelBits( D3DFORMAT fmt );
	UINT DXUTDepthBits( D3DFORMAT fmt );
	UINT DXUTStencilBits( D3DFORMAT fmt );
};

//LRESULT TxWndProc(HWND, UINT, WPARAM, LPARAM);

TXDRAW_EXPORTS_API ITxUIAgent* CreateTxAgent();