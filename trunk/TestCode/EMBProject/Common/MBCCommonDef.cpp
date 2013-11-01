#include "stdafx.h"
#include "MBCCommonDef.h"
 BOOL ST_TXMSGBASE::operator <<( CTxSerialize& ar) const
{
	ar << nMsgType;
	ar << nMsgId;
	ar << nRequestSendTime;
	ar << nReplySendTime;
	ar << nMsgState;
	return TRUE;
}
 BOOL ST_TXMSGBASE::operator >>(CTxSerialize& ar)  
{
	ar >> nMsgType;
	ar >> nMsgId;
	ar >> nRequestSendTime;
	ar >> nReplySendTime;
	ar >> nMsgState;

	return TRUE;
}


BOOL ST_TXCHANNELINFO::operator <<( CTxSerialize& ar) const
{
	ar<< szChannelId;
	LONG nLen = sizeof(SOCKADDR_IN);
	ar.Serialize((BYTE*)&addrDest, nLen);
	ar.Serialize((BYTE*)&addrDestSecond, nLen);
	ar.Serialize((BYTE*)&addrCtrl, nLen);

	return TRUE;
}
BOOL ST_TXCHANNELINFO::operator >>(CTxSerialize& ar)  
{
	ar >> szChannelId;
	LONG nLen = sizeof(SOCKADDR_IN);
	ar.Serialize((BYTE*)&addrDest, nLen);
	ar.Serialize((BYTE*)&addrDestSecond, nLen);
	ar.Serialize((BYTE*)&addrCtrl, nLen);
	return TRUE;
}


 BOOL ST_TXMSG_LIVEQA::operator <<( CTxSerialize& ar) const
{
	ST_TXMSGBASE::operator <<(ar);
	ar << nSelfType;
	ar << nRemoteState;
	return TRUE;
}
 BOOL ST_TXMSG_LIVEQA::operator >>(CTxSerialize& ar)  
{
	ST_TXMSGBASE::operator >>(ar);
	ar >> nSelfType;
	ar >> nRemoteState;
	return TRUE;
}


 BOOL ST_TXMSG_ACTQA::operator <<( CTxSerialize& ar) const
{
	ST_TXMSGBASE::operator <<(ar);
	LONG nCharLen = strlen(strChId);
	ar.Serialize((char*)strChId, nCharLen);
	LONG nLen = sizeof(SOCKADDR_IN);
	ar.Serialize((BYTE*)&addrDest, nLen);
	ar << nResult;

	return TRUE;
}
 BOOL ST_TXMSG_ACTQA::operator >>(CTxSerialize& ar)  
{
	ST_TXMSGBASE::operator >>(ar);
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

  void CHInfoV62CHInfo(ST_MBCCHANNELINFO_FORVC6& infoV6, ST_TXCHANNELINFO& infoOut)
 {
	 infoOut.szChannelId = infoV6.szChannelId;
	 infoOut.addrDest = infoV6.addrDest;
	 infoOut.addrDestSecond = infoV6.addrDestSecond;
	 infoOut.addrCtrl = infoV6.addrCtrl;
 }