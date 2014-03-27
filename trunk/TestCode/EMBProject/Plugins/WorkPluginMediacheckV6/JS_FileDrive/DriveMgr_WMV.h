// DriveMgr_WMV.h: interface for the CDriveMgr_WMV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEMGR_WMV_H__F42F41A3_36C7_4BF9_8DE3_A2AEC406D37A__INCLUDED_)
#define AFX_DRIVEMGR_WMV_H__F42F41A3_36C7_4BF9_8DE3_A2AEC406D37A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DriveMgr.h"
#include "FileInfoExtract_WMV.h"
#include "BaseGraph_WMV.h"

class _JS_Fix CDriveMgr_WMV : public CDriveMgr  
{
public:
	CDriveMgr_WMV(HWND ParentHwnd);
	CDriveMgr_WMV();
	virtual ~CDriveMgr_WMV();
protected: 
	CFileInfoExtract_WMV      m_InfoExtract;
	CBaseGraph_WMV            m_DecoderGraph;

};

#endif // !defined(AFX_DRIVEMGR_WMV_H__F42F41A3_36C7_4BF9_8DE3_A2AEC406D37A__INCLUDED_)
