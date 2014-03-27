// NetMessageTranslate.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"

#include "NetMsgTransHeader.h"
#include "MBCTransMsg.h"
#include "EMBMessageDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern "C"
int EMBNETMSGTRANSDLL_API ExpFunPackEMBTcpMsg( const int nTcpMsgTypeIn, const char* szGuidIn, const char* szXmlIn, char* szBuffOut, const int nbuffLenIn, int& nUsedOut )
{
	if (szGuidIn == NULL || szXmlIn == NULL)
	{
		return -1;
	}
	int nUsed;
	ST_EMBTRANSMSG msgRet(nTcpMsgTypeIn);
	msgRet.nMsgId = 0;
	msgRet.strGuid = szGuidIn;
	msgRet.strData = szXmlIn;
	CEMBAutoBuffer buffer(msgRet);
	PackMBCMsg(msgRet, buffer, buffer.GetSize(), nUsed);
	if (nbuffLenIn < nUsed)
	{
		return -2;
	}
	nUsedOut = nUsed;
	memcpy(szBuffOut, buffer, nUsed);
	return 0;
}

extern "C"
int EMBNETMSGTRANSDLL_API ExpFunUnPackEMBTcpMsg( const char* szMsgBuffIn, const int nMsgbuffLenIn, int& nTcpMsgTypeOut, char* szGuidOut, char* szXmlBuffOut, const int nXmlBuffLen )
{

	if (nMsgbuffLenIn <= 0 || nXmlBuffLen <=0)
	{
		return -1;
	}
	int nMsgType =  GetMBCMsgType((char*)szMsgBuffIn, (int)nMsgbuffLenIn);
	if (nMsgType == msgtype_NONE)
	{
		return -2;
	}
	nTcpMsgTypeOut = nMsgType;

	ST_EMBTRANSMSG msgIn(nMsgType);
	if(UnPackMBCMsg((char*)szMsgBuffIn, (int)nMsgbuffLenIn, msgIn) != 0)
	{
		ASSERT(FALSE);
		return -4;
	}

	if (msgIn.strData.GetLength() > nXmlBuffLen -1)
	{
		return -3;
	}

	strcpy(szGuidOut, msgIn.strGuid);
	strcpy(szXmlBuffOut, msgIn.strData);
	
	return 0;
}
