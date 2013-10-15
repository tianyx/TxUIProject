#pragma once
#include "MBCHeartBeatObj.h"

class CMBCWatchStateQueryObj;

interface IWatchStateIncomingCallback
{
	virtual HRESULT OnRecvGraphState(CMBCWatchStateQueryObj* pObj, ST_GRAPHSTATE& gstateIn) = 0;
};
class CMBCWatchStateQueryObj :
	public CMBCHeartBeatObj
{
public:
	CMBCWatchStateQueryObj(void);
	virtual ~CMBCWatchStateQueryObj(void);

	virtual HRESULT TxTimerCallbackProc(DWORD dwEvent, LPARAM lparam);

	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed );

	virtual HRESULT SetGraphStateIncomingCallback(IWatchStateIncomingCallback* pCallback);

	IWatchStateIncomingCallback* m_pGsInCallback;
};
