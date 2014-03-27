// DriveMgr_MOV.h: interface for the CDriveMgr_MOV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEMGR_MOV_H__1A40BBFD_4A35_4855_8E92_62E9D700C1DD__INCLUDED_)
#define AFX_DRIVEMGR_MOV_H__1A40BBFD_4A35_4855_8E92_62E9D700C1DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JS_FileDrive.h"
#include "movInfoExtract.h"
#include "BaseGraph_MOV.h"
#include "DriveMgr.h"

class _JS_Fix CDriveMgr_MOV:public CDriveMgr  
{
public:
	CDriveMgr_MOV();
	CDriveMgr_MOV(HWND ParentHwnd);
	virtual ~CDriveMgr_MOV();
protected: 
	CMOVInfoExtract      m_InfoExtract;
	CBaseGraph_MOV       m_DecoderGraph;

};

#endif // !defined(AFX_DRIVEMGR_MOV_H__1A40BBFD_4A35_4855_8E92_62E9D700C1DD__INCLUDED_)
