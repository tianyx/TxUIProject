#include "StdAfx.h"
#include "HttpNetOp.h"
#include "afxinet.h"
#include "ACConfig.h"
CHttpNetOp::CHttpNetOp(void)
{
}


CHttpNetOp::~CHttpNetOp(void)
{
}



BOOL CHttpNetOp::DoGet()
{
	BOOL bSuc = TRUE;
	CInternetSession iConn;
	CHttpConnection* pHpCon = iConn.GetHttpConnection(m_preData.strSvr, INTERNET_INVALID_PORT_NUMBER, m_preData.strUser, m_preData.strPw);
	if (!pHpCon)
	{
		ASSERT(FALSE);
		ACWriteLog(TEXT("create http connection err = %d"), GetLastError());
		bSuc = FALSE;
	}

	CHttpFile* pHpFile = pHpCon->OpenRequest(CHttpConnection::HTTP_VERB_GET, m_preData.strUrl, NULL,1, NULL, TEXT("HTTP/1.1"));
	if (pHpFile)
	{
		bSuc = pHpFile->SendRequest();
		if (!bSuc)
		{
			ASSERT(FALSE);
			ACWriteLog(TEXT("request err =%d"), GetLastError());
		}
	}
	else
	{
		ASSERT(FALSE);
		ACWriteLog(TEXT("open request err = %d"), GetLastError());
		bSuc = FALSE;
	}

	//check status
	if (bSuc)
	{
		DWORD dwStatusCode = 0;
		pHpFile->QueryInfoStatusCode(dwStatusCode);
		if (dwStatusCode != HTTP_STATUS_OK)
		{
			ACWriteLog(TEXT("server return status code err = %d"), dwStatusCode);
			bSuc = FALSE;
		}
	}

	//read file
	if (bSuc)
	{
		CFile flLocal;
		if(!flLocal.Open(m_preData.strFile, CFile::modeWrite|CFile::modeCreate)) 
		{
			ASSERT(FALSE);
			bSuc = FALSE;
			ACWriteLog(TEXT("create file error! file = %s"), m_preData.strFile);
		}
		else
		{
			flLocal.SeekToBegin();
			pHpFile->SeekToBegin();
			BYTE pBuffer[1024];
			int nRead = 0;
			while( (nRead = pHpFile->Read(pBuffer, 1024) )>0)
			{
				flLocal.Write(pBuffer, nRead);
			}
			flLocal.Flush();
			flLocal.Close();
		}

		if (pHpFile)
		{
			pHpFile->Close();
			delete pHpFile;
			pHpFile = NULL;
		}
		if (pHpCon)
		{
			pHpCon->Close();
			delete pHpCon;
			pHpCon = NULL;
		}
	}
	return bSuc;

}
