/********************************************************************
	created:	2013/11/28
	filename: 	Util.h
	author:		zhangjianchao
	purpose:	����xml�ļ� 
    History: 
*********************************************************************/

#pragma once
#include "Markup.h"
#include "EmbStructDef.h"
#include "EMBDocDef.h"


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
	Input��		strFileName �ļ��� tskReport ������Ϣ
	Return:		true �ɹ�; 
	History��
	*/
	static bool XmlFileAppend(const CString& strFileName, ST_TASKREPORT& tskReport);
	static bool QueryXmlFile(const CString& strFileName, const CString& strTaskGuid, ST_TASKREPORT& tskInfor );
};
