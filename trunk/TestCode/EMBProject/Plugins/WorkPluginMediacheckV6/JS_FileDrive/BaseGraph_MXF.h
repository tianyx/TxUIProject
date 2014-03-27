// BaseGraph_MXF.h: interface for the CBaseGraph_MXF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MXFGRAPH_H__24D60AC0_2738_43BE_B44C_54F7BC6A163D__INCLUDED_)
#define AFX_MXFGRAPH_H__24D60AC0_2738_43BE_B44C_54F7BC6A163D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JS_FileDrive.h"
#include "BaseGraph.h"

class _JS_Fix CBaseGraph_MXF : public CBaseGraph  
{
public:
	CBaseGraph_MXF();
	virtual ~CBaseGraph_MXF();

protected:
	HRESULT BuildGrabberGraph();


};

#endif // !defined(AFX_MXFGRAPH_H__24D60AC0_2738_43BE_B44C_54F7BC6A163D__INCLUDED_)
