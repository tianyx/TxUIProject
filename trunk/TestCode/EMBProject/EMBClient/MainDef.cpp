#include "StdAfx.h"
#include "MainDef.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "io.h"
#include "EmbStructDef.h"

ST_GLOBAL g_GlobalInfo;
BOOL InitGlobalConfig()
{
		
	g_GlobalInfo.szAppPath = GetAppPath().c_str();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\EMBClient.xml");
	g_GlobalInfo.szLogFile	 = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szLogFile	 += TEXT("\\Log");
	CreateDirectory(g_GlobalInfo.szLogFile, NULL);
	g_GlobalInfo.szLogFile	 += TEXT("\\EMBClientLog.log");

	g_GlobalInfo.szPluginPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szPluginPath +=TEXT("\\Plugin");
	if (_access(g_GlobalInfo.szIniPath, 04) == -1)
	{
		CFWriteLog(0, TEXT("找不到配置文件EMBClient.xml！"));
		return FALSE;
	}

	CFile file;
	CString strXml;
	if (file.Open(g_GlobalInfo.szIniPath, CFile::modeRead, NULL))
	{
		int nFileLen = file.GetLength();
		if (nFileLen == 0)
		{
			file.Close();
			ASSERT(FALSE);
			return FALSE;
		}
		char* pbuff = new char[nFileLen+1];
		ZeroMemory(pbuff, nFileLen +1);
		int nSize = file.Read(pbuff, nFileLen);
		ASSERT(nSize == nFileLen);
		strXml = pbuff;
		file.Close();
		delete[] pbuff;
	}

	g_GlobalInfo.uiclientCfg.FromString(strXml);
	if (g_GlobalInfo.uiclientCfg.addrMain.sin_family != AF_INET
		|| g_GlobalInfo.uiclientCfg.addrSlave.sin_family != AF_INET)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}


