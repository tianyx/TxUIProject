#include "StdAfx.h"
#include "TaskRiserMgr.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "TxAutoComPtr.h"

#define MAXTASKLIMIT 1000

using namespace EMB;

/*
* Description���������������߳�
* Input Param��
*		parmIn�������߳�ָ��
* Return Param�����سɹ���ʧ��
* History��
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
*Description����ʼ������
*Input Param��
*Return Param��
*History��
*/
HRESULT CTaskRiserMgr::OnFirstInit()
{
	//��ʼ����־
	CString strFile(GetAppPath().c_str());
	strFile +=TEXT("\\log\\TaskRiser.log");
	GetTxLogMgr()->AddNewLogFile(LOGKEY_TASKRISER, strFile);

	return S_OK;
}

/*
*Description���ͷź���
*Input Param��
*Return Param��
*History��
*/
void CTaskRiserMgr::OnFinalRelease()
{
    //���¼����Ӳ��-����洢������ӶϿ�
	if (m_pTaskStorage)
	{
		Disconnect(m_pTaskStorage);
	}
	g_pPluginInstane = NULL;

	//��־����ͷ�
	ReleaseTxLogMgr();
	TRACE("\nCTaskRiserMgr::OnFinalRelease() ");
	delete this; 
}

/*
*Description����ѯ�����Ϣ�����뵽����б���
*Input Param��
*Return Param��
*History��
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
*Description����ȡ����ӿ�
*Input Param��
*Return Param��
*History��
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
*Description�������������
*Input Param��
*Return Param�����سɹ���ʧ��
*History��
*/
HRESULT CTaskRiserMgr::Run_Plugin()
{
	Stop_Plugin();
	m_nMaxTaskLimit = MAXTASKLIMIT;
	m_hQuitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hTaskEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	//���������߳�
	m_hTaskCheckProc = CreateThread(NULL, NULL, TaskLoopProc, (LPVOID)this, 0, 0);
	ASSERT(m_hTaskCheckProc != NULL);
	HRESULT hr = S_OK;
	//create task prob
	for (int i = 0; i < m_config.vProbes.size(); ++i)
	{
		if (m_config.vProbes[i].nType == embTaskproberType_tcp)
		{
			CTaskProberTcp* pProb = new CTaskProberTcp;
			//���õ�ַ
			pProb->SetScokAddr(&m_config.vProbes[i].data.ipdata.addrListen, &m_config.vProbes[i].data.ipdata.addrListen);
			//���ûص�����
			pProb->SetTaskProcessor(this);  //û����������ú�TaskCollectCallback��ι�����һ��ģ���TaskProberTcp������������õģ�
			//��������
			hr =pProb->Run_Prober();
			ASSERT(SUCCEEDED(hr));
			//��
			m_vProbers.push_back(pProb);
			
		}
	}


	return S_FALSE;
}

/*
*Description��ֹͣ�������
*Input Param��
*Return Param�����سɹ���ʧ��
*History��
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
* Description����������ִ���߳�
* Input Param��
* Return Param�����سɹ���ʧ��
* History��
*/
BOOL CTaskRiserMgr::TaskCheckProc()
{

		while(TRUE)
		{
			//�ȴ������¼�����λ��ʱ��TASK���յ����¼����˳��¼�
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
* Description�����յ�����XML�ص�����
* Input Param��
*		taskIn������XML�ַ���
* Return Param�����سɹ���ʧ��
* History��
*/
HRESULT CTaskRiserMgr::TaskCollectCallback( CString& taskIn )
{
	if(taskIn.IsEmpty())
	{
		ASSERT(FALSE);
		return EMBERR_INVALIDARG;
	}
	//�ж��Ƿ񳬹����������
	if (m_vTasks.size() == m_nMaxTaskLimit)
	{
		CFWriteLog(LOGKEY_TASKRISER, TEXT("task list is full!!"));
		return EMBERR_FULL;
	}

	CAutoLock locTask(&m_lockTask);
	//��ֵ������Ϣ����
	ST_EMBTASKDATA task;
	task.taskDesc = taskIn;
	//������������
	m_vTasks.push_back(task);
	SetEvent(m_hTaskEvent);
	return S_OK;
}

/*
* Description����ȡ�����б�
* Input Param��
*		vTaskOut�������б����ñ���
* Return Param�����سɹ���ʧ��
* History��
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
* Description����������洢���
* Input Param��
*		pInterfaceIn���洢���ָ��
* Return Param�����سɹ���ʧ��
* History��
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
* Description���Ͽ�����洢���
* Input Param��
*		pInterfaceIn���洢���ָ��
* Return Param�����سɹ���ʧ��
* History��
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
* Description����������洢����봫�����������Ƿ����
* Input Param��
*		pInterfaceIn�����ղ��ָ��
* Return Param�����سɹ���ʧ��
* History��
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
* Description���洢�������ղ���Ͽ�
* Input Param��
*		pInterfaceIn�����ղ��ָ��
* Return Param�����سɹ���ʧ��
* History��
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
* Description����ȡ������ú���
* Input Param��
*		szIn����ѯ������ʶ
*       szOut:���ز�ѯ���
* Return Param�����سɹ���ʧ��
* History��
*/
HRESULT EMB::CTaskRiserMgr::GetParam( const CTaskString& szIn, CTaskString& szOut )
{
	return S_OK;
}

/*
* Description�����ò�����ú���
* Input Param��
*		szIn�����������ļ�����
*       szOut:����ֵ
* Return Param�����سɹ���ʧ��
* History��
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
