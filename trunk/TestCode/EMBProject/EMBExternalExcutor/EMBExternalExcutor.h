/********************************************************************
	created:	2013/11/23
	filename: 	EMBExternalExcutor.h
	author:		tianyx
	purpose:	ִ����(exe) ���ù���dll,���ܴ���dll������Ϣ
    History: 
*********************************************************************/


#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEMBExternalExcutorApp:
// �йش����ʵ�֣������ EMBExternalExcutor.cpp
//

class CEMBExternalExcutorApp : public CWinAppEx
{
public:
	CEMBExternalExcutorApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CEMBExternalExcutorApp theApp;