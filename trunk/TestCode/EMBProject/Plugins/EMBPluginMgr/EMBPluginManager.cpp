#include "StdAfx.h"
#include "EMBPluginManager.h"
#include "FGlobal.h"
#include "EMBCommonFunc.h"
#include "TxLogManager.h"
#include "StrConvert.h"
using namespace EMB;
CEMBPluginManager::CEMBPluginManager(void)
{
	m_bDeepSearch = FALSE;
	m_strExten = TEXT("dll");
}

CEMBPluginManager::~CEMBPluginManager(void)
{
	g_pPluginInstane = NULL;
}

HRESULT CEMBPluginManager::QueryPluginInfo( VECPLUGINFOS& vInfoInOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_PPluginMgr;
	info.nPlugInType = PluinType_PluginManager;
	info.nSubType = SubType_None;
	vInfoInOut.push_back(info);
	return S_OK;
}

HRESULT CEMBPluginManager::QueryInterface( const GUID& guidIn, LPVOID& pInterfaceOut )
{
	pInterfaceOut = NULL;

	if (guidIn == GuidEMBPlugin_IPluginManager)
	{
		pInterfaceOut = dynamic_cast<IPluginManagerInterface*>(this);
		AddRef();
		return S_OK;
	}
	else if (guidIn == GuidEMBPlugin_IBase)
	{
		pInterfaceOut = dynamic_cast<IPluginBaseInterface*>(this);
		AddRef();
		return S_OK;
	}
	else
	{
		return __super::QueryInterface(guidIn, pInterfaceOut);
	}
}

void CEMBPluginManager::Init()
{
	char szPath[MAX_PATH];
	GetModuleFileName(g_hGlobalDllModule, szPath, MAX_PATH);
	CString strThisPath = szPath;
	CString strFileFmt(GetAppPath().c_str());
	VECSTRINGS vFiles;
	GetFilesInFolder(strFileFmt, vFiles, m_strExten, m_bDeepSearch);
	for (size_t i = 0; i < vFiles.size(); ++i)
	{
		CString strDllFile =vFiles[i];
		if (strDllFile.CompareNoCase(strThisPath) == 0)
		{
			continue;
		}
	
		EMB::IPluginBaseInterface* pPluginBase = NULL;
		HMODULE hPluginModule = NULL;
		::TxLoadPlugin(strDllFile, hPluginModule, (LPVOID&)pPluginBase);
		if (hPluginModule && pPluginBase)
		{
			VECPLUGINFOS vtmpInfos;
			HRESULT hr = pPluginBase->QueryPluginInfo(vtmpInfos);
			if (hr == S_OK)
			{
				for (size_t i = 0; i < vtmpInfos.size(); ++i)
				{
					ST_PLUGINMGRDATA mgrdata;
					mgrdata.strFile = strDllFile;
					mgrdata.plugInfo = vtmpInfos[i];
					TXGUID strGuid = mgrdata.plugInfo.pluginGuid;
					if(m_mapPluginInfo.find(strGuid) != m_mapPluginInfo.end())
					{
						CFWriteLog(TEXT("plugin guid already defined %s, %s"), mgrdata.strFile, (CString)strGuid);
					}
					m_mapPluginInfo[strGuid] = mgrdata;
				}
			}
			else
			{
				ASSERT(FALSE);
				CFWriteLog(TEXT("plugin info get error %s"), strDllFile);
			}
			pPluginBase->Release();
			TxUnloadPlugin(hPluginModule);
		}

	}
}

HRESULT CEMBPluginManager::FindPlugin( const UINT nPluginType, const UINT nSubType, GUID& guidOut )
{
	HRESULT hr = S_FALSE;
	MAPPLUGINMAGRDATAS::iterator itb = m_mapPluginInfo.begin();
	MAPPLUGINMAGRDATAS::iterator ite = m_mapPluginInfo.end();
	for (; itb != ite; ++itb)
	{
		if (itb->second.plugInfo.nPlugInType == nPluginType)
		{
			if (nSubType == 0 ||itb->second.plugInfo.nSubType == nSubType)
			{
				//found it;
				guidOut = itb->second.plugInfo.pluginGuid;
				hr = S_OK;
				break;
			}
		}
	}

	return hr;
}

HANDLE CEMBPluginManager::LoadPlugin( const GUID guidIn, IPluginBaseInterface*& pInterfaceOut )
{
	TXGUID guid = guidIn;
	MAPPLUGINMAGRDATAS::iterator itf = m_mapPluginInfo.find(guid);
	if (itf != m_mapPluginInfo.end())
	{
		//load new;
		HMODULE hModule = NULL;
		::TxLoadPlugin(itf->second.strFile, hModule, (LPVOID&)pInterfaceOut);
		if (hModule)
		{
			itf->second.vModules[hModule] = hModule;
		}
		return hModule;
	}

	return NULL;
}

void CEMBPluginManager::OnFinalRelease()
{
	g_pPluginInstane = NULL;
	UnInit();
	ReleaseTxLogMgr();
	delete this;
}

void CEMBPluginManager::UnInit()
{
	MAPPLUGINMAGRDATAS::iterator itb = m_mapPluginInfo.begin();
	MAPPLUGINMAGRDATAS::iterator ite = m_mapPluginInfo.end();
	for (; itb != ite; ++itb)
	{
		MAPMODULES& vModule = itb->second.vModules;
		MAPMODULES::iterator itb = vModule.begin();
		MAPMODULES::iterator ite = vModule.end();
		for (; itb != ite; ++itb)
		{
			TxUnloadPlugin(itb->first);
		}
		vModule.clear();
	}

	//
	m_mapPluginInfo.clear();
}

HRESULT CEMBPluginManager::OnFirstInit()
{
	//init all
	CString strFile = GetAppPath().c_str();
	strFile +=TEXT("\\log\\PluginMgr.log");
	GetTxLogMgr()->AddNewLogFile(LOGKEY_PLUGINMGR, strFile);
	return S_OK;
}


HRESULT EMB::CEMBPluginManager::UnloadPlugin(const GUID guidIn, HANDLE handle )
{
	HRESULT hr = S_OK;
	if (guidIn == GUID_NULL)
	{
		if (handle == NULL)
		{
			return E_INVALIDARG;
		}
		//find all handle
		hr = E_FAIL;
		MAPPLUGINMAGRDATAS::iterator itb = m_mapPluginInfo.begin();
		MAPPLUGINMAGRDATAS::iterator ite = m_mapPluginInfo.end();
		for (; itb != ite; ++itb)
		{
			MAPMODULES& vModule = itb->second.vModules;
			MAPMODULES::iterator itfH =vModule.find((HMODULE)handle);
			if (itfH != vModule.end())
			{
				TxUnloadPlugin(itfH->first);
				vModule.erase(itfH);
				hr = S_OK;
				break;
			}
		}
		
		return hr;
	}


	MAPPLUGINMAGRDATAS::iterator itf = m_mapPluginInfo.find(guidIn);
	if (itf != m_mapPluginInfo.end())
	{
		MAPMODULES& vModule = itf->second.vModules;
		if (handle != NULL)
		{
			MAPMODULES::iterator itfH =vModule.find((HMODULE)handle);
			if (itfH != vModule.end())
			{
				TxUnloadPlugin(itfH->first);
				vModule.erase(itfH);
			}
		}
		else
		{
			MAPMODULES::iterator itb = vModule.begin();
			MAPMODULES::iterator ite = vModule.end();
			for (; itb != ite; ++itb)
			{
				TxUnloadPlugin(itb->first);
			}
			vModule.clear();
		}
	}
	else
	{
		hr = S_FALSE;
	}

	return S_FALSE;
}

HRESULT EMB::CEMBPluginManager::InitPluginsSearch( BOOL bDeepSearch, LPCTSTR szFileExtern )
{
	m_bDeepSearch = bDeepSearch;
	if (szFileExtern != NULL)
	{
		m_strExten = szFileExtern;
	}

	Init();

	return S_OK;
}
