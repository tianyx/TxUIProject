/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:08
	filename: 	EMBDefine.h
	author:		tianyx
	
	purpose:	emb type def
*********************************************************************/
#pragma once

#include "MBCCommonDef.h"
///
#define INVALID_ID -1
//////////////////////////////////////////////////////////////////////////
#define EMB_OK			S_OK
#define EMB_FAIL		E_FAIL
#define EMBERR_INVALIDARG	E_INVALIDARG
#define EMBERR_POINTER	E_POINTER
#define EMBERR_HANDLE	E_HANDLE
#define EMBERR_ABORT	E_ABORT
#define EMBERR_NOTIMPL	E_NOTIMPL
//custom def
#define EMBERR_NOTREADY 0x800A0001
#define EMBERR_FULL		0x800A0002
#define EMBERR_EXISTED	0x800A0003
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
#define embxmltype_none				0
#define embxmltype_task				1
#define embxmltype_ret				2
#define embxmltype_getResource		3
#define embxmltype_getActor			4
//////////////////////////////////////////////////////////////////////////
#define embtaskstate_error			-1
#define embtaskstate_zero			0
#define embtaskstate_dispatching	1
#define embtaskstate_dispatched		2
#define embtaskstate_finished		3
//////////////////////////////////////////////////////////////////////////
#define embexcuteType_none			-1
#define embexcuteType_filetrans		1
#define embexucteType_md5check		2
#define embexcuteType_filecode		3
#define embexcuteType_filecheck		4
#define embexcuteType_db			5
//////////////////////////////////////////////////////////////////////////
#define embConnState_ok		1
#define embConnState_fail	0
//////////////////////////////////////////////////////////////////////////
#define ActorConnState_none 0
#define ActorConnState_conn 1
#define ActorConnState_ok	2
#define ActorConnState_del	3
//////////////////////////////////////////////////////////////////////////
#define embSvrType_master	1
#define embSvrType_slave	0
//////////////////////////////////////////////////////////////////////////
#define embSvrState_active	 1
#define embSvrState_deactive 0
//////////////////////////////////////////////////////////////////////////
#define embTaskproberType_tcp 1


#define MSG_EMBEXCUTORMSG WM_USER+1888
#define MSG_EMBACTORMSG WM_USER+1889


#define MSG_EMBEXCUTORREG WM_USER+1890
#define MSG_EMBKILLEXCUTOR WM_USER+1891
//////////////////////////////////////////////////////////////////////////

typedef int EXCUTORID;
typedef int ACTORID;

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
