
// MBCWatchDog.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "MiniDump.h"

// CMBCWatchDogApp:
// �йش����ʵ�֣������ MBCWatchDog.cpp
//

class CMBCWatchDogApp : public CWinAppEx
{
public:
	CMBCWatchDogApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��
	CMiniDump m_dump;
	DECLARE_MESSAGE_MAP()
};

extern CMBCWatchDogApp theApp;