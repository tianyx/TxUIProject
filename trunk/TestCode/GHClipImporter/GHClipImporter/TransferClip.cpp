#include "StdAfx.h"
#include "TransferClip.h"
#include "TxADOConn.h"
#import "msxml.dll"
#include "StrConvert.h"
#include "FGlobal.h"
#include "afxinet.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FREADBUFF 4000

CTransferClip::CTransferClip(ITaskReporter* pIReporter)
{
	m_pIReporter = pIReporter;
	for (int i = 0; i < 20; ++i)
	{
		m_vxmlTaskData.push_back(TEXT(""));
	}
}

CTransferClip::~CTransferClip(void)
{
}

BOOL CTransferClip::AnalyseXmlFile()
{
	CoInitialize(NULL);
	MSXML::IXMLDOMDocumentPtr pCommandDoc;
	pCommandDoc.CreateInstance(__uuidof(MSXML::DOMDocument));           
	pCommandDoc->put_async(VARIANT_FALSE);            
	pCommandDoc->put_validateOnParse(VARIANT_FALSE);           
	pCommandDoc->put_resolveExternals(VARIANT_FALSE);           
	pCommandDoc->put_preserveWhiteSpace(VARIANT_TRUE); 
	if (!pCommandDoc->load(m_taskInfo.strTaskXMlFile.GetBuffer(0)))
	{
		return FALSE;
	}

	MSXML::IXMLDOMNodeListPtr pNodeListMetaData = pCommandDoc->getElementsByTagName("Metadata"); 
	if(pNodeListMetaData->length<3)
	{
		pCommandDoc->Release();
		return FALSE;
	}

	//Package
	MSXML::IXMLDOMNodeListPtr IXMLDOMNodeListPtrTemp;
	MSXML::IXMLDOMNodePtr IXMLDOMNodePtrTemp;
	_variant_t vTemp;

	IXMLDOMNodePtrTemp = pCommandDoc->selectSingleNode("ADI//Metadata//AMS");
	vTemp = IXMLDOMNodePtrTemp->attributes->getNamedItem("Asset_ID")->text;
	m_vxmlTaskData[ek_packId] = vTemp.bstrVal;//PacakgeID
	if (m_vxmlTaskData[ek_packId].IsEmpty())
	{
		WriteMsg(TEXT("Asset_ID为空"));
	}
	vTemp.Clear();
	//MOV
	IXMLDOMNodeListPtrTemp = pCommandDoc->selectNodes("ADI//Asset//Asset//Metadata//AMS");
	IXMLDOMNodePtrTemp = IXMLDOMNodeListPtrTemp->Getitem(IXMLDOMNodeListPtrTemp->Getlength()-1);
	vTemp = IXMLDOMNodePtrTemp->attributes->getNamedItem("Asset_Class")->text;
	CString strTemp = "Movie";
	if(vTemp.bstrVal!=strTemp)
	{
		WriteMsg(TEXT("Asset_Class必须为Movie"));
		return FALSE;
	}
	vTemp = IXMLDOMNodePtrTemp->attributes->getNamedItem("Asset_ID")->text;
	m_vxmlTaskData[ek_movId] = TEXT("NULL");

	vTemp.Clear();
	vTemp = IXMLDOMNodePtrTemp->attributes->getNamedItem("Asset_Name")->text;
	m_vxmlTaskData[ek_ClipName] = vTemp.bstrVal;

	vTemp.Clear();
	vTemp = IXMLDOMNodePtrTemp->attributes->getNamedItem("Creation_Date")->text;
	m_vxmlTaskData[ek_strBroadTime] = vTemp.bstrVal;

	//FTP
	IXMLDOMNodeListPtrTemp = pCommandDoc->selectNodes("ADI//Asset//Asset//Content");
	IXMLDOMNodePtrTemp = IXMLDOMNodeListPtrTemp->Getitem(IXMLDOMNodeListPtrTemp->Getlength()-1);
	
	vTemp.Clear();
	vTemp = IXMLDOMNodePtrTemp->attributes->getNamedItem("Value")->text;
	m_vxmlTaskData[ek_ftp] = vTemp.bstrVal;

	//Title
	IXMLDOMNodeListPtrTemp = pCommandDoc->selectNodes("ADI//Asset//Metadata//App_Data");
	IXMLDOMNodePtrTemp = pCommandDoc->selectSingleNode("ADI//Asset//Metadata//AMS");
	vTemp.Clear();
	vTemp = IXMLDOMNodePtrTemp->attributes->getNamedItem("Asset_Class")->text;
// 	strTemp = "title";
// 	if(vTemp.bstrVal!=strTemp)
// 	{
// 		WriteMsg(TEXT(""))
// 		return FALSE;
// 	}
	for(long i=0; i<IXMLDOMNodeListPtrTemp->length-1; i++)
	{
		vTemp.Clear();
		vTemp = IXMLDOMNodeListPtrTemp->Getitem(i+1)->attributes->getNamedItem("Name")->text;
		strTemp = "Licensing_Window_End";
		CString str = "Show_Type";
		CString strRunTime = "Run_Time";
		if(vTemp.bstrVal==strTemp)
		{
			vTemp = IXMLDOMNodeListPtrTemp->Getitem(i+1)->attributes->getNamedItem("Value")->text;
			m_vxmlTaskData[ek_holdTime] = vTemp.bstrVal;

		}
		else if(vTemp.bstrVal==str)
		{
			vTemp = IXMLDOMNodeListPtrTemp->Getitem(i+1)->attributes->getNamedItem("Value")->text;
			m_vxmlTaskData[ek_type] = vTemp.bstrVal;
		}
		else if (vTemp.bstrVal==strRunTime)
		{
			vTemp = IXMLDOMNodeListPtrTemp->Getitem(i+1)->attributes->getNamedItem("Value")->text;
			m_vxmlTaskData[ek_duration] = vTemp.bstrVal;
		}
	}
	return TRUE;
		
}

BOOL CTransferClip::InitDB()
{
	CoInitialize(NULL); 

	return TRUE;
}


CString CTransferClip::GenClipID()
{
	CString strRet;
	CTxADOCommand cmd;
	_RecordsetPtr ptr = cmd.ExecuteStoredProc(TEXT("sp_GetNewDUBID"));
	if (ptr)
	{
		strRet = ptr->GetCollect("NewDubID");
	}

	if (strRet.IsEmpty())
	{
		CFWriteLog(TEXT("获取NewDubID失败"));
		WriteMsg(TEXT("从数据库获取NewDubID失败"));
	}

	return strRet;
}

BOOL CTransferClip::Run()
{
	WriteMsg(TEXT("task start."));
	if (!InitDB())
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	//分析xml文件获取素材信息
	if( !AnalyseXmlFile() )
	{
		WriteMsg(TEXT("xml 解析失败"));
		ASSERT(FALSE);
		return FALSE;
	}
		
	if (CheckLogicIDInDB(m_vxmlTaskData[ek_packId]))
	{
		return FALSE;
	}

	m_strClipId = GenClipID();
	if (m_strClipId.IsEmpty())
	{
		WriteMsg(TEXT("生成物理ID失败, 数据库访问失败"));
		ASSERT(FALSE);
		return FALSE;
	}

	WriteMsg(TEXT("进行ftp迁移"));

	if (!TransFile())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	WriteMsg(TEXT("迁移成功"));

	

	WriteMsg(TEXT("进行注册素材"));
	if (!ClipToDB(m_strClipId))
	{
		return FALSE;
		WriteMsg(TEXT("注册素材失败，数据库访问失败"));

	}
	WriteMsg(TEXT("注册素材成功"));

	WriteMsg(TEXT("任务完成"));
	
	return TRUE;
	//
}

void CTransferClip::WriteMsg( LPCTSTR szMsgIn )
{
	CString strMsg;
	CTime tm(time(NULL));
	strMsg.Format(TEXT("[%s]  %s"), tm.Format(TEXT("%Y-%m-%d %H:%M:%S")), szMsgIn);
	m_vExcMsgs.push_back(szMsgIn);
}


BOOL CTransferClip::CheckLogicIDInDB(CString strLogicID)
{
	CString strSQL;
	strSQL.Format("select * from t_bvsid where strClipLogicID = '%s'", strLogicID);
	CTxADORecordSet rst(m_taskInfo.nExcID);
	if (rst.ExecuteSQL(strSQL))
	{
		if (rst.GetRecordCount() > 0)
		{
			CString strMsg;
			strMsg.Format(TEXT("素材逻辑ID重复, %s"), strLogicID);
			WriteMsg(strMsg);
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		CString strMsg;
		strMsg.Format(TEXT("Sql执行错误, %s"), strSQL);
		WriteMsg(strMsg);
		return TRUE;
	}
}

DWORD CTransferClip::AnalyseFTPLink( CString strSrcFile, CString &strIP, CString &strDir, CString &strFileName, CString &strUser, CString &strPass)
{
	//分析路径
	long nIndexUser = strSrcFile.Find("://") + 3;
	long nIndexPass  = strSrcFile.Find(":", nIndexUser) + 1;
	long nIndexLink = strSrcFile.Find("@") + 1;
	long nIndexFileName = strSrcFile.ReverseFind('/') +1;

	strUser = strSrcFile.Mid(nIndexUser, nIndexPass-nIndexUser-1);
	strPass = strSrcFile.Mid(nIndexPass, nIndexLink-nIndexPass-1);
	strFileName = strSrcFile.Mid(nIndexFileName, strSrcFile.GetLength()-nIndexFileName);

	CString strLink = strSrcFile.Mid(nIndexLink, strSrcFile.GetLength()-nIndexLink);

	long nIndexFTP =  strLink.Find("/");
	long nIndexDir = strLink.ReverseFind('/');
	strIP = strLink.Mid(0, nIndexFTP);
	strDir = strLink.Mid(nIndexFTP+1, nIndexDir-nIndexFTP);

	if (strIP.GetLength()<=0 || strFileName.GetLength()<=0 || strUser.GetLength()<=0 || strPass.GetLength()<=0 )
	{
		return FALSE;
	} 
	else
	{
		return TRUE;
	}
}

BOOL CTransferClip::TransFile()
{
	CString strIP, strDir, strFileName, strUser, strPass;
	if (!AnalyseFTPLink( m_vxmlTaskData[ek_ftp],strIP, strDir, strFileName, strUser, strPass))
	{
		WriteMsg(TEXT("源文件地址分析失败"));
		return FALSE;
	}
	
	CString strMsg;
	strMsg.Format(TEXT("迁移文件:源文件<%s> 目标<%s%s>"), strFileName, m_strClipId, g_GlobalInfo.strDestFilePostfix);
	CFWriteLog(strMsg);
	WriteMsg(strMsg);
	if (!TansferFile(m_strClipId, strIP, strDir, strFileName, strUser, strPass))
	{
		WriteMsg(TEXT("文件传输失败"));
		return FALSE;
	}

	return TRUE;
}


DWORD CTransferClip::TansferFile(CString strClipID, CString &strFTPSrc, CString &strDirSrc, CString &strFileNameSrc, CString &strUserNameSrc, CString &strPwSrc)
{
	BOOL bRet = FALSE;
	CInternetSession *pInetSession= new CInternetSession (NULL, 1, PRE_CONFIG_INTERNET_ACCESS);
	if (!pInetSession)
	{
		WriteMsg(TEXT("CInternetSession 创建失败"));
	}
	pInetSession->SetOption(INTERNET_OPTION_CODEPAGE , CP_UTF8);
	CFtpConnection *pFtpConnection = NULL, *pFtpConnectionDest = NULL;
	try
	{
		pFtpConnection = pInetSession->GetFtpConnection(strFTPSrc,strUserNameSrc,strPwSrc);
		pFtpConnectionDest = pInetSession->GetFtpConnection(g_GlobalInfo.szFtpIp,g_GlobalInfo.szFtpUser,g_GlobalInfo.szFtpPw);
	} 
	catch (CInternetException* e)
	{   
		WriteMsg(TEXT("ftp Connection 创建失败"));
		e->Delete();
	} 

	if (pFtpConnection!=NULL && pFtpConnectionDest!=NULL) 
	{
		//		long nFileSizeSrc = 0, nFileSizeDest = 0;  //用于简单的文件大小校验
		string szSrcDir = strDirSrc;
		wstring wszSrcDir = Ansi2W(szSrcDir);
		string szDirSrcUtf8 = W2UTF8(wszSrcDir);

		string szDesDir = g_GlobalInfo.szFtpDir;
		wstring wszDesDir = Ansi2W(szDesDir);
		string szDesDirUtf8 = W2UTF8(wszDesDir);

		string szNameSrc = strFileNameSrc;
		wstring wszNameSrc = Ansi2W(szNameSrc);
		string szNameSrcUtf8 = W2UTF8(wszNameSrc);
	
		string szNameDes = strClipID;
		szNameDes += g_GlobalInfo.strDestFilePostfix;
		wstring wszNameDes = Ansi2W(szNameDes);
		string szNameDesUtf8 = W2UTF8(wszNameDes);

		BOOL bSrcRet = TRUE;
		if (szDirSrcUtf8.size()> 0)
		{
			bSrcRet = pFtpConnection->SetCurrentDirectory(szDirSrcUtf8.c_str());

		}
		BOOL bDesRet = TRUE;
		if (szDesDirUtf8.size() > 0)
		{
			bDesRet = pFtpConnectionDest->SetCurrentDirectory(szDesDirUtf8.c_str());
		}

		INT64 nSrcFileLength = 0;
		if (bSrcRet && bDesRet)
		{
			CFtpFileFind finder(pFtpConnection);
			BOOL bFind = finder.FindFile(szNameSrcUtf8.c_str());  // 查找服务器上当前目录的任意文件 
			
			if (!bFind)   // 如果一个文件都找不到，结束查找 
			{ 
				WriteMsg(TEXT("源文件未找到！"));
			} 
			else
			{
				finder.FindNextFile();
				nSrcFileLength = finder.GetLength();
				TRACE("src file length = %I64dM", nSrcFileLength/1024/1024);
			}
			//后缀名
			finder.Close();

			CFtpFileFind finderDes(pFtpConnectionDest);
			BOOL bFindDes = finderDes.FindFile(szNameDesUtf8.c_str());
			finderDes.Close();
			if (bFindDes)
			{
				CFWriteLog(TEXT("des ftp file already existed, %s"), szNameDes.c_str());
				WriteMsg(TEXT("目标文件已存在，删除并继续"));
				pFtpConnectionDest->Remove(szNameDesUtf8.c_str());
			}


			if (bFind)
			{
				//start copy file
				CInternetFile* pFileSrc =pFtpConnection->OpenFile(szNameSrcUtf8.c_str());
				CInternetFile* pFileDes = pFtpConnectionDest->OpenFile(szNameDesUtf8.c_str(), GENERIC_WRITE);
				if (pFileSrc && pFileDes)
				{
					//start trans
					try
					{
						
						INT64 nFileTransed= 0;
						INT64 nPercent = 0;
						const INT64  nSleepPot = g_GlobalInfo.nMaxFtpSpeedPerExc*1024/100;
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
								nFileTransed += nRead;
								INT64 nNewPercent = nFileTransed*100/nSrcFileLength; 
								if (nNewPercent > nPercent)
								{
									nPercent = nNewPercent;
									if (m_pIReporter)
									{
										ST_REALTRASINFO realInfo;
										realInfo.nTaskId = m_taskInfo.nTaskId;
										realInfo.nPercent = nPercent;
										realInfo.nSpeedKBPerS = nSpeedKBPerS;
										realInfo.nSrcFileSize = nSrcFileLength;
										realInfo.nRetry = m_taskInfo.nRetry;
										realInfo.nExcId = m_taskInfo.nExcID;
										realInfo.strMsg= TEXT("正在迁移文件");
										m_pIReporter->UpdateTaskExcInfo(realInfo);
									}
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
										TRACE("\ntask%I64d, speed %dKB/s", m_taskInfo.nTaskId, nSpeedKBPerS);
										TRACE("\ntask%I64d, percent%d", m_taskInfo.nTaskId, nPercent);
									}
								}


							}
							else
							{
								if (nFileTransed >= nSrcFileLength)
								{
									//task finished.
									bRet = TRUE;
								}
								else
								{
									//
									WriteMsg(TEXT("读取源文件失败"));
									CFWriteLog(TEXT("src file write err %s"), szNameSrc.c_str());
								}
								break;
							}
						}

						//file trans finished
						if (nFileTransed >= nSrcFileLength)
						{
							bRet = TRUE;
							TRACE("\nfile trans finished");
						}
						

					}
					catch (CInternetException* e)
					{
						ASSERT(FALSE);
						CFWriteLog(TEXT("ftp file transfer error!"));
						WriteMsg(TEXT("文件传输出现异常"));
						bRet = FALSE;
					}
					catch(...)
					{
						ASSERT(FALSE);
						CFWriteLog(TEXT("ftp file transfer error!"));
						WriteMsg(TEXT("文件传输出现异常"));
						bRet = FALSE;
					}
				}
				else
				{
					ASSERT(FALSE);
					bRet = FALSE;
					CFWriteLog(TEXT("ftp文件打开失败, %s"), pFileDes? szNameSrc.c_str():szNameDes.c_str());
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
			}
		}
		else
		{
			ASSERT(FALSE);
			CString strMsg;
			strMsg.Format(TEXT("%s路径不正确"), bSrcRet? TEXT("目标"): TEXT("源"));
			WriteMsg(TEXT("strMsg"));
			CFWriteLog(0, TEXT("%s ftp path not found"), bSrcRet? szDesDir.c_str(): szSrcDir.c_str());
			bRet = FALSE;
		}
	}
	else
	{
		ASSERT(FALSE);
		WriteMsg(TEXT("ftp连接失败"));
	}

	if (pFtpConnection!=NULL) 
	{ 
		pFtpConnection->Close();
		delete pFtpConnection;
	}

	if (pFtpConnectionDest!=NULL) 
	{ 
		pFtpConnectionDest->Close();
		delete pFtpConnectionDest;
	}

	if(pInetSession!=NULL)
	{
		pInetSession->Close();
		delete pInetSession;
	}

	return bRet;
} 

//分析时间长度
CString AnalyseTime(CString strTime)
{
	if (8==strTime.GetLength())
	{
		strTime += ":00";
	}
	if (11!=strTime.GetLength())
	{
		return "00:00:00:00";
	}
	else
	{
		return strTime;
	}
}

long Duration2Frame(CString strDuration)
{
	CString strHour, strMin, strSec, strFram;

	strHour = strDuration.Mid(0, 2);
	strMin = strDuration.Mid(3, 2);
	strSec = strDuration.Mid(6, 2);
	strFram = strDuration.Mid(9, 2);

	long nHour, nMin, nSec, nFram, nResult;

	nHour = atoi(strHour);
	nMin = atoi(strMin);
	nSec = atoi(strSec);
	nFram = atoi(strFram);

	nResult = ((nHour*60+nMin)*60+nSec)*25+nFram;

	return nResult;
}

CString Fram2Duration(long nFrame)
{
	CString strDuraiont = "00:00:00:00";

	long nHour, nMin, nSec, nFram;

	nFram = nFrame%25;
	nFrame /= 25;

	nSec = nFrame%60;
	nFrame /= 60;

	nMin = nFrame%60;
	nFrame /= 60;

	nHour = nFrame;

	if (nHour<100)
	{
		strDuraiont.Format("%02d:%02d:%02d:%02d", nHour, nMin, nSec, nFram);
	}


	return strDuraiont;
}

CString DurationToEOM(CString strDuraion)
{
	long nFrame = Duration2Frame(strDuraion);
	return Fram2Duration(nFrame-1);
}

DWORD CTransferClip::ClipToDB(CString strClipID)
{
	long nClipType = 0;
	map<CString, long>::iterator itf = g_GlobalInfo.m_clipTypeMap.find(m_vxmlTaskData[ek_type]);
	if (itf != g_GlobalInfo.m_clipTypeMap.end())
	{
		nClipType = itf->second;
	}

	CString strHoldDays;
	if (g_GlobalInfo.nHoldDay==0)
	{
		strHoldDays = m_vxmlTaskData[ek_holdTime];
	}
	else
	{
		CString strEndTime = m_vxmlTaskData[ek_strBroadTime];
		long nY, nM, nD;
		 sscanf_s(strEndTime, "%d-%d-%d", &nY, &nM, &nD);
		CTime cTime(nY, nM, nD, 0, 0, 0);
		CTimeSpan span(g_GlobalInfo.nHoldDay, 0, 0, 0);
		cTime += span;
		strHoldDays = cTime.Format("%Y-%m-%d");
	}

	vector<CString> vSqls;
	CString strSQL;
	strSQL.Format("insert into t_bvsid (strClipID,strClipLogicID,strClipName,strSOM,strEOM,strDuration,strLSOM,strLEOM,strLDuration,strLocation,strChnID,\
				  nBitRate,nDelete,nReady,nCheck,nClipType,tPlanAirTime,tPlanLastAirTime,tDubDateTime,strDubBy ) \
				  values ('%s','%s','%s','%s','%s',\
				  '%s','%s','%s','%s','%s',\
				  '%s','%d','%d','%d','%d',\
				  '%d','%s','%s','%s','%s' )", 
				  strClipID, m_vxmlTaskData[ek_packId],m_vxmlTaskData[ek_ClipName],"00:00:00:00",DurationToEOM(m_vxmlTaskData[ek_duration]),
				  m_vxmlTaskData[ek_duration],"00:00:00:00",DurationToEOM(m_vxmlTaskData[ek_duration]),m_vxmlTaskData[ek_duration], g_GlobalInfo.strLocation, 
				  g_GlobalInfo.strChannleID,0 ,0, 1, 0, 
				  nClipType,m_vxmlTaskData[ek_strBroadTime],strHoldDays,m_vxmlTaskData[ek_strBroadTime],"歌华迁移");
	vSqls.push_back(strSQL);

	strSQL.Format(TEXT("delete from t_locationinfo where strStorageName = '%s' and strClipID = '%s'"), g_GlobalInfo.strStorageName, strClipID);
	vSqls.push_back(strSQL);


	strSQL.Format("insert into t_locationinfo (strStorageName, strMediaType, strClipID,strClipLogicID, nBitRate,nDelete,nReady,nCheck,tCreateTime, \
				  strFileSize, strPath, nAFDType, nAfdOpResult) values ('%s','%s','%s','%s','%d','%d','%d','%d','%s','%s','%s','%d','%d' )\
				  ",g_GlobalInfo.strStorageName, g_GlobalInfo.strMediaType,strClipID, m_vxmlTaskData[ek_packId],0 ,0, 1, 0,m_vxmlTaskData[ek_strBroadTime], m_vxmlTaskData[ek_duration], "", 0, 1);
	vSqls.push_back(strSQL);

	CTxADOCommand rst(m_taskInfo.nExcID);
	if (!rst.ExecuteSQLs(vSqls))
	{
		WriteMsg(TEXT("注册素材到数据库失败"));
		return FALSE;
	}
	return TRUE;
}

BOOL CTransferClip::Start()
{
	if (!Run())
	{
		m_taskInfo.nRunState = TaskRunState_error;
	}
	else
	{
		m_taskInfo.nRunState = TaskRunState_finished;

	}
	//report task
	if (m_pIReporter)
	{
		m_pIReporter->ReportTask(m_taskInfo, m_vExcMsgs);
	}

	return TRUE;
}

BOOL CTransferClip::Init( ST_TASKINFO& taskInfo , HANDLE hEventQuit)
{
	m_taskInfo = taskInfo;
	m_hEventQuit = hEventQuit;
	return TRUE;
}



