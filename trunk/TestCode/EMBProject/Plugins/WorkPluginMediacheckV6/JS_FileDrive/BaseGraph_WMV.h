// BaseGraph_WMV.h: interface for the CBaseGraph_WMV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEGRAPH_WMV_H__A2B825ED_5B00_4718_B374_793E053E8BA1__INCLUDED_)
#define AFX_BASEGRAPH_WMV_H__A2B825ED_5B00_4718_B374_793E053E8BA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseGraph.h"

class CBaseGraph_WMV : public CBaseGraph  
{
public:
	CBaseGraph_WMV();
	virtual ~CBaseGraph_WMV();
	virtual HRESULT  GetSrcStreamMedaiType(int nStream,AM_MEDIA_TYPE* AM);
	virtual int      GetSrcStreamCount();
protected:
	HRESULT AddFilters2Graph();
	HRESULT ConnectFiltersInGraph();
	virtual HRESULT BuildGrabberGraph();

};

#endif // !defined(AFX_BASEGRAPH_WMV_H__A2B825ED_5B00_4718_B374_793E053E8BA1__INCLUDED_)
