// DriveMgr_MXF.h: interface for the CDriveMgr_MXF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEMGR_MXF_H__1A40BBFD_4A35_4855_8E92_62E9D700C1DD__INCLUDED_)
#define AFX_DRIVEMGR_MXF_H__1A40BBFD_4A35_4855_8E92_62E9D700C1DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JS_FileDrive.h"
#include "MxfInfoExtract.h"
#include "BaseGraph_MXF.h"
#include "DriveMgr.h"


int _JS_Fix LoadDict(const char* DictPath);

class _JS_Fix CDriveMgr_MXF:public CDriveMgr  
{
public:
	CDriveMgr_MXF();
	CDriveMgr_MXF(HWND ParentHwnd);
	virtual ~CDriveMgr_MXF();
protected: 
	CMxfInfoExtract      m_InfoExtract;
	CBaseGraph_MXF       m_DecoderGraph;

};

#endif // !defined(AFX_DRIVEMGR_MXF_H__1A40BBFD_4A35_4855_8E92_62E9D700C1DD__INCLUDED_)
