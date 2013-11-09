#pragma once
#include "TxLogManager.h"
#include "IEMBBaseInterface.h"
#include "embstructdef.h"

struct ST_GLOBAL
{
	CString szAppPath;
	CString szIniPath;
	CString szCfgSrcPath;
	CString szPluginPath;
	CString szLogFile;
	CString strGuid;
	EXCUTORID guid;
	HWND hwndActor;
	ST_EXCUTORREG excInfo;

	ST_GLOBAL()
	{
		hwndActor = NULL;
		
		guid = INVALID_ID;
	}


};

BOOL InitGlobalConfig();
BOOL LoadPluginManager();
BOOL UnLoadPluginManager();

extern ST_GLOBAL g_GlobalInfo;
extern HMODULE g_hModulePluginMgr;
extern EMB::IPluginBaseInterface* g_pIPluginMgr;