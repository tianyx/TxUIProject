/********************************************************************
	created:	2013/11/25
	filename: 	EMBTask.h
	author:		zhangjianchao
	purpose:	ý���������ݶ��壺Ǩ������
    History: 
*********************************************************************/

#pragma once
#include "TxParamString.h"
#include "Markup.h"
#include "SystemResourceInfo.h"

interface IXmlMeta
{
	virtual CString ToXml() = 0;
	virtual BOOL    ToMeta(const CString& strXml, const CString& strPath) = 0;
	/*
    Description��xml�ڵ�·�����
	Input��		strPath �ڵ�·���� strSubPath �ӽڵ�·��
	Return:		��ϵ��ӽڵ�·��
	History��
	*/
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


// �ύ��Ϣ
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

// ������Ϣ
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


// ý���ļ����λ��
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


// Ǩ��������Ϣ
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

// ������Ϣ�������ֲ����������Ϣ
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

	// ����������Ϣ
	CTransferTaskRequest m_transfer; // Ǩ��
	// ��������Ϣ�� MD5, ת��, ����
};

// 
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

// edoc_main 
class CEdocMain : public IXmlMeta
{
public:
	CEdocMain();
	~CEdocMain();

	virtual CString ToXml();
	virtual BOOL    ToMeta(const CString& strXml, const CString& strPath);

public: 
	CString m_strVer;
	int		m_nType;
	CString m_strGuid;

	vector<CSystemResourceInfo> m_arrSysRes;
};