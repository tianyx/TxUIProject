
// MulticastEndTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMulticastEndTestApp:
// �йش����ʵ�֣������ MulticastEndTest.cpp
//

class CMulticastEndTestApp : public CWinAppEx
{
public:
	CMulticastEndTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CMulticastEndTestApp theApp;