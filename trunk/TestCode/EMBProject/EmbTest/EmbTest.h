
// EmbTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEmbTestApp:
// �йش����ʵ�֣������ EmbTest.cpp
//

class CEmbTestApp : public CWinAppEx
{
public:
	CEmbTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CEmbTestApp theApp;