#pragma once

#include "txSerialize.h"
using namespace std;

#define MSG_MBCNETSOCK WM_USER + 7999
#define MSG_MBCREQUESTBASEOBJ WM_USER + 8100

#define IDTIMER_RECONN 1032
#define IDTIMER_LIVECHECK 1031

#define IDTIMER_GRAPHCHECK 1033
#define IDTIMER_COOLDOWN	1034

#define MAXRECVBUFF 4000
#define MAXGRAPHBUFF 4000

enum ENUM_MBCOBJTYPE
{
	MBCOBJTYPE_BASE = -1,


	MBCOBJTYPE_SVR = 0,
	MBCOBJTYPE_CLIENT = 1,

	MBCOBJTYPE_ENDBASE = 100,
	MBCOBJTYPE_ENDMASTER = 101,
	MBCOBJTYPE_ENDSLAVE = 102,
	MBCOBJTYPE_ENDBACK = 103,
	MBCOBJTYPE_ENDMAX = 104,
	
	MBCOBJTYPE_REMOTEHOST = 200,
	MBCOBJTYPE_GRAPHHOST= 201,
	MBCOBJTYPE_GRAPHCLIENT= 202,


};


enum ENUM_MBCSTATE
{
	MBCSTATE_OK = 0,
	MBCSTATE_STOP	= 1,
	MBCSTATE_NETERR = -1,
	MBCSTATE_NOTRESPOND = -2,
	MBCSTATE_STREAMDOWN = -3, // for endobj
};



#define selfType_NONE	-1
#define selfType_CLIENT 0
#define selfType_SVRMASTER 1
#define selfType_SVRSECOND 2

#define actid_notsended -1
#define actid_closed  -2  //replyed and close
#define actid_timeout -3

#define actType_none 0
#define actType_changeMBCDest 1
#define actType_relayDest 2

#define actResult_none 0
#define actResult_ok 0
#define actResult_failed -1

#define msgtype_NONE	0	
#define msgtype_LIVEQA	1
#define msgtype_ACTQA	3
#define msgtype_GraphState 4
#define msgtype_ChangeRelay 5	//for 2->1 relay mode

#define msgState_none 0
#define msgState_Q 1
#define msgState_A 2

#define graphSate_none -1
#define graphState_running 1
#define graphState_stop 0


#define MBCSENDINGPACKSIZE	64
#pragma pack(4)

struct ST_MBCMSGBASE
{
protected:
	int nMsgType;
public:
	INT64 nMsgId;
	LONG nRequestSendTime;
	LONG nReplySendTime;
	int nMsgState;

	ST_MBCMSGBASE():nMsgType(msgtype_NONE)
	{
		nRequestSendTime = 0;
		nReplySendTime = 0;
		nMsgId = 0;
		nMsgState = msgState_none;
	}

	int GetType() const {return nMsgType;}

	virtual BOOL operator <<( CTxSerialize& ar) const;
	
	virtual BOOL operator >>(CTxSerialize& ar)  ;



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



struct ST_MBCCHANNELINFO
{
	CString szChannelId;
	SOCKADDR_IN addrDest;
	SOCKADDR_IN addrDestSecond;
	SOCKADDR_IN addrCtrl; //for svr, it's a client ip; for client, it's a  ip to  control svr
	ST_MBCCHANNELINFO()
	{
		
	}

	BOOL operator <<( CTxSerialize& ar) const;
	
	BOOL operator >>(CTxSerialize& ar);
	


};
struct ST_MBCMSG_LIVEQA : public ST_MBCMSGBASE
{
	int nSelfType;
	int nRemoteState;
	std::vector<ST_MBCCHANNELINFO> vCHInfo;

	ST_MBCMSG_LIVEQA()
	{
		nMsgType = msgtype_LIVEQA;
		nSelfType = selfType_NONE;
		nRemoteState = MBCSTATE_OK;
	}

	virtual BOOL operator <<( CTxSerialize& ar) const;
	
	virtual BOOL operator >>(CTxSerialize& ar);  



};

struct ST_MBCMSG_ACTQA: public ST_MBCMSGBASE
{
	int nActType;
	char strChId[64];
	SOCKADDR_IN addrDest;
	int nResult;
	ST_MBCMSG_ACTQA()
	{
		nMsgType = msgtype_ACTQA;
		nActType = actType_changeMBCDest;
		nResult = actResult_none;
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
	ST_MBCMSG_LIVEQA remoteInfo;

	ST_OBJSTATEINFO()
	{
		pObj = NULL;
		nObjType = NULL;
		nSelfState = MBCSTATE_STOP;
	}

	virtual BOOL operator <<( CTxSerialize& ar) const;
	
	virtual BOOL operator >>(CTxSerialize& ar);  
	

};


struct ST_GRAPHSTATE
{
	int nUniqueId; //this id must init after graph loaded
	int nGraphState;
	int GetType() const {return msgtype_GraphState;}
	vector<ST_OBJSTATEINFO> vClientInfo;
	vector<ST_OBJSTATEINFO> vSvrInfo;
	vector<ST_OBJSTATEINFO> vEndInfo;

	ST_GRAPHSTATE()
	{
		nUniqueId = -1;
		nGraphState = graphSate_none;
	}


	virtual BOOL operator <<( CTxSerialize& ar) const;
	
	virtual BOOL operator >>(CTxSerialize& ar)  ;
};

void CHInfoV62CHInfo(ST_MBCCHANNELINFO_FORVC6& infoV6, ST_MBCCHANNELINFO& infoOut);


struct ST_CHANGERELAY: public ST_MBCMSGBASE
{
	int nId;
	char strChId[64];
	SOCKADDR_IN addrRelay;
	ST_CHANGERELAY()
	{
		nMsgType = msgtype_ChangeRelay;
		addrRelay.sin_family = 0;
		ZeroMemory(strChId, 64);
		nId = -1;
	}

	virtual BOOL operator <<( CTxSerialize& ar) const;

	virtual BOOL operator >>(CTxSerialize& ar);  


};
#pragma pack()

