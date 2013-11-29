/********************************************************************
	created:	2013/11/28
	filename: 	Util.h
	author:		zhangjianchao
	purpose:	保存xml文件 
    History: 
*********************************************************************/

#pragma once
#include "Markup.h"

// 释放指针
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
    Description：提交任务
	Input：		strFileName 文件名 strXmlContent 文件内容
	Return:		true 成功; 
	History：
	*/
	static bool SaveXmlFile(const CString& strFileName, const CString& strXmlContent);
};
