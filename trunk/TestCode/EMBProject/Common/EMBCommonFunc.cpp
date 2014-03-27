#include "StdAfx.h"
#include "EMBCommonFunc.h"
#include "TxParamString.h"
#include "TxAutoComPtr.h"
#include "EMBDocDef.h"
#include "MBCTransMsg.h"
#include "EMBMessageDef.h"
using namespace EMB;


typedef int (__cdecl *GETPLUGININSTANCE)(LPVOID& pInterface);

BOOL TxLoadPlugin(const CString& strFileIn, HMODULE& hModuleOut, LPVOID& pInterfaceOut)
{
		int nPos = strFileIn.ReverseFind(_T('\\'));
		CString strDllPath = strFileIn.Left(nPos);
		SetDllDirectory(strDllPath);
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

BOOL TxLoadPluginVC6( const CString& strFileIn, HMODULE& hModuleOut, LPVOID& pInterfaceOut )
{

	int nPos = strFileIn.ReverseFind(_T('\\'));
	CString strDllPath = strFileIn.Left(nPos);
	SetDllDirectory(strDllPath);
	hModuleOut = LoadLibrary(strFileIn);
	if (hModuleOut != NULL)
	{
		GETPLUGININSTANCE  pFun = (GETPLUGININSTANCE)GetProcAddress(hModuleOut, "GetPluginInstanceVC6");
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

BOOL GetTaskBasicInfo( const CString& strTaskIn, ST_TASKBASIC& infoOut )
{
	infoOut.strGuid = "";
	CTxParamString txParam(strTaskIn);
	CString strPath = EPATH_TASKBASIC;
	CTxParamString txBasicStr;
	txParam.GetSubNodeString(strPath, txBasicStr);
	if (txBasicStr.IsEmpty())
	{
		return FALSE;
	}

	infoOut.FromString(txBasicStr);

	return TRUE;
}

BOOL GetEmbXmlMainInfo( const CString& strTaskIn, ST_EMBXMLMAININFO& infoOut )
{
	int nPos = strTaskIn.Find(EK_MAIN);
	if (nPos > 0 && strTaskIn.GetAt(nPos -1) == '<')
	{
		//try find right
		int nPosR = strTaskIn.Find('>', nPos);
		if (nPosR != -1)
		{

			CString	strSub =strTaskIn.Mid(nPos -1, nPosR -nPos+2);

			if (strTaskIn.GetAt(nPosR -1) == '/')
			{
			}
			else
			{
				strSub = strTaskIn.Mid(nPos -1, nPosR -nPos+2);
				strSub += TEXT("</");
				strSub +=EK_MAIN;
				strSub += TEXT(">");
			}
			infoOut.FromString(strSub);

			return TRUE;
		}
	}

	return FALSE;
}

HRESULT SendMainXmlMsg( SOCKET sock, int nMsgType, int nXmltype, LPCTSTR szGuid /*= NULL*/ )
{
	HRESULT hr = S_OK;
	ST_EMBTRANSMSG msg(nMsgType);
	msg.strData.Format(EDOC_MAINHEADERFMT, 1, nXmltype, szGuid);
	CEMBAutoBuffer szbuff(msg);
	int nUsed = 0;
	PackMBCMsg(msg, szbuff, szbuff.GetSize(), nUsed);
	hr = send(sock, szbuff, nUsed, 0);
	if (hr == SOCKET_ERROR)
	{
		ASSERT(FALSE);
		hr = WSAGetLastError();
	}

	return hr;
}

CString GetExcutorMappingName( EXCUTORID excId, HWND hActorWnd )
{
	CString strfmt;
	strfmt.Format(TEXT("excutor%dtoactor%x"), excId, hActorWnd);
	return strfmt;
}

CString GetActorMappingName( HWND hActorWnd, EXCUTORID excId )
{
	CString strfmt;
	strfmt.Format(TEXT("actor%xtoexcutor%d"),hActorWnd, excId);
	return strfmt;
}


