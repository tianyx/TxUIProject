
// 3DTestdlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy3DTestdlgApp:
// �йش����ʵ�֣������ 3DTestdlg.cpp
//

class CMy3DTestdlgApp : public CWinAppEx
{
public:
	CMy3DTestdlgApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy3DTestdlgApp theApp;