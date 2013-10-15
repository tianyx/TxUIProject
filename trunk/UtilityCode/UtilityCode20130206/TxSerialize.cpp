/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxSerialize.cpp
	file base:	TxSerialize
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "TxSerialize.h"
#include "shlwapi.h"
#define MAX_SERIALIZE_BUFFER 4096

CTxSerialize::CTxSerialize(void* pDataIn, LONG lLenIn, BOOL bOut/* = TRUE*/)
{
	m_bSelfBuffer = FALSE;
	m_pData = NULL;
	m_lmaxlen = 0;
	m_lUsed = 0;
	m_pPos = NULL;
	if (bOut && (pDataIn == NULL || lLenIn < 0))
	{
		ASSERT(FALSE);
		return;
	}

	if (pDataIn != NULL) //can in/out
	{
		m_pData = (BYTE*)pDataIn;
		m_lmaxlen = lLenIn;
	}
	else // only in
	{
		m_bSelfBuffer = TRUE;
		m_lmaxlen = MAX_SERIALIZE_BUFFER;
		if (lLenIn > MAX_SERIALIZE_BUFFER)
		{
			m_lmaxlen = lLenIn;
		}
		m_pData = new BYTE[m_lmaxlen];
	}
	m_pPos = m_pData;
	m_bOut = bOut;
}

CTxSerialize::CTxSerialize()
{
	m_bOut = FALSE;
	m_lUsed = 0;
	m_bSelfBuffer = TRUE;
	m_pData = new BYTE[MAX_SERIALIZE_BUFFER];
	m_pPos = m_pData;
	m_lmaxlen = MAX_SERIALIZE_BUFFER;
}

CTxSerialize::~CTxSerialize(void)
{
	if (m_bSelfBuffer && m_pData)
	{
		delete[] m_pData;
	}
}

BOOL CTxSerialize::operator <<( const LONG data )
{
	if (m_bOut /*|| !CanSaveIn(data)*/)
	{
		return FALSE;
	}

	long nLen = sizeof(data);
	AutoIncreaseBuffer(nLen);
	memcpy(m_pPos, &data, nLen);
	m_pPos += nLen;
	m_lUsed += nLen;
	return TRUE;
}

BOOL CTxSerialize::operator >>( LONG& data )
{
	if (!m_bOut /*|| !CanWriteOut(data)*/)
	{
		return FALSE;
	}
	long nlen = sizeof(data);
	memcpy(&data, m_pPos, sizeof(data));
	m_pPos += nlen;
	m_lUsed += nlen;
	return TRUE;
}

BOOL CTxSerialize::operator>>( ULONG& data )
{
	if (!m_bOut /*|| !CanWriteOut(data)*/)
	{
		return FALSE;
	}
	long nlen = sizeof(data);
	memcpy(&data, m_pPos, sizeof(data));
	m_pPos += nlen;
	m_lUsed += nlen;
	return TRUE;
}

BOOL CTxSerialize::operator<<( const ULONG data )
{
	if (m_bOut /*|| !CanSaveIn(data)*/)
	{
		return FALSE;
	}

	long nLen = sizeof(data);
	AutoIncreaseBuffer(nLen);
	memcpy(m_pPos, &data, nLen);
	m_pPos += nLen;
	m_lUsed += nLen;
	return TRUE;
}


BOOL CTxSerialize::operator>>( ULONG64& data )
{
	if (!m_bOut /*|| !CanWriteOut(data)*/)
	{
		return FALSE;
	}
	long nlen = sizeof(data);
	memcpy(&data, m_pPos, sizeof(data));
	m_pPos += nlen;
	m_lUsed += nlen;
	return TRUE;
}

BOOL CTxSerialize::operator<<( const ULONG64 data )
{
	if (m_bOut /*|| !CanSaveIn(data)*/)
	{
		return FALSE;
	}

	long nLen = sizeof(data);
	AutoIncreaseBuffer(nLen);
	memcpy(m_pPos, &data, nLen);
	m_pPos += nLen;
	m_lUsed += nLen;
	return TRUE;
}


BOOL CTxSerialize::operator >>( INT& data )
{
	if (!m_bOut /*|| !CanWriteOut(data)*/)
	{
		return FALSE;
	}
	long nlen = sizeof(data);
	memcpy(&data, m_pPos, sizeof(data));
	m_pPos += nlen;
	m_lUsed += nlen;
	return TRUE;
}

BOOL CTxSerialize::operator <<( const INT data )
{
	if (m_bOut /*|| !CanSaveIn(data)*/)
	{
		return FALSE;
	}

	long nLen = sizeof(data);
	AutoIncreaseBuffer(nLen);
	memcpy(m_pPos, &data, sizeof(data));
	m_pPos += nLen;
	m_lUsed += nLen;
	return TRUE;
}

BOOL CTxSerialize::operator >>( bool& data )
{
	LONG lTmp;
	BOOL bRet = operator>>(lTmp);
	data = lTmp > 0? true:false;
	return bRet;
}

BOOL CTxSerialize::operator <<( const bool& data )
{
	LONG ltmp = data? 1:0;
	return operator <<(ltmp);
}

BOOL CTxSerialize::operator >>( CString& data )
{
	data.Empty();
	LONG nLen = 0;
	memcpy(&nLen, m_pPos, sizeof(nLen));
	if (!m_bOut || (m_lmaxlen - m_lUsed < nLen))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	LONG szlen = (nLen / sizeof(TCHAR)) + 1;
	TCHAR* ptmp = new TCHAR[szlen];
	ZeroMemory(ptmp, szlen* sizeof(TCHAR));
	BOOL bRet = Serialize(ptmp, szlen);
	data = ptmp;

	delete[] ptmp;

	return bRet;

}



BOOL CTxSerialize::operator <<( const CString& data )
{
	LONG lLen = data.GetLength();
	AutoIncreaseBuffer(lLen);
	if (m_bOut)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	TCHAR* pTmp = new TCHAR[lLen + 1];
	ZeroMemory(pTmp, (lLen+ 1)* sizeof(TCHAR));
	StrCpy(pTmp, data);
	BOOL bRet = Serialize(pTmp, lLen);
	delete[] pTmp;
	return bRet;
}


BOOL CTxSerialize::operator >>( INT64& data )
{
	if (!m_bOut /*|| !CanWriteOut(data)*/)
	{
		return FALSE;
	}
	long nlen = sizeof(data);
	memcpy(&data, m_pPos, sizeof(data));
	m_pPos += nlen;
	m_lUsed += nlen;
	return TRUE;
}

BOOL CTxSerialize::operator <<( const INT64 data )
{
	if (m_bOut /*|| !CanSaveIn(data)*/)
	{
		return FALSE;
	}

	long nLen = sizeof(data);
	AutoIncreaseBuffer(nLen);
	memcpy(m_pPos, &data, sizeof(data));
	m_pPos += nLen;
	m_lUsed += nLen;
	return TRUE;
}


BOOL CTxSerialize::operator>>( SOCKADDR_IN& data )
{
	LONG lLen = sizeof(SOCKADDR_IN);
	if (!m_bOut || (m_lmaxlen - m_lUsed < lLen))
	{
		ASSERT(FALSE);
		return FALSE;
	}

	return Serialize((BYTE*)&data, lLen);
}


BOOL CTxSerialize::operator<<( const SOCKADDR_IN& data )
{
	LONG lLen = sizeof(SOCKADDR_IN);
	AutoIncreaseBuffer(lLen);
	if (m_bOut)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	return Serialize((BYTE*)(&data), lLen);

}


BOOL CTxSerialize::Serialize( TCHAR* data, LONG& szlenInOut )
{
	BOOL bRet = TRUE;
	if (m_bOut)
	{
		//write out
		LONG nLen = 0;
		operator>>(nLen);
		if (nLen > (szlenInOut * sizeof(TCHAR)))
		{
			ASSERT(FALSE);
			bRet = FALSE;
		}
		else
		{
			memcpy(data, m_pPos, nLen);
		}
		m_pPos += nLen;
		m_lUsed += nLen;
	}
	else
	{
		//save in
		LONG nLen = szlenInOut * sizeof(TCHAR);
		AutoIncreaseBuffer(nLen);
		operator<<(nLen);
		if (nLen > 0)
		{
			memcpy(m_pPos, data, nLen);
		}
		m_pPos += nLen;
		m_lUsed += nLen;
	}
	return bRet;
}

BOOL CTxSerialize::Serialize(BYTE* data, LONG& szlenInOut )
{
	BOOL bRet = TRUE;
	if (m_bOut)
	{
		//write out
		LONG nLen = 0;
		operator>>(nLen);
		if (nLen > (szlenInOut * sizeof(BYTE)))
		{
			ASSERT(FALSE);
			bRet = FALSE;
		}
		else
		{
			memcpy(data, m_pPos, nLen);
			szlenInOut = nLen;
		}
		m_pPos += nLen;
		m_lUsed += nLen;
	}
	else
	{
		//save in
		LONG nLen = szlenInOut * sizeof(BYTE);
		AutoIncreaseBuffer(nLen);
		operator<<(nLen);
		if (nLen > 0)
		{
			memcpy(m_pPos, data, nLen);
		}
		m_pPos += nLen;
		m_lUsed += nLen;
	}
	return bRet;
}
BOOL CTxSerialize::WriteBuffer( void* pBuffIn, long& lLenInOut )
{
	if (m_bOut || lLenInOut < m_lUsed)
	{
		lLenInOut = m_lUsed;
		return FALSE;
	}
	memcpy(pBuffIn, m_pData, m_lUsed);
	lLenInOut = m_lUsed;
	return TRUE;
}

LONG CTxSerialize::GetUsed()
{
	return m_lUsed;
}

LONG CTxSerialize::WriteToFile( CFile* pFile, const LONG lenIn )
{
	ASSERT(pFile);
	if (m_lmaxlen - m_lUsed < lenIn)
	{
		ASSERT(FALSE);
		return 0;
	}
	pFile->Write(m_pPos, lenIn);
	pFile->Flush();

	m_pPos += lenIn;
	m_lUsed += lenIn;
	return lenIn;
}

const BYTE* CTxSerialize::GetBuffer( long& lSize )
{
	lSize = m_lUsed;
	return m_pData;
}

void CTxSerialize::Reset()
{
	m_lUsed = 0;
	m_pPos = m_pData;
}

BOOL CTxSerialize::AutoIncreaseBuffer( long nNeeded )
{
	if (m_lmaxlen - m_lUsed -1 < nNeeded)
	{
		if (m_bSelfBuffer)
		{
			BYTE* pTmp = m_pData;
			int nNewLen = (nNeeded < m_lmaxlen)? m_lmaxlen*2:(nNeeded+m_lmaxlen*2);
			m_pData = new BYTE[nNewLen];
			memcpy(m_pData, pTmp, m_lmaxlen);
			m_pPos = m_pData + m_lUsed;

			m_lmaxlen = nNewLen;
			delete[] pTmp;
		}
		else
		{
			ASSERT(FALSE);
			return FALSE;
		}
	}

	return TRUE;
}

LONG CTxSerialize::WriteAllDataToFile( CFile* pFile )
{
	ASSERT(pFile);
	if (m_lUsed < 1)
	{
		ASSERT(FALSE);
		return 0;
	}

	pFile->Write(m_pData, m_lUsed);
	pFile->Flush();
	return m_lUsed;
}

LONG CTxSerialize::ReadFromFile( CFile* pFile )
{
	if (!pFile)
	{
		return 0;
	}

	LONG nFileLen = pFile->GetLength();
	BYTE* pTmpData = new BYTE[nFileLen];
	ZeroMemory(pTmpData, nFileLen);
	pFile->Read(pTmpData, nFileLen);
	Serialize(pTmpData, nFileLen);
	delete[] pTmpData;
	return nFileLen;
}

const BYTE* CTxSerialize::GetRemainBuffer( long& lRemainLen)
{
	lRemainLen = m_lmaxlen - m_lUsed;
	return m_pPos;
}
