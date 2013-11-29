/********************************************************************
	created:	2013/11/28
	filename: 	Util.h
	author:		zhangjianchao
	purpose:	����xml�ļ� 
    History: 
*********************************************************************/

#pragma once
#include "Markup.h"

// �ͷ�ָ��
#define PTRDELETE(ptr)	\
	if (ptr)			\
	{					\
		delete ptr;		\
		ptr = NULL;		\
	}					\

class CUtil
{
public:
	CUtil(void);
	~CUtil(void);

public:
	/*
    Description���ύ����
	Input��		strFileName �ļ��� strXmlContent �ļ�����
	Return:		true �ɹ�; 
	History��
	*/
	static bool SaveXmlFile(const CString& strFileName, const CString& strXmlContent);
};
