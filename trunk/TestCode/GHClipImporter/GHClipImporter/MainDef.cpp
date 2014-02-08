#include "StdAfx.h"
#include "MainDef.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "io.h"
#include "TxParamString.h"
#include "TxADOConn.h"

ST_GLOBAL g_GlobalInfo;

/*
*Description：初始化配置文件
*Input Param：
*Return Param：返回成功或失败
*History：
*/
BOOL InitGlobalConfig()
{
	//策略管理配置文件
	g_GlobalInfo.szAppPath = GetAppPath().c_str();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\GHClipImporter.ini");
	//日志配置文件
	g_GlobalInfo.szLogFile	 = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szLogFile	 += TEXT("\\Log");
	CreateDirectory(g_GlobalInfo.szLogFile, NULL);
	g_GlobalInfo.szLogFile	 += TEXT("\\GHClipImporter.log");
	if (_access(g_GlobalInfo.szIniPath, 04) == -1)
	{
		CFWriteLog("找不到配置文件GHClipImporter.ini！");
		return FALSE;
	}

	char szbuff[1024];
	ZeroMemory(szbuff, sizeof(szbuff));
	GetPrivateProfileString("config", "db", "", szbuff, 1024, g_GlobalInfo.szIniPath);
	g_GlobalInfo.szDBConnString = szbuff;
	if (g_GlobalInfo.szDBConnString.IsEmpty())
	{
		CFWriteLog(TEXT("数据库连接字符串错误"));
		return FALSE;
	}
	g_GlobalInfo.nScanTimePos = GetPrivateProfileInt("config", "nScanTimePos", 1, g_GlobalInfo.szIniPath);

	g_GlobalInfo.nAutoCheckInterval = GetPrivateProfileInt("config", "AutoCheckInterval", 10*60, g_GlobalInfo.szIniPath);
	TCHAR sztmpBuff[MAX_PATH+2];
	GetPrivateProfileString("config","DestFTP","",sztmpBuff,MAX_PATH,g_GlobalInfo.szIniPath);
	g_GlobalInfo.szFtpIp = sztmpBuff;
	GetPrivateProfileString("config","DestUser","",sztmpBuff,MAX_PATH,g_GlobalInfo.szIniPath);
	g_GlobalInfo.szFtpUser = sztmpBuff;
	GetPrivateProfileString("config","DestPass","",sztmpBuff,MAX_PATH,g_GlobalInfo.szIniPath);
	g_GlobalInfo.szFtpPw = sztmpBuff;
	GetPrivateProfileString("config","DestDir","",sztmpBuff,MAX_PATH,g_GlobalInfo.szIniPath);
	g_GlobalInfo.szFtpDir = sztmpBuff;
	GetPrivateProfileString("config","ScanDir","",sztmpBuff,MAX_PATH,g_GlobalInfo.szIniPath);
	g_GlobalInfo.szScanDir = sztmpBuff;
	GetPrivateProfileString("config","WorkDir","",sztmpBuff,MAX_PATH,g_GlobalInfo.szIniPath);
	g_GlobalInfo.szWorkDir = sztmpBuff;

	g_GlobalInfo.nMaxSession = GetPrivateProfileInt("config", "MaxSession", 3, g_GlobalInfo.szIniPath);
	g_GlobalInfo.nMaxRetry = GetPrivateProfileInt("config", "MaxRetry", 1, g_GlobalInfo.szIniPath);

	
	if (g_GlobalInfo.szFtpIp.IsEmpty() || g_GlobalInfo.szFtpUser.IsEmpty() || g_GlobalInfo.szWorkDir.IsEmpty()
		||g_GlobalInfo.szScanDir.IsEmpty())
	{
		CFWriteLog(0, TEXT("配置文件不正确，请检查"));
	}

	if (_access(g_GlobalInfo.szScanDir, 00) ==-1)
	{
		CFWriteLog(0, TEXT("扫描目录无法访问！%s"), g_GlobalInfo.szScanDir);
		return FALSE;
	}

	CreateDirectory(g_GlobalInfo.szWorkDir, NULL);
	if (_access(g_GlobalInfo.szWorkDir, 00) == -1)
	{
		CFWriteLog(0, TEXT("工作目录无法访问！ %s"), g_GlobalInfo.szWorkDir);
	}

	g_GlobalInfo.szWorkErrDir = g_GlobalInfo.szWorkDir + DIR_ERRORTASK;
	CreateDirectory(g_GlobalInfo.szWorkErrDir, NULL);
	g_GlobalInfo.szWorkFinishedDir = g_GlobalInfo.szWorkDir + DIR_FINISHEDTASK;
	CreateDirectory(g_GlobalInfo.szWorkFinishedDir, NULL);

	GetPrivateProfileString("config","strLocation","1000000000",sztmpBuff,MAX_PATH,g_GlobalInfo.szIniPath);
	g_GlobalInfo.strLocation = sztmpBuff;

	GetPrivateProfileString("config","strChannleID","CHN01",sztmpBuff,MAX_PATH,g_GlobalInfo.szIniPath);
	g_GlobalInfo.strChannleID =sztmpBuff;

	GetPrivateProfileString("config","strStorageName","SECOND",sztmpBuff,MAX_PATH,g_GlobalInfo.szIniPath);
	g_GlobalInfo.strStorageName =sztmpBuff;
	
	GetPrivateProfileString("config","strMediaType","FMS",sztmpBuff,MAX_PATH,g_GlobalInfo.szIniPath);

	g_GlobalInfo.strMediaType =sztmpBuff;
	
	g_GlobalInfo.nHoldDay = GetPrivateProfileInt("config","nHoldDay",  0, g_GlobalInfo.szIniPath);
	
	GetPrivateProfileString("config","strDestFilePostfix",".mpg",sztmpBuff,MAX_PATH,g_GlobalInfo.szIniPath);

	g_GlobalInfo.strDestFilePostfix =sztmpBuff;


	g_GlobalInfo.nMaxFtpSpeedPerExc = GetPrivateProfileInt("config","nMaxFtpSpeedPerExc",  40000, g_GlobalInfo.szIniPath);

	return TRUE;
}

BOOL IsSameDirStarted()
{

	CString strFullFileName = g_GlobalInfo.szAppPath;

	strFullFileName.Replace('\\','.');

	HANDLE hInstance = ::CreateMutex(NULL,TRUE,strFullFileName);
	DWORD error = GetLastError();

	TRACE(_T("the filename = %s\n"), strFullFileName);

	if(error == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hInstance);

		return TRUE;
	}
	return FALSE;
}

CString TxGetFileName(const CString& strFileIn)
{
	CString strRet;
	int nPos = strFileIn.ReverseFind(TEXT('\\'));
	if (nPos != -1)
	{
		strRet = strFileIn.Mid(nPos+1, strFileIn.GetLength() - nPos-1);
	}
	return strRet;
}

CString TxGetFileTitle(const CString& strFileIn )
{
	CString strRet;
	int nPos = strFileIn.ReverseFind(TEXT('\\'));
	int nPos2 = strFileIn.ReverseFind(TEXT('.'));
	if (nPos != -1 && nPos2 != -1 && nPos2 > nPos)
	{
		strRet = strFileIn.Mid(nPos+1, nPos2 - nPos-1);
	}
	return strRet;
}

BOOL RefreshClipType()
{
	g_GlobalInfo.m_clipTypeMap.clear();

	CString strSQL;
	CTxADORecordSet rst;
	strSQL.Format("select ncliptypeid,strcliptypeName from t_cliptype");
	if (rst.ExecuteSQL(strSQL))
	{
		while (!rst.IsEOF())
		{
			CString strName = rst.GetVal("strClipTypeName").GetAsString();
			long nClipTypeID = rst.GetVal("nClipTypeID").GetAsInt(-1);
			g_GlobalInfo.m_clipTypeMap[strName] = nClipTypeID;
			rst.MoveNext();
		}
	}
	else
	{
		CFWriteLog(TEXT("素材类型获取失败"));
		return FALSE;
	}

	return g_GlobalInfo.m_clipTypeMap.size() > 0;
	
}

CString TxGetFtpFileName( const CString& strFileIn )
{
	CString strRet;
	int nPos = strFileIn.ReverseFind(TEXT('\\'));
	if (nPos != -1)
	{
		strRet = strFileIn.Mid(nPos+1, strFileIn.GetLength() - nPos-1);
	}
	return strRet;
}

BOOL GetTaskInFolder( CString strFolderIn, MMAPTIMETASKS& mmapOut )
{
	CFileFind finder;
	// build a string with wildcards
	CString strWildcard(strFolderIn);
	strWildcard.TrimRight(TEXT("\\"));
	strWildcard +=TEXT("\\*.log");

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if (finder.IsDots())
			continue;
		// if it's a directory, recursively search it
		if (finder.IsDirectory() )
		{
			continue;
		}

		CString strFileTmp = finder.GetFilePath();
		CTime tmLastModify;
		finder.GetLastWriteTime(tmLastModify);
		mmapOut.insert(pair<INT64, CString>(tmLastModify.GetTime(), strFileTmp));
	}

	finder.Close();

	return TRUE;
}



