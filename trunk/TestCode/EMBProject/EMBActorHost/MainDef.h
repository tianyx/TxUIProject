/********************************************************************
	created:	2013/11/27
	filename: 	MainDef.h
	author:		zhangjianchao
	purpose:	���ݶ��壺ִ���߽�����Ϣ
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

// ִ���߽�����Ϣ
struct tagExecutorProcess
{
	CString m_strName;  // ����
	CString m_strID;    // ��ʶ
	CString m_strState; // ״̬��Ǩ��,ת�룬����MD5 
	CString m_strTaskGuid; // �����ʶ
	int		m_nPercent;    // ����
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