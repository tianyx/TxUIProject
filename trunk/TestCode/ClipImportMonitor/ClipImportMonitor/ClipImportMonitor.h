/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	ClipImportMonitor.h
	file base:	ClipImportMonitor
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
// ClipImportMonitor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CClipImportMonitorApp:
// �йش����ʵ�֣������ ClipImportMonitor.cpp
//

class CClipImportMonitorApp : public CWinAppEx
{
public:
	CClipImportMonitorApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CClipImportMonitorApp theApp;