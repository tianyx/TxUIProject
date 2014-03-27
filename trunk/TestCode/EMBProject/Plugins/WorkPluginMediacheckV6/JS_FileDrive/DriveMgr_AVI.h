// DriveMgr_AVI.h: interface for the CDriveMgr_AVI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEMGR_AVI_H__DAD519C6_8482_45A5_8C6B_92D6240CF17A__INCLUDED_)
#define AFX_DRIVEMGR_AVI_H__DAD519C6_8482_45A5_8C6B_92D6240CF17A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DriveMgr.h"
#include "AVIInfoExtract.h"
#include "BaseGraph_AVI.h"

class _JS_Fix CDriveMgr_AVI : public CDriveMgr  
{
public:
	CDriveMgr_AVI(HWND ParentHwnd);
	CDriveMgr_AVI();
	virtual ~CDriveMgr_AVI();
protected:
	CAVIInfoExtract      m_InfoExtract;
	CBaseGraph_AVI       m_DecoderGraph;
};

#endif // !defined(AFX_DRIVEMGR_AVI_H__DAD519C6_8482_45A5_8C6B_92D6240CF17A__INCLUDED_)
