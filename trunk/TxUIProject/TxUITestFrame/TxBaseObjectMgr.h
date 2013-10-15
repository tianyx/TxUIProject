/********************************************************************
	created:	2012/08/03
	created:	3:8:2012   10:46
	filename: 	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame\TxBaseObjectMgr.h
	file path:	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame
	file base:	TxBaseObjectMgr
	file ext:	h
	author:		tian_yx
	
	purpose:	
*********************************************************************/
#pragma once
#include <map>
#include "TxPreDef.h"
#include "TxObjectIDMgr.h"
using namespace std;

class CTxBaseObjectMgr
{
public:
	CTxBaseObjectMgr(void);
	~CTxBaseObjectMgr(void);

public:
	IDTXOBJ AddObject(CTxObjectBase* pObj);
	int DeleteObject(const IDTXOBJ nObjIn);
	CTxObjectBase* HitTestObject(CPoint& ptIn, CTxObjectBase* pLastHitedObj= NULL);
	inline CTxObjectBase* GetLastHited(){return m_pLastHitedObj;}
	inline CTxObjectBase* GetFirstObj(){return m_pFirstObj;}
	int DoAction(UINT nActionType, WPARAM wparam, LPARAM lparam );
	
private:
	CMutex m_mutex;
	MAPTXOBJ m_mapObj;
	CTxObjectBase* m_pLastHitedObj;
	CTxObjectIDMgr m_IdGenMgr;
	CTxObjectBase* m_pFirstObj;
};

CString GenGuid();