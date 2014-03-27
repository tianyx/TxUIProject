// DriveMgr_3GP.h: interface for the CDriveMgr_3GP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEMGR_3GP_H__53F862E7_3712_493E_A4BA_626E0D48797F__INCLUDED_)
#define AFX_DRIVEMGR_3GP_H__53F862E7_3712_493E_A4BA_626E0D48797F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DriveMgr.h"
#include "fileInfoExtract_3GP.h"
#include "BaseGraph_3GP.h"

class _JS_Fix CDriveMgr_3GP : public CDriveMgr  
{
public:
	CDriveMgr_3GP();
	CDriveMgr_3GP(HWND ParentHwnd);
	virtual ~CDriveMgr_3GP();
protected: 
	CFileInfoExtract_3GP m_InfoExtract;
	CBaseGraph_3GP       m_DecoderGraph;
};

#endif // !defined(AFX_DRIVEMGR_3GP_H__53F862E7_3712_493E_A4BA_626E0D48797F__INCLUDED_)
