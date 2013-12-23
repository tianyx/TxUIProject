#include "StdAfx.h"
#include "ExcutorMgr.h"
#include "EMBDefine.h"
#include "io.h"
#include "StrConvert.h"
#include "FGlobal.h"
#include "MBCTransMsg.h"
#include "EMBCommonFunc.h"
#include "EMBMessageDef.h"

LRESULT CALLBACK ExcMgrWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL RegisterExcMgrWnd();
BOOL g_bRegisterExcMgrWnd = RegisterExcMgrWnd();
static TCHAR g_szSockWndClsName[] = TEXT ("ExcMgrMsgWnd");
CExcutorMgr* g_pExcMgr = NULL;

BOOL RegisterExcMgrWnd()
{
	HINSTANCE hInstance = GetSelfModuleHandle();

	WNDCLASSEX   wndclassex = {0};
	wndclassex.cbSize        = sizeof(WNDCLASSEX);
	wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc   = ExcMgrWndProc;
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
		ASSERT(hr == 0x00000582);

	}

	return TRUE;
}

//CreateThreadWindow
DWORD __stdcall ExcMgrMsgLoopThread( void* lparam )
{
	CExcutorMgr* pMgr = (CExcutorMgr*)lparam;
	HINSTANCE hInstance = GetSelfModuleHandle();
	HWND& hwnd = pMgr->m_hMessageWnd;
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

LRESULT CALLBACK ExcMgrWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message > MSG_EMBMSGMIN
		&&message < MSG_EMBMSGMAX)
	{
		if (g_pExcMgr)
		{
			g_pExcMgr->SaveMessageToPool(hwnd, message, wParam, lParam);
		}
		return 0;
	}

	
	return DefWindowProc (hwnd, message, wParam, lParam);
}


DWORD __stdcall ExcutorCreateProc(LPVOID lparam)
{
	CExcutorMgr* pMgr = (CExcutorMgr*)lparam;
	if(pMgr == NULL)
	{
		return 0;
	}

	pMgr->CheckExcutorLoop();
	return 0;


}

DWORD __stdcall MsgPoolCheckProc(LPVOID lparam)
{
	CExcutorMgr* pMgr = (CExcutorMgr*)lparam;
	if(pMgr == NULL)
	{
		ASSERT(FALSE);
		return 0;
	}

	pMgr->WndMsgPoolCheckLoop();
	return 0;


}

//////////////////////////////////////////////////////////////////////////
CExcutorMgr* CExcutorMgr::m_spExcMgr = NULL;
CExcutorMgr::CExcutorMgr(void)
{
	nfgExcutorLaunchTimeout = 30;
#ifdef _DEBUG
	nfgExcutorLaunchTimeout = 30;

#endif // _DEBUG

	m_nActorID = INVALID_ID;
	m_nMinExcutorId = 0;
	m_nMaxExcutorId = 100;
	m_hMessageWnd = NULL;
	m_hCheckThread = NULL;
	m_hMsgLoopThread = NULL;
	m_pIExcCallBack = NULL;
	m_hMsgPoolCheckThread = NULL;
	m_hEventQuitLoop = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventPoolMsgArrival = CreateEvent(NULL, TRUE, FALSE, NULL);
	ASSERT(g_pExcMgr == NULL);
	g_pExcMgr = this;
	
}

CExcutorMgr::~CExcutorMgr(void)
{
	ASSERT(g_pExcMgr == this);
	g_pExcMgr = NULL;
	if (m_hMessageWnd)
	{
		DestroyWindow(m_hMessageWnd);
		m_hMessageWnd = NULL;
	}
}



HRESULT CExcutorMgr::OnExcutorMessage( ST_EMBWNDMSG& msgIn)
{
	if (msgIn.message == MSG_EMBEXCUTORREG)
	{
		return OnExcutorReg(msgIn.hwnd, msgIn.message, msgIn.wparam, msgIn.lparam);
	}
	else if (msgIn.message == MSG_EMBTOACTORMSG)
	{
		if (msgIn.pStr && msgIn.nBuffLen > 0)
		{
			int nType = GetMBCMsgType(msgIn.pStr, msgIn.nBuffLen);
			if (nType ==embmsgtype_ExcutorToActorMsg)
			{
				ST_EMBTRANSMSG msg(nType);
				UnPackMBCMsg(msgIn.pStr, msgIn.nBuffLen, msg);
				if (m_pIExcCallBack)
				{
					m_pIExcCallBack->OnExcutorMessage((EXCUTORID)msgIn.wparam, msg.strData);
				}
			}
			else
			{
				ASSERT(FALSE);
			}
		}

	}
	else if (msgIn.message == MSG_EMBEXCUTORQUIT)
	{
		EXCUTORID excId = (EXCUTORID)msgIn.wparam;
		CFWriteLog(0, TEXT("excutor %d quit"), excId);

		if (m_pIExcCallBack)
		{
			m_pIExcCallBack->OnExcutorExit(excId);
		}

		//remove from map
		CAutoLock lock(&m_csExcutors);
		MAPEXCUTORS::iterator itf = m_mapExcutors.find(excId);
		if (itf != m_mapExcutors.end())
		{
			//delete mapping file
			if (itf->second.hmemMap)
			{
				CloseHandle(itf->second.hmemMap);
			}
			// m_mapExcutors 删除对应的信息
			m_mapExcutors.erase(itf);
			CFWriteLog(0, TEXT("remove executor: %d infor from m_mapExcutors"), excId);
		}
	}
	return S_OK;
}

CExcutorMgr* CExcutorMgr::GetExcutorMgr()
{
	if (m_spExcMgr == NULL)
	{
		m_spExcMgr = new CExcutorMgr;
	}
	return m_spExcMgr;
}

BOOL CExcutorMgr::Release()
{
	if (m_spExcMgr)
	{
		delete m_spExcMgr;
		m_spExcMgr = NULL;
	}

	return TRUE;
}

HRESULT CExcutorMgr::Run()
{
	if (m_strExcPath.IsEmpty())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	Stop();
	m_hMsgLoopThread = CreateThread(NULL, 0, ExcMgrMsgLoopThread, (LPVOID)this,  0,0);
	ASSERT(m_hMsgLoopThread);

	m_hCheckThread = CreateThread(NULL, 0, ExcutorCreateProc, (LPVOID)this, 0, 0);
	ASSERT(m_hCheckThread != NULL);

	m_hMsgPoolCheckThread = CreateThread(NULL, 0, MsgPoolCheckProc, (LPVOID)this, 0, 0);

	return m_hMsgLoopThread != NULL? S_OK:S_FALSE;
}

BOOL CExcutorMgr::Init( LPCTSTR strExcPathIn, ST_ACTORCONFIG& actCfg, IExcutorMsgCallBack* pCallBack)
{
	if (access(strExcPathIn, 0) == -1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	m_pIExcCallBack = pCallBack;
	m_strExcPath = strExcPathIn;
	int nPos = m_strExcPath.ReverseFind('\\');
	m_strExcFolder = m_strExcPath.Left(nPos);
	m_strExcName = m_strExcPath.Mid(nPos+1, m_strExcPath.GetLength() - nPos-1);
	m_strExcutorWorkFolder = m_strExcFolder;
	m_strExcutorWorkFolder +=TEXT("_work");
	CreateDirectory(m_strExcutorWorkFolder, NULL);
	m_nActorID = actCfg.actorId;
	m_nMinExcutorId = actCfg.nExcutorMinId;
	m_nMaxExcutorId = actCfg.nExcutorMaxId;
	return TRUE;
}

HRESULT CExcutorMgr::Stop()
{
	if (m_hMsgLoopThread)
	{

		::PostMessage(m_hMessageWnd, WM_QUIT,0,0);
		WaitForSingleObject(m_hMsgLoopThread, INFINITE);
		CloseHandle(m_hMsgLoopThread);
		m_hMsgLoopThread = NULL;
		m_hMessageWnd = NULL;
	}

	if (m_hCheckThread)
	{
		SetEvent(m_hEventQuitLoop);
		WaitForSingleObject(m_hCheckThread, INFINITE);
		m_hCheckThread = NULL;

		if (m_hMsgPoolCheckThread)
		{
			if(WaitForSingleObject(m_hMsgPoolCheckThread, 5000) != WAIT_OBJECT_0)
			{
				TerminateThread(m_hMsgPoolCheckThread, 0);
			}
			m_hMsgPoolCheckThread = NULL;
		}
	}

	ResetEvent(m_hEventPoolMsgArrival);
	ResetEvent(m_hEventQuitLoop);
	ClearMsgPool();
	m_hMsgLoopThread = NULL;
	m_hMessageWnd = NULL;
	return S_OK;
}


EXCUTORID CExcutorMgr::GetFirstNotUsedExcutorId()
{
	EXCUTORID nRet = INVALID_ID;

	if (m_mapExcutors.size() == 0)
	{
		nRet = m_nMinExcutorId;
	}
	else
	{
		// m_mapExcutors 查询空闲的Executor
// 		MAPEXCUTORS::iterator itor = m_mapExcutors.begin();
// 		for(; itor != m_mapExcutors.end(); ++itor)
// 		{
// 			if (EXE_IDLE == itor->second.m_eState )
// 			{
// 				nRet = itor->first; // idle executor
// 				break;
// 			}
// 		}

		// 若没有查询到空闲Executor
		if (INVALID_ID == nRet && m_mapExcutors.size() < (m_nMaxExcutorId - m_nMinExcutorId + 1))
		{
			for(EXCUTORID id = m_nMinExcutorId; id <= m_nMaxExcutorId; ++id)
			{
				MAPEXCUTORS::iterator findItor = m_mapExcutors.find(id);

				if (findItor == m_mapExcutors.end()) // id 号未使用
				{
					nRet = id;
					break;
				}
			}
		}

		//------------------------------------------------------------------
		//MAPEXCUTORS::reverse_iterator itrb = m_mapExcutors.rbegin(); 
		//if (itrb->first != m_nMaxExcutorId)
		//{
		//	nRet = itrb->first+1;
		//}
		//else
		//{
		//	//find first idle excutor
		//	for (EXCUTORID i = m_nMinExcutorId; i <= m_nMaxExcutorId; ++i)
		//	{
		//		if (m_mapExcutors.find(i) == m_mapExcutors.end())
		//		{
		//			nRet= i;
		//		}
		//	}
		//}
	}

	return nRet;
}


EXCUTORID CExcutorMgr::CreateNewExcutor()
{
	CAutoLock lock(&m_csExcutors);

	EXCUTORID excId = GetFirstNotUsedExcutorId();
	//
	if (excId != INVALID_ID)
	{
		MAPEXCUTORS::iterator itor = m_mapExcutors.find(excId);
		if (itor == m_mapExcutors.end()) // 不存在于m_mapExcutors
		{
			ST_EXCUTORINFO excInfo;
			excInfo.excutorId = excId;
			excInfo.hmemMap = CreateExchangemapping(excId);
			m_mapExcutors[excId] = excInfo;
		}
	}
	
	return excId;
}

HRESULT CExcutorMgr::CheckExcutorLoop()
{
	while(TRUE)
	{
		if (WaitForSingleObject(m_hEventQuitLoop, 1000) != WAIT_OBJECT_0)
		{
			//check excutor state
			CheckExcutor();
		}
		else
		{
			return S_OK;
		}
	}
}

HRESULT CExcutorMgr::CheckExcutor()
{
	vector<EXCUTORID> vToRemove;
	{//for release the lock;
		CAutoLock lock(&m_csExcutors);
		MAPEXCUTORS::iterator itb = m_mapExcutors.begin();
		MAPEXCUTORS::iterator ite = m_mapExcutors.end();
		for (; itb != ite; ++itb)
		{
			ST_EXCUTORINFO& excInfo = itb->second;
			if (excInfo.tmLastcheck == 0)
			{
				//create the excutor;
				if (!LaunchExcutorFile(itb->first, excInfo.hProcessId))
				{
					ASSERT(FALSE);
					vToRemove.push_back(itb->first);
				}
				excInfo.m_eState = EXE_RUN; // 置为运行
				excInfo.tmLastcheck = time(NULL);
			}
			else
			{
				if (!::IsWindow(excInfo.hwnd))
				{
					CTimeSpan tmSpan(time(NULL) - excInfo.tmLastcheck );
					if (tmSpan.GetTotalSeconds() < nfgExcutorLaunchTimeout)
					{
						//wait a while
						continue;
					}
					else
					{
						//crashed 
						//terminate the process
						HANDLE handle = OpenProcess(PROCESS_TERMINATE, TRUE, excInfo.hProcessId);
						if (handle)
						{
							BOOL bSuc = TerminateProcess(handle, 0);
							ASSERT(bSuc);
						}
						vToRemove.push_back(itb->first);
					}
				}
				else
				{
					//safe
				}
			}

		}
	}

	if (vToRemove.size() > 0)
	{
		for (size_t i = 0; i < vToRemove.size(); ++i)
		{
			if (m_pIExcCallBack)
			{
				m_pIExcCallBack->OnExcutorExit(vToRemove[i]);
				CAutoLock lock(&m_csExcutors);
				CFWriteLog(0, TEXT("executor%d lost!"), vToRemove[i]);
				m_mapExcutors.erase(vToRemove[i]);
			}
		}
	}

	return S_OK;
}

BOOL CExcutorMgr::LaunchExcutorFile( const EXCUTORID excId , DWORD& dwProcessId)
{
	dwProcessId = NULL;
	if(excId == INVALID_ID)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if (_access(m_strExcPath, 00) == -1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	CreateDirectory(m_strExcutorWorkFolder, NULL);
	CString strDes =TEXT("\"");
	strDes += m_strExcutorWorkFolder;
	strDes += TEXT("\\");
	CTxStrConvert val;
	val.SetVal(excId);
	strDes += val.GetAsString();
	CString strExcExePath = strDes.Mid(1);
	CreateDirectory(strExcExePath, NULL);
	strExcExePath += TEXT("\\");
	strExcExePath += m_strExcName;
	if (_access(strExcExePath, 00) == -1)
	{
		//not exist
		strDes += TEXT("\\*.*\"");
		CString strCopyStr = m_strExcFolder;
		strCopyStr += TEXT("\\*.* \"");
		strCopyStr +=strDes;
		strCopyStr += TEXT("\" /S /C /Y");
// 		HINSTANCE hIns = ShellExecute(NULL, 0, TEXT("xcopy"), strCopyStr, NULL, SW_HIDE);
// 		if ((int)hIns <=32)
// 		{
// 			return FALSE;
// 		}
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );
		strCopyStr = TEXT("xcopy ") + strCopyStr;
		CFWriteLog(0, TEXT("launch excutor %s"), strCopyStr);

		// Start the child process. 
		if( !CreateProcess( NULL,   // No module name (use command line)
			strCopyStr.LockBuffer(),        // Command line
			FALSE,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi )           // Pointer to PROCESS_INFORMATION structure
			) 
		{
			ASSERT(FALSE);
			return  FALSE;
		}
		else
		{
			//close the handle
			WaitForSingleObject(pi.hProcess, INFINITE);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			dwProcessId = pi.dwProcessId;
		}
	}

	// Executor 是否在运行?
	ST_EXCUTORINFO infor;
	if (QueryExecutor(excId, infor) && ::IsWindow(infor.hwnd)) // 在运行
	{
		// 向 Executor 发送  MSG_EMBEXCUTORREGED
		::SendMessage(infor.hwnd, MSG_EMBEXCUTORREGED, 1,0);

		return TRUE;
	}
	// -----------------------------------------------------------------

	//run it
	BOOL bRet = FALSE;
	CString strRunStr;
	ST_EXCUTORREG regInfo;
	regInfo.actorId = m_nActorID;
	regInfo.guid = excId;
	regInfo.hwndActor = m_hMessageWnd;
	regInfo.hwndExcutor = NULL;
	regInfo.ToExcParamString(strRunStr);
	strRunStr.Insert(0, _T('\"'));
	strRunStr += TEXT("\"");
	if (_access(strExcExePath, 04) != -1)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof(si);
		ZeroMemory( &pi, sizeof(pi) );

		// Start the child process.
		strExcExePath += TEXT(" ");
		strRunStr = strExcExePath + strRunStr;
		CFWriteLog(0, TEXT("launch excutor %s"), strRunStr);
		if( !CreateProcess( NULL,   // No module name (use command line)
			strRunStr.LockBuffer(),        // Command line
			FALSE,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			CREATE_NEW_CONSOLE,         
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi )           // Pointer to PROCESS_INFORMATION structure
			) 
		{
			ASSERT(FALSE);
			bRet = FALSE;
		}
		else
		{
			//close the handle
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			dwProcessId = pi.dwProcessId;
			bRet = TRUE;
		}
	}
	else
	{
		ASSERT(FALSE);
	}
	
	return bRet;
}

HRESULT CExcutorMgr::SendToExcutor( const EXCUTORID excutorId,CString& szInfo )
{
	ST_EMBTRANSMSG msg(embmsgtype_ActorToExcutorMsg);
	msg.strData = szInfo;
	CEMBAutoBuffer buff(msg);
	int nUsed  = 0;
	PackMBCMsg(msg, buff, buff.GetSize(), nUsed);
	ASSERT(nUsed > 0);
	CAutoLock lock(&m_csExcutors);
	MAPEXCUTORS::iterator itf = m_mapExcutors.find(excutorId);
	if (itf != m_mapExcutors.end())
	{
		ST_EXCUTORINFO& excInfo = itf->second;
		if (excInfo.hwnd && excInfo.hmemMap != NULL)
		{
			char* pmapBuff = (char*)MapViewOfFile(excInfo.hmemMap, FILE_MAP_WRITE,0,0,nUsed);
			if (pmapBuff)
			{
				memcpy(pmapBuff, buff, nUsed);
				UnmapViewOfFile(pmapBuff);
				::SendMessage(excInfo.hwnd, MSG_EMBTOEXCUTORMSG, (WPARAM)m_hMessageWnd, (LPARAM)nUsed);
			}
			else
			{
				ASSERT(FALSE);
			}
		}
	}

	return S_OK;
}

HRESULT CExcutorMgr::WndMsgPoolCheckLoop()
{
	while(TRUE)
	{
		// 等待消息
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

			// 消息处理
			for (size_t i = 0; i < vMsgs.size(); ++i)
			{
				//process the msg
				ST_EMBWNDMSG& msg = vMsgs[i];
				OnExcutorMessage(msg);
				if (msg.pStr)
				{
					delete[] msg.pStr;
					msg.pStr = NULL;
				}
			}

		}
		else if (dwRet == WAIT_OBJECT_0+1)
		{
			//quit
			return S_OK;
		}

	}
}

HRESULT CExcutorMgr::SaveMessageToPool( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	if (message == MSG_EMBTOACTORMSG)
	{
		EXCUTORID excId = (WPARAM)wparam;
		int nLen = (LPARAM)lparam;
		
		CString strMappingName = GetExcutorMappingName(excId, m_hMessageWnd);
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
			msg.lparam = lparam;
			msg.wparam = wparam;
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
	else if (message == MSG_EMBEXCUTORREG
		||message == MSG_EMBEXCUTORQUIT)
	{
		ST_EMBWNDMSG msg;
		msg.wparam =wparam;
		msg.lparam = lparam;
		msg.wparam = wparam;
		msg.message =message;
		msg.hwnd = hwnd;
		msg.pStr= NULL;
		CAutoLock lock(&m_csMsgPool);
		m_vMsgPool.push_back(msg);
		SetEvent(m_hEventPoolMsgArrival);
	}
	
	return S_OK;
}

HANDLE CExcutorMgr::CreateExchangemapping( EXCUTORID excId )
{
	CString strMapName = GetActorMappingName(m_hMessageWnd, excId);
	HANDLE hRecvMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_COMMIT, 0, 1024*10, strMapName);
	if (hRecvMap == NULL)
	{
		ASSERT(FALSE);
	}
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		//ASSERT(FALSE);
	}
	return hRecvMap;
}

HRESULT CExcutorMgr::OnExcutorReg( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	EXCUTORID excID = (EXCUTORID)wparam;
	HWND hwndExcMsg = (HWND)lparam;
	if(hwnd == NULL || excID == INVALID_ID)
	{
		ASSERT(FALSE);
	}
	else
	{
		CFWriteLog(0, TEXT("excutor %d registed"), excID);
		CAutoLock lock(&m_csExcutors);
		MAPEXCUTORS::iterator itf = m_mapExcutors.find(excID);
		if (itf != m_mapExcutors.end())
		{
			itf->second.hwnd = hwndExcMsg;
			//itf->second.hmemMap = CreateExchangemapping(excID);
		}
		//send msg backto excutor
		::SendMessage(itf->second.hwnd, MSG_EMBEXCUTORREGED, 1,0);
	}

	return S_OK;
}


void CExcutorMgr::ClearMsgPool()
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

HRESULT CExcutorMgr::GetExecutors( vector<ST_EXCUTORINFO>& vExecutor )
{
	// clear up
	vExecutor.clear();

	CAutoLock lock(&m_csExcutors);

	// 复制到vExecutor
	for(MAPEXCUTORS::iterator itor = m_mapExcutors.begin(); itor != m_mapExcutors.end(); ++itor)
	{
		vExecutor.push_back(itor->second);
	}

	return S_OK;
}

HRESULT CExcutorMgr::SetExecutorState( EXCUTORID id, EXE_STATE eState )
{
	CAutoLock lock(&m_csExcutors);

	MAPEXCUTORS::iterator itor = m_mapExcutors.find(id);

	if (itor != m_mapExcutors.end())
	{
		itor->second.m_eState = eState;
	}

	return S_OK;
}

bool CExcutorMgr::QueryExecutor( const EXCUTORID& id, ST_EXCUTORINFO& infor )
{
	CAutoLock lock(&m_csExcutors);

	MAPEXCUTORS::iterator itor = m_mapExcutors.find(id);
	bool ok = false;

	if (itor != m_mapExcutors.end())
	{
		infor = itor->second;
		ok = true;
	}

	return ok;
}

int CExcutorMgr::GetExcResUsage()
{
	CAutoLock lock(&m_csExcutors);
	return ((int)m_mapExcutors.size()*100/(m_nMaxExcutorId - m_nMinExcutorId+1));
}
