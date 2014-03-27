// DriveMgr_PNCL.h: interface for the CDriveMgr_PNCL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEMGR_PNCL_H__91852B04_EDAB_44A3_829B_175BA70DE96A__INCLUDED_)
#define AFX_DRIVEMGR_PNCL_H__91852B04_EDAB_44A3_829B_175BA70DE96A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DriveMgr.h"
#include "FileInfoExtract_MSS.h"
#include "BaseGraph_MSS.h"
#include "DriveMgr.h"

class _JS_Fix CDriveMgr_MSS : public CDriveMgr  
{
public:
	CDriveMgr_MSS(HWND ParentHwnd);
	CDriveMgr_MSS();
	virtual ~CDriveMgr_MSS();

protected: 
	CFileInfoExtract_MSS      m_InfoExtract;
	CBaseGraph_MSS            m_DecoderGraph;

};

#endif // !defined(AFX_DRIVEMGR_PNCL_H__91852B04_EDAB_44A3_829B_175BA70DE96A__INCLUDED_)
