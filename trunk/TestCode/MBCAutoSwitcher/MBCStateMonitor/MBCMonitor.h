
// MBCMonitor.h : MBCMonitor Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "MiniDump.h"

// CMBCMonitorApp:
// �йش����ʵ�֣������ MBCMonitor.cpp
//

class CMBCMonitorApp : public CWinApp
{
public:
	CMBCMonitorApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	CMiniDump m_dump;
public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CMBCMonitorApp theApp;
