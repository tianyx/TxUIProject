#pragma once
#include "IEMBBaseInterface.h"
#include <map>
#include <vector>
using namespace std;
namespace EMB{
struct ST_PLUGINMGRDATA
{
	vector<HMODULE> vModules;
	CString strFile;
	ST_PluginInfo plugInfo;
};
typedef map<CString, ST_PLUGINMGRDATA> MAPPLUGINMAGRDATAS;

class CEMBPluginManager: public IPluginBaseInterface, public IPluginManagerInterface
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
	virtual HRESULT FindPlugin(const UINT nPluginType, const UINT nSubType, GUID& guidOut);
	virtual HRESULT LoadPlugin(GUID& guidIn, IPluginBaseInterface*& pInterfaceOut);

private:
	void Init();
	void UnInit();
	MAPPLUGINMAGRDATAS m_mapPluginInfo;

};

}//namespace EMB