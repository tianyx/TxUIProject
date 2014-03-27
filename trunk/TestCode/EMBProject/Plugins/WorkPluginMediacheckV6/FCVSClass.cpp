#include "StdAfx.h"
#include "FCVSClass.h"
#include "EMBCommonFunc.h"
#include "RuntimeClassFactory.h"
#include <fstream>
#include "TxLogManager.h"
#include "io.h"
LRESULT CALLBACK FCVSObjectWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
// BOOL		 RegisterFCVSObjectWnd();
// BOOL		 g_bRegisterFCVSMgrWnd = RegisterFCVSObjectWnd();
static TCHAR g_szSockWndClsName[] = TEXT ("FCVSObjectMsgWnd");
CFCVSClass* g_pFCVSObject = NULL;
// HINSTANCE GetSelfModuleHandle()
// {
// 	MEMORY_BASIC_INFORMATION mbi;
// 	
// 	return ((::VirtualQuery(GetSelfModuleHandle, &mbi, sizeof(mbi)) != 0) 
// 		? (HMODULE) mbi.AllocationBase : NULL);
// }

BOOL RegisterFCVSObjectWnd()
{
// 	HINSTANCE hInstance = GetSelfModuleHandle();
// 	
// 	WNDCLASSEX   wndclassex = {0};
// 	wndclassex.cbSize        = sizeof(WNDCLASSEX);
// 	wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
// 	wndclassex.lpfnWndProc   = FCVSObjectWndProc;
// 	wndclassex.cbClsExtra    = 0;
// 	wndclassex.cbWndExtra    = 0;
// 	wndclassex.hInstance     = hInstance;
// 	wndclassex.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
// 	wndclassex.hCursor       = LoadCursor (NULL, IDC_ARROW);
// 	wndclassex.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
// 	wndclassex.lpszMenuName  = NULL;
// 	CString newGuid = Guid2String(TxGenGuid());
// 	wndclassex.lpszClassName = newGuid;
// 	wndclassex.hIconSm       = wndclassex.hIcon;
	
// 	if ( 0 == RegisterClassEx (&wndclassex))
// 	{
// 		HRESULT hr = GetLastError();
// 		if (hr != 1410)
// 		{		
// 			ASSERT(hr);
// 		}
// 
// 	}
	
	return TRUE;
}

//CreateThreadWindow
UINT /*__stdcall*/ FCVSObjMsgLoopThread( LPVOID lparam )
{
	CFCVSClass* FCVScs = (CFCVSClass*)lparam;
	HINSTANCE hInstance = GetSelfModuleHandle();

	WNDCLASSEX   wndclassex = {0};
	wndclassex.cbSize        = sizeof(WNDCLASSEX);
	wndclassex.style         = CS_HREDRAW | CS_VREDRAW;
	wndclassex.lpfnWndProc   = FCVSObjectWndProc;
	wndclassex.cbClsExtra    = 0;
	wndclassex.cbWndExtra    = 0;
	wndclassex.hInstance     = hInstance;
	wndclassex.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
	wndclassex.hCursor       = LoadCursor (NULL, IDC_ARROW);
	wndclassex.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH);
	wndclassex.lpszMenuName  = NULL;
	CString newGuid = Guid2String(TxGenGuid());
	wndclassex.lpszClassName = newGuid;
	wndclassex.hIconSm       = wndclassex.hIcon;
	if ( 0== RegisterClassEx (&wndclassex))
	{
		HRESULT hr = GetLastError();
		ASSERT(FALSE);
		
	}
	HWND& hwnd = FCVScs->m_hwd;
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

LRESULT CALLBACK FCVSObjectWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message > MSG_EMBFCVSMSGMIN && message < MSG_EMBFCVSMSGMAX)
	{
		if (g_pFCVSObject)
		{
			// put msg into m_vMsgPool
			g_pFCVSObject->SaveMessageToPool(hwnd, message, wParam, lParam);
			
			//add adr  写这写这。

		}
		
		return 0;
	}
	
	// Window msg
	return DefWindowProc (hwnd, message, wParam, lParam);
}
HRESULT CFCVSClass::SaveMessageToPool( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	// 从 file mapping object 读取消息内容
	char* p = (char*)wparam;
	char* p1 = (char*)lparam;
	CString strMsg(p);
	CString strTskID(p1);
	delete p;
	delete p1;
	Lock();
	m_strSendMsgArr.Add(strMsg);
	UnLock();

	SetEvent(m_hEventMsgProcdureThread);
	return S_OK;

}


CFCVSClass::CFCVSClass():m_ThrdExit(FALSE,TRUE,NULL,NULL)
{
	//d_ForMsg.ShowWindow(SW_HIDE);g
	m_hwd = NULL;
	InitializeCriticalSection(&m_Lock);
	m_hEventAnaQuit = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hEventMsgProcdureThread = CreateEvent(NULL, TRUE, FALSE, NULL);
	g_pFCVSObject = this;
	GetTxLogMgr()->AddNewLogFile(1,"FCVS");
}
CFCVSClass::~CFCVSClass()
{

	m_ThrdExit.SetEvent();
	POSITION ps = 0;
	ps = m_ThreadList.GetHeadPosition();
	//PostQuitMessage(0);
	if (m_hwd!=NULL)
	{	
		PostMessage(m_hwd,WM_QUIT,NULL,NULL);
	}

	while (ps)
	{
		CWinThread* pThread = (CWinThread*)m_ThreadList.GetNext(ps);
		if (pThread)
		{
			HANDLE hTemp = pThread->m_hThread;
			if (WaitForSingleObject(hTemp, INFINITE)!= WAIT_OBJECT_0)
			{
				ASSERT(FALSE);
			}
			
			delete pThread;
		}
	
	}

    m_ThreadList.RemoveAll();
	//OnDestroy();
	g_pFCVSObject = NULL;
// 	SAFE_RELEASE(m_hEventAnaQuit);
// 	SAFE_RELEASE(m_hEventMsgProcdureThread);
	DeleteCriticalSection(&m_Lock);
	ReleaseTxLogMgr();
}
//void CFCVSClass::
HRESULT CFCVSClass::Taskstart()
{
	if(MSGget == "")
	{
		return EMBERR_EMPTY;
	}
	else
	{
		HRESULT hr = TaskInit();
		if (FAILED(hr))
		{
			return hr;
		}
		m_ThrdExit.ResetEvent();
		CWinThread *pThread = NULL;
		pThread = AfxBeginThread(MsgProcdureThread,this);
		pThread->m_bAutoDelete =FALSE;
		m_ThreadList.AddTail(pThread);
		pThread = AfxBeginThread(SendMsgProcdureThread,this,THREAD_PRIORITY_NORMAL,0,0);
		pThread->m_bAutoDelete =FALSE;
		m_ThreadList.AddTail(pThread);
		pThread = AfxBeginThread(FCVSObjMsgLoopThread,this);
		pThread->m_bAutoDelete =FALSE;
		m_ThreadList.AddTail(pThread);
		Sleep(100);//等待线程启动
		transcodeXML();
		SetEvent(m_hEventMsgProcdureThread);
		return S_OK;
	}
}
UINT CFCVSClass::MsgProcdureThread(LPVOID pParam)
{

	AfxOleInit();
	CFCVSClass* pParent = (CFCVSClass*)pParam;
	while ( !pParent->ThreadShouldExit() )
	{
		
// 		DWORD dwRet = TxWaitObjWithQuit(pParent->m_hEventMsgProcdureThread, pParent->m_hEventAnaQuit, INFINITE);
// 		if (WAIT_OBJECT_0 != dwRet)
// 		{
// 			break;
// 			
// 		}
		 if(pParent->m_strMsgArr.GetSize() > 0)
		{
			//	OutputDebugString("myOut:1 AnalysisMsg");
			//ResetEvent(m_hEventMsgProcdureThread);
			pParent->AnalysisMsg();
		 }else{
			 Sleep(100);
		 }
		 
	}
	return 1;
}

UINT CFCVSClass::SendMsgProcdureThread(LPVOID pParam)
{
	AfxOleInit();
	CFCVSClass* pParent = (CFCVSClass*)pParam;
	while ( !pParent->ThreadShouldExit() )
	{
		DWORD dwRet = TxWaitObjWithQuit(pParent->m_hEventMsgProcdureThread, pParent->m_hEventAnaQuit, INFINITE);
		if (WAIT_OBJECT_0 != dwRet)
		{
			break;
			
		}
		while(pParent->m_strSendMsgArr.GetSize() > 0)
		{
			//	OutputDebugString("myOut:1 AnalysisSendMsg");
			pParent->AnalysisSendMsg();
			
		}
	}
	return 1;
}
bool CFCVSClass::ThreadShouldExit()
{
	return WAIT_OBJECT_0 == WaitForSingleObject(m_ThrdExit, 0);
}
HRESULT CFCVSClass::ConfigInit()
{
	ofstream outtext;
	outtext.open("d:\\alsjljsljalalalalala.xml");
	outtext<<MSGget.GetBuffer(MSGget.GetLength());
	outtext.close();
	if (m_pConfig.InitConfig(MSGget))
	{
		return S_OK;
	}
	GetTxLogMgr()->WriteLog(1,"配置参数错误");
	return EMBERR_SUBTASKTEXT;	
}
HRESULT CFCVSClass::TaskInit()
{
	taskinfo.FromString(MSGget);
	CString temptex ;
	taskinfo.ToString(temptex);
	for (int i =0 ; i<GetFCVSServerConfig().m_ResultLevelArr.GetSize();i++) //选择技审级别
	{
		if (taskinfo.usedDetectLevel.CompareNoCase(GetFCVSServerConfig().m_ResultLevelArr.GetAt(i).m_strLevelName) ==0)
		{
			GetFCVSServerConfig().m_FilterLevel = GetFCVSServerConfig().m_ResultLevelArr.GetAt(i);
			if (GetFCVSServerConfig().m_bUseSectionFunction)
			{
				nSectionUsedMore = GetFCVSServerConfig().m_FilterLevel.m_nMax;
			}
			break;
		}
	}
	if ((taskinfo.filePath == "")||(taskinfo.fileName =="")||(taskinfo.fileMediaType == ""))
	{
		GetTxLogMgr()->WriteLog(1,"缺失素材基本信息，请检查filePath,FileName,FileMediaType");
		return EMBERR_SUBTASKTEXT;
	}
	return S_OK;
}
void CFCVSClass::TaskCancel()
{
	CFileDriver *pdriver = NULL;
	POSITION psFind = NULL;
	pdriver= FindTaskByID("01",psFind);
	pdriver->CancelTask();
	GetTxLogMgr()->WriteLog(1,"取消任务");
	return;	
}
BOOL CFCVSClass::AnalysisMsg()
{
	OutputDebugString("\nmyOut: AnalysisMsg into !\n");
	Lock();
	CString strMsg = m_strMsgArr.GetAt(0);
	m_strMsgArr.RemoveAt(0);
 	UnLock();
	CXMLOp XmlOp,ReXmlOp;
	CString strError = "";
	XmlOp.CleanXMLDocument();
	if(!XmlOp.LoadXMLFromString(strMsg))
	{
		OutputDebugString("\nmyOut: AnalysisMsg out 1 !\n");
		return FALSE;
	}
	CString strLog = "";
	CString strTagName((char*)XmlOp.GetFistTagName());
	if (strTagName.CompareNoCase("TaskOperation") == 0)
	{
		m_nMsgNotReturnCount++;
		CString strOpType = "";
		CString strOp = "";
		XmlOp.GetAttribute("OpType",strOpType,strError,"TaskInfo");
		XmlOp.GetAttribute("Op",strOp,strError,"TaskInfo");
		if (strOpType.CompareNoCase("TskOp") == 0)
		{
			if (strOp.CompareNoCase("Submit") == 0)
			{
				CString strFileType = "";
				XmlOp.GetAttribute("FileType",strFileType,strError,"TaskData");
				CString strClassName = GetFCVSServerConfig().GetClassNameByFileType(strFileType);
				OutputDebugString(GetFCVSServerConfig().m_strServerName);
				OutputDebugString(m_pConfig.m_strServerName);
				OutputDebugString(strClassName);
				OutputDebugString("\n**************");
				if (strClassName.IsEmpty())
				{
					ReXmlOp.CleanXMLDocument();
					ReXmlOp.CreateOperationReRoot(strError);
					MSXML2::IXMLDOMElementPtr Info = XmlOp.FindNodeByNodeName("TaskInfo");
					ReXmlOp.AppendNode(Info,strError,"Root");
					ReXmlOp.CreateNode("TaskData",strError,"Root");
					CString strTskID = "";
					XmlOp.GetAttribute("TskID",strTskID,strError,"TaskData");
					ReXmlOp.SetAttribute("TskID",strTskID,strError,"TaskData");
					CString strReturn = "";
					strReturn.Format("%d",OPRET_FILE_TYPE_NOT_SUPPORT);
					CString strErrInfo = GetOPReturnInfo(OPRET_FILE_TYPE_NOT_SUPPORT);
					ReXmlOp.SetAttribute("Result",strReturn,strError,"TaskData");
					ReXmlOp.SetAttribute("ErrInfo",strErrInfo,strError,"TaskData");
					CString strReMsg = ReXmlOp.GetXMLAsString();
					
					Lock();
					m_strSendMsgArr.Add(strReMsg);
					UnLock();

                    strLog.Format("TaskID = %s,ConText = %s(%s)",strTskID,"不支持的文件类型",strFileType);
				//	glReportLog("Log_Task",strLog);
				}
				else
				{
					Lock();
					int nTskCountTemp = 0;//wangjun 20080925
					nTskCountTemp = m_TaskList.GetCount();
					UnLock();
					if ( nTskCountTemp >= GetFCVSServerConfig().m_nTaskMaxCount)
					//if (m_TaskList.GetCount() >= GetFCVSServerConfig().m_nTaskMaxCount)
					{
						ReXmlOp.CleanXMLDocument();
						ReXmlOp.CreateOperationReRoot(strError);
						MSXML2::IXMLDOMElementPtr Info = XmlOp.FindNodeByNodeName("TaskInfo");
						ReXmlOp.AppendNode(Info,strError,"Root");
						ReXmlOp.CreateNode("TaskData",strError,"Root");
						CString strTskID = "";
						XmlOp.GetAttribute("TskID",strTskID,strError,"TaskData");
						ReXmlOp.SetAttribute("TskID",strTskID,strError,"TaskData");
						CString strReturn = "";
						strReturn.Format("%d",OPRET_TSK_IS_FULL);
						CString strErrInfo = GetOPReturnInfo(OPRET_TSK_IS_FULL);
						ReXmlOp.SetAttribute("Result",strReturn,strError,"TaskData");
						ReXmlOp.SetAttribute("ErrInfo",strErrInfo,strError,"TaskData");
						CString strReMsg = ReXmlOp.GetXMLAsString();

						Lock();
						m_strSendMsgArr.Add(strReMsg);
						UnLock();

						strLog.Format("TaskID = %s,ConText = %s(%d)",strTskID,"不能超过最大可执行数",nTskCountTemp);
					 //   glReportLog("Log_Task",strLog);
					}
					else
					{
						BuildTask(strClassName,strMsg);
					}
				}
			}
			//通过TaskID查询当前任务执行状态
			else if (strOp.CompareNoCase("AskRunTask") == 0)
			{
				CString strTskID = "";
				XmlOp.GetAttribute("TskID",strTskID,strError,"TaskData");
			//	IsTaskOperating(strTskID,strMsg);
			}
			else
			{
				CString strTskID = "";
				XmlOp.GetAttribute("TskID",strTskID,strError,"TaskData");
				CString strOp = "";
				XmlOp.GetAttribute("Op",strOp,strError,"TaskInfo");
				strOp.TrimLeft();
	            strOp.TrimRight();

				POSITION psFind = NULL;
				CFileDriver* pDriver = 0;
				
				Lock();
				pDriver = FindTaskByID(strTskID,psFind);
				if (pDriver)
				{
					OperateTask(pDriver,strMsg);
					UnLock();

					strLog.Format("TaskID = %s,ConText = %s(%s)",strTskID,"操作任务",strOp);
		         //   glReportLog("Log_Task",strLog);
				}
				else
				{
					UnLock();

					ReXmlOp.CleanXMLDocument();
					ReXmlOp.CreateOperationReRoot(strError);
					MSXML2::IXMLDOMElementPtr Info = XmlOp.FindNodeByNodeName("TaskInfo");
					ReXmlOp.AppendNode(Info,strError,"Root");
					ReXmlOp.CreateNode("TaskData",strError,"Root");
					CString strTskID = "";
					XmlOp.GetAttribute("TskID",strTskID,strError,"TaskData");
					ReXmlOp.SetAttribute("TskID",strTskID,strError,"TaskData");
					CString strReturn = "";
					strReturn.Format("%d",OPRET_TSK_NOT_FOUND);
					CString strErrInfo = GetOPReturnInfo(OPRET_TSK_NOT_FOUND);
					ReXmlOp.SetAttribute("Result",strReturn,strError,"TaskData");
					ReXmlOp.SetAttribute("ErrInfo",strErrInfo,strError,"TaskData");
					CString strReMsg = ReXmlOp.GetXMLAsString();
					Lock();
					m_strSendMsgArr.Add(strReMsg);
					UnLock();

					strLog.Format("TaskID = %s,ConText = %s(%s)",strTskID,"操作失败，没有找到任务",strOp);
			//		glReportLog("Log_Task",strLog);
				}
			}
		}
	}
	else if (strTagName.CompareNoCase("ServiceOperation") == 0)
	{
		m_nMsgNotReturnCount++;
		//OnServiceOperation(strMsg);
	}
	else if (strTagName.CompareNoCase("DetectOperation") == 0)
	{
		m_nMsgNotReturnCount++;
		//OnDetectOperation(strMsg);
	}
	OutputDebugString("\nmyOut: AnalysisMsg out end !\n");
	return TRUE;
}
BOOL CFCVSClass::AnalysisSendMsg()
{
	OutputDebugString("myOut:1 AnalysisSendMsg In");
	Lock();
	//
	CString strMsg = m_strSendMsgArr.GetAt(0);
	m_strSendMsgArr.RemoveAt(0);
	int nCount = m_strSendMsgArr.GetSize();
 	UnLock();

	if (strMsg.IsEmpty())
	{
		OutputDebugString("myOut:1 AnalysisSendMsg Out Msg Is Empty");
		return FALSE;
	}
	CXMLOp ReXmlOp;
	CString strErr = "";
	ReXmlOp.CleanXMLDocument();
	ReXmlOp.LoadXMLFromString(strMsg);
	CString strFirstTagName((char*)ReXmlOp.GetFistTagName());
	if (strFirstTagName.CompareNoCase("TaskOperation_Return") == 0)
	{
		CString strTskID = "";
		strTskID = ReXmlOp.GetTskID();
	//	GetNetMsg()->SendNetMessageByTskID(strMsg,strTskID);
		m_nMsgNotReturnCount--;
		//chenAdd New 
		//==========================================================
		CString strOp = "";
		ReXmlOp.GetAttribute("Op",strOp,strErr,"TaskInfo");
		return TRUE;
	}
	if (strFirstTagName.CompareNoCase("TaskBroadCast") == 0)
	{
		CString strTskID = "";
		strTskID = ReXmlOp.GetTskID();
		CString strBDType = "";
		ReXmlOp.GetAttribute("BDType",strBDType,strErr,"Info");
// 		CString strLog;
// 		strLog.Format("myOut:1 TaskBroadCast Count = %d Msg = %s",nCount,strMsg);
// 		OutputDebugString(strLog);
		if (strBDType.CompareNoCase("FileInfo") == 0 || strBDType.CompareNoCase("DetectResult") == 0)
		{
		//	GetNetMsg()->SendNetMessage(GetFCVSServerConfig().m_strBroadCastServiceName,strMsg);
			return TRUE;
		}
	//	GetNetMsg()->SendNetMessageByTskID(strMsg,strTskID);
		if (strBDType.CompareNoCase("TskStatus") == 0)
		{
			CString strOut = "%JS%:" + strMsg;
			OutputDebugString(strOut);
			CString strStatus = "";
			ReXmlOp.GetAttribute("Status",strStatus,strErr,"Data");
			int nStatus = -1;
			nStatus = atoi(strStatus);
		//	SetTskListStatus(strTskID,nStatus);
			//wangjun 2008.09.20
			//当任务正常结束、取消、发生错误的状态下，释放任务
			//	if (nStatus == TSK_IS_CANNELED || nStatus == TSK_IS_FINISHED )
			if (nStatus == TSK_IS_CANNELED || nStatus == TSK_IS_FINISHED || nStatus == TSK_IS_ABORTED)
			{
				Sleep(2000);//wangjun 20081031
			//	RemoveTskFromList(strTskID);
			//	GetNetMsg()->UnRegisterTskID(strTskID);
				//有时当任务发生错误时，任务还没有添加到m_TaskList
				ReleaseTskByTskID(strTskID);
				Sleep(500);//wangjun 20081031
				m_reportST.nPercent = 100;
				GetTxLogMgr()->WriteLog(1,"任务结束");
				if (nStatus == TSK_IS_CANNELED /*|| nStatus == TSK_IS_ABORTED*/)
				{
					m_reportST.nPercent = -1;
					m_reportST.code = EMBERR_FCVSERROR;
					GetTxLogMgr()->WriteLog(1,"任务错误");
				}
			//	RemoveTskFromList(strTskID);
			}
		//	SetTaskBarIcon1(nStatus);
		}
		if (strBDType.CompareNoCase("Progress") == 0)
		{
			CString strPrecent = "";
			ReXmlOp.GetAttribute("Precent",strPrecent,strErr,"Data");
			int tempReport = 0;
			sscanf(strPrecent.GetBuffer(strPrecent.GetLength()),"%d",&tempReport);	
			m_reportST.code = S_OK;
			m_reportST.nPercent = tempReport;
			if (	m_reportST.nPercent  ==100)
			{
				m_reportST.nPercent = 99;
			}
		//	reportPencent(FALSE);
// 			CString reportS ="";
// 			reportST.ToString(reportS);
//			m_reportCallBack->OnDllReportTaskProgress(reportS);
			// report写这写这
		//	SetTskListProgress(strTskID,strPrecent);
		}
		OutputDebugString("myOut:1 AnalysisSendMsg Out TaskBroadCast");
		return TRUE;
	}
	if (strFirstTagName.CompareNoCase("ServiceOperation_Return") == 0)
	{
		CString strMsgID = ReXmlOp.GetMsgID();
	//	GetNetMsg()->SendNetMessageByMsgID(strMsg,strMsgID);
		m_nMsgNotReturnCount--;
    //	GetNetMsg()->UnRegisterMsgID(strMsgID);
		OutputDebugString("myOut:1 AnalysisSendMsg Out ServiceOperation_Return");
		return TRUE;
	}
	if (strFirstTagName.CompareNoCase("AnlysisResult") == 0)
	{
		CString strTskID = "";
		strTskID = ReXmlOp.GetTskID();
	//	GetNetMsg()->SendNetMessageByTskID(strMsg,strTskID);
		OutputDebugString("myOut:1 AnalysisSendMsg Out AnlysisResult");
		return TRUE;
	}
	if(strFirstTagName.CompareNoCase("DetectOperation_Return") == 0)
	{
		CString strMsgID = ReXmlOp.GetMsgID();
		int nLength = strMsg.GetLength();
	//	GetNetMsg()->SendNetMessageByMsgID(strMsg,strMsgID);
		m_nMsgNotReturnCount--;
	//	GetNetMsg()->UnRegisterMsgID(strMsgID);
		OutputDebugString("myOut:1 AnalysisSendMsg Out DetectOperation_Return");
		return TRUE;
	}
	OutputDebugString("myOut:1 AnalysisSendMsg Out Nothing");
	return FALSE;
}
BOOL CFCVSClass::OperateTask(CFileDriver* pDriver,CString strMsg)
{
	CString strError = "";
	emuOPRETURNCODE nRetCode;
	CXMLOp XmlOp;
	XmlOp.CleanXMLDocument();
	XmlOp.LoadXMLFromString(strMsg);
	CString strOp = "";
	XmlOp.GetAttribute("Op",strOp,strError,"TaskInfo");
	strOp.TrimLeft();
	strOp.TrimRight();
	if (strOp.CompareNoCase("Pause") == 0)
	{
		if (pDriver->PauseTask())
		{
			nRetCode = OPRET_TSK_PAUSE_SUCCESS;
		}
		else
		{
			nRetCode = OPRET_TSK_PAUSE_FAILE;
		}
	}
	else if (strOp.CompareNoCase("Run") == 0)
	{
		if (pDriver->RunTask())
		{
			nRetCode = OPRET_TSK_RUN_SUCCESS;
		}
		else
		{
			nRetCode = OPRET_TSK_RUN_FAILE;
		}
	}
	else if (strOp.CompareNoCase("Cancel") == 0)
	{
		if (pDriver->CancelTask())
		{
			nRetCode = OPRET_TSK_CANCEL_SUCCESS;
		}
		else
		{
			nRetCode = OPRET_TSK_CANCEL_FAILE;
		}
	}
	CString strRetCode = "";
	strRetCode.Format("%d",nRetCode);
	
	CXMLOp ReXmlOp;
	ReXmlOp.CleanXMLDocument();
	ReXmlOp.CreateOperationReRoot(strError);
	MSXML2::IXMLDOMElementPtr Info = XmlOp.FindNodeByNodeName("TaskInfo");
	ReXmlOp.AppendNode(Info,strError,"Root");
	ReXmlOp.CreateNode("TaskData",strError,"Root");
	CString strTskID = "";
	XmlOp.GetAttribute("TskID",strTskID,strError,"TaskData");
	ReXmlOp.SetAttribute("TskID",strTskID,strError,"TaskData");
	CString strErrInfo = GetOPReturnInfo(nRetCode);
	ReXmlOp.SetAttribute("Result",strRetCode,strError,"TaskData");
	ReXmlOp.SetAttribute("ErrInfo",strErrInfo,strError,"TaskData");
	
	CString strReMsg = ReXmlOp.GetXMLAsString(); 
	Lock();
	m_strSendMsgArr.Add(strReMsg);
	UnLock();
	return TRUE;
}
BOOL CFCVSClass ::transcodeXML()
{
	//taskinfo.
	CString strErr;
	CXMLOp XmlOp;
	XmlOp.CleanXMLDocument();
	XmlOp.CreateOperationRoot(strErr);
	XmlOp.CreateNode("TaskInfo",strErr,"Root");
	CString strMsgID = TEXT("888888");
	//strMsgID = GenerateGUID();
	XmlOp.SetAttribute("MsgID",strMsgID,strErr,"TaskInfo");
	XmlOp.SetAttribute("OpType","TskOp",strErr,"TaskInfo");
	XmlOp.SetAttribute("Op","Submit",strErr,"TaskInfo");
	XmlOp.CreateNode("TaskData",strErr,"Root");
	XmlOp.SetAttribute("CheckKind","File",strErr,"TaskData");
	XmlOp.SetAttribute("CheckOpt","1",strErr,"TaskData");
	XmlOp.SetAttribute("TskID",taskinfo.fileName,strErr,"TaskData");
	XmlOp.SetAttribute("ClipID","888",strErr,"TaskData");
	CString m_strMsg ;
	m_strMsg = taskinfo.filePath;
	m_strMsg.TrimRight();
	if (!m_strMsg.IsEmpty() && m_strMsg[m_strMsg.GetLength()-1] != '\\')
	{
		m_strMsg+= TEXT("\\");
	}
	m_strMsg+= taskinfo.fileName;
	CString h_log;
	h_log.Format("素材路径%s",m_strMsg);
	GetTxLogMgr()->WriteLog(1,h_log);
	if(access(m_strMsg.GetBuffer(m_strMsg.GetLength()),0)==-1)
	{
		GetTxLogMgr()->WriteLog(1,"文件不存在");
		return FALSE;
	}
	XmlOp.SetAttribute("SaveXmlPath", taskinfo.strSaveXmlPath, strErr, "TaskData"); // 技审结果xml保存路径
	XmlOp.SetAttribute("SrcDir",m_strMsg,strErr,"TaskData");
	XmlOp.SetAttribute("CheckItem","1111",strErr,"TaskData");
	XmlOp.SetAttribute("PRI","70",strErr,"TaskData");
	XmlOp.SetAttribute("FileType",taskinfo.fileMediaType,strErr,"TaskData");
	XmlOp.SetAttribute("SubFileType","SD",strErr,"TaskData");
	XmlOp.SetAttribute("ClipType",taskinfo.clipType,strErr,"TaskData");
	//OutputDebugString(taskinfo.clipType);
	CString sSectionID;
	CString sTotalSectionCount;
	sSectionID.Format("%d",taskinfo.nSectionID);
	sTotalSectionCount.Format("%d",taskinfo.nTotalSectionCount);
	XmlOp.SetAttribute("nSectionID",sSectionID,strErr,"TaskData");
	XmlOp.SetAttribute("nTotalSectionCount",sTotalSectionCount,strErr,"TaskData");
	CString strTaskXml = XmlOp.GetXMLAsString();
	
	Lock();
	m_strMsgArr.Add(strTaskXml);
 	UnLock();
	return TRUE;
}
BOOL CFCVSClass::BuildTask(CString strClassName,CString strMsg)
{
	//wangjun 20081018
	CString strLog = "";
	CFileDriver* pDriver = (CFileDriver*)GetRunTimeClassFactory()->CreateByName(strClassName);
	if(pDriver)
	{
		
		//原则：首先把任务加入任务链表中，如果发生任何错误，那么通过状态通知来取消任务
		pDriver->SetParentWnd(m_hwd);
		if(!pDriver->InitTask(strMsg))//wangjun 20081007 Modify
		{
			delete pDriver;
			pDriver = NULL;
			
			strLog.Format("TaskID = %s,ConText = %s(%s)","","初始化任务",strMsg);
//			glReportLog("Log_Task",strLog);
			return FALSE;
		}
		else
		{ 
			// 先加入到任务列表
			// 后显示到界面
			/*入点解析*/
			CString strErr = "";
			CXMLOp XmlOp;
			XmlOp.CleanXMLDocument();
			if(!XmlOp.LoadXMLFromString(strMsg))
			{
				OutputDebugString("\nmyOut: AnalysisMsg out 1 !\n");
				return FALSE;
			}
			CString strTagName((char*)XmlOp.GetFistTagName());
			CString strTaskSOM = "";
			CString strTaskDuration = ""; 
			CString strSectionID = "";
			CString strTotalSectionCount = "";
			
			
			/*入点解析END*/
			Lock();
			m_TaskList.AddTail(pDriver);
			UnLock();
			
			//AddTskToList(pDriver);
			
			Sleep(500);//万一素材太小，休息一下，以防任务管理程序反应不过来:)
			pDriver->SetRetryInfo(GetFCVSServerConfig().m_dwRetryTimes, GetFCVSServerConfig().m_dwRetryInterval); // wang.zh add
			
			strLog.Format("TaskID = %s, ConText = %s(%s)", "", ">>提交任务开始", strMsg);
		//	glReportLog("Log_Task", strLog);
			if (strTagName.CompareNoCase("TaskOperation") == 0)
			{
				if (GetFCVSServerConfig().m_bUseSectionFunction)
				{
					XmlOp.GetAttribute("nSectionID",strSectionID,strErr,"TaskData");
					XmlOp.GetAttribute("nTotalSectionCount",strTotalSectionCount,strErr,"TaskData");
					pDriver->m_TaskSectionID = strSectionID;
				}
			}

			pDriver->Submit();
			if (GetFCVSServerConfig().m_bUseSectionFunction)
			{
				GetTxLogMgr()->WriteLog(1,"切片任务");
				pDriver->PauseTask();//切片功能，必须把任务先停住
			}
			
			Sleep(500);
			if (strTagName.CompareNoCase("TaskOperation") == 0)
			{
				if (GetFCVSServerConfig().m_bUseSectionFunction)
				{
					XmlOp.GetAttribute("nSectionID",strSectionID,strErr,"TaskData");
					XmlOp.GetAttribute("nTotalSectionCount",strTotalSectionCount,strErr,"TaskData");
					int tempSecID,tempTotalSecCnt;
					tempSecID = _ttoi(strSectionID);
					tempTotalSecCnt = _ttoi(strTotalSectionCount);					
					if (tempSecID ==0 || tempTotalSecCnt<=1)
					{
						OutputDebugString("\n切片ID不正确");
						return FALSE;
					}
					LONGLONG tempTaskDuration  = pDriver->GetFileDuration();
					

					LONGLONG tEOM,tSOM,tDuration;
					tSOM = tempTaskDuration *(tempSecID-1) / tempTotalSecCnt;
					if (tempSecID != tempTotalSecCnt )
					{
						tEOM = tempTaskDuration *tempSecID/tempTotalSecCnt  + nSectionUsedMore;
					}else
					{
						tEOM = tempTaskDuration;
					}
					tDuration = tEOM - tSOM;
					pDriver->m_TaskSOM = tSOM;
					if(GetFCVSServerConfig().m_bUseSectionFunction)
					{
						pDriver->m_DetectResult.m_som = tSOM;
					}else
					{
						pDriver->m_DetectResult.m_som = 0;
					}
					pDriver->m_TaskDuration = tDuration ;				
					pDriver->m_TaskSectionID = strSectionID;
					pDriver->m_totalSectionCount = tempTotalSecCnt;
				}
				
			}
			LONGLONG SOM  = pDriver->m_TaskSOM;
			LONGLONG Duration = pDriver->m_TaskDuration;
			if ((Duration != 0)&&(GetFCVSServerConfig().m_bUseSectionFunction))
			{ 
				pDriver->SeekTo(SOM,Duration);
				
				 if(!pDriver->RunTask())
				 {
					OutputDebugString("seekError————————————————————");	
				 }
			}
			m_reportST.nPercent = 1;//开始任务即置1
			strLog.Format("TaskID = %s, ConText = %s(%s)", "", ">>提交任务结束", strMsg);
		//	glReportLog("Log_Task", strLog);
		}
		
		return TRUE;
	}
	return FALSE;
}
CFileDriver* CFCVSClass::FindTaskByID(CString strTskID,POSITION& FindPs)
{
	Lock();
	
	CFileDriver* pDriver = NULL;
	strTskID.TrimLeft();
	strTskID.TrimRight();
	POSITION ps = m_TaskList.GetHeadPosition();
	while (ps)
	{
		FindPs = ps;
		pDriver = m_TaskList.GetNext(ps);
		if (pDriver)//wangjun 20090338
		{
			if (strTskID.CompareNoCase(pDriver->GetTskID()) == 0)
			{
				break;
			}
			else//wangjun 20081006
			{
				FindPs = NULL;
				pDriver = NULL;
			}
		}
	}
	
	UnLock();
	return pDriver;
}
BOOL CFCVSClass::ReleaseTskByTskID(CString strTskID /* =  */)
{
	Lock();
	strTskID.TrimLeft();
	strTskID.TrimRight();
	if (strTskID.IsEmpty())
	{
		POSITION ps = m_TaskList.GetHeadPosition();
		while (ps)
		{
			CFileDriver* pDreiver = (CFileDriver*)m_TaskList.GetNext(ps);
			if (pDreiver)//wangjun 20080922
			{
				delete pDreiver;
			}
			
		}
		m_TaskList.RemoveAll();
	}
	else
	{
		POSITION ps = NULL;
		CFileDriver* pDriver = FindTaskByID(strTskID,ps);
		if (pDriver)//wangjun 20080922
		{
			delete pDriver;
		}
		if (ps != NULL)//wangjun 20080922
		{
			m_TaskList.RemoveAt(ps);
		}
	}
	UnLock();
	return TRUE;
}
CString CFCVSClass::reportPencent(BOOL isAskPercent)
{
	CString reportS;
	m_reportST.ToString(reportS);
	if(!isAskPercent)
		m_reportCallBack->OnDllReportTaskProgress(reportS);
	return reportS;
}
