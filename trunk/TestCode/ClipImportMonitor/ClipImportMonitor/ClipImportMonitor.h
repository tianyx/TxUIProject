/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	ClipImportMonitor.h
	file base:	ClipImportMonitor
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
// ClipImportMonitor.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CClipImportMonitorApp:
// 有关此类的实现，请参阅 ClipImportMonitor.cpp
//

class CClipImportMonitorApp : public CWinAppEx
{
public:
	CClipImportMonitorApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CClipImportMonitorApp theApp;