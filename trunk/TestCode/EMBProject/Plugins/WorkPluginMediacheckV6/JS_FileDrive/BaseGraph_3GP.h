// BaseGraph_3GP.h: interface for the CBaseGraph_3GP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASEGRAPH_3GP_H__97CD7F37_0DE3_4A83_A34C_156AC05602A8__INCLUDED_)
#define AFX_BASEGRAPH_3GP_H__97CD7F37_0DE3_4A83_A34C_156AC05602A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseGraph.h"

class _JS_Fix CBaseGraph_3GP : public CBaseGraph  
{
public:
	CBaseGraph_3GP();
	virtual ~CBaseGraph_3GP();
	virtual LONGLONG GetDuration();         //时长单位可以在继承函数中定义
	HRESULT AddFilters2Graph();
	HRESULT ConnectFiltersInGraph();
protected:
	virtual HRESULT BuildGrabberGraph();
};

#endif // !defined(AFX_BASEGRAPH_3GP_H__97CD7F37_0DE3_4A83_A34C_156AC05602A8__INCLUDED_)
