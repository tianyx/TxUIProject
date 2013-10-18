#include "StdAfx.h"
#include "IMSerialize.h"
#include "shlwapi.h"
#define MAX_SERIALIZE_BUFFER 4096

CIMSerialize::CIMSerialize(void* pDataIn, LONG lLenIn, BOOL bOut/* = TRUE*/)
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

CIMSerialize::CIMSerialize()
{
	m_bOut = FALSE;
	m_lUsed = 0;
	m_bSelfBuffer = TRUE;
	m_pData = new BYTE[MAX_SERIALIZE_BUFFER];
	m_pPos = m_pData;
	m_lmaxlen = MAX_SERIALIZE_BUFFER;
}

CIMSerialize::CIMSerialize( CFile* pFile )
{
	m_bOut = TRUE;
	m_lUsed = 0;
	m_bSelfBuffer = TRUE;
	m_pData = NULL;
	m_lmaxlen = 0;
	if (!pFile)
	{
		ASSERT(FALSE);
		return;
	}
	m_lmaxlen = pFile->GetLength();
	m_pData = new BYTE[m_lmaxlen];
	ZeroMemory(m_pData, m_lmaxlen);
	pFile->Read(m_pData, m_lmaxlen);
	m_pPos = m_pData;
}

CIMSerialize::~CIMSerialize(void)
{
	if (m_bSelfBuffer && m_pData)
	{
		delete[] m_pData;
	}
}

BOOL CIMSerialize::operator <<( const LONG data )
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

BOOL CIMSerialize::operator >>( LONG& data )
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

BOOL CIMSerialize::operator>>( ULONG& data )
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

BOOL CIMSerialize::operator<<( const ULONG data )
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


BOOL CIMSerialize::operator>>( ULONG64& data )
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

BOOL CIMSerialize::operator<<( const ULONG64 data )
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


BOOL CIMSerialize::operator >>( INT& data )
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

BOOL CIMSerialize::operator <<( const INT data )
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

BOOL CIMSerialize::operator >>( bool& data )
{
	LONG lTmp;
	BOOL bRet = operator>>(lTmp);
	data = lTmp > 0? true:false;
	return bRet;
}

BOOL CIMSerialize::operator <<( const bool& data )
{
	LONG ltmp = data? 1:0;
	return operator <<(ltmp);
}

BOOL CIMSerialize::operator >>( CString& data )
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



BOOL CIMSerialize::operator <<( const CString& data )
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


BOOL CIMSerialize::Serialize( TCHAR* data, LONG& szlenInOut )
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

BOOL CIMSerialize::Serialize(BYTE* data, LONG& szlenInOut )
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
BOOL CIMSerialize::WriteBuffer( void* pBuffIn, long& lLenInOut )
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

LONG CIMSerialize::GetUsed()
{
	return m_lUsed;
}

LONG CIMSerialize::SerializeFileOut( CFile* pFile, const LONG lenIn )
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

const BYTE* CIMSerialize::GetBuffer( long& lSize )
{
	lSize = m_lUsed;
	return m_pData;
}

void CIMSerialize::Reset()
{
	m_lUsed = 0;
	m_pPos = m_pData;
}

BOOL CIMSerialize::AutoIncreaseBuffer( long nNeeded )
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

LONG CIMSerialize::SerializeFileOut( CFile* pFile )
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

LONG CIMSerialize::SerializeFileIn( CFile* pFile )
{
	if (m_bOut)
	{
		ASSERT(FALSE);
		return 0;
	}

	if (!pFile)
	{
		ASSERT(FALSE);
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

const BYTE* CIMSerialize::GetRemainBuffer( long& lRemainLen)
{
	lRemainLen = m_lmaxlen - m_lUsed;
	return m_pPos;
}
