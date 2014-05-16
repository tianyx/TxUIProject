#include "StdAfx.h"
#include "MCResultMerge.h"
#include "IEMBBaseInterface.h"
#include "TxLogManager.h"
#include "EMBDefine.h"
#include "EmbStructDef.h"
#include "TxParamString.h"
#include <fstream>
#include "TxADOConn.h"
#include "EMBCommonFunc.h"
//////////////////////////////////////////////////////////////////////////
using namespace EMB;

DWORD __stdcall TaskRunProc(LPVOID lparam)
{
	CMCResultMerge* pObj = (CMCResultMerge*)lparam;
	if (pObj)
	{
		pObj->RunTaskLoop();
	}

	return 0;
}

CMCResultMerge::CMCResultMerge(void)
{
	m_hThreadTask = NULL;
	m_pReportCallback = NULL;
	m_nPercent = 0;
	m_pThread = NULL;
	isTaskRun =TRUE;
	m_hErrorCode = S_OK;
	m_hEventQuitTask = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CMCResultMerge::~CMCResultMerge(void)
{
}

HRESULT EMB::CMCResultMerge::QueryPluginInfo( VECPLUGINFOS& vInfoOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_PWorkerMCResultMerge;
	info.nPlugInType = PluginType_Wroker;
	info.nSubType = SubType_MCResultMerge;
	vInfoOut.push_back(info);
	return S_OK;
}

HRESULT EMB::CMCResultMerge::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
{
	pInterfaceOut = NULL;
	if (guidIn == GuidEMBPlugin_IBase)
	{
		pInterfaceOut = dynamic_cast<IPluginBaseInterface*>(this);
		AddRef();
		return S_OK;
	}
	else if (guidIn == GuidEMBPlugin_ITaskWorkerCall)
	{
		pInterfaceOut = dynamic_cast<ITaskWorkerCallInterface*>(this);
		AddRef();
		return S_OK;

	}
	else if (guidIn == GuidEMBPlugin_ITaskWorkerOnMessage)
	{
		pInterfaceOut = dynamic_cast<ITaskWorkerOnMessageInterface*>(this);
		AddRef();
		return S_OK;

	}
	else
	{
		return __super::QueryInterface(guidIn, pInterfaceOut);
	}

}



HRESULT EMB::CMCResultMerge::OnFirstInit()
{
	GetTxLogMgr()->AddNewLogFile(LOGKEY_MD5, TEXT("MCResultMerge"));

	return S_OK;

}

void EMB::CMCResultMerge::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	ReleaseTxLogMgr();
	TRACE("\nCMCResultMerge::OnFinalRelease() ");

	delete this;
}

HRESULT EMB::CMCResultMerge::DoTask( const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback )
{
	if (m_hThreadTask)
	{
		ASSERT(FALSE);
		return EMBERR_WORKERBUSY;
	}

	ResetEvent(m_hEventQuitTask);

	//to do... and check code
//	ASSERT(FALSE);

	m_strTask = szTaskIn;
	m_structTask.FromString(m_strTask);
	m_pReportCallback = pICallback;
	m_hErrorCode = S_OK;
	//time to get runtime taskinfo
	if (m_structTask.bRuntimeUpdate)
	{
		CString strRecall;
		ST_WORKERRECALL recalInfo;
		recalInfo.nReCallType = embrecalltype_RequestRuntimeInfo;
		recalInfo.nRuntimeType = embruntimeInfoType_fcvsMergeInfo;
		recalInfo.ToString(strRecall);
		CEMBWorkString strRecallRet;
		m_pReportCallback->OnDllRuntimeCall(strRecall, strRecallRet);
		CString strNewMergeInfo = strRecallRet;
		if (!strNewMergeInfo.IsEmpty())
		{
			ST_FCVSRUNTIMEMERGEINFO mgrNewInfo;
			mgrNewInfo.FromString(strNewMergeInfo);
			if (mgrNewInfo.nTotalSplit > 0 && !mgrNewInfo.strPath.IsEmpty() &&
				!mgrNewInfo.strFileName.IsEmpty() )
			{

				CString strFilePath = mgrNewInfo.strPath;
				strFilePath.TrimRight(TEXT("\\"));
				strFilePath += TEXT("\\");
				strFilePath += mgrNewInfo.strFileName;
				m_structTask.filePath = strFilePath;
				m_structTask.nTotalSectionCount = mgrNewInfo.nTotalSplit;
				CFWriteLog(0, TEXT("runtime mergeinfo changed, filePath = %s, splitcount = %d"), strFilePath, mgrNewInfo.nTotalSplit);

				if (mgrNewInfo.nTotalSplit == m_structTask.nTotalSectionCount
					&& mgrNewInfo.vSubIds.size() == mgrNewInfo.nTotalSplit)
				{
					CFWriteLog(TEXT("receive runtime subtaskList:%s"), strNewMergeInfo);
					for (int i = 0; i < mgrNewInfo.nTotalSplit; ++i)
					{
						ST_TASKRUNSTATE state;
						state.guid = String2Guid(mgrNewInfo.vSubIds[i]);
						m_vTaskRunState.push_back(state);
					}
				}
			}
		}

		//change db info
		if (m_structTask.bRegisterToDB)
		{
			recalInfo.nRuntimeType = embruntimeInfoType_filedest;
			recalInfo.ToString(strRecall);
			CEMBWorkString strRecallRet;
			m_pReportCallback->OnDllRuntimeCall(strRecall, strRecallRet);
			ST_TRANSRESULT transNew;
			CString strTranRet =strRecallRet;
			transNew.FromString(strTranRet);
			if (!transNew.strDestDBConn.IsEmpty())
			{
				CFWriteLog(0, TEXT("runtime dbinfo changed, dbconn = %s"), transNew.strDestDBConn);
				m_structTask.strDBConn = transNew.strDestDBConn;
			}
		}

		/*//get runtime sub task id;
		recalInfo.nRuntimeType = embruntimeInfoType_fcvsMergeInfo;
		recalInfo.ToString(strRecall);
		CEMBWorkString strRecallRet3;
		m_pReportCallback->OnDllRuntimeCall(strRecall, strRecallRet3);
		CString strMgrRet = strRecallRet3;
		ST_FCVSRUNTIMEMERGEINFO rtMergeInfo;
		rtMergeInfo.FromString(strMgrRet);
		if (rtMergeInfo.nTotalSplit == m_structTask.nTotalSectionCount
			&& rtMergeInfo.vSubIds.size() == rtMergeInfo.nTotalSplit)
		{
			CFWriteLog(TEXT("receive runtime subtaskList:%s"), strMgrRet);
			for (int i = 0; i < rtMergeInfo.nTotalSplit; ++i)
			{
				ST_TASKRUNSTATE state;
				state.guid = String2Guid(rtMergeInfo.vSubIds[i]);
				m_vTaskRunState.push_back(state);
			}
		}*/
				
	}
	

	if (m_structTask.filePath.IsEmpty() || m_structTask.nTotalSectionCount < 1
		||(m_structTask.bRegisterToDB && m_structTask.strDBConn.IsEmpty()))
	{
		OutputDebugString("任务初始化失败");
		return EMBERR_INVALIDARG;
	}

	AnalysisFilePath();
	m_hThreadTask = CreateThread(NULL, 0, TaskRunProc, (LPVOID)this, 0, 0);
	return S_OK;
}

HRESULT EMB::CMCResultMerge::CancelTask()
{
	isTaskRun = FALSE;
	if (m_hThreadTask)
	{
		SetEvent(m_hEventQuitTask);
		WaitForSingleObject(m_hThreadTask, INFINITE);
		m_hThreadTask = NULL;
	}

	return S_OK;
}

HRESULT EMB::CMCResultMerge::GetTaskProgress( CEMBWorkString& szInfo )
{
	//get it
	//ASSERT(FALSE);
	m_nPercent =1;

	ST_WORKERREPORT report;
	report.nPercent = m_nPercent;
	report.code = m_hErrorCode;
	CString strRet;
	report.ToString(strRet);
	szInfo = strRet;
	return S_OK;
}

BOOL EMB::CMCResultMerge::RunTaskLoop()
{
	//to do...
	//ASSERT(FALSE);
	//CWinThread *pThread = NULL;
	//m_pThread = AfxBeginThread(FindEnoughthread,this);
	WaitForFinish();
	return TRUE;
}
void EMB::CMCResultMerge::AnalysisFilePath()
{
	TCHAR drive[MAX_PATH],dir[MAX_PATH],fname[MAX_PATH],ext[MAX_PATH];
	_tsplitpath(m_structTask.filePath.GetBuffer(0),drive,dir,fname,ext);

	if (m_structTask.nTotalSectionCount == 1)
	{
		CString strTempFile;
		strTempFile.Format("%s%s%s%s",drive,dir,fname,"_Detect.xml");
		m_csLockSecXmlArr.Lock();
		ST_SUBTASKCHECKFILE tmpchk;
		tmpchk.strFile = strTempFile;
		m_vSubFiles.push_back(tmpchk);
		m_csLockSecXmlArr.Unlock();
		CFWriteLog(TEXT("merge check file: %s"), strTempFile);

	}
	else
	{
		// index begin of 1
		for (int i = 1;i <= m_structTask.nTotalSectionCount;i++)
		{
			CString strTempFile;
			strTempFile.Format("%s%s%s_%d%s",drive,dir,fname,i,"_Detect.xml");
			m_csLockSecXmlArr.Lock();
			ST_SUBTASKCHECKFILE tmpchk;
			tmpchk.strFile = strTempFile;
			m_vSubFiles.push_back(tmpchk);
			m_csLockSecXmlArr.Unlock();
			CFWriteLog(TEXT("merge check file%d: %s"),i, strTempFile);

		}
	}
	
}
HRESULT EMB::CMCResultMerge::WaitForFinish()  //寻找文件线程
{
	const int nCheckMaxTimeOut = 60; //second
	const int nCheckInterval =5;	//second
	int nCheckPoint = 0;
	
	HRESULT hr = S_OK;
	while(WaitForSingleObject(m_hEventQuitTask,  1000) != WAIT_OBJECT_0)
	{
		nCheckPoint= (++nCheckPoint)%nCheckInterval;
		hr = S_OK;
		int nReadyFileCount = 0;
		for (int i = 0; i < m_vSubFiles.size(); i++)
		{
			if (m_vSubFiles[i].bReady)
			{
				nReadyFileCount++;
				continue;
			}
			CString strFirstPath = m_vSubFiles[i].strFile;
			if (_access(strFirstPath, 04) != -1)
			{
				nReadyFileCount++;
				m_vSubFiles[i].bReady = TRUE;
				if (m_structTask.nTotalSectionCount == 1)
				{
					//only one
				}
				else
				{
					OutputDebugString(strFirstPath);
					m_xmlDoc.LoadXMLFromFile(strFirstPath);
					structResult temp;
					temp.strFileParh = strFirstPath;
					m_xmlDoc.GetResultInfos(temp);
					vecResults.push_back(temp);
					m_nCntDetectExtend = 250;
					::DeleteFile(temp.strFileParh);
					stResult =temp;
				}
			}
			else
			{
				if (i == 0)
				{
					CFWriteLog(0, TEXT("xml for subtask 1 not found, merge failed!"));
					hr = EMBERR_FILEACCESS;
					break;
				}

				CAutoLock lock(&m_csRunState);
				ST_TASKRUNSTATE& stateRef = m_vTaskRunState[i];
				CTime tmNow(time(NULL));
				CTimeSpan tReportSpan =tmNow - CTime(stateRef.tmLastReport);
				BOOL bNeedCheck = FALSE;
				if (stateRef.tmLastReport == 0)
				{
					if (stateRef.tmLastCheck == 0|| tReportSpan.GetTotalSeconds() < nCheckMaxTimeOut)
					{
						bNeedCheck = TRUE;
						//send request
					}
					else
					{	
						CFWriteLog(0, TEXT("subtask request runstate timeout! id= %s"), Guid2String(stateRef.guid));
						hr = EMBERR_TIMEOUT;
					}
				}
				else
				{
					//
					if (stateRef.nState == embtaskstate_finished)
					{
						//file not found!
						CFWriteLog(0, TEXT("subtask report finished, but required file not found! id= %s"), Guid2String(stateRef.guid));
						hr = EMBERR_FILEACCESS;
					}
					else
					{
						//c
						if (tReportSpan.GetTotalSeconds() < nCheckMaxTimeOut)
						{
							bNeedCheck = TRUE;
						}
						else
						{
							//request timeout
							CFWriteLog(0, TEXT("request subtask runinfo timeout, id= %s"), Guid2String(stateRef.guid));
							hr = EMBERR_TIMEOUT;
						}
					}
				}

				if (FAILED(hr))
				{
					if (m_pReportCallback)
					{
						CString strRst;
						ST_WORKERREPORT repot;
						repot.code = hr;
						repot.nPercent = -1;
						repot.ToString(strRst);
						m_pReportCallback->OnDllReportTaskProgress(strRst);
					}
					break;
				}

				if (bNeedCheck && nCheckPoint == 0)
				{
					if (m_pReportCallback)
					{
						CString strRecall;
						ST_WORKERRECALL recalInfo;
						recalInfo.nReCallType = embrecalltype_RequestTaskRunInfo;
						recalInfo.nRuntimeType = embruntimeInfoType_taskrunstate;
						ST_TASKRUNSTATE stateTmp;
						stateTmp.guid = stateRef.guid;
						stateTmp.ToString(recalInfo.strRuntimeInfo, TRUE);

						recalInfo.ToString(strRecall);
						CEMBWorkString strRecallRet;
						m_pReportCallback->OnDllRuntimeCall(strRecall, strRecallRet);	
						stateRef.tmLastCheck = time(NULL);
					}
					else
					{
						//
						ASSERT(FALSE);

					}

				}
			}

			
		}

		if (FAILED(hr))
		{
			//quit while loop
			m_hErrorCode = hr;
			break;
		}
				
		if (nReadyFileCount>= m_structTask.nTotalSectionCount)
		{
			if (m_structTask.nTotalSectionCount > 1)
			{
				ManageResult();
			}

			if(m_structTask.bRegisterToDB)
			{

				hr = RegisterToDB();
			}

			m_nPercent = 100;

			if (m_pReportCallback)
			{
				CString strRst;
				ST_WORKERREPORT repot;
				repot.code = hr;
				repot.nPercent = 100;
				repot.ToString(strRst);
				m_pReportCallback->OnDllReportTaskProgress(strRst);
			}
			
			break;
		}
		
	}

	return hr;
}
void EMB::CMCResultMerge::ManageResult()
{

	BOOL bInPiece = FALSE;
	size_t n = 0;
	for (n=0;n<vecResults.size();n++)
	{
		bInPiece = FALSE;
		if (n<vecResults.size()-1)
		{
			LONGLONG nPos = _atoi64(vecResults[n+1].strSOM)+m_nCntDetectExtend;
			vector<ResultData>::iterator nIdx;
			if (vecResults[n].vecResultDatas.size() ==0)
			{
				continue;
			}
			if (_atoi64(vecResults[n].vecResultDatas[vecResults[n].vecResultDatas.size()-1].strFrameEnd)<nPos)
			{
				bInPiece = FALSE;
			}
			if ((vecResults[n+1].vecResultDatas.size()>0)&&(n<vecResults.size()-1))
			{
				for (nIdx=vecResults[n+1].vecResultDatas.begin();nIdx!=vecResults[n+1].vecResultDatas.end();nIdx++)
				{
					if (_atoi64(nIdx->strFrameStart)>=nPos)
					{
						break;
					}
				}

				if (nIdx == vecResults[n+1].vecResultDatas.end())
				{
					nIdx--;
				}

				if (_atoi64(vecResults[n].vecResultDatas[vecResults[n].vecResultDatas.size()-1].strFrameEnd)
					!=_atoi64(nIdx->strFrameStart))
				{
					bInPiece = FALSE;
				}
				else
				{
					if (vecResults[n].vecResultDatas[vecResults[n].vecResultDatas.size()-1].strDetectType.Compare(nIdx->strDetectType)==0)
					{
						bInPiece = TRUE;
					}
					else
					{
						bInPiece = FALSE;
					}
				}

				if (!bInPiece)
				{
					vecResults[n+1].vecResultDatas.erase(vecResults[n+1].vecResultDatas.begin(),nIdx);
				}
				else
				{
					if (nIdx!=vecResults[n+1].vecResultDatas.begin())
					{
						nIdx--;
					}
					vecResults[n].vecResultDatas[vecResults[n].vecResultDatas.size()-1].strFrameEnd = nIdx->strFrameEnd;
					vecResults[n].vecResultDatas[vecResults[n].vecResultDatas.size()-1].strFrameDuration.Format("%I64d",
						_atoi64(vecResults[n].vecResultDatas[vecResults[n].vecResultDatas.size()-1].strFrameEnd)-
						_atoi64(vecResults[n].vecResultDatas[vecResults[n].vecResultDatas.size()-1].strFrameStart));

					vecResults[n+1].vecResultDatas.erase(vecResults[n+1].vecResultDatas.begin(),nIdx);
				}
			}
		}

		for (size_t k=0;k<vecResults[n].vecResultDatas.size();k++)
		{
			vecResultDatas.push_back(vecResults[n].vecResultDatas[k]);
		}
	}
// 	struct ResultData
// 	{
// 		CString strDetectType;
// 		CString strDetectTypeName;
// 		CString strFrameStart;
// 		CString strFrameEnd;
// 		CString strFrameDuration;
// 	};
// 
// 	typedef struct _structResult
// 	{
// 		CString strFileParh;
// 		CString strTskID;
// 		int nSecID;
// 		CString strSOM;
// 		CString strEOM;
// 		CString strTotalSectionCount;
// 		vector<ResultData> vecResultDatas;
// 		bool operator < (const _structResult &m)const {	return nSecID < m.nSecID;}
// 
// 	}structResult;

	if (!m_structTask.bWirteResultXml)
	{
		return;
	}
	//下面写文件
	CTxParamString XmlOp;
	CTxStrConvert val;
	CString strErr = "";
	val.SetVal(strErr);
	XmlOp.SetElemVal("AnlysisResult", val);
	XmlOp.GoIntoKey("AnlysisResult");
	XmlOp.SetElemVal("TskInfo", val);
	XmlOp.GoIntoKey("TskInfo");
	val.SetVal(stResult.strTskID);
	XmlOp.SetAttribVal( NULL, "TskID", val);
	val.SetVal(stResult.strFileParh);
	XmlOp.SetAttribVal(NULL, "SrcDir", val);
	CString tempSecID;
	tempSecID.Format("%d",stResult.nSecID);
	val.SetVal(tempSecID);
	XmlOp.SetAttribVal(NULL,"SecID",val);
	val.SetVal(stResult.strSOM);
	XmlOp.SetAttribVal(NULL,"SOM",val);
	val.SetVal(stResult.strEOM);
	XmlOp.SetAttribVal(NULL,"EOM",val);
	val.SetVal(stResult.strTotalSectionCount);
	XmlOp.SetAttribVal(NULL,"nTotalSectionCount",val);
	XmlOp.OutofKey();
	//进入TskData   KEY
	val.SetVal(strErr);
	XmlOp.SetElemVal("TskData",val);
	XmlOp.GoIntoKey("TskData");
	//保留FileInfo项
	val.SetVal(strErr);
	XmlOp.SetElemVal("FileInfo",val);
	XmlOp.GoIntoKey("FileInfo");
	XmlOp.OutofKey();
	//进入技审结果统计KEY
	val.SetVal(strErr);
	XmlOp.SetElemVal("ResultInfo",val);
	XmlOp.GoIntoKey("ResultInfo");
    for (int i = 0 ;i<vecResultDatas.size();i++)
    {
		CString strNode = "";
		/*/
		strNode.Format("%s","ResultData");
		/*/
		strNode.Format("%s%04d","ResultData",i);
		//*/
		val.SetVal("");
		XmlOp.SetElemVal(strNode, val);
		XmlOp.GoIntoKey(strNode);

		val.SetVal(vecResultDatas[i].strDetectType);
		XmlOp.SetAttribVal(NULL, "DetectType", val);

		val.SetVal(vecResultDatas[i].strDetectTypeName);
		XmlOp.SetAttribVal(NULL, "DetectTypeName",val);

		val.SetVal(vecResultDatas[i].strFrameStart);
		XmlOp.SetAttribVal(NULL, "FrameStart",val);

		val.SetVal(vecResultDatas[i].strFrameEnd);
		XmlOp.SetAttribVal(NULL, "FrameEnd",val);

		val.SetVal(vecResultDatas[i].strFrameDuration);
		XmlOp.SetAttribVal(NULL, "FrameDuration",val);
		XmlOp.OutofKey();
    }
	 XmlOp.UpdateData();
	CString strXML;
	strXML = XmlOp;
	 TCHAR drive[MAX_PATH],dir[MAX_PATH],fname[MAX_PATH],ext[MAX_PATH];
	_tsplitpath(m_structTask.filePath.GetBuffer(0),drive,dir,fname,ext);
	CString totalXMLPath ;
	totalXMLPath.Format("%s%s%s%s",drive,dir,fname,"_Detect.xml");
	ofstream fileXML  ;
	fileXML.open(totalXMLPath);
	if (fileXML.is_open())
	{
		fileXML<<strXML.GetBuffer(strXML.GetLength());
		fileXML.close();
	}
	
	
}

HRESULT EMB::CMCResultMerge::OnMessageToWorker( const CTaskString& szInfo )
{
	HRESULT hr = S_OK;
	ST_EMBXMLMAININFO mainInfo;
	GetEmbXmlMainInfo(szInfo, mainInfo);
	if (mainInfo.nType == embxmltype_taskRunState)
	{
		ST_TASKRUNSTATE runState;
		runState.FromString(szInfo, TRUE);
		if (runState.guid == GUID_NULL)
		{
			ASSERT(FALSE);
			return E_FAIL;
		}
		else
		{
			CAutoLock lock(&m_csRunState);
			for (size_t i = 0; i < m_vTaskRunState.size(); ++i)
			{
				if (m_vTaskRunState[i].guid == runState.guid)
				{
					m_vTaskRunState[i].nState = runState.nState;
					m_vTaskRunState[i].tmLastReport = time(NULL);
					CFWriteLog(TEXT("receive subtaskRunstate id = %s, state = %d,percent = %d"), Guid2String(runState.guid), runState.nState, runState.nPercent);

					break;
				}
			}
		}
	}
	
	return hr;
}

BOOL EMB::CMCResultMerge::RegisterToDB()
{
	HRESULT hr = S_OK;
	CString strFilePathName;
	TCHAR drive[MAX_PATH],dir[MAX_PATH],fname[MAX_PATH],ext[MAX_PATH];
	_tsplitpath(m_structTask.filePath.GetBuffer(0),drive,dir,fname,ext);
	strFilePathName.Format("%s%s%s",drive,dir,fname);

	CFWriteLog(NULL, TEXT("start write merge result [%s] to db!!....."), m_structTask.strGuidBase);
	BOOL bRet = CTxADODBMgr::GetADODBMgr()->OpenDB(1, m_structTask.strDBConn);
	if (bRet)
	{
		if (m_structTask.nTotalSectionCount == 1)
		{
			CString strTaskFile = strFilePathName;
			strTaskFile += TEXT(".sql");
			CFile fileSingle;
			if (fileSingle.Open(strTaskFile, CFile::modeRead, NULL))
			{
				//single file mode
				long nLen = fileSingle.GetLength();
				if (nLen > 0)
				{
					char* pBuffer = new char[nLen+1];

					long nRead = fileSingle.Read(pBuffer, nLen);

					ASSERT(nRead == nLen);
					pBuffer[nLen] = '\0';
					CTxADOCommand cmd;
					//delete old first
					CString strSql;
					strSql.Format(TEXT("delete from T_FCVSFileInfo where strTaskID = '%s'"), m_structTask.strGuidBase);
					if(!cmd.ExecuteSQL(strSql))
					{
						ASSERT(FALSE);
					}
					strSql = pBuffer;
					if (!cmd.ExecuteSQL(strSql))
					{
						ASSERT(FALSE);
						CFWriteLog(TEXT("文件执行失败%s"), strTaskFile);
					}

					delete pBuffer;

				}
				else
				{
					CFWriteLog(TEXT("file no content to exec, %s"), strTaskFile);
				}
				fileSingle.Close();
				//delete sql file whatever
				DeleteFile(strTaskFile);
			}
		}
		else
		{
			CString strTaskPre = strFilePathName;
			for (size_t i = 0; i < m_structTask.nTotalSectionCount; ++i)
			{
				CString strTaskSplitFile;
				strTaskSplitFile.Format(TEXT("%s_wow%d.sql"),strTaskPre, i+1);
				if (_access(strTaskSplitFile, 04) == -1)
				{
					CFWriteLog(0, TEXT("split file can't access %s"), strTaskSplitFile);
					hr = EMBERR_FILEACCESS;
					break;
				}
				else
				{
					CFile fileSp;
					if (fileSp.Open(strTaskSplitFile, CFile::modeRead, NULL))
					{
						//single file mode
						long nLen = fileSp.GetLength();
						if (nLen > 0)
						{
							char* pBuffer = new char[nLen+1];

							long nRead = fileSp.Read(pBuffer, nLen);

							ASSERT(nRead == nLen);
							pBuffer[nLen] = '\0';
							CTxADOCommand cmd;
							CString strSql;
							if (i == 0)
							{
								//delete old first
								strSql.Format(TEXT("delete from T_FCVSFileInfo where strTaskID = '%s'"), m_structTask.strGuidBase);
								if(!cmd.ExecuteSQL(strSql))
								{
									ASSERT(FALSE);
								}
							}

							strSql = pBuffer;
							delete pBuffer;
							if (!cmd.ExecuteSQL(strSql))
							{
								ASSERT(FALSE);
								CFWriteLog(0, TEXT("文件执行失败%s"), strTaskSplitFile);
							}

						}
						else
						{
							CFWriteLog(0, TEXT("file no content to exec, %s"), strTaskSplitFile);
						}

						fileSp.Close();
						//delete sql file whatever
						DeleteFile(strTaskSplitFile);

					}
				}
			}

		}	
	}
	else
	{
		hr = EMBERR_DBOPEN;
	}

	CTxADODBMgr::ReleaseDBMgr();
	return hr;
}
