
// ConfigUtility.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "afxwinappex.h"

// CConfigUtilityApp:
// �йش����ʵ�֣������ ConfigUtility.cpp
//

class CConfigUtilityApp : public CWinAppEx
{
public:
	CConfigUtilityApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CConfigUtilityApp theApp;