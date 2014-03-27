#pragma once

typedef void* (*TXMALLOC)(size_t size );

typedef void (*TXFREE)(void *memblock  );

class CEMBWorkString
{
public:
	CEMBWorkString(const TCHAR *str=NULL); //构造函数
	CEMBWorkString(const CEMBWorkString &other); //拷贝构造函数
	~CEMBWorkString(void); //析构函数
	CEMBWorkString& operator=(const CEMBWorkString &other); //等号操作符重载
	operator const TCHAR*(){return m_data;}
	CEMBWorkString& operator =(const CString& other);
	int GetLength(){return m_nStrLen;};
private:
	TCHAR *m_data; //指针
	int m_nStrLen;
	TXMALLOC pMalloc;
	TXFREE	pFree;

};


