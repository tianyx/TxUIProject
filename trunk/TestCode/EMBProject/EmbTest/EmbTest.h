
// EmbTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "MiniDump.h"

// CEmbTestApp:
// �йش����ʵ�֣������ EmbTest.cpp
//

class CEmbTestApp : public CWinAppEx
{
public:
	CEmbTestApp();

	CMiniDump m_dump;
// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CEmbTestApp theApp;