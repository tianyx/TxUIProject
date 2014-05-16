/********************************************************************
	created:	2013/11/27
	filename: 	MainDef.h
	author:		zhangjianchao
	purpose:	数据定义：执行者进程信息
    History: 
*********************************************************************/

#pragma once
#include "TxLogManager.h"
#include "embstructdef.h"

struct ST_GLOBAL
{
	CString szAppPath;
	CString szIniPath;
	CString szDBConn;
	ST_GLOBAL()
	{
	
	}
};


BOOL InitGlobalConfig();

extern ST_GLOBAL g_GlobalInfo;

extern int DBKEY_EMB;
