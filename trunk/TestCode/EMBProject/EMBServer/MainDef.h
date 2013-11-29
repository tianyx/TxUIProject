/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	MainDef.h
	author:		zhouliang
	
	purpose:	初始化管理类
*********************************************************************/

#pragma once
#include "TxLogManager.h"
#include "IEMBBaseInterface.h"
#include <vector>
#include "EmbStructDef.h"
using namespace std;

//全局信息存储结构体
struct ST_GLOBAL
{
	CString szAppPath;
	CString szIniPath;
	CString szCfgSrcPath;
	CString szPluginPath;
	CString szLogFile;
	VECLOADEDPLUGINS vPlugins;
	
	ST_GLOBAL()
	{
		
	}


};

//全局存储信息
extern ST_GLOBAL g_GlobalInfo;
//插件管理插件动态库加载指针
extern HMODULE g_hModulePluginMgr;
// 插件管理动态库指针
extern EMB::IPluginBaseInterface* g_pIPluginMgr;

BOOL InitGlobalConfig();
BOOL LoadPluginManager();
BOOL UnLoadPluginManager();


BOOL InitServer();
BOOL UnInitServer();
BOOL RunServer(BOOL bRun = TRUE);
