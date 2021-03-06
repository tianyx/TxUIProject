/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:07
	filename: 	IEMBBaseInterface.h
	author:		tianyx
	
	purpose:	emb plugin interface
*********************************************************************/
#pragma once
#include "EMBGuid.h"
#include "EMBWorkString.h"
#include <vector>

#ifdef VC6PREDEF
typedef int EXCUTORID;
typedef int ACTORID;
typedef int DISPATCHID;
#else
#include "EMBDefine.h"
#endif // VC6PREDEF

using namespace std;
typedef CString CTaskString;
namespace EMB{

enum ENUM_PLUGINTYPE
{
	PluinType_None				= 0x0,
	PluinType_PluginManager		= 0x1,
	PluginType_TaskRiser		= 0x2,
	PluginType_Dispatcher		= 0x4,
	PluginType_ActorManager		= 0x8,
	PluginType_Actor			= 0x10,
	PluginType_Storage			= 0x20,
	PluginType_Wroker			= 0x40,
	PluginType_UIServer			= 0x80,
	PluginType_InfoPublish		= 0x100,

};

enum ENUM_SUBTYPE
{
	SubType_None		= 0,
	SubType_FileTrans	= 1,
	SubType_FileCodec	= 2,
	SubType_MediaCheck	= 4,
	SubType_MD5Check	= 8,
	SubType_WorkSample	= 16,
	SubType_DBWriter	= 32,
	SubType_MCResultMerge	= 64,
	SubType_MergeDBWriter	= 128,
	SubType_Sleeper	= 256,


};


//接口基类
interface ITxUnkown
{
	int m_nRef;
	ITxUnkown():m_nRef(0){}
	virtual ~ITxUnkown(){}
	virtual void AddRef()
	{
		++m_nRef;
		if (m_nRef == 1)
		{
			OnFirstInit();
		}
	}
	virtual void Release()
	{
		--m_nRef;
		if (m_nRef == 0)
		{
			OnFinalRelease();
		}
	}
	//manual call it after new.
	virtual HRESULT OnFirstInit()
	{
		return E_NOTIMPL;
	}
	virtual void OnFinalRelease()
	{
		return ;
	}
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut)
	{
		pInterfaceOut = NULL;
		if (guidIn == GuidEMBPlugin_IUnknow)
		{
			AddRef();
			pInterfaceOut = this;
			return S_OK;
		}
		return E_NOTIMPL;
	}
};

struct ST_PluginInfo
{
	GUID pluginGuid;
	DWORD nPlugInType;
	DWORD nSubType;
	ST_PluginInfo()
	{
		pluginGuid = GUID_NULL;
		nPlugInType = 0;
		nSubType = 0;
	}

};
//////////////////////////////////////////////////////////////////////////
typedef std::vector<ST_PluginInfo> VECPLUGINFOS;
//all plugin must implement this////////////////////////////////////////////////////////////////////////
interface IPluginBaseInterface :virtual public ITxUnkown
{
	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoOut) = 0;
};
//VC6.0接口
interface IPluginBaseInterfaceVC6 :virtual public ITxUnkown
{
	virtual HRESULT QueryPluginInfo(ST_PluginInfo& infoOut) = 0;
};

//////////////////////////////////////////////////////////////////////////
interface IPluginConfigInterface :virtual public ITxUnkown
{
	virtual HRESULT GetParam(const CTaskString& szIn, CTaskString& szOut) = 0;
	virtual HRESULT SetParam(const CTaskString& szIn, CTaskString& szOut) = 0;
};

//////////////////////////////////////////////////////////////////////////
//to connect plugin, only call connect once
interface IPluginConnectorInterce:virtual public ITxUnkown
{
	//you riseup a action to connect/disconnect to other plugin
	virtual HRESULT Connect(ITxUnkown* pInterfaceIn) = 0;
	virtual HRESULT Disconnect(ITxUnkown* pInterfaceIn) = 0;

	//other plugin inform you after riseup a action that Connect()/Disconnect() you
	virtual HRESULT OnConnect(ITxUnkown* pInterfaceIn) = 0;
	virtual HRESULT OnDisconnect(ITxUnkown* pInterfaceIn) = 0;
};
interface IPluginControlInterface:virtual public ITxUnkown
{
	virtual HRESULT Run_Plugin() = 0;
	virtual HRESULT Stop_Plugin() = 0;
};
//plugin manager///////////////////////////////////////////////////////////////////////
interface IPluginManagerInterface:virtual public ITxUnkown
{
	virtual HRESULT InitPluginsSearch(BOOL bSearchDeep, LPCTSTR szFileExtern) = 0;
	virtual HRESULT FindPlugin(const UINT nPluginType, const UINT nSubType, GUID& guidOut) = 0;
	virtual HANDLE LoadPlugin(const GUID guidIn, ITxUnkown*& pInterfaceOut) = 0;
	//this will unload the plugin library, if handle = NULL, unload all plugin that same guid
	virtual HRESULT UnloadPlugin(const GUID guidIn, HANDLE handle) = 0;
};

//plugin task generate////////////////////////////////////////////////////////////////////////
// interface IPluginTaskRiserInterface:virtual public ITxUnkown
// {
// 	virtual HRESULT AddTaskProber(const CTaskString& szTaskTypeDesc, CTaskString& szRet) = 0;
// 	//virtual HRESULT ConnectTo(IPluginBaseInterface* pInterface) = 0;
// };

//to connect with taskriser, must implement this interface////////////////////////////////////////////////////////////////////////
interface IPluginTaskCommit:virtual public ITxUnkown
{
	virtual HRESULT SubmitTask(const CTaskString& szTaskIn, CTaskString& szRet) = 0;
};


//plugin task Actor////////////////////////////////////////////////////////////////////////
interface IActorMsgCallBackInterface
{
	virtual HRESULT ActCallbackProc(CTaskString& szActMsg, CTaskString& szRet) = 0;
};

//
interface ITaskReportToExcutorInterface
{
	virtual HRESULT OnDllReportTaskProgress(const CTaskString& szInfo) = 0;
	virtual HRESULT OnDllRuntimeCall(const CTaskString& szInfo, CEMBWorkString& strRet) = 0;
};

interface ITaskWorkerCallInterface:virtual public ITxUnkown
{
	virtual HRESULT DoTask(const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback) = 0;
	virtual HRESULT CancelTask() = 0;
	virtual HRESULT GetTaskProgress(CEMBWorkString& szInfo) = 0;
};

interface ITaskWorkerOnMessageInterface:virtual public ITxUnkown
{
	virtual HRESULT OnMessageToWorker(const CTaskString& szInfo) = 0;
};



//////////////////////////////////////////////////////////////////////////
typedef vector<CTaskString> VECTASKS;
interface IPluginStorageInterface:virtual public ITxUnkown
{
	virtual HRESULT UpdateTaskToStorage(const DISPATCHID nDispatchID, CTaskString& szTaskIn) = 0;
	virtual HRESULT FetchTaskFromStorage(const DISPATCHID nDispatchID, int nMinPriority, int nDesiredNum, VECTASKS& vTasks) = 0;
	virtual HRESULT GetDispatchedTaskFromStorage(const DISPATCHID nDispatchID, VECTASKS& vTasks) = 0;
	virtual HRESULT UpdateActorID(CTaskString& strTaskGuid, ACTORID actorId){return S_OK;};

};

// Actor 界面获取信息接口
interface IActorUI : virtual public ITxUnkown
{
	virtual HRESULT GetExecutors(vector<CString>& vExecutor) = 0;
	virtual HRESULT GetTaskInActor(vector<CString>& vTask) = 0;
};

//to implement this interface for process UI call
interface IUIMessageProcessInterface : virtual public ITxUnkown
{
	virtual HRESULT OnUIMessage(CTaskString& strMsg, CTaskString& szRet) = 0;
};


//
interface IDispatchNotifyCallbackInterface
{
	virtual HRESULT OnDispatchNotify(CTaskString& strNotify) = 0;
};

interface IDispatchNotifyRegisterInterface: virtual public ITxUnkown
{
	virtual HRESULT RegisterNotifier(IDispatchNotifyCallbackInterface* pNotifier) = 0;
	virtual HRESULT UnRegisterNotifier(IDispatchNotifyCallbackInterface* pNotifier) = 0;
};

// Server 界面获取信息接口
interface IServerUI : virtual public ITxUnkown
{
	virtual HRESULT GetActors(vector<CString>& vActor) = 0;
};


//for publish
//
interface IEMBPublishCallbackInterface
{
	virtual HRESULT OnReceivePublishInfo(CTaskString& strInfoIn, const int nPubType) = 0;
};

//see embpublishtype_xxx
interface IEMBInfoPublishRegisterInterface: virtual public ITxUnkown
{
	virtual HRESULT RegisterPublisher(IEMBPublishCallbackInterface* pPublisher, const int nPubType) = 0;
	virtual HRESULT UnRegisterPublisher(IEMBPublishCallbackInterface* pPublisher, const int nPubType) = 0;

};

}//namespace EMB
