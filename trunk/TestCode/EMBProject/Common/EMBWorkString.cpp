#include "StdAfx.h"
#include "EMBWorkString.h"
#include "StrConvert.h"
#include "shlwapi.h"

CEMBWorkString::~CEMBWorkString()
{
	pFree(m_data); //�����������ͷŵ�ַ�ռ�
}
CEMBWorkString::CEMBWorkString(const TCHAR *str)
{
	pMalloc = malloc;
	pFree = free;
	if (str==NULL)//����ʼ���������ڵ�ʱ��Ϊm_data����һ���ռ���'\0'��
	{
		m_data=(TCHAR*)pMalloc(sizeof(TCHAR));
		*m_data=_T('\0');
		m_nStrLen = 0;
	}
	else//����ʼ�������ڵ�ʱ��Ϊm_data����ͬ����С�Ŀռ��Ÿô���
	{
		m_nStrLen =TxStrLen(str);
		m_data=(TCHAR*)pMalloc(sizeof(TCHAR) *(m_nStrLen+1));
		StrCpy(m_data,str);
	}
}


CEMBWorkString::CEMBWorkString(const CEMBWorkString &other)//�������캯���������빹�캯�����ơ�
{
	pMalloc = malloc;
	pFree = free;

	m_nStrLen= other.m_nStrLen;
	m_data=(TCHAR*)pMalloc(sizeof(TCHAR) *(m_nStrLen+1));
	StrCpy(m_data,other.m_data);
}
CEMBWorkString& CEMBWorkString::operator =(const CEMBWorkString &other) 
{
	if (this==&other)//����ַ��ͬʱ��ֱ�ӷ��أ�
		return *this; 

	pFree(m_data);//����ַ����ͬʱ��ɾ��ԭ������Ŀռ䣬���¿�ʼ���죻

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
