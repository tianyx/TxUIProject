#include "StdAfx.h"
#include "TxDxAgent.h"
#include "DXUT.h"
#include "TxWndDrawClockMgr.h"
#include "ITxDrawBase.h"
#include "TxDxAgentMgr.h"
#include "TxDrawBaseImp.h"
#include "GlobalFunction.h"
#include "DXUT.h"

#define MIN_WINDOW_SIZE_X 200
#define MIN_WINDOW_SIZE_Y 200
#define DXUTERR_SWITCHEDTOREF     MAKE_HRESULT(SEVERITY_ERROR, FACILITY_ITF, 0x1001)

typedef IDirect3D9* (WINAPI* LPDIRECT3DCREATE9)(UINT SDKVersion);
typedef DECLSPEC_IMPORT UINT (WINAPI* LPTIMEBEGINPERIOD)( UINT uPeriod );


LRESULT CALLBACK TxAgentStaticWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
void    CALLBACK OnTxFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, WPARAM wparam );

bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
								 D3DFORMAT BackBufferFormat, bool bWindowed );

void CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps );
//////////////////////////////////////////////////////////////////////////
void    CALLBACK OnTxFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, WPARAM wparam )
{
	if (wparam)
	{
		CTxDxAgent* pAgent = (CTxDxAgent*)wparam;
		pAgent->Render();
	}
}

//////////////////////////////////////////////////////////////////////////
BOOL CTxDxAgent::InitDxAgent( HWND hwnd )
{
	m_hwnd = NULL;
	if (!::IsWindow(hwnd))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_hwnd = hwnd;
	
	CRect rcClient;
	::GetClientRect(hwnd, &rcClient);
	//
	//set dx window
	DXUTSetWindow(m_hwnd, m_hwnd, m_hwnd, false);
	//	DXUTInit( false, false, true ); // Parse the command line, handle the default hotkeys, and show msgboxes

	DXUTCreateDevice( D3DADAPTER_DEFAULT, true, rcClient.Width(), rcClient.Height(), IsDeviceAcceptable, ModifyDeviceSettings );

	
	DXUTSetCallbackFrameRender(OnTxFrameRender, (WPARAM)this);

	//
#ifdef _WIN64
	LONG_PTR nResult = SetWindowLongPtr( m_hwnd, GWLP_WNDPROC, (LONG_PTR)TxAgentStaticWndProc );
#else
	LONG_PTR nResult = SetWindowLongPtr( m_hwnd, GWLP_WNDPROC, (LONG)(LONG_PTR)TxAgentStaticWndProc );
#endif 

	DWORD dwError = GetLastError();
	if( nResult == 0 )
	{
		ASSERT(FALSE);
		return FALSE;
	}
	else
	{
		m_pOldWndProc = (FUNTXAGENTWNDPROC)nResult;
	}


	//Create Draw
	m_pIDraw = CTxDrawBaseImp::CreateDrawBase(this);

	CTxDxAgentMgr::GetTxDxAgentMgr()->RegisterAgent(*this);
	//register clock
	ST_DRAWCLOCKDATA data;
	data.hwnd = m_hwnd;
	data.nTickInternal = 100;
	CTxWndDrawClockMgr::GetTxDrawClockMgr()->RegisterDrawClock(data);
	m_bInited = TRUE;
	return TRUE;
}

CTxDxAgent::CTxDxAgent( void ):m_mutex(NULL, GenerateGUID())
{
	m_pOldWndProc = NULL;
	m_hwnd = NULL;
	m_bInited = NULL;
	m_pRender = NULL;
	m_pIDraw = NULL;
	m_bNotifyDrawing = FALSE;
}

CTxDxAgent::~CTxDxAgent( void )
{

}

BOOL CTxDxAgent::UnInitDxAgent()
{
	if (m_pIDraw)
	{
		m_pIDraw->Release();
	}
	ST_DRAWCLOCKDATA data;
	data.hwnd = m_hwnd;
	data.nTickInternal = 33;
	CTxWndDrawClockMgr::GetTxDrawClockMgr()->UnRegisterDrawClock(data);
	CTxDxAgentMgr::GetTxDxAgentMgr()->UnRegisterAgent(*this);
	if (m_pOldWndProc)
	{

#ifdef _WIN64
		LONG_PTR nResult = SetWindowLongPtr( m_hwnd, GWLP_WNDPROC, (LONG_PTR)m_pOldWndProc );
#else
		LONG_PTR nResult = SetWindowLongPtr( m_hwnd, GWLP_WNDPROC, (LONG)(LONG_PTR)m_pOldWndProc );
#endif 
	}

	DXUTCleanup3DEnvironment(true);
	m_bInited = FALSE;
	return TRUE;
}

void CTxDxAgent::Release()
{
	if (m_bInited)
	{
		UnInitDxAgent();
	}
	delete this;
}

void CTxDxAgent::Init( HWND hwnd, ITxD3DToDraw* pRender )
{
	m_pRender = pRender;
	InitDxAgent(hwnd);
}

void CTxDxAgent::Render()
{
	HRESULT hr;
	D3DXMATRIXA16 mWorld;
	D3DXMATRIXA16 mView;
	D3DXMATRIXA16 mProj;
	D3DXMATRIXA16 mWorldViewProjection;

	// Clear the render target and the zbuffer 
	IDirect3DDevice9* pd3dDevice = DXUTGetD3DDevice();
	V( pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.0f, 0) );
	// Render the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		GetTxResourceMgr().m_pStateBlock->Capture();

		pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
		pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
		pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );

		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2 );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE );

		pd3dDevice->SetVertexShader( NULL );
		pd3dDevice->SetPixelShader( NULL );

		//pd3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
		pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );

		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
		pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

		pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

		GetTxResourceMgr().m_pSprite->Begin( D3DXSPRITE_DONOTSAVESTATE );


		if (m_pRender && m_pIDraw)
		{
			m_pIDraw->SetBeginDraw();
			m_pRender->Tx3DDraw(m_pIDraw);
			m_pIDraw->SetEndDraw();
		}



		GetTxResourceMgr().m_pSprite->End();

		GetTxResourceMgr().m_pStateBlock->Apply();
		pd3dDevice->EndScene();
	}
}

IDirect3DDevice9* CTxDxAgent::GetD3dDevice()
{
	return DXUTGetD3DDevice();
}

void CTxDxAgent::PreRender()
{
	if (!m_mutex.Lock(0))
	{
		return;
	}
	DXUTRender3DEnvironment();
	m_mutex.Unlock();
	
}

LRESULT CALLBACK TxAgentStaticWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT hrRet = 0;

	CTxDxAgentMgr* pAgentMgr = CTxDxAgentMgr::GetTxDxAgentMgr();
	if (pAgentMgr)
	{
		CTxDxAgent* pAgent = pAgentMgr->FindAgent(hWnd);
		if (pAgent)
		{
			if (uMsg == MSG_TXDRAWCLOCK)
			{

				pAgent->PreRender();
			}
			else
			{
				switch (uMsg)
				{
				case WM_SIZE:
					// Pick up possible changes to window style due to maximize, etc.
					if( pAgent->DXUTIsWindowed() && pAgent->DXUTGetHWND() != NULL )
						pAgent->GetDXUTState().SetWinStyle( GetWindowLong( pAgent->DXUTGetHWND(), GWL_STYLE ) );

					if( SIZE_MINIMIZED == wParam )
					{
						if( pAgent->GetDXUTState().GetClipCursorWhenFullScreen() && !pAgent->DXUTIsWindowed() )
							ClipCursor( NULL );
						pAgent->DXUTPause( true, true ); // Pause while we're minimized
						pAgent->GetDXUTState().SetMinimized( true );
						pAgent->GetDXUTState().SetMaximized( false );
					}
					else if( SIZE_MAXIMIZED == wParam )
					{
						if( pAgent->GetDXUTState().GetMinimized() )
							pAgent->DXUTPause( false, false ); // Unpause since we're no longer minimized
						pAgent->GetDXUTState().SetMinimized( false );
						pAgent->GetDXUTState().SetMaximized( true );
						pAgent->DXUTHandlePossibleSizeChange();
					}
					else if( SIZE_RESTORED == wParam )
					{
						if( pAgent->GetDXUTState().GetMaximized() )
						{
							pAgent->GetDXUTState().SetMaximized( false );
							pAgent->DXUTHandlePossibleSizeChange();
						}
						else if( pAgent->GetDXUTState().GetMinimized() )
						{
							pAgent->DXUTPause( false, false ); // Unpause since we're no longer minimized
							pAgent->GetDXUTState().SetMinimized( false );
							pAgent->DXUTHandlePossibleSizeChange();
						}
						else
						{
							// If we're neither maximized nor minimized, the window size 
							// is changing by the user dragging the window edges.  In this 
							// case, we don't reset the device yet -- we wait until the 
							// user stops dragging, and a WM_EXITSIZEMOVE message comes.
						}
					}
					break;

				case WM_GETMINMAXINFO:
					((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
					((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
					break;

				case WM_ENTERSIZEMOVE:
					// Halt frame movement while the app is sizing or moving
					pAgent->DXUTPause( true, true );
					break;

				case WM_EXITSIZEMOVE:
					pAgent->DXUTPause( false, false );
					pAgent->DXUTHandlePossibleSizeChange();
					break;

					/*	case WM_SETCURSOR:
					// Turn off Windows cursor in full screen mode
					if( !DXUTIsRenderingPaused() && !DXUTIsWindowed() )
					{
					SetCursor( NULL );
					IDirect3DDevice9* pd3dDevice = DXUTGetD3DDevice();
					if( pd3dDevice)
					pd3dDevice->ShowCursor( true );
					return true; // prevent Windows from setting cursor to window class cursor
					}
					break;*/

				case WM_MOUSEMOVE:
					if( !pAgent->DXUTIsRenderingPaused() && !pAgent->DXUTIsWindowed() )
					{
						IDirect3DDevice9* pd3dDevice = pAgent->DXUTGetD3DDevice();
						if( pd3dDevice )
						{
							POINT ptCursor;
							GetCursorPos( &ptCursor );
							pd3dDevice->SetCursorPosition( ptCursor.x, ptCursor.y, 0 );
						}
					}
					break;

				case WM_ENTERMENULOOP:
					// Pause the app when menus are displayed
					pAgent->DXUTPause( true, true );
					break;

				case WM_EXITMENULOOP:
					pAgent->DXUTPause( false, false );
					break;

				case WM_NCHITTEST:
					// Prevent the user from selecting the menu in full screen mode
					if( !pAgent->DXUTIsWindowed() )
						return HTCLIENT;
					break;

				case WM_POWERBROADCAST:
					switch( wParam )
					{
#ifndef PBT_APMQUERYSUSPEND
#define PBT_APMQUERYSUSPEND 0x0000
#endif
				case PBT_APMQUERYSUSPEND:
					// At this point, the app should save any data for open
					// network connections, files, etc., and prepare to go into
					// a suspended mode.  The app can use the MsgProc callback
					// to handle this if desired.
					return true;

#ifndef PBT_APMRESUMESUSPEND
#define PBT_APMRESUMESUSPEND 0x0007
#endif
				case PBT_APMRESUMESUSPEND:
					// At this point, the app should recover any data, network
					// connections, files, etc., and resume running from when
					// the app was suspended. The app can use the MsgProc callback
					// to handle this if desired.
					return true;
					}
					break;

				default:
					// At this point the message is still not handled.  We let the
					// CDXUTIMEEditBox's static message proc handle the msg.
					// This is because some IME messages must be handled to ensure
					// proper functionalities and the static msg proc ensures that
					// this happens even if no control has the input focus.
					// 			if( CDXUTIMEEditBox::StaticMsgProc( uMsg, wParam, lParam ) )
					// 				return 0;
					break;
				}

			}
			hrRet = CallWindowProc(pAgent->GetOldWndProc(), hWnd, uMsg, wParam, lParam);
		}
	}

	return hrRet;
}

//////////////////////////////////////////////////////////////////////////
void CTxDxAgent::DXUTCleanup3DEnvironment( bool bReleaseSettings )
{
	GetTxResourceMgr().OnLostDevice();
	GetTxResourceMgr().OnDestroyDevice();

	DXUTGetGlobalDialogResourceManager()->OnLostDevice();
	DXUTGetGlobalDialogResourceManager()->OnDestroyDevice();

	DXUTGetGlobalResourceCache().OnLostDevice();
	DXUTGetGlobalResourceCache().OnDestroyDevice();

	IDirect3DDevice9* pd3dDevice = DXUTGetD3DDevice();
	if( pd3dDevice != NULL )
	{
		// If the settings dialog exists, then call its OnLostDevice() and OnDestroyedDevice()
		CD3DSettingsDlg* pD3DSettingsDlg = GetDXUTState().GetD3DSettingsDlg();
		if( pD3DSettingsDlg )
		{
			pD3DSettingsDlg->OnLostDevice();
			pD3DSettingsDlg->OnDestroyDevice();
		}

		GetDXUTState().SetInsideDeviceCallback( true );

		GetTxResourceMgr().OnLostDevice();
		DXUTGetGlobalDialogResourceManager()->OnLostDevice();
		DXUTGetGlobalResourceCache().OnLostDevice();

		// Call the app's device lost callback
		LPDXUTCALLBACKDEVICELOST pCallbackDeviceLost = GetDXUTState().GetDeviceLostFunc();
		if( pCallbackDeviceLost != NULL )
			pCallbackDeviceLost();
		GetDXUTState().SetDeviceObjectsReset( false );

		// Call the app's device destroyed callback
		LPDXUTCALLBACKDEVICEDESTROYED pCallbackDeviceDestroyed = GetDXUTState().GetDeviceDestroyedFunc();
		if( pCallbackDeviceDestroyed != NULL )
			pCallbackDeviceDestroyed();
		GetDXUTState().SetDeviceObjectsCreated( false );

		GetDXUTState().SetInsideDeviceCallback( false );

		// Release the D3D device and in debug configs, displays a message box if there 
		// are unrelease objects.
		if( pd3dDevice )
		{
			if( pd3dDevice->Release() > 0 )  
			{
				DXUTDisplayErrorMessage( DXUTERR_NONZEROREFCOUNT );
				DXUT_ERR( L"DXUTCleanup3DEnvironment", DXUTERR_NONZEROREFCOUNT );
			}
		}
		GetDXUTState().SetD3DDevice( NULL );

		if( bReleaseSettings )
		{
			DXUTDeviceSettings* pOldDeviceSettings = GetDXUTState().GetCurrentDeviceSettings();
			SAFE_DELETE(pOldDeviceSettings);  
			GetDXUTState().SetCurrentDeviceSettings( NULL );
		}

		D3DSURFACE_DESC* pbackBufferSurfaceDesc = GetDXUTState().GetBackBufferSurfaceDesc();
		ZeroMemory( pbackBufferSurfaceDesc, sizeof(D3DSURFACE_DESC) );

		D3DCAPS9* pd3dCaps = GetDXUTState().GetCaps();
		ZeroMemory( pd3dCaps, sizeof(D3DCAPS9) );

		GetDXUTState().SetDeviceCreated( false );
	}
}


void CTxDxAgent::DXUTRender3DEnvironment()
{
	HRESULT hr;

	IDirect3DDevice9* pd3dDevice = DXUTGetD3DDevice();
	if( NULL == pd3dDevice )
		return;

	if( GetDXUTState().GetDeviceLost() || DXUTIsRenderingPaused() )
	{
		// Window is minimized or paused so yield 
		// CPU time to other processes
		Sleep( 100 ); 
	}

	if( !GetDXUTState().GetActive() )
	{
		// Window is not in focus so yield CPU time to other processes
		Sleep( 20 );
	}

	if( GetDXUTState().GetDeviceLost() && !GetDXUTState().GetRenderingPaused() )
	{
		// Test the cooperative level to see if it's okay to render
		if( FAILED( hr = pd3dDevice->TestCooperativeLevel() ) )
		{
			if( D3DERR_DEVICELOST == hr )
			{
				// The device has been lost but cannot be reset at this time.  
				// So wait until it can be reset.
				Sleep( 50 );
				return;
			}

			// If we are windowed, read the desktop format and 
			// ensure that the Direct3D device is using the same format 
			// since the user could have changed the desktop bitdepth 
			if( DXUTIsWindowed() )
			{
				D3DDISPLAYMODE adapterDesktopDisplayMode;
				IDirect3D9* pD3D = DXUTGetD3DObject();
				DXUTDeviceSettings* pDeviceSettings = GetDXUTState().GetCurrentDeviceSettings();
				pD3D->GetAdapterDisplayMode( pDeviceSettings->AdapterOrdinal, &adapterDesktopDisplayMode );
				if( pDeviceSettings->AdapterFormat != adapterDesktopDisplayMode.Format )
				{
					DXUTMatchOptions matchOptions;
					matchOptions.eAdapterOrdinal     = DXUTMT_PRESERVE_INPUT;
					matchOptions.eDeviceType         = DXUTMT_PRESERVE_INPUT;
					matchOptions.eWindowed           = DXUTMT_PRESERVE_INPUT;
					matchOptions.eAdapterFormat      = DXUTMT_PRESERVE_INPUT;
					matchOptions.eVertexProcessing   = DXUTMT_CLOSEST_TO_INPUT;
					matchOptions.eResolution         = DXUTMT_CLOSEST_TO_INPUT;
					matchOptions.eBackBufferFormat   = DXUTMT_CLOSEST_TO_INPUT;
					matchOptions.eBackBufferCount    = DXUTMT_CLOSEST_TO_INPUT;
					matchOptions.eMultiSample        = DXUTMT_CLOSEST_TO_INPUT;
					matchOptions.eSwapEffect         = DXUTMT_CLOSEST_TO_INPUT;
					matchOptions.eDepthFormat        = DXUTMT_CLOSEST_TO_INPUT;
					matchOptions.eStencilFormat      = DXUTMT_CLOSEST_TO_INPUT;
					matchOptions.ePresentFlags       = DXUTMT_CLOSEST_TO_INPUT;
					matchOptions.eRefreshRate        = DXUTMT_CLOSEST_TO_INPUT;
					matchOptions.ePresentInterval    = DXUTMT_CLOSEST_TO_INPUT;

					DXUTDeviceSettings deviceSettings = DXUTGetDeviceSettings();
					deviceSettings.AdapterFormat = adapterDesktopDisplayMode.Format;

					hr = DXUTFindValidDeviceSettings( &deviceSettings, &deviceSettings, &matchOptions );
					if( FAILED(hr) ) // the call will fail if no valid devices were found
					{
						DXUTDisplayErrorMessage( DXUTERR_NOCOMPATIBLEDEVICES );
						DXUTShutdown();
					}

					// Change to a Direct3D device created from the new device settings.  
					// If there is an existing device, then either reset or recreate the scene
					hr = DXUTChangeDevice( &deviceSettings, NULL, false );
					if( FAILED(hr) )  
					{
						DXUTShutdown();
					}

					return;
				}
			}

			// Try to reset the device
			if( FAILED( hr = DXUTReset3DEnvironment() ) )
			{
				if( D3DERR_DEVICELOST == hr )
				{
					// The device was lost again, so continue waiting until it can be reset.
					Sleep( 50 );
					return;
				}
				else if( DXUTERR_RESETTINGDEVICEOBJECTS == hr || 
					DXUTERR_MEDIANOTFOUND == hr )
				{
					DXUTDisplayErrorMessage( hr );
					DXUTShutdown();
					return;
				}
				else
				{
					// Reset failed, but the device wasn't lost so something bad happened, 
					// so recreate the device to try to recover
					DXUTDeviceSettings* pDeviceSettings = GetDXUTState().GetCurrentDeviceSettings();
					if( FAILED( DXUTChangeDevice( pDeviceSettings, NULL, true ) ) )
					{
						DXUTShutdown();
						return;
					}
				}
			}
		}

		GetDXUTState().SetDeviceLost( false );
	}

	// Get the app's time, in seconds. Skip rendering if no time elapsed
	double fTime        = DXUTGetGlobalTimer()->GetTime();
	float fElapsedTime  = (float) DXUTGetGlobalTimer()->GetElapsedTime();

	// Store the time for the app
	if( GetDXUTState().GetConstantFrameTime() )
	{        
		fElapsedTime = GetDXUTState().GetTimePerFrame();
		fTime     = DXUTGetTime() + fElapsedTime;
	}

	GetDXUTState().SetTime( fTime );
	GetDXUTState().SetElapsedTime( fElapsedTime );

	// Update the FPS stats
	DXUTUpdateFrameStats();

	// If the settings dialog exists and is being shown, then 
	// render it instead of rendering the app's scene
	CD3DSettingsDlg* pD3DSettingsDlg = GetDXUTState().GetD3DSettingsDlg();
	if( pD3DSettingsDlg && DXUTGetShowSettingsDialog() )
	{
		if( !GetDXUTState().GetRenderingPaused() )
		{
			// Clear the render target and the zbuffer 
			pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0x00003F3F, 1.0f, 0);

			// Render the scene
			if( SUCCEEDED( pd3dDevice->BeginScene() ) )
			{
				pD3DSettingsDlg->OnRender( fElapsedTime );
				pd3dDevice->EndScene();
			}
		}
	}
	else
	{
		DXUTHandleTimers();

		// Animate the scene by calling the app's frame move callback
		LPDXUTCALLBACKFRAMEMOVE pCallbackFrameMove = GetDXUTState().GetFrameMoveFunc();
		if( pCallbackFrameMove != NULL )
		{
			pCallbackFrameMove( pd3dDevice, fTime, fElapsedTime );
			pd3dDevice = DXUTGetD3DDevice();
			if( NULL == pd3dDevice )
				return;
		}

		if( !GetDXUTState().GetRenderingPaused() )
		{
			// Render the scene by calling the app's render callback
			LPDXUTCALLBACKFRAMERENDER pCallbackFrameRender = GetDXUTState().GetFrameRenderFunc();
			if( pCallbackFrameRender != NULL )
			{
				pCallbackFrameRender( pd3dDevice, fTime, fElapsedTime , GetDXUTState().GetFrameRenderWparam());
				pd3dDevice = DXUTGetD3DDevice();
				if( NULL == pd3dDevice )
					return;
			}
		}
	}

	if( !GetDXUTState().GetRenderingPaused() )
	{
		// Show the frame on the primary surface.
		hr = pd3dDevice->Present( NULL, NULL, NULL, NULL );
		if( FAILED(hr) )
		{
			if( D3DERR_DEVICELOST == hr )
			{
				GetDXUTState().SetDeviceLost( true );
			}
			else if( D3DERR_DRIVERINTERNALERROR == hr )
			{
				// When D3DERR_DRIVERINTERNALERROR is returned from Present(),
				// the application can do one of the following:
				// 
				// - End, with the pop-up window saying that the application cannot continue 
				//   because of problems in the display adapter and that the user should 
				//   contact the adapter manufacturer.
				//
				// - Attempt to restart by calling IDirect3DDevice9::Reset, which is essentially the same 
				//   path as recovering from a lost device. If IDirect3DDevice9::Reset fails with 
				//   D3DERR_DRIVERINTERNALERROR, the application should end immediately with the message 
				//   that the user should contact the adapter manufacturer.
				// 
				// The framework attempts the path of resetting the device
				// 
				GetDXUTState().SetDeviceLost( true );
			}
		}
	}

	// Update current frame #
	int nFrame = GetDXUTState().GetCurrentFrameNumber();
	nFrame++;
	GetDXUTState().SetCurrentFrameNumber( nFrame );

	// Check to see if the app should shutdown due to cmdline
	if( GetDXUTState().GetOverrideQuitAfterFrame() != 0 )
	{
		if( nFrame > GetDXUTState().GetOverrideQuitAfterFrame() )
			DXUTShutdown();
	}

	return;
}

//--------------------------------------------------------------------------------------
// Sets a previously created window for the framework to use.  If DXUTInit() 
// has not already been called, it will call it with the default parameters.  
// Instead of calling this, you can call DXUTCreateWindow() to create a new window.  
//--------------------------------------------------------------------------------------
HRESULT CTxDxAgent::DXUTSetWindow( HWND hWndFocus, HWND hWndDeviceFullScreen, HWND hWndDeviceWindowed, bool bHandleMessages )
{
	HRESULT hr;

	// Not allowed to call this from inside the device callbacks
	if( GetDXUTState().GetInsideDeviceCallback() )
		return DXUT_ERR_MSGBOX( L"DXUTCreateWindow", E_FAIL );

	GetDXUTState().SetWindowCreateCalled( true );

	// To avoid confusion, we do not allow any HWND to be NULL here.  The
	// caller must pass in valid HWND for all three parameters.  The same
	// HWND may be used for more than one parameter.
	if( hWndFocus == NULL || hWndDeviceFullScreen == NULL || hWndDeviceWindowed == NULL )
		return DXUT_ERR_MSGBOX( L"DXUTSetWindow", E_INVALIDARG );

	// If subclassing the window, set the pointer to the local window procedure
	if( bHandleMessages )
	{
		// Switch window procedures
#ifdef _WIN64
//		LONG_PTR nResult = SetWindowLongPtr( hWndFocus, GWLP_WNDPROC, (LONG_PTR)DXUTStaticWndProc );
#else
//		LONG_PTR nResult = SetWindowLongPtr( hWndFocus, GWLP_WNDPROC, (LONG)(LONG_PTR)DXUTStaticWndProc );
#endif 

// 		DWORD dwError = GetLastError();
// 		if( nResult == 0 )
// 			return DXUT_ERR_MSGBOX( L"SetWindowLongPtr", HRESULT_FROM_WIN32(dwError) );
	}

	if( !GetDXUTState().GetDXUTInited() ) 
	{
		// If DXUTInit() was already called and failed, then fail.
		// DXUTInit() must first succeed for this function to succeed
		if( GetDXUTState().GetDXUTInitCalled() )
			return E_FAIL; 

		// If DXUTInit() hasn't been called, then automatically call it
		// with default params
		hr = DXUTInit();
		if( FAILED(hr) )
			return hr;
	}

	WCHAR* strCachedWindowTitle = GetDXUTState().GetWindowTitle();
	GetWindowText( hWndFocus, strCachedWindowTitle, 255 );
	strCachedWindowTitle[255] = 0;

	// Get the window's initial style
	DWORD dwWindowStyle = GetWindowLong( hWndDeviceWindowed, GWL_STYLE );
	GetDXUTState().SetWinStyle( dwWindowStyle );
	GetDXUTState().SetWindowCreatedWithDefaultPositions( false );

	// Store the client and window rects of the windowed-mode device window
	RECT rcClient;
	GetClientRect( hWndDeviceWindowed, &rcClient );
	GetDXUTState().SetWindowClientRect( rcClient );

	RECT rcWindow;
	GetWindowRect( hWndDeviceWindowed, &rcWindow );
	GetDXUTState().SetWindowBoundsRect( rcWindow );

	GetDXUTState().SetWindowCreated( true );
	GetDXUTState().SetHWNDFocus( hWndFocus );
	GetDXUTState().SetHWNDDeviceFullScreen( hWndDeviceFullScreen );
	GetDXUTState().SetHWNDDeviceWindowed( hWndDeviceWindowed );

	return S_OK;
}

DXUTState& CTxDxAgent::GetDXUTState()
{
	return m_Dxstate;
}


void CTxDxAgent::DXUTSetCallbackFrameRender( LPDXUTCALLBACKFRAMERENDER pCallbackFrameRender, WPARAM wparam ) 
{ 
	GetDXUTState().SetFrameRenderFunc( pCallbackFrameRender ); GetDXUTState().SetFrameRenderWparam(wparam);
}


//--------------------------------------------------------------------------------------
// Display an custom error msg box 
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTDisplayErrorMessage( HRESULT hr )
{
	WCHAR strBuffer[512];

	int nExitCode;
	bool bFound = true; 
	switch( hr )
	{
	case DXUTERR_NODIRECT3D:             nExitCode = 2; wcsncpy( strBuffer, L"Could not initialize Direct3D. You may want to check that the latest version of DirectX is correctly installed on your system.  Also make sure that this program was compiled with header files that match the installed DirectX DLLs.", 512 ); break;
	case DXUTERR_INCORRECTVERSION:       nExitCode = 10; wcsncpy( strBuffer, L"Incorrect version of Direct3D and/or D3DX.", 512 ); break;
	case DXUTERR_MEDIANOTFOUND:          nExitCode = 4; wcsncpy( strBuffer, L"Could not find required media. Ensure that the DirectX SDK is correctly installed.", 512 ); break;
	case DXUTERR_NONZEROREFCOUNT:        nExitCode = 5; wcsncpy( strBuffer, L"The D3D device has a non-zero reference count, meaning some objects were not released.", 512 ); break;
	case DXUTERR_CREATINGDEVICE:         nExitCode = 6; wcsncpy( strBuffer, L"Failed creating the Direct3D device.", 512 ); break;
	case DXUTERR_RESETTINGDEVICE:        nExitCode = 7; wcsncpy( strBuffer, L"Failed resetting the Direct3D device.", 512 ); break;
	case DXUTERR_CREATINGDEVICEOBJECTS:  nExitCode = 8; wcsncpy( strBuffer, L"Failed creating Direct3D device objects.", 512 ); break;
	case DXUTERR_RESETTINGDEVICEOBJECTS: nExitCode = 9; wcsncpy( strBuffer, L"Failed resetting Direct3D device objects.", 512 ); break;
	case DXUTERR_SWITCHEDTOREF:          nExitCode = 0; wcsncpy( strBuffer, L"Switching to the reference rasterizer,\na software device that implements the entire\nDirect3D feature set, but runs very slowly.", 512 ); break;
	case DXUTERR_NOCOMPATIBLEDEVICES:    
		nExitCode = 3; 
		if( GetSystemMetrics(SM_REMOTESESSION) != 0 )
			wcsncpy( strBuffer, L"Direct3D does not work over a remote session.", 512 ); 
		else
			wcsncpy( strBuffer, L"Could not find any compatible Direct3D devices.", 512 ); 
		break;
	default: bFound = false; nExitCode = 1;break;
	}   
	strBuffer[511] = 0;

	GetDXUTState().SetExitCode(nExitCode);

	bool bShowMsgBoxOnError = GetDXUTState().GetShowMsgBoxOnError();
	if( bFound && bShowMsgBoxOnError )
	{
		if( DXUTGetWindowTitle()[0] == 0 )
			MessageBox( DXUTGetHWND(), strBuffer, L"DirectX Application", MB_ICONERROR|MB_OK );
		else
			MessageBox( DXUTGetHWND(), strBuffer, DXUTGetWindowTitle(), MB_ICONERROR|MB_OK );
	}
}



//--------------------------------------------------------------------------------------
// Change to a Direct3D device created from the device settings or passed in.
// The framework will only reset if the device is similar to the previous device 
// otherwise it will cleanup the previous device (if there is one) and recreate the 
// scene using the app's device callbacks.
//--------------------------------------------------------------------------------------
HRESULT CTxDxAgent::DXUTChangeDevice( DXUTDeviceSettings* pNewDeviceSettings, IDirect3DDevice9* pd3dDeviceFromApp, bool bForceRecreate )
{
	HRESULT hr;
	DXUTDeviceSettings* pOldDeviceSettings = GetDXUTState().GetCurrentDeviceSettings();

	if( DXUTGetD3DObject() == NULL )
		return S_FALSE;

	// Make a copy of the pNewDeviceSettings on the heap
	DXUTDeviceSettings* pNewDeviceSettingsOnHeap = new DXUTDeviceSettings;
	if( pNewDeviceSettingsOnHeap == NULL )
		return E_OUTOFMEMORY;
	memcpy( pNewDeviceSettingsOnHeap, pNewDeviceSettings, sizeof(DXUTDeviceSettings) );
	pNewDeviceSettings = pNewDeviceSettingsOnHeap;

	GetDXUTState().SetCurrentDeviceSettings( pNewDeviceSettings );

	DXUTPause( true, true );

	// When a WM_SIZE message is received, it calls DXUTHandlePossibleSizeChange().
	// A WM_SIZE message might be sent when adjusting the window, so tell 
	// DXUTHandlePossibleSizeChange() to ignore size changes temporarily
	GetDXUTState().SetIgnoreSizeChange( true );

	// Update thread safety on/off depending on Direct3D device's thread safety
	g_bThreadSafe = ((pNewDeviceSettings->BehaviorFlags & D3DCREATE_MULTITHREADED) != 0 );

	// Only apply the cmd line overrides if this is the first device created
	// and DXUTSetDevice() isn't used
	if( NULL == pd3dDeviceFromApp && NULL == pOldDeviceSettings )
	{
		// Updates the device settings struct based on the cmd line args.  
		// Warning: if the device doesn't support these new settings then CreateDevice() will fail.
		DXUTUpdateDeviceSettingsWithOverrides( pNewDeviceSettings );
	}

	// If windowed, then update the window client rect and window bounds rect
	// with the new pp.BackBufferWidth & pp.BackBufferHeight
	// The window will be resized after the device is reset/creeted
	if( pNewDeviceSettings->pp.Windowed )
	{
		// Don't allow smaller than what's used in WM_GETMINMAXINFO
		// otherwise the window size will be different than the backbuffer size
		if( pNewDeviceSettings->pp.BackBufferWidth < MIN_WINDOW_SIZE_X )
			pNewDeviceSettings->pp.BackBufferWidth = MIN_WINDOW_SIZE_X;
		if( pNewDeviceSettings->pp.BackBufferHeight < MIN_WINDOW_SIZE_Y )
			pNewDeviceSettings->pp.BackBufferHeight = MIN_WINDOW_SIZE_Y;

		RECT rcWindowClient = GetDXUTState().GetWindowClientRect();
		rcWindowClient.right = pNewDeviceSettings->pp.BackBufferWidth;
		rcWindowClient.bottom = pNewDeviceSettings->pp.BackBufferHeight;
		AdjustWindowRect( &rcWindowClient, GetDXUTState().GetWinStyle(), ( GetDXUTState().GetMenu() != NULL ) ? true : false );
		SetRect( &rcWindowClient, 0, 0, rcWindowClient.right - rcWindowClient.left, rcWindowClient.bottom - rcWindowClient.top );
		GetDXUTState().SetWindowClientRect( rcWindowClient );

		RECT rcWindowBounds = GetDXUTState().GetWindowBoundsRect();
		SetRect( &rcWindowBounds, rcWindowBounds.left, rcWindowBounds.top, rcWindowBounds.left + rcWindowClient.right, rcWindowBounds.top + rcWindowClient.bottom );
		GetDXUTState().SetWindowBoundsRect( rcWindowBounds );
	}

	// Enable/disable StickKeys shortcut, ToggleKeys shortcut, FilterKeys shortcut, and Windows key 
	// to prevent accidental task switching
	if( pNewDeviceSettings->pp.Windowed )
		DXUTAllowShortcutKeys( GetDXUTState().GetAllowShortcutKeysWhenWindowed() );
	else
		DXUTAllowShortcutKeys( GetDXUTState().GetAllowShortcutKeysWhenFullscreen() );

	// If AdapterOrdinal and DeviceType are the same, we can just do a Reset().
	// If they've changed, we need to do a complete device tear down/rebuild.
	// Also only allow a reset if pd3dDevice is the same as the current device 
	if( !bForceRecreate && 
		(pd3dDeviceFromApp == NULL || pd3dDeviceFromApp == DXUTGetD3DDevice()) && 
		pOldDeviceSettings &&
		pOldDeviceSettings->AdapterOrdinal == pNewDeviceSettings->AdapterOrdinal &&
		pOldDeviceSettings->DeviceType == pNewDeviceSettings->DeviceType &&
		pOldDeviceSettings->BehaviorFlags == pNewDeviceSettings->BehaviorFlags )
	{
		// Reset the Direct3D device 
		hr = DXUTReset3DEnvironment();
		if( FAILED(hr) )
		{
			if( D3DERR_DEVICELOST == hr )
			{
				// The device is lost, so wait until it can be reset
				SAFE_DELETE( pOldDeviceSettings );
				DXUTPause( false, false );
				GetDXUTState().SetDeviceLost( true );
				return S_OK;
			}
			else if( DXUTERR_RESETTINGDEVICEOBJECTS == hr || 
				DXUTERR_MEDIANOTFOUND == hr )
			{
				SAFE_DELETE( pOldDeviceSettings );
				DXUTDisplayErrorMessage( hr );
				DXUTShutdown();
				return hr;
			}
			else // DXUTERR_RESETTINGDEVICE
			{
				// Reset failed, but the device wasn't lost so something bad happened, 
				// so recreate the device to try to recover
				GetDXUTState().SetCurrentDeviceSettings( pOldDeviceSettings );
				if( FAILED( DXUTChangeDevice( pNewDeviceSettings, pd3dDeviceFromApp, true ) ) )
				{
					SAFE_DELETE( pOldDeviceSettings );
					DXUTShutdown();
					return DXUTERR_CREATINGDEVICE;
				}
				else
				{
					SAFE_DELETE( pOldDeviceSettings );
					return S_OK;
				}
			}
		}
	}
	else
	{
		// Recreate the Direct3D device 
		if( pOldDeviceSettings )
		{
			// The adapter and device type don't match so 
			// cleanup and create the 3D device again
			DXUTCleanup3DEnvironment( false );
		}

		IDirect3DDevice9* pd3dDevice = NULL;

		// Only create a Direct3D device if one hasn't been supplied by the app
		if( pd3dDeviceFromApp == NULL )
		{
			if( NULL == pOldDeviceSettings && 
				pNewDeviceSettings->DeviceType == D3DDEVTYPE_REF && 
				!GetDXUTState().GetOverrideForceREF() )
			{
				DXUTDisplayErrorMessage( DXUTERR_SWITCHEDTOREF );
			}

			// Try to create the device with the chosen settings
			IDirect3D9* pD3D = DXUTGetD3DObject();
			hr = pD3D->CreateDevice( pNewDeviceSettings->AdapterOrdinal, pNewDeviceSettings->DeviceType, 
				DXUTGetHWNDFocus(), pNewDeviceSettings->BehaviorFlags,
				&pNewDeviceSettings->pp, &pd3dDevice );
			if( FAILED(hr) )
			{
				DXUTPause( false, false );
				DXUTDisplayErrorMessage( DXUTERR_CREATINGDEVICE );
				return DXUT_ERR( L"CreateDevice", hr );
			}
		}
		else
		{
			pd3dDeviceFromApp->AddRef();
			pd3dDevice = pd3dDeviceFromApp;
		}

		GetDXUTState().SetD3DDevice( pd3dDevice );

		// Now that the device is created, update the window and misc settings and
		// call the app's DeviceCreated and DeviceReset callbacks.
		hr = DXUTInitialize3DEnvironment();
		if( FAILED(hr) )
		{
			DXUTDisplayErrorMessage( hr );
			DXUTPause( false, false );
			return hr;
		}

		// Update the device stats text
		CD3DEnumeration* pd3dEnum = DXUTPrepareEnumerationObject();
		CD3DEnumAdapterInfo* pAdapterInfo = pd3dEnum->GetAdapterInfo( pNewDeviceSettings->AdapterOrdinal );
		DXUTUpdateDeviceStats( pNewDeviceSettings->DeviceType, 
			pNewDeviceSettings->BehaviorFlags, 
			&pAdapterInfo->AdapterIdentifier );
	}

	SAFE_DELETE( pOldDeviceSettings );

	IDirect3D9* pD3D = DXUTGetD3DObject();
	HMONITOR hAdapterMonitor = pD3D->GetAdapterMonitor( pNewDeviceSettings->AdapterOrdinal );
	GetDXUTState().SetAdapterMonitor( hAdapterMonitor );

	// When moving from full screen to windowed mode, it is important to
	// adjust the window size after resetting the device rather than
	// beforehand to ensure that you get the window size you want.  For
	// example, when switching from 640x480 full screen to windowed with
	// a 1000x600 window on a 1024x768 desktop, it is impossible to set
	// the window size to 1000x600 until after the display mode has
	// changed to 1024x768, because windows cannot be larger than the
	// desktop.
	if( pNewDeviceSettings->pp.Windowed )
	{
		RECT rcWindowBounds = GetDXUTState().GetWindowBoundsRect();

		// Resize the window
		POINT ptClient = { rcWindowBounds.left, rcWindowBounds.top };
		ScreenToClient( GetParent( DXUTGetHWNDDeviceWindowed() ), &ptClient );
		SetWindowPos( DXUTGetHWND(), HWND_NOTOPMOST,
			ptClient.x, ptClient.y,
			( rcWindowBounds.right - rcWindowBounds.left ),
			( rcWindowBounds.bottom - rcWindowBounds.top ),
			0 );

		// Update the cache of the window style
		GetDXUTState().SetWinStyle( GetDXUTState().GetWinStyle() | WS_VISIBLE );

		// Check to see if the window changed monitors
		MONITORINFO miAdapter;
		miAdapter.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo( hAdapterMonitor, &miAdapter );
		int nMonitorWidth = miAdapter.rcWork.right - miAdapter.rcWork.left;
		int nMonitorHeight = miAdapter.rcWork.bottom - miAdapter.rcWork.top;

		HMONITOR hWindowMonitor = MonitorFromWindow( DXUTGetHWND(), MONITOR_DEFAULTTOPRIMARY );
		MONITORINFO miWindow;
		miWindow.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo( hWindowMonitor, &miWindow );

		// Dragging the window does not cause a monitor change here, 
		// but changing the adapter does
		bool bMonitorChanged = ( hAdapterMonitor != hWindowMonitor );

		rcWindowBounds = GetDXUTState().GetWindowBoundsRect();
		int nWindowOffsetX = rcWindowBounds.left - miWindow.rcMonitor.left;
		int nWindowOffsetY = rcWindowBounds.top - miWindow.rcMonitor.top;
		int nWindowWidth = rcWindowBounds.right - rcWindowBounds.left;
		int nWindowHeight = rcWindowBounds.bottom - rcWindowBounds.top;

		if( GetDXUTState().GetWindowCreatedWithDefaultPositions() )
		{
			// Since the window was created with a default window position
			// center it in the work area if its outside the monitor's work area

			// Only do this the first time.
			GetDXUTState().SetWindowCreatedWithDefaultPositions( false );

			// Center window if the bottom or right of the window is outside the monitor's work area
			if( miAdapter.rcWork.left + nWindowOffsetX + nWindowWidth > miAdapter.rcWork.right )
				nWindowOffsetX = (nMonitorWidth - nWindowWidth) / 2;
			if( miAdapter.rcWork.top + nWindowOffsetY + nWindowHeight > miAdapter.rcWork.bottom )
				nWindowOffsetY = (nMonitorHeight - nWindowHeight) / 2;
		}        

		if( bMonitorChanged )
		{
			// Make sure that when changing between monitors of different resolutions, the window isn't 
			// positioned outside the monitor's client rect
			if( nWindowOffsetX + nWindowWidth > nMonitorWidth )
				nWindowOffsetX = nMonitorWidth - nWindowWidth;
			if( nWindowOffsetY + nWindowHeight > nMonitorHeight )
				nWindowOffsetY = nMonitorHeight - nWindowHeight;
		}

		// Move & show the window 
		ptClient.x = miAdapter.rcMonitor.left + nWindowOffsetX;
		ptClient.y = miAdapter.rcMonitor.top + nWindowOffsetY;
		ScreenToClient( GetParent( DXUTGetHWND() ), &ptClient );
		SetWindowPos( DXUTGetHWND(), HWND_NOTOPMOST, 
			ptClient.x, 
			ptClient.y, 
			0, 0, SWP_NOSIZE|SWP_SHOWWINDOW );

		// Save the window position & size 
		RECT rcWindowClient;
		GetClientRect( DXUTGetHWNDDeviceWindowed(), &rcWindowClient );
		GetDXUTState().SetWindowClientRect( rcWindowClient );
		GetWindowRect( DXUTGetHWNDDeviceWindowed(), &rcWindowBounds );
		GetDXUTState().SetWindowBoundsRect( rcWindowBounds );
	}
	else
	{
		RECT rcWindowClient;
		SetRect( &rcWindowClient, 0, 0, pNewDeviceSettings->pp.BackBufferWidth,pNewDeviceSettings->pp.BackBufferHeight );
		GetDXUTState().SetFullScreenClientRect( rcWindowClient );  
	}

	GetDXUTState().SetIgnoreSizeChange( false );
	DXUTPause( false, false );
	GetDXUTState().SetDeviceCreated( true );

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Updates the frames/sec stat once per second
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTUpdateFrameStats()
{
	// Keep track of the frame count
	double fLastTime = GetDXUTState().GetLastStatsUpdateTime();
	DWORD dwFrames  = GetDXUTState().GetLastStatsUpdateFrames();
	double fTime = DXUTGetGlobalTimer()->GetAbsoluteTime();
	dwFrames++;
	GetDXUTState().SetLastStatsUpdateFrames( dwFrames );

	// Update the scene stats once per second
	if( fTime - fLastTime > 1.0f )
	{
		float fFPS = (float) (dwFrames / (fTime - fLastTime));
		GetDXUTState().SetFPS( fFPS );
		GetDXUTState().SetLastStatsUpdateTime( fTime );
		GetDXUTState().SetLastStatsUpdateFrames( 0 );

		WCHAR* pstrFrameStats = GetDXUTState().GetFrameStats();
		WCHAR* pstrStaticFrameStats = GetDXUTState().GetStaticFrameStats();
		_snwprintf( pstrFrameStats, 256, pstrStaticFrameStats, fFPS );
		pstrFrameStats[255] = 0;
	}
}

//--------------------------------------------------------------------------------------
// Internal helper function to handle calling the user defined timer callbacks
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTHandleTimers()
{
	float fElapsedTime = DXUTGetElapsedTime();

	CGrowableArray<DXUT_TIMER>* pTimerList = GetDXUTState().GetTimerList();
	if( pTimerList == NULL )
		return;

	// Walk through the list of timer callbacks
	for( int i=0; i<pTimerList->GetSize(); i++ )
	{
		DXUT_TIMER DXUTTimer = pTimerList->GetAt(i);
		if( DXUTTimer.bEnabled )
		{
			DXUTTimer.fCountdown -= fElapsedTime;

			// Call the callback if count down expired
			if( DXUTTimer.fCountdown < 0 )
			{
				DXUTTimer.pCallbackTimer( i );
				DXUTTimer.fCountdown = DXUTTimer.fTimeoutInSecs;
			}
			pTimerList->SetAt(i, DXUTTimer);
		}
	}
}

//--------------------------------------------------------------------------------------
// Updates the device settings struct based on the cmd line args.  
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTUpdateDeviceSettingsWithOverrides( DXUTDeviceSettings* pDeviceSettings )
{
	if( GetDXUTState().GetOverrideAdapterOrdinal() != -1 )
		pDeviceSettings->AdapterOrdinal = GetDXUTState().GetOverrideAdapterOrdinal();

	if( GetDXUTState().GetOverrideFullScreen() )
		pDeviceSettings->pp.Windowed = false;
	if( GetDXUTState().GetOverrideWindowed() )
		pDeviceSettings->pp.Windowed = true;

	if( GetDXUTState().GetOverrideForceREF() )
		pDeviceSettings->DeviceType = D3DDEVTYPE_REF;
	else if( GetDXUTState().GetOverrideForceHAL() )
		pDeviceSettings->DeviceType = D3DDEVTYPE_HAL;

	if( GetDXUTState().GetOverrideWidth() != 0 )
		pDeviceSettings->pp.BackBufferWidth = GetDXUTState().GetOverrideWidth();
	if( GetDXUTState().GetOverrideHeight() != 0 )
		pDeviceSettings->pp.BackBufferHeight = GetDXUTState().GetOverrideHeight();

	if( GetDXUTState().GetOverrideForcePureHWVP() )
	{
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		pDeviceSettings->BehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		pDeviceSettings->BehaviorFlags |= D3DCREATE_PUREDEVICE;
	}
	else if( GetDXUTState().GetOverrideForceHWVP() )
	{
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
		pDeviceSettings->BehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else if( GetDXUTState().GetOverrideForceSWVP() )
	{
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_PUREDEVICE;
		pDeviceSettings->BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
}


//--------------------------------------------------------------------------------------
// Enables/disables Windows keys, and disables or restores the StickyKeys/ToggleKeys/FilterKeys 
// shortcut to help prevent accidental task switching
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTAllowShortcutKeys( bool bAllowKeys )
{
	GetDXUTState().SetAllowShortcutKeys( bAllowKeys );

	if( bAllowKeys )
	{
		// Restore StickyKeys/etc to original state and enable Windows key      
		STICKYKEYS sk = GetDXUTState().GetStartupStickyKeys();
		TOGGLEKEYS tk = GetDXUTState().GetStartupToggleKeys();
		FILTERKEYS fk = GetDXUTState().GetStartupFilterKeys();

		SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &sk, 0);
		SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tk, 0);
		SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fk, 0);
	}
	else
	{
		// Set low level keyboard hook if haven't already
		if( GetDXUTState().GetKeyboardHook() == NULL )
		{
			// Set the low-level hook procedure.  Only works on Windows 2000 and above
			OSVERSIONINFO OSVersionInfo;
			OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVersionInfo);
			GetVersionEx(&OSVersionInfo);
			if( OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT && OSVersionInfo.dwMajorVersion > 4 )
			{
// 				HHOOK hKeyboardHook = SetWindowsHookEx( WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0 );
// 				GetDXUTState().SetKeyboardHook( hKeyboardHook );
			}
		}

		// Disable StickyKeys/etc shortcuts but if the accessibility feature is on, 
		// then leave the settings alone as its probably being usefully used

		STICKYKEYS skOff = GetDXUTState().GetStartupStickyKeys();
		if( (skOff.dwFlags & SKF_STICKYKEYSON) == 0 )
		{
			// Disable the hotkey and the confirmation
			skOff.dwFlags &= ~SKF_HOTKEYACTIVE;
			skOff.dwFlags &= ~SKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETSTICKYKEYS, sizeof(STICKYKEYS), &skOff, 0);
		}

		TOGGLEKEYS tkOff = GetDXUTState().GetStartupToggleKeys();
		if( (tkOff.dwFlags & TKF_TOGGLEKEYSON) == 0 )
		{
			// Disable the hotkey and the confirmation
			tkOff.dwFlags &= ~TKF_HOTKEYACTIVE;
			tkOff.dwFlags &= ~TKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tkOff, 0);
		}

		FILTERKEYS fkOff = GetDXUTState().GetStartupFilterKeys();
		if( (fkOff.dwFlags & FKF_FILTERKEYSON) == 0 )
		{
			// Disable the hotkey and the confirmation
			fkOff.dwFlags &= ~FKF_HOTKEYACTIVE;
			fkOff.dwFlags &= ~FKF_CONFIRMHOTKEY;

			SystemParametersInfo(SPI_SETFILTERKEYS, sizeof(FILTERKEYS), &fkOff, 0);
		}
	}
}

//--------------------------------------------------------------------------------------
// Internal helper function to prepare the enumeration object by creating it if it 
// didn't already exist and enumerating if desired.
//--------------------------------------------------------------------------------------
CD3DEnumeration* CTxDxAgent::DXUTPrepareEnumerationObject( bool bEnumerate )
{
	// Create a new CD3DEnumeration object and enumerate all devices unless its already been done
	CD3DEnumeration* pd3dEnum = GetDXUTState().GetD3DEnumeration();
	if( pd3dEnum == NULL )
	{
		pd3dEnum = DXUTGetEnumeration(); 
		GetDXUTState().SetD3DEnumeration( pd3dEnum );

		bEnumerate = true;
	}

	if( bEnumerate )
	{
		// Enumerate for each adapter all of the supported display modes, 
		// device types, adapter formats, back buffer formats, window/full screen support, 
		// depth stencil formats, multisampling types/qualities, and presentations intervals.
		//
		// For each combination of device type (HAL/REF), adapter format, back buffer format, and
		// IsWindowed it will call the app's ConfirmDevice callback.  This allows the app
		// to reject or allow that combination based on its caps/etc.  It also allows the 
		// app to change the BehaviorFlags.  The BehaviorFlags defaults non-pure HWVP 
		// if supported otherwise it will default to SWVP, however the app can change this 
		// through the ConfirmDevice callback.
		IDirect3D9* pD3D = DXUTGetD3DObject();
		pd3dEnum->Enumerate( pD3D, GetDXUTState().GetIsDeviceAcceptableFunc() );
	}

	return pd3dEnum;
}

//--------------------------------------------------------------------------------------
// Updates the string which describes the device 
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTUpdateDeviceStats( D3DDEVTYPE DeviceType, DWORD BehaviorFlags, D3DADAPTER_IDENTIFIER9* pAdapterIdentifier )
{
	// Store device description
	WCHAR* pstrDeviceStats = GetDXUTState().GetDeviceStats();
	if( DeviceType == D3DDEVTYPE_REF )
		wcscpy( pstrDeviceStats, L"REF" );
	else if( DeviceType == D3DDEVTYPE_HAL )
		wcscpy( pstrDeviceStats, L"HAL" );
	else if( DeviceType == D3DDEVTYPE_SW )
		wcscpy( pstrDeviceStats, L"SW" );

	if( BehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING &&
		BehaviorFlags & D3DCREATE_PUREDEVICE )
	{
		if( DeviceType == D3DDEVTYPE_HAL )
			wcscat( pstrDeviceStats, L" (pure hw vp)" );
		else
			wcscat( pstrDeviceStats, L" (simulated pure hw vp)" );
	}
	else if( BehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING )
	{
		if( DeviceType == D3DDEVTYPE_HAL )
			wcscat( pstrDeviceStats, L" (hw vp)" );
		else
			wcscat( pstrDeviceStats, L" (simulated hw vp)" );
	}
	else if( BehaviorFlags & D3DCREATE_MIXED_VERTEXPROCESSING )
	{
		if( DeviceType == D3DDEVTYPE_HAL )
			wcscat( pstrDeviceStats, L" (mixed vp)" );
		else
			wcscat( pstrDeviceStats, L" (simulated mixed vp)" );
	}
	else if( BehaviorFlags & D3DCREATE_SOFTWARE_VERTEXPROCESSING )
	{
		wcscat( pstrDeviceStats, L" (sw vp)" );
	}

	if( DeviceType == D3DDEVTYPE_HAL )
	{
		// Be sure not to overflow m_strDeviceStats when appending the adapter 
		// description, since it can be long.  Note that the adapter description
		// is initially CHAR and must be converted to WCHAR.
		wcscat( pstrDeviceStats, L": " );

		const int cchDesc = sizeof(pAdapterIdentifier->Description);
		WCHAR szDescription[cchDesc];

		MultiByteToWideChar( CP_ACP, 0, pAdapterIdentifier->Description, -1, szDescription, cchDesc );
		szDescription[cchDesc-1] = 0;
		int maxAppend = 255 - lstrlen(pstrDeviceStats) - 1;
		wcsncat( pstrDeviceStats, szDescription, maxAppend );
		pstrDeviceStats[255] = 0;
	}
}

void CTxDxAgent::DXUTShutdown()
{
	HWND hWnd = DXUTGetHWND();
	if( hWnd != NULL )
		SendMessage( hWnd, WM_CLOSE, 0, 0 );

	DXUTCleanup3DEnvironment( true );

	// Restore shortcut keys (Windows key, accessibility shortcuts) to original state
	// This is important to call here if the shortcuts are disabled, 
	// because accessibility setting changes are permanent.
	// This means that if this is not done then the accessibility settings 
	// might not be the same as when the app was started. 
	// If the app crashes without restoring the settings, this is also true so it
	// would be wise to backup/restore the settings from a file so they can be 
	// restored when the crashed app is run again.
	DXUTAllowShortcutKeys( true );

	GetDXUTState().SetD3DEnumeration( NULL );

	IDirect3D9* pD3D = DXUTGetD3DObject();
	SAFE_RELEASE( pD3D );
	GetDXUTState().SetD3D( NULL );
}

void CTxDxAgent::DXUTResetFrameworkState()
{
	DXUTShutdown();
	GetDXUTState().Destroy();
	GetDXUTState().Create();
}


bool CTxDxAgent::DXUTIsWindowed()                               
{ 
	DXUTDeviceSettings* pDeviceSettings = GetDXUTState().GetCurrentDeviceSettings(); 
	if(pDeviceSettings) 
		return (pDeviceSettings->pp.Windowed != 0); 
	else 
		return false; 
}

//--------------------------------------------------------------------------------------
// External state access functions
//--------------------------------------------------------------------------------------
IDirect3D9* CTxDxAgent::DXUTGetD3DObject()                      { return GetDXUTState().GetD3D(); }        
IDirect3DDevice9* CTxDxAgent::DXUTGetD3DDevice()                { return GetDXUTState().GetD3DDevice(); }  
const D3DSURFACE_DESC* CTxDxAgent::DXUTGetBackBufferSurfaceDesc() { return GetDXUTState().GetBackBufferSurfaceDesc(); }
const D3DCAPS9* CTxDxAgent::DXUTGetDeviceCaps()                 { return GetDXUTState().GetCaps(); }
HWND CTxDxAgent::DXUTGetHWND()                                  { return DXUTIsWindowed() ? GetDXUTState().GetHWNDDeviceWindowed() : GetDXUTState().GetHWNDDeviceFullScreen(); }
HWND CTxDxAgent::DXUTGetHWNDFocus()                             { return GetDXUTState().GetHWNDFocus(); }
HWND CTxDxAgent::DXUTGetHWNDDeviceFullScreen()                  { return GetDXUTState().GetHWNDDeviceFullScreen(); }
HWND CTxDxAgent::DXUTGetHWNDDeviceWindowed()                    { return GetDXUTState().GetHWNDDeviceWindowed(); }
const RECT & CTxDxAgent::DXUTGetWindowClientRect()               { return GetDXUTState().GetWindowClientRect(); }
double CTxDxAgent::DXUTGetTime()                                { return GetDXUTState().GetTime(); }
float CTxDxAgent::DXUTGetElapsedTime()                          { return GetDXUTState().GetElapsedTime(); }
float CTxDxAgent::DXUTGetFPS()                                  { return GetDXUTState().GetFPS(); }
LPCWSTR CTxDxAgent::DXUTGetWindowTitle()                        { return GetDXUTState().GetWindowTitle(); }
LPCWSTR CTxDxAgent::DXUTGetFrameStats()                         { return GetDXUTState().GetFrameStats(); }
LPCWSTR CTxDxAgent::DXUTGetDeviceStats()                        { return GetDXUTState().GetDeviceStats(); }
bool CTxDxAgent::DXUTGetShowSettingsDialog()                    { return GetDXUTState().GetShowD3DSettingsDlg(); }
bool CTxDxAgent::DXUTIsRenderingPaused()                        { return GetDXUTState().GetPauseRenderingCount() > 0; }
bool CTxDxAgent::DXUTIsTimePaused()                             { return GetDXUTState().GetPauseTimeCount() > 0; }
int CTxDxAgent::DXUTGetExitCode()                               { return GetDXUTState().GetExitCode(); }


//--------------------------------------------------------------------------------------
// Checks if the window client rect has changed and if it has, then reset the device
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTHandlePossibleSizeChange()
{
	if( !GetDXUTState().GetDeviceCreated() || GetDXUTState().GetIgnoreSizeChange() )
		return;

	DXUTDeviceSettings* pDeviceSettings = GetDXUTState().GetCurrentDeviceSettings();
	if( false == pDeviceSettings->pp.Windowed )
		return;

	HRESULT hr = S_OK;
	RECT rcClientOld = GetDXUTState().GetWindowClientRect();

	// Update window properties
	RECT rcWindowClient;
	GetClientRect( DXUTGetHWNDDeviceWindowed(), &rcWindowClient );
	GetDXUTState().SetWindowClientRect( rcWindowClient );

	RECT rcWindowBounds;
	GetWindowRect( DXUTGetHWNDDeviceWindowed(), &rcWindowBounds );
	GetDXUTState().SetWindowBoundsRect( rcWindowBounds );

	// Check if the window client rect has changed
	if( rcClientOld.right - rcClientOld.left != rcWindowClient.right - rcWindowClient.left ||
		rcClientOld.bottom - rcClientOld.top != rcWindowClient.bottom - rcWindowClient.top )
	{
		// A new window size will require a new backbuffer
		// size, so the 3D structures must be changed accordingly.
		DXUTPause( true, true );

		pDeviceSettings = GetDXUTState().GetCurrentDeviceSettings();
		pDeviceSettings->pp.BackBufferWidth  = rcWindowClient.right - rcWindowClient.left;
		pDeviceSettings->pp.BackBufferHeight = rcWindowClient.bottom - rcWindowClient.top;

		// Reset the 3D environment
		IDirect3DDevice9* pd3dDevice = DXUTGetD3DDevice();
		if( pd3dDevice )
		{
			if( FAILED( hr = DXUTReset3DEnvironment() ) )
			{
				if( D3DERR_DEVICELOST == hr )
				{
					// The device is lost, so wait until it can be reset
					GetDXUTState().SetDeviceLost( true );
				}
				else if( DXUTERR_RESETTINGDEVICEOBJECTS == hr || 
					DXUTERR_MEDIANOTFOUND == hr )
				{
					DXUTDisplayErrorMessage( hr );
					DXUTShutdown();
					return;
				}
				else // DXUTERR_RESETTINGDEVICE
				{
					// Reset failed, but the device wasn't lost so something bad happened, 
					// so recreate the device to try to recover
					pDeviceSettings = GetDXUTState().GetCurrentDeviceSettings();
					if( FAILED( DXUTChangeDevice( pDeviceSettings, NULL, true ) ) )
					{
						DXUTShutdown();
						return;
					}
				}
			}
		}

		DXUTPause( false, false );
	}

	DXUTCheckForWindowMonitorChange();
}


//--------------------------------------------------------------------------------------
// Checks to see if the HWND changed monitors, and if it did it creates a device 
// from the monitor's adapter and recreates the scene.
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTCheckForWindowMonitorChange()
{
	// Don't do this if the user doesn't want it
	if( GetDXUTState().GetAutoChangeAdapter() == false )
		return;

	HRESULT hr;
	HMONITOR hWindowMonitor = MonitorFromWindow( DXUTGetHWND(), MONITOR_DEFAULTTOPRIMARY );
	HMONITOR hAdapterMonitor = GetDXUTState().GetAdapterMonitor();
	if( hWindowMonitor != hAdapterMonitor )
	{
		DXUTPause( true, true );

		UINT newOrdinal;
		if( SUCCEEDED( DXUTGetAdapterOrdinalFromMonitor( hWindowMonitor, &newOrdinal ) ) )
		{
			// Find the closest valid device settings with the new ordinal
			DXUTDeviceSettings deviceSettings = DXUTGetDeviceSettings();
			deviceSettings.AdapterOrdinal = newOrdinal;

			DXUTMatchOptions matchOptions;
			matchOptions.eAdapterOrdinal     = DXUTMT_PRESERVE_INPUT;
			matchOptions.eDeviceType         = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eWindowed           = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eAdapterFormat      = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eVertexProcessing   = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eResolution         = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eBackBufferFormat   = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eBackBufferCount    = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eMultiSample        = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eSwapEffect         = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eDepthFormat        = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eStencilFormat      = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.ePresentFlags       = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.eRefreshRate        = DXUTMT_CLOSEST_TO_INPUT;
			matchOptions.ePresentInterval    = DXUTMT_CLOSEST_TO_INPUT;

			hr = DXUTFindValidDeviceSettings( &deviceSettings, &deviceSettings, &matchOptions );
			if( SUCCEEDED(hr) ) 
			{
				// Create a Direct3D device using the new device settings.  
				// If there is an existing device, then it will either reset or recreate the scene.
				hr = DXUTChangeDevice( &deviceSettings, NULL, false );
				if( FAILED(hr) )
				{
					DXUTShutdown();
					DXUTPause( false, false );
					return;
				}
			}
		}

		DXUTPause( false, false );
	}    
}

//--------------------------------------------------------------------------------------
// Look for an adapter ordinal that is tied to a HMONITOR
//--------------------------------------------------------------------------------------
HRESULT CTxDxAgent::DXUTGetAdapterOrdinalFromMonitor( HMONITOR hMonitor, UINT* pAdapterOrdinal )
{
	*pAdapterOrdinal = 0;

	CD3DEnumeration* pd3dEnum = DXUTPrepareEnumerationObject();
	IDirect3D9*      pD3D     = DXUTGetD3DObject();

	CGrowableArray<CD3DEnumAdapterInfo*>* pAdapterList = pd3dEnum->GetAdapterInfoList();
	for( int iAdapter=0; iAdapter<pAdapterList->GetSize(); iAdapter++ )
	{
		CD3DEnumAdapterInfo* pAdapterInfo = pAdapterList->GetAt(iAdapter);
		HMONITOR hAdapterMonitor = pD3D->GetAdapterMonitor( pAdapterInfo->AdapterOrdinal );
		if( hAdapterMonitor == hMonitor )
		{
			*pAdapterOrdinal = pAdapterInfo->AdapterOrdinal;
			return S_OK;
		}
	}

	return E_FAIL;
}

//--------------------------------------------------------------------------------------
// Optionally parses the command line and sets if default hotkeys are handled
//
//       Possible command line parameters are:
//          -adapter:#              forces app to use this adapter # (fails if the adapter doesn't exist)
//          -windowed               forces app to start windowed
//          -fullscreen             forces app to start full screen
//          -forcehal               forces app to use HAL (fails if HAL doesn't exist)
//          -forceref               forces app to use REF (fails if REF doesn't exist)
//          -forcepurehwvp          forces app to use pure HWVP (fails if device doesn't support it)
//          -forcehwvp              forces app to use HWVP (fails if device doesn't support it)
//          -forceswvp              forces app to use SWVP 
//          -width:#                forces app to use # for width. for full screen, it will pick the closest possible supported mode
//          -height:#               forces app to use # for height. for full screen, it will pick the closest possible supported mode
//          -startx:#               forces app to use # for the x coord of the window position for windowed mode
//          -starty:#               forces app to use # for the y coord of the window position for windowed mode
//          -constantframetime:#    forces app to use constant frame time, where # is the time/frame in seconds
//          -quitafterframe:x       forces app to quit after # frames
//          -noerrormsgboxes        prevents the display of message boxes generated by the framework so the application can be run without user interaction
//
//      Hotkeys handled by default are:
//          ESC                 app exits
//          Alt-Enter           toggle between full screen & windowed
//          F2                  device selection dialog
//          F3                  toggle HAL/REF
//          F8                  toggle wire-frame mode
//          Pause               pauses time
//--------------------------------------------------------------------------------------
HRESULT CTxDxAgent::DXUTInit( bool bParseCommandLine, bool bHandleDefaultHotkeys, bool bShowMsgBoxOnError )
{
	GetDXUTState().SetDXUTInitCalled( true );

	// Not always needed, but lets the app create GDI dialogs
	InitCommonControls();

	// Save the current sticky/toggle/filter key settings so DXUT can restore them later
	STICKYKEYS sk = {sizeof(STICKYKEYS), 0};
	SystemParametersInfo(SPI_GETSTICKYKEYS, sizeof(STICKYKEYS), &sk, 0);
	GetDXUTState().SetStartupStickyKeys( sk );

	TOGGLEKEYS tk = {sizeof(TOGGLEKEYS), 0};
	SystemParametersInfo(SPI_GETTOGGLEKEYS, sizeof(TOGGLEKEYS), &tk, 0);
	GetDXUTState().SetStartupToggleKeys( tk );

	FILTERKEYS fk = {sizeof(FILTERKEYS), 0};
	SystemParametersInfo(SPI_GETFILTERKEYS, sizeof(FILTERKEYS), &fk, 0);
	GetDXUTState().SetStartupFilterKeys( fk );

	// Increase the accuracy of Sleep() without needing to link to winmm.lib
	WCHAR wszPath[MAX_PATH+1];
	if( !::GetSystemDirectory( wszPath, MAX_PATH+1 ) )
		return E_FAIL;
	lstrcatW( wszPath, L"\\winmm.dll" );
	HINSTANCE hInstWinMM = LoadLibrary( wszPath );
	if( hInstWinMM != NULL ) 
	{
		LPTIMEBEGINPERIOD pTimeBeginPeriod = (LPTIMEBEGINPERIOD)GetProcAddress( hInstWinMM, "timeBeginPeriod" );
		if( NULL != pTimeBeginPeriod )
			pTimeBeginPeriod(1);
	}
	FreeLibrary(hInstWinMM);

	GetDXUTState().SetShowMsgBoxOnError( bShowMsgBoxOnError );
	GetDXUTState().SetHandleDefaultHotkeys( bHandleDefaultHotkeys );

	if( bParseCommandLine )
		DXUTParseCommandLine();

	// Verify D3DX version
	if( !D3DXCheckVersion( D3D_SDK_VERSION, D3DX_SDK_VERSION ) )
	{
		DXUTDisplayErrorMessage( DXUTERR_INCORRECTVERSION );
		return DXUT_ERR( L"D3DXCheckVersion", DXUTERR_INCORRECTVERSION );
	}

	// Create a Direct3D object if one has not already been created
	IDirect3D9* pD3D = DXUTGetD3DObject();
	if( pD3D == NULL )
	{
		// This may fail if DirectX 9 isn't installed
		// This may fail if the DirectX headers are out of sync with the installed DirectX DLLs
		pD3D = DXUT_Dynamic_Direct3DCreate9( D3D_SDK_VERSION );
		GetDXUTState().SetD3D( pD3D );
	}

	if( pD3D == NULL )
	{
		// If still NULL, then something went wrong
		DXUTDisplayErrorMessage( DXUTERR_NODIRECT3D );
		return DXUT_ERR( L"Direct3DCreate9", DXUTERR_NODIRECT3D );
	}

	// Reset the timer
	DXUTGetGlobalTimer()->Reset();

	GetDXUTState().SetDXUTInited( true );

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Parses the command line for parameters.  See DXUTInit() for list 
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTParseCommandLine()
{
	WCHAR* strCmdLine = GetCommandLine();

	// Skip past program name (first token in command line).
	if (*strCmdLine == L'"')  // Check for and handle quoted program name
	{
		strCmdLine++;

		// Skip over until another double-quote or a null 
		while (*strCmdLine && (*strCmdLine != L'"'))
			strCmdLine++;

		// Skip over double-quote
		if (*strCmdLine == L'"')            
			strCmdLine++;    
	}
	else   
	{
		// First token wasn't a quote
		while (*strCmdLine > L' ')
			strCmdLine++;
	}

	for(;;)
	{
		// Skip past any white space preceding the next token
		while (*strCmdLine && (*strCmdLine <= L' '))
			strCmdLine++;
		if( *strCmdLine == 0 )
			break;

		WCHAR strFlag[256];
		int nFlagLen = 0;

		// Skip past the flag marker
		if( *strCmdLine == L'/' ||
			*strCmdLine == L'-' )
			strCmdLine++;

		// Compare the first N letters w/o regard to case
		wcscpy( strFlag, L"adapter" ); nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			if( DXUTGetCmdParam( strCmdLine, strFlag, nFlagLen ) )
			{
				int nAdapter = _wtoi(strFlag);
				GetDXUTState().SetOverrideAdapterOrdinal( nAdapter );
			}
			continue;
		}

		wcscpy( strFlag, L"windowed" ); nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			GetDXUTState().SetOverrideWindowed( true );
			strCmdLine += nFlagLen;
			continue;
		}

		wcscpy( strFlag, L"fullscreen" ); nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			GetDXUTState().SetOverrideFullScreen( true );
			strCmdLine += nFlagLen;
			continue;
		}

		wcscpy( strFlag, L"forcehal" ); nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			GetDXUTState().SetOverrideForceHAL( true );
			strCmdLine += nFlagLen;
			continue;
		}

		wcscpy( strFlag, L"forceref" ); nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			GetDXUTState().SetOverrideForceREF( true );
			strCmdLine += nFlagLen;
			continue;
		}

		wcscpy( strFlag, L"forcepurehwvp" ); nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			GetDXUTState().SetOverrideForcePureHWVP( true );
			strCmdLine += nFlagLen;
			continue;
		}

		wcscpy( strFlag, L"forcehwvp" ); nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			GetDXUTState().SetOverrideForceHWVP( true );
			strCmdLine += nFlagLen;
			continue;
		}

		wcscpy( strFlag, L"forceswvp" ); nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			GetDXUTState().SetOverrideForceSWVP( true );
			strCmdLine += nFlagLen;
			continue;
		}

		wcscpy( strFlag, L"width" ); nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			if( DXUTGetCmdParam( strCmdLine, strFlag, nFlagLen ) )
			{
				int nWidth = _wtoi(strFlag);
				GetDXUTState().SetOverrideWidth( nWidth );
			}
			continue;
		}

		wcscpy( strFlag, L"height" ); nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			if( DXUTGetCmdParam( strCmdLine, strFlag, nFlagLen ) )
			{
				int nHeight = _wtoi(strFlag);
				GetDXUTState().SetOverrideHeight( nHeight );
			}
			continue;
		}

		wcscpy( strFlag, L"startx" );
		nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			if( DXUTGetCmdParam( strCmdLine, strFlag, nFlagLen ) )
			{
				int nX = _wtoi(strFlag);
				GetDXUTState().SetOverrideStartX( nX );
			}
			continue;
		}

		wcscpy( strFlag, L"starty" );
		nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			if( DXUTGetCmdParam( strCmdLine, strFlag, nFlagLen ) )
			{
				int nY = _wtoi(strFlag);
				GetDXUTState().SetOverrideStartY( nY );
			}
			continue;
		}

		wcscpy( strFlag, L"constantframetime" ); nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			float fTimePerFrame;
			if( DXUTGetCmdParam( strCmdLine, strFlag, nFlagLen ) )
				fTimePerFrame = (float)wcstod( strFlag, NULL );
			else
				fTimePerFrame = 0.0333f;
			GetDXUTState().SetOverrideConstantFrameTime( true );
			GetDXUTState().SetOverrideConstantTimePerFrame( fTimePerFrame );
			DXUTSetConstantFrameTime( true, fTimePerFrame );
			continue;
		}

		wcscpy( strFlag, L"quitafterframe" ); nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			if( DXUTGetCmdParam( strCmdLine, strFlag, nFlagLen ) )
			{
				int nFrame = _wtoi(strFlag);
				GetDXUTState().SetOverrideQuitAfterFrame( nFrame );
			}
			continue;
		}      

		wcscpy( strFlag, L"noerrormsgboxes" ); nFlagLen = (int) wcslen(strFlag);
		if( _wcsnicmp( strCmdLine, strFlag, nFlagLen ) == 0 )
		{
			GetDXUTState().SetShowMsgBoxOnError( false );
			strCmdLine += nFlagLen;
			continue;
		}        

		// Unrecognized flag
		wcsncpy( strFlag, strCmdLine, 256 ); strFlag[255] = 0; 
		WCHAR* strSpace = strFlag;
		while (*strSpace && (*strSpace > L' '))
			strSpace++;
		*strSpace = 0;

		DXUTOutputDebugString( L"Unrecognized flag: %s", strFlag );
		strCmdLine += wcslen(strFlag);
	}
}

//--------------------------------------------------------------------------------------
// Helper function for DXUTParseCommandLine.  Updates strCmdLine and strFlag 
//      Example: if strCmdLine=="-width:1024 -forceref"
// then after: strCmdLine==" -forceref" and strFlag=="1024"
//--------------------------------------------------------------------------------------
bool CTxDxAgent::DXUTGetCmdParam( WCHAR*& strCmdLine, WCHAR* strFlag, int nFlagLen )
{
	strCmdLine += nFlagLen;
	if( *strCmdLine == L':' )
	{       
		strCmdLine++; // Skip ':'

		// Place NULL terminator in strFlag after current token
		wcsncpy( strFlag, strCmdLine, 256 );
		strFlag[255] = 0;
		WCHAR* strSpace = strFlag;
		while (*strSpace && (*strSpace > L' '))
			strSpace++;
		*strSpace = 0;

		// Update strCmdLine
		strCmdLine += wcslen(strFlag);
		return true;
	}
	else
	{
		strFlag[0] = 0;
		return false;
	}
}

//--------------------------------------------------------------------------------------
// Pauses time or rendering.  Keeps a ref count so pausing can be layered
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTPause( bool bPauseTime, bool bPauseRendering )
{
	int nPauseTimeCount = GetDXUTState().GetPauseTimeCount();
	nPauseTimeCount += ( bPauseTime ? +1 : -1 );
	if( nPauseTimeCount < 0 )
		nPauseTimeCount = 0;
	GetDXUTState().SetPauseTimeCount( nPauseTimeCount );

	int nPauseRenderingCount = GetDXUTState().GetPauseRenderingCount();
	nPauseRenderingCount += ( bPauseRendering ? +1 : -1 );
	if( nPauseRenderingCount < 0 )
		nPauseRenderingCount = 0;
	GetDXUTState().SetPauseRenderingCount( nPauseRenderingCount );

	if( nPauseTimeCount > 0 )
	{
		// Stop the scene from animating
		DXUTGetGlobalTimer()->Stop();
	}
	else
	{
		// Restart the timer
		DXUTGetGlobalTimer()->Start();
	}

	GetDXUTState().SetRenderingPaused( nPauseRenderingCount > 0 );
	GetDXUTState().SetTimePaused( nPauseTimeCount > 0 );
}


//--------------------------------------------------------------------------------------
// Return the device settings of the current device.  If no device exists yet, then
// return blank device settings 
//--------------------------------------------------------------------------------------
DXUTDeviceSettings CTxDxAgent::DXUTGetDeviceSettings()   
{ 
	DXUTDeviceSettings* pDS = GetDXUTState().GetCurrentDeviceSettings();
	if( pDS )
	{
		return *pDS;
	}
	else
	{
		DXUTDeviceSettings ds;
		ZeroMemory( &ds, sizeof(DXUTDeviceSettings) );
		return ds;
	}
} 

void CTxDxAgent::DXUTSetConstantFrameTime( bool bEnabled, float fTimePerFrame ) 
{ 
	if( GetDXUTState().GetOverrideConstantFrameTime() ) 
	{ 
		bEnabled = GetDXUTState().GetOverrideConstantFrameTime(); 
		fTimePerFrame = GetDXUTState().GetOverrideConstantTimePerFrame(); 
	} 
	GetDXUTState().SetConstantFrameTime(bEnabled); 
	GetDXUTState().SetTimePerFrame(fTimePerFrame); 
}


//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
								 D3DFORMAT BackBufferFormat, bool bWindowed )
{
// 	// Skip backbuffer formats that don't support alpha blending
// 	IDirect3D9* pD3D = DXUTGetD3DObject(); 
// 	if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
// 		AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
// 		D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
// 		return false;

	// Must support pixel shader 1.1
	if( pCaps->PixelShaderVersion < D3DPS_VERSION( 1, 1 ) )
		return false;

	return true;
}


//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// the sample framework will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
//--------------------------------------------------------------------------------------
void CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps )
{
	// If device doesn't support HW T&L or doesn't support 1.1 vertex shaders in HW 
	// then switch to SWVP.
	if( (pCaps->DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		pCaps->VertexShaderVersion < D3DVS_VERSION(1,1) )
	{
		pDeviceSettings->BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else
	{
		pDeviceSettings->BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}

	// This application is designed to work on a pure device by not using 
	// IDirect3D9::Get*() methods, so create a pure device if supported and using HWVP.
	if ((pCaps->DevCaps & D3DDEVCAPS_PUREDEVICE) != 0 && 
		(pDeviceSettings->BehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) != 0 )
		pDeviceSettings->BehaviorFlags |= D3DCREATE_PUREDEVICE;

	// Debugging vertex shaders requires either REF or software vertex processing 
	// and debugging pixel shaders requires REF.  
#ifdef DEBUG_VS
	if( pDeviceSettings->DeviceType != D3DDEVTYPE_REF )
	{
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
		pDeviceSettings->BehaviorFlags &= ~D3DCREATE_PUREDEVICE;                            
		pDeviceSettings->BehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
#endif
#ifdef DEBUG_PS
	pDeviceSettings->DeviceType = D3DDEVTYPE_REF;
#endif
}

TXDRAW_EXPORTS_API ITxUIAgent* CreateTxAgent()
{
	ITxUIAgent* pAgent = new CTxDxAgent;
	return pAgent;
}



//--------------------------------------------------------------------------------------
// Creates a Direct3D device. If DXUTCreateWindow() or DXUTSetWindow() has not already 
// been called, it will call DXUTCreateWindow() with the default parameters.  
// Instead of calling this, you can call DXUTSetDevice() or DXUTCreateDeviceFromSettings() 
//--------------------------------------------------------------------------------------
HRESULT CTxDxAgent::DXUTCreateDevice( UINT AdapterOrdinal, bool bWindowed, 
						 int nSuggestedWidth, int nSuggestedHeight,
						 LPDXUTCALLBACKISDEVICEACCEPTABLE pCallbackIsDeviceAcceptable,
						 LPDXUTCALLBACKMODIFYDEVICESETTINGS pCallbackModifyDeviceSettings )
{
	HRESULT hr;

	// Not allowed to call this from inside the device callbacks
	if( GetDXUTState().GetInsideDeviceCallback() )
		return DXUT_ERR_MSGBOX( L"DXUTCreateWindow", E_FAIL );

	// Record the function arguments in the global state 
	GetDXUTState().SetIsDeviceAcceptableFunc( pCallbackIsDeviceAcceptable );
	GetDXUTState().SetModifyDeviceSettingsFunc( pCallbackModifyDeviceSettings );

	GetDXUTState().SetDeviceCreateCalled( true );

	// If DXUTCreateWindow() or DXUTSetWindow() has not already been called, 
	// then call DXUTCreateWindow() with the default parameters.         
	if( !GetDXUTState().GetWindowCreated() ) 
	{
		// If DXUTCreateWindow() or DXUTSetWindow() was already called and failed, then fail.
		// DXUTCreateWindow() or DXUTSetWindow() must first succeed for this function to succeed
		if( GetDXUTState().GetWindowCreateCalled() )
			return E_FAIL; 

		// If DXUTCreateWindow() or DXUTSetWindow() hasn't been called, then 
		// automatically call DXUTCreateWindow() with default params
		ASSERT(FALSE);
		//hr = DXUTCreateWindow();
// 		if( FAILED(hr) )
// 			return hr;
	}

	// Force an enumeration with the new IsDeviceAcceptable callback
	DXUTPrepareEnumerationObject( true );

	DXUTMatchOptions matchOptions;
	matchOptions.eAdapterOrdinal     = DXUTMT_PRESERVE_INPUT;
	matchOptions.eDeviceType         = DXUTMT_IGNORE_INPUT;
	matchOptions.eWindowed           = DXUTMT_PRESERVE_INPUT;
	matchOptions.eAdapterFormat      = DXUTMT_IGNORE_INPUT;
	matchOptions.eVertexProcessing   = DXUTMT_IGNORE_INPUT;
	matchOptions.eResolution         = DXUTMT_CLOSEST_TO_INPUT;
	matchOptions.eBackBufferFormat   = DXUTMT_IGNORE_INPUT;
	matchOptions.eBackBufferCount    = DXUTMT_IGNORE_INPUT;
	matchOptions.eMultiSample        = DXUTMT_IGNORE_INPUT;
	matchOptions.eSwapEffect         = DXUTMT_IGNORE_INPUT;
	matchOptions.eDepthFormat        = DXUTMT_IGNORE_INPUT;
	matchOptions.eStencilFormat      = DXUTMT_IGNORE_INPUT;
	matchOptions.ePresentFlags       = DXUTMT_IGNORE_INPUT;
	matchOptions.eRefreshRate        = DXUTMT_IGNORE_INPUT;
	matchOptions.ePresentInterval    = DXUTMT_IGNORE_INPUT;

	DXUTDeviceSettings deviceSettings;
	ZeroMemory( &deviceSettings, sizeof(DXUTDeviceSettings) );
	deviceSettings.AdapterOrdinal      = AdapterOrdinal;
	deviceSettings.pp.Windowed         = bWindowed;
	deviceSettings.pp.BackBufferWidth  = nSuggestedWidth;
	deviceSettings.pp.BackBufferHeight = nSuggestedHeight;

	// Override with settings from the command line
	if( GetDXUTState().GetOverrideWidth() != 0 )
		deviceSettings.pp.BackBufferWidth = GetDXUTState().GetOverrideWidth();
	if( GetDXUTState().GetOverrideHeight() != 0 )
		deviceSettings.pp.BackBufferHeight = GetDXUTState().GetOverrideHeight();

	if( GetDXUTState().GetOverrideAdapterOrdinal() != -1 )
		deviceSettings.AdapterOrdinal = GetDXUTState().GetOverrideAdapterOrdinal();

	if( GetDXUTState().GetOverrideFullScreen() )
	{
		deviceSettings.pp.Windowed = FALSE;
		if( GetDXUTState().GetOverrideWidth() == 0 && GetDXUTState().GetOverrideHeight() == 0 )
			matchOptions.eResolution = DXUTMT_IGNORE_INPUT;
	}
	if( GetDXUTState().GetOverrideWindowed() )
		deviceSettings.pp.Windowed = TRUE;

	if( GetDXUTState().GetOverrideForceHAL() )
	{
		deviceSettings.DeviceType = D3DDEVTYPE_HAL;
		matchOptions.eDeviceType = DXUTMT_PRESERVE_INPUT;
	}
	if( GetDXUTState().GetOverrideForceREF() )
	{
		deviceSettings.DeviceType = D3DDEVTYPE_REF;
		matchOptions.eDeviceType = DXUTMT_PRESERVE_INPUT;
	}

	if( GetDXUTState().GetOverrideForcePureHWVP() )
	{
		deviceSettings.BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
		matchOptions.eVertexProcessing = DXUTMT_PRESERVE_INPUT;
	}
	else if( GetDXUTState().GetOverrideForceHWVP() )
	{
		deviceSettings.BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		matchOptions.eVertexProcessing = DXUTMT_PRESERVE_INPUT;
	}
	else if( GetDXUTState().GetOverrideForceSWVP() )
	{
		deviceSettings.BehaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		matchOptions.eVertexProcessing = DXUTMT_PRESERVE_INPUT;
	}

	hr = DXUTFindValidDeviceSettings( &deviceSettings, &deviceSettings, &matchOptions );
	if( FAILED(hr) ) // the call will fail if no valid devices were found
	{
		DXUTDisplayErrorMessage( hr );
		return DXUT_ERR( L"DXUTFindValidDeviceSettings", hr );
	}

	// If the ModifyDeviceSettings callback is non-NULL, then call it to 
	// let the app change the settings
	if( pCallbackModifyDeviceSettings )
	{
		D3DCAPS9 caps;
		IDirect3D9* pD3D = DXUTGetD3DObject();
		pD3D->GetDeviceCaps( deviceSettings.AdapterOrdinal, deviceSettings.DeviceType, &caps );

		pCallbackModifyDeviceSettings( &deviceSettings, &caps );
	}

	// Change to a Direct3D device created from the new device settings.  
	// If there is an existing device, then either reset or recreated the scene
	hr = DXUTChangeDevice( &deviceSettings, NULL, false );
	if( FAILED(hr) )
		return hr;

	return S_OK;
}

HRESULT CTxDxAgent::DXUTInitialize3DEnvironment()
{
	HRESULT hr = S_OK;

	IDirect3DDevice9* pd3dDevice = DXUTGetD3DDevice();
	assert( pd3dDevice != NULL );

	GetDXUTState().SetDeviceObjectsCreated( false );
	GetDXUTState().SetDeviceObjectsReset( false );

	// Prepare the window for a possible change between windowed mode 
	// and full screen mode by adjusting the window style and its menu.
	DXUTAdjustWindowStyle( DXUTGetHWND(), DXUTIsWindowed() );

	// Prepare the device with cursor info and 
	// store backbuffer desc and caps from the device
	DXUTPrepareDevice( pd3dDevice );

	// If the settings dialog exists, then call OnCreatedDevice() & OnResetDevice() on it.
	CD3DSettingsDlg* pD3DSettingsDlg = GetDXUTState().GetD3DSettingsDlg();
	if( pD3DSettingsDlg )
	{
		hr = pD3DSettingsDlg->OnCreateDevice( pd3dDevice );
		if( FAILED(hr) )
			return DXUT_ERR( L"OnCreateDevice", DXUTERR_CREATINGDEVICEOBJECTS );

		hr = pD3DSettingsDlg->OnResetDevice();
		if( FAILED(hr) )
			return DXUT_ERR( L"OnCreateDevice", DXUTERR_CREATINGDEVICEOBJECTS );
	}

	//call txdxresource manager to create
	GetTxResourceMgr().OnCreateDevice(pd3dDevice);
	if( FAILED(hr) )
	{
		if( hr == DXUTERR_MEDIANOTFOUND )
			return DXUT_ERR( L"OnCreateDevice", DXUTERR_MEDIANOTFOUND );
		else
			return DXUT_ERR( L"OnCreateDevice", DXUTERR_CREATINGDEVICEOBJECTS );
	}

	// Call the GUI resource device created function
	hr = DXUTGetGlobalDialogResourceManager()->OnCreateDevice( pd3dDevice );
	if( FAILED(hr) )
	{
		if( hr == DXUTERR_MEDIANOTFOUND )
			return DXUT_ERR( L"OnCreateDevice", DXUTERR_MEDIANOTFOUND );
		else
			return DXUT_ERR( L"OnCreateDevice", DXUTERR_CREATINGDEVICEOBJECTS );
	}

	// Call the resource cache created function
	hr = DXUTGetGlobalResourceCache().OnCreateDevice( pd3dDevice );
	if( FAILED(hr) )
	{
		if( hr == DXUTERR_MEDIANOTFOUND )
			return DXUT_ERR( L"OnCreateDevice", DXUTERR_MEDIANOTFOUND );
		else
			return DXUT_ERR( L"OnCreateDevice", DXUTERR_CREATINGDEVICEOBJECTS );
	}

	// Call the app's device created callback if set
	const D3DSURFACE_DESC* pbackBufferSurfaceDesc = DXUTGetBackBufferSurfaceDesc();
	GetDXUTState().SetInsideDeviceCallback( true );
	LPDXUTCALLBACKDEVICECREATED pCallbackDeviceCreated = GetDXUTState().GetDeviceCreatedFunc();
	hr = S_OK;
	if( pCallbackDeviceCreated != NULL )
		hr = pCallbackDeviceCreated( pd3dDevice, pbackBufferSurfaceDesc );
	GetDXUTState().SetInsideDeviceCallback( false );
	if( FAILED(hr) )  
	{
		// Cleanup upon failure
		DXUTCleanup3DEnvironment();

		DXUT_ERR( L"DeviceCreated callback", hr );        
		if( hr == DXUTERR_MEDIANOTFOUND )
			return DXUT_ERR( L"DeviceCreatedCallback", DXUTERR_MEDIANOTFOUND );
		else
			return DXUT_ERR( L"DeviceCreatedCallback", DXUTERR_CREATINGDEVICEOBJECTS );
	}
	else
	{
		//call txresourcemgr
		hr = GetTxResourceMgr().OnResetDevice();
		if( FAILED(hr) )
			return DXUT_ERR( L"OnResetDevice", DXUTERR_RESETTINGDEVICEOBJECTS );

		// Call the GUI resource device reset function
		hr = DXUTGetGlobalDialogResourceManager()->OnResetDevice();
		if( FAILED(hr) )
			return DXUT_ERR( L"OnResetDevice", DXUTERR_RESETTINGDEVICEOBJECTS );

		// Call the resource cache device reset function
		hr = DXUTGetGlobalResourceCache().OnResetDevice( pd3dDevice );
		if( FAILED(hr) )
			return DXUT_ERR( L"OnResetDevice", DXUTERR_RESETTINGDEVICEOBJECTS );

		// Call the app's device reset callback if set
		GetDXUTState().SetDeviceObjectsCreated( true );
		GetDXUTState().SetInsideDeviceCallback( true );
		LPDXUTCALLBACKDEVICERESET pCallbackDeviceReset = GetDXUTState().GetDeviceResetFunc();
		hr = S_OK;
		if( pCallbackDeviceReset != NULL )
			hr = pCallbackDeviceReset( pd3dDevice, pbackBufferSurfaceDesc );
		GetDXUTState().SetInsideDeviceCallback( false );
		if( FAILED(hr) )
		{
			DXUT_ERR( L"DeviceReset callback", hr );
			if( hr == DXUTERR_MEDIANOTFOUND )
				return DXUTERR_MEDIANOTFOUND;
			else
				return DXUTERR_RESETTINGDEVICEOBJECTS;
		}
		else
		{
			GetDXUTState().SetDeviceObjectsReset( true );
			return S_OK;
		}
	}
}


//--------------------------------------------------------------------------------------
// Prepare the window for a possible change between windowed mode and full screen mode
// by adjusting the window style and its menu.
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTAdjustWindowStyle( HWND hWnd, bool bWindowed )
{
	if( bWindowed )
	{
		// If different device windows are used for windowed mode and fullscreen mode,
		// hide the fullscreen window so that it doesn't obscure the screen.
		if( GetDXUTState().GetHWNDDeviceFullScreen() != GetDXUTState().GetHWNDDeviceWindowed() )
		{
			::ShowWindow( GetDXUTState().GetHWNDDeviceFullScreen(), SW_HIDE );
		}

		// Set windowed-mode style
		SetWindowLong( hWnd, GWL_STYLE, GetDXUTState().GetWinStyle() );
		if( GetDXUTState().GetMenu() != NULL )
		{
			SetMenu( hWnd, GetDXUTState().GetMenu() );
		}
	}
	else
	{
		// If different device windows are used for windowed mode and fullscreen mode,
		// restore and show the fullscreen device window.
		if( GetDXUTState().GetHWNDDeviceFullScreen() != GetDXUTState().GetHWNDDeviceWindowed() )
		{
			if( ::IsIconic( GetDXUTState().GetHWNDDeviceFullScreen() ) )
				::ShowWindow( GetDXUTState().GetHWNDDeviceFullScreen(), SW_RESTORE );
			::ShowWindow( GetDXUTState().GetHWNDDeviceFullScreen(), SW_SHOW );
		}

		// Set full screen mode style
		SetWindowLong( hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );
		if( GetDXUTState().GetMenu() )
		{
			HMENU hMenu = GetMenu( hWnd );
			GetDXUTState().SetMenu( hMenu );
			SetMenu( hWnd, NULL );
		}
	}
}

//--------------------------------------------------------------------------------------
// Prepares a new or resetting device by with cursor info and 
// store backbuffer desc and caps from the device
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTPrepareDevice( IDirect3DDevice9* pd3dDevice )
{
	HRESULT hr;

	// Update the device stats text
	DXUTUpdateStaticFrameStats();

	// Store render target surface desc
	IDirect3DSurface9* pBackBuffer;
	hr = pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	D3DSURFACE_DESC* pbackBufferSurfaceDesc = GetDXUTState().GetBackBufferSurfaceDesc();
	ZeroMemory( pbackBufferSurfaceDesc, sizeof(D3DSURFACE_DESC) );
	if( SUCCEEDED(hr) )
	{
		pBackBuffer->GetDesc( pbackBufferSurfaceDesc );
		SAFE_RELEASE( pBackBuffer );
	}

	// Update GetDXUTState()'s copy of D3D caps 
	D3DCAPS9* pd3dCaps = GetDXUTState().GetCaps();
	pd3dDevice->GetDeviceCaps( pd3dCaps );

	// Set up the full screen cursor
	if( GetDXUTState().GetShowCursorWhenFullScreen() && !DXUTIsWindowed() )
	{
		HCURSOR hCursor;
#ifdef _WIN64
		hCursor = (HCURSOR)GetClassLongPtr( DXUTGetHWND(), GCLP_HCURSOR );
#else
		hCursor = (HCURSOR)ULongToHandle( GetClassLong( DXUTGetHWND(), GCL_HCURSOR ) );
#endif
		DXUTSetDeviceCursor( pd3dDevice, hCursor, false );
		pd3dDevice->ShowCursor( true );
	}

	// Confine cursor to full screen window
	if( GetDXUTState().GetClipCursorWhenFullScreen() )
	{
		if( !DXUTIsWindowed() )
		{
			RECT rcWindow;
			GetWindowRect( DXUTGetHWND(), &rcWindow );
			ClipCursor( &rcWindow );
		}
		else
		{
			ClipCursor( NULL );
		}
	}
}

//--------------------------------------------------------------------------------------
// Updates the static part of the frame stats so it doesn't have be generated every frame
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTUpdateStaticFrameStats()
{
	DXUTDeviceSettings* pDeviceSettings = GetDXUTState().GetCurrentDeviceSettings();
	if( NULL == pDeviceSettings )
		return;
	CD3DEnumeration* pd3dEnum = DXUTPrepareEnumerationObject();
	if( NULL == pd3dEnum )
		return;

	CD3DEnumDeviceSettingsCombo* pDeviceSettingsCombo = pd3dEnum->GetDeviceSettingsCombo( pDeviceSettings->AdapterOrdinal, pDeviceSettings->DeviceType, pDeviceSettings->AdapterFormat, pDeviceSettings->pp.BackBufferFormat, pDeviceSettings->pp.Windowed );
	if( NULL == pDeviceSettingsCombo )
		return;

	WCHAR strFmt[100];
	D3DPRESENT_PARAMETERS* pPP = &pDeviceSettings->pp;

	if( pDeviceSettingsCombo->AdapterFormat == pDeviceSettingsCombo->BackBufferFormat )
	{
		wcsncpy( strFmt, DXUTD3DFormatToString( pDeviceSettingsCombo->AdapterFormat, false ), 100 );
	}
	else
	{
		_snwprintf( strFmt, 100, L"backbuf %s, adapter %s", 
			DXUTD3DFormatToString( pDeviceSettingsCombo->BackBufferFormat, false ), 
			DXUTD3DFormatToString( pDeviceSettingsCombo->AdapterFormat, false ) );
	}
	strFmt[99] = 0;

	WCHAR strDepthFmt[100];
	if( pPP->EnableAutoDepthStencil )
	{
		_snwprintf( strDepthFmt, 100, L" (%s)", DXUTD3DFormatToString( pPP->AutoDepthStencilFormat, false ) );
		strDepthFmt[99] = 0;
	}
	else
	{
		// No depth buffer
		strDepthFmt[0] = 0;
	}

	WCHAR strMultiSample[100];
	switch( pPP->MultiSampleType )
	{
	case D3DMULTISAMPLE_NONMASKABLE: wcsncpy( strMultiSample, L" (Nonmaskable Multisample)", 100 ); break;
	case D3DMULTISAMPLE_NONE:        wcsncpy( strMultiSample, L"", 100 ); break;
	default:                         _snwprintf( strMultiSample, 100, L" (%dx Multisample)", pPP->MultiSampleType ); break;
	}
	strMultiSample[99] = 0;

	WCHAR* pstrStaticFrameStats = GetDXUTState().GetStaticFrameStats();
	_snwprintf( pstrStaticFrameStats, 256, L"%%.02f fps (%dx%d), %s%s%s", 
		pPP->BackBufferWidth, pPP->BackBufferHeight,
		strFmt, strDepthFmt, strMultiSample );
	pstrStaticFrameStats[255] = 0;
}


//--------------------------------------------------------------------------------------
// Resets the 3D environment by:
//      - Calls the device lost callback 
//      - Resets the device
//      - Stores the back buffer description
//      - Sets up the full screen Direct3D cursor if requested
//      - Calls the device reset callback 
//--------------------------------------------------------------------------------------
HRESULT CTxDxAgent::DXUTReset3DEnvironment()
{
	HRESULT hr;

	IDirect3DDevice9* pd3dDevice = DXUTGetD3DDevice();
	assert( pd3dDevice != NULL );

	CD3DSettingsDlg* pD3DSettingsDlg = GetDXUTState().GetD3DSettingsDlg();
	if( pD3DSettingsDlg )
		pD3DSettingsDlg->OnLostDevice();

	// Release all vidmem objects
	if( GetDXUTState().GetDeviceObjectsReset() )
	{
		GetDXUTState().SetInsideDeviceCallback( true );

		DXUTGetGlobalDialogResourceManager()->OnLostDevice();
		DXUTGetGlobalResourceCache().OnLostDevice();

		LPDXUTCALLBACKDEVICELOST pCallbackDeviceLost = GetDXUTState().GetDeviceLostFunc();
		if( pCallbackDeviceLost != NULL )
			pCallbackDeviceLost();
		GetDXUTState().SetDeviceObjectsReset( false );
		GetDXUTState().SetInsideDeviceCallback( false );
	}

	// Prepare the window for a possible change between windowed mode 
	// and full screen mode by adjusting the window style and its menu.
	DXUTAdjustWindowStyle( DXUTGetHWND(), DXUTIsWindowed() );

	// Reset the device
	DXUTDeviceSettings* pDeviceSettings = GetDXUTState().GetCurrentDeviceSettings();
	hr = pd3dDevice->Reset( &pDeviceSettings->pp );
	if( FAILED(hr) )  
	{
		if( hr == D3DERR_DEVICELOST )
			return D3DERR_DEVICELOST; // Reset could legitimately fail if the device is lost
		else
			return DXUT_ERR( L"Reset", DXUTERR_RESETTINGDEVICE );
	}

	// Prepare the device with cursor info and 
	// store backbuffer desc and caps from the device
	DXUTPrepareDevice( pd3dDevice );

	// If the settings dialog exists call its OnResetDevice() 
	if( pD3DSettingsDlg )
	{
		hr = pD3DSettingsDlg->OnResetDevice();
		if( FAILED(hr) )
			return DXUT_ERR( L"OnResetDevice", DXUTERR_RESETTINGDEVICEOBJECTS );
	}

	hr = DXUTGetGlobalDialogResourceManager()->OnResetDevice();
	if( FAILED(hr) )
		return DXUT_ERR( L"OnResetDevice", DXUTERR_RESETTINGDEVICEOBJECTS );

	hr = DXUTGetGlobalResourceCache().OnResetDevice( pd3dDevice );
	if( FAILED(hr) )
		return DXUT_ERR( L"OnResetDevice", DXUTERR_RESETTINGDEVICEOBJECTS );

	// Initialize the app's device-dependent objects
	GetDXUTState().SetInsideDeviceCallback( true );
	const D3DSURFACE_DESC* pbackBufferSurfaceDesc = DXUTGetBackBufferSurfaceDesc();
	LPDXUTCALLBACKDEVICERESET pCallbackDeviceReset = GetDXUTState().GetDeviceResetFunc();
	hr = S_OK;
	if( pCallbackDeviceReset != NULL )
		hr = pCallbackDeviceReset( pd3dDevice, pbackBufferSurfaceDesc );
	GetDXUTState().SetInsideDeviceCallback( false );
	if( FAILED(hr) )
	{
		DXUT_ERR( L"DeviceResetCallback", hr );
		if( hr != DXUTERR_MEDIANOTFOUND )
			hr = DXUTERR_RESETTINGDEVICEOBJECTS;

		DXUTGetGlobalDialogResourceManager()->OnLostDevice();
		DXUTGetGlobalResourceCache().OnLostDevice();

		LPDXUTCALLBACKDEVICELOST pCallbackDeviceLost = GetDXUTState().GetDeviceLostFunc();
		if( pCallbackDeviceLost != NULL )
			pCallbackDeviceLost();
	}
	else
	{
		// Success
		GetDXUTState().SetDeviceObjectsReset( true );
	}

	return hr;
}

ITxResourceMgr* CTxDxAgent::GetResourceMgr()
{
	return &m_resMgr;
}

DXUTTextureNode* CTxDxAgent::GetTexture( int nIdx )
{
	return m_resMgr.GetTextureNode(nIdx);
}


//--------------------------------------------------------------------------------------
// This function tries to find valid device settings based upon the input device settings 
// struct and the match options.  For each device setting a match option in the 
// DXUTMatchOptions struct specifies how the function makes decisions.  For example, if 
// the caller wants a HAL device with a back buffer format of D3DFMT_A2B10G10R10 but the 
// HAL device on the system does not support D3DFMT_A2B10G10R10 however a REF device is 
// installed that does, then the function has a choice to either use REF or to change to 
// a back buffer format to compatible with the HAL device.  The match options lets the 
// caller control how these choices are made.
//
// Each match option must be one of the following types: 
//      DXUTMT_IGNORE_INPUT: Uses the closest valid value to a default 
//      DXUTMT_PRESERVE_INPUT: Uses the input without change, but may cause no valid device to be found
//      DXUTMT_CLOSEST_TO_INPUT: Uses the closest valid value to the input 
//
// If pMatchOptions is NULL then, all of the match options are assumed to be DXUTMT_IGNORE_INPUT.  
// The function returns failure if no valid device settings can be found otherwise 
// the function returns success and the valid device settings are written to pOut.
//--------------------------------------------------------------------------------------
HRESULT CTxDxAgent::DXUTFindValidDeviceSettings( DXUTDeviceSettings* pOut, DXUTDeviceSettings* pIn, 
									DXUTMatchOptions* pMatchOptions )
{
	if( pOut == NULL )
		return DXUT_ERR_MSGBOX( L"DXUTFindValidDeviceSettings", E_INVALIDARG );

	CD3DEnumeration* pd3dEnum = DXUTPrepareEnumerationObject( false );
	IDirect3D9*      pD3D     = DXUTGetD3DObject();

	// Default to DXUTMT_IGNORE_INPUT for everything unless pMatchOptions isn't NULL
	DXUTMatchOptions defaultMatchOptions;
	if( NULL == pMatchOptions )
	{
		ZeroMemory( &defaultMatchOptions, sizeof(DXUTMatchOptions) );
		pMatchOptions = &defaultMatchOptions;
	}

	// Build an optimal device settings structure based upon the match 
	// options.  If the match option is set to ignore, then a optimal default value is used.
	// The default value may not exist on the system, but later this will be taken 
	// into account.
	DXUTDeviceSettings optimalDeviceSettings;
	DXUTBuildOptimalDeviceSettings( &optimalDeviceSettings, pIn, pMatchOptions );

	// Find the best combination of:
	//      Adapter Ordinal
	//      Device Type
	//      Adapter Format
	//      Back Buffer Format
	//      Windowed
	// given what's available on the system and the match options combined with the device settings input.
	// This combination of settings is encapsulated by the CD3DEnumDeviceSettingsCombo class.
	float fBestRanking = -1.0f;
	CD3DEnumDeviceSettingsCombo* pBestDeviceSettingsCombo = NULL;
	D3DDISPLAYMODE adapterDesktopDisplayMode;

	CGrowableArray<CD3DEnumAdapterInfo*>* pAdapterList = pd3dEnum->GetAdapterInfoList();
	for( int iAdapter=0; iAdapter<pAdapterList->GetSize(); iAdapter++ )
	{
		CD3DEnumAdapterInfo* pAdapterInfo = pAdapterList->GetAt(iAdapter);

		// Get the desktop display mode of adapter 
		pD3D->GetAdapterDisplayMode( pAdapterInfo->AdapterOrdinal, &adapterDesktopDisplayMode );

		// Enum all the device types supported by this adapter to find the best device settings
		for( int iDeviceInfo=0; iDeviceInfo<pAdapterInfo->deviceInfoList.GetSize(); iDeviceInfo++ )
		{
			CD3DEnumDeviceInfo* pDeviceInfo = pAdapterInfo->deviceInfoList.GetAt(iDeviceInfo);

			// Enum all the device settings combinations.  A device settings combination is 
			// a unique set of an adapter format, back buffer format, and IsWindowed.
			for( int iDeviceCombo=0; iDeviceCombo<pDeviceInfo->deviceSettingsComboList.GetSize(); iDeviceCombo++ )
			{
				CD3DEnumDeviceSettingsCombo* pDeviceSettingsCombo = pDeviceInfo->deviceSettingsComboList.GetAt(iDeviceCombo);

				// If windowed mode the adapter format has to be the same as the desktop 
				// display mode format so skip any that don't match
				if (pDeviceSettingsCombo->Windowed && (pDeviceSettingsCombo->AdapterFormat != adapterDesktopDisplayMode.Format))
					continue;

				// Skip any combo that doesn't meet the preserve match options
				if( false == DXUTDoesDeviceComboMatchPreserveOptions( pDeviceSettingsCombo, pIn, pMatchOptions ) )
					continue;           

				// Get a ranking number that describes how closely this device combo matches the optimal combo
				float fCurRanking = DXUTRankDeviceCombo( pDeviceSettingsCombo, &optimalDeviceSettings, &adapterDesktopDisplayMode );

				// If this combo better matches the input device settings then save it
				if( fCurRanking > fBestRanking )
				{
					pBestDeviceSettingsCombo = pDeviceSettingsCombo;
					fBestRanking = fCurRanking;
				}                
			}
		}
	}

	// If no best device combination was found then fail
	if( pBestDeviceSettingsCombo == NULL ) 
		return DXUTERR_NOCOMPATIBLEDEVICES;

	// Using the best device settings combo found, build valid device settings taking heed of 
	// the match options and the input device settings
	DXUTDeviceSettings validDeviceSettings;
	DXUTBuildValidDeviceSettings( &validDeviceSettings, pBestDeviceSettingsCombo, pIn, pMatchOptions );
	*pOut = validDeviceSettings;

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Internal helper function to build a device settings structure based upon the match 
// options.  If the match option is set to ignore, then a optimal default value is used.
// The default value may not exist on the system, but later this will be taken 
// into account.
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTBuildOptimalDeviceSettings( DXUTDeviceSettings* pOptimalDeviceSettings, 
									DXUTDeviceSettings* pDeviceSettingsIn, 
									DXUTMatchOptions* pMatchOptions )
{
	IDirect3D9* pD3D = DXUTGetD3DObject();
	D3DDISPLAYMODE adapterDesktopDisplayMode;

	ZeroMemory( pOptimalDeviceSettings, sizeof(DXUTDeviceSettings) ); 

	//---------------------
	// Adapter ordinal
	//---------------------    
	if( pMatchOptions->eAdapterOrdinal == DXUTMT_IGNORE_INPUT )
		pOptimalDeviceSettings->AdapterOrdinal = D3DADAPTER_DEFAULT; 
	else
		pOptimalDeviceSettings->AdapterOrdinal = pDeviceSettingsIn->AdapterOrdinal;      

	//---------------------
	// Device type
	//---------------------
	if( pMatchOptions->eDeviceType == DXUTMT_IGNORE_INPUT )
		pOptimalDeviceSettings->DeviceType = D3DDEVTYPE_HAL; 
	else
		pOptimalDeviceSettings->DeviceType = pDeviceSettingsIn->DeviceType;

	//---------------------
	// Windowed
	//---------------------
	if( pMatchOptions->eWindowed == DXUTMT_IGNORE_INPUT )
		pOptimalDeviceSettings->pp.Windowed = TRUE; 
	else
		pOptimalDeviceSettings->pp.Windowed = pDeviceSettingsIn->pp.Windowed;

	//---------------------
	// Adapter format
	//---------------------
	if( pMatchOptions->eAdapterFormat == DXUTMT_IGNORE_INPUT )
	{
		// If windowed, default to the desktop display mode
		// If fullscreen, default to the desktop display mode for quick mode change or 
		// default to D3DFMT_X8R8G8B8 if the desktop display mode is < 32bit
		pD3D->GetAdapterDisplayMode( pOptimalDeviceSettings->AdapterOrdinal, &adapterDesktopDisplayMode );
		if( pOptimalDeviceSettings->pp.Windowed || DXUTColorChannelBits(adapterDesktopDisplayMode.Format) >= 8 )
			pOptimalDeviceSettings->AdapterFormat = adapterDesktopDisplayMode.Format;
		else
			pOptimalDeviceSettings->AdapterFormat = D3DFMT_X8R8G8B8;
	}
	else
	{
		pOptimalDeviceSettings->AdapterFormat = pDeviceSettingsIn->AdapterFormat;
	}

	//---------------------
	// Vertex processing
	//---------------------
	if( pMatchOptions->eVertexProcessing == DXUTMT_IGNORE_INPUT )
		pOptimalDeviceSettings->BehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING; 
	else
		pOptimalDeviceSettings->BehaviorFlags = pDeviceSettingsIn->BehaviorFlags;

	//---------------------
	// Resolution
	//---------------------
	if( pMatchOptions->eResolution == DXUTMT_IGNORE_INPUT )
	{
		// If windowed, default to 640x480
		// If fullscreen, default to the desktop res for quick mode change
		if( pOptimalDeviceSettings->pp.Windowed )
		{
			pOptimalDeviceSettings->pp.BackBufferWidth = 640;
			pOptimalDeviceSettings->pp.BackBufferHeight = 480;
		}
		else
		{
			pD3D->GetAdapterDisplayMode( pOptimalDeviceSettings->AdapterOrdinal, &adapterDesktopDisplayMode );
			pOptimalDeviceSettings->pp.BackBufferWidth = adapterDesktopDisplayMode.Width;
			pOptimalDeviceSettings->pp.BackBufferHeight = adapterDesktopDisplayMode.Height;
		}
	}
	else
	{
		pOptimalDeviceSettings->pp.BackBufferWidth = pDeviceSettingsIn->pp.BackBufferWidth;
		pOptimalDeviceSettings->pp.BackBufferHeight = pDeviceSettingsIn->pp.BackBufferHeight;
	}

	//---------------------
	// Back buffer format
	//---------------------
	if( pMatchOptions->eBackBufferFormat == DXUTMT_IGNORE_INPUT )
		pOptimalDeviceSettings->pp.BackBufferFormat = pOptimalDeviceSettings->AdapterFormat; // Default to match the adapter format
	else
		pOptimalDeviceSettings->pp.BackBufferFormat = pDeviceSettingsIn->pp.BackBufferFormat;

	//---------------------
	// Back buffer count
	//---------------------
	if( pMatchOptions->eBackBufferCount == DXUTMT_IGNORE_INPUT )
		pOptimalDeviceSettings->pp.BackBufferCount = 2; // Default to triple buffering for perf gain
	else
		pOptimalDeviceSettings->pp.BackBufferCount = pDeviceSettingsIn->pp.BackBufferCount;

	//---------------------
	// Multisample
	//---------------------
	if( pMatchOptions->eMultiSample == DXUTMT_IGNORE_INPUT )
	{
		// Default to no multisampling 
		pOptimalDeviceSettings->pp.MultiSampleType = D3DMULTISAMPLE_NONE;
		pOptimalDeviceSettings->pp.MultiSampleQuality = 0; 
	}
	else
	{
		pOptimalDeviceSettings->pp.MultiSampleType = pDeviceSettingsIn->pp.MultiSampleType;
		pOptimalDeviceSettings->pp.MultiSampleQuality = pDeviceSettingsIn->pp.MultiSampleQuality;
	}

	//---------------------
	// Swap effect
	//---------------------
	if( pMatchOptions->eSwapEffect == DXUTMT_IGNORE_INPUT )
		pOptimalDeviceSettings->pp.SwapEffect = D3DSWAPEFFECT_DISCARD; 
	else
		pOptimalDeviceSettings->pp.SwapEffect = pDeviceSettingsIn->pp.SwapEffect;

	//---------------------
	// Depth stencil 
	//---------------------
	if( pMatchOptions->eDepthFormat == DXUTMT_IGNORE_INPUT &&
		pMatchOptions->eStencilFormat == DXUTMT_IGNORE_INPUT )
	{
		UINT nBackBufferBits = DXUTColorChannelBits( pOptimalDeviceSettings->pp.BackBufferFormat );
		if( nBackBufferBits >= 8 )
			pOptimalDeviceSettings->pp.AutoDepthStencilFormat = D3DFMT_D32; 
		else
			pOptimalDeviceSettings->pp.AutoDepthStencilFormat = D3DFMT_D16; 
	}
	else
	{
		pOptimalDeviceSettings->pp.AutoDepthStencilFormat = pDeviceSettingsIn->pp.AutoDepthStencilFormat;
	}

	//---------------------
	// Present flags
	//---------------------
	if( pMatchOptions->ePresentFlags == DXUTMT_IGNORE_INPUT )
		pOptimalDeviceSettings->pp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	else
		pOptimalDeviceSettings->pp.Flags = pDeviceSettingsIn->pp.Flags;

	//---------------------
	// Refresh rate
	//---------------------
	if( pMatchOptions->eRefreshRate == DXUTMT_IGNORE_INPUT )
		pOptimalDeviceSettings->pp.FullScreen_RefreshRateInHz = 0;
	else
		pOptimalDeviceSettings->pp.FullScreen_RefreshRateInHz = pDeviceSettingsIn->pp.FullScreen_RefreshRateInHz;

	//---------------------
	// Present interval
	//---------------------
	if( pMatchOptions->ePresentInterval == DXUTMT_IGNORE_INPUT )
	{
		// For windowed, default to D3DPRESENT_INTERVAL_IMMEDIATE
		// which will wait not for the vertical retrace period to prevent tearing, 
		// but may introduce tearing.
		// For full screen, default to D3DPRESENT_INTERVAL_DEFAULT 
		// which will wait for the vertical retrace period to prevent tearing.
		if( pOptimalDeviceSettings->pp.Windowed )
			pOptimalDeviceSettings->pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		else
			pOptimalDeviceSettings->pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	}
	else
	{
		pOptimalDeviceSettings->pp.PresentationInterval = pDeviceSettingsIn->pp.PresentationInterval;
	}
}


//--------------------------------------------------------------------------------------
// Returns a ranking number that describes how closely this device 
// combo matches the optimal combo based on the match options and the optimal device settings
//--------------------------------------------------------------------------------------
float CTxDxAgent::DXUTRankDeviceCombo( CD3DEnumDeviceSettingsCombo* pDeviceSettingsCombo, 
						  DXUTDeviceSettings* pOptimalDeviceSettings,
						  D3DDISPLAYMODE* pAdapterDesktopDisplayMode )
{
	float fCurRanking = 0.0f; 

	// Arbitrary weights.  Gives preference to the ordinal, device type, and windowed
	const float fAdapterOrdinalWeight   = 1000.0f;
	const float fDeviceTypeWeight       = 100.0f;
	const float fWindowWeight           = 10.0f;
	const float fAdapterFormatWeight    = 1.0f;
	const float fVertexProcessingWeight = 1.0f;
	const float fResolutionWeight       = 1.0f;
	const float fBackBufferFormatWeight = 1.0f;
	const float fMultiSampleWeight      = 1.0f;
	const float fDepthStencilWeight     = 1.0f;
	const float fRefreshRateWeight      = 1.0f;
	const float fPresentIntervalWeight  = 1.0f;

	//---------------------
	// Adapter ordinal
	//---------------------
	if( pDeviceSettingsCombo->AdapterOrdinal == pOptimalDeviceSettings->AdapterOrdinal )
		fCurRanking += fAdapterOrdinalWeight;

	//---------------------
	// Device type
	//---------------------
	if( pDeviceSettingsCombo->DeviceType == pOptimalDeviceSettings->DeviceType )
		fCurRanking += fDeviceTypeWeight;
	// Slightly prefer HAL 
	if( pDeviceSettingsCombo->DeviceType == D3DDEVTYPE_HAL )
		fCurRanking += 0.1f; 

	//---------------------
	// Windowed
	//---------------------
	if( pDeviceSettingsCombo->Windowed == pOptimalDeviceSettings->pp.Windowed )
		fCurRanking += fWindowWeight;

	//---------------------
	// Adapter format
	//---------------------
	if( pDeviceSettingsCombo->AdapterFormat == pOptimalDeviceSettings->AdapterFormat )
	{
		fCurRanking += fAdapterFormatWeight;
	}
	else
	{
		int nBitDepthDelta = abs( (long) DXUTColorChannelBits(pDeviceSettingsCombo->AdapterFormat) -
			(long) DXUTColorChannelBits(pOptimalDeviceSettings->AdapterFormat) );
		float fScale = max(0.9f - (float)nBitDepthDelta*0.2f, 0);
		fCurRanking += fScale * fAdapterFormatWeight;
	}

	if( !pDeviceSettingsCombo->Windowed )
	{
		// Slightly prefer when it matches the desktop format or is D3DFMT_X8R8G8B8
		bool bAdapterOptimalMatch;
		if( DXUTColorChannelBits(pAdapterDesktopDisplayMode->Format) >= 8 )
			bAdapterOptimalMatch = (pDeviceSettingsCombo->AdapterFormat == pAdapterDesktopDisplayMode->Format);
		else
			bAdapterOptimalMatch = (pDeviceSettingsCombo->AdapterFormat == D3DFMT_X8R8G8B8);

		if( bAdapterOptimalMatch )
			fCurRanking += 0.1f;
	}

	//---------------------
	// Vertex processing
	//---------------------
	if( (pOptimalDeviceSettings->BehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) != 0 || 
		(pOptimalDeviceSettings->BehaviorFlags & D3DCREATE_MIXED_VERTEXPROCESSING) != 0 )
	{
		if( (pDeviceSettingsCombo->pDeviceInfo->Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0 )
			fCurRanking += fVertexProcessingWeight;
	}
	// Slightly prefer HW T&L
	if( (pDeviceSettingsCombo->pDeviceInfo->Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0 )
		fCurRanking += 0.1f;

	//---------------------
	// Resolution
	//---------------------
	bool bResolutionFound = false;
	for( int idm = 0; idm < pDeviceSettingsCombo->pAdapterInfo->displayModeList.GetSize(); idm++ )
	{
		D3DDISPLAYMODE displayMode = pDeviceSettingsCombo->pAdapterInfo->displayModeList.GetAt( idm );
		if( displayMode.Format != pDeviceSettingsCombo->AdapterFormat )
			continue;
		if( displayMode.Width == pOptimalDeviceSettings->pp.BackBufferWidth &&
			displayMode.Height == pOptimalDeviceSettings->pp.BackBufferHeight )
			bResolutionFound = true;
	}
	if( bResolutionFound )
		fCurRanking += fResolutionWeight;

	//---------------------
	// Back buffer format
	//---------------------
	if( pDeviceSettingsCombo->BackBufferFormat == pOptimalDeviceSettings->pp.BackBufferFormat )
	{
		fCurRanking += fBackBufferFormatWeight;
	}
	else
	{
		int nBitDepthDelta = abs( (long) DXUTColorChannelBits(pDeviceSettingsCombo->BackBufferFormat) -
			(long) DXUTColorChannelBits(pOptimalDeviceSettings->pp.BackBufferFormat) );
		float fScale = max(0.9f - (float)nBitDepthDelta*0.2f, 0);
		fCurRanking += fScale * fBackBufferFormatWeight;
	}

	// Check if this back buffer format is the same as 
	// the adapter format since this is preferred.
	bool bAdapterMatchesBB = (pDeviceSettingsCombo->BackBufferFormat == pDeviceSettingsCombo->AdapterFormat);
	if( bAdapterMatchesBB )
		fCurRanking += 0.1f;

	//---------------------
	// Back buffer count
	//---------------------
	// No caps for the back buffer count

	//---------------------
	// Multisample
	//---------------------
	bool bMultiSampleFound = false;
	for( int i=0; i<pDeviceSettingsCombo->multiSampleTypeList.GetSize(); i++ )
	{
		D3DMULTISAMPLE_TYPE msType = pDeviceSettingsCombo->multiSampleTypeList.GetAt(i);
		DWORD msQuality  = pDeviceSettingsCombo->multiSampleQualityList.GetAt(i);

		if( msType == pOptimalDeviceSettings->pp.MultiSampleType &&
			msQuality >= pOptimalDeviceSettings->pp.MultiSampleQuality )
		{
			bMultiSampleFound = true;
			break;
		}
	}
	if( bMultiSampleFound )
		fCurRanking += fMultiSampleWeight;

	//---------------------
	// Swap effect
	//---------------------
	// No caps for swap effects

	//---------------------
	// Depth stencil 
	//---------------------
	if( pDeviceSettingsCombo->depthStencilFormatList.Contains( pOptimalDeviceSettings->pp.AutoDepthStencilFormat ) )
		fCurRanking += fDepthStencilWeight;

	//---------------------
	// Present flags
	//---------------------
	// No caps for the present flags

	//---------------------
	// Refresh rate
	//---------------------
	bool bRefreshFound = false;
	for( int idm = 0; idm < pDeviceSettingsCombo->pAdapterInfo->displayModeList.GetSize(); idm++ )
	{
		D3DDISPLAYMODE displayMode = pDeviceSettingsCombo->pAdapterInfo->displayModeList.GetAt( idm );
		if( displayMode.Format != pDeviceSettingsCombo->AdapterFormat )
			continue;
		if( displayMode.RefreshRate == pOptimalDeviceSettings->pp.FullScreen_RefreshRateInHz )
			bRefreshFound = true;
	}
	if( bRefreshFound )
		fCurRanking += fRefreshRateWeight;

	//---------------------
	// Present interval
	//---------------------
	// If keep present interval then check that the present interval is supported by this combo
	if( pDeviceSettingsCombo->presentIntervalList.Contains( pOptimalDeviceSettings->pp.PresentationInterval ) )
		fCurRanking += fPresentIntervalWeight;

	return fCurRanking;
}


//--------------------------------------------------------------------------------------
// Builds valid device settings using the match options, the input device settings, and the 
// best device settings combo found.
//--------------------------------------------------------------------------------------
void CTxDxAgent::DXUTBuildValidDeviceSettings( DXUTDeviceSettings* pValidDeviceSettings, 
								  CD3DEnumDeviceSettingsCombo* pBestDeviceSettingsCombo, 
								  DXUTDeviceSettings* pDeviceSettingsIn, 
								  DXUTMatchOptions* pMatchOptions )
{
	IDirect3D9* pD3D = DXUTGetD3DObject();
	D3DDISPLAYMODE adapterDesktopDisplayMode;
	pD3D->GetAdapterDisplayMode( pBestDeviceSettingsCombo->AdapterOrdinal, &adapterDesktopDisplayMode );

	// For each setting pick the best, taking into account the match options and 
	// what's supported by the device

	//---------------------
	// Adapter Ordinal
	//---------------------
	// Just using pBestDeviceSettingsCombo->AdapterOrdinal

	//---------------------
	// Device Type
	//---------------------
	// Just using pBestDeviceSettingsCombo->DeviceType

	//---------------------
	// Windowed 
	//---------------------
	// Just using pBestDeviceSettingsCombo->Windowed

	//---------------------
	// Adapter Format
	//---------------------
	// Just using pBestDeviceSettingsCombo->AdapterFormat

	//---------------------
	// Vertex processing
	//---------------------
	DWORD dwBestBehaviorFlags = 0;
	if( pMatchOptions->eVertexProcessing == DXUTMT_PRESERVE_INPUT )   
	{
		dwBestBehaviorFlags = pDeviceSettingsIn->BehaviorFlags;
	}
	else if( pMatchOptions->eVertexProcessing == DXUTMT_IGNORE_INPUT )    
	{
		// The framework defaults to HWVP if available otherwise use SWVP
		if ((pBestDeviceSettingsCombo->pDeviceInfo->Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0)
			dwBestBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else
			dwBestBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	else // if( pMatchOptions->eVertexProcessing == DXUTMT_CLOSEST_TO_INPUT )    
	{
		// Default to input, and fallback to SWVP if HWVP not available 
		dwBestBehaviorFlags = pDeviceSettingsIn->BehaviorFlags;
		if ((pBestDeviceSettingsCombo->pDeviceInfo->Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 && 
			( (dwBestBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) != 0 || 
			(dwBestBehaviorFlags & D3DCREATE_MIXED_VERTEXPROCESSING) != 0) )
		{
			dwBestBehaviorFlags &= ~D3DCREATE_HARDWARE_VERTEXPROCESSING;
			dwBestBehaviorFlags &= ~D3DCREATE_MIXED_VERTEXPROCESSING;
			dwBestBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		// One of these must be selected
		if( (dwBestBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) == 0 &&
			(dwBestBehaviorFlags & D3DCREATE_MIXED_VERTEXPROCESSING) == 0 &&
			(dwBestBehaviorFlags & D3DCREATE_SOFTWARE_VERTEXPROCESSING) == 0 )
		{
			if ((pBestDeviceSettingsCombo->pDeviceInfo->Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0)
				dwBestBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
			else
				dwBestBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}
	}

	//---------------------
	// Resolution
	//---------------------
	D3DDISPLAYMODE bestDisplayMode;  
	if( pMatchOptions->eResolution == DXUTMT_PRESERVE_INPUT )   
	{
		bestDisplayMode.Width = pDeviceSettingsIn->pp.BackBufferWidth;
		bestDisplayMode.Height = pDeviceSettingsIn->pp.BackBufferHeight;
	}
	else 
	{
		D3DDISPLAYMODE displayModeIn;  
		if( pMatchOptions->eResolution == DXUTMT_CLOSEST_TO_INPUT &&
			pDeviceSettingsIn && (pDeviceSettingsIn->pp.BackBufferWidth != 0 && pDeviceSettingsIn->pp.BackBufferWidth != 0) )   
		{
			displayModeIn.Width = pDeviceSettingsIn->pp.BackBufferWidth;
			displayModeIn.Height = pDeviceSettingsIn->pp.BackBufferHeight;
		}
		else // if( pMatchOptions->eResolution == DXUTMT_IGNORE_INPUT )   
		{
			if( pBestDeviceSettingsCombo->Windowed )
			{
				// The framework defaults to 640x480 for windowed
				displayModeIn.Width = 640;
				displayModeIn.Height = 480;
			}
			else
			{
				// The framework defaults to desktop resolution for fullscreen to try to avoid slow mode change
				displayModeIn.Width = adapterDesktopDisplayMode.Width;
				displayModeIn.Height = adapterDesktopDisplayMode.Height;
			}
		}

		// Call a helper function to find the closest valid display mode to the optimal 
		DXUTFindValidResolution( pBestDeviceSettingsCombo, displayModeIn, &bestDisplayMode );
	}

	//---------------------
	// Back Buffer Format
	//---------------------
	// Just using pBestDeviceSettingsCombo->BackBufferFormat

	//---------------------
	// Back buffer count
	//---------------------
	UINT bestBackBufferCount;
	if( pMatchOptions->eBackBufferCount == DXUTMT_PRESERVE_INPUT )   
	{
		bestBackBufferCount = pDeviceSettingsIn->pp.BackBufferCount;
	}
	else if( pMatchOptions->eBackBufferCount == DXUTMT_IGNORE_INPUT )   
	{
		// The framework defaults to triple buffering 
		bestBackBufferCount = 2;
	}
	else // if( pMatchOptions->eBackBufferCount == DXUTMT_CLOSEST_TO_INPUT )   
	{
		bestBackBufferCount = pDeviceSettingsIn->pp.BackBufferCount;
		if( bestBackBufferCount > 3 )
			bestBackBufferCount = 3;
		if( bestBackBufferCount < 1 )
			bestBackBufferCount = 1;
	}

	//---------------------
	// Multisample
	//---------------------
	D3DMULTISAMPLE_TYPE bestMultiSampleType;
	DWORD bestMultiSampleQuality;
	if( pDeviceSettingsIn && pDeviceSettingsIn->pp.SwapEffect != D3DSWAPEFFECT_DISCARD )
	{
		// Swap effect is not set to discard so multisampling has to off
		bestMultiSampleType = D3DMULTISAMPLE_NONE;
		bestMultiSampleQuality = 0;
	}
	else
	{
		if( pMatchOptions->eBackBufferCount == DXUTMT_PRESERVE_INPUT )   
		{
			bestMultiSampleType    = pDeviceSettingsIn->pp.MultiSampleType;
			bestMultiSampleQuality = pDeviceSettingsIn->pp.MultiSampleQuality;
		}
		else if( pMatchOptions->eBackBufferCount == DXUTMT_IGNORE_INPUT )   
		{
			// Default to no multisampling (always supported)
			bestMultiSampleType = D3DMULTISAMPLE_NONE;
			bestMultiSampleQuality = 0;
		}
		else if( pMatchOptions->eBackBufferCount == DXUTMT_CLOSEST_TO_INPUT )   
		{
			// Default to no multisampling (always supported)
			bestMultiSampleType = D3DMULTISAMPLE_NONE;
			bestMultiSampleQuality = 0;

			for( int i=0; i < pBestDeviceSettingsCombo->multiSampleTypeList.GetSize(); i++ )
			{
				D3DMULTISAMPLE_TYPE type = pBestDeviceSettingsCombo->multiSampleTypeList.GetAt(i);
				DWORD qualityLevels = pBestDeviceSettingsCombo->multiSampleQualityList.GetAt(i);

				// Check whether supported type is closer to the input than our current best
				if( abs(type - pDeviceSettingsIn->pp.MultiSampleType) < abs(bestMultiSampleType - pDeviceSettingsIn->pp.MultiSampleType) )
				{
					bestMultiSampleType = type; 
					bestMultiSampleQuality = min( qualityLevels-1, pDeviceSettingsIn->pp.MultiSampleQuality );
				}
			}
		}
		else
		{
			// Error case
			bestMultiSampleType = D3DMULTISAMPLE_NONE;
			bestMultiSampleQuality = 0;
		}
	}

	//---------------------
	// Swap effect
	//---------------------
	D3DSWAPEFFECT bestSwapEffect;
	if( pMatchOptions->eSwapEffect == DXUTMT_PRESERVE_INPUT )   
	{
		bestSwapEffect = pDeviceSettingsIn->pp.SwapEffect;
	}
	else if( pMatchOptions->eSwapEffect == DXUTMT_IGNORE_INPUT )   
	{
		bestSwapEffect = D3DSWAPEFFECT_DISCARD;
	}
	else // if( pMatchOptions->eSwapEffect == DXUTMT_CLOSEST_TO_INPUT )   
	{
		bestSwapEffect = pDeviceSettingsIn->pp.SwapEffect;

		// Swap effect has to be one of these 3
		if( bestSwapEffect != D3DSWAPEFFECT_DISCARD &&
			bestSwapEffect != D3DSWAPEFFECT_FLIP &&
			bestSwapEffect != D3DSWAPEFFECT_COPY )
		{
			bestSwapEffect = D3DSWAPEFFECT_DISCARD;
		}
	}

	//---------------------
	// Depth stencil 
	//---------------------
	D3DFORMAT bestDepthStencilFormat;
	BOOL bestEnableAutoDepthStencil;

	CGrowableArray< int > depthStencilRanking;
	depthStencilRanking.SetSize( pBestDeviceSettingsCombo->depthStencilFormatList.GetSize() );

	UINT dwBackBufferBitDepth = DXUTColorChannelBits( pBestDeviceSettingsCombo->BackBufferFormat );       
	UINT dwInputDepthBitDepth = 0;
	if( pDeviceSettingsIn )
		dwInputDepthBitDepth = DXUTDepthBits( pDeviceSettingsIn->pp.AutoDepthStencilFormat );

	for( int i=0; i<pBestDeviceSettingsCombo->depthStencilFormatList.GetSize(); i++ )
	{
		D3DFORMAT curDepthStencilFmt = pBestDeviceSettingsCombo->depthStencilFormatList.GetAt(i);
		DWORD dwCurDepthBitDepth = DXUTDepthBits( curDepthStencilFmt );
		int nRanking;

		if( pMatchOptions->eDepthFormat == DXUTMT_PRESERVE_INPUT )
		{                       
			// Need to match bit depth of input
			if(dwCurDepthBitDepth == dwInputDepthBitDepth)
				nRanking = 0;
			else
				nRanking = 10000;
		}
		else if( pMatchOptions->eDepthFormat == DXUTMT_IGNORE_INPUT )
		{
			// Prefer match of backbuffer bit depth
			nRanking = abs((int)dwCurDepthBitDepth - (int)dwBackBufferBitDepth*4);
		}
		else // if( pMatchOptions->eDepthFormat == DXUTMT_CLOSEST_TO_INPUT )
		{
			// Prefer match of input depth format bit depth
			nRanking = abs((int)dwCurDepthBitDepth - (int)dwInputDepthBitDepth);
		}

		depthStencilRanking.Add( nRanking );
	}

	UINT dwInputStencilBitDepth = 0;
	if( pDeviceSettingsIn )
		dwInputStencilBitDepth = DXUTStencilBits( pDeviceSettingsIn->pp.AutoDepthStencilFormat );

	for( int i=0; i<pBestDeviceSettingsCombo->depthStencilFormatList.GetSize(); i++ )
	{
		D3DFORMAT curDepthStencilFmt = pBestDeviceSettingsCombo->depthStencilFormatList.GetAt(i);
		int nRanking = depthStencilRanking.GetAt(i);
		DWORD dwCurStencilBitDepth = DXUTStencilBits( curDepthStencilFmt );

		if( pMatchOptions->eStencilFormat == DXUTMT_PRESERVE_INPUT )
		{                       
			// Need to match bit depth of input
			if(dwCurStencilBitDepth == dwInputStencilBitDepth)
				nRanking += 0;
			else
				nRanking += 10000;
		}
		else if( pMatchOptions->eStencilFormat == DXUTMT_IGNORE_INPUT )
		{
			// Prefer 0 stencil bit depth
			nRanking += dwCurStencilBitDepth;
		}
		else // if( pMatchOptions->eStencilFormat == DXUTMT_CLOSEST_TO_INPUT )
		{
			// Prefer match of input stencil format bit depth
			nRanking += abs((int)dwCurStencilBitDepth - (int)dwInputStencilBitDepth);
		}

		depthStencilRanking.SetAt( i, nRanking );
	}

	int nBestRanking = 100000;
	int nBestIndex = -1;
	for( int i=0; i<pBestDeviceSettingsCombo->depthStencilFormatList.GetSize(); i++ )
	{
		int nRanking = depthStencilRanking.GetAt(i);
		if( nRanking < nBestRanking )
		{
			nBestRanking = nRanking;
			nBestIndex = i;
		}
	}

	if( nBestIndex >= 0 )
	{
		bestDepthStencilFormat = pBestDeviceSettingsCombo->depthStencilFormatList.GetAt(nBestIndex);
		bestEnableAutoDepthStencil = true;
	}
	else
	{
		bestDepthStencilFormat = D3DFMT_UNKNOWN;
		bestEnableAutoDepthStencil = false;
	}


	//---------------------
	// Present flags
	//---------------------
	DWORD dwBestFlags;
	if( pMatchOptions->ePresentFlags == DXUTMT_PRESERVE_INPUT )   
	{
		dwBestFlags = pDeviceSettingsIn->pp.Flags;
	}
	else if( pMatchOptions->ePresentFlags == DXUTMT_IGNORE_INPUT )   
	{
		dwBestFlags = 0;
		if( bestEnableAutoDepthStencil )
			dwBestFlags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;            
	}
	else // if( pMatchOptions->ePresentFlags == DXUTMT_CLOSEST_TO_INPUT )   
	{
		dwBestFlags = pDeviceSettingsIn->pp.Flags;
		if( bestEnableAutoDepthStencil )
			dwBestFlags |= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	}

	//---------------------
	// Refresh rate
	//---------------------
	if( pBestDeviceSettingsCombo->Windowed )
	{
		// Must be 0 for windowed
		bestDisplayMode.RefreshRate = 0;
	}
	else
	{
		if( pMatchOptions->eRefreshRate == DXUTMT_PRESERVE_INPUT )   
		{
			bestDisplayMode.RefreshRate = pDeviceSettingsIn->pp.FullScreen_RefreshRateInHz;
		}
		else 
		{
			UINT refreshRateMatch;
			if( pMatchOptions->eRefreshRate == DXUTMT_CLOSEST_TO_INPUT )   
			{
				refreshRateMatch = pDeviceSettingsIn->pp.FullScreen_RefreshRateInHz;
			}
			else // if( pMatchOptions->eRefreshRate == DXUTMT_IGNORE_INPUT )   
			{
				refreshRateMatch = adapterDesktopDisplayMode.RefreshRate;
			}

			bestDisplayMode.RefreshRate = 0;

			if( refreshRateMatch != 0 )
			{
				int nBestRefreshRanking = 100000;
				CGrowableArray<D3DDISPLAYMODE>* pDisplayModeList = &pBestDeviceSettingsCombo->pAdapterInfo->displayModeList;
				for( int iDisplayMode=0; iDisplayMode<pDisplayModeList->GetSize(); iDisplayMode++ )
				{
					D3DDISPLAYMODE displayMode = pDisplayModeList->GetAt(iDisplayMode);                
					if( displayMode.Format != pBestDeviceSettingsCombo->AdapterFormat || 
						displayMode.Height != bestDisplayMode.Height ||
						displayMode.Width != bestDisplayMode.Width )
						continue; // Skip display modes that don't match 

					// Find the delta between the current refresh rate and the optimal refresh rate 
					int nCurRanking = abs((int)displayMode.RefreshRate - (int)refreshRateMatch);

					if( nCurRanking < nBestRefreshRanking )
					{
						bestDisplayMode.RefreshRate = displayMode.RefreshRate;
						nBestRefreshRanking = nCurRanking;

						// Stop if perfect match found
						if( nBestRefreshRanking == 0 )
							break;
					}
				}
			}
		}
	}

	//---------------------
	// Present interval
	//---------------------
	UINT bestPresentInterval;
	if( pMatchOptions->ePresentInterval == DXUTMT_PRESERVE_INPUT )   
	{
		bestPresentInterval = pDeviceSettingsIn->pp.PresentationInterval;
	}
	else if( pMatchOptions->ePresentInterval == DXUTMT_IGNORE_INPUT )   
	{
		if( pBestDeviceSettingsCombo->Windowed )
		{
			// For windowed, the framework defaults to D3DPRESENT_INTERVAL_IMMEDIATE
			// which will wait not for the vertical retrace period to prevent tearing, 
			// but may introduce tearing
			bestPresentInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		}
		else
		{
			// For full screen, the framework defaults to D3DPRESENT_INTERVAL_DEFAULT 
			// which will wait for the vertical retrace period to prevent tearing
			bestPresentInterval = D3DPRESENT_INTERVAL_DEFAULT;
		}
	}
	else // if( pMatchOptions->ePresentInterval == DXUTMT_CLOSEST_TO_INPUT )   
	{
		if( pBestDeviceSettingsCombo->presentIntervalList.Contains( pDeviceSettingsIn->pp.PresentationInterval ) )
		{
			bestPresentInterval = pDeviceSettingsIn->pp.PresentationInterval;
		}
		else
		{
			if( pBestDeviceSettingsCombo->Windowed )
				bestPresentInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
			else
				bestPresentInterval = D3DPRESENT_INTERVAL_DEFAULT;
		}
	}

	// Fill the device settings struct
	ZeroMemory( pValidDeviceSettings, sizeof(DXUTDeviceSettings) );
	pValidDeviceSettings->AdapterOrdinal                 = pBestDeviceSettingsCombo->AdapterOrdinal;
	pValidDeviceSettings->DeviceType                     = pBestDeviceSettingsCombo->DeviceType;
	pValidDeviceSettings->AdapterFormat                  = pBestDeviceSettingsCombo->AdapterFormat;
	pValidDeviceSettings->BehaviorFlags                  = dwBestBehaviorFlags;
	pValidDeviceSettings->pp.BackBufferWidth             = bestDisplayMode.Width;
	pValidDeviceSettings->pp.BackBufferHeight            = bestDisplayMode.Height;
	pValidDeviceSettings->pp.BackBufferFormat            = pBestDeviceSettingsCombo->BackBufferFormat;
	pValidDeviceSettings->pp.BackBufferCount             = bestBackBufferCount;
	pValidDeviceSettings->pp.MultiSampleType             = bestMultiSampleType;  
	pValidDeviceSettings->pp.MultiSampleQuality          = bestMultiSampleQuality;
	pValidDeviceSettings->pp.SwapEffect                  = bestSwapEffect;
	pValidDeviceSettings->pp.hDeviceWindow               = pBestDeviceSettingsCombo->Windowed ? DXUTGetHWNDDeviceWindowed() : DXUTGetHWNDDeviceFullScreen();
	pValidDeviceSettings->pp.Windowed                    = pBestDeviceSettingsCombo->Windowed;
	pValidDeviceSettings->pp.EnableAutoDepthStencil      = bestEnableAutoDepthStencil;  
	pValidDeviceSettings->pp.AutoDepthStencilFormat      = bestDepthStencilFormat;
	pValidDeviceSettings->pp.Flags                       = dwBestFlags;                   
	pValidDeviceSettings->pp.FullScreen_RefreshRateInHz  = bestDisplayMode.RefreshRate;
	pValidDeviceSettings->pp.PresentationInterval        = bestPresentInterval;
}


//--------------------------------------------------------------------------------------
// Internal helper function to find the closest allowed display mode to the optimal 
//--------------------------------------------------------------------------------------
HRESULT CTxDxAgent::DXUTFindValidResolution( CD3DEnumDeviceSettingsCombo* pBestDeviceSettingsCombo, 
								D3DDISPLAYMODE displayModeIn, D3DDISPLAYMODE* pBestDisplayMode )
{
	D3DDISPLAYMODE bestDisplayMode;
	ZeroMemory( &bestDisplayMode, sizeof(D3DDISPLAYMODE) );

	if( pBestDeviceSettingsCombo->Windowed )
	{
		// Get the desktop resolution of the current monitor to use to keep the window
		// in a reasonable size in the desktop's 
		// This isn't the same as the current resolution from GetAdapterDisplayMode
		// since the device might be fullscreen 
		CD3DEnumeration* pd3dEnum = DXUTPrepareEnumerationObject();
		CD3DEnumAdapterInfo* pAdapterInfo = pd3dEnum->GetAdapterInfo( pBestDeviceSettingsCombo->AdapterOrdinal );                       
		DEVMODE devMode;
		ZeroMemory( &devMode, sizeof(DEVMODE) );
		devMode.dmSize = sizeof(DEVMODE);
		WCHAR strDeviceName[256];
		MultiByteToWideChar( CP_ACP, 0, pAdapterInfo->AdapterIdentifier.DeviceName, -1, strDeviceName, 256 );
		strDeviceName[255] = 0;
		EnumDisplaySettings( strDeviceName, ENUM_REGISTRY_SETTINGS, &devMode );
		UINT nMonitorWidth = devMode.dmPelsWidth;
		UINT nMonitorHeight = devMode.dmPelsHeight;

		// For windowed mode, just keep it something reasonable within the size 
		// of the working area of the desktop
		if( displayModeIn.Width > nMonitorWidth - 20 )
			displayModeIn.Width = nMonitorWidth - 20;
		if( displayModeIn.Height > nMonitorHeight - 100 )
			displayModeIn.Height = nMonitorHeight - 100;

		*pBestDisplayMode = displayModeIn;
	}
	else
	{
		int nBestRanking = 100000;
		int nCurRanking;
		CGrowableArray<D3DDISPLAYMODE>* pDisplayModeList = &pBestDeviceSettingsCombo->pAdapterInfo->displayModeList;
		for( int iDisplayMode=0; iDisplayMode<pDisplayModeList->GetSize(); iDisplayMode++ )
		{
			D3DDISPLAYMODE displayMode = pDisplayModeList->GetAt(iDisplayMode);

			// Skip display modes that don't match the combo's adapter format
			if( displayMode.Format != pBestDeviceSettingsCombo->AdapterFormat )
				continue;

			// Find the delta between the current width/height and the optimal width/height
			nCurRanking = abs((int)displayMode.Width - (int)displayModeIn.Width) + 
				abs((int)displayMode.Height- (int)displayModeIn.Height);

			if( nCurRanking < nBestRanking )
			{
				bestDisplayMode = displayMode;
				nBestRanking = nCurRanking;

				// Stop if perfect match found
				if( nBestRanking == 0 )
					break;
			}
		}

		if( bestDisplayMode.Width == 0 )
		{
			*pBestDisplayMode = displayModeIn;
			return E_FAIL; // No valid display modes found
		}

		*pBestDisplayMode = bestDisplayMode;
	}

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Internal helper function to return the adapter format from the first device settings 
// combo that matches the passed adapter ordinal, device type, backbuffer format, and windowed.  
//--------------------------------------------------------------------------------------
HRESULT CTxDxAgent::DXUTFindAdapterFormat( UINT AdapterOrdinal, D3DDEVTYPE DeviceType, D3DFORMAT BackBufferFormat, 
							  BOOL Windowed, D3DFORMAT* pAdapterFormat )
{
	CD3DEnumeration* pd3dEnum = DXUTPrepareEnumerationObject();
	CD3DEnumDeviceInfo* pDeviceInfo = pd3dEnum->GetDeviceInfo( AdapterOrdinal, DeviceType );
	if( pDeviceInfo )
	{
		for( int iDeviceCombo=0; iDeviceCombo<pDeviceInfo->deviceSettingsComboList.GetSize(); iDeviceCombo++ )
		{
			CD3DEnumDeviceSettingsCombo* pDeviceSettingsCombo = pDeviceInfo->deviceSettingsComboList.GetAt(iDeviceCombo);
			if( pDeviceSettingsCombo->BackBufferFormat == BackBufferFormat &&
				pDeviceSettingsCombo->Windowed == Windowed )
			{
				// Return the adapter format from the first match
				*pAdapterFormat = pDeviceSettingsCombo->AdapterFormat;
				return S_OK;
			}
		}
	}

	*pAdapterFormat = BackBufferFormat;
	return E_FAIL;
}

bool CTxDxAgent::DXUTDoesDeviceComboMatchPreserveOptions( CD3DEnumDeviceSettingsCombo* pDeviceSettingsCombo, 
											 DXUTDeviceSettings* pDeviceSettingsIn, 
											 DXUTMatchOptions* pMatchOptions )
{
	//---------------------
	// Adapter ordinal
	//---------------------
	if( pMatchOptions->eAdapterOrdinal == DXUTMT_PRESERVE_INPUT && 
		(pDeviceSettingsCombo->AdapterOrdinal != pDeviceSettingsIn->AdapterOrdinal) )
		return false;

	//---------------------
	// Device type
	//---------------------
	if( pMatchOptions->eDeviceType == DXUTMT_PRESERVE_INPUT && 
		(pDeviceSettingsCombo->DeviceType != pDeviceSettingsIn->DeviceType) )
		return false;

	//---------------------
	// Windowed
	//---------------------
	if( pMatchOptions->eWindowed == DXUTMT_PRESERVE_INPUT && 
		(pDeviceSettingsCombo->Windowed != pDeviceSettingsIn->pp.Windowed) )
		return false;

	//---------------------
	// Adapter format
	//---------------------
	if( pMatchOptions->eAdapterFormat == DXUTMT_PRESERVE_INPUT && 
		(pDeviceSettingsCombo->AdapterFormat != pDeviceSettingsIn->AdapterFormat) )
		return false;

	//---------------------
	// Vertex processing
	//---------------------
	// If keep VP and input has HWVP, then skip if this combo doesn't have HWTL 
	if( pMatchOptions->eVertexProcessing == DXUTMT_PRESERVE_INPUT && 
		((pDeviceSettingsIn->BehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING) != 0) && 
		((pDeviceSettingsCombo->pDeviceInfo->Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0) )
		return false;

	//---------------------
	// Resolution
	//---------------------
	// If keep resolution then check that width and height supported by this combo
	if( pMatchOptions->eResolution == DXUTMT_PRESERVE_INPUT )
	{
		bool bFound = false;
		for( int i=0; i< pDeviceSettingsCombo->pAdapterInfo->displayModeList.GetSize(); i++ )
		{
			D3DDISPLAYMODE displayMode = pDeviceSettingsCombo->pAdapterInfo->displayModeList.GetAt( i );
			if( displayMode.Format != pDeviceSettingsCombo->AdapterFormat )
				continue; // Skip this display mode if it doesn't match the combo's adapter format

			if( displayMode.Width == pDeviceSettingsIn->pp.BackBufferWidth &&
				displayMode.Height == pDeviceSettingsIn->pp.BackBufferHeight )
			{
				bFound = true;
				break;
			}
		}

		// If the width and height are not supported by this combo, return false
		if( !bFound )
			return false;
	}

	//---------------------
	// Back buffer format
	//---------------------
	if( pMatchOptions->eBackBufferFormat == DXUTMT_PRESERVE_INPUT && 
		pDeviceSettingsCombo->BackBufferFormat != pDeviceSettingsIn->pp.BackBufferFormat )
		return false;

	//---------------------
	// Back buffer count
	//---------------------
	// No caps for the back buffer count

	//---------------------
	// Multisample
	//---------------------
	if( pMatchOptions->eMultiSample == DXUTMT_PRESERVE_INPUT )
	{
		bool bFound = false;
		for( int i=0; i<pDeviceSettingsCombo->multiSampleTypeList.GetSize(); i++ )
		{
			D3DMULTISAMPLE_TYPE msType = pDeviceSettingsCombo->multiSampleTypeList.GetAt(i);
			DWORD msQuality  = pDeviceSettingsCombo->multiSampleQualityList.GetAt(i);

			if( msType == pDeviceSettingsIn->pp.MultiSampleType &&
				msQuality >= pDeviceSettingsIn->pp.MultiSampleQuality )
			{
				bFound = true;
				break;
			}
		}

		// If multisample type/quality not supported by this combo, then return false
		if( !bFound )
			return false;
	}

	//---------------------
	// Swap effect
	//---------------------
	// No caps for swap effects

	//---------------------
	// Depth stencil 
	//---------------------
	// If keep depth stencil format then check that the depth stencil format is supported by this combo
	if( pMatchOptions->eDepthFormat == DXUTMT_PRESERVE_INPUT &&
		pMatchOptions->eStencilFormat == DXUTMT_PRESERVE_INPUT )
	{
		if( pDeviceSettingsIn->pp.AutoDepthStencilFormat != D3DFMT_UNKNOWN &&
			!pDeviceSettingsCombo->depthStencilFormatList.Contains( pDeviceSettingsIn->pp.AutoDepthStencilFormat ) )
			return false;
	}

	// If keep depth format then check that the depth format is supported by this combo
	if( pMatchOptions->eDepthFormat == DXUTMT_PRESERVE_INPUT &&
		pDeviceSettingsIn->pp.AutoDepthStencilFormat != D3DFMT_UNKNOWN )
	{
		bool bFound = false;
		UINT dwDepthBits = DXUTDepthBits( pDeviceSettingsIn->pp.AutoDepthStencilFormat );
		for( int i=0; i<pDeviceSettingsCombo->depthStencilFormatList.GetSize(); i++ )
		{
			D3DFORMAT depthStencilFmt = pDeviceSettingsCombo->depthStencilFormatList.GetAt(i);
			UINT dwCurDepthBits = DXUTDepthBits( depthStencilFmt );
			if( dwCurDepthBits - dwDepthBits == 0)
				bFound = true;
		}

		if( !bFound )
			return false;
	}

	// If keep depth format then check that the depth format is supported by this combo
	if( pMatchOptions->eStencilFormat == DXUTMT_PRESERVE_INPUT &&
		pDeviceSettingsIn->pp.AutoDepthStencilFormat != D3DFMT_UNKNOWN )
	{
		bool bFound = false;
		UINT dwStencilBits = DXUTStencilBits( pDeviceSettingsIn->pp.AutoDepthStencilFormat );
		for( int i=0; i<pDeviceSettingsCombo->depthStencilFormatList.GetSize(); i++ )
		{
			D3DFORMAT depthStencilFmt = pDeviceSettingsCombo->depthStencilFormatList.GetAt(i);
			UINT dwCurStencilBits = DXUTStencilBits( depthStencilFmt );
			if( dwCurStencilBits - dwStencilBits == 0)
				bFound = true;
		}

		if( !bFound )
			return false;
	}

	//---------------------
	// Present flags
	//---------------------
	// No caps for the present flags

	//---------------------
	// Refresh rate
	//---------------------
	// If keep refresh rate then check that the resolution is supported by this combo
	if( pMatchOptions->eRefreshRate == DXUTMT_PRESERVE_INPUT )
	{
		bool bFound = false;
		for( int i=0; i<pDeviceSettingsCombo->pAdapterInfo->displayModeList.GetSize(); i++ )
		{
			D3DDISPLAYMODE displayMode = pDeviceSettingsCombo->pAdapterInfo->displayModeList.GetAt( i );
			if( displayMode.Format != pDeviceSettingsCombo->AdapterFormat )
				continue;
			if( displayMode.RefreshRate == pDeviceSettingsIn->pp.FullScreen_RefreshRateInHz )
			{
				bFound = true;
				break;
			}
		}

		// If refresh rate not supported by this combo, then return false
		if( !bFound )
			return false;
	}

	//---------------------
	// Present interval
	//---------------------
	// If keep present interval then check that the present interval is supported by this combo
	if( pMatchOptions->ePresentInterval == DXUTMT_PRESERVE_INPUT &&
		!pDeviceSettingsCombo->presentIntervalList.Contains( pDeviceSettingsIn->pp.PresentationInterval ) )
		return false;

	return true;
}

UINT CTxDxAgent::DXUTColorChannelBits( D3DFORMAT fmt )
{
	switch( fmt )
	{
	case D3DFMT_R8G8B8:
		return 8;
	case D3DFMT_A8R8G8B8:
		return 8;
	case D3DFMT_X8R8G8B8:
		return 8;
	case D3DFMT_R5G6B5:
		return 5;
	case D3DFMT_X1R5G5B5:
		return 5;
	case D3DFMT_A1R5G5B5:
		return 5;
	case D3DFMT_A4R4G4B4:
		return 4;
	case D3DFMT_R3G3B2:
		return 2;
	case D3DFMT_A8R3G3B2:
		return 2;
	case D3DFMT_X4R4G4B4:
		return 4;
	case D3DFMT_A2B10G10R10:
		return 10;
	case D3DFMT_A8B8G8R8:
		return 8;
	case D3DFMT_A2R10G10B10:
		return 10;
	case D3DFMT_A16B16G16R16:
		return 16;
	default:
		return 0;
	}
}




//--------------------------------------------------------------------------------------
// Returns the number of depth bits in the specified D3DFORMAT
//--------------------------------------------------------------------------------------
UINT CTxDxAgent::DXUTDepthBits( D3DFORMAT fmt )
{
	switch( fmt )
	{
	case D3DFMT_D32F_LOCKABLE:
	case D3DFMT_D32:
		return 32;

	case D3DFMT_D24X8:
	case D3DFMT_D24S8:
	case D3DFMT_D24X4S4:
	case D3DFMT_D24FS8:
		return 24;

	case D3DFMT_D16_LOCKABLE:
	case D3DFMT_D16:
		return 16;

	case D3DFMT_D15S1:
		return 15;

	default:
		return 0;
	}
}



// Returns the number of stencil bits in the specified D3DFORMAT
//--------------------------------------------------------------------------------------
UINT CTxDxAgent::DXUTStencilBits( D3DFORMAT fmt )
{
	switch( fmt )
	{
	case D3DFMT_D16_LOCKABLE:
	case D3DFMT_D16:
	case D3DFMT_D32F_LOCKABLE:
	case D3DFMT_D32:
	case D3DFMT_D24X8:
		return 0;

	case D3DFMT_D15S1:
		return 1;

	case D3DFMT_D24X4S4:
		return 4;

	case D3DFMT_D24S8:
	case D3DFMT_D24FS8:
		return 8;

	default:
		return 0;
	}
}


