// DriveMgr_M2V.h: interface for the CDriveMgr_M2V class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEMGR_M2V_H__5DDCF3ED_0579_43C8_9885_749CE4465519__INCLUDED_)
#define AFX_DRIVEMGR_M2V_H__5DDCF3ED_0579_43C8_9885_749CE4465519__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DriveMgr.h"
#include "M2vInfoExtract.h"
#include "BaseGraph_M2V.h"

class _JS_Fix CDriveMgr_M2V : public CDriveMgr  
{
public:
	CDriveMgr_M2V(HWND ParentHwnd);
	CDriveMgr_M2V();
	virtual ~CDriveMgr_M2V();

protected: 
	CM2vInfoExtract      m_InfoExtract;
	CBaseGraph_M2V       m_DecoderGraph;

};

#endif // !defined(AFX_DRIVEMGR_M2V_H__5DDCF3ED_0579_43C8_9885_749CE4465519__INCLUDED_)
