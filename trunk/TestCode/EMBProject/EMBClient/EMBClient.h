
// EMBClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEMBClientApp:
// �йش����ʵ�֣������ EMBClient.cpp
//

class CEMBClientApp : public CWinAppEx
{
public:
	CEMBClientApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEMBClientApp theApp;