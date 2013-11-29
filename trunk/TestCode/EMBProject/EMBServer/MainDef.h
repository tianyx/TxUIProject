/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	MainDef.h
	author:		zhouliang
	
	purpose:	��ʼ��������
*********************************************************************/

#pragma once
#include "TxLogManager.h"
#include "IEMBBaseInterface.h"
#include <vector>
#include "EmbStructDef.h"
using namespace std;

//ȫ����Ϣ�洢�ṹ��
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

//ȫ�ִ洢��Ϣ
extern ST_GLOBAL g_GlobalInfo;
//�����������̬�����ָ��
extern HMODULE g_hModulePluginMgr;
// �������̬��ָ��
extern EMB::IPluginBaseInterface* g_pIPluginMgr;

BOOL InitGlobalConfig();
BOOL LoadPluginManager();
BOOL UnLoadPluginManager();


BOOL InitServer();
BOOL UnInitServer();
BOOL RunServer(BOOL bRun = TRUE);
