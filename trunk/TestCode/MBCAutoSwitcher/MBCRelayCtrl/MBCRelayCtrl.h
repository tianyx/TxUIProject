// MBCRelayCtrl.h : MBCRelayCtrl DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMBCRelayCtrlApp
// �йش���ʵ�ֵ���Ϣ������� MBCRelayCtrl.cpp
//

class CMBCRelayCtrlApp : public CWinApp
{
public:
	CMBCRelayCtrlApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
