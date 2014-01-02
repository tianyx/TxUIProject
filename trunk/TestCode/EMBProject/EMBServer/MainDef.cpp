#include "StdAfx.h"
#include "MainDef.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "io.h"
#include "EMBCommonFunc.h"
#include "TxParamString.h"
#include "EMBDocDef.h"
#include "TxAutoComPtr.h"
using namespace EMB;

// 全局信息存储变量？尽量少用全局变量
ST_GLOBAL g_GlobalInfo;
// 管理插件动态库指针
HMODULE g_hModulePluginMgr = NULL;
// 插件管理插件指针
EMB::IPluginBaseInterface* g_pIPluginMgr = NULL;

ITxUnkown* g_pTaskDispatchMgr = NULL;

/*
*Description：初始化配置文件
*Input Param：
*Return Param：返回成功或失败
*History：
*/
BOOL InitGlobalConfig()
{
	//任务管理配置文件
	g_GlobalInfo.szAppPath = GetAppPath().c_str();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\EMBServer.xml");
	//日志配置文件
	g_GlobalInfo.szLogFile	 = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szLogFile	 += TEXT("\\Log");
	CreateDirectory(g_GlobalInfo.szLogFile, NULL);
	g_GlobalInfo.szLogFile	 += TEXT("\\MainLog.log");

	//插件配置文件
	g_GlobalInfo.szPluginPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szPluginPath +=TEXT("\\Plugin");
	if (_access(g_GlobalInfo.szIniPath, 04) == -1)
	{
		CFWriteLog("找不到配置文件EMBServer.xml！");
		return FALSE;
	}

	return TRUE;
}

/*
*Description：加载插件管理插件动态库
*Input Param：
*Return Param：返回成功或失败
*History：
*/
BOOL LoadPluginManager()
{
	//插件管理动态库
	CString strFile = g_GlobalInfo.szAppPath;
	strFile +=TEXT("\\plugin\\EMBPluginMgr.epl");
	if (_access(strFile, 0) == -1)
	{
		ASSERT(FALSE);
		CFWriteLog(LOGKEYMAIN, TEXT("EMBPluginMgr.epl not Found"));
		return FALSE;
	}
	
	// 从文件路径加载
	TxLoadPlugin(strFile, g_hModulePluginMgr, (LPVOID&)g_pIPluginMgr);
	if (g_pIPluginMgr)
	{
		CTxAutoComPtr<IPluginManagerInterface> apPluginMgr;
		g_pIPluginMgr->QueryInterface(GuidEMBPlugin_IPluginManager, (LPVOID&) *&apPluginMgr);
		if (apPluginMgr)
		{
			apPluginMgr->InitPluginsSearch(FALSE, NULL);
		}

	}

	return (g_hModulePluginMgr != NULL && g_pIPluginMgr != NULL);
}

/*
*Description：注销插件管理插件动态库
*Input Param：
*Return Param：返回成功或失败
*History：
*/
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


/*
*Description：初始化任务管理相应服务
*Input Param：
*Return Param：返回成功或失败
*History：
*/
BOOL InitServer()
{
	//读取配置文件
	CFile xmlfile;
	CString strParam;
	if (xmlfile.Open(g_GlobalInfo.szIniPath, CFile::modeRead, NULL))
	{
		int nFileLen = xmlfile.GetLength();
		char* pBuff = new char[nFileLen +1];
		pBuff[nFileLen] = '\0';
		int nRead = xmlfile.Read(pBuff, nFileLen);
		strParam = pBuff;
		delete[] pBuff;
		pBuff = NULL;
		xmlfile.Close();
	}
	else
	{
		ASSERT(FALSE);
		AfxMessageBox("read config file err");
		return FALSE;
	}

	CTxParamString txParam(strParam);
	//find plugin config
	// 获取接收任务插件配置
	CString strKeyPath = EPATH_MAIN;
	strKeyPath += TEXT("\\");
	strKeyPath += EK_TASKRISERCONFIG;
	CTxParamString strTaskRiserConfig("");
	txParam.GetSubNodeString(strKeyPath, strTaskRiserConfig);
	ASSERT(!strTaskRiserConfig.IsEmpty());
	
	// 获取存储任务插件配置
	strKeyPath = EPATH_MAIN;
	strKeyPath += TEXT("\\");
	strKeyPath +=EK_TASKSTORAGECONFIG;
	CTxParamString strTaskStorageConfig("");
	txParam.GetSubNodeString(strKeyPath, strTaskStorageConfig);
	ASSERT(!strTaskStorageConfig.IsEmpty());

	// 获取分配任务插件配置
	strKeyPath = EPATH_MAIN;
	strKeyPath += TEXT("\\");
	strKeyPath +=EK_TASKDISPATCHCONFIG;
	CTxParamString strTaskDispatchConfig;
	txParam.GetSubNodeString(strKeyPath, strTaskDispatchConfig);
	ASSERT(!strTaskDispatchConfig.IsEmpty());

	strKeyPath = EPATH_MAIN;
	strKeyPath += TEXT("\\");
	strKeyPath +=EK_UISVRCONFIG;
	CTxParamString strUISvrConfig;
	txParam.GetSubNodeString(strKeyPath, strUISvrConfig);
	ASSERT(!strUISvrConfig.IsEmpty());



	//load plugin, set param, then connect them
	if (!g_pIPluginMgr)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//加载任务接收、任务存储、任务分配插件，并连接他们
	ST_LOADEDPLUGIN tmpPlugin;
	if (LoadPluginByPluginMgr(PluginType_TaskRiser, SubType_None, g_pIPluginMgr, tmpPlugin))
	{
		tmpPlugin.strParam = strTaskRiserConfig;
		g_GlobalInfo.vPlugins.push_back(tmpPlugin);
	}

	if (LoadPluginByPluginMgr(PluginType_Storage, SubType_None, g_pIPluginMgr, tmpPlugin))
	{
		tmpPlugin.strParam = strTaskStorageConfig;

		g_GlobalInfo.vPlugins.push_back(tmpPlugin);
	}

	if (LoadPluginByPluginMgr(PluginType_Dispatcher, SubType_None, g_pIPluginMgr, tmpPlugin))
	{
		tmpPlugin.strParam = strTaskDispatchConfig;
		g_GlobalInfo.vPlugins.push_back(tmpPlugin);

		g_pTaskDispatchMgr = tmpPlugin.pIface;
	}

	if (g_GlobalInfo.vPlugins.size() < 3)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (LoadPluginByPluginMgr(PluginType_UIServer, SubType_None, g_pIPluginMgr, tmpPlugin))
	{
		CFWriteLog(LOGKEY_EMBSERVER, TEXT("ui server plugin founded."));
		tmpPlugin.strParam = strUISvrConfig;
		g_GlobalInfo.vPlugins.push_back(tmpPlugin);
	}

	if (g_GlobalInfo.vPlugins.size() < 4)
	{
		ASSERT(FALSE);
		//no uiserver, but not quit
	}
	//set param;
	for (size_t i = 0; i < g_GlobalInfo.vPlugins.size(); ++i)
	{
		CString strRet;
		SetPluginParam(g_GlobalInfo.vPlugins[i].pIface, g_GlobalInfo.vPlugins[i].strParam, strRet);
	}
	//connect taskriser<-->storage<-->dispath
	for (size_t i = 0; i < 2; ++i)
	{
		HRESULT hr = ConnectPlugins(g_GlobalInfo.vPlugins[i].pIface, g_GlobalInfo.vPlugins[i+1].pIface);
		if (FAILED(hr))
		{
			return FALSE;
		}
	}

	if (g_GlobalInfo.vPlugins.size() >3)
	{
		//connect uiserver -->dispatch
		HRESULT hr = ConnectPlugins(g_GlobalInfo.vPlugins[pluginSerialID_UIServer].pIface, g_GlobalInfo.vPlugins[pluginSerialID_TaskDispatch].pIface);
		if (FAILED(hr))
		{
			return FALSE;
		}

		//connect uiserver-->storage 
		hr = ConnectPlugins(g_GlobalInfo.vPlugins[pluginSerialID_UIServer].pIface, g_GlobalInfo.vPlugins[pluginSerialID_TaskStorage].pIface);
		if (FAILED(hr))
		{
			return FALSE;
		}
	}
	

	return TRUE;
}

/*
*Description：注销任务管理相应服务
*Input Param：
*Return Param：返回成功或失败
*History：
*/
BOOL UnInitServer()
{
	if (g_GlobalInfo.vPlugins.size() == 0)
	{
		return TRUE;
	}
	//disconnect
	for (int i = 0; i < 2; ++i)
	{
		HRESULT hr = DisConnectPlugins(g_GlobalInfo.vPlugins[i].pIface, g_GlobalInfo.vPlugins[i+1].pIface);
		if (FAILED(hr))
		{
			ASSERT(FALSE);
		}
	}

	if (g_GlobalInfo.vPlugins.size() > 3)
	{
		CFWriteLog(LOGKEY_EMBSERVER, TEXT("start conn ui server!"));
		//disconnect uiserver -->dispatch
		HRESULT hr = DisConnectPlugins(g_GlobalInfo.vPlugins[pluginSerialID_UIServer].pIface, g_GlobalInfo.vPlugins[pluginSerialID_TaskDispatch].pIface);
		if (FAILED(hr))
		{
			return FALSE;
		}

		//disconnect uiserver-->storage 
		hr = DisConnectPlugins(g_GlobalInfo.vPlugins[pluginSerialID_UIServer].pIface, g_GlobalInfo.vPlugins[pluginSerialID_TaskStorage].pIface);
		if (FAILED(hr))
		{
			return FALSE;
		}
	}
	

	//unload 
	for (int i = 0; i < g_GlobalInfo.vPlugins.size(); ++i)
	{
		UnLoadPluginByPluginMgr(g_pIPluginMgr, g_GlobalInfo.vPlugins[i]);
	}

	g_GlobalInfo.vPlugins.clear();
	return TRUE;
}

/*
*Description：启动各插件服务
*Input Param：
*Return Param：返回成功或失败
*History：
*/
BOOL RunServer( BOOL bRun )
{
	for (size_t i = 0; i < g_GlobalInfo.vPlugins.size(); ++i)
	{
		ST_LOADEDPLUGIN& tmpPlug = g_GlobalInfo.vPlugins[i];
	
		CTxAutoComPtr<IPluginControlInterface> apCtrl;
		tmpPlug.pIface->QueryInterface(GuidEMBPlugin_IControler, (LPVOID&) *&apCtrl);
		if(apCtrl)
		{
			HRESULT hr = bRun? apCtrl->Run_Plugin():apCtrl->Stop_Plugin();
			if (FAILED(hr))
			{
				ASSERT(FALSE);
				return FALSE;
			}
		}
	}

	return TRUE;
}

