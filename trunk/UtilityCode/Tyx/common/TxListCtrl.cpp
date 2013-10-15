/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxListCtrl.cpp
	file base:	TxListCtrl
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
// TxListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "ClipImportMonitor.h"
#include "TxListCtrl.h"


// CTxListCtrl

IMPLEMENT_DYNAMIC(CTxListCtrl, CTxScrollWnd)

CTxListCtrl::CTxListCtrl():m_headerCtrl(this)
{

}

CTxListCtrl::~CTxListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTxListCtrl, CTxScrollWnd)
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CTxListCtrl 消息处理程序



LRESULT CTxListCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == MSG_TXHEADER_WIDTHCHANGED)
	{
		if (m_headerCtrl.IsFitView())
		{
			ASSERT(FALSE);
		}
		else
		{
			CSize szView = GetScrollViewSize();
			m_szScrollView.cx = m_headerCtrl.GetHeaderWidth();
			SetScrollViewSize(szView);
		}
	}
	else
	{
		MSG msg;
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;
		m_headerCtrl.RelayMsg(&msg);
	}
	return CTxScrollWnd::WindowProc(message, wParam, lParam);
}

void CTxListCtrl::TxGetClientRect( CRect& rcClient )
{
	CWnd::GetClientRect(rcClient);
	if (m_headerCtrl.IsEnable())
	{
		rcClient.DeflateRect(0,m_headerCtrl.GetHeaderHeight(), 0,0);
	}
}

BOOL CTxListCtrl::OnEraseBkgnd( CDC* pDC )
{
	BOOL bRet = CTxScrollWnd::OnEraseBkgnd(pDC);
	GetListHeader().Draw(pDC);
	return bRet;
}

void CTxListCtrl::SetScrollViewSize( CSize szView )
{
	m_headerCtrl.SetViewWidth(szView.cx);
	CTxScrollWnd::SetScrollViewSize(szView);
}
