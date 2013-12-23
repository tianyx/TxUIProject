#include "StdAfx.h"
#include "MainDef.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "io.h"
#include "EMBCommonFunc.h"
#include "TxAutoComPtr.h"
#include "IEMBBaseInterface.h"

ST_GLOBAL g_GlobalInfo;
HMODULE g_hModulePluginMgr = NULL;
EMB::IPluginBaseInterface* g_pIPluginMgr = NULL;
BOOL InitGlobalConfig()
{
	//check arg
#ifdef EXCSELFTEST
	g_GlobalInfo.excInfo.actorId = 1;
	g_GlobalInfo.excInfo.guid = 1;
	g_GlobalInfo.excInfo.hwndActor = GetDesktopWindow();
#else

	int nArgC = __argc;
	if (nArgC != 2)
	{
		ASSERT(FALSE);
		//to debug launch a executor, set follow param in project setting->debug->running paramepter
		//"<excutorreg actorId=""1"" guid=""1"" hwndActor=""0"" hwndExcutor=""0""/>"
		return FALSE;
	}

	CString strParam = __argv[1];
	g_GlobalInfo.excInfo.FromString(strParam);
	if (g_GlobalInfo.excInfo.hwndActor == 0)
	{
		//debug lunch mode
		g_GlobalInfo.excInfo.hwndActor = ::GetDesktopWindow();
	}
	if (!::IsWindow(g_GlobalInfo.excInfo.hwndActor))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (g_GlobalInfo.excInfo.guid == INVALID_ID
		||g_GlobalInfo.excInfo.actorId == INVALID_ID)
	{
		ASSERT(FALSE);
		return FALSE;
	}
#endif // _DEBUG


	g_GlobalInfo.strGuid.Format(TEXT("excutor%d_actor%d"), g_GlobalInfo.excInfo.guid, g_GlobalInfo.excInfo.actorId);

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
		//return FALSE;
	}

	return TRUE;
}

BOOL LoadPluginManager()
{
	CString strFile = g_GlobalInfo.szAppPath;
	strFile +=TEXT("\\EMBPluginMgr.epl");
	if (_access(strFile, 0) == -1)
	{
		ASSERT(FALSE);
		CFWriteLog(LOGKEYMAIN, TEXT("EMBPluginMgr.epl not Found"));
		return FALSE;
	}
	
	TxLoadPlugin(strFile, g_hModulePluginMgr, (LPVOID&)g_pIPluginMgr);
	if (g_pIPluginMgr)
	{
		CTxAutoComPtr<EMB::IPluginManagerInterface> apPluginMgr;
		g_pIPluginMgr->QueryInterface(GuidEMBPlugin_IPluginManager, (LPVOID&) *&apPluginMgr);
		if (apPluginMgr)
		{
			apPluginMgr->InitPluginsSearch(TRUE, NULL);
		}

	}
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
