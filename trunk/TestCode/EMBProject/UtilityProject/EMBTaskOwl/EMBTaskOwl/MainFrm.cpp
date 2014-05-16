
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "EmbTaskOwl.h"

#include "MainFrm.h"
#include "globaldef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	m_pMainView = NULL;
	m_pOutputView = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ����һ����ͼ��ռ�ÿ�ܵĹ�����
	m_dlgTopbar.Create(CDlgTopBar::IDD, this);
	CRect rc(0,0,200,400);
	m_wndView.Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rc, this, 1001);
	m_pOutputView = (COutputView*)m_wndView.m_wndView.GetPane(1,0);
	m_pMainView = (CMainView*)m_wndView.m_wndView.GetPane(0,0);

	//m_wndView.ShowWindow(SW_HIDE);
	Relayout();
	//load task template file
	CFWriteLog(TEXT("loading emb task template..."));
	//
	g_GlobalInfo.bIsTemplateOk = LoadEmbTemplate();
	//load stg file;
	CFWriteLog(TEXT("loading strategy..."));
	g_GlobalInfo.bIsStrategyOk = LoadStgConfig();

	if (!(g_GlobalInfo.bIsTemplateOk && g_GlobalInfo.bIsStrategyOk))
	{
		m_pMainView->m_dlgAuto.UpdateAutoMsg(TEXT("���Գ�ʼ��ʧ�ܣ��Զ������޷�ʹ�ã�"));
		m_pMainView->m_dlgAuto.m_btnRun.EnableWindow(FALSE);
	}
	else
	{
		m_pMainView->m_dlgAuto.UpdateAutoMsg(TEXT("����"));
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

// 	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
// 		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
// 
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// ������ǰ�Ƶ���ͼ����
	if (m_wndView.GetSafeHwnd())
	{
		m_wndView.SetFocus();

	}
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ����ͼ��һ�γ��Ը�����
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// ����ִ��Ĭ�ϴ���
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
	

	return CFrameWnd::OnCreateClient(lpcs, pContext);
	
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	
	Relayout();
	// TODO: �ڴ˴������Ϣ����������
}

void CMainFrame::Relayout()
{
	if (!m_dlgTopbar.GetSafeHwnd())
	{
		return;
	}
	CRect rcThis;
	GetClientRect(rcThis);
	CRect rcTop(rcThis);
	rcTop.bottom = rcTop.top + 80;
	m_dlgTopbar.MoveWindow(rcTop);

	CRect rcView(rcThis);
	rcView.top = rcTop.bottom;
	m_wndView.MoveWindow(rcView);
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	lpMMI->ptMinTrackSize.x = 720;
	lpMMI->ptMinTrackSize.y = 480;
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE ||  pMsg->wParam == VK_F1
			||pMsg ->wParam == VK_RETURN)
		{
			return TRUE;
		}

		if (pMsg->wParam == VK_F11 && ((GetKeyState(VK_CONTROL) & 0x8000) != 0)
			&& ((GetKeyState(VK_LSHIFT) & 0x8000) != 0))
		{
			static BOOL  bShowDbg = TRUE;
			bShowDbg = !bShowDbg;
			//show debug
			HWND hwndCnol =GetConsoleWindow();
			if (hwndCnol)
			{
				::ShowWindow(hwndCnol, bShowDbg? SW_SHOW:SW_HIDE);

			}
			m_dlgTopbar.m_btnTest.ShowWindow(bShowDbg? SW_SHOW:SW_HIDE);
			g_GlobalInfo.bEnableTaskTrace = bShowDbg;
		}
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}
