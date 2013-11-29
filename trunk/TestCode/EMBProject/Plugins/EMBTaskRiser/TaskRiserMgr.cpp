#include "StdAfx.h"
#include "TaskRiserMgr.h"
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

	CTaskRiserMgr* pProber = (CTaskRiserMgr*)parmIn;
	pProber->TaskCheckProc();
	return 0;
}

CTaskRiserMgr::CTaskRiserMgr()
{
	m_pTaskStorage = NULL;
	m_hTaskCheckProc = NULL;
	m_hQuitEvent = NULL;
	m_hTaskEvent = NULL;
}

CTaskRiserMgr::~CTaskRiserMgr()
{
	g_pPluginInstane = NULL;
}

/*
*Description：初始化函数
*Input Param：
*Return Param：
*History：
*/
HRESULT CTaskRiserMgr::OnFirstInit()
{
	//初始化日志
	CString strFile(GetAppPath().c_str());
	strFile +=TEXT("\\log\\TaskRiser.log");
	GetTxLogMgr()->AddNewLogFile(LOGKEY_TASKRISER, strFile);

	return S_OK;
}

/*
*Description：释放函数
*Input Param：
*Return Param：
*History：
*/
void CTaskRiserMgr::OnFinalRelease()
{
    //与下级连接插件-任务存储插件连接断开
	if (m_pTaskStorage)
	{
		Disconnect(m_pTaskStorage);
	}
	g_pPluginInstane = NULL;

	//日志插件释放
	ReleaseTxLogMgr();
	TRACE("\nCTaskRiserMgr::OnFinalRelease() ");
	delete this; 
}

/*
*Description：查询插件信息，加入到插件列表中
*Input Param：
*Return Param：
*History：
*/
HRESULT CTaskRiserMgr::QueryPluginInfo( VECPLUGINFOS& vInfoInOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_PTaskRiser;
	info.nPlugInType = PluginType_TaskRiser;
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
HRESULT CTaskRiserMgr::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
{
	pInterfaceOut = NULL;

// 	if (guidIn == GuidEMBPlugin_TaskRiser)
// 	{
// 		pInterfaceOut = dynamic_cast<IPluginTaskRiserInterface*>(this);
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
HRESULT CTaskRiserMgr::Run_Plugin()
{
	Stop_Plugin();
	m_nMaxTaskLimit = MAXTASKLIMIT;
	m_hQuitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hTaskEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//启动监听线程
	m_hTaskCheckProc = CreateThread(NULL, NULL, TaskLoopProc, (LPVOID)this, 0, 0);
	ASSERT(m_hTaskCheckProc != NULL);
	HRESULT hr = S_OK;
	//create task prob
	for (int i = 0; i < m_config.vProbes.size(); ++i)
	{
		if (m_config.vProbes[i].nType == embTaskproberType_tcp)
		{
			CTaskProberTcp* pProb = new CTaskProberTcp;
			//设置地址
			pProb->SetScokAddr(&m_config.vProbes[i].data.ipdata.addrListen, &m_config.vProbes[i].data.ipdata.addrListen);
			//设置回调函数
			pProb->SetTaskProcessor(this);  //没明白这个设置和TaskCollectCallback如何关联在一起的，与TaskProberTcp又是如何起作用的？
			//启动监听
			hr =pProb->Run_Prober();
			ASSERT(SUCCEEDED(hr));
			//？
			m_vProbers.push_back(pProb);
			
		}
	}


	return S_FALSE;
}

/*
*Description：停止插件服务
*Input Param：
*Return Param：返回成功或失败
*History：
*/
HRESULT CTaskRiserMgr::Stop_Plugin()
{

	for (size_t i = 0; i < m_vProbers.size(); ++i)
	{
		CTaskProber* pProb =m_vProbers[i];
		if (pProb)
		{
			pProb->Stop_Prober();
			delete pProb;
		}
	}
	m_vProbers.clear();

	SetEvent(m_hQuitEvent);
	if (m_hTaskCheckProc)
	{
		if (WaitForSingleObject(m_hTaskCheckProc, 5000) != WAIT_OBJECT_0)
		{
			TerminateThread(m_hTaskCheckProc, 0);
		}
		m_hTaskCheckProc = NULL;
	}
	m_vTasks.clear();
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
BOOL CTaskRiserMgr::TaskCheckProc()
{

		while(TRUE)
		{
			//等待两个事件都到位或超时：TASK接收到的事件或退出事件
			DWORD dwWait = TxWaitObjWithQuit(m_hTaskEvent, m_hQuitEvent);

			if (dwWait == WAIT_OBJECT_0)
			{
				//have one or more tasks
				VECTASKDATAS vTasks;
				//get task list
				PopTask(vTasks);
				for (size_t i = 0; i < vTasks.size(); ++i)
				{
					if (m_pTaskStorage)
					{
						//submit task to StoragePulgIn
						CString strRet;
						m_pTaskStorage->SubmitTask(vTasks[i].taskDesc, strRet);
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

/*
* Description：接收到任务XML回调函数
* Input Param：
*		taskIn：任务XML字符串
* Return Param：返回成功或失败
* History：
*/
HRESULT CTaskRiserMgr::TaskCollectCallback( CString& taskIn )
{
	if(taskIn.IsEmpty())
	{
		ASSERT(FALSE);
		return EMBERR_INVALIDARG;
	}
	//判断是否超过最大任务数
	if (m_vTasks.size() == m_nMaxTaskLimit)
	{
		CFWriteLog(LOGKEY_TASKRISER, TEXT("task list is full!!"));
		return EMBERR_FULL;
	}

	CAutoLock locTask(&m_lockTask);
	//赋值任务信息变脸
	ST_EMBTASKDATA task;
	task.taskDesc = taskIn;
	//加入任务组中
	m_vTasks.push_back(task);
	SetEvent(m_hTaskEvent);
	return S_OK;
}

/*
* Description：获取任务列表
* Input Param：
*		vTaskOut：任务列表引用变量
* Return Param：返回成功或失败
* History：
*/
BOOL CTaskRiserMgr::PopTask( VECTASKDATAS& vTaskOut )
{

		CAutoLock locTask(&m_lockTask);
		vTaskOut = m_vTasks;
		m_vTasks.clear();
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
HRESULT EMB::CTaskRiserMgr::Connect( ITxUnkown* pInterfaceIn )
{
	if (pInterfaceIn == m_pTaskStorage)
	{
		return S_OK;
	}
	if (m_pTaskStorage)
	{
		ASSERT(FALSE);
		Disconnect(m_pTaskStorage);
		m_pTaskStorage = NULL;
	}
	pInterfaceIn->QueryInterface(GuidEMBPlugin_ITaskCommit, (LPVOID&) m_pTaskStorage);
	if (m_pTaskStorage)
	{
		CTxAutoComPtr<IPluginConnectorInterce> pConn;
		m_pTaskStorage->QueryInterface(GuidEMBPlugin_IConnector, (LPVOID&) (*&pConn));
		ASSERT(pConn);
		pConn->OnConnect(this);
	
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

/*
* Description：断开任务存储插件
* Input Param：
*		pInterfaceIn：存储插件指针
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CTaskRiserMgr::Disconnect( ITxUnkown* pInterfaceIn )
{
	//find if have connected this interface
	HRESULT hr = S_FALSE;

	BOOL bFound = FALSE;
	if (m_pTaskStorage == pInterfaceIn)
	{
		bFound = TRUE;
	}
	else
	{
		CTxAutoComPtr<IPluginTaskCommit> pDispath;
		if (pInterfaceIn->QueryInterface(GuidEMBPlugin_ITaskCommit, (LPVOID&)(*&pDispath))
			&& pDispath == m_pTaskStorage)
		{
			bFound = TRUE;
		}
	}
	
	if (bFound)
	{
		//found it, disconnect the interface
		CTxAutoComPtr<IPluginConnectorInterce> pConn;
		m_pTaskStorage->QueryInterface(GuidEMBPlugin_IConnector, (LPVOID&) (*&pConn));
		ASSERT(pConn);
		pConn->OnDisconnect(this);
		m_pTaskStorage->Release();
		m_pTaskStorage = NULL;
		hr =S_OK;
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
HRESULT EMB::CTaskRiserMgr::OnConnect( ITxUnkown* pInterfaceIn )
{
	if (m_pTaskStorage != NULL)
	{
		ASSERT(FALSE);
		return E_FAIL;
	}
	if (pInterfaceIn->QueryInterface(GuidEMBPlugin_ITaskCommit, (LPVOID&)(*&m_pTaskStorage))
		&& m_pTaskStorage != NULL)
	{
		return S_OK;
	}
	else
	{
		return S_FALSE;
	}
}

/*
* Description：存储插件与接收插件断开
* Input Param：
*		pInterfaceIn：接收插件指针
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CTaskRiserMgr::OnDisconnect( ITxUnkown* pInterfaceIn )
{
	if (m_pTaskStorage == NULL)
	{
		ASSERT(FALSE);
		return E_FAIL;
	}

	HRESULT hr = S_FALSE;
	BOOL bFound = FALSE;
	if (m_pTaskStorage == pInterfaceIn)
	{
		bFound = TRUE;
	}
	else
	{
		CTxAutoComPtr<IPluginTaskCommit> pDispath;
		if (pInterfaceIn->QueryInterface(GuidEMBPlugin_ITaskCommit, (LPVOID&)(*&pDispath))
			&& pDispath == m_pTaskStorage)
		{
			bFound = TRUE;
		}
	}

	if (bFound)
	{
		//found it, disconnect the interface
		m_pTaskStorage->Release();
		m_pTaskStorage = NULL;
		hr =S_OK;
	}


	return hr;
}

/*
* Description：获取插件配置函数
* Input Param：
*		szIn：查询参数标识
*       szOut:返回查询结果
* Return Param：返回成功或失败
* History：
*/
HRESULT EMB::CTaskRiserMgr::GetParam( const CTaskString& szIn, CTaskString& szOut )
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
HRESULT EMB::CTaskRiserMgr::SetParam( const CTaskString& szIn, CTaskString& szOut )
{
	ST_TASKRISERCONFIG tmpConfig;
	tmpConfig.FromString(szIn);
	if (tmpConfig.vProbes.size() == 0)
	{
		return E_INVALIDARG;
	}
	m_config = tmpConfig;
	return S_OK;
}
