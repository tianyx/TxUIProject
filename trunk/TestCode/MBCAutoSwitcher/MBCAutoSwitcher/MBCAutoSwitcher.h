
// MBCAutoSwitcher.h : MBCAutoSwitcher Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "MiniDump.h"

// CMBCAutoSwitcherApp:
// �йش����ʵ�֣������ MBCAutoSwitcher.cpp
//

class CMBCAutoSwitcherApp : public CWinApp
{
public:
	CMBCAutoSwitcherApp();


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

extern CMBCAutoSwitcherApp theApp;
