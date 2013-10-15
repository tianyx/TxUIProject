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
		vOut.push_back(addr);
	}
	return TRUE;
}

BOOL InitGlobalConfig()
{
	g_GlobalInfo.szAppPath = GetAppPath();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\MBCAutoSwitcherConfig.ini");
	if (_access(g_GlobalInfo.szIniPath.c_str(), 04) == -1)
	{
		CFWriteLog("找不到配置文件MBCAutoSwitcherConfig.ini！");
		return FALSE;
	}
	char szBuff[512];
	ZeroMemory(szBuff, sizeof(szBuff));

	//client
	GetPrivateProfileStringA("Config","ClientList","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	std::vector<string> vClientList;
	SplitteStrings(szBuff,vClientList);
	
	GetSubConfig(vClientList, g_GlobalInfo.vClientAddr);

	//svr
	GetPrivateProfileStringA("Config","SvrList","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	std::vector<string> vSvrList;
	SplitteStrings(szBuff,vSvrList);

	GetSubConfig(vSvrList, g_GlobalInfo.vSvrAddr);


	//master end
	GetPrivateProfileStringA("Config","MasterEndList","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	std::vector<string> vMasterEndList;
	SplitteStrings(szBuff,vMasterEndList);

	GetSubConfig( vMasterEndList, g_GlobalInfo.vEndAddrMaster);

	//slave end
	GetPrivateProfileStringA("Config","SlaveEndList","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	std::vector<string> vSlaveEndList;
	SplitteStrings(szBuff,vSlaveEndList);

	GetSubConfig( vSlaveEndList, g_GlobalInfo.vEndAddrSlave);


	//back end
	GetPrivateProfileStringA("Config","BacksvrList","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	std::vector<string> BacksvrList;
	SplitteStrings(szBuff,BacksvrList);
	GetSubConfig( BacksvrList, g_GlobalInfo.vEndAddrBack);


	//
	GetPrivateProfileStringA("Config","GraphStateHostListen","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	if (strlen(szBuff) == 0)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	g_GlobalInfo.addrGStateHostListen = GetAddrFromStr(CString(szBuff));

	g_GlobalInfo.nLiveCheckInterval = GetPrivateProfileInt("Config", "HeartbeatCheckInterval", 1000, g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nLiveCheckStartDelay = GetPrivateProfileInt("Config", "HeartbeatCheckStartDelay", 3000, g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nSockDownReconnDelay = GetPrivateProfileInt("Config", "SockDownReconnDelay", 5000, g_GlobalInfo.szIniPath.c_str());

	g_GlobalInfo.nStateCheckInterval = GetPrivateProfileInt("Config", "StateCheckInterval", 5000, g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nEndBitRateCheckInterval = GetPrivateProfileInt("Config", "EndBitRateCheckInterval", 1000, g_GlobalInfo.szIniPath.c_str());

	g_GlobalInfo.nSwitchCD = GetPrivateProfileInt("Config", "SwitchCD", 5000, g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nRelayCD = GetPrivateProfileInt("Config", "RelayCD", 5000, g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nUDPRelayTTL = GetPrivateProfileInt("Config", "UDPRelayTTL", 32, g_GlobalInfo.szIniPath.c_str());

	g_GlobalInfo.nAutoJudgeMasterClient = GetPrivateProfileInt("Config", "AutoJudgeMasterClient", 1, g_GlobalInfo.szIniPath.c_str());
	g_GlobalInfo.nCacheBeforeRelay = GetPrivateProfileInt("Config", "SleepBeforeRelay",1024*1024, g_GlobalInfo.szIniPath.c_str());

	//set to global variable
	 g_SOCK_INTERVAL_RECONN  = g_GlobalInfo.nSockDownReconnDelay;
	 g_INTERVAL_LIVECHECK  = g_GlobalInfo.nLiveCheckInterval;
	 g_DELAY_BEFORELIVECHECK  = g_GlobalInfo.nLiveCheckStartDelay;
	 g_nMBCSockOptTTL = g_GlobalInfo.nUDPRelayTTL;
	g_nEndBitRateCheckInterval = g_GlobalInfo.nEndBitRateCheckInterval;



	

	/*

	std::vector<string> vSvrAddr;
	GetPrivateProfileStringA("Config","SvrCtrlAddr","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	SplitteStrings(szBuff,vSvrAddr);

	std::vector<string> vEndAddr;
	GetPrivateProfileStringA("Config","MasterEndAddr","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	SplitteStrings(szBuff,vEndAddr);

	std::vector<string> vEndAddrSlave;
	GetPrivateProfileStringA("Config","SlaveEndAddr","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	SplitteStrings(szBuff,vEndAddrSlave);

	std::vector<string> vEndAddrBack;
	GetPrivateProfileStringA("Config","BackEndAddr","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	SplitteStrings(szBuff,vEndAddrBack);

	std::vector<string> vChannelList;
	GetPrivateProfileStringA("Config","MasterChannelList","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	SplitteStrings(szBuff,vChannelList);

	std::vector<string> vSlaveChannelList;
	GetPrivateProfileStringA("Config","SlaveChannelList","", szBuff, sizeof(szBuff), g_GlobalInfo.szIniPath.c_str());
	SplitteStrings(szBuff,vSlaveChannelList);

	if (vChannelList.size() != vEndAddr.size() || vSlaveChannelList.size() != vEndAddrSlave.size())
	{
		ASSERT(FALSE);
		CFWriteLog(TEXT("配置文件不正确"));
		return FALSE;
	}


	for (size_t i = 0; i < vSvrAddr.size(); ++i)
	{
		CString szkey = vSvrAddr[i].c_str();
		ST_TSADDR tmpAddr;
		tmpAddr.szAddr = vSvrAddr[i];
		tmpAddr.addr =GetAddrFromStr(szkey);
		tmpAddr.type = MBCOBJTYPE_SVR;
		g_GlobalInfo.vSvrAddr.push_back(tmpAddr);
	
	}

	for(size_t i = 0; i < vClientAddr.size(); ++i)
	{
		CString szkey = vClientAddr[i].c_str();
		ST_TSADDR tmpAddr;
		tmpAddr.szAddr = vClientAddr[i];
		tmpAddr.addr =GetAddrFromStr(szkey);
		tmpAddr.type = MBCOBJTYPE_CLIENT;
		g_GlobalInfo.vClientAddr.push_back(tmpAddr);

	}

	for (size_t i = 0; i < vEndAddr.size(); ++i)
	{
		CString szkey = vEndAddr[i].c_str();
		ST_TSADDR tmpAddr;
		tmpAddr.szAddr = vEndAddr[i];
		tmpAddr.addr =GetAddrFromStr(szkey);
		tmpAddr.type = MBCOBJTYPE_ENDMASTER;
		g_GlobalInfo.vEndAddrMaster.push_back(tmpAddr);

	}


	for (size_t i = 0; i < vEndAddrSlave.size(); ++i)
	{
		CString szkey = vEndAddrSlave[i].c_str();
		ST_TSADDR tmpAddr;
		tmpAddr.szAddr = vEndAddrSlave[i];
		tmpAddr.addr =GetAddrFromStr(szkey);
		tmpAddr.type = MBCOBJTYPE_ENDSLAVE;
		g_GlobalInfo.vEndAddrSlave.push_back(tmpAddr);
	}
		
	for (size_t i = 0; i < vEndAddrBack.size(); ++i)
	{
		CString szkey = vEndAddrBack[i].c_str();
		ST_TSADDR tmpAddr;
		tmpAddr.szAddr = vEndAddrBack[i];
		tmpAddr.addr =GetAddrFromStr(szkey);
		tmpAddr.type = MBCOBJTYPE_ENDBACK;
		g_GlobalInfo.vEndAddrBack.push_back(tmpAddr);
	}
	
	g_GlobalInfo.vCHList = vChannelList;
	g_GlobalInfo.vCHListSlave = vSlaveChannelList;

*/

	return TRUE;
}

