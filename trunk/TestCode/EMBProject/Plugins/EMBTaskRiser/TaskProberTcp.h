/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	TaskProberTcp.h
	author:		tianyx
	
	purpose:	
*********************************************************************/
#pragma once
#include "taskprober.h"
#include "MBCRemoteObj.h"
#include "EMBDefine.h"

class CTaskProberTcp :
	public CTaskProber, public CMBCRemoteObj
{
public:
	CTaskProberTcp(void);
	virtual ~CTaskProberTcp(void);
	virtual HRESULT TransTask(ST_EMBTRANSMSG& msgIn, ST_EMBTRANSMSG& msgRet);
	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed);

	virtual HRESULT Run_Prober();
	virtual HRESULT Stop_Prober();
};
