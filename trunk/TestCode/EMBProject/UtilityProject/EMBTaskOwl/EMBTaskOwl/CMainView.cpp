
// CMainView.cpp : CMainView 类的实现
//

#include "stdafx.h"
#include "EmbTaskOwl.h"
#include "CMainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainView
IMPLEMENT_DYNCREATE(CMainView, CView)

CMainView::CMainView()
{
	m_nCurrTab = -1;
}

CMainView::~CMainView()
{
}


BEGIN_MESSAGE_MAP(CMainView, CView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CMainView 消息处理程序

BOOL CMainView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CView::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CMainView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO: 在此处添加消息处理程序代码
	
	// 不要为绘制消息而调用 CWnd::OnPaint()
}


int CMainView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_dlgAuto.Create(CDlgAutoTask::IDD, this);
	m_dlgManual.Create(CDlgManualTask::IDD, this);
	m_dlgPlaylist.Create(CDlgPlaylistTask::IDD, this);

	m_vTabs.push_back(m_dlgAuto.GetSafeHwnd());
	m_vTabs.push_back(m_dlgManual.GetSafeHwnd());
	m_vTabs.push_back(m_dlgPlaylist.GetSafeHwnd());

	ActiveTab(0);
	return 0;
}

void CMainView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if (!m_dlgManual.GetSafeHwnd())
	{
		return;
	}
	CRect rcthis;
	GetClientRect(rcthis);
	m_dlgManual.MoveWindow(rcthis);
	m_dlgAuto.MoveWindow(rcthis);
	m_dlgPlaylist.MoveWindow(rcthis);
	// TODO: 在此处添加消息处理程序代码
}

void CMainView::ActiveTab( const int nTabId )
{
	if (m_nCurrTab == nTabId)
	{
		return;
	}

	if (nTabId < 0 || nTabId >= m_vTabs.size() )
	{
		return;
	}

	AlertIfAutoRunning();

	for (size_t i = 0; i < m_vTabs.size(); ++i)
	{
		::ShowWindow(m_vTabs[i], i == nTabId? SW_SHOW:SW_HIDE);
	}
	m_nCurrTab = nTabId;
}

void CMainView::OnDestroy()
{
	m_vTabs.clear();
	m_nCurrTab = -1;
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CMainView::OnDraw( CDC* pDC )
{

}

BOOL CMainView::AlertIfAutoRunning()
{
	if (g_GlobalInfo.bShowManualAlert
		&&m_dlgAuto.IsRunning())
	{
		AfxMessageBox(TEXT("自动任务正在运行,为确保查询结果准确，在执行手动任务时请先停止自动任务。"));
	}
	return TRUE;
}
