// BaseGraph_LXF.h: interface for the CBaseGraph_LXF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEGRAPH_LXF_H__923CFA93_76AA_4916_A83B_4DCD17D9B59A__INCLUDED_)
#define AFX_BASEGRAPH_LXF_H__923CFA93_76AA_4916_A83B_4DCD17D9B59A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseGraph.h"

class _JS_Fix CBaseGraph_LXF : public CBaseGraph  
{
public:
	HRESULT AddFilters2Graph();
	HRESULT ConnectFiltersInGraph();
	CBaseGraph_LXF();
	virtual ~CBaseGraph_LXF();
protected:
	virtual HRESULT BuildGrabberGraph();

};

#endif // !defined(AFX_BASEGRAPH_LXF_H__923CFA93_76AA_4916_A83B_4DCD17D9B59A__INCLUDED_)
