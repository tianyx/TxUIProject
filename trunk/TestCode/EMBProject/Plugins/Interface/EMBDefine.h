#pragma once

#include "MBCCommonDef.h"
///
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

#define embmsgtype_None				10
#define embmsgtype_TaskIncoming		11
#define embmsgtype_ReportGuid		12
#define embmsgtype_ReportActorInfo	13
#define embmsgtype_TaskDispath		14

#define embmsgtype_ExcutorRegister	20
#define embmsgtype_ExcutorUnRegister 21

//////////////////////////////////////////////////////////////////////////
#define MSG_EMBEXCUTORMSG WM_USER+1888
#define MSG_EMBACTORMSG WM_USER+1889


#define MSG_EMBEXCUTORREG WM_USER+1890
#define MSG_EMBKILLEXCUTOR WM_USER+1891
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
