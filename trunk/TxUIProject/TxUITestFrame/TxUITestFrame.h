
// TxUITestFrame.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTxUITestFrameApp:
// �йش����ʵ�֣������ TxUITestFrame.cpp
//

class CTxUITestFrameApp : public CWinAppEx
{
public:
	CTxUITestFrameApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CTxUITestFrameApp theApp;