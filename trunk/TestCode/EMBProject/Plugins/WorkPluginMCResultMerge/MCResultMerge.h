#pragma once
#include "IEMBBaseInterface.h"
#include "EmbStructDef.h"
#include "XMLOp.h"
#include "afxmt.h"
#include "io.h"
#include "AutoCritSec.h"
namespace EMB
{

	struct ST_SUBTASKCHECKFILE
	{
		CString	 strFile;
		BOOL bReady;
		ST_SUBTASKCHECKFILE()
		{
			bReady = FALSE;
		}

	};

	class CMCResultMerge:
		public IPluginBaseInterface,
		public ITaskWorkerCallInterface,
		public ITaskWorkerOnMessageInterface
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

public:
	virtual HRESULT OnMessageToWorker(const CTaskString& szInfo);

	BOOL RunTaskLoop();
	
private:
	BOOL isTaskRun;
	HANDLE m_hThreadTask;
	HANDLE m_hEventQuitTask;
	ITaskReportToExcutorInterface* m_pReportCallback;
	int m_nPercent;
	HRESULT m_hErrorCode;
	CString m_strTask;
	//CStringArray m_arrSecXmlPath;
	vector<ST_SUBTASKCHECKFILE> m_vSubFiles;
	CCriticalSection m_csLockSecXmlArr;
	CAutoCritSec m_csRunState;
	void ManageResult();
	BOOL RegisterToDB();
	CWinThread *m_pThread;
private:
	structResult stResult;
	vector<structResult> vecResults;
	vector<ST_TASKRUNSTATE> m_vTaskRunState;
	void AnalysisFilePath();
	HRESULT WaitForFinish();
	CXMLOp m_xmlDoc;
	int nReadyFileCount;
	vector<ResultData> vecResultDatas;
public:
	ST_FCVSRESULTTASK m_structTask; 
	int  m_nCntDetectExtend;

	//add for runtime dest file change
	ST_TRANSRESULT m_runtimeDestInfo;
};
}

extern EMB::CMCResultMerge* g_pPluginInstane;
