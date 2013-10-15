
// GraphStateView.cpp : CGraphStateView 类的实现
//

#include "stdafx.h"
#include "MBCMonitor.h"
#include "GraphStateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGraphStateView

CGraphStateView::CGraphStateView()
{
}

CGraphStateView::~CGraphStateView()
{
}


BEGIN_MESSAGE_MAP(CGraphStateView, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CGraphStateView 消息处理程序

BOOL CGraphStateView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CGraphStateView::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO: 在此处添加消息处理程序代码
	
	// 不要为绘制消息而调用 CWnd::OnPaint()
}

HRESULT CGraphStateView::SetGraphState( ST_GRAPHSTATE& gstateIn )
{
	CAutoLock lock(&m_lockgs);
	m_gsState = gstateIn;
	return S_OK;
}

