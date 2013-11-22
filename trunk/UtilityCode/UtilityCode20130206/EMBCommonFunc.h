#pragma once
#include "IEMBBaseInterface.h"
#include "EmbStructDef.h"

BOOL TxLoadPlugin(const CString& strFileIn, HMODULE& hModuleOut, LPVOID& pInterfaceOut);
BOOL TxLoadPluginVC6(const CString& strFileIn, HMODULE& hModuleOut, LPVOID& pInterfaceOut);

BOOL TxUnloadPlugin(HMODULE hModuleIn);


DWORD TxWaitObjWithQuit(HANDLE hWait, HANDLE hQuit, DWORD dwTimeOut = INFINITE);

//plugin manager utility
BOOL LoadPluginByPluginMgr(int nType, int nSubType, EMB::ITxUnkown* pPluginMgr, ST_LOADEDPLUGIN& pluginOut);
BOOL UnLoadPluginByPluginMgr(EMB::ITxUnkown* pPluginMgr, ST_LOADEDPLUGIN& pluginOut);

BOOL SetPluginParam(EMB::ITxUnkown* pIPlugin, CString& strParam, CString& strRet);

HRESULT ConnectPlugins(EMB::ITxUnkown* pIPlugin1, EMB::ITxUnkown* pIPlugin2);
HRESULT DisConnectPlugins(EMB::ITxUnkown* pIPlugin1, EMB::ITxUnkown* pIPlugin2);

BOOL GetXmlFirstNode(const CString& strXmlIn, CString& strNodeOut);

BOOL GetTaskBasicInfo(const CString& strTaskIn, ST_TASKBASIC& infoOut);
BOOL GetEmbXmlMainInfo(const CString& strTaskIn, ST_EMBXMLMAININFO& infoOut);

HRESULT SendMainXmlMsg(SOCKET sock, int nXmltype, LPCTSTR szGuid = NULL);

CString GetExcutorMappingName(EXCUTORID excId, HWND hActorWnd);
CString GetActorMappingName(HWND hActorWnd, EXCUTORID excId);
