#pragma once
#include "IEMBBaseInterface.h"
#include "ActorConnector.h"
#include "ExcutorMgr.h"
namespace EMB
{
class CTaskActor:
	public IPluginBaseInterface,
	public IPluginControlInterface,
// 	public IPluginConnectorInterce,
// 	public IActorMsgCallBackInterface,
	public IActorConnectorCallback

{
public:
	CTaskActor(void);
	virtual ~CTaskActor(void);

	virtual HRESULT OnFirstInit();
	virtual void	OnFinalRelease();

	//interface for IPluginBaseInterface
	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoInOut);
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	//for IPluginControlInterface
	virtual HRESULT Run_Plugin();
	virtual HRESULT Stop_Plugin();
	//for IPluginConnectorInterce
// 	//you riseup a action to connect/disconnect to other plugin
// 	virtual HRESULT Connect(ITxUnkown* pInterfaceIn);
// 	virtual HRESULT Disconnect(ITxUnkown* pInterfaceIn);
// 
// 	//other plugin inform you after riseup a action that Connect()/Disconnect() you
// 	virtual HRESULT OnConnect(ITxUnkown* pInterfaceIn);
// 	virtual HRESULT OnDisconnect(ITxUnkown* pInterfaceIn);

	//interface IActorMsgCallBackInterface
// 	virtual HRESULT ActCallbackProc(CTaskString& szActMsg, CTaskString& szRet);

	//for IActorConnectorCallback
	virtual HRESULT OnActorConnectorMsg(ST_EMBTRANSMSG& pMsg);

private:
	BOOL SwitchActorConn(BOOL bMainConn);
private:
	CActorConnector m_actorconnMain;
	CActorConnector m_actorconnSlave;
	CActorConnector* m_pCurrActor;
	CExcutorMgr* m_pExcMgr;
};


}
extern EMB::CTaskActor* g_pPluginInstane;
