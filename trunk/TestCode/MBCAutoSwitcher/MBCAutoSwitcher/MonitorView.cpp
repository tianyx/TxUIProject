
// MonitorView.cpp : CMonitorView 类的实现
//

#include "stdafx.h"
#include "MBCAutoSwitcher.h"
#include "MonitorView.h"
#include "GlobalDef.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IDTIMER_STATEGET 1001

// CMonitorView

CMonitorView::CMonitorView()
{
}

CMonitorView::~CMonitorView()
{
}


BEGIN_MESSAGE_MAP(CMonitorView, CMBCStateViewWnd)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()



// CMonitorView 消息处理程序

BOOL CMonitorView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;	
}


void CMonitorView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == IDTIMER_STATEGET)
	{
		if (g_pMainFrame)
		{
			CAutoLock lock(&m_lockgs);
			g_pMainFrame->m_Mongraph.GetGraphState(m_gsState);
			PostMessage(MSG_UPDATE_GRAPHSTATE);
		}
		return;
	}

	CMBCStateViewWnd::OnTimer(nIDEvent);
}

void CMonitorView::OnDestroy()
{
	CMBCStateViewWnd::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

LRESULT CMonitorView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CMBCStateViewWnd::WindowProc(message, wParam, lParam);
}

int CMonitorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMBCStateViewWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	SetTimer(IDTIMER_STATEGET, 2000, NULL );

	return 0;
}
