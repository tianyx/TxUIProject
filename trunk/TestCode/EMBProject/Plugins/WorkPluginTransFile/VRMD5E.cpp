/* ************************************************************** *
 *	VRMD5E.cpp
 *  CVRMD5E 的实现
 *
 *  Created on:      2008-03-31 16:25:00
 *  Original author: 张鲁平
 *
 *	功能：MD5数据加密方法实现.
 *
 * *************************************************************** */

#include "stdafx.h"
#include "VRMD5E.h"
// #include "comdef.h"

// CVRMD5E

BOOL CVRMD5E::MD5Encrypt(CHAR* pcharCtxSrc, LONG lCtxLen, CHAR* pcharMD5CtxDst)
{
	// TODO: Add your implementation code here
	if( pcharCtxSrc == NULL )
	{
		return FALSE;
	}
	
	if( pcharMD5CtxDst == NULL )
	{
		return FALSE;
	}

	try
	{
		m_cMD5.MD5Init( &m_struMD5Ctx );
		m_cMD5.MD5Update( &m_struMD5Ctx, ( unsigned char *)pcharCtxSrc,lCtxLen );
		m_cMD5.MD5Final( ( unsigned char *)pcharMD5CtxDst, &m_struMD5Ctx );
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

HRESULT CVRMD5E::MD5EncryptFile(CHAR* pcharFliePath, CHAR* pcharMD5CtxDst)
{
	// TODO: Add your implementation code here
	if( pcharFliePath == NULL )
	{
		return S_FALSE;
	}

	if( pcharMD5CtxDst == NULL )
	{
		return S_FALSE;
	}

	HANDLE hFile;
	hFile = CreateFile(pcharFliePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( hFile == INVALID_HANDLE_VALUE ) 
	{ 
		printf( "Could not open file (error %d)\n", GetLastError() );
		return S_FALSE;
	}

	m_dwLength = 0;
	memset( m_pBuffer, 0, m_dwBufferSize );
	ULARGE_INTEGER tmpSize;
	tmpSize.LowPart = GetFileSize(hFile, &tmpSize.HighPart);
	if (tmpSize.LowPart ==  INVALID_FILE_SIZE)
	{
		ASSERT(FALSE);
		return S_FALSE;
	}

	m_FileLength = tmpSize.QuadPart;
	m_nCurrReadPos = 0;

	try
	{
		m_cMD5.MD5Init( &m_struMD5Ctx );

		DWORD dwBefore = GetTickCount(); // --------------
		do
		{
			ReadFile( hFile, m_pBuffer, m_dwBufferSize, &m_dwLength, NULL );
			m_cMD5.MD5Update( &m_struMD5Ctx, m_pBuffer, m_dwLength );
			m_nCurrReadPos += m_dwLength;
		}
		while ( m_dwLength > 0 );

		DWORD dwEnd = GetTickCount();// --------------

		m_cMD5.MD5Final( ( unsigned char *)pcharMD5CtxDst, &m_struMD5Ctx );
	}
	// -------- Report any file exceptions in debug mode only ----------------
	catch (...)
	{
		return S_FALSE;
	}

	CloseHandle( hFile );

	return S_OK;
}

BOOL CVRMD5E::SetFileBuffer(ULONG ulFileBuffer)
{
	// TODO: Add your implementation code here
	Resume();

	m_dwBufferSize = ulFileBuffer;

	m_pBuffer = new BYTE[m_dwBufferSize];

	return S_OK;
}

// -------- 内部环境初始化和释放 ---------------------------------
void  CVRMD5E::Init()
{
	m_FileLength = 0;
	m_nCurrReadPos = 0;
	m_dwLength = 0;
	m_dwBufferSize = 1024*1024;

	m_pBuffer = new BYTE[m_dwBufferSize];
}

void  CVRMD5E::Resume()
{
	if ( m_pBuffer != NULL )
	{
		delete m_pBuffer;
		m_pBuffer = NULL;
	}
}

int CVRMD5E::GetProgress()
{
	if (m_FileLength > 0)
	{
		if (m_nCurrReadPos == m_FileLength)
		{
			//100 will wait the task finished.
			return 99;
		}
		else
		{
			return (m_nCurrReadPos*100 /m_FileLength);
		}
	}
	else
	{
		return 0;
	}
}

// ---------------------------------------------------------------

CString CMD5CALEX::GetResult()
{
		CTime t = CTime::GetCurrentTime();

		char ucMD5Ret[16];
		memset( ucMD5Ret, 0, 16 );
		char strClipMD5[33];
		memset( strClipMD5, 0, 33 );


		m_cMD5.MD5Final( ( unsigned char *)ucMD5Ret, &m_struMD5Ctx );

		for (int i = 0; i < 16; i++)
		{
			char cmd5[3];
			sprintf(cmd5,"%02x",(BYTE)ucMD5Ret[i]);
			strcat(strClipMD5,cmd5);
			TRACE("%02x", (BYTE)ucMD5Ret[i]);
		}  
		CString tmpmd5 = strClipMD5;
		t = CTime::GetCurrentTime();
		return tmpmd5;

}

BOOL CMD5CALEX::Begin()
{
	m_cMD5.MD5Init( &m_struMD5Ctx );
	return TRUE;	
}

BOOL CMD5CALEX::CalBuff(unsigned char* pBuffIn, int nbuffLenIn)
{
	m_cMD5.MD5Update( &m_struMD5Ctx, pBuffIn, nbuffLenIn);
	return TRUE;
}

