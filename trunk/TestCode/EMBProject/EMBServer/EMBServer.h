/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:04
	filename: 	EMBServer.h
	author:		zhouliang
	
	purpose:	�������������
*********************************************************************/
// EMBServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "MiniDump.h"

// CEMBServerApp:
// �йش����ʵ�֣������ EMBServer.cpp
// 

class CEMBServerApp : public CWinAppEx
{
public:
	CEMBServerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
protected:
	CMiniDump m_dump;
};

extern CEMBServerApp theApp;