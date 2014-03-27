// BaseGraph_MP4.h: interface for the CBaseGraph_MP4 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEGRAPH_MP4_H__9BA3C15B_F366_446F_8E0F_CD92FD39B2F7__INCLUDED_)
#define AFX_BASEGRAPH_MP4_H__9BA3C15B_F366_446F_8E0F_CD92FD39B2F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseGraph.h"

class _JS_Fix CBaseGraph_MP4 : public CBaseGraph  
{
public:
	CBaseGraph_MP4();
	virtual ~CBaseGraph_MP4();
	virtual LONGLONG GetDuration();         //时长单位可以在继承函数中定义

	HRESULT AddFilters2Graph();
	HRESULT ConnectFiltersInGraph();
protected:
	virtual HRESULT BuildGrabberGraph();
};

#endif // !defined(AFX_BASEGRAPH_MP4_H__9BA3C15B_F366_446F_8E0F_CD92FD39B2F7__INCLUDED_)
