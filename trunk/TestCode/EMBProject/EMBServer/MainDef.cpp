#include "StdAfx.h"
#include "MainDef.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "io.h"
#include "EMBCommonFunc.h"

ST_GLOBAL g_GlobalInfo;
HMODULE g_hModulePluginMgr = NULL;
EMB::IPluginBaseInterface* g_pIPluginMgr = NULL;
BOOL InitGlobalConfig()
{
	g_GlobalInfo.szAppPath = GetAppPath().c_str();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\EMBServer.ini");
	g_GlobalInfo.szLogFile	 = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szLogFile	 += TEXT("\\Log");
	CreateDirectory(g_GlobalInfo.szLogFile, NULL);
	g_GlobalInfo.szLogFile	 += TEXT("\\MainLog.log");

	g_GlobalInfo.szPluginPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szPluginPath +=TEXT("\\Plugin");
	if (_access(g_GlobalInfo.szIniPath, 04) == -1)
	{
		CFWriteLog("找不到配置文件EMBServer.ini！");
		return FALSE;
	}

	return TRUE;
}

BOOL LoadPluginManager()
{
	CString strFile = g_GlobalInfo.szAppPath;
	strFile +=TEXT("\\plugins\\EMBPluginMgr.dll");
	if (_access(strFile, 0) == -1)
	{
		ASSERT(FALSE);
		CFWriteLog(LOGKEYMAIN, TEXT("EMBPluginMgr.dll not Found"));
		return FALSE;
	}
	
	LoadPlugin(strFile, g_hModulePluginMgr, (LPVOID&)g_pIPluginMgr);
	return (g_hModulePluginMgr != NULL && g_pIPluginMgr != NULL);
}
