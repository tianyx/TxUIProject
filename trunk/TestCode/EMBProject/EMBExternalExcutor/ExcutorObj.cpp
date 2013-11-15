#include "stdafx.h"
#include "ExcutorObj.h"
#include "MBCTransMsg.h"
#include "EMBCommonFunc.h"
#include "embstructdef.h"
#include "maindef.h"
#include "EMBDocDef.h"
#include "EMBDefine.h"
#include "TxParamString.h"

LRESULT CALLBACK ExcObjectWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL RegisterExcObjectWnd();
BOOL g_bRegisterExcMgrWnd = RegisterExcObjectWnd();
static TCHAR g_szSockWndClsName[] = TEXT ("ExcObjectMsgWnd");
CExcutorObj* g_pExcObject = NULL;

BOOL RegisterExcObjectWnd()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASSEX   wndclassex = {0};
	wndclassex.cbSize        = sizeof(WNDCLASSEX);
	wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc   = ExcObjectWndProc;
	wndclassex.cbClsExtra    = 0;
	wndclassex.cbWndExtra    = 0;
	wndclassex.hInstance     = hInstance;
	wndclassex.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
	wndclassex.hCursor       = LoadCursor (NULL, IDC_ARROW);
	wndclassex.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
	wndclassex.lpszMenuName  = NULL;
	wndclassex.lpszClassName = g_szSockWndClsName;
	wndclassex.hIconSm       = wndclassex.hIcon;

	if ( 0== RegisterClassEx (&wndclassex))
	{
		HRESULT hr = GetLastError();
		ASSERT(FALSE);

	}

	return TRUE;
}

//CreateThreadWindow
DWORD __stdcall ExcObjMsgLoopThread( void* lparam )
{
	CExcutorObj* pExcObj = (CExcutorObj*)lparam;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HWND& hwnd = pExcObj->m_hwndExcMsg;
	ASSERT(hwnd == NULL);
	hwnd = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW, 
		g_szSockWndClsName, 
		TEXT (""),
		WS_OVERLAPPEDWINDOW,
		-10, 
		-10, 
		2, 
		2, 
		NULL, 
		NULL, 
		hInstance,0);

	ASSERT(::IsWindow(hwnd));
	//::SetWindowLong(hwnd,  GWL_USERDATA, (LONG)pMgr);

	//CFWriteLog(TEXT("SockmsgWnd = %x, bind sock %d"), hwnd, this->m_hSock);

	ShowWindow (hwnd, SW_HIDE);
	UpdateWindow (hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK ExcObjectWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message > MSG_EMBMSGMIN
		&&message < MSG_EMBMSGMAX)
	{
		if (g_pExcObject)
		{
			g_pExcObject->SaveMessageToPool(hwnd, message, wParam, lParam);
		}	
	}
	else
	{
		return 0;
	}
	
	return DefWindowProc (hwnd, message, wParam, lParam);
}

DWORD __stdcall ExcMsgPoolCheckProc(LPVOID lparam)
{
	CExcutorObj* pMgr = (CExcutorObj*)lparam;
	if(pMgr != NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	pMgr->WndMsgPoolCheckLoop();
	return 0;
}

DWORD __stdcall ExcTaskCheckProc(LPVOID lparam)
{
	CExcutorObj* pMgr = (CExcutorObj*)lparam;
	if(pMgr != NULL)
	{
		ASSERT(FALSE);
		return 0;
	}
	pMgr->TaskCheckLoop();
	return 0;
}
//////////////////////////////////////////////////////////////////////////
CExcutorObj* CExcutorObj::m_spExcObj = NULL;
CExcutorObj::CExcutorObj( )
{
	m_pTaskDllInterface = NULL;
	m_hMapping = NULL;
	m_hThreadTaskCheck = NULL;
	m_hThreadMsgLoop = NULL;
	m_hThreadMsgPoolCheck = NULL;
	m_hEventQuitLoop = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventPoolMsgArrival = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_bRegistered = FALSE;


	nfgTaskCheckInterval = 1;
	nfgIdleReportInterval = 10;
	nfgIdleTimeoutForQuit = 60;


	ASSERT(g_pExcObject == NULL);
	g_pExcObject  = this;
	LoadPluginManager();
}

CExcutorObj::~CExcutorObj()
{
	g_pExcObject = NULL;
	UnLoadPluginManager();
}

BOOL CExcutorObj::Start()
{
	Stop();
	m_hMapping = CreateMappingFile();
	m_strActorMappingName = GetActorMappingName(m_hwndActor, m_ExcutorGuid);
	m_hThreadMsgLoop = CreateThread(NULL, 0, ExcObjMsgLoopThread, (LPVOID)this, 0, 0);
	if (m_hThreadMsgLoop)
	{
		int i = 0;
		while(m_hwndExcMsg == 0 && i <10000)
		{
			++i;
			Sleep(1);
		}
	}

	m_hThreadMsgPoolCheck = CreateThread(NULL, 0, ExcMsgPoolCheckProc, (LPVOID)this, 0, 0);


	if (m_hwndExcMsg)
	{
		::SendMessage(m_hwndActor, MSG_EMBEXCUTORREG, (WPARAM)m_ExcutorGuid,  (LPARAM)m_hwndExcMsg);
	}

	//create taskcheck loop
	m_hThreadTaskCheck = CreateThread(NULL, 0, ExcTaskCheckProc, (LPVOID)this, 0, 0);
	return m_hwndExcMsg != NULL;
}

BOOL CExcutorObj::Stop()
{
	return TRUE;

	if (m_hThreadMsgLoop)
	{

		::DestroyWindow(m_hwndExcMsg);
		if (WaitForSingleObject(m_hThreadMsgLoop, 10000) != WAIT_OBJECT_0)
		{
			TerminateThread(m_hThreadMsgLoop, 0);
		}
		m_hThreadMsgLoop = NULL;
		m_hwndExcMsg = NULL;

	}

	if (m_hThreadMsgPoolCheck)
	{
		SetEvent(m_hEventQuitLoop);
		if(WaitForSingleObject(m_hThreadMsgPoolCheck, 5000) != WAIT_OBJECT_0)
		{
			TerminateThread(m_hThreadMsgPoolCheck, 0);
			m_hThreadMsgPoolCheck = NULL;
		}
	}

	ResetEvent(m_hEventPoolMsgArrival);
	ResetEvent(m_hEventQuitLoop);
	ClearMsgPool();
	if (m_hMapping)
	{
		CloseHandle(m_hMapping);
		m_hMapping = NULL;
	}
}

BOOL CExcutorObj::Init( const EXCUTORID& guidIn,ACTORID actorId,  HWND hActor, HWND hMain )
{
	m_ExcutorGuid = guidIn;
	m_hwndActor = hActor;
	m_hwndMain = hMain;
	m_actorId = actorId;
	return TRUE;
}

CExcutorObj* CExcutorObj::GetExcutorObj( const EXCUTORID& guidIn,ACTORID actorID, HWND hActor, HWND hMain )
{
	if (!m_spExcObj)
	{
		m_spExcObj = new CExcutorObj;
		if(!m_spExcObj->Init(guidIn, actorID, hActor, hMain))
		{
			delete m_spExcObj;
			m_spExcObj = NULL;
		}
	}
	else
	{
		ASSERT(FALSE);
		return NULL;
	}

	return m_spExcObj;
}



BOOL CExcutorObj::Release()
{
	if (m_spExcObj)
	{
		delete m_spExcObj;
		m_spExcObj = NULL;
	}

	return TRUE;
}

BOOL CExcutorObj::OnExcutorRegistered( )
{
	//query task
	CString strMsg;
	strMsg.Format(EDOC_MAINHEADERFMT, 1, embxmltype_excOnIdle, TEXT(""));
	m_bRegistered = TRUE;
	SendToActor(strMsg);
	return TRUE;
}

BOOL CExcutorObj::SendToActor( CString& strMsg )
{
	if (m_hwndActor && ::IsWindow(m_hwndActor)
		&& m_hMapping != NULL)
	{
		ST_EMBTRANSMSG msg(embmsgtype_ExcutorToActorMsg);
		msg.strData = strMsg;
		CEMBAutoBuffer szbuff(msg);
		int nUsed = 0;
		PackMBCMsg(msg, szbuff, szbuff.GetSize(), nUsed);

		CAutoLock m_LocPipe(&m_csPipeWrite);
		char* pmapBuff = (char*)MapViewOfFile(m_hMapping, FILE_MAP_WRITE,0,0,nUsed);
		if (pmapBuff)
		{
			memcpy(pmapBuff, szbuff, nUsed);
			UnmapViewOfFile(pmapBuff);
			::SendMessage(m_hwndActor, MSG_EMBTOACTORMSG, (WPARAM)m_ExcutorGuid, (LPARAM)nUsed);
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else
	{
		//actor is down, quit
		Quit();
	}

	return TRUE;
}

BOOL CExcutorObj::Quit()
{
	return TRUE;
}



HANDLE CExcutorObj::CreateMappingFile()
{
	CString strMapName = GetExcutorMappingName(m_ExcutorGuid, m_hwndActor);
	HANDLE hRecvMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, 1024*10, strMapName);
	if (hRecvMap == NULL)
	{
		ASSERT(FALSE);
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		ASSERT(FALSE);
	}
	return hRecvMap;
}


HRESULT CExcutorObj::SaveMessageToPool( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	if (message == MSG_EMBTOEXCUTORMSG)
	{
		HWND hactorWnd = (HWND)wparam;
		ASSERT(hactorWnd == m_hwndActor);
		int nLen = (LPARAM)lparam;

		CString strMappingName = GetActorMappingName(m_hwndActor, m_ExcutorGuid);
		HANDLE hMapping = OpenFileMapping(FILE_MAP_READ, FALSE, strMappingName);
		if (hMapping == NULL)
		{
			ASSERT(FALSE);
			return FALSE;
		}

		DWORD nReaded = 0;
		byte* pmemBuff = (LPBYTE)MapViewOfFile(hMapping,FILE_MAP_READ,0,0,nLen);
		if (pmemBuff != NULL)
		{
			ST_EMBWNDMSG msg;
			msg.pStr = new char[nLen];
			msg.hwnd = hwnd;
			msg.nBuffLen = nLen;
			memcpy(msg.pStr, pmemBuff, nLen);
			UnmapViewOfFile(pmemBuff);
			CAutoLock lock(&m_csMsgPool);
			m_vMsgPool.push_back(msg);
			SetEvent(m_hEventPoolMsgArrival);
		}
		else
		{
			ASSERT(FALSE);
		}
		CloseHandle(hMapping);

	}
	else if (message == MSG_EMBEXCUTORREGED)
	{
		ST_EMBWNDMSG msg;
		msg.wparam =wparam;
		msg.lparam = lparam;
		msg.hwnd = hwnd;
		msg.pStr= NULL;
		CAutoLock lock(&m_csMsgPool);
		m_vMsgPool.push_back(msg);
		SetEvent(m_hEventPoolMsgArrival);
	}

	return S_OK;
}


HRESULT CExcutorObj::WndMsgPoolCheckLoop()
{
	while(TRUE)
	{
		DWORD dwRet = TxWaitObjWithQuit(m_hEventPoolMsgArrival, m_hEventQuitLoop, INFINITE);
		if (dwRet == WAIT_OBJECT_0)
		{
			VECWNDMSG vMsgs;
			//check pool
			{
				CAutoLock lock(&m_csMsgPool);
				vMsgs = m_vMsgPool;
				m_vMsgPool.clear();
				ResetEvent(m_hEventPoolMsgArrival);
			}

			for (size_t i = 0; i < vMsgs.size(); ++i)
			{
				//process the msg
				ST_EMBWNDMSG& msg = vMsgs[i];
				OnActorMessage(msg);
				if (msg.pStr)
				{
					delete[] msg.pStr;
					msg.pStr = NULL;
				}
			}

		}
		else
		{
			//quit
			break;
		}
	}
	return S_OK;

}

HRESULT CExcutorObj::OnActorMessage( ST_EMBWNDMSG& msgIn )
{
	if (msgIn.message == MSG_EMBEXCUTORREGED)
	{
		OnExcutorRegistered();
	}
	else if (msgIn.message == MSG_EMBTOEXCUTORMSG)
	{
		if (msgIn.pStr && msgIn.nBuffLen > 0)
		{
			int nType = GetMBCMsgType(msgIn.pStr, msgIn.nBuffLen);
			if (nType ==embmsgtype_ActorToExcutorMsg)
			{
				ST_EMBTRANSMSG msgActor(nType);
				UnPackMBCMsg(msgIn.pStr, msgIn.nBuffLen, msgActor);
				CString strRet;
				OnActorMessage(msgActor.strData, strRet);
				if (!strRet.IsEmpty())
				{
					//send back;
					SendToActor(strRet);
				}
			}
			else
			{
				ASSERT(FALSE);
			}
		}
		else
		{
			ASSERT(FALSE);
		}
		CString strInfo;

	}
	return S_OK;
}

HRESULT CExcutorObj::OnActorMessage( CString& strInfo, CString& strRet )
{
	ST_EMBXMLMAININFO mainInfo;
	GetEmbXmlMainInfo(strInfo, mainInfo);
	if (mainInfo.nType == embxmltype_task)
	{
		if (!m_strTask.IsEmpty())
		{
			//report state error
			ST_TASKREPORT report;
			report.actorId = m_actorId;
			report.excutorId = m_ExcutorGuid;
			report.strGuid = mainInfo.guid;
			report.nState = embtaskstate_error;
			report.ToString(strRet);
		}
		else
		{
			//wait for task check proc
			CAutoLock lock(&m_csTaskInfo);
			m_strTask = strInfo;
			m_runState.nState = embtaskstate_dispatched;

		}
	}
	else if (mainInfo.nType == embxmltype_taskReport)
	{
		ST_TASKREPORT report;
		report.FromString(strInfo);
		TXGUID guid = String2Guid(report.strGuid);
		if (m_runState.guid != guid)
		{
			//error guid
			report.nState = embtaskstate_error;
			report.nSubErrorCode = EMBERR_NOTMYTASK;
			report.excutorId = m_ExcutorGuid;
			report.actorId = m_actorId;
		}
		else
		{
			CAutoLock lock(&m_csTaskInfo);
			report.nState = m_runState.nState;
			report.nStep = m_runState.nCurrStep;
			report.nPercent = m_runState.nPercent;
		}
		report.ToString(strRet);
	}

	return S_OK;
}

HRESULT CExcutorObj::TaskCheckLoop()
{
	int nTaskCheckTime = 0;
	int nIdleCheckTime = 0;
	int nIdleQuitTime = 0;

	while(TRUE)
	{


		if (WaitForSingleObject(m_hEventQuitLoop, 1000)!= WAIT_OBJECT_0)
		{
			BOOL bIdle = TRUE;
			if (m_bRegistered)
			{		
				BOOL bReport = FALSE;
				CTaskString szInfo;
				if (!m_strTask.IsEmpty())
				{
					if (m_runState.nState == embtaskstate_error)
					{
						//task error;
						OnTaskProgress(-1);
						continue;
					}
					// task 
					bIdle = FALSE;
					if (m_vSubTasks.size() == 0)
					{
						//first dispatched, create sub task and reset task run state
						InitTask();
					}
					else
					{
						nTaskCheckTime = (++nTaskCheckTime)%nfgTaskCheckInterval;
						{
							CAutoLock lock(&m_csTaskInfo);
							if (nTaskCheckTime == 0 && m_pTaskDllInterface)
							{
								int nPercent = 0;
								HRESULT hr = m_pTaskDllInterface->GetTaskProgress(szInfo);
								ASSERT(hr == S_OK);
								bReport = TRUE;
							}
						}
					}
				}

				if (bReport)
				{
					OnDllReportTaskProgress(szInfo);
				}

			}
			
			if (bIdle)
			{
				//wait while time out
				
				if (m_bRegistered)
				{
					nIdleCheckTime = (++nIdleQuitTime)%nfgIdleReportInterval;
					if (nIdleCheckTime == 0)
					{
						OnExcutorRegistered();
					}
				}
				nIdleQuitTime++;
				if (nIdleQuitTime > nfgIdleTimeoutForQuit)
				{
					Quit();
					nIdleQuitTime = 0;
					break;
				}
			}
			else
			{
				nIdleQuitTime = 0;
				nIdleCheckTime = 0;
			}
		}
		else
		{
			//quit
			break;
		}


	}

	return S_OK;
}

void CExcutorObj::ClearMsgPool()
{
	CAutoLock lock(&m_csMsgPool);
	for (size_t i = 0 ;i < m_vMsgPool.size(); ++i)
	{
		char* pBuff = m_vMsgPool[i].pStr;
		if (pBuff)
		{
			delete[] pBuff;
		}
	}
	m_vMsgPool.clear();
}

HRESULT CExcutorObj::OnDllReportTaskProgress( const CTaskString& szInfo )
{
	CTxStrConvert val;
	val.SetVal(szInfo);
	int nPercent = val.GetAsInt(-1);
	if (nPercent == -1)
	{
		ASSERT(FALSE);
	}

	OnTaskProgress(nPercent);
	return S_OK;
}

HRESULT CExcutorObj::InitTask()
{
	CAutoLock lock(&m_csTaskInfo);
	//the info is locked outside;
	if (m_strTask.IsEmpty() || m_vSubTasks.size() > 0)
	{
		ASSERT(FALSE);
		return EMBERR_INVALIDARG;
	}

	ST_TASKBASIC basicInfo;
	CTxParamString txParam(m_strTask);
	CString strPath = EPATH_TASKBASIC;
	CTxParamString txBasicStr;
	txParam.GetSubNodeString(strPath, txBasicStr);
	if (txBasicStr.IsEmpty() ||basicInfo.strGuid.IsEmpty())
	{
		ASSERT(FALSE);
		//clean task
		m_strTask.Empty();
		m_runState.nState = embtaskstate_none;
		return EMBERR_INVALIDARG;
	}

	HRESULT hr = S_OK;
	if ( basicInfo.nStartStep >= basicInfo.vSubTask.size())
	{
		ASSERT(FALSE);
		hr = EMBERR_TASKSTEPERR;

	}
	else
	{
		for (size_t i = 0; i< basicInfo.vSubTask.size(); ++i)
		{
			CTxParamString txsubTask;
			CString strsubPath = EPATH_MAIN;
			strsubPath += TEXT("\\");
			strsubPath += basicInfo.vSubTask[i];
			txParam.GetSubNodeString(strsubPath, txsubTask);
			if (txsubTask.IsEmpty())
			{
				ASSERT(FALSE);
				hr = EMBERR_SUBTASKERR;
				break;
			}

			ST_SUBTASKDATA subData;
			subData.nType = txsubTask.GetAttribVal(basicInfo.vSubTask[i], EA_SUBTASK_TYPE).GetAsInt(0);
			subData.strName = txsubTask.GetAttribVal(basicInfo.vSubTask[i], EA_SUBTASK_NAME).GetAsString();
			if (subData.nType == 0 || subData.strName.IsEmpty())
			{
				ASSERT(FALSE);
				hr = EMBERR_SUBTASKTYPEERR;
				break;
			}
			CString strPath2 = TEXT(".\\");
			strPath2 += basicInfo.vSubTask[i];
			strPath2 += TEXT("\\");
			strPath2 += subData.strName;
			CTxParamString txsub2;
			txsubTask.GetSubNodeString(strPath2, txsub2);
			subData.strSubTask = txsub2;
			if (txsub2.IsEmpty())
			{
				ASSERT(FALSE);
				hr = EMBERR_SUBTASKTEXT;
				break;
			}
			m_vSubTasks.push_back(subData);
		}
	}

	if (hr == S_OK)
	{
		m_runState.guid = String2Guid(basicInfo.strGuid);
		m_runState.actorId = m_actorId;
		m_runState.excId = m_ExcutorGuid;
		m_runState.nCurrStep = basicInfo.nStartStep;

		//LoadTaskDll and run it
		UnLoadTaskDll();
		LunchTaskDll(m_vSubTasks[m_runState.nCurrStep].nType);
		CString strRet;
		if (m_pTaskDllInterface)
		{
			hr = m_pTaskDllInterface->DoTask(m_vSubTasks[m_runState.nCurrStep].strSubTask, strRet, this);
			if (hr != S_OK)
			{
				ASSERT(FALSE);
			}
		}
		else
		{
			ASSERT(FALSE);
			hr = EMBERR_SUBTASKLOADDLL;
		}
	}
	if (hr != S_OK)
	{
		//report error;
		//set task string none
		m_strTask.Empty();
		m_vSubTasks.clear();
		ST_TASKREPORT report;
		report.nState = embtaskstate_error;
		report.strGuid = basicInfo.strGuid;
		report.nSubErrorCode = hr;
		CString strRet;
		report.ToString(strRet);
		SendToActor(strRet);
	}
	return hr;	
}

void CExcutorObj::OnTaskProgress( int nPercent )
{
	if (nPercent < 0 || nPercent > 100)
	{
		ASSERT(FALSE);
	}

	CAutoLock lock(&m_csTaskInfo);
	m_runState.nPercent = nPercent;
	if (nPercent >= 100)
	{
		if (m_runState.nCurrStep == m_vSubTasks.size()-1)
		{
			//task finished
			m_runState.nState = embtaskstate_finished;
			//clear task 
			m_strTask.Empty();
			m_vSubTasks.clear();
		}
		else
		{
			//start next step
			m_runState.nCurrStep++;
			//change dll, and send sub task info
		}
	}
	else if (nPercent < 0)
	{
		//error
		m_runState.nState = embtaskstate_error;
	}
	

	ST_TASKREPORT report;
	report.nState = m_runState.nState;
	report.nStep = m_runState.nCurrStep;
	report.nPercent = m_runState.nPercent;
	CString strRet;
	report.ToString(strRet);
	SendToActor(strRet);
}

BOOL CExcutorObj::LunchTaskDll( int nTaskType )
{
	ST_LOADEDPLUGIN tmpPlugin;
	if (LoadPluginByPluginMgr(nTaskType, SubType_None, g_pIPluginMgr, tmpPlugin))
	{
		m_taskDll = tmpPlugin;
		tmpPlugin.pIface->QueryInterface(GuidEMBPlugin_ITaskExcDll, (LPVOID&) m_pTaskDllInterface );
		if (!m_pTaskDllInterface)
		{
			UnLoadTaskDll();
			ASSERT(FALSE);
		}
		
	}

	return TRUE;

}

BOOL CExcutorObj::UnLoadTaskDll()
{
	if (m_pTaskDllInterface)
	{
		m_pTaskDllInterface->Release();
	}
	
	if (m_taskDll.handle)
	{
		UnLoadPluginByPluginMgr(g_pIPluginMgr, m_taskDll);
	}
		
	return TRUE;
}


