
// MonitorTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMonitorTestApp:
// �йش����ʵ�֣������ MonitorTest.cpp
//

class CMonitorTestApp : public CWinAppEx
{
public:
	CMonitorTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CMonitorTestApp theApp;