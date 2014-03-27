#include "StdAfx.h"
#include "MCResultMerge.h"
#include "IEMBBaseInterface.h"
#include "TxLogManager.h"
#include "EMBDefine.h"
#include "EmbStructDef.h"
#include "TxParamString.h"
#include <fstream>
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

	//to do... and check code
//	ASSERT(FALSE);

	m_strTask = szTaskIn;
	m_structTask.FromString(m_strTask);
	m_pReportCallback = pICallback;
	
	if (m_structTask.filePath == "" || m_structTask.nTotalSectionCount <=1)
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
	return S_OK;
}

HRESULT EMB::CMCResultMerge::GetTaskProgress( CEMBWorkString& szInfo )
{
	//get it
	//ASSERT(FALSE);
	m_nPercent =1;

	ST_WORKERREPORT report;
	report.nPercent = m_nPercent;
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
	FindEnoughthread();
	return TRUE;
}
void EMB::CMCResultMerge::AnalysisFilePath()
{
	TCHAR drive[MAX_PATH],dir[MAX_PATH],fname[MAX_PATH],ext[MAX_PATH];
	_tsplitpath(m_structTask.filePath.GetBuffer(0),drive,dir,fname,ext);
	// index begin of 1
	for (int i = 1;i <= m_structTask.nTotalSectionCount;i++)
	{
		CString tempArrs;
		tempArrs.Format("%s%s%s_%d%s",drive,dir,fname,i,"_Detect.xml");
		m_csLockSecXmlArr.Lock();
		m_arrSecXmlPath.Add(tempArrs);
		m_csLockSecXmlArr.Unlock();
	}
}
void EMB::CMCResultMerge::FindEnoughthread()  //寻找文件线程
{
	deletedFileCount = 0;
	while(isTaskRun)
	{
		Sleep(1000);
// 		DWORD dwRet = TxWaitObjWithQuit(m_hEventMsgProcdureThread, m_hEventAnaQuit, INFINITE);
// 		if (WAIT_OBJECT_0 != dwRet)
// 		{
// 			break;
// 
// 		}
		CString strFirstPath = m_arrSecXmlPath.GetAt(0);
		if (_access(strFirstPath, 0) == 0)
		{
			OutputDebugString(strFirstPath);
			m_xmlDoc.LoadXMLFromFile(strFirstPath);
			//mamzero()
			//memset(&stResult,0,sizeof(stResult));
			structResult temp;
			
			temp.strFileParh = strFirstPath;
			m_xmlDoc.GetResultInfos(temp);
			vecResults.push_back(temp);
			m_nCntDetectExtend = 250;
			::DeleteFile(temp.strFileParh);
			stResult =temp;
			m_arrSecXmlPath.RemoveAt(0);
			deletedFileCount++;
		}
		if (deletedFileCount>= m_structTask.nTotalSectionCount)
		{
			ManageResult();
			m_nPercent = 100;

			if (m_pReportCallback)
			{
				CString strRst;
				strRst.Format("<workReport percent=\"%d\" code =\"%d\"></workReport>", 100, 0);
				m_pReportCallback->OnDllReportTaskProgress(strRst);
			}
			
			break;
		}
		else
		{
			continue;
		}
	}
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
	fileXML<<strXML.GetBuffer(strXML.GetLength());
	fileXML.close();
	
}