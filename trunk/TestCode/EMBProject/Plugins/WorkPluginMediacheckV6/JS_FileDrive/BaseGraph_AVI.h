// BaseGraph_AVI.h: interface for the CBaseGraph_AVI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEGRAPH_AVI_H__1FF595D2_1F27_4438_B53B_DDA0A279FD99__INCLUDED_)
#define AFX_BASEGRAPH_AVI_H__1FF595D2_1F27_4438_B53B_DDA0A279FD99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseGraph.h"

class _JS_Fix CBaseGraph_AVI : public CBaseGraph  
{
public:
	CBaseGraph_AVI();
	virtual ~CBaseGraph_AVI();

	HRESULT AddFilters2Graph();
	HRESULT ConnectFiltersInGraph();
protected:
	virtual HRESULT BuildGrabberGraph();
};

#endif // !defined(AFX_BASEGRAPH_AVI_H__1FF595D2_1F27_4438_B53B_DDA0A279FD99__INCLUDED_)
