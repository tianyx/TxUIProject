#pragma once
#include "mbcgraphdrawbaseobj.h"

class CMBCGDrawEndObj :
	public CMBCGDrawBaseObj
{
public:
	CMBCGDrawEndObj(void);
	virtual ~CMBCGDrawEndObj(void);

	virtual BOOL Init(ST_OBJSTATEINFO& objInfo, const int nGraphState);
	virtual BOOL CanConnectIn(CMBCGDrawBaseObj* pObjIn);
	virtual BOOL ResetDrawData();

};
