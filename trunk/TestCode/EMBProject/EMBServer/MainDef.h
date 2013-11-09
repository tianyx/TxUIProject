#pragma once
#include "TxLogManager.h"
#include "IEMBBaseInterface.h"
#include <vector>
#include "EmbStructDef.h"
using namespace std;


struct ST_GLOBAL
{
	CString szAppPath;
	CString szIniPath;
	CString szCfgSrcPath;
	CString szPluginPath;
	CString szLogFile;
	VECLOADEDPLUGINS vPlugins;
	
	ST_GLOBAL()
	{
		
	}


};

extern ST_GLOBAL g_GlobalInfo;
extern HMODULE g_hModulePluginMgr;
extern EMB::IPluginBaseInterface* g_pIPluginMgr;

BOOL InitGlobalConfig();
BOOL LoadPluginManager();
BOOL UnLoadPluginManager();


BOOL InitServer();
BOOL UnInitServer();
BOOL RunServer(BOOL bRun = TRUE);
