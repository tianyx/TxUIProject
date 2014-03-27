#include "StdAfx.h"
#include "EMBWorkString.h"
#include "StrConvert.h"
#include "shlwapi.h"

CEMBWorkString::~CEMBWorkString()
{
	pFree(m_data); //析构函数，释放地址空间
}
CEMBWorkString::CEMBWorkString(const TCHAR *str)
{
	pMalloc = malloc;
	pFree = free;
	if (str==NULL)//当初始化串不存在的时候，为m_data申请一个空间存放'\0'；
	{
		m_data=(TCHAR*)pMalloc(sizeof(TCHAR));
		*m_data=_T('\0');
		m_nStrLen = 0;
	}
	else//当初始化串存在的时候，为m_data申请同样大小的空间存放该串；
	{
		m_nStrLen =TxStrLen(str);
		m_data=(TCHAR*)pMalloc(sizeof(TCHAR) *(m_nStrLen+1));
		StrCpy(m_data,str);
	}
}


CEMBWorkString::CEMBWorkString(const CEMBWorkString &other)//拷贝构造函数，功能与构造函数类似。
{
	pMalloc = malloc;
	pFree = free;

	m_nStrLen= other.m_nStrLen;
	m_data=(TCHAR*)pMalloc(sizeof(TCHAR) *(m_nStrLen+1));
	StrCpy(m_data,other.m_data);
}
CEMBWorkString& CEMBWorkString::operator =(const CEMBWorkString &other) 
{
	if (this==&other)//当地址相同时，直接返回；
		return *this; 

	pFree(m_data);//当地址不相同时，删除原来申请的空间，重新开始构造；

	int length= TxStrLen (other.m_data);
	m_nStrLen = other.m_nStrLen;
	m_data=(TCHAR*)pMalloc(sizeof(TCHAR) *(m_nStrLen+1));
	StrCpy(m_data,other.m_data);

	return *this; 
}

CEMBWorkString& CEMBWorkString::operator=( const CString& other )
{
	pFree(m_data);

	m_nStrLen = other.GetLength();
	m_data = (TCHAR*)pMalloc(sizeof(TCHAR) *(m_nStrLen+1));
	StrCpy(m_data, (LPCTSTR)other);
	return *this;
}
