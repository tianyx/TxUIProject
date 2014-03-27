/////////////////////////////////////////////////////////////////////////////
////
////
#ifndef _MEDIASAMPLE_H_
#define _MEDIASAMPLE_H_
#include "StdAfx.h"
#include <CRTDBG.H>
#include <Windows.h>


// CQueue
//
//���ζ��У���ʼ����ڵ���Ŀ�Ϳռ䣬����"��","д"�źſ��Ʋ�ͬ�̶߳Զ��е�д��Ͷ���������
//
#define DEFAULT_QSIZE  10

template <class T> class CQueue2 {
private:
	struct Node
	{
		T*      Data;
		Node    *next;

	};

    HANDLE          hSemPut;        // ����"д"�����ź�
    HANDLE          hSemGet;        // ����"��"�����ź�
    CRITICAL_SECTION CritSect;      // ���ݸı�ʱ�����ٽ���
    int             nMax;           // �����е����ڵ���
	int             m_iCount;         //�����е���Ч�ڵ�
	Node*           m_Head;         //����ͷ�ڵ�
	Node*           m_End;          //����β���
    void Initialize(int n) {
        nMax = n;
        InitializeCriticalSection(&CritSect);
		//������д���ź�
        hSemPut = CreateSemaphore(NULL, n, n, NULL);
		//�����������ź�
        hSemGet = CreateSemaphore(NULL, 0, n, NULL);

		m_iCount = 0;
		m_Head = m_End = new Node;
		m_Head->Data   = new T;
		m_Head->next = m_End->next = m_End;//����ͷ��㣬ָ���Լ�

		for (int i = 1; i < nMax; i++)
		{
            m_Head->next = new Node;
			m_Head       = m_Head->next;
			m_Head->Data = new T;
			m_Head->next = m_End;
		}
		m_Head = m_End;
    }
	
	
public:
    CQueue2(int n) {
        Initialize(n);
    }
	
    CQueue2() {
        Initialize(DEFAULT_QSIZE);
    }
	
    ~CQueue2() {
		OutputDebugString("WWW:free Queue memory!\n");
		Node* work = NULL;
		Node* begin = m_Head;
		int i = 0;
		while (m_Head->next !=begin)
		{	
			work = m_Head->next;
			if (m_Head->Data)
			{
				delete m_Head->Data;
				m_Head->Data = NULL;
			}
			if (m_Head)
			{
				delete m_Head;
				m_Head       = NULL;
			}	
			m_Head = work;
		}
		delete m_Head->Data;
		m_Head->Data = NULL;
		delete m_Head;
		m_Head       = NULL;
		
        DeleteCriticalSection(&CritSect);
        CloseHandle(hSemPut);
        CloseHandle(hSemGet);
    }
	
	//�õ�������һ���յĽ�㣬���÷��Խ�㿪ʼ������������
	//���������������Ժ󣬵��÷��������GetFullObjectComplete(void)����ʾ��������������������
    T* GetFullObject() 
	{
        T *pObject = NULL;
        long lPrevious;
        // �ȴ���ֱ���������п��еĽ��
		WaitForSingleObject(hSemGet,10);
     //   TRACE("leave wait for hSemGet!\n");


        EnterCriticalSection(&CritSect);
		if (m_iCount > 0)
		{
			pObject = m_End->Data;
		}
		else
          ReleaseSemaphore(hSemGet, 1L, &lPrevious);
		LeaveCriticalSection(&CritSect);
		return pObject;

    }
	//��ʾ���������������������
	void GetFullObjectComplete(void) 
	{
		long lPrevious;
       
        EnterCriticalSection(&CritSect);
		if (m_iCount > 0)
		{
			m_iCount--;
		    m_End = m_End->next;
		}
		LeaveCriticalSection(&CritSect);
	//	TRACE("Leave GetFullObjectComplete!\n");
		ReleaseSemaphore(hSemPut, 1L, &lPrevious);//"д"�ź�-1
	}

	//��ʾ"д"���������������
	void GetEmptyObjectComplete(void) 
	{
		long lPrevious;

        EnterCriticalSection(&CritSect);
		m_iCount++;
		m_Head  = m_Head->next;
		LeaveCriticalSection(&CritSect);
	//	TRACE("Leave GetEmptyObjectComplete!\n");
		ReleaseSemaphore(hSemGet, 1L, &lPrevious);//�������ź�+1
	}
    
	//�õ�������һ���յĽ�㣬���÷��Խ�㿪ʼ��д��������
	//��д�����������Ժ󣬵��÷��������GetFullObjectComplete(void)����ʾ��д����������������
	T* GetEmptyObject()
	{
		//	int iSlot;
		T   *pObject = NULL;
		long lPrevious;
		WaitForSingleObject(hSemPut,INFINITE);
	//	TRACE("leave wait for hSemPut!\n");
		EnterCriticalSection(&CritSect);
		if (m_iCount < nMax)
		{
			pObject = m_Head->Data;
		}
		else
		{
          ReleaseSemaphore(hSemPut, 1L, &lPrevious);
		}
		LeaveCriticalSection(&CritSect);
		return pObject;
	}

	//���ö���Ϊ��
	void ReSet(void)
	{
		m_iCount = 0;
		m_Head   = m_End;
		m_Head->next = m_End->next;
	}
	bool Empty()
	{
	//	TRACE1("Queue:count=%d\n",m_iCount);
		if(m_iCount > 0)
			return false;
		return true;
	}
};




class MyMediaSample
{
public:
    MyMediaSample()
	{
		m_pBuffer     = NULL;
		m_dSampleTime = -1;
		m_lBufferLen  = -1;
		m_pBuffer = new BYTE[6*1024*1024];
	}
	MyMediaSample::MyMediaSample( const MyMediaSample& RightSides) //�������캯���Ķ���
	{
		m_pBuffer     = NULL;
		m_dSampleTime = -1;
		m_lBufferLen  = -1;
		m_pBuffer = new BYTE[6*1024*1024];

		if (RightSides.GetLen() > 0)
		{
			SetData(RightSides.m_pBuffer,RightSides.GetLen());
		}
		
	}
	ReleaseBuffer()
	{
		if (m_pBuffer)
		{
			delete[] m_pBuffer;
			m_pBuffer = NULL;
			m_lBufferLen = -1;
		}
	
	}
	//��ֵ����������
	MyMediaSample & MyMediaSample::operator = ( MyMediaSample& RightSides)
	{
		*this = (MyMediaSample&)RightSides;
		return *this;
	}

	~MyMediaSample()
	{
		ReleaseBuffer();
	}
    HRESULT SetData(const BYTE  *pBuffer,long nBufferLen) 
	{
		if (!m_pBuffer || !pBuffer || nBufferLen <= 0)
		{
			return S_FALSE;
		}

		if (nBufferLen > 6*1024*1024) 
			return E_FAIL;
		m_lBufferLen = nBufferLen;
		memcpy(m_pBuffer,pBuffer,nBufferLen);
		return S_OK;
		
	}
	BYTE* GetData() const
	{
		return m_pBuffer;
	}
	long GetLen() const
	{
		return m_lBufferLen;
	}
	HRESULT SetTime(double dTime)
	{
		m_dSampleTime = dTime;
		return 1;
	}
	double GetTime() const
	{
		return m_dSampleTime;
	}
//	BYTE   m_Buffer[810*1024];
protected:
	BYTE   *m_pBuffer;
	
	double m_dSampleTime;
	long   m_lBufferLen;
	
};

#endif 