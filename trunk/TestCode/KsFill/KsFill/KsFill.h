
// KsFill.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CKsFillApp:
// �йش����ʵ�֣������ KsFill.cpp
//

class CKsFillApp : public CWinAppEx
{
public:
	CKsFillApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CKsFillApp theApp;