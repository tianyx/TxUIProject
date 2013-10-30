#pragma once
#include "EMBGuid.h"
#include <vector>

typedef CString CTaskString;

namespace EMB{

enum ENUM_PLUGINTYPE
{
	PluinType_None				= 0x0,
	PluinType_PluginManager		= 0x1,
	PluginType_TaskRiser	= 0x2,
	PluginType_Dispatcher		= 0x4,
	PluginType_ActorManager		= 0x8,
	PluginType_Actor			= 0x10,
};

enum ENUM_SUBTYPE
{
	SubType_None		= 0x0,
	SubType_TransFile	= 0x1,
	SubType_TransCode	= 0x2,
	SubType_Mediacheck	= 0x4,
	SubType_MD5Check	= 0x8
};



interface ITxUnkown
{
	int m_nRef;
	ITxUnkown():m_nRef(1){}
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
	virtual HRESULT FindPlugin(const UINT nPluginType, const UINT nSubType, GUID& guidOut) = 0;
	virtual HRESULT LoadPlugin(GUID& guidIn, IPluginBaseInterface*& pInterfaceOut) = 0;
};

//plugin task generate////////////////////////////////////////////////////////////////////////
// interface IPluginTaskRiserInterface:virtual public ITxUnkown
// {
// 	virtual HRESULT AddTaskProber(const CTaskString& szTaskTypeDesc, CTaskString& szRet) = 0;
// 	//virtual HRESULT ConnectTo(IPluginBaseInterface* pInterface) = 0;
// };

//plugin task dispatch////////////////////////////////////////////////////////////////////////
interface IPluginTaskDispatch:virtual public ITxUnkown
{
	virtual HRESULT SubmitTask(const CTaskString& szTaskIn, CTaskString& szRet) = 0;
};

//plugin task Actor////////////////////////////////////////////////////////////////////////
interface IActorMsgCallBackInterface
{
	virtual HRESULT ActCallbackProc(CTaskString& szActMsg) = 0;
};
interface IPluginActorManager:virtual public ITxUnkown
{
	virtual HRESULT GetActorList(CTaskString& szActorsDesc) = 0;
	virtual UINT RegisterActorReceiver(const CTaskString& szActorDesc, IActorMsgCallBackInterface* pInterface) = 0;
	virtual UINT UnRegisterActorReceiver(UINT nReceiverID) = 0;
	virtual HRESULT AssignTask(const CTaskString& szTaskIn, CTaskString& szRet) = 0;
};

//plugin excutor////////////////////////////////////////////////////////////////////////
interface IExcutorMsgCallBack
{
	virtual HRESULT ExcutorCallbackProc(CTaskString& szMsg) = 0;
};
interface IPluginActorInterface:virtual public ITxUnkown
{
	virtual HRESULT GetExcutorList(CTaskString& szRet) = 0;
	virtual UINT AddExcutor(CTaskString& szExcutorDesc) = 0;
	virtual HRESULT RemoveExcutor(UINT nExcutorId) = 0;
};

//plugin IExcutor//////////////////////////////////////////////////////////////////////
interface IPluginExcutorInterface:virtual public ITxUnkown
{
	virtual HRESULT GetState(CTaskString& szRet) = 0;
	virtual UINT RegisterCallback(CTaskString& szDesc, IExcutorMsgCallBack* pInterface) = 0;
	virtual HRESULT UnRegisterCallback(UINT nActorId);
};

//plugin task 

interface ITaskCreateInterface
{
	HRESULT AssignTask();
};

interface IUIInterface
{
	HWND GetMainWnd();
};


}//namespace EMB