#pragma once

#include "MainDef.h"
#include <vector>
using namespace std;
enum ENUM_XMLKEY
{
	ek_packId = 0,
	ek_movId,
	ek_ClipName,
	ek_strBroadTime,
	ek_holdTime =4,
	ek_type = 5,
	ek_ftp = 6,
	ek_duration = 10


};

interface ITaskReporter
{
	virtual void ReportTask(const ST_TASKINFO&taskIn, vector<CString>& vExcMsgsIn) =0;
	virtual void UpdateTaskExcInfo(ST_REALTRASINFO& infoIn) = 0;
};

class CTransferClip
{
public:
	CTransferClip(ITaskReporter* pIReporter);
	~CTransferClip(void);
	ITaskReporter* m_pIReporter;

public:
	BOOL Init(ST_TASKINFO& taskInfo, HANDLE hEventQuit);
	BOOL Start();

private:
	HANDLE m_hEventQuit;
	CString m_strTargetXml;
	ST_TASKINFO m_taskInfo;
	vector<CString> m_vExcMsgs;
	vector<CString> m_vxmlTaskData;
	int nPercent;
	CString m_strClipId;

	void WriteMsg(LPCTSTR szMsgIn);

	BOOL InitDB();
	BOOL InitFtp();
	CString GenClipID();
	BOOL AnalyseXmlFile();
	BOOL TransFile();
	DWORD TansferFile(CString strClipID, CString &strFTP, CString &strDir, CString &strFileName, CString &strUserName, CString &strPassWord);
	DWORD AnalyseFTPLink( CString strSrcFile, CString &strIP, CString &strDir, CString &strFileName, CString &strUser, CString &strPass);
	DWORD ClipToDB(CString strClipID);
	BOOL CheckLogicIDInDB(CString strLogicID);

	BOOL Run();
};
