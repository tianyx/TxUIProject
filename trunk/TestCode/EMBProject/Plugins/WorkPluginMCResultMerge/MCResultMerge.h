#pragma once
#include "IEMBBaseInterface.h"
#include "EmbStructDef.h"
#include "XMLOp.h"
#include "afxmt.h"
#include "io.h"
namespace EMB
{


	class CMCResultMerge:
		public IPluginBaseInterface,
		public ITaskWorkerCallInterface
{
public:
	CMCResultMerge(void);
	virtual ~CMCResultMerge(void);
public:
	virtual HRESULT QueryInterface(const GUID& guidIn, LPVOID& pInterfaceOut);

	virtual HRESULT QueryPluginInfo(VECPLUGINFOS& vInfoOut);

	virtual HRESULT OnFirstInit();
	virtual void	OnFinalRelease();


public:
	virtual HRESULT DoTask(const CTaskString& szTaskIn, CEMBWorkString& szRet, ITaskReportToExcutorInterface* pICallback);
	virtual HRESULT CancelTask();
	virtual HRESULT GetTaskProgress(CEMBWorkString& szInfo);

	BOOL RunTaskLoop();
	
private:
	BOOL isTaskRun;
	HANDLE m_hThreadTask;
	ITaskReportToExcutorInterface* m_pReportCallback;
	int m_nPercent;
	CString m_strTask;
	CStringArray m_arrSecXmlPath;
	CCriticalSection m_csLockSecXmlArr;
	void ManageResult();
	CWinThread *m_pThread;
private:
	structResult stResult;
	vector<structResult> vecResults; 
	void AnalysisFilePath();
	void FindEnoughthread();
	CXMLOp m_xmlDoc;
	int deletedFileCount;
	vector<ResultData> vecResultDatas;
public:
	ST_FCVSRESULTTASK m_structTask; 
	int  m_nCntDetectExtend;
};
}

extern EMB::CMCResultMerge* g_pPluginInstane;
