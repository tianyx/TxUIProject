#pragma once

#include "txSerialize.h"
using namespace std;

enum ENUM_MBCOBJTYPE
{
	MBCOBJTYPE_BASE = -1,


	MBCOBJTYPE_SVR = 0,
	MBCOBJTYPE_CLIENT = 1,

	MBCOBJTYPE_REMOTEHOST = 200,
};

enum ENUM_MBCSTATE
{
	MBCSTATE_OK = 0,
	MBCSTATE_STOP	= 1,
	MBCSTATE_NETERR = -1,
	MBCSTATE_NOTRESPOND = -2,
	MBCSTATE_STREAMDOWN = -3, // for endobj
};


#define msgtype_NONE	0	
#define msgtype_LIVEQA	1
#define msgtype_ACTQA	3

#define msgState_none 0
#define msgState_Q 1
#define msgState_A 2


#define MBCSENDINGPACKSIZE	64
#pragma pack(4)

struct ST_TXMSGBASE
{
protected:
	int nMsgType;
public:
	INT64 nMsgId;
	LONG nRequestSendTime;
	LONG nReplySendTime;
	int nMsgState;

	ST_TXMSGBASE():nMsgType(msgtype_NONE)
	{
		nRequestSendTime = 0;
		nReplySendTime = 0;
		nMsgId = 0;
		nMsgState = msgState_none;
	}

	int GetType() const {return nMsgType;}

	virtual BOOL operator <<( CTxSerialize& ar) const;
	
	virtual BOOL operator >>(CTxSerialize& ar) ;
};


/*
struct ST_MBCMSG_LIVEREQUEST: public ST_MBCMSGBASE
{
	ST_MBCMSG_LIVEREQUEST()
	{
		nMsgType = msgtype_LIVEREQUEST;
	}
};*/

struct ST_MBCCHANNELINFO_FORVC6
{
	char szChannelId[64];
	SOCKADDR_IN addrDest;
	SOCKADDR_IN addrDestSecond;
	SOCKADDR_IN addrCtrl; 
};



struct ST_TXCHANNELINFO
{
	CString szChannelId;
	SOCKADDR_IN addrDest;
	SOCKADDR_IN addrDestSecond;
	SOCKADDR_IN addrCtrl; //for svr, it's a client ip; for client, it's a  ip to  control svr
	ST_TXCHANNELINFO()
	{
		
	}

	BOOL operator <<( CTxSerialize& ar) const;
	
	BOOL operator >>(CTxSerialize& ar);
	


};
struct ST_TXMSG_LIVEQA : public ST_TXMSGBASE
{
	int nSelfType;
	int nRemoteState;
	ST_TXMSG_LIVEQA()
	{
		nMsgType = msgtype_LIVEQA;
		nSelfType = 0;
		nRemoteState = MBCSTATE_OK;
	}

	virtual BOOL operator <<( CTxSerialize& ar) const;
	
	virtual BOOL operator >>(CTxSerialize& ar);  

};

struct ST_TXMSG_ACTQA: public ST_TXMSGBASE
{
	int nActType;
	char strChId[64];
	SOCKADDR_IN addrDest;
	int nResult;
	ST_TXMSG_ACTQA()
	{
		nMsgType = msgtype_ACTQA;
		nActType = 0;
		nResult = 0;
		strChId[0] = 0;
	}

	virtual BOOL operator <<( CTxSerialize& ar) const;
	
	virtual BOOL operator >>(CTxSerialize& ar);  
	

};

/*
struct ST_MBCMSG_ACTREPLY: public ST_MBCMSGBASE
{
	int nActId;
	int nResult;

	ST_MBCMSG_ACTREPLY()
	{
		nMsgType = msgtype_ACTREPLY;
		nResult = 0;
		nActId = 0;
	}

	virtual BOOL operator <<( CTxSerialize& ar) const
	{
		ST_MBCMSGBASE::operator <<(ar);
		ar << nActId;
		ar << nResult;
		return TRUE;
	}
	virtual BOOL operator >>(CTxSerialize& ar)  
	{
		ST_MBCMSGBASE::operator >>(ar);

		ar >> nActId;
		ar >> nResult;
		return TRUE;
	}

};
*/

struct ST_OBJSTATEINFO
{
	ULONG pObj;
	int nObjType;
	CString  strName;
	CString strChId; //only for backend obj
	int nSelfState;
	SOCKADDR_IN addrRemote;
	SOCKADDR_IN addrLocal;
	ST_TXMSG_LIVEQA remoteInfo;

	ST_OBJSTATEINFO()
	{
		pObj = NULL;
		nObjType = NULL;
		nSelfState = MBCSTATE_STOP;
	}

	virtual BOOL operator <<( CTxSerialize& ar) const;
	
	virtual BOOL operator >>(CTxSerialize& ar);  
	

};


void CHInfoV62CHInfo(ST_MBCCHANNELINFO_FORVC6& infoV6, ST_TXCHANNELINFO& infoOut);

#pragma pack()

