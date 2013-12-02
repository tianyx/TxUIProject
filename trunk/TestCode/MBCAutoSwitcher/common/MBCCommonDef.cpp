#include "stdafx.h"
#include "MBCCommonDef.h"
 BOOL ST_MBCMSGBASE::operator <<( CTxSerialize& ar) const
{
	ar << nMsgType;
	ar << nMsgId;
	ar << nRequestSendTime;
	ar << nReplySendTime;
	ar << nMsgState;
	return TRUE;
}
 BOOL ST_MBCMSGBASE::operator >>(CTxSerialize& ar)  
{
	ar >> nMsgType;
	ar >> nMsgId;
	ar >> nRequestSendTime;
	ar >> nReplySendTime;
	ar >> nMsgState;

	return TRUE;
}


BOOL ST_MBCCHANNELINFO::operator <<( CTxSerialize& ar) const
{
	ar<< szChannelId;
	LONG nLen = sizeof(SOCKADDR_IN);
	ar.Serialize((BYTE*)&addrDest, nLen);
	ar.Serialize((BYTE*)&addrDestSecond, nLen);
	ar.Serialize((BYTE*)&addrCtrl, nLen);

	return TRUE;
}
BOOL ST_MBCCHANNELINFO::operator >>(CTxSerialize& ar)  
{
	ar >> szChannelId;
	LONG nLen = sizeof(SOCKADDR_IN);
	ar.Serialize((BYTE*)&addrDest, nLen);
	ar.Serialize((BYTE*)&addrDestSecond, nLen);
	ar.Serialize((BYTE*)&addrCtrl, nLen);
	return TRUE;
}


 BOOL ST_MBCMSG_LIVEQA::operator <<( CTxSerialize& ar) const
{
	ST_MBCMSGBASE::operator <<(ar);
	ar << nSelfType;
	ar << nRemoteState;
	ar <<vCHInfo;
	return TRUE;
}
 BOOL ST_MBCMSG_LIVEQA::operator >>(CTxSerialize& ar)  
{
	ST_MBCMSGBASE::operator >>(ar);
	ar >> nSelfType;
	ar >> nRemoteState;

	ar >>vCHInfo;

	return TRUE;
}


 BOOL ST_MBCMSG_ACTQA::operator <<( CTxSerialize& ar) const
{
	ST_MBCMSGBASE::operator <<(ar);
	LONG nCharLen = strlen(strChId);
	ar.Serialize((char*)strChId, nCharLen);
	LONG nLen = sizeof(SOCKADDR_IN);
	ar.Serialize((BYTE*)&addrDest, nLen);
	ar << nResult;

	return TRUE;
}
 BOOL ST_MBCMSG_ACTQA::operator >>(CTxSerialize& ar)  
{
	ST_MBCMSGBASE::operator >>(ar);
	LONG nCharLen = 64;
	ZeroMemory(strChId, 64);
	ar.Serialize(strChId, nCharLen);
	LONG nLen = sizeof(SOCKADDR_IN);
	ar.Serialize((BYTE*)&addrDest, nLen);
	ar >> nResult;
	return TRUE;
}


 BOOL ST_OBJSTATEINFO::operator <<( CTxSerialize& ar) const
{
	ar << pObj;
	ar <<nObjType;
	ar <<strName;
	ar <<strChId;
	ar <<nSelfState;
	ar <<addrRemote;
	ar <<addrLocal;
	ar <<remoteInfo;
	return TRUE;
}
 BOOL ST_OBJSTATEINFO::operator >>(CTxSerialize& ar)  
{
	ar >> pObj;
	ar >>nObjType;
	ar >>strName;
	ar >>strChId;
	ar >>nSelfState;
	ar >>addrRemote;
	ar >>addrLocal;
	ar >>remoteInfo;
	return TRUE;
}

 BOOL ST_GRAPHSTATE::operator <<( CTxSerialize& ar) const
{
	ar << nUniqueId;
	ar << nGraphState;
	ar <<vClientInfo;
	ar <<vSvrInfo;
	ar <<vEndInfo;

	return TRUE;
}
 BOOL ST_GRAPHSTATE::operator >>(CTxSerialize& ar)  
{
	ar >> nUniqueId;
	ar >> nGraphState;
	ar >>vClientInfo;
	ar >>vSvrInfo;
	ar >>vEndInfo;
	return TRUE;
}

 void CHInfoV62CHInfo(ST_MBCCHANNELINFO_FORVC6& infoV6, ST_MBCCHANNELINFO& infoOut)
 {
	 infoOut.szChannelId = infoV6.szChannelId;
	 infoOut.addrDest = infoV6.addrDest;
	 infoOut.addrDestSecond = infoV6.addrDestSecond;
	 infoOut.addrCtrl = infoV6.addrCtrl;
 }

 BOOL ST_CHANGERELAY::operator<<( CTxSerialize& ar ) const
 {
	 ST_MBCMSGBASE::operator <<(ar);
	 LONG nCharLen = strlen(strChId);
	 ar.Serialize((char*)strChId, nCharLen);
	 LONG nLen = sizeof(SOCKADDR_IN);
	 ar.Serialize((BYTE*)&addrRelay, nLen);
	 ar << nId;

	 return TRUE;
 }

 BOOL ST_CHANGERELAY::operator>>( CTxSerialize& ar )
 {
	 ST_MBCMSGBASE::operator >>(ar);
	 LONG nCharLen = 64;
	 ZeroMemory(strChId, 64);
	 ar.Serialize(strChId, nCharLen);
	 LONG nLen = sizeof(SOCKADDR_IN);
	 ar.Serialize((BYTE*)&addrRelay, nLen);
	 ar >> nId;
	 return TRUE;
 }
