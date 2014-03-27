// BaseGraph_TS.h: interface for the CBaseGraph_TS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEGRAPH_TS_H__2DA67B58_CC75_43D2_AAC8_CBD938E9BF62__INCLUDED_)
#define AFX_BASEGRAPH_TS_H__2DA67B58_CC75_43D2_AAC8_CBD938E9BF62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseGraph.h"

class CBaseGraph_TS : public CBaseGraph  
{
public:
//	HRESULT  ClearUpForStaticGhaph();
	void ConfigDemultiplexer();
	CBaseGraph_TS();
	virtual ~CBaseGraph_TS();
	virtual LONGLONG GetDuration();         //时长单位可以在继承函数中定义
	HRESULT ConnectFiltersInGraph();
	HRESULT AddFilters2Graph();
protected:
	virtual HRESULT BuildGrabberGraph();
};

#endif // !defined(AFX_BASEGRAPH_TS_H__2DA67B58_CC75_43D2_AAC8_CBD938E9BF62__INCLUDED_)
