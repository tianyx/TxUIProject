
// TS2ASISvr.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTS2ASISvrApp:
// �йش����ʵ�֣������ TS2ASISvr.cpp
//

class CTS2ASISvrApp : public CWinAppEx
{
public:
	CTS2ASISvrApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTS2ASISvrApp theApp;