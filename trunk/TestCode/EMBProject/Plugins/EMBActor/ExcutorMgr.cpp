#include "StdAfx.h"
#include "ExcutorMgr.h"
#include "EMBDefine.h"

LRESULT CALLBACK ExcMgrWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL RegisterExcMgrWnd();
BOOL g_bRegisterExcMgrWnd = RegisterExcMgrWnd();
static TCHAR g_szSockWndClsName[] = TEXT ("ExcMgrMsgWnd");
CExcutorMgr* g_pExcMgr = NULL;

BOOL RegisterExcMgrWnd()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASSEX   wndclassex = {0};
	wndclassex.cbSize        = sizeof(WNDCLASSEX);
	wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc   = ExcMgrWndProc;
	wndclassex.cbClsExtra    = 0;
	wndclassex.cbWndExtra    = 0;
	wndclassex.hInstance     = hInstance;
	wndclassex.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
	wndclassex.hCursor       = LoadCursor (NULL, IDC_ARROW);
	wndclassex.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
	wndclassex.lpszMenuName  = NULL;
	wndclassex.lpszClassName = g_szSockWndClsName;
	wndclassex.hIconSm       = wndclassex.hIcon;

	if ( 0== RegisterClassEx (&wndclassex))
	{
		HRESULT hr = GetLastError();
		ASSERT(FALSE);

	}

	return TRUE;
}

//CreateThreadWindow
DWORD __stdcall ExcMgrMsgLoopThread( void* lparam )
{
	CExcutorMgr* pMgr = (CExcutorMgr*)lparam;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HWND& hwnd = pMgr->m_hMessageWnd;
	ASSERT(hwnd == NULL);
	hwnd = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW, 
		g_szSockWndClsName, 
		TEXT (""),
		WS_OVERLAPPEDWINDOW,
		-10, 
		-10, 
		2, 
		2, 
		NULL, 
		NULL, 
		hInstance,0);

	ASSERT(::IsWindow(hwnd));
	//::SetWindowLong(hwnd,  GWL_USERDATA, (LONG)pMgr);

	//CFWriteLog(TEXT("SockmsgWnd = %x, bind sock %d"), hwnd, this->m_hSock);

	ShowWindow (hwnd, SW_HIDE);
	UpdateWindow (hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK ExcMgrWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case MSG_EMBEXCUTORMSG:
		{	
			if (g_pExcMgr)
			{
				g_pExcMgr->OnExcutorMessage(hwnd, message, wParam, lParam);
			}
			return 0;
		}	
	}
	return DefWindowProc (hwnd, message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////
CExcutorMgr* CExcutorMgr::m_spExcMgr = NULL;
CExcutorMgr::CExcutorMgr(void)
{
	m_hMessageWnd = NULL;
	ASSERT(g_pExcMgr == NULL);
	g_pExcMgr = this;
	
}

CExcutorMgr::~CExcutorMgr(void)
{
	ASSERT(g_pExcMgr == this);
	g_pExcMgr = NULL;
	if (m_hMessageWnd)
	{
		DestroyWindow(m_hMessageWnd);
		m_hMessageWnd = NULL;
	}
}

HRESULT CExcutorMgr::OnExcutorMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return S_OK;
}

CExcutorMgr* CExcutorMgr::GetExcutorMgr()
{
	if (m_spExcMgr == NULL)
	{
		m_spExcMgr = new CExcutorMgr;
	}
	return m_spExcMgr;
}

BOOL CExcutorMgr::Release()
{
	if (m_spExcMgr)
	{
		delete m_spExcMgr;
		m_spExcMgr = NULL;
	}

	return TRUE;
}
