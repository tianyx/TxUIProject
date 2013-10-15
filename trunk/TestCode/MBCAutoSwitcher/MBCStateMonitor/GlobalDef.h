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

	SOCKADDR_IN addrRemote;
	SOCKADDR_IN addrLocal;
	//graphconfig

	int nStateGetInterval;
	int nStateGetStartDelay;
	int nSockDownReconnDelay;

	ST_GLOBAL()
	{
		nStateGetInterval = 1000;
		nStateGetStartDelay= 2000;
		nSockDownReconnDelay = 5000;
	}

};

BOOL InitGlobalConfig();

extern ST_GLOBAL g_GlobalInfo;
extern HWND g_hFrame;

class CMainFrame;
extern CMainFrame* g_pMainFrame;