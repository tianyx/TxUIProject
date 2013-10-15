#pragma once
#include "mbcgraphdrawbaseobj.h"

class CMBCGDrawClientObj :
	public CMBCGDrawBaseObj
{
public:
	CMBCGDrawClientObj(void);
	virtual ~CMBCGDrawClientObj(void);

	virtual BOOL Init(ST_OBJSTATEINFO& objInfo, const int nGraphState);
	virtual BOOL CanConnectIn(CMBCGDrawBaseObj* pObjIn);

	virtual BOOL ResetDrawData();

	virtual void Draw(Graphics& gc);

};
