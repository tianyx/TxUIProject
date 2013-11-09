/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:06
	filename: 	MBCTransMsg.h
	author:		tianyx
	
	purpose:	
*********************************************************************/
#pragma once
#include "MBCCommonDef.h"

#define MBCCHECKSUM  0x58
#define SMALLBUFSIZE 256
#define MIDBUFFSIZE 1024
#define LARGEBUFFSIZE 1024*1024


template <typename T>
int UnPackMBCMsg( char* bufferIn, int nBufferSize, T& objOut )
{
	if (!bufferIn|| bufferIn[0] != (char)MBCCHECKSUM  )
	{
		ASSERT(FALSE);
		return E_FAIL;
	}

	char* pBuff = bufferIn;
	pBuff++;
	int& nMsgSize = (int&)(*pBuff);
	if (nMsgSize != nBufferSize)
	{
		ASSERT(FALSE);
		return E_FAIL;
	}

	pBuff += 4;
	int& nMsgType= (int&)(*pBuff);
	if (nMsgType != objOut.GetType())
	{
		ASSERT(FALSE);
		return E_FAIL;
	}

	pBuff +=4;
	int nstLen = 0;
	CTxSerialize ar(pBuff, nBufferSize- 9, TRUE);
	ar >> objOut;

	return S_OK;
}

template <typename T>
int PackMBCMsg(const T& objIn, char* bufferOut, int nBufferLenIn, int& nUsedOut )
{
	if (nBufferLenIn < 9)
	{
		ASSERT(FALSE);
		return E_FAIL;
	}

	HRESULT hr = S_OK;
	bufferOut[0] = (byte)MBCCHECKSUM; //checksum
	char* pBuff = bufferOut;
	pBuff++;
	int& nMsgSize = (int&)(*pBuff);
	pBuff +=4;
	int& nMsgType= (int&)(*pBuff);
	nMsgType = objIn.GetType();
	pBuff +=4;
	nUsedOut = 9;
	
	CTxSerialize ar(pBuff, nBufferLenIn- nUsedOut, FALSE);
	ar<< objIn;
	nUsedOut += ar.GetUsed();
	nMsgSize = nUsedOut;
	return S_OK;
}


int GetMBCMsgType( char* bufferIn, int nBufferLen );

