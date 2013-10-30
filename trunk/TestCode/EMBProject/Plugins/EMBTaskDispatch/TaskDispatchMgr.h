#pragma once
#include "IEMBBaseInterface.h"
#include "ActorHolder.h"

namespace EMB{

class CTaskDispatchMgr:
	public IPluginBaseInterface,
	public IPluginControlInterface, 
	public IPluginTaskDispatch, 
	public IPluginConnectorInterce,
	public IEMBActorHolderCallBackInterface
{
public:
	CTaskDispatchMgr(void);
	virtual ~CTaskDispatchMgr(void);

	//interface for Iunknow
	virtual void	OnFinalRelease();
	virtual HRESULT OnFirstInit();
	//interface for IPluginBaseInterface
	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoInOut);
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	//for IPluginConnectorInterce
	//you riseup a action to connect/disconnect to other plugin
	virtual HRESULT Connect(ITxUnkown* pInterfaceIn);
	virtual HRESULT Disconnect(ITxUnkown* pInterfaceIn);

	//other plugin inform you after riseup a action that Connect()/Disconnect() you
	virtual HRESULT OnConnect(ITxUnkown* pInterfaceIn);
	virtual HRESULT OnDisconnect(ITxUnkown* pInterfaceIn);

	//interface for IPluginControlInterface
	virtual HRESULT Run_Plugin();
	virtual HRESULT Stop_Plugin();
	//for IPluginTaskDispatch
	virtual HRESULT SubmitTask(const CTaskString& szTaskIn, CTaskString& szRet);

	//for IEMBActorHolderCallBackInterface
	virtual HRESULT OnActorConnect(const GUID& szActorGuid);
	virtual HRESULT OnActorDisConnect(const GUID& szActorGuid);
	virtual HRESULT OnActorReportInfo(const GUID& szActorGuid, CString& szActorInfoIn);
	virtual HRESULT OnActorDispatchTask(const GUID& szActorGuid, CString& szActorInfoIn);


	CActorHolder m_actHolder;
};

}