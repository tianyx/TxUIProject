// OutputView.cpp : 实现文件
//

#include "stdafx.h"
#include "EmbTaskOwl.h"
#include "OutputView.h"


// COutputView

IMPLEMENT_DYNCREATE(COutputView, CView)

COutputView::COutputView()
{

}

COutputView::~COutputView()
{
}

BEGIN_MESSAGE_MAP(COutputView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// COutputView 绘图

void COutputView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
}


// COutputView 诊断

#ifdef _DEBUG
void COutputView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void COutputView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// COutputView 消息处理程序

int COutputView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_dlgOutput.Create(CDlgOutput::IDD, this);
	return 0;
}

void COutputView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	// TODO: 在此处添加消息处理程序代码
	if (!m_dlgOutput.GetSafeHwnd())
	{
		return;
	}

	CRect rcthis;
	GetClientRect(rcthis);
	m_dlgOutput.MoveWindow(rcthis);
}

void COutputView::OnSetFocus(CWnd* pOldWnd)
{
	if (m_dlgOutput.GetSafeHwnd())
	{
		m_dlgOutput.SetFocus();
	}
	//CView::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码
}
