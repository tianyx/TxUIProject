// TxObjectBase.cpp : 实现文件
//

#include "stdafx.h"
#include "TxObjectBase.h"
#include "TxBaseObjectMgr.h"
#include "TxWndContainer.h"


//////////////////////////////////////////////////////////////////////////
// CTxObjectBase
IMPLEMENT_DYNAMIC(CTxObjectBase, CObject)

CTxObjectBase::CTxObjectBase():m_ptOffSet(0,0),m_ptOffSetWnd(0,0),m_szSize(0,0)
{
	m_nId = 0;
	m_hWnd = 0;
	m_pParent = NULL;
	m_nLevel = 0;
	m_nObjState = TS_INIT_STATE;
	m_nObjStyle = WS_VISIBLE;
	m_bAddtoObjMgr = TRUE;
}

CTxObjectBase::~CTxObjectBase()
{

}

HRESULT CTxObjectBase::DefaultMsgProc(UINT nMsg, WPARAM wparam, LPARAM lparam )
{
	switch (nMsg)
	{
	case WM_MOUSEMOVE:
		{
			if (!IsInState(TS_MOUSEOVER))
			{
				ModifyState(TS_MOUSEOVER, 0);
			}
		}break;
	case WM_MOUSELEAVE:
		{
			ModifyState(0, TS_MOUSEOVER);
		}
		break;
	case WM_DESTROY:
		{
			if (IsInState(TS_SETCAPTURED))
			{
				ReleaseCapture();
			}
		}
	}
	return 0;
}

void CTxObjectBase::Draw( CDC* pDC )
{
}


BOOL CTxObjectBase::Create( CTxObjectBase* pParent, ULONG nInitStyle /*= WS_VISIBLE*/, ULONG nInitState /*= TS_INIT_STATE*/ )
{
	if (pParent == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_pParent = pParent;
	m_hWnd = pParent->m_hWnd;
	m_nLevel = pParent->m_nLevel +1; //must init level first

	if (m_bAddtoObjMgr)
	{
		CTxWndContainer* pContainer = NULL;
		TXMACRO_GETCONTAINER(pContainer);
		ASSERT(pContainer);
		m_nId = pContainer->GetObjMgr()->AddObject(this);
		ASSERT(m_nId != 0);
	}
	m_nObjState = nInitState;
	m_nObjStyle = nInitStyle;
	m_ptOffSetWnd = pParent->m_ptOffSetWnd;
	m_ptOffSet = pParent->m_ptOffSet;
	pParent->AddChild(this);

	DefaultMsgProc(WM_CREATE, 0,0);
	return TRUE;
}


BOOL CTxObjectBase::CreateBase( HWND hwnd, ULONG nInitStyle /*= WS_VISIBLE*/, ULONG nInitState /*= TS_INIT_STATE*/ )
{
	if (hwnd == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_hWnd = hwnd;
	m_nLevel = 0;	//must init level first

	CTxWndContainer* pContainer = NULL;
	TXMACRO_GETCONTAINER(pContainer);
	m_nId = pContainer->GetObjMgr()->AddObject(this);
	ASSERT(m_nId != 0);

	m_nObjState = nInitState;
	m_nObjStyle = nInitStyle;

	DefaultMsgProc(WM_CREATE, 0,0);
	return TRUE;
}

void CTxObjectBase::MoveWindow( CRect& rcIn )
{
	CRect oldrc = GetInWndRect();
	CPoint ptDeta(rcIn.left - m_ptOffSet.x, rcIn.top - m_ptOffSet.y);
	m_ptOffSetWnd +=ptDeta;
	m_ptOffSet +=ptDeta;
	m_szSize = rcIn.Size();


	if (oldrc.Size() != rcIn.Size())
	{
		DefaultMsgProc(WM_SIZE, 0,0);
	}
	if (oldrc != rcIn)
	{
		InvalidateRect( m_hWnd, &oldrc, FALSE);
		InvalidateRect(m_hWnd, &GetInWndRect(), FALSE);
	}
	
}

void CTxObjectBase::ModifyStyle( ULONG nAdded, ULONG nRemoved )
{
	if (nRemoved != 0)
	{
		m_nObjStyle &= ~nRemoved;
	}
	if (nAdded != 0)
	{
		m_nObjStyle |=  nAdded;
	}
}

void CTxObjectBase::ModifyState( ULONG nAdded, ULONG nRemoved )
{
	if (nRemoved != 0)
	{
		m_nObjState &= ~nRemoved;
	}
	if (nAdded != 0)
	{
		m_nObjState |= nAdded;
	}
}

void CTxObjectBase::AddChild( CTxObjectBase* pChild )
{
	m_mapChildObj[pChild->m_nId] = pChild;
}

void CTxObjectBase::SetCapture()
{
	CTxWndContainer *pContaienr = NULL;
	TXMACRO_GETCONTAINER(pContaienr);
	ASSERT(pContaienr);
	pContaienr->SetTxObjCapture(this);
	ModifyState(TS_SETCAPTURED, 0);
}

void CTxObjectBase::ReleaseCapture()
{
	CTxWndContainer *pContaienr = NULL;
	TXMACRO_GETCONTAINER(pContaienr);
	ASSERT(pContaienr);
	pContaienr->ReleaseTxObjCapture(this);
	ModifyState(0, TS_SETCAPTURED);
}



// CTxObjectBase 成员函数
