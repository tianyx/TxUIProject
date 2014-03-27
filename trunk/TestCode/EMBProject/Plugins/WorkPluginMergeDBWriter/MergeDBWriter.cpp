#include "StdAfx.h"
#include "MergeDBWriter.h"
#include "TxLogManager.h"
#include "TxADOConn.h"
#include "io.h"
using namespace EMB;

DWORD __stdcall TaskRunProc(LPVOID lparam)
{
	CMergeDBWriter* pObj = (CMergeDBWriter*)lparam;
	if (pObj)
	{
		pObj->RunTaskLoop();
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
CMergeDBWriter::CMergeDBWriter(void)
{
	m_hThreadTask = NULL;
	m_pReportCallback = NULL;
	m_nPercent = 0;
	m_nMaxSplitCount = 100;
}

CMergeDBWriter::~CMergeDBWriter(void)
{
}


HRESULT EMB::CMergeDBWriter::QueryPluginInfo( VECPLUGINFOS& vInfoOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_PWorkerMergeDBWriter;
	info.nPlugInType = PluginType_Wroker;
	info.nSubType = SubType_MergeDBWriter;
	vInfoOut.push_back(info);
	return S_OK;

}

HRESULT EMB::CMergeDBWriter::DoTask( const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback )
{
	if (m_hThreadTask)
	{
		ASSERT(FALSE);
		return EMBERR_WORKERBUSY;
	}

	ST_MERGETODBWRITER info;
	info.FromString(szTaskIn);
	if (info.strDBConn.IsEmpty()
		|| info.strTaskId.IsEmpty()
		||info.strTaskPath.IsEmpty())
	{
		ASSERT(FALSE);
		return EMBERR_INVALIDARG;
	}
	
	m_taskInfo = info;
	m_strTask = szTaskIn;
	m_pReportCallback = pICallback;
	m_hThreadTask = CreateThread(NULL, 0, TaskRunProc, (LPVOID)this, 0, 0);
	return S_OK;
}

HRESULT EMB::CMergeDBWriter::CancelTask()
{
	return S_OK;
}

HRESULT EMB::CMergeDBWriter::GetTaskProgress( CEMBWorkString& szInfo )
{
	ST_WORKERREPORT report;
	report.nPercent = m_nPercent;
	CString strRet;
	report.ToString(strRet);
	szInfo = strRet;
	return S_OK;
}

HRESULT EMB::CMergeDBWriter::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
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

HRESULT EMB::CMergeDBWriter::OnFirstInit()
{
	GetTxLogMgr()->AddNewLogFile(LOGKEY_MERGEDBWRITER, TEXT("MergeDBwriter"));

	return S_OK;
}

void EMB::CMergeDBWriter::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	ReleaseTxLogMgr();
	TRACE("\nCWorkSample::OnFinalRelease() ");

	delete this;
}

BOOL EMB::CMergeDBWriter::RunTaskLoop()
{
	//
	CFWriteLog(NULL, TEXT("work started!!....."));

	ST_WORKERREPORT report;

	m_taskInfo.strTaskPath.TrimRight("\\");
	if (_access(m_taskInfo.strTaskPath, 00) == -1)
	{
		report.code = EMBERR_FILEACCESS;
		CFWriteLog(TEXT("file path not existed! %s"), m_taskInfo.strTaskPath);
	}

	CFWriteLog(NULL, TEXT("start write merge result [%s] to db!!....."), m_taskInfo.strTaskId);
	BOOL bRet = CTxADODBMgr::GetADODBMgr()->OpenDB(1, m_taskInfo.strDBConn);
	if (bRet)
	{
		
		CString strTaskFile = m_taskInfo.strTaskPath;
		strTaskFile += TEXT("\\");
		strTaskFile += m_taskInfo.strTaskId;
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
				strSql.Format(TEXT("delete from T_FCVSFileInfo where strTaskID = '%s'"), m_taskInfo.strTaskId);
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
		else
		{
			CString strTaskPre = m_taskInfo.strTaskPath;
			strTaskPre += TEXT("\\");
			strTaskPre += m_taskInfo.strTaskId;
			for (size_t i = 0; i < m_nMaxSplitCount; ++i)
			{
				CString strTaskSplitFile;
				strTaskSplitFile.Format(TEXT("%s_wow%d.sql"),strTaskPre, i+1);
				if (_access(strTaskSplitFile, 04) == -1)
				{
					CFWriteLog(TEXT("split file can't access %s"), strTaskSplitFile);
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
								strSql.Format(TEXT("delete from T_FCVSFileInfo where strTaskID = '%s'"), m_taskInfo.strTaskId);
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
								CFWriteLog(TEXT("文件执行失败%s"), strTaskSplitFile);
							}
							
						}
						else
						{
							CFWriteLog(TEXT("file no content to exec, %s"), strTaskSplitFile);
						}

						fileSp.Close();
						//delete sql file whatever
						DeleteFile(strTaskSplitFile);


					}
				}
			}
			
		}
		
		m_nPercent = 100;

	}
	else
	{
		report.code = EMBERR_DBOPEN;
	}

	CTxADODBMgr::ReleaseDBMgr();

	if (m_pReportCallback)
	{
		CString strRet;
		report.ToString(strRet);
		m_pReportCallback->OnDllReportTaskProgress(strRet);
	}

	CFWriteLog(NULL, TEXT("work end!!....."));
	m_hThreadTask = NULL;

	return TRUE;

}