#include "StdAfx.h"
#include "MainDef.h"
#include "FGlobal.h"
#include "TxLogManager.h"
#include "io.h"
#include "EMBCommonFunc.h"
#include "TxAutoComPtr.h"
#include "IEMBBaseInterface.h"
using namespace EMB;

ST_GLOBAL g_GlobalInfo;
HMODULE g_hModulePluginMgr = NULL;
EMB::IPluginBaseInterface* g_pIPluginMgr = NULL;

HMODULE g_hActorPlugin  = NULL;
EMB::IPluginBaseInterface* g_pIActorPlugin = NULL;
BOOL InitGlobalConfig()
{
	//check arg
	int nArgC = __argc;
	if (nArgC != 2)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CString strHwnd = __argv[1];
	g_GlobalInfo.excInfo.FromString(strHwnd);

	if (!::IsWindow(g_GlobalInfo.excInfo.hwndActor))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (g_GlobalInfo.excInfo.guid == -1)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	g_GlobalInfo.szAppPath = GetAppPath().c_str();
	g_GlobalInfo.szIniPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szIniPath += TEXT("\\EMBActorHost.xml");
	g_GlobalInfo.szLogFile	 = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szLogFile	 += TEXT("\\Log");
	CreateDirectory(g_GlobalInfo.szLogFile, NULL);
	g_GlobalInfo.szLogFile	 += TEXT("\\ActorHostLog.log");

	g_GlobalInfo.szPluginPath = g_GlobalInfo.szAppPath;
	g_GlobalInfo.szPluginPath +=TEXT("\\Plugin");
	if (_access(g_GlobalInfo.szIniPath, 04) == -1)
	{
		//CFWriteLog("�Ҳ��������ļ�Excutor.ini��");
		return FALSE;
	}

	CFile file;
	CString strXml;
	if (file.Open(g_GlobalInfo.szIniPath, CFile::modeRead, NULL))
	{
		int nFileLen = file.GetLength();
		if (nFileLen == 0)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		char* pbuff = new char[nFileLen+1];
		ZeroMemory(pbuff, nFileLen +1);
		int nSize = file.Read(pbuff, nFileLen);
		ASSERT(nSize == nFileLen);
		strXml = pbuff;
		
	}


	//Load actorhost
	if (!LoadActorHost(strXml))
	{
		return FALSE;
	}
	

	return TRUE;
}


BOOL LoadActorHost(CString& strConfig)
{
	CString strFile = g_GlobalInfo.szAppPath;
	strFile +=TEXT("\\EMBActor.dll");
	if (_access(strFile, 0) == -1)
	{
		ASSERT(FALSE);
		CFWriteLog(LOGKEYMAIN, TEXT("EMBActor.dll not Found"));
		return FALSE;
	}

	TxLoadPlugin(strFile, g_hActorPlugin, (LPVOID&)g_pIActorPlugin);
	if (g_hActorPlugin != NULL && g_pIActorPlugin != NULL)
	{
		//set param
		CTxAutoComPtr<IPluginConfigInterface> pConfig;
		g_pIActorPlugin->QueryInterface(GuidEMBPlugin_IConfig, (LPVOID&) (*&pConfig));
		CString strRet;
		HRESULT hr = pConfig->SetParam(strConfig, strRet);
		if(hr != S_OK)
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}
	return FALSE;
}

BOOL UnloadActorHost()
{
	if (g_pIActorPlugin)
	{
		g_pIActorPlugin->Release();
		g_pIActorPlugin = NULL;
	}
	if (g_hActorPlugin)
	{
		FreeLibrary(g_hActorPlugin);
		g_hActorPlugin = NULL;
	}

	return TRUE;
}


BOOL LoadPluginManager()
{
	CString strFile = g_GlobalInfo.szAppPath;
	strFile +=TEXT("\\plugins\\EMBPluginMgr.dll");
	if (_access(strFile, 0) == -1)
	{
		ASSERT(FALSE);
		CFWriteLog(LOGKEYMAIN, TEXT("EMBPluginMgr.dll not Found"));
		return FALSE;
	}
	
	TxLoadPlugin(strFile, g_hModulePluginMgr, (LPVOID&)g_pIPluginMgr);
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

