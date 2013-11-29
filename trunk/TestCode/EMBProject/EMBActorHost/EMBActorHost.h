/********************************************************************
	created:	2013/11/23
	filename: 	EMBActorHost.h
	author:		tianyx
	purpose:	Actor(exe) 负责excutor注册，启动，关闭; 向excutor提交任务，接受反馈信息 
    History: 
*********************************************************************/

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CEMBActorHostApp:
// 有关此类的实现，请参阅 EMBActorHost.cpp
//

class CEMBActorHostApp : public CWinAppEx
{
public:
	CEMBActorHostApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CEMBActorHostApp theApp;