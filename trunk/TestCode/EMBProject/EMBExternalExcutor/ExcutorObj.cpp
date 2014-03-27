#include "stdafx.h"
#include "ExcutorObj.h"
#include "MBCTransMsg.h"
#include "EMBCommonFunc.h"
#include "embstructdef.h"
#include "maindef.h"
#include "EMBDocDef.h"
#include "EMBDefine.h"
#include "TxParamString.h"
#include "Util.h"

LRESULT CALLBACK ExcObjectWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
CExcutorObj* g_pExcObject = NULL;



//CreateThreadWindow
DWORD __stdcall ExcObjMsgLoopThread( void* lparam )
{
	CExcutorObj* pExcObj = (CExcutorObj*)lparam;
	HINSTANCE hInstance = GetSelfModuleHandle();

	WNDCLASSEX   wndclassex = {0};
	wndclassex.cbSize        = sizeof(WNDCLASSEX);
	wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc   = ExcObjectWndProc;
	wndclassex.cbClsExtra    = 0;
	wndclassex.cbWndExtra    = 0;
	wndclassex.hInstance     = hInstance;
	wndclassex.hIcon         = NULL;
	wndclassex.hCursor       = NULL;
	wndclassex.hbrBackground = NULL;
	wndclassex.lpszMenuName  = NULL;
	CString strClsName = Guid2String(TxGenGuid());
	wndclassex.lpszClassName = strClsName;
	wndclassex.hIconSm       = wndclassex.hIcon;

	if ( 0 == RegisterClassEx (&wndclassex))
	{
		HRESULT hr = GetLastError();
		ASSERT(FALSE);
	}

	HWND& hwnd = pExcObj->m_hwndExcMsg;
	ASSERT(hwnd == NULL);
	hwnd = CreateWindowEx (WS_EX_OVERLAPPEDWINDOW, 
		wndclassex.lpszClassName, 
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
	if (message > MSG_EMBMSGMIN && message < MSG_EMBMSGMAX)
	{
		if (g_pExcObject)
		{
			// put msg into m_vMsgPool
			g_pExcObject->SaveMessageToPool(hwnd, message, wParam, lParam);
		}

		return 0;
	}
	
	// Window msg
	return DefWindowProc (hwnd, message, wParam, lParam);
}

DWORD __stdcall ExcMsgPoolCheckProc(LPVOID lparam)
{
	CExcutorObj* pMgr = (CExcutorObj*)lparam;

	if(pMgr == NULL)
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
	if(pMgr == NULL)
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
	m_hwndExcMsg = NULL;

	nfgTaskCheckInterval = 1;
	nfgIdleReportInterval = 5;
	nfgIdleTimeoutForQuit = 20;
	nfgTaskRetryMax = 1;
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

	// 创建MappingFile: 用于Actor.exe 与 Executor.exe通信
	m_hMapping = CreateMappingFile();
	// 接受消息线程
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
	else
	{
		return FALSE;
	}

	m_hThreadMsgPoolCheck = CreateThread(NULL, 0, ExcMsgPoolCheckProc, (LPVOID)this, 0, 0);

	if (m_hwndExcMsg)
	{
		CFWriteLog(0, TEXT("send reg msg to actor %x"), m_executorReg.hwndActor);
		// 向Actor发送 注册信息
		::SendMessage(m_executorReg.hwndActor, MSG_EMBEXCUTORREG, (WPARAM)m_executorReg.guid,  (LPARAM)m_hwndExcMsg);
	}

	//create taskcheck loop
	m_hThreadTaskCheck = CreateThread(NULL, 0, ExcTaskCheckProc, (LPVOID)this, 0, 0);
	return m_hwndExcMsg != NULL;
}

BOOL CExcutorObj::Stop()
{
	if (m_hThreadMsgLoop)
	{
		::PostMessage(m_hwndExcMsg, WM_QUIT, 0,0);
		WaitForSingleObject(m_hThreadMsgLoop, INFINITE);
		CloseHandle(m_hThreadMsgLoop);
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

	return TRUE;
}

BOOL CExcutorObj::Init( ST_EXCUTORREG& tExecutorReg, HWND hMain )
{
	m_executorReg = tExecutorReg;
	m_hwndMain = hMain;

	return TRUE;
}

CExcutorObj* CExcutorObj::GetExcutorObj( ST_EXCUTORREG& tExecutorReg, HWND hMain )
{
	if (!m_spExcObj)
	{
		m_spExcObj = new CExcutorObj;
		if(!m_spExcObj->Init(tExecutorReg, hMain))
		{
			PTRDELETE(m_spExcObj);
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
	PTRDELETE(m_spExcObj);

	return TRUE;
}

BOOL CExcutorObj::OnExcutorRegistered( )
{
	// 向Actor.exe 发送消息
	CFWriteLog(0, TEXT("send idle msg to actor."));
	CTxStrConvert strVal;
	strVal.SetVal(m_executorReg.guid);

	CString strMsg;
	strMsg.Format(EDOC_MAINHEADERFMT, 1, embxmltype_excOnIdle,  strVal);
	m_bRegistered = TRUE;
	SendToActor(strMsg); // Executor处于空闲

	return TRUE;
}

BOOL CExcutorObj::SendToActor( CString& strMsg )
{
	if (m_executorReg.hwndActor && ::IsWindow(m_executorReg.hwndActor)
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
			// SendMessage 消息处理后才返回
			::SendMessage(m_executorReg.hwndActor, MSG_EMBTOACTORMSG, (WPARAM)(m_executorReg.guid), (LPARAM)nUsed);
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
	SendMessage(m_executorReg.hwndActor, MSG_EMBEXCUTORQUIT, (WPARAM)(m_executorReg.guid), 0);
	::PostMessage(m_hwndMain, MSG_EMBKILLEXCUTOR, 0,0);
	return TRUE;
}



HANDLE CExcutorObj::CreateMappingFile()
{
	CString strMapName = GetExcutorMappingName(m_executorReg.guid, m_executorReg.hwndActor);
	HANDLE hRecvMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, 1024*10, strMapName);
	if (hRecvMap == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
	{
		ASSERT(FALSE);
	}

	return hRecvMap;
}


HRESULT CExcutorObj::SaveMessageToPool( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	// 从 file mapping object 读取消息内容
	if (message == MSG_EMBTOEXCUTORMSG)
	{
		HWND hactorWnd = (HWND)wparam;
		ASSERT(hactorWnd == m_executorReg.hwndActor);
		int nLen = (LPARAM)lparam;

		CString strMappingName = GetActorMappingName(m_executorReg.hwndActor, m_executorReg.guid);
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
			msg.message =message;
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
		msg.message = message;
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

		if (WAIT_OBJECT_0 != dwRet)
		{
			break;

		}

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

	return S_OK;
}

HRESULT CExcutorObj::OnActorMessage( ST_EMBWNDMSG& msgIn )
{
	if (msgIn.message == MSG_EMBEXCUTORREGED)
	{
		CFWriteLog(0, TEXT("actor returned reg successful msg."));
		OnExcutorRegistered();
	}
	else if (msgIn.message == MSG_EMBTOEXCUTORMSG)
	{
		if (msgIn.pStr && msgIn.nBuffLen > 0)
		{
			int nType = GetMBCMsgType(msgIn.pStr, msgIn.nBuffLen);
			if (nType == embmsgtype_ActorToExcutorMsg) // Actor.exe 发送的消息
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
	}

	return S_OK;
}

HRESULT CExcutorObj::OnActorMessage( CString& strInfo, CString& strRet )
{
	ST_EMBXMLMAININFO mainInfo;
	GetEmbXmlMainInfo(strInfo, mainInfo);
	if (mainInfo.nType == embxmltype_task) // 提交任务
	{
		CFWriteLog(0, TEXT("receive task %s"), mainInfo.guid);
		if (!m_strTask.IsEmpty()) // 是否有正在执行的任务?
		{
			//report state error
			ST_TASKREPORT report;
			report.actorId = m_executorReg.actorId;
			report.excutorId = m_executorReg.guid;
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
			m_runState.nPercent = 0;

		}
	}
	else if (mainInfo.nType == embxmltype_taskReport) // 查询任务状态
	{
		CFWriteLog(0, TEXT("actor ask taskReport."));
		ST_TASKREPORT report;
		report.FromString(strInfo);
		TXGUID guid = String2Guid(report.strGuid);
		if (m_runState.guid != guid) // 当前执行的任务不是指定的查询任务
		{
			//error guid
			ASSERT(FALSE);
			report.nState = embtaskstate_error;
			report.nSubErrorCode = EMBERR_NOTMYTASK;
			report.excutorId = m_executorReg.guid;
			report.actorId = m_executorReg.actorId;
		}
		else
		{
			CAutoLock lock(&m_csTaskInfo);
			report.nState = m_runState.nState;
			if (m_runState.nCurrStep == -1)
			{
				report.nStep = 0;
				report.nPercent = 0;
			}
			else
			{
				report.nStep = m_runState.nCurrStep;
				report.nPercent = m_runState.nPercent;
				report.nExcType = m_runState.nExcType;
			}
		}
		report.ToString(strRet);
	}
	else if (embxmltype_taskCancel == mainInfo.nType) // 取消任务
	{
		m_runState.nState = embtaskstate_cancle;
		CFWriteLog(0, TEXT("Excutor receive cancle task %s"), strInfo);
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
		// to do 可能超时，将跳过分步任务处理
		if (WaitForSingleObject(m_hEventQuitLoop, 1000) != WAIT_OBJECT_0)
		{
			BOOL bIdle = TRUE;
			if (m_bRegistered)
			{
				CAutoLock lock(&m_csTaskInfo);
				BOOL bReport = FALSE;
				CTaskString szInfo;
				if (!m_strTask.IsEmpty())
				{
					if (m_runState.nState == embtaskstate_error)
					{
						//task error;
						//unload the dll
						UnLoadTaskDll();
						//clean the task
						m_strTask.Empty();
						m_vSubTasks.clear();
						m_runState.nState = embtaskstate_none;
						
						continue;
					}
					else if (m_runState.nState == embtaskstate_dispatched)
					{
						OutputDebugString("---check step task");
						if (m_runState.nPercent == 100)
						{
							OutputDebugString("---next step");
							//start next step
							m_runState.nCurrStep++;  // 当前处理步骤
							if (m_runState.nCurrStep>= 0 && m_runState.nCurrStep < m_vSubTasks.size())
							{
								m_runState.nExcType = m_vSubTasks[m_runState.nCurrStep].nType;
							}
							
							m_runState.nPercent = 0;
							//change dll, and send sub task info
							HRESULT hr = LaunchTask();
							if (hr != S_OK)
							{
								//dll launch failed
								m_runState.nState = embtaskstate_error;
							}
							continue;
						}
					}
					else if (m_runState.nState == embtaskstate_finished)
					{
						//clear task
						UnLoadTaskDll();
						m_strTask.Empty();
						m_vSubTasks.clear();
						m_runState.nState = embtaskstate_none;
						continue;
					}
					else if (m_runState.nState == embtaskstate_cancle)
					{
						m_pTaskDllInterface->CancelTask();
						m_runState.nState = embtaskstate_error;
						
						continue;
					}
					
					// task 
					bIdle = FALSE;
					if (m_vSubTasks.size() == 0)
					{
						//first dispatched, create sub task and reset task run state
						InitTask();
						OutputDebugString("---inittask");
					}
					else
					{
						nTaskCheckTime = (++nTaskCheckTime)%nfgTaskCheckInterval;
						{
							if (nTaskCheckTime == 0 && m_pTaskDllInterface)
							{
								int nPercent = 0;
								// 查询任务执行状态信息
								CEMBWorkString szWorkStr;
								HRESULT hr = m_pTaskDllInterface->GetTaskProgress(szWorkStr);
								szInfo = szWorkStr;
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
				if (m_pTaskDllInterface)
				{
					UnLoadTaskDll();
				}
				
				if (m_bRegistered)
				{
					nIdleCheckTime = (++nIdleQuitTime)%nfgIdleReportInterval;
					if (nIdleCheckTime == 0)
					{
						//not send idle, disable it temporarily
						//OnExcutorRegistered();
					}
				}
				nIdleQuitTime++;
				if (nIdleQuitTime > nfgIdleTimeoutForQuit)
				{
					//if debug mode not quit
					if (m_executorReg.hwndActor != GetDesktopWindow())
					{
						Quit();
						nIdleQuitTime = 0;
						break;

					}
					else
					{
						continue;
					}
					
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
	ST_WORKERREPORT workreport;
	workreport.FromString(szInfo); // xml msg to data
	int nPercent = workreport.nPercent;

	if (nPercent == -1 || FAILED(workreport.code)) // 任务失败
	{
		nPercent = -1;
		//ASSERT(FALSE);
	}

	OnTaskProgress(nPercent); // 进度信息
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
	basicInfo.FromString(txBasicStr); // xml to Task
	if (txBasicStr.IsEmpty() ||basicInfo.strGuid.IsEmpty())
	{
		//ASSERT(FALSE);
		//clean task
		ResetTaskInfor();
		m_runState.nState = embtaskstate_none;
		return EMBERR_INVALIDARG;
	}

	CFWriteLog(0, TEXT("start process task %s"), basicInfo.strGuid);

	HRESULT hr = S_OK;
	if ( basicInfo.nStartStep >= (int)basicInfo.vSubTask.size())
	{
		ASSERT(FALSE);
		hr = EMBERR_TASKSTEPERR;

	}
	else
	{
		// 分步任务处理
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

			// 将分步任务放入 m_vSubTasks
			m_vSubTasks.push_back(subData);
		}
	}

	if (hr == S_OK)
	{
		m_runState.guid = String2Guid(basicInfo.strGuid);
		m_runState.actorId = m_executorReg.actorId;
		m_runState.excId = m_executorReg.guid;
		m_runState.nCurrStep = basicInfo.nStartStep;
		m_runState.nPercent = 0;
		m_runState.nExcType = m_vSubTasks[m_runState.nCurrStep].nType;

		// 启动媒体处理分步任务
		hr = LaunchTask();
	}
	if (hr != S_OK)
	{
		//report error;
		//set task string none
		ResetTaskInfor();
		ST_TASKREPORT report;
		report.nState = embtaskstate_error;
		report.strGuid = basicInfo.strGuid;
		report.nSubErrorCode = hr;	// 具体错误信息
		CString strRet;
		report.ToString(strRet);
		CFWriteLog(0, TEXT("task %s init failed, code = %x,report to actor "), report.strGuid, hr);
		SendToActor(strRet);
	}
	return hr;	
}

void CExcutorObj::OnTaskProgress( int nPercent, HRESULT codeIn )
{
	CFWriteLog(0, TEXT("task %s step %d, percent %d"), Guid2String(m_runState.guid), m_runState.nCurrStep, nPercent);
	if (nPercent < 0 || nPercent > 100)
	{
		//ASSERT(FALSE);
	}

	CAutoLock lock(&m_csTaskInfo);
	m_runState.nPercent = nPercent;
	if (nPercent >= 100)
	{
		// 当前分步任务是否为 最后一步的任务?
		if (m_runState.nCurrStep >= (int)m_vSubTasks.size()-1)
		{
			//task finished
			m_runState.nState = embtaskstate_finished;
		}
		else
		{
			//wait check thread to launch next workdll
		}
		m_runState.nPercent = 100;
	}
	else if (nPercent < 0)
	{
		if (codeIn !=EMBERR_INVALIDARG &&m_runState.nRetry < nfgTaskRetryMax)
		{
			m_runState.nRetry++;
			m_runState.nState = embtaskstate_dispatched;
			m_runState.nCurrStep--;
			m_runState.nPercent =100;
			//back to the task init
			CFWriteLog(0, TEXT("task setp [%d] err, rollback and retry!"), m_runState.nCurrStep+1);
		}
		else
		{
			//error
			m_runState.nState = embtaskstate_error;
		}

	}

	

	ST_TASKREPORT report;
	report.strGuid = Guid2String(m_runState.guid);
	report.actorId = m_executorReg.actorId;
	report.excutorId = m_executorReg.guid;
	report.nState = m_runState.nState;
	report.nStep = m_runState.nCurrStep;
	report.nPercent = m_runState.nPercent;
	report.nExcType = m_runState.nExcType;
	CString strRet;
	report.ToString(strRet);
	SendToActor(strRet); // 向Actor.exe 反馈任务状态信息
}

BOOL CExcutorObj::LunchTaskDll( int nTaskType )
{
	BOOL bOk = FALSE;
	CFWriteLog(0, TEXT("lunch task dll type = %d"), nTaskType);
	ST_LOADEDPLUGIN tmpPlugin;
	if (LoadPluginByPluginMgr(PluginType_Wroker, nTaskType, g_pIPluginMgr, tmpPlugin))
	{
		m_taskDll = tmpPlugin;
		tmpPlugin.pIface->QueryInterface(GuidEMBPlugin_ITaskWorkerCall, (LPVOID&) m_pTaskDllInterface );
		if (!m_pTaskDllInterface)
		{
			UnLoadTaskDll();
		}
		else
		{
			bOk = TRUE;
		}
	}

	if (!bOk)
	{
		CFWriteLog(0, TEXT("workplugin load failed! type = %d"), nTaskType);
	}

	return bOk;
}

BOOL CExcutorObj::UnLoadTaskDll()
{
	CFWriteLog(0, TEXT("unload work dll"));
	if (m_pTaskDllInterface)
	{
		m_pTaskDllInterface->Release();
		m_pTaskDllInterface = NULL;
	}
	
	if (m_taskDll.handle)
	{
		UnLoadPluginByPluginMgr(g_pIPluginMgr, m_taskDll);
		m_taskDll.handle = NULL;
		m_taskDll.pIface = NULL;
	}
		
	return TRUE;
}

HRESULT CExcutorObj::LaunchTask()
{
	HRESULT hr = S_OK;
	CFWriteLog(0, TEXT("start launch workdll"));
	//LoadTaskDll and run it
	UnLoadTaskDll();

	// 根据分步任务类型，加载对应的功能dll
	BOOL bOk = LunchTaskDll(m_vSubTasks[m_runState.nCurrStep].nType);
	if (!bOk)
	{
		OutputDebugString("---未加载功能dll");
		return EMBERR_SUBTASKLOADDLL; // 未加载功能dll
	}

	CString strRet;

	if (m_pTaskDllInterface)
	{
		// 向功能dll提交分步任务
		CEMBWorkString szWorkRet;
		hr = m_pTaskDllInterface->DoTask(m_vSubTasks[m_runState.nCurrStep].strSubTask, szWorkRet, this);
		if (hr != S_OK)
		{
			hr = EMBERR_TASKSUBMIT; // 提交失败
			//ASSERT(FALSE);
		}
		else
		{
			//time to check if need change task or other action
			CString strRetInfo = szWorkRet;
			if (!strRetInfo.IsEmpty())
			{
				ST_WORKERRET workRet;
				CString strTmp = szWorkRet;
				workRet.FromString(strTmp);
				ST_EXCCALLBACKINFO callInfo;
				callInfo.nActorId = m_executorReg.actorId;
				callInfo.nExcId = m_executorReg.guid;
				callInfo.nRetType = workRet.nRetType;
				callInfo.strExtInfo = workRet.strRetInfo;
				callInfo.nStep = m_runState.nCurrStep;
				CString strCallRet;
				callInfo.ToString(strCallRet);
				SendToActor(strCallRet);
			}
			
		}
		CFWriteLog(0, TEXT("end launch workdll"));
	}
	else
	{
		//ASSERT(FALSE);
		hr = EMBERR_SUBTASKLOADDLL;
	}

	return hr;
}

BOOL CExcutorObj::TestRunTask( CString& strTaskIn )
{
	if (!m_strTask.IsEmpty())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_bRegistered = TRUE;
	m_vSubTasks.clear();
	m_strTask = strTaskIn;
	m_runState.nState = embtaskstate_dispatched;
	m_runState.nPercent = 0;
	return TRUE;
}

void CExcutorObj::ResetTaskInfor()
{
	m_strTask.Empty();
	m_vSubTasks.clear();
}



