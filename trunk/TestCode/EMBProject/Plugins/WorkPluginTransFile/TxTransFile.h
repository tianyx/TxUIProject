#pragma once
#include "IEMBBaseInterface.h"
#include "EmbStructDef.h"

enum E_TRANSFERKIND
{
	TRANSFER_FTP = 0,  // Ftp��ʽ
	TRANSFER_FILE = 1, // �ļ���ʽ
};




struct tagTransTaskInfo
{
	CString strTaskID;
	E_TRANSFERKIND eSrcMethod;
	CString strSrcDirectory;
	CString strSrcFileName;
	CString strSrcFtpIP;
	CString strSrcFtpUser;
	CString strSrcFtpPwd;
	int     nSrcFtpPort;
	int nPassiveSrc;

	E_TRANSFERKIND eDesMethod;
	CString strDesDirectory;
	CString strDesFileName;
	CString strDesFtpIP;
	CString strDesFtpUser;
	CString strDesFtpPwd;
	int     nDesFtpPort;
	int nPassiveDes;

	BOOL     bDelDiskSrc;// ɾ�������ļ�
	INT64 nSpeedLimit; //kB/s

	tagTransTaskInfo()
	{
		strTaskID           = "";
		eSrcMethod			= TRANSFER_FTP;
		strSrcDirectory     = "";
		strSrcFileName      = "";
		strSrcFtpIP         = "";
		strSrcFtpUser       = "";
		strSrcFtpPwd        = "";
		nSrcFtpPort         = 21;
		nSpeedLimit = 50*1024;
		nPassiveSrc = 1;

		eDesMethod		    = TRANSFER_FTP;
		strDesDirectory     = "";
		strDesFileName      = "";
		strDesFtpIP         = "";
		strDesFtpUser       = "";
		strDesFtpPwd        = "";
		nDesFtpPort         = 21;
		nPassiveSrc = 0;

		bDelDiskSrc         = FALSE;

	}
};



namespace EMB
{


class CTxTransFile:
		public IPluginBaseInterface,//�������ʵ�ֽӿ�
		public ITaskWorkerCallInterface//ִ���߲������ʵ�ֵĽӿ�
{
public:
	CTxTransFile(void);
	virtual ~CTxTransFile(void);


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

	//bool ParseXml(const CString& strXml, tagTransTaskInfo& tsk );
	HRESULT ParseTaskInfo(const CString& strXml);
	
	//write localResult
	HRESULT WriteLocalResult();
	//register db
	HRESULT RegisterToDB();
	//
	HRESULT TansferFile();
	BOOL m_bTaskRunning;
	HANDLE m_hThreadTask;
	HANDLE m_hEventQuit;
	ITaskReportToExcutorInterface* m_pReportCallback;
	int m_nPercent;
	int m_nSubErrCode;
	//tagTransTaskInfo m_taskInfo;
	ST_TRANSFILEINFO m_taskInfo;
	INT64 m_nMaxFtpSpeedPerExc;
	int m_nCurrSrcSite;
	int m_nCurrDesSite;
	CString m_strMD5Val;
};

}

extern EMB::CTxTransFile* g_pPluginInstane;