#pragma once
#include "MBCRemoteObj.h"
interface IGraphStateCallbackInterface
{
	virtual HRESULT GetGraphState(ST_GRAPHSTATE& outData) = 0;
	virtual HRESULT ChangeRelay(ST_CHANGERELAY& relayIn) = 0;
};
class CMBCGraphStateSvrObj :
	public CMBCRemoteObj
{
public:
	CMBCGraphStateSvrObj(void);
	virtual ~CMBCGraphStateSvrObj(void);
	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed);

	void SetGraphStateCallback(IGraphStateCallbackInterface* pICall);

	IGraphStateCallbackInterface* m_pIGSCallback;
};
