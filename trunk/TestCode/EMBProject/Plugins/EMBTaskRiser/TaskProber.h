/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:03
	filename: 	TaskProber.h
	author:		tianyx
	
	purpose:	
*********************************************************************/
#pragma once
#include "EMBCommonFunc.h"
#include "AutoCritSec.h"
#include "TxParamString.h"
#include <vector>

#define TaskWeight_Normal 10
#define TaskWeight_High 20
using namespace std;
struct ST_EMBTASKDATA
{
	time_t tm;
	CString taskDesc;
	ST_EMBTASKDATA()
	{
		tm = time(NULL);
	}
};

typedef vector<ST_EMBTASKDATA> VECTASKDATAS;

interface ITaskCollectCallbackInterface
{
	virtual HRESULT TaskCollectCallback(CString& taskIn) = 0;
};


class CTaskProber
{
public:
	 CTaskProber(void);
	virtual ~CTaskProber(void);
	BOOL SetTaskProcessor(ITaskCollectCallbackInterface* pProcessor);
	virtual HRESULT AddTask(CString& taskIn);
	virtual HRESULT Run_Prober();
	virtual HRESULT Stop_Prober();
private:
	ITaskCollectCallbackInterface* m_pTaskProcessor;

};
