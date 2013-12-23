/********************************************************************
	created:	2013/11/25
	filename: 	EMBTask.h
	author:		zhangjianchao
	purpose:	媒体任务数据定义：迁移任务
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
    Description：xml节点路径组合
	Input：		strPath 节点路径； strSubPath 子节点路径
	Return:		组合的子节点路径
	History：
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


// 提交信息
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

	int     m_nPRI;        // 任务优先级
	int     m_nRetry;      // 若失败，重试次数
};

// 返回信息
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


// 媒体文件存放位置
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


// 迁移任务信息
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
	CString      m_strTaskID;  // 任务id
	CFileAddress m_SourceFile; // 源文件
	CFileAddress m_DestFile;   // 目标文件
};

// 任务信息：包含分步处理具体信息
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
	// 属性
	CString m_strTaskID;
	CString m_strErrorCode;
	CString m_strDescription;

	int		m_nStatus;
	int     m_nPercent;

	// 处理流程信息
	CTransferTaskRequest m_transfer; // 迁移
	// 待增加信息： MD5, 转码, 技审
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