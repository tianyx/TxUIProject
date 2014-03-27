#pragma once

typedef void* (*TXMALLOC)(size_t size );

typedef void (*TXFREE)(void *memblock  );

class CEMBWorkString
{
public:
	CEMBWorkString(const TCHAR *str=NULL); //���캯��
	CEMBWorkString(const CEMBWorkString &other); //�������캯��
	~CEMBWorkString(void); //��������
	CEMBWorkString& operator=(const CEMBWorkString &other); //�ȺŲ���������
	operator const TCHAR*(){return m_data;}
	CEMBWorkString& operator =(const CString& other);
	int GetLength(){return m_nStrLen;};
private:
	TCHAR *m_data; //ָ��
	int m_nStrLen;
	TXMALLOC pMalloc;
	TXFREE	pFree;

};


