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
	//check arg
	int nArgC = __argc;
	if (nArgC != 2)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CString strHwnd = __argv[1];
	g_GlobalInfo.excInfo.FromString(strHwnd);

	if (!::IsWindow(g_GlobalInfo.excInfo.hwndActor))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (g_GlobalInfo.excInfo.guid == INVALID_ID)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	g_GlobalInfo.szAppPath = GetAppPath().c_str();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\EMBExcutor.ini");
	g_GlobalInfo.szLogFile	 = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szLogFile	 += TEXT("\\Log");
	CreateDirectory(g_GlobalInfo.szLogFile, NULL);
	g_GlobalInfo.szLogFile	 += TEXT("\\ExcutorLog.log");

	g_GlobalInfo.szPluginPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szPluginPath +=TEXT("\\Plugin");
	if (_access(g_GlobalInfo.szIniPath, 04) == -1)
	{
		//CFWriteLog("找不到配置文件Excutor.ini！");
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
	
	TxLoadPlugin(strFile, g_hModulePluginMgr, (LPVOID&)g_pIPluginMgr);
	return (g_hModulePluginMgr != NULL && g_pIPluginMgr != NULL);
}

BOOL UnLoadPluginManager()
{
	if (g_pIPluginMgr)
	{
		g_pIPluginMgr->Release();
		g_pIPluginMgr = NULL;
	}
	if (g_hModulePluginMgr)
	{
		FreeLibrary(g_hModulePluginMgr);
		g_hModulePluginMgr = NULL;
	}
	return TRUE;
}
