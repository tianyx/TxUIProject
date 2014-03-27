#pragma once
#include "IEMBBaseInterface.h"
#include "EmbStructDef.h"

namespace EMB
{
class CWorkSleeper:
	public IPluginBaseInterface,//�������ʵ�ֽӿ�
	public ITaskWorkerCallInterface//ִ���߲������ʵ�ֵĽӿ�
{
public:
	CWorkSleeper(void);
	virtual ~CWorkSleeper(void);
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
	HANDLE m_hQuitEvent;
	ST_SLEEPTASKINFO m_taskInfo;
	ITaskReportToExcutorInterface* m_pReportCallback;
	int m_nPercent;
	int m_nCodeRet;


};

}

extern EMB::CWorkSleeper* g_pPluginInstane;
