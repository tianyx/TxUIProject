#pragma once

#include "IEMBBaseInterface.h"
#include "MBCRemoteObj.h"

interface IUISvrMsgCallbackInterface
{
	virtual HRESULT UIMessageCallback(CString& strMsgIn, CString& strRet) = 0;    
};

interface IUISvrProberBase
{
	virtual BOOL SetMsgProcessor(IUISvrMsgCallbackInterface* pProcessor) = 0;
	virtual HRESULT SendToUI(CString& taskIn) = 0;
	virtual HRESULT Run_Prober() = 0;
	virtual HRESULT Stop_Prober() = 0;
};

class CUISvrProber:
	public IUISvrProberBase,
	public CMBCRemoteObj
{
public:
	CUISvrProber(void);
	virtual ~CUISvrProber(void);

	// 
	BOOL SetMsgProcessor(IUISvrMsgCallbackInterface* pProcessor);
	//
	virtual HRESULT SendToUI(CString& taskIn);
	//
	virtual HRESULT Run_Prober();
	// 
	virtual HRESULT Stop_Prober();
	
	//from CMBCRemoteObj
	virtual HRESULT ProcessIncomingMsg(CMBCSocket* pMBCSock, int nMsgType, char* bufferIn, int nUsed);
	virtual HRESULT OnSockConnected(CMBCSocket* pMBCSock);

private:
	//
	IUISvrMsgCallbackInterface* m_pIProcessor; 
};
