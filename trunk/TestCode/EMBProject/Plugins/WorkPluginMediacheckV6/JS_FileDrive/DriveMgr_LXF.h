// DriveMgr_LXF.h: interface for the CDriveMgr_LXF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEMGR_LXF_H__9DE0818A_CBC4_4AC2_B715_47317A5086E4__INCLUDED_)
#define AFX_DRIVEMGR_LXF_H__9DE0818A_CBC4_4AC2_B715_47317A5086E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DriveMgr.h"
#include "LxfInfoExtract.h"
#include "BaseGraph_LXF.h"

class _JS_Fix CDriveMgr_LXF : public CDriveMgr  
{
public:
	CDriveMgr_LXF(HWND ParentHwnd);
	CDriveMgr_LXF();
	virtual ~CDriveMgr_LXF();
protected: 
	CLxfInfoExtract      m_InfoExtract;
	CBaseGraph_LXF       m_DecoderGraph;

};

#endif // !defined(AFX_DRIVEMGR_LXF_H__9DE0818A_CBC4_4AC2_B715_47317A5086E4__INCLUDED_)
