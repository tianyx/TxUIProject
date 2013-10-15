#include "StdAfx.h"
#include "TxBaseObjectMgr.h"
#include "TxObjectBase.h"
#pragma comment(lib, "Rpcrt4.lib")


CTxBaseObjectMgr::CTxBaseObjectMgr(void):m_mutex(NULL, GenGuid())
{
	m_pLastHitedObj = NULL;
	m_pFirstObj = NULL;
}

CTxBaseObjectMgr::~CTxBaseObjectMgr(void)
{
}

IDTXOBJ CTxBaseObjectMgr::AddObject(CTxObjectBase* pObj )
{
	ASSERT(pObj);
	int nID = m_IdGenMgr.GenTxObjID(pObj->m_nLevel);
	pObj->m_nId = nID;
	m_mutex.Lock();
	m_mapObj[pObj->m_nId] = pObj;
	m_mutex.Unlock();
	if (!m_pFirstObj)
	{
		m_pFirstObj = pObj;
	}
	return nID;
}

int CTxBaseObjectMgr::DeleteObject( const IDTXOBJ nObjIn )
{
	if (nObjIn <0)
	{
		return 0;
	}

	m_mutex.Lock();
	MAPTXOBJ::iterator itf = m_mapObj.find(nObjIn);
	if (itf != m_mapObj.end())
	{
		m_mapObj.erase(itf);
	}
	m_mutex.Unlock();
	return 0;
}

CTxObjectBase* CTxBaseObjectMgr::HitTestObject( CPoint& ptIn, CTxObjectBase* pLastHitedObj/*= NULL*/)
{
/*
	if (m_pLastHitedObj && m_pLastHitedObj->IsVisable())
	{
		if (m_pLastHitedObj->PtInRect(ptIn))
		{
			if (pLastHitedObj)
			{
				pLastHitedObj = m_pLastHitedObj;
			}
			
			return m_pLastHitedObj;
		}
	}*/

	BOOL bHited = FALSE;
	m_mutex.Lock();
	CTxObjectBase* pRet = NULL;
	MAPTXOBJ::reverse_iterator itb = m_mapObj.rbegin();
	MAPTXOBJ::reverse_iterator ite = m_mapObj.rend();
	for (; itb != ite; itb++)
	{
		if (itb->second->IsVisable() && itb->second->PtInRect(ptIn))
		{
			pRet =  itb->second;
			bHited = TRUE;
			break;
		}
	}
	m_mutex.Unlock();
	if (pLastHitedObj)
	{
		pLastHitedObj = m_pLastHitedObj;
	}
#ifdef _DEBUG
	if (m_pLastHitedObj != pRet)
	{
		//TRACE("\nhit obj = %x", pRet);
	}
#endif // _DEBUG
	m_pLastHitedObj = bHited? pRet:NULL;

	return pRet;
}

int CTxBaseObjectMgr::DoAction( UINT nActionType, WPARAM wparam, LPARAM lparam )
{
	m_mutex.Lock();
	int nRet = 0;
	MAPTXOBJ::iterator itb = m_mapObj.begin();
	MAPTXOBJ::iterator ite = m_mapObj.end();
	for (; itb != ite; itb++)
	{
		switch(nActionType)
		{
		case WM_ERASEBKGND:
		case WM_PAINT:
			{
				CDC* pDC = (CDC*)wparam;
				pDC->OffsetViewportOrg(itb->second->m_ptOffSetWnd.x, itb->second->m_ptOffSetWnd.y);
				itb->second->Draw(pDC);
				pDC->SetViewportOrg(0,0);
			}break;
		}
	}
	m_mutex.Unlock();
	return nRet;
}

CString GenGuid()
{
	GUID guidNew;
	CoCreateGuid(&guidNew);
	RPC_WSTR pStr;
	UuidToString(&guidNew, &pStr);
	CString strOut((LPCWSTR)pStr);
	RpcStringFree(&pStr);
	return strOut;
}
