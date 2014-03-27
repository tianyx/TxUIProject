#include "StdAfx.h"
#include "TxTransFile.h"
#include "StrConvert.h"
#include "IEMBBaseInterface.h"
#include "EMBDefine.h"
#include "EMBCommonFunc.h"
#include "TxLogManager.h"
#include "Markup.h"
#include "afxinet.h"
#include "io.h"
#include "VRMD5E.h"
#include "TxADOConn.h"

#define FREADBUFF 3840
using namespace EMB;


DWORD __stdcall TaskRunProc(LPVOID lparam)
{
	CTxTransFile* pObj = (CTxTransFile*)lparam;
	if (pObj)
	{
		pObj->RunTaskLoop();
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
CTxTransFile::CTxTransFile(void)
{
	m_hThreadTask = NULL;
	m_pReportCallback = NULL;
	m_nPercent = 0;
	m_nSubErrCode = S_OK;
	m_nMaxFtpSpeedPerExc = 40*1024*1024;
	m_hEventQuit = CreateEvent(NULL, TRUE, FALSE, NULL);

}

CTxTransFile::~CTxTransFile(void)
{
}


HRESULT EMB::CTxTransFile::QueryPluginInfo( VECPLUGINFOS& vInfoOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_PWorkerTransFile;
	info.nPlugInType = PluginType_Wroker;
	info.nSubType = SubType_FileTrans;
	vInfoOut.push_back(info);
	return S_OK;

}

HRESULT EMB::CTxTransFile::DoTask( const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback )
{
	if (m_hThreadTask)
	{
		ASSERT(FALSE);
		return EMBERR_WORKERBUSY;
	}

	HRESULT hr = ParseTaskInfo(szTaskIn);
	if (hr != S_OK)
	{
		return hr;
	}

	m_nMaxFtpSpeedPerExc = m_taskInfo.nSpeedLimit < 1000? 1000: m_taskInfo.nSpeedLimit*1000;
	m_nCurrSrcSite = -1;
	m_nCurrDesSite = -1;

	ResetEvent(m_hEventQuit);
	m_pReportCallback = pICallback;
	m_hThreadTask = CreateThread(NULL, 0, TaskRunProc, (LPVOID)this, 0, 0);
	return S_OK;
}

HRESULT EMB::CTxTransFile::CancelTask()
{
	SetEvent(m_hEventQuit);
	return S_OK;
}

HRESULT EMB::CTxTransFile::GetTaskProgress( CEMBWorkString& szInfo )
{
	ST_WORKERREPORT report;
	report.nPercent = m_nPercent;
	CString strRet;
	report.ToString(strRet);
	szInfo = strRet;
	return S_OK;
}

HRESULT EMB::CTxTransFile::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
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

HRESULT EMB::CTxTransFile::OnFirstInit()
{
	GetTxLogMgr()->AddNewLogFile(LOGKEY_TRASNFILE, TEXT("Transfile"));

	return S_OK;
}

void EMB::CTxTransFile::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	ReleaseTxLogMgr();
	TRACE("\nCTxTransFile::OnFinalRelease() ");

	delete this;
}

BOOL EMB::CTxTransFile::RunTaskLoop()
{
	//
	CFWriteLog(NULL, TEXT("task started!..., logicId = %s"), m_taskInfo.strClipLogicID);
	HRESULT hr = TansferFile();
	do 
	{
		if (FAILED(hr))
		{
			break;
		}
		if (m_taskInfo.bWriteLocalResult)
		{
			hr = WriteLocalResult();
			if (FAILED(hr))
			{
				break;
			}
		}

		if (m_taskInfo.bRegisterToDB)
		{
			//write to db
			hr = RegisterToDB();
			if (FAILED(hr))
			{
				break;
			}
		}
			
	} while (FALSE);
	
	if (FAILED(hr))
	{
		m_nPercent = -1;
	}
	else
	{
		m_nPercent = 100;
	}

	ST_WORKERREPORT report;
	if (m_pReportCallback)
	{
		report.nPercent = m_nPercent;
		report.code = hr;
		CString strRet;
		report.ToString(strRet);
		m_pReportCallback->OnDllReportTaskProgress(strRet);
	}

	CFWriteLog(NULL, TEXT("task end!!....."));
	m_hThreadTask = NULL;

	return TRUE;

}

HRESULT EMB::CTxTransFile::ParseTaskInfo( const CString& strXml)
{
	HRESULT hr = S_OK;
	m_taskInfo.FromString(strXml);
	if (m_taskInfo.vSitSrc.size() == 0
		||m_taskInfo.vSitDes.size() == 0)
	{
		CFWriteLog(0, TEXT("task param error, no ftp sit"));
		hr = EMBERR_INVALIDARG;
		return hr;
	}

	if (m_taskInfo.strClipLogicID.IsEmpty())
	{
		CFWriteLog(0, TEXT("task param error, no cliplogicid"));
		hr = EMBERR_INVALIDARG;
		return hr;
	}

	if (m_taskInfo.bRegisterToDB)
	{
		//check des ftp sit dbconn
		for (size_t i = 0; i < m_taskInfo.vSitDes.size(); ++i)
		{
			if (m_taskInfo.vSitDes[i].strDBConn.IsEmpty())
			{
				CFWriteLog(0, TEXT("no db desc in des ftp site info "));
				hr = EMBERR_INVALIDARG;
				return hr;
			}
		}
	}

	if (m_taskInfo.bDownToLocal)
	{
		//check down directory;
		if (m_taskInfo.strLocalDownDir.IsEmpty() || m_taskInfo.strLocalDownFileName.IsEmpty())
		{
			CFWriteLog(0, TEXT("local down file or path is empty"));
			hr = EMBERR_INVALIDARG;
		}
		CreateDirectory(m_taskInfo.strLocalDownDir, NULL);
		if (_access(m_taskInfo.strLocalDownDir, 0) == -1)
		{
			CFWriteLog(0, TEXT("local down directory not existed"));
			hr = EMBERR_INVALIDARG;
		}

	}

	if (m_taskInfo.bMD5Check && m_taskInfo.strMD5Compare.IsEmpty())
	{
		CFWriteLog(0, TEXT("md5 compare text is empty"));
		hr = EMBERR_INVALIDARG;
	}

	return hr;
}

HRESULT EMB::CTxTransFile::TansferFile()
{
	//create internet conn
	HRESULT hRet = E_FAIL;
	CFtpConnection *pFtpConnectionSrc = NULL;
	CFtpConnection *pFtpConnectionDes = NULL;

	try
	{
		CInternetSession pInetSession(NULL, 1, PRE_CONFIG_INTERNET_ACCESS);
		//set code page
		if(m_taskInfo.nCodePage == CP_UTF8)
		{
			pInetSession.SetOption(INTERNET_OPTION_CODEPAGE , CP_UTF8);
		}

		ST_FTPSITEINFO sitInfoSrc;
		ST_FTPSITEINFO sitInfoDes;
		int nFtpSrcID = -1;
		int nFtpDesID = -1;
		//find first valid ftp
		for (size_t i = 0; i < m_taskInfo.vSitSrc.size(); ++i)
		{
			ST_FTPSITEINFO& sitRef = m_taskInfo.vSitSrc[i];
			try
			{
				pFtpConnectionSrc = pInetSession.GetFtpConnection(sitRef.strFtpIp, sitRef.strUser, sitRef.strPw, sitRef.nFtpPort, sitRef.nPassive);
				if (pFtpConnectionSrc)
				{
					//valid
					m_nCurrSrcSite = i;
					sitInfoSrc = sitRef;
					break;
				}
			}
			catch ( CInternetException * e)
			{
				//ftp is not valid
				pFtpConnectionSrc = NULL;
				CFWriteLog(0, TEXT("ftp not valid %s:%d"), sitRef.strFtpIp, sitRef.nFtpPort);
			}
		}

		if (!pFtpConnectionSrc)
		{
			CFWriteLog(0, TEXT("no valid src ftp to use, clip trans failed, logicid = %s"), m_taskInfo.strClipLogicID);
			return EMBERR_INVALIDFTP;
		}

		//find first valid Des Ftp
		for (size_t i = 0; i < m_taskInfo.vSitDes.size(); ++i)
		{
			ST_FTPSITEINFO& sitRef = m_taskInfo.vSitDes[i];
			try
			{
				pFtpConnectionDes = pInetSession.GetFtpConnection(sitRef.strFtpIp, sitRef.strUser, sitRef.strPw, sitRef.nFtpPort, sitRef.nPassive);
				if (pFtpConnectionDes)
				{
					//valid
					m_nCurrDesSite = i;
					sitInfoDes = sitRef;
					break;
				}
			}
			catch ( CInternetException * e)
			{
				//ftp is not valid
				pFtpConnectionDes = NULL;
				CFWriteLog(0, TEXT("ftp not valid %s:%d"), sitRef.strFtpIp, sitRef.nFtpPort);
			}
		}

		if (!pFtpConnectionDes)
		{
			CFWriteLog(0, TEXT("no valid des ftp to use, clip trans failed, logicid = %s"), m_taskInfo.strClipLogicID);
			return EMBERR_INVALIDFTP;
		}

		//set file dir
		string szSrcDir = m_taskInfo.strSrcDir;
		string szDirSrcUtf8 = szSrcDir;

		string szDesDir = m_taskInfo.strDesDir;
		string szDesDirUtf8 = szDesDir;

		string szNameSrc = m_taskInfo.strSrcFileName;
		string szNameSrcUtf8 = szNameSrc;

		string szNameDes = m_taskInfo.strDesFileName;
		string szNameDesUtf8 = szNameDes;

		if (m_taskInfo.nCodePage == CP_UTF8)
		{
			wstring wszSrcDir = Ansi2W(szSrcDir);
			szDirSrcUtf8 = W2UTF8(wszSrcDir);

			wstring wszDesDir = Ansi2W(szDesDir);
			szDesDirUtf8 = W2UTF8(wszDesDir);

			wstring wszNameSrc = Ansi2W(szNameSrc);
			szNameSrcUtf8 = W2UTF8(wszNameSrc);

			wstring wszNameDes = Ansi2W(szNameDes);
			szNameDesUtf8 = W2UTF8(wszNameDes);
		}

		BOOL bSrcRet = TRUE;
		if (szDirSrcUtf8.size()> 0)
		{
			bSrcRet = pFtpConnectionSrc->SetCurrentDirectory(szDirSrcUtf8.c_str());
		}
		BOOL bDesRet = TRUE;
		if (szDesDirUtf8.size() > 0)
		{
			bDesRet = pFtpConnectionDes->SetCurrentDirectory(szDesDirUtf8.c_str());
		}

		INT64 nSrcFileLength = 0;
		if (!bSrcRet || !bDesRet)
		{
			ASSERT(FALSE);
			CFWriteLog(0, TEXT("%s ftp path not found"), bSrcRet? szDesDir.c_str(): szSrcDir.c_str());
			hRet = EMBERR_DIRACCESS;
			throw 1;
		}

		CFtpFileFind finder(pFtpConnectionSrc);
		BOOL bFind = finder.FindFile(szNameSrcUtf8.c_str());  // 查找服务器上当前目录的任意文件 

		if (!bFind)   // 如果一个文件都找不到，结束查找 
		{ 
			CFWriteLog(0, TEXT("src file not found. %s"), szNameSrc.c_str());
			hRet = EMBERR_NOTFOUND;
			throw 1;
		} 
		else
		{
			finder.FindNextFile();
			nSrcFileLength = finder.GetLength();
			CFWriteLog(0, "src file length = %I64dM", nSrcFileLength/1024/1024);
		}
		//后缀名
		finder.Close();

		CFtpFileFind finderDes(pFtpConnectionDes);
		BOOL bFindDes = finderDes.FindFile(szNameDesUtf8.c_str());
		finderDes.Close();
		if (bFindDes)
		{
			CFWriteLog(0, TEXT("des ftp file already existed,delete and continue. %s "), szNameDes.c_str());
			if(!pFtpConnectionDes->Remove(szNameDesUtf8.c_str()))
			{
				CFWriteLog(0, TEXT("des file del failed! err =%d"), GetLastError());
				hRet = EMBERR_FILEACCESS;
				throw 1;
			}
		}

		//start copy file
		CInternetFile* pFileSrc = NULL;
		CInternetFile* pFileDes = NULL;
		CFile* pFileLocal = NULL;
		CMD5CALEX* pMd5 = NULL;
		CString strMd5Result;
		//start trans
		try
		{
			pFileSrc =pFtpConnectionSrc->OpenFile(szNameSrcUtf8.c_str());
			if (!pFileSrc)
			{
				hRet = EMBERR_FILEACCESS;
				CFWriteLog(0, TEXT("src ftp文件打开失败, %s"),szNameSrc.c_str());
				throw 1;
			}
			pFileDes = pFtpConnectionDes->OpenFile(szNameDesUtf8.c_str(), GENERIC_WRITE);
			if (!pFileDes)
			{
				CFWriteLog(0, TEXT("des ftp文件打开失败, %s"), szNameDes.c_str());
				throw 1;
			}

			if (m_taskInfo.bDownToLocal)
			{
				CString strLocalFile = m_taskInfo.strLocalDownDir;
				strLocalFile.TrimRight(TEXT("\\"));
				CreateDirectory(strLocalFile, NULL);
				strLocalFile += TEXT("\\");
				strLocalFile += m_taskInfo.strLocalDownFileName;
				pFileLocal = new CFile;
				if (!pFileLocal->Open(strLocalFile, CFile::modeCreate|CFile::modeWrite, NULL))
				{
					CFWriteLog(0, TEXT("localdown file open failed. %s"), strLocalFile);
					hRet = EMBERR_FILEACCESS;
					throw 2;
				}
			}

			if (m_taskInfo.bMD5Check)
			{
				pMd5 = new CMD5CALEX;
				pMd5->Begin();
			}

			INT64 nFileTransed= 0;
			INT64 nPercent = 0;
			const INT64  nSleepPot = m_nMaxFtpSpeedPerExc/100;
			INT64 nCurrTransed  = 0;
			INT64 nTransedPerSecond = 0;
			DWORD nTickStart = GetTickCount();
			INT64 nSpeedKBPerS = 0;
			while(nFileTransed < nSrcFileLength)
			{
				if (m_hEventQuit && WaitForSingleObject(m_hEventQuit, 0) == WAIT_OBJECT_0)
				{
					break;
				}
				byte szbuff[FREADBUFF];
				UINT nRead = pFileSrc->Read(szbuff, FREADBUFF);
				if (nRead > 0)
				{
					pFileDes->Write(szbuff, nRead);
					if (pFileLocal)
					{
						pFileLocal->Write(szbuff, nRead);
					}
					if (pMd5)
					{
						pMd5->CalBuff(szbuff, nRead);
					}
					nFileTransed += nRead;
					INT64 nNewPercent = nFileTransed*100/nSrcFileLength; 
					if (nNewPercent > nPercent)
					{
						nPercent = nNewPercent;
						m_nPercent = nPercent == 100? 99:nPercent;
					}

					//limit speed
					nCurrTransed += nRead;
					if (nSleepPot > 0 && nCurrTransed > nSleepPot)
					{
						nCurrTransed = 0;
						//limit speed
						Sleep(10);
					}
					//calc speed
					nTransedPerSecond += nRead;
					DWORD nTickNow = GetTickCount();
					if (nTickNow > nTickStart)
					{
						if (nTickNow - nTickStart >= 1000)
						{
							//
							nSpeedKBPerS = nTransedPerSecond*1000/((nTickNow - nTickStart)*1024);
							nTransedPerSecond = 0;
							nTickStart = nTickNow;
							//TRACE("\ntask%I64d, speed %dKB/s", m_taskInfo.strTaskID, nSpeedKBPerS);
							// 										TRACE("\ntask%I64d, percent%d", m_taskInfo.nTaskId, nPercent);
						}
					}
				}
				else
				{
					if (nFileTransed == nSrcFileLength)
					{
						//task finished.
						hRet = S_OK;
						m_nPercent = 99;
					}
					else
					{
						//
						//CFWriteLog(0, TEXT("读取源文件失败"));
						CFWriteLog(0, TEXT("src file write err %s"), szNameSrc.c_str());
						hRet = EMBERR_FILEACCESS;
					}
					break;
				}
			}

			//file trans finished
			if (nFileTransed == nSrcFileLength)
			{
				hRet = S_OK;
				m_nPercent =99;
				TRACE("\nfile trans finished");
			}

			if (hRet==S_OK && pMd5)
			{
				strMd5Result = pMd5->GetResult();
				if (!strMd5Result.IsEmpty())
				{
					if (m_taskInfo.strMD5Compare.CompareNoCase(strMd5Result) != 0)
					{
						CFWriteLog(TEXT("md5 compare error src:%s, des:%s"), m_taskInfo.strMD5Compare, strMd5Result);
						hRet = EMBERR_MD5NOTMATCH;
					}
				}
			}


		}
		catch (CInternetException* e)
		{
			ASSERT(FALSE);
			CFWriteLog(0, TEXT("ftp file transfer error!"));
			hRet = EMBERR_INTERNET;
		}
		catch(...)
		{
			ASSERT(FALSE);
			CFWriteLog(0, TEXT("file transfer error!"));
		}

		if (pFileSrc)
		{
			pFileSrc->Close();
			delete pFileSrc;
		}
		if (pFileDes)
		{
			pFileDes->Close();
			delete pFileDes;
		}
		if (pFileLocal)
		{
			pFileLocal->Close();
			delete pFileLocal;
		}

		if (pMd5)
		{
			delete pMd5;
		}

	}
	catch (CInternetException* e)
	{
		hRet = EMBERR_INTERNET;
		CFWriteLog(0, TEXT("network error code = %d"), e->m_dwError);
	}
	catch(...)
	{
		ASSERT(FALSE);
	}

	if (pFtpConnectionSrc!=NULL) 
	{ 
		pFtpConnectionSrc->Close();
		delete pFtpConnectionSrc;
	}

	if (pFtpConnectionDes!=NULL) 
	{ 
		pFtpConnectionDes->Close();
		delete pFtpConnectionDes;
	}

	return hRet;
}

HRESULT EMB::CTxTransFile::WriteLocalResult()
{
	HRESULT hr = S_OK;
	ASSERT(m_nCurrDesSite >= 0 && m_nCurrDesSite < m_taskInfo.vSitDes.size());
	ST_TRANSRESULT rst;
	rst.strDestDBConn = m_taskInfo.vSitDes[m_nCurrDesSite].strDBConn;
	rst.strClipID  = m_taskInfo.strClipID;
	rst.strClipLogicID = m_taskInfo.strClipLogicID;
	CString strRes;
	rst.ToString(strRes);
	CString strResFile = m_taskInfo.strLocalDownDir;
	strResFile.TrimRight(TEXT("\\"));
	strResFile += TEXT("\\");
	strResFile += m_taskInfo.strLocalDownFileName;
	strResFile += TEXT(".res");
	CFile file;
	if (!file.Open(strResFile, CFile::modeCreate|CFile::modeWrite, NULL))
	{
		CFWriteLog(0, TEXT("result file write err! %s"), strResFile);
		hr = EMBERR_FILEACCESS;
	}
	else
	{
		file.Write(strRes, strRes.GetLength());
		file.Close();
	}

	return hr;
}

HRESULT EMB::CTxTransFile::RegisterToDB()
{
	HRESULT hr = S_OK;
	if (m_nCurrDesSite < 0 || m_nCurrDesSite >= m_taskInfo.vSitDes.size())
	{
		ASSERT(FALSE);
		CFWriteLog(0, TEXT("no des site."));
	}
	ST_FTPSITEINFO& sitRef = m_taskInfo.vSitDes[m_nCurrDesSite];
	ASSERT(!sitRef.strDBConn.IsEmpty());
	srand(time(NULL));
	int nRandKey = rand()%10000;
	BOOL bOpen = CTxADODBMgr::GetADODBMgr()->OpenDB(nRandKey, sitRef.strDBConn);
	if (!bOpen)
	{
		hr = EMBERR_DBOPEN;
		CFWriteLog(0, TEXT("dbopen error! %s"), sitRef.strDBConn);
		return hr;
	}

	//organize sql
	CString strSqlReq;
	VECSTRINGS vSqlRun;
	strSqlReq.Format(TEXT("select strClipLogicID from T_BVSID where strClipLogicID = '%s'"), m_taskInfo.strClipLogicID);
	CTxADORecordSet rs(nRandKey);
	if (!rs.ExecuteSQL(strSqlReq))
	{
		hr = EMBERR_SQLEXEC;
		return hr;
	}
	
	CString strTmpSql;
	BOOL bUpdateMode = FALSE;
	CTime tmNow(time(NULL));
	CString strTimeNow = tmNow.Format(TEXT("%Y-%m-%d %H:%M:%S"));
	CString strLocation = TEXT("00000000000000000000000000000000");
	strLocation.SetAt(sitRef.nLocation -1, '1');

	if (!rs.IsEOF())
	{
		CString strClipId = rs.GetVal(TEXT("strClipID")).GetAsString();
		strLocation = rs.GetVal(TEXT("strLocation"), TEXT("00000000000000000000000000000000")).GetAsString(TEXT(""));
		ASSERT(strLocation.GetLength() > sitRef.nLocation);
		strLocation.SetAt(sitRef.nLocation -1, '1');
		if (strClipId.CompareNoCase(m_taskInfo.strClipID) == 0)
		{
			CFWriteLog(0, TEXT("clip already existed in t_bvsid,only update location."));
			//same clipid, update location
			strTmpSql.Format(TEXT("update T_BVSID set strLocation = '%s' where strClipID = '%s'"), m_taskInfo.strClipLogicID);
			vSqlRun.push_back(strTmpSql);
		}
		else
		{
			CFWriteLog(0, TEXT("clip already existed in t_bvsid, but clipid not same [old:%s][new:%s], delete it and continue"), strClipId, m_taskInfo.strClipID);
			//delete old
			strTmpSql.Format(TEXT("delete from t_bvsid where strClipLogicID = '%s'"), m_taskInfo.strClipLogicID);
			vSqlRun.push_back(strTmpSql);

		}
	}

	if (!bUpdateMode)
	{
		strTmpSql.Format(TEXT("insert into T_BVSID ([strClipID],[strClipName],[strClipLogicID],[strSOM] ,[strEOM], [strDuration] \
							  ,[strLSOM],[strLEOM],[strLDuration],[strLocation],[nDelete],[nReady],[nCheck] ,[nClipType] \
							  ,[strChnID],[tPlanAirTime],[tPlanLastAirTime],[tDubDateTime],[strDubBy],[nBitRate],[strMD5] \
							  )values('%s', '%s', '%s', '%s', '%s', '%s' \
							  , '%s', '%s', '%s', '%s', '%d', '%d', '%d', '%d' \
							  , '%s', '%s', '%s', '%s', '%s', '%d', '%s');"), 
							  m_taskInfo.strClipID, m_taskInfo.dbBvsInfo.strClipName,  m_taskInfo.strClipLogicID, m_taskInfo.dbBvsInfo.strSOM, m_taskInfo.dbBvsInfo.strEOM, m_taskInfo.dbBvsInfo.strDuration,
							  m_taskInfo.dbBvsInfo.strLSOM, m_taskInfo.dbBvsInfo.strLEOM, m_taskInfo.dbBvsInfo.strLDuration, strLocation, 0, 1, 0, m_taskInfo.dbBvsInfo.nClipType, 
							  m_taskInfo.dbBvsInfo.strChID, m_taskInfo.dbBvsInfo.tPlanAirTime, m_taskInfo.dbBvsInfo.tPlanLastAirTime, strTimeNow, TEXT("EMB"), m_taskInfo.dbBvsInfo.nBitRate, m_strMD5Val);
		vSqlRun.push_back(strTmpSql);

	}
	

	strSqlReq.Format(TEXT("select strClipLogicID from T_LOCATIONINFO where strClipLogicID = '%s' and strStorageName = '%s'"), m_taskInfo.strClipLogicID, sitRef.strStoreName);

	if (!rs.ExecuteSQL(strSqlReq))
	{
		hr = EMBERR_SQLEXEC;
		return hr;
	}

	if (!rs.IsEOF())
	{
		CFWriteLog(0, TEXT("clip already existed in T_LOCATIONINFO, delete it and continue"));
		strTmpSql.Format(TEXT("delete from T_LOCATIONINFO where strClipLogicID = '%s' and strStorageName = '%s'"), m_taskInfo.strClipLogicID, sitRef.strFtpName);
		vSqlRun.push_back(strTmpSql);
	}

	//insert 
	strTmpSql.Format(TEXT("INSERT INTO T_LOCATIONINFO([strClipLogicID] ,[strStorageName],[strMediaType],[strClipID] \
		,[nBitRate],[nDelete],[nReady] \
		,[tCreateTime],[nCheck],[strFileSize], [nAFDType]) \
		VALUES('%s', '%s', '%s', '%s', \
		'%d', '%d', '%d', \
		'%s', '%d','%s', '%d')"),m_taskInfo.strClipLogicID, sitRef.strStoreName, m_taskInfo.dbLocationInfo.strMediaType, m_taskInfo.strClipID,
		m_taskInfo.dbLocationInfo.nBitRate, 0, 1, strTimeNow, 0, m_taskInfo.dbLocationInfo.strFileSize,m_taskInfo.dbLocationInfo.nAfdType);
	
	vSqlRun.push_back(strTmpSql);

	CTxADOCommand cmd(nRandKey);
	if (!cmd.ExecuteSQLs(vSqlRun))
	{
		hr = EMBERR_SQLEXEC;
	}
	
	return hr;
}
