// DriveMgr_Hik.h: interface for the CDriveMgr_Hik class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEMGR_HIK_H__061189DA_71D5_4F53_8676_E198E12DC51A__INCLUDED_)
#define AFX_DRIVEMGR_HIK_H__061189DA_71D5_4F53_8676_E198E12DC51A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DriveMgr.h"
#include "FileInfoExtract_Hik.h"
#include "BaseGraph_Hik.h"

class _JS_Fix CDriveMgr_Hik : public CDriveMgr  
{
public:
	CDriveMgr_Hik();
	virtual ~CDriveMgr_Hik();
	CDriveMgr_Hik(HWND ParentHwnd);
protected:
	CFileInfoExtract_Hik      m_InfoExtract;
	CBaseGraph_Hik            m_DecoderGraph;

};

#endif // !defined(AFX_DRIVEMGR_HIK_H__061189DA_71D5_4F53_8676_E198E12DC51A__INCLUDED_)
