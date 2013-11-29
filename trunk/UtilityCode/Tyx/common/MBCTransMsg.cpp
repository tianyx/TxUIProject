#include "StdAfx.h"
#include "MBCTransMsg.h"





int GetMBCMsgType( char* bufferIn, int nBufferLen )
{
	if (bufferIn == NULL || nBufferLen < 9 || (bufferIn[0] != (char)MBCCHECKSUM))
	{
		return msgtype_NONE;
	}

	char* pBuff = bufferIn;
	pBuff += 5;
	int* pnMsgType  = (int*) pBuff;
	return *pnMsgType;
}