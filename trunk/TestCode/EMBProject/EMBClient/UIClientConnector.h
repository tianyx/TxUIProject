#pragma once
#include "MBCBaseObj.h"
#include "MBCHeartBeatObj.h"
interface IUIClientMessageProcessInterface
{
	virtual HRESULT OnMessage(int nMaster, CString& strMsgIn, CString& strRet) = 0;
	virtual HRESULT OnConnStateChange(int nMaster, int nStateIn) = 0;
};
class CUIClientConnector:
	public CMBCHeartBeatObj
{
public:
	CUIClientConnector(void);
	virtual ~CUIClientConnector(void);
	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed);
	virtual HRESULT SockStateChange(CMBCSocket* pMBCSock, ENUMMBCSOCKSTATE socStateIn);
	
	virtual HRESULT SendtoUISvr(CString& strInfo);
	virtual HRESULT Run();
	virtual HRESULT Stop();
	void SetMsgProcessor(IUIClientMessageProcessInterface* pIproce);
	BOOL SetParam(int nMaster, SOCKADDR_IN& addrConn);
private:
	IUIClientMessageProcessInterface* m_pIProcess;
	int m_nMaster;
	int m_connState;
};
