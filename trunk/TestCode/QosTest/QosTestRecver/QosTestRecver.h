
// QosTestRecver.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CQosTestRecverApp:
// �йش����ʵ�֣������ QosTestRecver.cpp
//

class CQosTestRecverApp : public CWinAppEx
{
public:
	CQosTestRecverApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CQosTestRecverApp theApp;