// TxDrawBase.h : TxDrawBase DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTxDrawBaseApp
// �йش���ʵ�ֵ���Ϣ������� TxDrawBase.cpp
//

class CTxDrawBaseApp : public CWinApp
{
public:
	CTxDrawBaseApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
