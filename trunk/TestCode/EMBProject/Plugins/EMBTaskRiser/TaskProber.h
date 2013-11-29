/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:03
	filename: 	TaskProber.h
	author:		tianyx
	
	purpose:	������ջ���
*********************************************************************/
#pragma once
#include "EMBCommonFunc.h"
#include "AutoCritSec.h"
#include "TxParamString.h"
#include <vector>

using namespace std;

//ý�崦����������ṹ��
struct ST_EMBTASKDATA
{
	time_t tm;                       //�ύʱ��
	CString taskDesc;                //����XML����
	//��������
	ST_EMBTASKDATA()
	{
		tm = time(NULL);           
	}
};

typedef vector<ST_EMBTASKDATA> VECTASKDATAS;     //�����б�

interface ITaskCollectCallbackInterface
{
	virtual HRESULT TaskCollectCallback(CString& taskIn) = 0;    //�����ռ��ص�����
};


class CTaskProber
{
public:
	 CTaskProber(void);
	virtual ~CTaskProber(void);
	// ���ûص�����
	BOOL SetTaskProcessor(ITaskCollectCallbackInterface* pProcessor);
	// �������
	virtual HRESULT AddTask(CString& taskIn);
	// ������������ӿ�
	virtual HRESULT Run_Prober();
	// ֹͣ��������ӿ�
	virtual HRESULT Stop_Prober();
private:
	// �ص�����ָ��
	ITaskCollectCallbackInterface* m_pTaskProcessor; 

};
