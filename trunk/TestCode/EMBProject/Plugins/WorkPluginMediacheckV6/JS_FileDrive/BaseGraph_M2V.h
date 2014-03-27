// BaseGraph_M2V.h: interface for the CBaseGraph_M2V class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEGRAPH_M2V_H__EB0E9305_FC8E_4BE8_A7A2_ADE90446D376__INCLUDED_)
#define AFX_BASEGRAPH_M2V_H__EB0E9305_FC8E_4BE8_A7A2_ADE90446D376__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseGraph.h"

class _JS_Fix CBaseGraph_M2V : public CBaseGraph  
{
public:
	HRESULT ConnectFiltersInGraph();
	HRESULT AddFilters2Graph();
	CBaseGraph_M2V();
	virtual ~CBaseGraph_M2V();
protected:
	virtual HRESULT BuildGrabberGraph();

};

#endif // !defined(AFX_BASEGRAPH_M2V_H__EB0E9305_FC8E_4BE8_A7A2_ADE90446D376__INCLUDED_)
