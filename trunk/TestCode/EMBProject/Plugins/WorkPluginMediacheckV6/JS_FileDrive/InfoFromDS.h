// InfoFromDS.h: interface for the CInfoFromDS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOFROMDS_H__7C113AFE_19DF_49D6_B8D2_3C1A8A0FCB1D__INCLUDED_)
#define AFX_INFOFROMDS_H__7C113AFE_19DF_49D6_B8D2_3C1A8A0FCB1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BaseGraph.h"

class _JS_Fix CInfoFromDS_3GP : public CBaseGraph  
{
public:
	CInfoFromDS_3GP();
	virtual ~CInfoFromDS_3GP();
	virtual LONGLONG GetDuration();         //时长单位可以在继承函数中定义
	virtual HRESULT  GetSrcStreamMedaiType(int nStream,AM_MEDIA_TYPE* AM);
	virtual int      GetSrcStreamCount();
protected:
	HRESULT ConnectFiltersInGraph();
	HRESULT AddFilters2Graph();
	virtual HRESULT BuildGrabberGraph();
};

#endif // !defined(AFX_INFOFROMDS_H__7C113AFE_19DF_49D6_B8D2_3C1A8A0FCB1D__INCLUDED_)
