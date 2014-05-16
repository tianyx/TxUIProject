#include "StdAfx.h"
#include "EMBInfoPublishMgr.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "TxAutoComPtr.h"

#define MAXTASKLIMIT 1000

using namespace EMB;

/*
* Description：接收任务插件主线程
* Input Param：
*		parmIn：调用线程指针
* Return Param：返回成功或失败
* History：
*/
DWORD __stdcall TaskLoopProc(LPVOID parmIn)
{
	if (parmIn == NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	CEMBInfoPublishMgr* pProber = (CEMBInfoPublishMgr*)parmIn;
	pProber->TaskCheckProc();
	return 0;
}

CEMBInfoPublishMgr::CEMBInfoPublishMgr()
{
	m_hTaskCheckProc = NULL;
	m_hQuitEvent = NULL;
	m_hTaskEvent = NULL;
	m_pPublishRegister = NULL;
	m_cfgPub.vRegTypes.push_back(embpublishtype_ontaskend);

	LoadPublishPlugin();
}

CEMBInfoPublishMgr::~CEMBInfoPublishMgr()
{
	g_pPluginInstane = NULL;
	UnLoadPublishPlugin();
}

/*
*Description：初始化函数
*Input Param：
*Return Param：
*History：
*/
HRESULT CEMBInfoPublishMgr::OnFirstInit()
{
	//初始化日志
	CString strFile(GetAppPath().c_str());
	strFile +=TEXT("\\log\\EMBInfoPublish.log");
	GetTxLogMgr()->AddNewLogFile(LOGKEY_EMBInfoPublish, strFile);

	return S_OK;
}


void CEMBInfoPublishMgr::OnFinalRelease()
{
    //与下级连接插件-任务存储插件连接断开
	if (m_pPublishRegister)
	{
		Disconnect(m_pPublishRegister);
	}
	g_pPluginInstane = NULL;

	//日志插件释放
	ReleaseTxLogMgr();
	TRACE("\nCEMBInfoPublishMgr::OnFinalRelease() ");
	delete this; 
}

/*
*Description：查询插件信息，加入到插件列表中
*Input Param：
*Return Param：
*History：
*/
HRESULT CEMBInfoPublishMgr::QueryPluginInfo( VECPLUGINFOS& vInfoInOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_PEMBInfoPublish;
	info.nPlugInType = PluginType_InfoPublish;
	info.nSubType = SubType_None;
	vInfoInOut.push_back(info);
	return S_OK;
}

/*
*Description：获取插件接口
*Input Param：
*Return Param：
*History：
*/
HRESULT CEMBInfoPublishMgr::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
{
	pInterfaceOut = NULL;

// 	if (guidIn == GuidEMBPlugin_EMBInfoPublish)
// 	{
// 		pInterfaceOut = dynamic_cast<IPluginEMBInfoPublishInterface*>(this);
// 		AddRef();
// 		return S_OK;
// 	}
	if (guidIn == GuidEMBPlugin_IBase)
	{
		pInterfaceOut = dynamic_cast<IPluginBaseInterface*>(this);
		AddRef();
		return S_OK;
	}
	else if (guidIn == GuidEMBPlugin_IControler)
	{
		pInterfaceOut = dynamic_cast<IPluginControlInterface*>(this);
		AddRef();
		return S_OK;

	}
	else if (guidIn == GuidEMBPlugin_IConnector)
	{
		pInterfaceOut = dynamic_cast<IPluginConnectorInterce*>(this);
		AddRef();
		return S_OK;

	}
	else if (guidIn == GuidEMBPlugin_IConfig)
	{
		pInterfaceOut = dynamic_cast<IPluginConfigInterface*>(this);
		AddRef();
		return S_OK;

	}
	else
	{
		return __super::QueryInterface(guidIn, pInterfaceOut);
	}
}

/*
*Description：启动插件服务
*Input Param：
*Return Param：返回成功或失败
*History：
*/
HRESULT CEMBInfoPublishMgr::Run_Plugin()
{
	Stop_Plugin();
	m_nMaxTaskLimit = MAXTASKLIMIT;
	m_hQuitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hTaskEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//启动监听线程
	m_hTaskCheckProc = CreateThread(NULL, NULL, TaskLoopProc, (LPVOID)this, 0, 0);
	ASSERT(m_hTaskCheckProc != NULL);

	//register notifier
	IEMBInfoPublishRegisterInterface* pIRegister = dynamic_cast<IEMBInfoPublishRegisterInterface*>(m_pPublishRegister);
	if (pIRegister)
	{
		for (size_t i = 0; i < m_cfgPub.vRegTypes.size(); ++i)
		{
			pIRegister->RegisterPublisher(this,  m_cfgPub.vRegTypes[i]);
		}
	}
	//
	return S_OK;
}

/*
*Description：停止插件服务
*Input Param：
*Return Param：返回成功或失败
*History：
*/
HRESULT CEMBInfoPublishMgr::Stop_Plugin()
{


	IEMBInfoPublishRegisterInterface* pIRegister = dynamic_cast<IEMBInfoPublishRegisterInterface*>(m_pPublishRegister);
	if (pIRegister)
	{
		for (size_t i = 0; i < m_cfgPub.vRegTypes.size(); ++i)
		{
			pIRegister->UnRegisterPublisher(this,  m_cfgPub.vRegTypes[i]);
		}
	}

	SetEvent(m_hQuitEvent);
	if (m_hTaskCheckProc)
	{
		if (WaitForSingleObject(m_hTaskCheckProc, 5000) != WAIT_OBJECT_0)
		{
			TerminateThread(m_hTaskCheckProc, 0);
		}
		m_hTaskCheckProc = NULL;
	}
	m_vPubTasks.clear();
	if (m_hQuitEvent)
	{
		CloseHandle(m_hQuitEvent);
		m_hQuitEvent = NULL;
	}
	if (m_hTaskEvent)
	{
		CloseHandle(m_hTaskEvent);
		m_hTaskEvent = NULL;
	}
	

	return S_OK;
}

/*
* Description：接收任务执行线程
* Input Param：
* Return Param：返回成功或失败
* History：
*/
BOOL CEMBInfoPublishMgr::TaskCheckProc()
{

		while(TRUE)
		{
			//等待两个事件都到位或超时：TASK接收到的事件或退出事件
			DWORD dwWait = TxWaitObjWithQuit(m_hTaskEvent, m_hQuitEvent);

			if (dwWait == WAIT_OBJECT_0)
			{
				//have one or more tasks
				VECPUBINFODATA vTasks;
				//get task list
				PopTask(vTasks);
				for (size_t i = 0; i < vTasks.size(); ++i)
				{
					ST_PUBLISHINFODATA& dataRef = vTasks[i];
					if (m_vPubDlls.size() > 0)
					{
						//submit task to StoragePulgIn
						for (size_t j = 0; j < m_vPubDlls.size(); ++j)
						{
							ST_PUBLISHDLL& dllRef = m_vPubDlls[j];
							if ((dataRef.nType & dllRef.nType) != 0)
							{
								dllRef.pFunc(dataRef.strPubData, dataRef.nType);
							}
						}
					}
				}
			}
			else
			{
				break;
			}
		}

		return TRUE;


}

BOOL CEMBInfoPublishMgr::PopTask( VECPUBINFODATA& vTaskOut )
{

		CAutoLock locTask(&m_lockTask);
		vTaskOut = m_vPubTasks;
		m_vPubTasks.clear();
		ResetEvent(m_hTaskEvent);
		return TRUE;
}

/*
* Description：连接任务存储插件
* Input Param：
*		pInterfaceIn：存储插件指针
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CEMBInfoPublishMgr::Connect( ITxUnkown* pInterfaceIn )
{
	if (m_pPublishRegister)
	{
		ASSERT(FALSE);
		CFWriteLog(0, TEXT("EMBInfoPublishMgr plugin already connected!"));
		return E_FAIL;
	}

	CTxAutoComPtr<IUIMessageProcessInterface> apUIMsgProcess;
	pInterfaceIn->QueryInterface(GuidEMBPublishRegister, (LPVOID&)*&apUIMsgProcess);
	if (apUIMsgProcess)
	{
		
		m_pPublishRegister = apUIMsgProcess;
		m_pPublishRegister->AddRef();

		
	}

	return S_OK;
}

/*
* Description：断开任务存储插件
* Input Param：
*		pInterfaceIn：存储插件指针
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CEMBInfoPublishMgr::Disconnect( ITxUnkown* pInterfaceIn )
{
	//find if have connected this interface
	HRESULT hr = S_OK;
	if (pInterfaceIn == m_pPublishRegister)
	{
		
		m_pPublishRegister->Release();
		m_pPublishRegister = NULL;
	}
	else
	{
		return E_FAIL;
	}

	
	return hr;
}

/*
* Description：连接任务存储插件与传入插件？功能是否完成
* Input Param：
*		pInterfaceIn：接收插件指针
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CEMBInfoPublishMgr::OnConnect( ITxUnkown* pInterfaceIn )
{
	CFWriteLog(0, TEXT("CEMBInfoPublishMgr::OnConnect not implemented"));
	return E_NOTIMPL;
}

/*
* Description：存储插件与接收插件断开
* Input Param：
*		pInterfaceIn：接收插件指针
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CEMBInfoPublishMgr::OnDisconnect( ITxUnkown* pInterfaceIn )
{
	CFWriteLog(0, TEXT("CEMBInfoPublishMgr::OnDisconnect not implemented"));

	return E_NOTIMPL;
}

/*
* Description：获取插件配置函数
* Input Param：
*		szIn：查询参数标识
*       szOut:返回查询结果
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CEMBInfoPublishMgr::GetParam( const CTaskString& szIn, CTaskString& szOut )
{
	return S_OK;
}

/*
* Description：设置插件配置函数
* Input Param：
*		szIn：传入配置文件内容
*       szOut:返回值
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CEMBInfoPublishMgr::SetParam( const CTaskString& szIn, CTaskString& szOut )
{
	
	return S_OK;
}

HRESULT EMB::CEMBInfoPublishMgr::OnReceivePublishInfo( CTaskString& strInfoIn, const int nPubType )
{
	if (strInfoIn.IsEmpty())
	{
		return E_INVALIDARG;
	}
	CAutoLock locTask(&m_lockTask);
	if (m_vPubTasks.size() > MAXTASKLIMIT)
	{
		CFWriteLog(0, TEXT("publish task exceededed than %d!, pubdata will discard!"), MAXTASKLIMIT);
		return EMBERR_FULL;
	}
	if (nPubType & m_nAllNeedPubType)
	{
		m_vPubTasks.push_back(ST_PUBLISHINFODATA(nPubType, strInfoIn));
		SetEvent(m_hTaskEvent);
	}
	return S_OK;
}

BOOL EMB::CEMBInfoPublishMgr::UnLoadPublishPlugin()
{
	for (size_t i = 0; i < m_vPubDlls.size(); ++i)
	{
		UnLoadPublishDll(m_vPubDlls[i].hmodule);
	}

	m_vPubDlls.clear();
	m_nAllNeedPubType = 0;

	return TRUE;
}

BOOL EMB::CEMBInfoPublishMgr::LoadPublishPlugin()
{
	UnLoadPublishPlugin();
	char szPath[MAX_PATH];
	GetModuleFileName(g_hGlobalDllModule, szPath, MAX_PATH);
	CString strThisPath = szPath;
	int nPos = strThisPath.ReverseFind(_T('\\'));
	CString strFileFmt = strThisPath.Left(nPos);
	VECSTRINGS vFiles;
	GetFilesInFolder(strFileFmt, vFiles, TEXT("epp"), FALSE);
	for (size_t i = 0; i < vFiles.size(); ++i)
	{
		CString strDllFile =vFiles[i];
		ST_PUBLISHDLL dllInfo;
		if (LoadPublishDll(strDllFile, dllInfo.hmodule, dllInfo.pFunc, dllInfo.nType))
		{
			m_nAllNeedPubType |=dllInfo.nType;
			dllInfo.strFile = strDllFile;
			m_vPubDlls.push_back(dllInfo);
		}
	}

	return TRUE;
}


BOOL LoadPublishDll(const CString& strFileIn, HMODULE& hModuleOut, FUNCEMBPUBLISHINFO& pOut, int& nDesiredPubType)
{
	int nPos = strFileIn.ReverseFind(_T('\\'));
	CString strDllPath = strFileIn.Left(nPos);
	SetDllDirectory(strDllPath);
	hModuleOut = LoadLibrary(strFileIn);
	if (hModuleOut != NULL)
	{
		FUNCEMBPUBLISHINFO  pFun = (FUNCEMBPUBLISHINFO)GetProcAddress(hModuleOut, "OnPublishEmbInfo");
		FUNCGETDESIREDPUBTYPE  pFun2 = (FUNCGETDESIREDPUBTYPE)GetProcAddress(hModuleOut, "GetDesiredPubType");

		if (pFun && pFun2)
		{	nDesiredPubType = pFun2();
			pOut = pFun;
			return TRUE;
		}
		else
		{
			FreeLibrary(hModuleOut);
			hModuleOut = NULL;
		}

	}
	return FALSE;
}


BOOL UnLoadPublishDll(HMODULE hModuleIn)
{
	BOOL bRet = TRUE;
	if (hModuleIn)
	{
		bRet = FreeLibrary(hModuleIn);
	}

	return bRet;
}


BOOL ST_PUBLISHINFODATA::ToString()
{
	return TRUE;
}

BOOL ST_PUBLISHINFODATA::FromString()
{
	return TRUE;
}
