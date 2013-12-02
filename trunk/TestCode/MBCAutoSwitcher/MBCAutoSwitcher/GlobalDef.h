#pragma once
#include <string>
#include <vector>
#include "MBCCommonDef.h"

#define SWITCHERLOGKEY 101

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
	std::vector<ST_MBCOBJCFG> vSvrAddr;
	std::vector<ST_MBCOBJCFG> vClientAddr;

	//must same size
	std::vector<ST_MBCOBJCFG> vEndAddrMaster;
	std::vector<ST_MBCOBJCFG> vEndAddrSlave;

	std::vector<ST_MBCOBJCFG> vEndAddrBack;

	//for remote get state
	SOCKADDR_IN addrGStateHostListen;

	//graphconfig
	int nStateCheckInterval;
	int nEndBitRateCheckInterval;
	int nSwitchCD;
	int nRelayCD;

	int nLiveCheckInterval;
	int nLiveCheckStartDelay;
	int nSockDownReconnDelay;
	int nUDPRelayTTL;
	int nAutoJudgeMasterClient;
	int nCacheBeforeRelay;
	int nRelay2to1Mode;

	ST_GLOBAL()
	{
		nStateCheckInterval = 1000;
		nSwitchCD = 10;
		nRelayCD = 10;
	
		nLiveCheckInterval = 1000;
		nLiveCheckStartDelay= 2000;
		nSockDownReconnDelay = 5000;
		nUDPRelayTTL = 32;
		nAutoJudgeMasterClient = 1;
		nCacheBeforeRelay = 1024*1024;
		nRelay2to1Mode = 0;
	}


};

BOOL InitGlobalConfig();

extern ST_GLOBAL g_GlobalInfo;
extern HWND g_hFrame;

class CMainFrame;
extern CMainFrame* g_pMainFrame;

