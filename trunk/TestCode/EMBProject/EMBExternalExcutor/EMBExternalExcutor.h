
// EMBExternalExcutor.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEMBExternalExcutorApp:
// �йش����ʵ�֣������ EMBExternalExcutor.cpp
//

class CEMBExternalExcutorApp : public CWinAppEx
{
public:
	CEMBExternalExcutorApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CEMBExternalExcutorApp theApp;