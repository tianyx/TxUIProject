
// GraphStateView.cpp : CGraphStateView ���ʵ��
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



// CGraphStateView ��Ϣ�������

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
	CPaintDC dc(this); // ���ڻ��Ƶ��豸������
	
	// TODO: �ڴ˴������Ϣ����������
	
	// ��ҪΪ������Ϣ������ CWnd::OnPaint()
}

HRESULT CGraphStateView::SetGraphState( ST_GRAPHSTATE& gstateIn )
{
	CAutoLock lock(&m_lockgs);
	m_gsState = gstateIn;
	return S_OK;
}

