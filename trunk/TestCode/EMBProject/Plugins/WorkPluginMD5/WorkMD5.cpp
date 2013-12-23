#include "StdAfx.h"
#include "WorkMD5.h"
#include "StdAfx.h"
#include "TxParamString.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "io.h"


//////////////////////////////////////////////////////////////////////////
using namespace EMB;

DWORD __stdcall TaskRunProc(LPVOID lparam)
{
	CWorkMD5* pObj = (CWorkMD5*)lparam;
	if (pObj)
	{
		pObj->RunTaskLoop();
	}

	return 0;
}
//////////////////////////////////////////////////////////////////////////
CWorkMD5::CWorkMD5(void)
{
	m_hThreadTask = NULL;
	m_pReportCallback = NULL;
	m_nPercent = 0;
}

CWorkMD5::~CWorkMD5(void)
{
}


HRESULT EMB::CWorkMD5::QueryPluginInfo( VECPLUGINFOS& vInfoOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_PWorkerMD5;
	info.nPlugInType = PluginType_Wroker;
	info.nSubType = SubType_MD5Check;
	vInfoOut.push_back(info);
	return S_OK;

}

HRESULT EMB::CWorkMD5::DoTask( const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback )
{
	if (m_hThreadTask)
	{
		ASSERT(FALSE);
		return EMBERR_WORKERBUSY;
	}

	ST_MD5TASKINFO info;
	info.FromString(szTaskIn);
	if (info.strFileToCheck.IsEmpty())
	{
		ASSERT(FALSE);
		return EMBERR_INVALIDARG;
	}
	if (_access(info.strFileToCheck, 04) == -1)
	{
		ASSERT(FALSE);
		return EMBERR_INVALIDARG;
	}
	
	if (info.strFileToWriteResult.IsEmpty())
	{
		//create new
		info.strFileToWriteResult = info.strFileToCheck;
		info.strFileToWriteResult += TEXT(".md5");
	}
	CFile filetmp;
	if (filetmp.Open(info.strFileToWriteResult, CFile::modeWrite|CFile::modeCreate, NULL))
	{
		filetmp.Close();
	}
	else
	{
		ASSERT(FALSE);
		return EMBERR_FILEACCESS;
	}

	m_taskInfo = info;
	m_strTask = szTaskIn;
	m_pReportCallback = pICallback;
	m_hThreadTask = CreateThread(NULL, 0, TaskRunProc, (LPVOID)this, 0, 0);
	return S_OK;
}

HRESULT EMB::CWorkMD5::CancelTask()
{
	return S_OK;
}

HRESULT EMB::CWorkMD5::GetTaskProgress( CEMBWorkString& szInfo )
{
	m_nPercent = m_MD5Computer.GetProgress();
	ST_WORKERREPORT report;
	report.nPercent = m_nPercent;
	CString strRet;
	report.ToString(strRet);
	szInfo = strRet;
	return S_OK;
}

HRESULT EMB::CWorkMD5::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
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

HRESULT EMB::CWorkMD5::OnFirstInit()
{
	GetTxLogMgr()->AddNewLogFile(LOGKEY_MD5, TEXT("md5"));

	return S_OK;
}

void EMB::CWorkMD5::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	ReleaseTxLogMgr();
	TRACE("\nCWorkSample::OnFinalRelease() ");

	delete this;
}

BOOL EMB::CWorkMD5::RunTaskLoop()
{
	//
	CFWriteLog(NULL, TEXT("work started!!....."));

	ST_WORKERREPORT report;
	CFWriteLog(NULL, TEXT("start md5 check %s!!....."), m_taskInfo.strFileToCheck);

	CString strRetMD5 = ComputeMD5(m_taskInfo.strFileToCheck);
	if (!strRetMD5.IsEmpty())
	{
		CFWriteLog(NULL, TEXT("start write md5 info to %s!!....."), m_taskInfo.strFileToCheck);

		if(WriteMD5File(strRetMD5, m_taskInfo.strFileToWriteResult))
		{
			if (!m_taskInfo.strMD5Compare.IsEmpty()
				&& m_taskInfo.strMD5Compare.Compare(strRetMD5) != 0)
			{
					//md5 not match!!!
					report.code = EMBERR_NOTMATCH;
					CFWriteLog(0, TEXT("calc md5 not match with orgin md5, file =%s comp =%s, cal =%s"), m_taskInfo.strFileToCheck, m_taskInfo.strMD5Compare, strRetMD5);
			}
			else
			{
				report.code = S_OK;
				report.nPercent = 100;
			}
		}
		else
		{
			report.code = EMBERR_FILEACCESS;
		}

	}
	else
	{
		report.code = EMBERR_SUBTASKERR;
	}
	
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

CString EMB::CWorkMD5::ComputeMD5(const CString& strFilePath)
{

		CTime t = CTime::GetCurrentTime();

		char szCurrentFile[2048];
		memset( szCurrentFile, 0, 1024 );
		lstrcpy(szCurrentFile,strFilePath);
		char ucMD5Ret[16];
		memset( ucMD5Ret, 0, 16 );
		char strClipMD5[33];
		memset( strClipMD5, 0, 32 );


		m_MD5Computer.MD5EncryptFile(szCurrentFile,ucMD5Ret);
		for (int i = 0; i < 16; i++)
		{
			char cmd5[3];
			sprintf(cmd5,"%02x",(BYTE)ucMD5Ret[i]);
			strcat(strClipMD5,cmd5);
			TRACE("%02x", (BYTE)ucMD5Ret[i]);
		}  
		CString tmpmd5 = strClipMD5;
		t = CTime::GetCurrentTime();
		return tmpmd5;

}

BOOL EMB::CWorkMD5::WriteMD5File(const CString& strMD5In, const CString& strMd5FileIn)
{
	CString strMd5= strMD5In;
	CString strMd5File = strMd5FileIn;
	try{
		CFile file;
		if(file.Open(strMd5File,CFile::modeCreate|CFile::modeWrite,NULL) == 0)
			return false;

		file.Write(strMd5.LockBuffer(),strMd5.GetLength());
		file.Close();
	}
	catch(...)
	{
		return FALSE;
	}
	return TRUE;
}


