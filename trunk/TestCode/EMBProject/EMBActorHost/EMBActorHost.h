/********************************************************************
	created:	2013/11/23
	filename: 	EMBActorHost.h
	author:		tianyx
	purpose:	Actor(exe) ����excutorע�ᣬ�������ر�; ��excutor�ύ���񣬽��ܷ�����Ϣ 
    History: 
*********************************************************************/

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CEMBActorHostApp:
// �йش����ʵ�֣������ EMBActorHost.cpp
//

class CEMBActorHostApp : public CWinAppEx
{
public:
	CEMBActorHostApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CEMBActorHostApp theApp;