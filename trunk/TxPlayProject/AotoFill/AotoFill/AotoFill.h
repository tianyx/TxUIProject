
// AotoFill.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAotoFillApp:
// �йش����ʵ�֣������ AotoFill.cpp
//

class CAotoFillApp : public CWinAppEx
{
public:
	CAotoFillApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAotoFillApp theApp;