#pragma  once
#include "StdAfx.h"
#include "FCVSServerConfig.h"
#include "FileDriver.h"
#include "EmbStructDef.h"
/*#include "IEMBBaseInterface.h"*/
#define ANA_TASK_RUN				0x0000001L
#define ANA_TASK_CONFIG				0x0000010L



class CFCVSClass
{
public:
	CFCVSClass();
	~CFCVSClass();
public:
	HRESULT Taskstart();
	HRESULT ConfigInit();
	HRESULT TaskInit();
	void TaskCancel();
	//CFCVSServerConfig m_config;
//	int AnalysisGetMSG();
//	CString TaskInfoGet();
	CString MSGget;
	CString MSGsend;
	ST_FCVSTASKINFO taskinfo;
	ST_WORKERREPORT m_reportST;//进度存储
//	CWnd d_ForMsg;
	HWND m_hwd;
	int nSectionUsedMore;  // 切片所用，LEVEL中的最大值
public:
	BOOL transcodeXML();
	BOOL AnalysisMsg();
	BOOL AnalysisSendMsg();
	BOOL BuildTask(CString strClassName,CString strMsg);
	BOOL OperateTask(CFileDriver* pDriver,CString strMsg);
	BOOL IsTaskOperating(CString strTaskID = "",CString strMsg = "");//wangjun 20081006 查询任务状态 
	CString reportPencent(BOOL IsAskPercent);
	CFileDriver *FindTaskByID(CString strTskID,POSITION& FindPs);
	BOOL ReleaseTskByTskID(CString strTskID);
	HRESULT SaveMessageToPool( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );
	HANDLE m_hEventMsgProcdureThread;
	HANDLE m_hEventAnaQuit;
	ITaskReportToExcutorInterface *m_reportCallBack;
private:
	inline void Lock()const {::EnterCriticalSection(&m_Lock);};
	inline void UnLock()const {::LeaveCriticalSection(&m_Lock);};
	NOTIFYICONDATA m_Nid;
	CFCVSServerConfig m_pConfig;
	//CFCVSServerConfig GetFCVSServerConfig();
	mutable CRITICAL_SECTION m_Lock;
	CStringArray m_strMsgArr;
	CStringArray m_strSendMsgArr;
	CList<CFileDriver*,CFileDriver*> m_TaskList;
	CList<CWinThread *,CWinThread *> m_ThreadList;
	int m_nMsgNotReturnCount;
private:

	static UINT MsgProcdureThread(LPVOID pParam);
	static UINT SendMsgProcdureThread(LPVOID pParam);

/*	void ResetThreadStopFlag();*/
	CEvent m_ThrdExit;
	bool ThreadShouldExit();

};
