// OutputView.cpp : ʵ���ļ�
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


// COutputView ��ͼ

void COutputView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: �ڴ���ӻ��ƴ���
}


// COutputView ���

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


// COutputView ��Ϣ�������

int COutputView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_dlgOutput.Create(CDlgOutput::IDD, this);
	return 0;
}

void COutputView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	// TODO: �ڴ˴������Ϣ����������
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

	// TODO: �ڴ˴������Ϣ����������
}
