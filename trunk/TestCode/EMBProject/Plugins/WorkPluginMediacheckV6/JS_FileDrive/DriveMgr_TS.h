// DriveMgr_TS.h: interface for the CDriveMgr_TS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEMGR_TS_H__320CC2F3_52FE_49AE_B9C1_06B1DEE5A04C__INCLUDED_)
#define AFX_DRIVEMGR_TS_H__320CC2F3_52FE_49AE_B9C1_06B1DEE5A04C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JS_FileDrive.h"
#include "fileInfoExtract_TS.h"
#include "BaseGraph_TS.h"
#include "DriveMgr.h"

class _JS_Fix CDriveMgr_TS : public CDriveMgr  
{
public:
	CDriveMgr_TS();
	CDriveMgr_TS(HWND ParentHwnd);
	virtual ~CDriveMgr_TS();
protected: 
	CFileInfoExtract_TS m_InfoExtract;
	CBaseGraph_TS       m_DecoderGraph;

};

#endif // !defined(AFX_DRIVEMGR_TS_H__320CC2F3_52FE_49AE_B9C1_06B1DEE5A04C__INCLUDED_)
