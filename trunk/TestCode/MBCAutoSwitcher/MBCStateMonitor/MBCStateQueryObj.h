#pragma once
#include "MBCHeartBeatObj.h"

interface IGraphStateIncomingCallback
{
	virtual HRESULT SetGraphState(ST_GRAPHSTATE& gstateIn) = 0;
};

class CMBCStateQueryObj :
	public CMBCHeartBeatObj
{
public:
	CMBCStateQueryObj(void);
	virtual ~CMBCStateQueryObj(void);

	virtual HRESULT TxTimerCallbackProc(DWORD dwEvent, LPARAM lparam);

	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed );

	virtual HRESULT SetGraphStateIncomingCallback(IGraphStateIncomingCallback* pCallback);

	IGraphStateIncomingCallback* m_pGsInCallback;
};
