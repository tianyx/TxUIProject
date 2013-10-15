// TestTxObj.cpp : 实现文件
//

#include "stdafx.h"
#include "TxUITestFrame.h"
#include "TestTxObj.h"


// CTestTxObj

IMPLEMENT_DYNAMIC(CTestTxObj, CTxObjectBase)

CTestTxObj::CTestTxObj()
{
	m_bMouseEnter = FALSE;
}

CTestTxObj::~CTestTxObj()
{
}

// CTestTxObj 消息处理程序


void CTestTxObj::Draw( CDC* pDC )
{
	if (!IsVisable())
	{
		return;
	}
	CRect rcThis = GetInWndRect();

	COLORREF clrBk = RGB(0,0,255);
	if (IsInState(TS_MOUSEOVER))
	{
		clrBk = RGB(0,255,0);
	}

	pDC->FillSolidRect(rcThis, clrBk);

}

HRESULT CTestTxObj::DefaultMsgProc( UINT nMsg, WPARAM wparam, LPARAM lparam )
{
	if (nMsg == WM_SIZE)
	{
		if (m_childObj.IsWindow())
		{
			CRect rc(0,0,100,100);
			m_childObj.MoveWindow(rc);
		}
	}
	if (nMsg == WM_CREATE)
	{
		m_childObj.Create(this);
	}
	
	HRESULT hr = CTxObjectBase::DefaultMsgProc(nMsg, wparam, lparam);

	if (nMsg == WM_MOUSELEAVE)
	{
		m_bMouseEnter = FALSE;
		Invalidate();
	}
	if (nMsg == WM_MOUSEMOVE)
	{
		if (!m_bMouseEnter)
		{
			m_bMouseEnter = TRUE;
			Invalidate();
		}
	}

	return hr;

}
