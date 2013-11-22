/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	DlgMovedBase.cpp
	file base:	DlgMovedBase
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "DlgMovedBase.h"
#define ID_TIMER_READ_LAN 50

IMPLEMENT_DYNAMIC(CDlgMovedBase, CDialog)
CDlgMovedBase::CDlgMovedBase(UINT nIDTemplate, CWnd* pParentWnd /*= NULL*/)
:CDialog(nIDTemplate, pParentWnd)
{
	m_bDraging = FALSE;
	m_bDragWithWnd = FALSE;
	m_rcDragRect.SetRectEmpty();
}

CDlgMovedBase::~CDlgMovedBase(void)
{
}
BEGIN_MESSAGE_MAP(CDlgMovedBase, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_HELPINFO()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CDlgMovedBase::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_rcDragRect.IsRectEmpty())
	{
		GetWindowRect(m_rcOrg);
		GetCursorPos(&m_ptBeforeDrag);
		CRect rcDragScreen(m_rcDragRect);
		ClientToScreen(&rcDragScreen);
		if (rcDragScreen.PtInRect(m_ptBeforeDrag))
		{
			m_rcPre = m_rcOrg;
			m_ptPre = m_ptBeforeDrag;
			m_bDraging = TRUE;
			SetCapture();
			if (!m_bDragWithWnd)
			{
				DrawDragRect(&m_rcPre, &m_rcPre, 0);
			}
		}
		
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CDlgMovedBase::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bDraging)
	{
		return CDialog::OnLButtonUp(nFlags, point);;
	}

	m_bDraging = FALSE;
	ReleaseCapture();
	if (!m_bDragWithWnd)
	{
		DrawDragRect(&m_rcPre, &m_rcPre, 2);
	}

	CRect rcNext = m_rcOrg;
	CPoint ptCur;
	GetCursorPos(&ptCur);
	rcNext.OffsetRect(ptCur - m_ptBeforeDrag);

	MoveWindow(rcNext);
	//Invalidate(TRUE);
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgMovedBase::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!m_bDraging)
	{
		return CDialog::OnMouseMove(nFlags, point);
	}

	CRect rcNext = m_rcPre;
	CPoint ptCur;
	GetCursorPos(&ptCur);
	rcNext.OffsetRect(ptCur - m_ptPre);
	if (!m_bDragWithWnd)
	{
		DrawDragRect(&m_rcPre, &rcNext);
	}
	else
	{
		SetWindowPos(NULL, rcNext.left, rcNext.top, 0,0, SWP_NOSIZE);
	}
	m_rcPre= rcNext;
	m_ptPre = ptCur;
	CDialog::OnMouseMove(nFlags, point);
}


void CDlgMovedBase::DrawDragRect( CRect* pRcBefore, CRect* pRcAfter , int nMode /*= 1*/)
{
	//CPaintDC dc(GetDesktopWindow()); //this will failed, why
	CSize szBefor(2,2);
	CSize szAfter(2,2);
	if (nMode == 2)
	{
		szAfter = CSize(0,0);
	}
	else if (nMode == 0)
	{
		szBefor = CSize(0,0);
	}

	CDC *pDC = GetDesktopWindow()->GetWindowDC();
	ASSERT(pDC != NULL);
	pDC->DrawDragRect(pRcAfter, szAfter, pRcBefore,szBefor );
	GetDesktopWindow()->ReleaseDC(pDC);
}

BOOL CDlgMovedBase::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CDlgMovedBase::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
}

void CDlgMovedBase::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

BOOL CDlgMovedBase::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CDialog::OnHelpInfo(pHelpInfo);
}


BOOL CDlgMovedBase::PreTranslateMessage( MSG* pMsg )
{
	if (pMsg->message == WM_KEYDOWN 
		&& (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_F1 || pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE )
	{
		CWnd* pWnd = GetFocus();
		if ( !(pWnd && (pWnd->IsKindOf(RUNTIME_CLASS(CEdit)))) )
		{
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
