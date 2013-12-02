#pragma once
#include "IEMBBaseInterface.h"

namespace EMB
{
//�����װ����
class CWorkSample:
	public IPluginBaseInterface,//�������ʵ�ֽӿ�
	public ITaskWorkerCallInterface//ִ���߲������ʵ�ֵĽӿ�
{
public:
	CWorkSample(void);
	virtual ~CWorkSample(void);

public:
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoOut);

	virtual HRESULT OnFirstInit();
	virtual void	OnFinalRelease();


public:
	//����ʼ�ӿ�,�˺���Ҫ���������ء�����ִ���뿪��һ���߳�
	//����������Ϊ�ص��ӿ�
	virtual HRESULT DoTask(const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback);

	virtual HRESULT CancelTask();
	//���Ȳ�ѯ�ӿ�
	virtual HRESULT GetTaskProgress(CEMBWorkString& szInfo);
	
	BOOL RunTaskLoop();
	BOOL m_bTaskRunning;
	HANDLE m_hThreadTask;
	ITaskReportToExcutorInterface* m_pReportCallback;
	int m_nPercent;

};

}

extern EMB::CWorkSample* g_pPluginInstane;
