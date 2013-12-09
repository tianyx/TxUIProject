#include "StdAfx.h"
#include "EMBTask.h"

CCommonRequest::CCommonRequest(void)
{
	m_nRetry = 1;
	m_nPRI = 0;
}

CCommonRequest::CCommonRequest( const CCommonRequest& rht )
{
	// assign
	this->m_MsgID       = rht.m_MsgID;
	this->m_strTaskType = rht.m_strTaskType;
	this->m_strTaskOp	= rht.m_strTaskOp;
	this->m_strSourceID = rht.m_strSourceID;
	this->m_strUserCode = rht.m_strUserCode;
	this->m_strUserName = rht.m_strUserName;

	this->m_nPRI		= rht.m_nPRI;
	this->m_nRetry		= rht.m_nRetry;
}

CCommonRequest::~CCommonRequest(void)
{
}

CCommonRequest& CCommonRequest::operator=( const CCommonRequest& rht )
{
	if (this == &rht)
	{
		return *this;
	}

	// assign
	this->m_MsgID       = rht.m_MsgID;
	this->m_strTaskType = rht.m_strTaskType;
	this->m_strTaskOp	= rht.m_strTaskOp;
	this->m_strSourceID = rht.m_strSourceID;
	this->m_strUserCode = rht.m_strUserCode;
	this->m_strUserName = rht.m_strUserName;

	this->m_nPRI		= rht.m_nPRI;
	this->m_nRetry		= rht.m_nRetry;

	return *this;
}

CString CCommonRequest::ToXml()
{
	CTxParamString xParam(TEXT("<CommonRequest></CommonRequest>"));
	xParam.GoToPath(TEXT(".\\CommonRequest"));

	CTxStrConvert strVal;
	strVal.SetVal(m_MsgID);
	xParam.SetAttribVal(NULL, TEXT("MsgID"), strVal);

	strVal.SetVal(m_strTaskType);
	xParam.SetAttribVal(NULL, TEXT("TaskType"), strVal);

	strVal.SetVal(m_strTaskOp);
	xParam.SetAttribVal(NULL, TEXT("TaskOp"), strVal);

	strVal.SetVal(m_strSourceID);
	xParam.SetAttribVal(NULL, TEXT("SourceID"), strVal);

	strVal.SetVal(m_strUserCode);
	xParam.SetAttribVal(NULL, TEXT("UserCode"), strVal);

	strVal.SetVal(m_strUserName);
	xParam.SetAttribVal(NULL, TEXT("UserName"), strVal);

	strVal.SetVal(m_nPRI);
	xParam.SetAttribVal(NULL, TEXT("nPRI"), strVal);

	strVal.SetVal(m_nRetry);
	xParam.SetAttribVal(NULL, TEXT("nRetry"), strVal);

	xParam.UpdateData();
	CString strXml = xParam;

	return strXml;
}

BOOL CCommonRequest::ToMeta( const CString& strXml, const CString& strPath )
{
	if (strXml.IsEmpty())
	{
		return FALSE;
	}

	CTxParamString xParm(strXml);
	xParm.GoToPath(strPath);
	//
	m_MsgID = xParm.GetAttribVal(NULL, TEXT("MsgID")).GetAsString();
	m_strTaskType = xParm.GetAttribVal(NULL, TEXT("TaskType")).GetAsString();
	m_strTaskOp = xParm.GetAttribVal(NULL, TEXT("TaskOp")).GetAsString();
	m_strSourceID = xParm.GetAttribVal(NULL, TEXT("SourceID")).GetAsString();
	m_strUserCode = xParm.GetAttribVal(NULL, TEXT("UserCode")).GetAsString();
	m_strUserName  = xParm.GetAttribVal(NULL, TEXT("UserName")).GetAsString();

	m_nPRI = xParm.GetAttribVal(NULL, TEXT("nPRI")).GetAsInt();
	m_nRetry = xParm.GetAttribVal(NULL, TEXT("nRetry")).GetAsInt();

	return TRUE;
}

CFileAddress::CFileAddress()
{

}

CFileAddress::~CFileAddress()
{

}

CTransferTaskRequest::CTransferTaskRequest()
{

}

CTransferTaskRequest::CTransferTaskRequest( const CTransferTaskRequest& rht )
{
	m_strTaskID  = rht.m_strTaskID;
	m_SourceFile = rht.m_SourceFile;
	m_DestFile	 = rht.m_DestFile;
}

CTransferTaskRequest::~CTransferTaskRequest()
{

}

CTransferTaskRequest& CTransferTaskRequest::operator=( const CTransferTaskRequest& rht )
{
	if (this == &rht)
	{
		return *this;
	}

	m_strTaskID  = rht.m_strTaskID;
	m_SourceFile = rht.m_SourceFile;
	m_DestFile	 = rht.m_DestFile;

	return *this;
}

CString CTransferTaskRequest::ToXml()
{
	CTxParamString xParam(TEXT("<TransferTaskRequest></TransferTaskRequest>"));
	xParam.GoToPath(TEXT(".\\TransferTaskRequest"));

	CTxStrConvert strVal;
	strVal.SetVal(m_strTaskID);
	xParam.SetAttribVal(NULL, TEXT("TaskID"), strVal);

	// SourceFile
	CTxParamString xSourceFile(TEXT("<SourceFile></SourceFile>"));
	xSourceFile.GoToPath(TEXT(".\\SourceFile"));

	strVal.SetVal(m_SourceFile.m_strType);
	xSourceFile.SetAttribVal(NULL, TEXT("Type"), strVal);

	strVal.SetVal(m_SourceFile.m_strFilePath);
	xSourceFile.SetAttribVal(NULL, TEXT("FilePath"), strVal);

	strVal.SetVal(m_SourceFile.m_strFileName);
	xSourceFile.SetAttribVal(NULL, TEXT("FileName"), strVal);

	strVal.SetVal(m_SourceFile.m_strIP);
	xSourceFile.SetAttribVal(NULL, TEXT("IP"), strVal);

	strVal.SetVal(m_SourceFile.m_strPort);
	xSourceFile.SetAttribVal(NULL, TEXT("Port"), strVal);

	strVal.SetVal(m_SourceFile.m_strUser);
	xSourceFile.SetAttribVal(NULL, TEXT("User"), strVal);

	strVal.SetVal(m_SourceFile.m_strPwd);
	xSourceFile.SetAttribVal(NULL, TEXT("Pwd"), strVal);

	xParam.SetSubNodeString(".\\TransferTaskRequest", xSourceFile);

	// DestFile
	CTxParamString xDestFile(TEXT("<DestFile></DestFile>"));
	xDestFile.GoToPath(TEXT(".\\DestFile"));

	strVal.SetVal(m_DestFile.m_strType);
	xDestFile.SetAttribVal(NULL, TEXT("Type"), strVal);

	strVal.SetVal(m_DestFile.m_strFilePath);
	xDestFile.SetAttribVal(NULL, TEXT("FilePath"), strVal);

	strVal.SetVal(m_DestFile.m_strFileName);
	xDestFile.SetAttribVal(NULL, TEXT("FileName"), strVal);

	strVal.SetVal(m_DestFile.m_strIP);
	xDestFile.SetAttribVal(NULL, TEXT("IP"), strVal);

	strVal.SetVal(m_DestFile.m_strPort);
	xDestFile.SetAttribVal(NULL, TEXT("Port"), strVal);

	strVal.SetVal(m_DestFile.m_strUser);
	xDestFile.SetAttribVal(NULL, TEXT("User"), strVal);

	strVal.SetVal(m_DestFile.m_strPwd);
	xDestFile.SetAttribVal(NULL, TEXT("Pwd"), strVal);

	xParam.SetSubNodeString(".\\TransferTaskRequest", xDestFile);

	// set
	xParam.UpdateData();
	CString strXml = xParam;

	return strXml;
}

BOOL CTransferTaskRequest::ToMeta( const CString& strXml, const CString& strPath )
{
	if (strXml.IsEmpty())
	{
		return FALSE;
	}

	CTxParamString xParm(strXml);
	xParm.GoToPath(strPath);
	//
	m_strTaskID = xParm.GetAttribVal(NULL, TEXT("TaskID")).GetAsString();
	
	// sourcefile
	xParm.GoToPath(CombinePath(strPath, "SourceFile"));
	m_SourceFile.m_strType = xParm.GetAttribVal(NULL, TEXT("Type")).GetAsString();
	m_SourceFile.m_strFilePath = xParm.GetAttribVal(NULL, TEXT("FilePath")).GetAsString();
	m_SourceFile.m_strFileName = xParm.GetAttribVal(NULL, TEXT("FileName")).GetAsString();
	m_SourceFile.m_strIP = xParm.GetAttribVal(NULL, TEXT("IP")).GetAsString();
	m_SourceFile.m_strPort = xParm.GetAttribVal(NULL, TEXT("Port")).GetAsString();
	m_SourceFile.m_strUser = xParm.GetAttribVal(NULL, TEXT("User")).GetAsString();
	m_SourceFile.m_strPwd = xParm.GetAttribVal(NULL, TEXT("Pwd")).GetAsString();

	// destfile
	xParm.GoToPath(CombinePath(strPath, "DestFile"));
	m_DestFile.m_strType = xParm.GetAttribVal(NULL, TEXT("Type")).GetAsString();
	m_DestFile.m_strFilePath = xParm.GetAttribVal(NULL, TEXT("FilePath")).GetAsString();
	m_DestFile.m_strFileName = xParm.GetAttribVal(NULL, TEXT("FileName")).GetAsString();
	m_DestFile.m_strIP = xParm.GetAttribVal(NULL, TEXT("IP")).GetAsString();
	m_DestFile.m_strPort = xParm.GetAttribVal(NULL, TEXT("Port")).GetAsString();
	m_DestFile.m_strUser = xParm.GetAttribVal(NULL, TEXT("User")).GetAsString();
	m_DestFile.m_strPwd = xParm.GetAttribVal(NULL, TEXT("Pwd")).GetAsString();

	return TRUE;
}

CCommonResponse::CCommonResponse( void )
{

}

CCommonResponse::~CCommonResponse( void )
{

}

CString CCommonResponse::ToXml()
{
	CTxParamString xParam(TEXT("<CommonResponse></CommonResponse>"));
	xParam.GoToPath(TEXT(".\\CommonResponse"));

	CTxStrConvert strVal;
	strVal.SetVal(m_strTaskID);
	xParam.SetAttribVal(NULL, TEXT("TaskID"), strVal);

	strVal.SetVal(m_strStatus);
	xParam.SetAttribVal(NULL, TEXT("Status"), strVal);

	strVal.SetVal(m_strResult);
	xParam.SetAttribVal(NULL, TEXT("Result"), strVal);

	strVal.SetVal(m_strComment);
	xParam.SetAttribVal(NULL, TEXT("Comment"), strVal);

	strVal.SetVal(m_nPercent);
	xParam.SetAttribVal(NULL, TEXT("Percent"), strVal);

	xParam.UpdateData();
	CString strXml = xParam;

	return strXml;
}

BOOL CCommonResponse::ToMeta( const CString& strXml, const CString& strPath )
{
	if (strXml.IsEmpty())
	{
		return FALSE;
	}

	CTxParamString xParm(strXml);
	xParm.GoToPath(strPath);
	//
	m_strTaskID = xParm.GetAttribVal(NULL, TEXT("TaskID")).GetAsString();
	m_strStatus = xParm.GetAttribVal(NULL, TEXT("Status")).GetAsString();
	m_strResult = xParm.GetAttribVal(NULL, TEXT("Result")).GetAsString();
	m_strComment = xParm.GetAttribVal(NULL, TEXT("Comment")).GetAsString();

	m_nPercent = xParm.GetAttribVal(NULL, TEXT("Percent")).GetAsInt();

	return TRUE;
}

CTaskInfo::CTaskInfo()
{
	m_nStatus = 0;
	m_nPercent = 0;
}

CTaskInfo::CTaskInfo( const CTaskInfo& rht )
{
	m_strTaskID		= rht.m_strTaskID;
	m_strErrorCode	= rht.m_strErrorCode;
	m_strDescription = rht.m_strDescription;

	m_nPercent	 = m_nPercent;
	m_nStatus	= m_nStatus;
}

CTaskInfo::~CTaskInfo()
{

}

CTaskInfo& CTaskInfo::operator=( const CTaskInfo& rht )
{
	if (this == &rht)
	{
		return *this;
	}

	m_strTaskID		= rht.m_strTaskID;
	m_strErrorCode	= rht.m_strErrorCode;
	m_strDescription = rht.m_strDescription;

	m_nPercent	 = m_nPercent;
	m_nStatus	= m_nStatus;

	return *this;
}

CString CTaskInfo::ToXml()
{
	CTxParamString xParam(TEXT("<TaskInfo></TaskInfo>"));
	xParam.GoToPath(TEXT(".\\TaskInfo"));

	CTxStrConvert strVal;
	strVal.SetVal(m_strTaskID);
	xParam.SetAttribVal(NULL, TEXT("TaskID"), strVal);

	strVal.SetVal(m_strErrorCode);
	xParam.SetAttribVal(NULL, TEXT("ErrorCode"), strVal);

	strVal.SetVal(m_strDescription);
	xParam.SetAttribVal(NULL, TEXT("Description"), strVal);

	strVal.SetVal(m_nPercent);
	xParam.SetAttribVal(NULL, TEXT("Percent"), strVal);

	strVal.SetVal(m_nStatus);
	xParam.SetAttribVal(NULL, TEXT("Status"), strVal);

	// TransferTaskRequest
	CTxParamString xTransfer(m_transfer.ToXml());
	xParam.SetSubNodeString(".\\TaskInfo", xTransfer);

	//
	xParam.UpdateData();
	CString strXml = xParam;

	return strXml;
}

BOOL CTaskInfo::ToMeta( const CString& strXml, const CString& strPath )
{
	if (strXml.IsEmpty())
	{
		return FALSE;
	}

	CTxParamString xParm(strXml);
	xParm.GoToPath(strPath);
	//
	m_strTaskID = xParm.GetAttribVal(NULL, TEXT("TaskID")).GetAsString();
	m_strErrorCode = xParm.GetAttribVal(NULL, TEXT("ErrorCode")).GetAsString();
	m_strDescription = xParm.GetAttribVal(NULL, TEXT("Description")).GetAsString();

	m_nStatus = xParm.GetAttribVal(NULL, TEXT("Status")).GetAsInt();
	m_nPercent = xParm.GetAttribVal(NULL, TEXT("Percent")).GetAsInt();

	// TransferTaskRequest
	m_transfer.ToMeta(xParm, CombinePath(strPath, "TransferTaskRequest"));

	return TRUE;
}

CEMBTaskRequest::CEMBTaskRequest()
{

}

CEMBTaskRequest::CEMBTaskRequest( const CEMBTaskRequest& rht )
{
	m_request = rht.m_request;
	m_taskInfo = rht.m_taskInfo;
}

CEMBTaskRequest::~CEMBTaskRequest()
{

}

CEMBTaskRequest& CEMBTaskRequest::operator=( const CEMBTaskRequest& rht )
{
	if (this == &rht)
	{
		return *this;
	}

	m_request = rht.m_request;
	m_taskInfo = rht.m_taskInfo;

	return *this;
}

CString CEMBTaskRequest::ToXml()
{
	CTxParamString xParam(TEXT("<EMBTaskRequest></EMBTaskRequest>"));
	xParam.GoToPath(TEXT(".\\EMBTaskRequest"));

	// CommonRequest
	CTxParamString xCommonRequest(m_request.ToXml());
	xParam.SetSubNodeString(".\\EMBTaskRequest", xCommonRequest);

	// TaskInfo
	CTxParamString xTaskInfo(m_taskInfo.ToXml());
	xParam.SetSubNodeString(".\\EMBTaskRequest", xTaskInfo);

	//
	xParam.UpdateData();
	CString strXml = xParam;

	return strXml;
}

BOOL CEMBTaskRequest::ToMeta( const CString& strXml, const CString& strPath )
{
	if (strXml.IsEmpty())
	{
		return FALSE;
	}

	CTxParamString xParm(strXml);
	xParm.GoToPath(strPath);

	// CommonRequest
	m_request.ToMeta(xParm, ".\\EMBTaskRequest\\CommonRequest");

	// TaskInfo
	m_taskInfo.ToMeta(xParm, ".\\EMBTaskRequest\\TaskInfo");

	return TRUE;
}



CString CEdocMain::ToXml()
{
	CTxParamString xParam(TEXT("<edoc_main></edoc_main>"));
	xParam.GoToPath(TEXT(".\\edoc_main"));

	// set attribute value
	CTxStrConvert strVal;
	strVal.SetVal(m_strVer);
	xParam.SetAttribVal(NULL, TEXT("ver"), strVal);

	strVal.SetVal(m_nType);
	xParam.SetAttribVal(NULL, TEXT("type"), strVal);

	strVal.SetVal(m_strGuid);
	xParam.SetAttribVal(NULL, TEXT("guid"), strVal);

	if (m_arrSysRes.size() > 0)
	{
		CTxParamString xRes(TEXT("<SystemResource></SystemResource>"));
		xRes.GoToPath(TEXT(".\\SystemResource"));

		CString strTem;

		for (int i = 0; i < m_arrSysRes.size(); ++i)
		{
			CSystemResourceInfo info = m_arrSysRes[i];
			strTem.Format("<%s></%s>", info.m_strName, info.m_strName);
			CTxParamString xLeaf(strTem);
			xLeaf.GoIntoKey(info.m_strName);

			strVal.SetVal(info.m_eRes);
			xLeaf.SetAttribVal(NULL, "kind", strVal);

			strVal.SetVal(info.m_nUsedPercent);
			xLeaf.SetAttribVal(NULL, "usedPercent", strVal);

			xRes.SetSubNodeString(".\\SystemResource", xLeaf);
		}

		xParam.SetSubNodeString(".\\edoc_main", xRes);
	}

	//
	xParam.UpdateData();
	CString strXml = xParam;

	return strXml;
}

BOOL CEdocMain::ToMeta( const CString& strXml, const CString& strPath )
{
	if (strXml.IsEmpty())
	{
		return FALSE;
	}

	CTxParamString xParm(strXml);
	xParm.GoToPath(strPath);

	//
	m_strVer = xParm.GetAttribVal(NULL, TEXT("ver")).GetAsString();
	m_nType = xParm.GetAttribVal(NULL, TEXT("type")).GetAsInt();
	m_strGuid = xParm.GetAttribVal(NULL, TEXT("guid")).GetAsString();

	if (xParm.GoIntoKey("SystemResource"))
	{
		CMarkup xmlMark(strXml);

		if(xmlMark.FindChildElem("SystemResource"))
		{
			xmlMark.IntoElem();

			CTxStrConvert strTem;

			while(xmlMark.FindChildElem())
			{
				CSystemResourceInfo info;
				info.m_strName = xmlMark.GetChildTagName();
				//
				strTem.SetVal(xmlMark.GetChildAttrib("usedPercent"));
				info.m_nUsedPercent = strTem.GetAsInt();

				strTem.SetVal(xmlMark.GetChildAttrib("kind"));
				info.m_eRes = (SYSTEM_RES)strTem.GetAsInt();

				m_arrSysRes.push_back(info);
			}

			xmlMark.OutOfElem();
		}
	}

	return TRUE;
}

CEdocMain::CEdocMain()
{
	m_strVer = "1.0";
	m_strGuid.Empty();
	m_nType = 1;

	m_arrSysRes.clear();
}

CEdocMain::~CEdocMain()
{

}
