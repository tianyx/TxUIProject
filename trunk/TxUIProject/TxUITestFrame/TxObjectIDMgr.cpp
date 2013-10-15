/********************************************************************
	created:	2012/08/03
	created:	3:8:2012   10:46
	filename: 	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame\TxObjectIDMgr.cpp
	file path:	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame
	file base:	TxObjectIDMgr
	file ext:	cpp
	author:		tian_yx
	
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "TxObjectIDMgr.h"
#include "TxPreDef.h"
#define BASE_ID_INTERVAL 1000000

CTxObjectIDMgr::CTxObjectIDMgr(void)
{
	m_vIds.push_back(1000);
}

CTxObjectIDMgr::~CTxObjectIDMgr(void)
{
}

IDTXOBJ CTxObjectIDMgr::GenTxObjID(const  int nLevel )
{
	ASSERT(nLevel >= 0);
	int nSize = m_vIds.size();
	if (nSize -1 < nLevel)
	{
		for (int i = nSize; i <= nLevel; i++)
		{
			m_vIds.push_back(i*BASE_ID_INTERVAL);
		}
	}

	int nNewId = m_vIds[nLevel] + 1;
	m_vIds[nLevel] = nNewId;
	return nNewId;
}
