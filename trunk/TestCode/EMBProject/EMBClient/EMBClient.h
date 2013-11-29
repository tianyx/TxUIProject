/********************************************************************
	created:	2013/11/23
	filename: 	EMBClient.h
	author:		tianyx
	purpose:	exe; 输入任务信息（迁移，转码，技审，MD5）, 将信息提交给Actor.exe
    History: 
*********************************************************************/

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CEMBClientApp:
// 有关此类的实现，请参阅 EMBClient.cpp
//

class CEMBClientApp : public CWinAppEx
{
public:
	CEMBClientApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CEMBClientApp theApp;