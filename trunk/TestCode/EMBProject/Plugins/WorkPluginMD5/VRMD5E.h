/* ************************************************************** *
 *	VRMD5E.h
 *  CVRMD5E 的声明
 *
 *  Created on:      2008-03-31 16:25:00
 *
 *	功能：MD5数据加密方法实现.
 *
 * *************************************************************** */

#pragma once
#include "resource.h"       // main symbols

//#include "MD5Encrypt.h"
#include "MD5.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CVRMD5E

class CVRMD5E
{
public:
	CVRMD5E()
	{
		Init();
	}

	~CVRMD5E()
	{
		Resume();
	}

public:
	CNEWMD5 m_cMD5;
	MD5_CTX m_struMD5Ctx; 
	
	DWORD m_dwLength;
	BYTE *m_pBuffer;
	DWORD m_dwBufferSize;
	LONGLONG m_FileLength;
	LONGLONG m_nCurrReadPos;
protected:
	void Init();
	void Resume();

public:
	BOOL MD5Encrypt(CHAR* pcharCtxSrc, LONG lCtxLen, CHAR* pcharMD5CtxDst);
public:
	HRESULT MD5EncryptFile(CHAR* pcharFliePath, CHAR* pcharMD5CtxDst);
	int GetProgress();
public:
	BOOL SetFileBuffer(ULONG ulFileBuffer);
};

