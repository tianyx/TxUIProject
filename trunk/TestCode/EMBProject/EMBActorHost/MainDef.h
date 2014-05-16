/********************************************************************
	created:	2013/11/27
	filename: 	MainDef.h
	author:		zhangjianchao
	purpose:	数据定义：执行者进程信息
    History: 
*********************************************************************/

#pragma once
#include "TxLogManager.h"
#include "IEMBBaseInterface.h"
#include "embstructdef.h"

struct ST_GLOBAL
{
	CString szAppPath;
	CString szIniPath;
	CString szCfgSrcPath;
	CString szPluginPath;
	CString szLogFile;
	CString strGuid;
	CString strTitle;
	GUID guid;

	CString strHostName;
	CString szExcutorPath;
	ST_EXCUTORREG excInfo;

	ST_GLOBAL()
	{
		guid = GUID_NULL;
	}
};

// 执行者进程信息
struct tagExecutorProcess
{
	CString m_strName;  // 名称
	CString m_strID;    // 标识
	CString m_strState; // 状态：迁移,转码，技审，MD5 
	CString m_strTaskGuid; // 任务标识
	int		m_nPercent;    // 进度
};

BOOL InitGlobalConfig();
BOOL LoadActorHost(CString& strConfig);
BOOL UnloadActorHost();

BOOL LoadPluginManager();
BOOL UnLoadPluginManager();

extern ST_GLOBAL g_GlobalInfo;

extern HMODULE g_hActorPlugin;
extern EMB::IPluginBaseInterface* g_pIActorPlugin;

extern HMODULE g_hModulePluginMgr;
extern EMB::IPluginBaseInterface* g_pIPluginMgr;