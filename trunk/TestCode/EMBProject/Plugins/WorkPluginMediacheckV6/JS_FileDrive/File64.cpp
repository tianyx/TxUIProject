// File64.cpp: implementation of the CFile64 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "File64.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFile64::CFile64()
{

}

CFile64::~CFile64()
{

}

ULONGLONG CFile64::Seek(LONGLONG lOff, UINT nFrom)
{
	ASSERT_VALID(this);
	ASSERT((HANDLE)m_hFile != INVALID_HANDLE_VALUE);
	ASSERT(nFrom == begin || nFrom == end || nFrom == current);
	ASSERT(begin == FILE_BEGIN && end == FILE_END && current == FILE_CURRENT);
	
	LARGE_INTEGER liOff;
	
	liOff.QuadPart = lOff;
	liOff.LowPart = ::SetFilePointer((HANDLE)m_hFile, liOff.LowPart, &liOff.HighPart,
		(DWORD)nFrom);
	if (liOff.LowPart  == (DWORD)-1)
		if (::GetLastError() != NO_ERROR)
			CFileException::ThrowOsError((LONG)::GetLastError(), m_strFileName);
		
		return liOff.QuadPart;
}

ULONGLONG CFile64::GetPosition() 
{
	ASSERT_VALID(this);
	ASSERT((HANDLE)m_hFile != INVALID_HANDLE_VALUE);
	
	LARGE_INTEGER liPos;
	liPos.QuadPart = 0;
	liPos.LowPart = ::SetFilePointer((HANDLE)m_hFile, liPos.LowPart, &liPos.HighPart , FILE_CURRENT);
	if (liPos.LowPart == (DWORD)-1)
		if (::GetLastError() != NO_ERROR)
			CFileException::ThrowOsError((LONG)::GetLastError(), m_strFileName);
		
		return liPos.QuadPart;
}

void CFile64::LockRange(ULONGLONG dwPos, ULONGLONG dwCount)
{
	ASSERT_VALID(this);
	ASSERT((HANDLE)m_hFile != INVALID_HANDLE_VALUE);
	
	ULARGE_INTEGER liPos;
	ULARGE_INTEGER liCount;
	
	liPos.QuadPart = dwPos;
	liCount.QuadPart = dwCount;
	if (!::LockFile((HANDLE)m_hFile, liPos.LowPart, liPos.HighPart, liCount.LowPart, 
		liCount.HighPart))
	{
		CFileException::ThrowOsError((LONG)::GetLastError(), m_strFileName);
	}
}

void CFile64::UnlockRange(ULONGLONG dwPos, ULONGLONG dwCount)
{
	ASSERT_VALID(this);
	ASSERT((HANDLE)m_hFile != INVALID_HANDLE_VALUE);
	
	ULARGE_INTEGER liPos;
	ULARGE_INTEGER liCount;
	
	liPos.QuadPart = dwPos;
	liCount.QuadPart = dwCount;
	if (!::UnlockFile((HANDLE)m_hFile, liPos.LowPart, liPos.HighPart, liCount.LowPart,
		liCount.HighPart))
	{
		CFileException::ThrowOsError((LONG)::GetLastError(), m_strFileName);
	}
}

void CFile64::SetLength(ULONGLONG dwNewLen)
{
	ASSERT_VALID(this);
	ASSERT((HANDLE)m_hFile != INVALID_HANDLE_VALUE);
	
	Seek(dwNewLen, (UINT)begin);
	
	if (!::SetEndOfFile((HANDLE)m_hFile))
		CFileException::ThrowOsError((LONG)::GetLastError(), m_strFileName);
}

ULONGLONG CFile64::GetLength() 
{
	ASSERT_VALID(this);
	
	ULARGE_INTEGER liSize;
	liSize.LowPart = ::GetFileSize((HANDLE)m_hFile, &liSize.HighPart);
	if (liSize.LowPart == (DWORD)-1)
		if (::GetLastError() != NO_ERROR)
			CFileException::ThrowOsError((LONG)::GetLastError(), m_strFileName);
		
		return liSize.QuadPart;
}

