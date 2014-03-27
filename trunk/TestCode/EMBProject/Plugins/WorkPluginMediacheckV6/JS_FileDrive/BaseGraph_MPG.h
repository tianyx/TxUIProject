// BaseGraph_MPG.h: interface for the CBaseGraph_MPG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEGRAPH_MPG_H__966A64A0_A2F8_458C_AFEC_512EE4D0433C__INCLUDED_)
#define AFX_BASEGRAPH_MPG_H__966A64A0_A2F8_458C_AFEC_512EE4D0433C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseGraph.h"

class _JS_Fix CBaseGraph_MPG : public CBaseGraph  
{
public:
	CBaseGraph_MPG();
	virtual ~CBaseGraph_MPG();
	HRESULT ConnectFiltersInGraph();
	HRESULT AddFilters2Graph();

protected:
	virtual HRESULT BuildGrabberGraph();
};

#endif // !defined(AFX_BASEGRAPH_MPG_H__966A64A0_A2F8_458C_AFEC_512EE4D0433C__INCLUDED_)
