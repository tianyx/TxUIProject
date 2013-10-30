#pragma once

#include "MBCCommonDef.h"

//////////////////////////////////////////////////////////////////////////

#define EMBERR_UNKNOW	0x1000
#define EMBERR_INVALID	0x1001
#define EMBERR_FORMAT	0x1002
#define EMBERR_NOTREADY 0x1003
#define EMBERR_FULL		0x1004
//////////////////////////////////////////////////////////////////////////
//tcp msg  trans type
#define embmsgstate_Q 1
#define embmsgstate_A 2

#define embmsgtype_None 10
#define embmsgtype_Task 11
#define embmsgtype_ReportGuid 12
#define embmsgtype_ReportActorInfo 13
#define embmsgtype_TaskDispath 14


//////////////////////////////////////////////////////////////////////////
struct ST_EMBTRANSMSG :public ST_TXMSGBASE
{
	CString strGuid;
	CString strData;
	ST_EMBTRANSMSG(int nMsgTypeIn)
	{
		nMsgType = nMsgTypeIn;
	}

	virtual BOOL operator <<( CTxSerialize& ar) const
	{
		ST_TXMSGBASE::operator <<(ar);
		ar << strGuid;
		ar << strData;
		return TRUE;
	}

	virtual BOOL operator >>(CTxSerialize& ar)
	{
		ST_TXMSGBASE::operator >>(ar);
		ar >> strGuid;
		ar >> strData;
		return TRUE;
	}
};
