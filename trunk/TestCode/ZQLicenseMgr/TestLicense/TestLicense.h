
// TestLicense.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestLicenseApp:
// �йش����ʵ�֣������ TestLicense.cpp
//

class CTestLicenseApp : public CWinAppEx
{
public:
	CTestLicenseApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestLicenseApp theApp;