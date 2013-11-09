#include "StdAfx.h"
#include "EMBCommonFunc.h"

#include "TxAutoComPtr.h"
using namespace EMB;


typedef int (__cdecl *GETPLUGININSTANCE)(LPVOID& pInterface);

BOOL TxLoadPlugin(const CString& strFileIn, HMODULE& hModuleOut, LPVOID& pInterfaceOut)
{
		hModuleOut = LoadLibrary(strFileIn);
		if (hModuleOut != NULL)
		{
			GETPLUGININSTANCE  pFun = (GETPLUGININSTANCE)GetProcAddress(hModuleOut, "GetPluginInstance");
			if (pFun)
			{
				pFun(pInterfaceOut);
			}
			else
			{
				FreeLibrary(hModuleOut);
				hModuleOut = NULL;
			}
			
		}
	return TRUE;
}

BOOL TxUnloadPlugin(HMODULE hModuleIn)
{
	BOOL bRet = TRUE;
	if (hModuleIn)
	{
		bRet = FreeLibrary(hModuleIn);
	}

	return bRet;
}



BOOL GetXmlFirstNode(const CString& strXmlIn, CString& strNodeOut )
{
	strNodeOut.Empty();
	int nXmlLen = strXmlIn.GetLength();
	if (nXmlLen < 4)
	{
		return FALSE;
	}

	int nPos = 0;
	while (TRUE)
	{
		nPos = strXmlIn.Find('<', nPos);
		if (nPos < 0 || nPos >= nXmlLen-2)
		{
			return FALSE;
		}
		char chNext = strXmlIn.GetAt(nPos +1);
		if (chNext != '?'
			&& chNext != '!')
		{
			int nPos2 = strXmlIn.Find('>', nPos+1);
			if (nPos2 != -1)
			{
				strNodeOut = strXmlIn.Mid(nPos, nPos2 -nPos+1);
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			//continue
			nPos++;
			if (nPos >= nXmlLen -2)
			{

				return FALSE;
			}
		}

		
	}

	return FALSE;
}



BOOL LoadPluginByPluginMgr( int nType, int nSubType, EMB::ITxUnkown* pPluginMgr, ST_LOADEDPLUGIN& pluginOut )
{
	pluginOut.handle = NULL;
	pluginOut.pIface = NULL;
	CTxAutoComPtr<IPluginManagerInterface> apPluginMgr;
	pPluginMgr->QueryInterface(GuidEMBPlugin_IPluginManager, (LPVOID&) *&apPluginMgr);
	ASSERT(apPluginMgr != NULL);
	GUID guidNeed;
	apPluginMgr->FindPlugin(nType, nSubType, guidNeed);
	if (guidNeed == GUID_NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	pluginOut.handle =apPluginMgr->LoadPlugin(guidNeed, pluginOut.pIface);
	if (pluginOut.handle && pluginOut.pIface)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

BOOL SetPluginParam( ITxUnkown* pIPlugin, CString& strParam , CString& strRet)
{
	if (!pIPlugin)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	CTxAutoComPtr<IPluginConfigInterface> apIConfig;
	pIPlugin->QueryInterface(GuidEMBPlugin_IConfig, (LPVOID&)*&apIConfig);
	if (apIConfig)
	{
		return apIConfig->SetParam(strParam, strRet);
	}
	return FALSE;
}



BOOL UnLoadPluginByPluginMgr( EMB::ITxUnkown* pPluginMgr, ST_LOADEDPLUGIN& pluginOut )
{
	if (pluginOut.pIface)
	{
		pluginOut.pIface->Release();
	}
	CTxAutoComPtr<IPluginManagerInterface> apPluginMgr;
	pPluginMgr->QueryInterface(GuidEMBPlugin_IPluginManager, (LPVOID&) *&apPluginMgr);
	ASSERT(apPluginMgr != NULL);
	HRESULT hr = apPluginMgr->UnloadPlugin(GUID_NULL, pluginOut.handle);
	
	return SUCCEEDED(hr);
}

HRESULT ConnectPlugins( EMB::ITxUnkown* pIPlugin1, EMB::ITxUnkown* pIPlugin2 )
{
	if (!pIPlugin1 || !pIPlugin2)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	CTxAutoComPtr<IPluginConnectorInterce> apIConn1;
	pIPlugin1->QueryInterface(GuidEMBPlugin_IConnector, (LPVOID&)*&apIConn1);
	if (apIConn1)
	{
		return apIConn1->Connect(pIPlugin2);
	}

	return S_FALSE;
}

HRESULT DisConnectPlugins( EMB::ITxUnkown* pIPlugin1, EMB::ITxUnkown* pIPlugin2 )
{
	if (!pIPlugin1 || !pIPlugin2)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	CTxAutoComPtr<IPluginConnectorInterce> apIConn1;
	pIPlugin1->QueryInterface(GuidEMBPlugin_IConnector, (LPVOID&)*&apIConn1);
	if (apIConn1)
	{
		return apIConn1->Disconnect(pIPlugin2);
	}

	return S_FALSE;
}
