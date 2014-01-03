#include "StdAfx.h"
#include "TxUIContainer.h"
HWND g_hwnd = NULL;
CTxUIContainer* CTxUIContainer::m_spContainer = NULL;
//////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK TxAgentStaticWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	ASSERT(g_hwnd != NULL);
	if (hWnd == g_hwnd)
	{
		return CTxUIContainer.GetContainer()->OnContainerWndProc(hWnd, uMsg, wParam, lParam);
	}
}
//////////////////////////////////////////////////////////////////////////
CTxUIContainer* CTxUIContainer::AttachUIWnd( HWND hwnd )
{
	if (m_spContainer)
	{
		return NULL;
	}
	m_spContainer = new CTxUIContainer;
	m_spContainer->Init(hwnd);
	return m_spContainer;
}
//////////////////////////////////////////////////////////////////////////
CTxUIContainer::CTxUIContainer(void)
{
	m_pOldWndProc = NULL;
	m_hwnd = NULL;
}

CTxUIContainer::~CTxUIContainer(void)
{
}

CTxUIContainer* CTxUIContainer::GetContainer()
{
	return m_spContainer;
}



BOOL CTxUIContainer::Init( HWND hwnd )
{
	ASSERT(g_hwnd == NULL);
	g_hwnd = hwnd;
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
		m_pOldWndProc = (FNTXCONTAINERWNDPROC)nResult;
	}

	return TRUE;
}

BOOL CTxUIContainer::UnInit()
{
	if (m_pOldWndProc)
	{

#ifdef _WIN64
		LONG_PTR nResult = SetWindowLongPtr( m_hwnd, GWLP_WNDPROC, (LONG_PTR)m_pOldWndProc );
#else
		LONG_PTR nResult = SetWindowLongPtr( m_hwnd, GWLP_WNDPROC, (LONG)(LONG_PTR)m_pOldWndProc );
#endif 
	}

}

LRESULT CTxUIContainer::OnContainerWndProc( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_ERASEBKGND:
		break;
	case WM_PAINT:
		break;
		case wm_
	}
}

