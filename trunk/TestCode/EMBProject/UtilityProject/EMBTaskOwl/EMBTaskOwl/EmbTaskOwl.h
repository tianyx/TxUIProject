
// EmbTaskOwl.h : EmbTaskOwl Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include "MiniDump.h"

// CEmbTaskOwlApp:
// �йش����ʵ�֣������ EmbTaskOwl.cpp
//

class CEmbTaskOwlApp : public CWinApp
{
public:
	CEmbTaskOwlApp();

CMiniDump m_dump;
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CEmbTaskOwlApp theApp;
