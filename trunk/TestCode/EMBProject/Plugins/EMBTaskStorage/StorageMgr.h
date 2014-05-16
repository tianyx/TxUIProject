/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   17:56
	filename: 	StorageMgr.h
	author:		tianyx
	
	purpose:	ÈÎÎñ´æ´¢²å¼þ
*********************************************************************/
#pragma once
#include "IEMBBaseInterface.h"
#include "EMBStorage.h"
#include "FGlobal.h"
#include <map>
using namespace std;
typedef map<TXGUID, CTaskString> MAPTASKSTRING;
namespace EMB{
class CStorageMgr:
	public IPluginBaseInterface, 
	public IPluginControlInterface, 
	public IPluginConnectorInterce,
	public IPluginStorageInterface,
	public IPluginConfigInterface,
	public IPluginTaskCommit,
	public IEMBInfoPublishRegisterInterface
{
public:
	CStorageMgr(void);
	virtual ~CStorageMgr(void);
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

	
	//for IPluginTaskCommit
	virtual HRESULT SubmitTask(const CTaskString& szTaskIn, CTaskString& szRet);
	
	//for IPluginStorageInterface
	virtual HRESULT UpdateTaskToStorage(const int nDispatchID, CTaskString& szTaskIn);
	virtual HRESULT FetchTaskFromStorage(const int nDispatchID,  int nMinPriority, int nDesiredNum, VECTASKS& vTasks);
	virtual HRESULT GetDispatchedTaskFromStorage(const DISPATCHID nDispatchID, VECTASKS& vTasks);
	virtual HRESULT UpdateActorID(CTaskString& strTaskGuid, ACTORID actorId);


	virtual HRESULT RegisterPublisher(IEMBPublishCallbackInterface* pPublisher, const int nPubType);
	virtual HRESULT UnRegisterPublisher(IEMBPublishCallbackInterface* pPublisher, const int nPubType);

	IPluginStorageInterface* m_pIStorage;
	IPluginTaskCommit* m_pTaskCommit;
	ST_TASKSTORAGECONFIG m_cfgStorage;
};

}// name space EMB

extern EMB::CStorageMgr* g_pPluginInstane;
