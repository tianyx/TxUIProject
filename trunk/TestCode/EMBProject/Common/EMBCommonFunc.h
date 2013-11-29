/********************************************************************
	created:	2013/11/25
	created:	$2013:11:25
	filename: 	EMBCommonFunc.h
	file base:	EMBCommonFunc
	file ext:	h
	author:		tianyx
	
	purpose:	utility function for emb project
*********************************************************************/
#pragma once
#include "IEMBBaseInterface.h"
#include "EmbStructDef.h"

//*Description: load emb plugin from a file
//*Input Param: strFileIn: plugin file
//				hModuleOut: handle of plugin
//				pInterfaceOut: returned interface of  IPluginBaseInterface(IPluginBaseInterfaceVC6)
//*Return Param: 
//*History: 
BOOL TxLoadPlugin(const CString& strFileIn, HMODULE& hModuleOut, LPVOID& pInterfaceOut);

//*Description: load emb plugin from a file
//*Input Param: strFileIn: plugin file
//				hModuleOut: handle of plugin
//				pInterfaceOut: returned interface of  IPluginBaseInterfaceVC6
//*Return Param: 
//*History: 
BOOL TxLoadPluginVC6(const CString& strFileIn, HMODULE& hModuleOut, LPVOID& pInterfaceOut);


BOOL TxUnloadPlugin(HMODULE hModuleIn);


//*Description: like WaitForMultipleObjects
//*Input Param: hWait: wait object,  hQuit: quit object
//*Return Param: 
//*History: 
DWORD TxWaitObjWithQuit(HANDLE hWait, HANDLE hQuit, DWORD dwTimeOut = INFINITE);


//*Description: plugin manager utility
//*Input Param: nType: see ENUM_PLUGINTYPE
//				nSubType: see ENUM_SUBTYPE
//				pPluginMgr: interface for plugin manager
//				pluginOut: see ST_LOADEDPLUGIN
//*Return Param: 
//*History: 
BOOL LoadPluginByPluginMgr(int nType, int nSubType, EMB::ITxUnkown* pPluginMgr, ST_LOADEDPLUGIN& pluginOut);


BOOL UnLoadPluginByPluginMgr(EMB::ITxUnkown* pPluginMgr, ST_LOADEDPLUGIN& pluginOut);


//*Description: for set param to plugin
//*Input Param: pIPlugin: interface of plugin
//				strParam: xml param
//*Return Param: 
//*History: 
BOOL SetPluginParam(EMB::ITxUnkown* pIPlugin, CString& strParam, CString& strRet);

//*Description: for connect plugins
//*Input Param: pIPlugin1(2): interface of plugins
//*Return Param: 
//*History: 
HRESULT ConnectPlugins(EMB::ITxUnkown* pIPlugin1, EMB::ITxUnkown* pIPlugin2);

//*Description: for disconnect plugins
//*Input Param: pIPlugin1(2): interface of plugins
//*Return Param: 
//*History:
HRESULT DisConnectPlugins(EMB::ITxUnkown* pIPlugin1, EMB::ITxUnkown* pIPlugin2);


//*Description: extract first valid node info from xml
//*Input Param: 
//*Return Param: 
//*History: 
BOOL GetXmlFirstNode(const CString& strXmlIn, CString& strNodeOut);


//*Description: extract task basic info from task xml
//*Input Param: infoOut: see ST_TASKBASIC
//*Return Param: 
//*History: 
BOOL GetTaskBasicInfo(const CString& strTaskIn, ST_TASKBASIC& infoOut);


//*Description: extract task main info form task xml
//*Input Param: infoOut: see ST_EMBXMLMAININFO
//*Return Param: 
//*History: 
BOOL GetEmbXmlMainInfo(const CString& strTaskIn, ST_EMBXMLMAININFO& infoOut);


//*Description: send emb main node xml, see EDOC_MAINHEADERFMT
//*Input Param: see embxmltype_xxx
//*Return Param: see EMBERR_xxx
//*History: 
HRESULT SendMainXmlMsg(SOCKET sock, int nXmltype, LPCTSTR szGuid = NULL);


//*Description: make mapping name for executor->actor message transforming  from the input parameters 
//*Input Param: 
//*Return Param: 
//*History: 
CString GetExcutorMappingName(EXCUTORID excId, HWND hActorWnd);

//*Description: make mapping name for actor->executor message transforming from the input parameters
//*Input Param: 
//*Return Param: 
//*History: 
CString GetActorMappingName(HWND hActorWnd, EXCUTORID excId);
