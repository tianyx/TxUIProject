#include "StdAfx.h"
#include "EMBPluginManager.h"
#include "FGlobal.h"
#include "EMBCommonFunc.h"
#include "TxLogManager.h"
using namespace EMB;
CEMBPluginManager::CEMBPluginManager(void)
{
}

CEMBPluginManager::~CEMBPluginManager(void)
{
}

HRESULT CEMBPluginManager::QueryPluginInfo( VECPLUGINFOS& vInfoInOut )
{
	ST_PluginInfo info;
	info.pluginGuid = GuidEMBPlugin_IPluginManager;
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
	CFileFind ffind;
	CString strFileFmt(GetAppPath().c_str());
	strFileFmt.TrimRight('\\');
	strFileFmt += "\\plugins\\*.dll";
	BOOL bFind = ffind.FindFile(strFileFmt);
	if (!bFind)
	{
		CFWriteLog(TEXT("²å¼þ»ñÈ¡Ê§°Ü"));
		return;
	}
	//m_vecFiles.push_back(ffind.GetFilePath());
	while(bFind)
	{
		bFind = ffind.FindNextFile();
		CString strDllFile =ffind.GetFilePath();
		EMB::IPluginBaseInterface* pPluginBase = NULL;
		HMODULE hPluginModule = NULL;
		::LoadPlugin(strDllFile, hPluginModule, (LPVOID&)pPluginBase);
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
					CString strGuid = Guid2String(mgrdata.plugInfo.pluginGuid);
					if(m_mapPluginInfo.find(strGuid) != m_mapPluginInfo.end())
					{
						CFWriteLog(TEXT("plugin guid already defined %s, %s"), mgrdata.strFile, strGuid);
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
			UnloadPlugin(hPluginModule);
		}

	}
	ffind.Close();
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

HRESULT CEMBPluginManager::LoadPlugin( GUID& guidIn, IPluginBaseInterface*& pInterfaceOut )
{
	HRESULT hr = S_FALSE;
	CString strGuid = Guid2String(guidIn);
	MAPPLUGINMAGRDATAS::iterator itf = m_mapPluginInfo.find(strGuid);
	if (itf != m_mapPluginInfo.end())
	{
		//load new;
		HMODULE hModule = NULL;
		hr =::LoadPlugin(itf->second.strFile, hModule, (LPVOID&)pInterfaceOut);
		if (hModule)
		{
			itf->second.vModules.push_back(hModule);
		}
	}

	return hr;
}

void CEMBPluginManager::OnFinalRelease()
{
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
		vector<HMODULE>& vModule = itb->second.vModules;
		for (size_t i = 0; i < vModule.size(); ++i)
		{
			UnloadPlugin(vModule[i]);
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
	GetTxLogMgr()->AddNewLogFile(PLUGINMGR_LOGKEY, strFile);
	Init();
	return S_OK;
}

