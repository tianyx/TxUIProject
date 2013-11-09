#pragma once

// Data:		2013-11-05
// Description: ý�崦������

interface IXmlMeta
{
	virtual CString ToXml() = 0;
	virtual BOOL    ToMeta(const CString& strXml, const CString& strPath) = 0;
	inline CString CombinePath(const CString& strPath, const CString& strSubPath)
	{
		if (strPath.IsEmpty())
		{
			return strSubPath;
		}

		CString strRet;

		if (strPath.GetAt(strPath.GetLength() - 1) == '\\')
		{
			strRet = strPath + strSubPath;
		}
		else
		{
			strRet.Format("%s\\%s", strPath, strSubPath);
		}

		return strRet;
	}
};


// CommonRequest
class CCommonRequest : public IXmlMeta
{
public:
	CCommonRequest(void);
	~CCommonRequest(void);

	CCommonRequest(const CCommonRequest& rht);
	CCommonRequest& operator=(const CCommonRequest& rht);

public:
	virtual CString ToXml();
	virtual BOOL    ToMeta(const CString& strXml, const CString& strPath);

public:
	CString m_MsgID;
	CString m_strTaskType;      // Transfer  MD5  Transcode  FCVS
	CString m_strUserCode;
	CString m_strUserName;
	CString m_strTaskOp;      // add update delete
	CString m_strSourceID;  

	int     m_nPRI;        // �������ȼ�
	int     m_nRetry;      // ��ʧ�ܣ����Դ���
};

class CCommonResponse : public IXmlMeta
{
public:
	CCommonResponse(void);
	~CCommonResponse(void);

public:
	virtual CString ToXml();
	virtual BOOL    ToMeta(const CString& strXml, const CString& strPath);

public:
	CString m_strTaskID;
	CString m_strStatus;      
	CString m_strResult;
	CString m_strComment;
	int     m_nPercent;
};

// �ļ�·��
class CFileAddress
{
public:
	CFileAddress();
	~CFileAddress();

public:
	CString m_strType;
	CString m_strFilePath;
	CString m_strFileName;
	CString m_strIP;
	CString m_strPort;
	CString m_strUser;
	CString m_strPwd;
};

class CTransferTaskRequest : IXmlMeta
{
public:
	CTransferTaskRequest();
	~CTransferTaskRequest();

	CTransferTaskRequest(const CTransferTaskRequest& rht);
	CTransferTaskRequest& operator=(const CTransferTaskRequest& rht);

public:
	virtual CString ToXml();
	virtual BOOL    ToMeta(const CString& strXml, const CString& strPath);

public:
	CString      m_strTaskID;  // ����id
	CFileAddress m_SourceFile; // Դ�ļ�
	CFileAddress m_DestFile;   // Ŀ���ļ�
};

// TaskInfo
class CTaskInfo : IXmlMeta
{
public:
	CTaskInfo();
	~CTaskInfo();

	CTaskInfo(const CTaskInfo& rht);
	CTaskInfo& operator=(const CTaskInfo& rht);

public:
	virtual CString ToXml();
	virtual BOOL    ToMeta(const CString& strXml, const CString& strPath);

public:
	// ����
	CString m_strTaskID;
	CString m_strErrorCode;
	CString m_strDescription;

	int		m_nStatus;
	int     m_nPercent;

	// �ӽڵ�
	CTransferTaskRequest m_transfer; // Ǩ��
};

// EMBTaskRequest
class CEMBTaskRequest : public IXmlMeta
{
public:
	CEMBTaskRequest();
	~CEMBTaskRequest();

	CEMBTaskRequest(const CEMBTaskRequest& rht);
	CEMBTaskRequest& operator=(const CEMBTaskRequest& rht);

public:
	virtual CString ToXml();
	virtual BOOL    ToMeta(const CString& strXml, const CString& strPath);

public:
	CCommonRequest	m_request;
	CTaskInfo		m_taskInfo;
};