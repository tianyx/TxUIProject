#pragma once
#include "IEMBBaseInterface.h"
#include "EMBDefine.h"
#include "FGlobal.h"
#include <map>
#include <vector>
using namespace std;
typedef map<HMODULE, HMODULE> MAPMODULES;

namespace EMB{
struct ST_PLUGINMGRDATA
{
	MAPMODULES vModules;
	CString strFile;
	ST_PluginInfo plugInfo;
};
typedef map<TXGUID, ST_PLUGINMGRDATA> MAPPLUGINMAGRDATAS;

class CEMBPluginManager: 
	public IPluginBaseInterface,
	public IPluginManagerInterface
{
public:
	CEMBPluginManager(void);
	virtual ~CEMBPluginManager(void);
	
	//interface for Iunknow
	virtual HRESULT OnFirstInit();
	virtual void	OnFinalRelease();
	//interface for IPluginBaseInterface
	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoInOut);
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);
	//iterface for IPluginManagerInterface
	virtual HRESULT InitPluginsSearch(BOOL bSearchDeep, LPCTSTR szFileExtern);
	virtual HRESULT FindPlugin(const UINT nPluginType, const UINT nSubType, GUID& guidOut);
	virtual HANDLE  LoadPlugin(const GUID guidIn, IPluginBaseInterface*& pInterfaceOut);
	virtual HRESULT UnloadPlugin(const GUID guidIn, HANDLE handle);
private:
	void Init();
	void UnInit();
	MAPPLUGINMAGRDATAS m_mapPluginInfo;
	CString m_strExten;
	BOOL m_bDeepSearch;

};


}//namespace EMB

extern EMB::CEMBPluginManager* g_pPluginInstane;
extern HMODULE g_hGlobalDllModule;