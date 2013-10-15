#pragma once
#include <string>
#include <vector>
#include "MBCCommonDef.h"


struct ST_MBCOBJCFG
{
	SOCKADDR_IN addr;
	SOCKADDR_IN localBindAddr;
	std::string szChId;
	std::string szName;
	ST_MBCOBJCFG()
	{
		localBindAddr.sin_family = AF_INET;
		localBindAddr.sin_port = htons(0);
		localBindAddr.sin_addr.S_un.S_addr = inet_addr( INADDR_ANY );
	}
};

struct ST_GLOBAL
{
	std::string szAppPath;
	std::string szIniPath;
	std::string szCfgSrcPath;

	SOCKADDR_IN addrMasterRemote;
	SOCKADDR_IN addrMasterLocal;
	SOCKADDR_IN addrBackRemote;
	SOCKADDR_IN addrBackLocal;
	std::string szLocalWatchAppPath;

	int nStateGetStartDelay;
	int nSockDownReconnDelay;
	int nRestartLocalAppCD;
	int nStateGetInterval;
	int nAutoSwitchCriticalCount;
	ST_GLOBAL()
	{
		nStateGetInterval = 500;
		nAutoSwitchCriticalCount= 3000;
		nRestartLocalAppCD = 5000;
		nStateGetStartDelay = 1000;
		nSockDownReconnDelay = 3000;
	}

};

BOOL InitGlobalConfig();

extern ST_GLOBAL g_GlobalInfo;
extern HWND g_hFrame;

class CMainFrame;
extern CMainFrame* g_pMainFrame;