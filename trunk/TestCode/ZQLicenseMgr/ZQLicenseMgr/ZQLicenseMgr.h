
// ZQLicenseMgr.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "TxSystemOp.h"

// CZQLicenseMgrApp:
// �йش����ʵ�֣������ ZQLicenseMgr.cpp
//

class CZQLicenseMgrApp : public CWinAppEx
{
public:
	CZQLicenseMgrApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CZQLicenseMgrApp theApp;