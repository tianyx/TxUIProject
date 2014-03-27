// BaseGraph_PNCL.h: interface for the CBaseGraph_PNCL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEGRAPH_PNCL_H__CB4E822C_0682_46C7_93FE_593CA42B3B76__INCLUDED_)
#define AFX_BASEGRAPH_PNCL_H__CB4E822C_0682_46C7_93FE_593CA42B3B76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseGraph.h"

class _JS_Fix CBaseGraph_MSS : public CBaseGraph  
{
public:
	CBaseGraph_MSS();
	virtual ~CBaseGraph_MSS();

	HRESULT ConnectFiltersInGraph();
	HRESULT AddFilters2Graph();
protected:
	virtual HRESULT BuildGrabberGraph();

};

#endif // !defined(AFX_BASEGRAPH_PNCL_H__CB4E822C_0682_46C7_93FE_593CA42B3B76__INCLUDED_)
