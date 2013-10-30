#pragma once
#include "TxLogManager.h"
#include "IEMBBaseInterface.h"

struct ST_GLOBAL
{
	CString szAppPath;
	CString szIniPath;
	CString szCfgSrcPath;
	CString szPluginPath;
	CString szLogFile;

	ST_GLOBAL()
	{
		
	}


};

BOOL InitGlobalConfig();
BOOL LoadPluginManager();

extern ST_GLOBAL g_GlobalInfo;
extern HMODULE g_hModulePluginMgr;
extern EMB::IPluginBaseInterface* g_pIPluginMgr;