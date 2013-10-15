
// MonitorView.cpp : CMonitorView ���ʵ��
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



// CMonitorView ��Ϣ�������

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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

	// TODO: �ڴ˴������Ϣ����������
}

LRESULT CMonitorView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	return CMBCStateViewWnd::WindowProc(message, wParam, lParam);
}

int CMonitorView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMBCStateViewWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	SetTimer(IDTIMER_STATEGET, 2000, NULL );

	return 0;
}
