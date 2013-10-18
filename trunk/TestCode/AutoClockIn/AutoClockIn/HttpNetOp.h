#pragma once
#include "IMSerialize.h"
struct ST_NETOPINFO 
{
	CString strUser;
	CString strPw;
	CString strSvr;
	CString strUrl;
	CString strFile;
	ST_NETOPINFO()
	{
		strFile = TEXT("d:\autoclock_output.txt");
	}

	BOOL operator <<(CIMSerialize& ar) const
	{
		return ar << strUser
			&& ar << strPw
			&& ar << strSvr
			&& ar << strUrl
			&& ar << strFile;
	}

	BOOL operator >>(CIMSerialize& ar) 
	{
		return ar >> strUser
			&& ar >> strPw
			&& ar >> strSvr
			&& ar >> strUrl
			&& ar >> strFile;
	}
};
class CHttpNetOp
{
public:
	CHttpNetOp(void);
	~CHttpNetOp(void);

public:
	ST_NETOPINFO m_preData;

private:
	CListBox* m_pCtlLog;
public:
	BOOL Init(ST_NETOPINFO& dataIn){m_preData = dataIn; return TRUE;}
	BOOL DoGet();
};

