/********************************************************************
	created:	2012/08/03
	created:	3:8:2012   8:38
	filename: 	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame\TxWndContainer.cpp
	file path:	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame
	file base:	TxWndContainer
	file ext:	cpp
	author:		tian_yx
	
	purpose:	
*********************************************************************/
// TxWndContainer.cpp : 实现文件
//



#include "stdafx.h"
#include "TxWndContainer.h"
#include "TxObjectBase.h"

// CTxWndContainer

IMPLEMENT_DYNAMIC(CTxWndContainer, CWnd)

CTxWndContainer::CTxWndContainer()
{
	m_pFocusObj = NULL;
	m_bInFocus = FALSE;
	m_bMouseOver = FALSE;
	m_bSetLeaveTrack = FALSE;
	m_pCaptureObj = NULL;
	m_pTxAgent = CreateTxAgent();
}

CTxWndContainer::~CTxWndContainer()
{
}


BEGIN_MESSAGE_MAP(CTxWndContainer, CWnd)
END_MESSAGE_MAP()



// CTxWndContainer 消息处理程序



LRESULT CTxWndContainer::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (E_NOTIMPL == m_msgHander.HandleMsg(message, wParam, lParam))
	{
		//process message that need hittest
		switch (message)
		{
		case WM_NCDESTROY:
			{
				if (m_pTxAgent)
				{
					m_pTxAgent->Release();
				}
			}break;
		case WM_CREATE:
			{
				if (m_pTxAgent)
				{
					m_pTxAgent->Init(GetSafeHwnd(), this);
				}
			}break;
		case WM_MOUSEMOVE:
			{
				if (m_pCaptureObj != NULL)
				{
					//docapture process
					m_pCaptureObj->DefaultMsgProc(message, wParam, lParam);
				}
				else
				{
					m_bMouseOver = TRUE;
					if (!m_bSetLeaveTrack)
					{
						TRACKMOUSEEVENT msevent;
						msevent.cbSize = sizeof(msevent);
						msevent.dwFlags =TME_LEAVE;
						msevent.dwHoverTime = HOVER_DEFAULT;
						msevent.hwndTrack = this->GetSafeHwnd();
						BOOL bSuc =TrackMouseEvent(&msevent);
					}
				}
			}
		case WM_MOUSEHWHEEL:
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONUP:
		case WM_RBUTTONDOWN:
			{
				if (m_pCaptureObj != NULL)
				{
					//docapture process
					m_pCaptureObj->DefaultMsgProc(message, wParam, lParam);
				}
				else
				{
					CPoint pt;
					GetCursorPos(&pt);
					ScreenToClient(&pt);
					CTxObjectBase* pObjLastHited = NULL;
					CTxObjectBase* pObj = m_objMgr.HitTestObject(pt, pObjLastHited);

					if (pObjLastHited && pObjLastHited != pObj)
					{
						//send leave/deactive/
						SendStateChangeMsg(message, wParam, lParam, pObjLastHited, pObj);
					}

					while (pObj && pObj->DefaultMsgProc(message, wParam, lParam) == 0)
					{
						//send msg to parent
						pObj = pObj->GetParent();
					}
				}
				
			}break;
		case WM_MOUSELEAVE:
			{
				m_bSetLeaveTrack = FALSE;
				m_bMouseOver = FALSE;
				SendStateChangeMsg(message, wParam, lParam, m_objMgr.GetLastHited(), NULL);
			}break;
		case WM_ACTIVATE:
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			{
				SendStateChangeMsg(message, wParam, lParam, NULL, m_pFocusObj);
			}break;
		case  WM_PAINT:
		case  WM_ERASEBKGND:
			{
				
				CRect rcThis;;
				GetClientRect(rcThis);
				CPaintDC dc(this);
				CDC dcMem;
				dcMem.CreateCompatibleDC(&dc);
				ASSERT(dcMem.GetSafeHdc());
				HGDIOBJ hOldbmp = dcMem.SelectObject(m_bmpforDraw);
				m_objMgr.DoAction(message, (WPARAM)(&dcMem), lParam);
				dc.BitBlt(0,0, rcThis.Width(), rcThis.Height(), &dcMem, 0,0,SRCCOPY);
				dcMem.SelectObject(hOldbmp);
				if (m_pTxAgent)
				{
					return CWnd::WindowProc(message, wParam, lParam);
				}
			}
			break;
		case WM_SIZE:
			{
				CRect rcThis;;
				GetClientRect(rcThis);
				if (!rcThis.IsRectEmpty())
				{
					ResizeDrawBitmap(rcThis.Size());
				}
			}break;
		}
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

BOOL CTxWndContainer::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	return CWnd::PreTranslateMessage(pMsg);
}

HRESULT CTxWndContainer::SendStateChangeMsg( UINT nOrgMsg, WPARAM wparam, LPARAM lparam, CTxObjectBase* pOldObj, CTxObjectBase* pNewObj )
{
	switch(nOrgMsg)
	{
// 	case WM_ACTIVATE:
// 		{
// 		
// 			if (m_pFocusObj && m_pFocusObj->IsVisable())
// 			{
// 				wparam != WA_INACTIVE?m_pFocusObj->ModifyState(TS_ACTIVED, 0):m_pFocusObj->ModifyState(0,TS_ACTIVED);
// 			}
// 		}break;
	case WM_MOUSELEAVE:
		{
			if (pOldObj)
			{
				pOldObj->DefaultMsgProc(WM_MOUSELEAVE, 0,0);
			}
		}break;
	case WM_MOUSEMOVE:
		{
			if (pOldObj && (pNewObj != pOldObj))
			{
				pOldObj->DefaultMsgProc(WM_MOUSELEAVE, 0,0);
			}
		}break;

	case WM_SETFOCUS:
	case WM_KILLFOCUS:
		{
			if (m_pFocusObj)
			{
				m_pFocusObj->DefaultMsgProc(nOrgMsg, 0,0);
			}
		}break;

	case WM_LBUTTONDOWN:
		{
			if (pNewObj && (m_pFocusObj != pNewObj))
			{
				if (m_pFocusObj)
				{
					m_pFocusObj->DefaultMsgProc(WM_KILLFOCUS, 0,0);
				}
				m_pFocusObj = pNewObj;
				m_pFocusObj->DefaultMsgProc(WM_SETFOCUS, 0,0);

			}
		}break;
	}
	return TRUE;
}

void CTxWndContainer::SetTxObjCapture( CTxObjectBase* pCaptureObj )
{
	if (m_pCaptureObj != NULL)
	{
		ASSERT(FALSE);
	}
	m_pCaptureObj = pCaptureObj;
	SetCapture();
}

void CTxWndContainer::ReleaseTxObjCapture( CTxObjectBase* pCaptureObj )
{
	ASSERT(pCaptureObj == m_pCaptureObj);
	m_pCaptureObj = NULL;
	ReleaseCapture();
}

void CTxWndContainer::ResizeDrawBitmap( CSize& sizeIn )
{
	if (m_bmpforDraw.GetSafeHandle())
	{
		CSize szBmp = m_bmpforDraw.GetBitmapDimension();
		if (szBmp == sizeIn)
		{
			return;
		}
		else
		{
			m_bmpforDraw.DeleteObject();
		}
	}

	m_bmpforDraw.CreateBitmap(sizeIn.cx, sizeIn.cy,1, 32, NULL);
	ASSERT(m_bmpforDraw.GetSafeHandle());
}

HRESULT CTxWndContainer::Tx3DDraw( ITxDrawBase* pIDraw )
{
	CRect rc;
	GetClientRect(rc);
	pIDraw->FillSolidRect(rc, TXMAKEARGB(255,255,0,0));
	
	return 0;
}
