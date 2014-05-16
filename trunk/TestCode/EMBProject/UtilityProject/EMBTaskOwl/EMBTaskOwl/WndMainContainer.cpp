// WndMainContainer.cpp : 实现文件
//

#include "stdafx.h"
#include "EmbTaskOwl.h"
#include "CMainView.h"
#include "WndMainContainer.h"
#include "OutputView.h"


// CWndMainContainer

IMPLEMENT_DYNAMIC(CWndMainContainer, CWnd)

CWndMainContainer::CWndMainContainer()
{

}

CWndMainContainer::~CWndMainContainer()
{
}


BEGIN_MESSAGE_MAP(CWndMainContainer, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CWndMainContainer 消息处理程序



int CWndMainContainer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	BOOL bRet = m_wndView.CreateStatic(this,
		2, 1     // TODO: adjust the number of rows, columns
		);
	ASSERT(bRet);
	bRet = m_wndView.CreateView(0,0, RUNTIME_CLASS(CMainView),CSize(100,100), NULL);
	bRet = m_wndView.CreateView(1,0, RUNTIME_CLASS(COutputView),CSize(100,100), NULL);
	CRect rcThis;
	GetClientRect(rcThis);
	m_wndView.SetRowInfo(0,rcThis.Height()- 150, 10);
	//m_wndView.SetRowInfo(1,100, 10);
	return 0;
}

void CWndMainContainer::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (!m_wndView.GetSafeHwnd())
	{
		return;
	}
	CRect rcthis;
	GetClientRect(rcthis);
	m_wndView.MoveWindow(rcthis);
	m_wndView.SetRowInfo(0,rcthis.Height()- 150, 10);
	// TODO: 在此处添加消息处理程序代码
}
