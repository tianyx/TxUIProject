// BaseGraph_Hik.h: interface for the CBaseGraph_Hik class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEGRAPH_HIK_H__52B1167B_3067_4CB2_9C93_24792FD49FE2__INCLUDED_)
#define AFX_BASEGRAPH_HIK_H__52B1167B_3067_4CB2_9C93_24792FD49FE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseGraph.h"

class _JS_Fix CBaseGraph_Hik : public CBaseGraph  
{
public:
	CBaseGraph_Hik();
	virtual ~CBaseGraph_Hik();
	HRESULT AddFilters2Graph();
	HRESULT ConnectFiltersInGraph();
protected:
	virtual HRESULT BuildGrabberGraph();

};

#endif // !defined(AFX_BASEGRAPH_HIK_H__52B1167B_3067_4CB2_9C93_24792FD49FE2__INCLUDED_)
