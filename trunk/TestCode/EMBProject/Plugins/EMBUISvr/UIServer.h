#pragma once

#include "IEMBBaseInterface.h"
#include "EmbStructDef.h"
#include "UISvrProber.h"

namespace EMB{

class CUIServer:
		public IPluginBaseInterface,
		public IPluginConnectorInterce,
		public IPluginControlInterface,
		public IPluginConfigInterface,
		public IUISvrMsgCallbackInterface,
		public IDispatchNotifyCallbackInterface
{
public:
	CUIServer(void);
	virtual ~CUIServer(void);

	//interface for Iunknow
	virtual void	OnFinalRelease();
	virtual HRESULT OnFirstInit();
	//interface for IPluginBaseInterface
	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoInOut);
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	//interface IPluginConnectorInterce
	virtual HRESULT Connect(ITxUnkown* pInterfaceIn);
	virtual HRESULT Disconnect(ITxUnkown* pInterfaceIn);
	virtual HRESULT OnConnect(ITxUnkown* pInterfaceIn);
	virtual HRESULT OnDisconnect(ITxUnkown* pInterfaceIn);

	//for IPluginControlInterface
	virtual HRESULT Run_Plugin();
	virtual HRESULT Stop_Plugin();

	//for IPluginConfigInterface
	virtual HRESULT GetParam(const CTaskString& szIn, CTaskString& szOut);
	virtual HRESULT SetParam(const CTaskString& szIn, CTaskString& szOut);

	//for IUISvrMsgCallbackInterface
	virtual HRESULT UIMessageCallback(CString& taskIn, CString& strRet);  
	//for IDispatchNotifyCallbackInterface
	virtual HRESULT OnDispatchNotify(CTaskString& strNotify);


private:
	IUIMessageProcessInterface* m_pIUIMsgProcess;
	IPluginTaskCommit* m_pITaskCommit;
	CUISvrProber m_Prober;

	ST_UISVRCONFIG m_cfgUISvr;
};

}

extern EMB::CUIServer* g_pPluginInstane;