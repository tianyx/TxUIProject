/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:07
	filename: 	IEMBBaseInterface.h
	author:		tianyx
	
	purpose:	emb plugin interface
*********************************************************************/
#pragma once
#include "EMBGuid.h"
#include <vector>
#include "EMBDefine.h"
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
	PluginType_Wroker			=0x40
};

enum ENUM_SUBTYPE
{
	SubType_None		= 0x0,
	SubType_FileTrans	= 0x1,
	SubType_FileCodec	= 0x2,
	SubType_FileCheck	= 0x4,
	SubType_MD5Check	= 0x8,
	SubType_WorkSample	=0x10

};



interface ITxUnkown
{
	int m_nRef;
	ITxUnkown():m_nRef(0){}
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
	virtual HANDLE LoadPlugin(const GUID guidIn, IPluginBaseInterface*& pInterfaceOut) = 0;
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
};

interface ITaskWorkerCallInterface:virtual public ITxUnkown
{
	virtual HRESULT DoTask(const CTaskString& szTaskIn, CTaskString& szRet, ITaskReportToExcutorInterface* pICallback) = 0;
	virtual HRESULT CancelTask() = 0;
	virtual HRESULT GetTaskProgress(CTaskString& szInfo) = 0;
};



//////////////////////////////////////////////////////////////////////////
typedef vector<CTaskString> VECTASKS;
interface IPluginStorageInterface:virtual public ITxUnkown
{
	virtual HRESULT UpdateTaskToStorage(const DISPATCHID nDispatchID, CTaskString& szTaskIn) = 0;
	virtual HRESULT FetchTaskFromStorage(const DISPATCHID nDispatchID, int nMinPriority, int nDesiredNum, VECTASKS& vTasks) = 0;
	virtual HRESULT GetDispatchedTaskFromStorage(const DISPATCHID nDispatchID, VECTASKS& vTasks) = 0;

};


}//namespace EMB