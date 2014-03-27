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
//环形队列，初始分配节点数目和空间，依靠"读","写"信号控制不同线程对队列的写入和读出操作。
//
#define DEFAULT_QSIZE  10

template <class T> class CQueue2 {
private:
	struct Node
	{
		T*      Data;
		Node    *next;

	};

    HANDLE          hSemPut;        // 控制"写"队列信号
    HANDLE          hSemGet;        // 控制"读"队列信号
    CRITICAL_SECTION CritSect;      // 数据改变时建立临界区
    int             nMax;           // 队列中的最大节点数
	int             m_iCount;         //队列中的有效节点
	Node*           m_Head;         //队列头节点
	Node*           m_End;          //队列尾结点
    void Initialize(int n) {
        nMax = n;
        InitializeCriticalSection(&CritSect);
		//创建“写”信号
        hSemPut = CreateSemaphore(NULL, n, n, NULL);
		//创建“读”信号
        hSemGet = CreateSemaphore(NULL, 0, n, NULL);

		m_iCount = 0;
		m_Head = m_End = new Node;
		m_Head->Data   = new T;
		m_Head->next = m_End->next = m_End;//建立头结点，指向自己

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
	
	//得到队列中一个空的结点，调用方对结点开始“读”操作，
	//“读”操作结束以后，调用方必须调用GetFullObjectComplete(void)，表示“读”操作真正结束。
    T* GetFullObject() 
	{
        T *pObject = NULL;
        long lPrevious;
        // 等待，直到队列有有空闲的结点
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
	//表示“读”操作“真正”完成
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
		ReleaseSemaphore(hSemPut, 1L, &lPrevious);//"写"信号-1
	}

	//表示"写"操作“真正”完成
	void GetEmptyObjectComplete(void) 
	{
		long lPrevious;

        EnterCriticalSection(&CritSect);
		m_iCount++;
		m_Head  = m_Head->next;
		LeaveCriticalSection(&CritSect);
	//	TRACE("Leave GetEmptyObjectComplete!\n");
		ReleaseSemaphore(hSemGet, 1L, &lPrevious);//“读”信号+1
	}
    
	//得到队列中一个空的结点，调用方对结点开始“写”操作，
	//“写”操作结束以后，调用方必须调用GetFullObjectComplete(void)，表示“写”操作真正结束。
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

	//重置队列为空
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
	MyMediaSample::MyMediaSample( const MyMediaSample& RightSides) //拷贝构造函数的定义
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
	//赋值操作符重载
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