
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "MBCAutoSwitcher.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
extern CMainFrame* g_pMainFrame = NULL;

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	g_pMainFrame = this;
	this->SetMenuBarVisibility(AFX_MBV_DISPLAYONFOCUS );
	// ����һ����ͼ��ռ�ÿ�ܵĹ�����
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("δ�ܴ�����ͼ����\n");
		return -1;
	}

	m_dlgTopPanel.Create(IDD_DIALOG_TOPPANEL, this);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	Relayout();
	m_wndView.SetActiveWindow();

	//extract command
	int nargc = __argc;
	if (nargc  == 2)
	{
		CString strarg1 = __argv[1];
		if (strarg1.CompareNoCase("-autorun") ==0)
		{
			m_dlgTopPanel.OnBnClickedBtnRun();
		}
	}
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

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
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ����ͼ��һ�γ��Ը�����
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// ����ִ��Ĭ�ϴ���
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	//CFrameWnd::OnSize(nType, cx, cy);
	Relayout();
	// TODO: �ڴ˴������Ϣ����������
}

void CMainFrame::Relayout()
{
	if (!m_dlgTopPanel.GetSafeHwnd())
	{
		return;
	}
	CRect rcThis;
	GetClientRect(rcThis);
	CRect rcTop(rcThis);
	rcTop.bottom = rcTop.top + 60;
	m_dlgTopPanel.MoveWindow(rcTop);

	CRect rcView(rcThis);
	rcView.top = rcTop.bottom;
	m_wndView.MoveWindow(rcView);
}

void CMainFrame::OnDestroy()
{
	g_pMainFrame = NULL;
	m_Mongraph.Stop();
	CFrameWnd::OnDestroy();
	
	// TODO: �ڴ˴������Ϣ����������
}
