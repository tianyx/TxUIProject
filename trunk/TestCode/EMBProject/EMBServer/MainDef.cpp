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

ST_GLOBAL g_GlobalInfo;
HMODULE g_hModulePluginMgr = NULL;
EMB::IPluginBaseInterface* g_pIPluginMgr = NULL;
BOOL InitGlobalConfig()
{
	g_GlobalInfo.szAppPath = GetAppPath().c_str();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\EMBServer.xml");
	g_GlobalInfo.szLogFile	 = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szLogFile	 += TEXT("\\Log");
	CreateDirectory(g_GlobalInfo.szLogFile, NULL);
	g_GlobalInfo.szLogFile	 += TEXT("\\MainLog.log");

	g_GlobalInfo.szPluginPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szPluginPath +=TEXT("\\Plugin");
	if (_access(g_GlobalInfo.szIniPath, 04) == -1)
	{
		CFWriteLog("找不到配置文件EMBServer.xml！");
		return FALSE;
	}

	return TRUE;
}

BOOL LoadPluginManager()
{
	CString strFile = g_GlobalInfo.szAppPath;
	strFile +=TEXT("\\plugin\\EMBPluginMgr.dll");
	if (_access(strFile, 0) == -1)
	{
		ASSERT(FALSE);
		CFWriteLog(LOGKEYMAIN, TEXT("EMBPluginMgr.dll not Found"));
		return FALSE;
	}
	
	TxLoadPlugin(strFile, g_hModulePluginMgr, (LPVOID&)g_pIPluginMgr);
	if (g_pIPluginMgr)
	{
		CTxAutoComPtr<IPluginManagerInterface> apPluginMgr;
		g_pIPluginMgr->QueryInterface(GuidEMBPlugin_IPluginManager, (LPVOID&) *&apPluginMgr);
		if (apPluginMgr)
		{
			apPluginMgr->InitPluginsSearch(FALSE, TEXT("dll"));
		}

	}

	return (g_hModulePluginMgr != NULL && g_pIPluginMgr != NULL);
}

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



BOOL InitServer()
{
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
	CString strKeyPath = EPATH_MAIN;
	strKeyPath += TEXT("\\");
	strKeyPath += EK_TASKRISERCONFIG;
	CTxParamString strTaskRiserConfig("");
	txParam.GetSubNodeString(strKeyPath, strTaskRiserConfig);
	ASSERT(!strTaskRiserConfig.IsEmpty());
	
	strKeyPath = EPATH_MAIN;
	strKeyPath += TEXT("\\");
	strKeyPath +=EK_TASKSTORAGECONFIG;
	CTxParamString strTaskStorageConfig("");
	txParam.GetSubNodeString(strKeyPath, strTaskStorageConfig);
	ASSERT(!strTaskStorageConfig.IsEmpty());

	strKeyPath = EPATH_MAIN;
	strKeyPath += TEXT("\\");
	strKeyPath +=EK_TASKDISPATCHCONFIG;
	CTxParamString strTaskDispatchConfig;
	txParam.GetSubNodeString(strKeyPath, strTaskDispatchConfig);
	ASSERT(!strTaskDispatchConfig.IsEmpty());

	//load plugin, set param, then connect them
	if (!g_pIPluginMgr)
	{
		ASSERT(FALSE);
		return FALSE;
	}

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
	}

	if (g_GlobalInfo.vPlugins.size() != 3)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//set param;
	for (size_t i = 0; i < g_GlobalInfo.vPlugins.size(); ++i)
	{
		CString strRet;
		SetPluginParam(g_GlobalInfo.vPlugins[i].pIface, g_GlobalInfo.vPlugins[i].strParam, strRet);
	}
	//connect
	for (size_t i = 0; i < g_GlobalInfo.vPlugins.size() -1; ++i)
	{
		HRESULT hr = ConnectPlugins(g_GlobalInfo.vPlugins[i].pIface, g_GlobalInfo.vPlugins[i+1].pIface);
		if (FAILED(hr))
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL UnInitServer()
{
	if (g_GlobalInfo.vPlugins.size() == 0)
	{
		return TRUE;
	}
	//disconnect
	for (int i = 0; i < g_GlobalInfo.vPlugins.size()-1; ++i)
	{
		HRESULT hr = DisConnectPlugins(g_GlobalInfo.vPlugins[i].pIface, g_GlobalInfo.vPlugins[i+1].pIface);
		if (FAILED(hr))
		{
			ASSERT(FALSE);
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

