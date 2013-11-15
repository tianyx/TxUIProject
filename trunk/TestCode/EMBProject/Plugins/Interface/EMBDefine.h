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
#define INVALID_VALUE -1
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
#define EMBERR_EMPTY	0x800A0004
#define EMBERR_NOTMYTASK	0x800A0005
#define EMBERR_TASKSTEPERR	0x800A0006
#define EMBERR_SUBTASKERR	0x800A0007
#define EMBERR_SUBTASKTYPEERR	0x800A0008
#define EMBERR_SUBTASKTEXT	0x800A0009
#define EMBERR_SUBTASKLOADDLL	0x800A000a

//////////////////////////////////////////////////////////////////////////
//tcp msg  trans type
#define embmsgstate_Q 1
#define embmsgstate_A 2

#define embmsgtype_None				10
#define embmsgtype_TaskIncoming		11
#define embmsgtype_ActorReportGuid		12
#define embmsgtype_ActorToDispatchMsg			13

#define embmsgtype_ActorToDispath		14
#define embmsgtype_ExcutorToActorMsg	15
#define embmsgtype_ActorToExcutorMsg	16
#define embmsgtype_DispatchToActorMsg			17
#define embmsgtype_max				100
//////////////////////////////////////////////////////////////////////////
#define embxmltype_none				0
#define embxmltype_task				1
#define embxmltype_ret				2
#define embxmltype_actorState		3
#define embxmltype_getActor			4
#define embxmltype_taskupdate		5
#define embxmltype_taskReport		6
#define embxmltype_svrActive		7
#define embxmltype_excOnIdle		8



//////////////////////////////////////////////////////////////////////////
#define embtaskupdatetype_none		0
#define embtaskupdatetype_finish	1
#define embtaskupdatetype_del		2
#define mebtaskupdatetype_changepri	3
//////////////////////////////////////////////////////////////////////////
//task excute state
#define embtaskstate_none			-1
#define embtaskstate_zero			0
#define embtaskstate_dispatching	1
#define embtaskstate_dispatched		2
#define embtaskstate_finished		3
#define embtaskstate_error			99
//////////////////////////////////////////////////////////////////////////
//to specify the actor for the task, set the task priority more than actorlevel;
#define embtaskPriority_normal		0
#define embtaskPriority_high		1
#define embActorLevel_normal		0
#define embActorLevel_high			1

//	////////////////////////////////////////////////////////////////////////
#define embexcuteType_none			-1
#define embexcuteType_filetrans		1
#define embexucteType_md5check		2
#define embexcuteType_filecode		3
#define embexcuteType_filecheck		4
#define embexcuteType_db			5
//////////////////////////////////////////////////////////////////////////
#define embConnState_ok		1
#define embConnState_error	0
//////////////////////////////////////////////////////////////////////////
#define embActorConnState_none 0
#define embActorConnState_conn 1
#define embActorConnState_ok	2
#define embActorConnState_del	3
//////////////////////////////////////////////////////////////////////////
#define embSvrType_master	1
#define embSvrType_slave	0
//////////////////////////////////////////////////////////////////////////
#define embSvrState_active	 1
#define embSvrState_deactive 0
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define embTaskproberType_tcp 1

#define MSG_EMBMSGMIN	WM_USER +1700
#define MSG_EMBTOEXCUTORMSG WM_USER+1888

#define MSG_EMBTOACTORMSG WM_USER+1889


#define MSG_EMBEXCUTORREG WM_USER+1890
#define MSG_EMBKILLEXCUTOR WM_USER+1891
#define MSG_EMBEXCUTORREGED WM_USER+1892
#define MSG_EMBEXCUTOREXIT WM_USER+1893

#define MSG_EMBMSGMAX	WM_USER +1700


//////////////////////////////////////////////////////////////////////////

typedef int EXCUTORID;
typedef int ACTORID;
typedef int DISPATCHID;

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

class CEMBAutoBuffer
{
public:
	CEMBAutoBuffer(ST_EMBTRANSMSG& msgIn)
	{
		//calc the needed size
#ifdef _UNICODE
		m_nBuffSize = 256+(msgIn.strGuid.GetLength()+msgIn.strData.GetLength())*2;
#else
		m_nBuffSize = 256+msgIn.strGuid.GetLength()+msgIn.strData.GetLength();

#endif
		m_pBuff = new char[m_nBuffSize];

	}

	CEMBAutoBuffer(int nLen)
	{
		m_pBuff = new char[nLen];
	}


	~CEMBAutoBuffer()
	{
		if (m_pBuff)
		{
			delete[] m_pBuff;
			m_pBuff = NULL;
		}
	}

	operator char*(){return m_pBuff;}
	operator LONG_PTR (){return (LONG_PTR)m_pBuff;}
	int GetSize(){return m_nBuffSize;}
private:
	int m_nBuffSize;
	char* m_pBuff;
};