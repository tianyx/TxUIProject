
// UIDlgTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUIDlgTestApp:
// �йش����ʵ�֣������ UIDlgTest.cpp
//

class CUIDlgTestApp : public CWinAppEx
{
public:
	CUIDlgTestApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUIDlgTestApp theApp;