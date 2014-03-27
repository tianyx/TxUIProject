// DriveMgr_MPG.h: interface for the CDriveMgr_MPG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEMGR_MPG_H__186A818A_08A4_44DA_B94B_D23BC1C06259__INCLUDED_)
#define AFX_DRIVEMGR_MPG_H__186A818A_08A4_44DA_B94B_D23BC1C06259__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FileInfoExtract_MPG.h"
#include "BaseGraph_MPG.h"

#include "DriveMgr.h"

class _JS_Fix CDriveMgr_MPG : public CDriveMgr  
{
public:
	CDriveMgr_MPG(HWND ParentHwnd);
	CDriveMgr_MPG();
	virtual ~CDriveMgr_MPG();

protected: 
	CFileInfoExtract_MPG      m_InfoExtract;
	CBaseGraph_MPG            m_DecoderGraph;
};

#endif // !defined(AFX_DRIVEMGR_MPG_H__186A818A_08A4_44DA_B94B_D23BC1C06259__INCLUDED_)
