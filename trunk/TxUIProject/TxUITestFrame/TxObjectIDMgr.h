/********************************************************************
	created:	2012/08/03
	created:	3:8:2012   10:46
	filename: 	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame\TxObjectIDMgr.h
	file path:	f:\MySrc\SvnWorkSpace\TxUIProject\TxUITestFrame
	file base:	TxObjectIDMgr
	file ext:	h
	author:		tian_yx
	
	purpose:	
*********************************************************************/
#pragma once
#include "TxPreDef.h"
#include <vector>
using namespace std;

typedef vector<int> VEC_IDGEN;

 
class CTxObjectIDMgr
{
public:
	CTxObjectIDMgr(void);
	~CTxObjectIDMgr(void);
public:
	IDTXOBJ GenTxObjID(const int nLevel);
private:
	VEC_IDGEN m_vIds;
};
