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
	GUID guid;

	CString strHostName;
	CString szExcutorPath;
	ST_EXCUTORREG excInfo;

	ST_GLOBAL()
	{
		guid = GUID_NULL;
	}


};

BOOL InitGlobalConfig();
BOOL LoadActorHost(CString& strConfig);
BOOL UnloadActorHost();

BOOL LoadPluginManager();
BOOL UnLoadPluginManager();

extern ST_GLOBAL g_GlobalInfo;

extern HMODULE g_hActorPlugin;
extern EMB::IPluginBaseInterface* g_pIActorPlugin;

extern HMODULE g_hModulePluginMgr;
extern EMB::IPluginBaseInterface* g_pIPluginMgr;