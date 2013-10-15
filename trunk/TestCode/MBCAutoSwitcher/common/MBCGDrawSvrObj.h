#pragma once
#include "mbcgraphdrawbaseobj.h"

class CMBCGDrawSvrObj :
	public CMBCGDrawBaseObj
{
public:
	CMBCGDrawSvrObj(void);
	virtual ~CMBCGDrawSvrObj(void);

	virtual BOOL Init(ST_OBJSTATEINFO& objInfo, const int nGraphState);
	virtual BOOL CanConnectIn(CMBCGDrawBaseObj* pObjIn);
	virtual BOOL ResetDrawData();

};
