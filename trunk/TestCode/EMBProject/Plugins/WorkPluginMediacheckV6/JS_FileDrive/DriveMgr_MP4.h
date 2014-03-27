// DriveMgr_MP4.h: interface for the CDriveMgr_MP4 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRIVEMGR_MP4_H__6592446B_76CF_48E8_ADDB_38872312D5AD__INCLUDED_)
#define AFX_DRIVEMGR_MP4_H__6592446B_76CF_48E8_ADDB_38872312D5AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DriveMgr.h"
#include "fileInfoExtract_MP4.h"
#include "BaseGraph_MP4.h"

class _JS_Fix CDriveMgr_MP4 : public CDriveMgr  
{
public:
	CDriveMgr_MP4();
	CDriveMgr_MP4(HWND ParentHwnd);
	virtual ~CDriveMgr_MP4();
protected: 
	CFileInfoExtract_MP4 m_InfoExtract;
	CBaseGraph_MP4       m_DecoderGraph;
};

#endif // !defined(AFX_DRIVEMGR_MP4_H__6592446B_76CF_48E8_ADDB_38872312D5AD__INCLUDED_)
