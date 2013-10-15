#include "StdAfx.h"
#include "GlobalDef.h"
#include "FGlobal.h"
#include "io.h"

ST_GLOBAL g_GlobalInfo;

HWND g_hFrame = NULL;
using namespace std;

BOOL GetSubConfig(std::vector<string>& vItemlist, std::vector<ST_MBCOBJCFG>& vOut)
{
	char szBuff[512];
	ZeroMemory(szBuff, sizeof(szBuff));
	for (size_t i = 0; i < vItemlist.size(); ++i)
	{

		ST_MBCOBJCFG addr;
		addr.szName = vItemlist[i].c_str();
		GetPrivateProfileStringA(vItemlist[i].c_str(),"ChId","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
		addr.szChId = szBuff;
		
		GetPrivateProfileStringA(vItemlist[i].c_str(),"RemoteAddr","0.0.0.0:0", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
		addr.addr = GetAddrFromStr(CString(szBuff));

		GetPrivateProfileStringA(vItemlist[i].c_str(),"LocalBindIp","0.0.0.0", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
		addr.localBindAddr.sin_addr.S_un.S_addr = inet_addr(szBuff);
	}
	return TRUE;
}

BOOL InitGlobalConfig()
{
	g_GlobalInfo.szAppPath = GetAppPath();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\MBCWatchDogConfig.ini");
	if (_access(g_GlobalInfo.szIniPath.c_str(), 04) == -1)
	{
		CFWriteLog("找不到配置文件MBCWatchDogConfig.ini！");
		return FALSE;
	}
	char szBuff[512];
	ZeroMemory(szBuff, sizeof(szBuff));

	GetPrivateProfileStringA("Config","addrMasterLocal","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	if (strlen(szBuff) == 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	g_GlobalInfo.addrMasterLocal = GetAddrFromStr(CString(szBuff));

	GetPrivateProfileStringA("Config","addrMasterRemote","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	if (strlen(szBuff) == 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	g_GlobalInfo.addrMasterRemote = GetAddrFromStr(CString(szBuff));

	GetPrivateProfileStringA("Config","addrBackLocal","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	if (strlen(szBuff) == 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	g_GlobalInfo.addrBackLocal = GetAddrFromStr(CString(szBuff));

	GetPrivateProfileStringA("Config","addrBackRemote","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	if (strlen(szBuff) == 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	g_GlobalInfo.addrBackRemote = GetAddrFromStr(CString(szBuff));

	GetPrivateProfileStringA("Config","szLocalWatchAppPath","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	if (strlen(szBuff) == 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	g_GlobalInfo.szLocalWatchAppPath = (szBuff);

	g_GlobalInfo.nRestartLocalAppCD = GetPrivateProfileInt("Config", "RestartLocalAppCD", 300, g_GlobalInfo.szIniPath.c_str());

	g_GlobalInfo.nStateGetInterval = GetPrivateProfileInt("Config", "StateGetInterval", 300, g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nAutoSwitchCriticalCount = GetPrivateProfileInt("Config", "AutoSwitchCriticalCount", 3000, g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nSockDownReconnDelay = GetPrivateProfileInt("Config", "SockDownReconnDelay", 3000, g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nStateGetStartDelay = GetPrivateProfileInt("Config", "StateGetStartDelay", 3000, g_GlobalInfo.szIniPath.c_str());

	g_SOCK_INTERVAL_RECONN = g_GlobalInfo.nSockDownReconnDelay;
	g_INTERVAL_LIVECHECK = g_GlobalInfo.nStateGetInterval;
	g_DELAY_BEFORELIVECHECK = 0;
	return TRUE;
}

