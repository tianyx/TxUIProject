
// FtpTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFtpTestApp:
// �йش����ʵ�֣������ FtpTest.cpp
//

class CFtpTestApp : public CWinAppEx
{
public:
	CFtpTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFtpTestApp theApp;