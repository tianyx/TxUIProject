/********************************************************************
	created:	2013/11/23
	filename: 	EMBExternalExcutor.h
	author:		tianyx
	purpose:	执行者(exe) 调用功能dll,接受处理dll反馈信息
    History: 
*********************************************************************/


#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CEMBExternalExcutorApp:
// 有关此类的实现，请参阅 EMBExternalExcutor.cpp
//

class CEMBExternalExcutorApp : public CWinAppEx
{
public:
	CEMBExternalExcutorApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CEMBExternalExcutorApp theApp;