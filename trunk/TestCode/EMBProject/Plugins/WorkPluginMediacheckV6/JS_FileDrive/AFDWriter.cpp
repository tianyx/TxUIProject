#include "stdafx.h"
#include "AFDWriter.h"
#include "io.h"

HWND g_hNotifier = NULL;

#define BERLENBYTE 8


#ifndef ENSURE
#define ENSURE_THROW(cond)	\
do { int __afx_condVal=!!(cond); ASSERT(__afx_condVal); if (!(__afx_condVal)){::AfxThrowFileException(-100);} } while (false)
#define ENSURE(cond)		ENSURE_THROW(cond)
#endif



void WriteLog( LPCTSTR format,... )
{

	//add line number to format

	va_list ap;
	va_start(ap, format);
	TCHAR line[LINE_BUFFER_SIZE];
	memset(line, 0, sizeof(line));
	vsprintf(line, format, ap);
	_RPT0(0, line);
	_RPT0(0, TEXT("\n"));
	if(g_hNotifier)
	{
		SendMessage(g_hNotifier, MSG_LOGNOTIFY, (WPARAM)line, 0);
	}
	va_end(ap);
}

CAFDWriter::CAFDWriter(void)
{
	m_pFile = NULL;
	CloseMXF();
}


CAFDWriter::~CAFDWriter(void)
{
	if (m_pFile)
	{
		delete m_pFile;
		m_pFile = NULL;
	}
}

int CAFDWriter::GetAFDInfo( byte& bAFDOut, INT64& nSeePosOut)
{
	if (m_pFile == NULL)
	{
		return AWERR_FILEOPEN;
	}

	UINT nRead = 0;
#ifdef _DEBUG
	byte sbKey[16];
	nRead = m_pFile->Read(sbKey, 16);
	ENSURE(nRead == 16);
#else
	m_pFile->Seek(16,CFile64::begin);
#endif // _DEBUG
	//get partition pack len
	byte bLen;
	nRead = m_pFile->Read(&bLen, 1);
	ENSURE(nRead == 1);
	INT64 nPPackLen = bLen;
	if (bLen > 0x80)
	{
		nPPackLen = ConvertBERLen(bLen - 0x80);
		ENSURE(nPPackLen > 0);
	}

	int nOffRead = 32;
	m_pFile->Seek(32, CFile64::current);
	//read file metadata len
	UINT64 nFileMetaLen = 0; //
	nFileMetaLen = ConvertBERLen(8);
	ENSURE(nFileMetaLen >=0);
	nOffRead += 8;
	if (nFileMetaLen == 0)
	{
		return AWERR_NOFILEMETA;
	}

	m_pFile->Seek(nPPackLen-nOffRead, CFile64::current);
	
	//reach file metadata
	//jump primer pack
#ifdef _DEBUG
	nRead = m_pFile->Read(sbKey, 16);
	ENSURE(nRead == 16);
	INT64 nDbgCurrPos = m_pFile->GetPosition();
#else
	m_pFile->Seek(16,CFile64::begin);
#endif // _DEBUG
	nFileMetaLen -=16;
	nRead = m_pFile->Read(&bLen, 1);
	ENSURE(nRead == 1);
	INT64 nPrimLen =bLen;
	if (bLen >0x80)
	{
		nPrimLen = ConvertBERLen(bLen - 0x80);
		ENSURE(nPrimLen >0);
		nFileMetaLen -= bLen - 0x80;
	}
	else
	{
		nFileMetaLen -=1;
	}
	m_pFile->Seek(nPrimLen, CFile64::current);
	//reach meta preset
	nFileMetaLen -= nPrimLen;
	INT64 nMetasetReaded = 0;
	while(nMetasetReaded < nFileMetaLen)
	{
		//read structural key
		byte bszstKey[16];
		nRead = m_pFile->Read(bszstKey, 16);
#ifdef _DEBUG
		nDbgCurrPos = m_pFile->GetPosition();
#endif // _DEBUG
		ENSURE(nRead == 16);
		nMetasetReaded +=16;
		byte bMetaL = 0;
		nRead = m_pFile->Read(&bMetaL, 1);
		ENSURE(nRead == 1);
		nMetasetReaded +=1;
		INT64 nMetaLen = bMetaL;
		if (bMetaL > 0x80)
		{
			nMetaLen = ConvertBERLen(bMetaL - 0x80);
			ENSURE(nMetaLen > 0);
			nMetasetReaded +=bMetaL - 0x80;
		}
		//check key
		if (bszstKey[4]== 0x02 && bszstKey[10]== 0x01
			&&bszstKey[12]==0x01 && bszstKey[13] == 0x01 && (bszstKey[14]== 0x27 || bszstKey[14] ==0x51))
		{
			//find generic picture essence descriptor set
			unsigned long nPos = m_pFile->GetPosition();
			//start load 
			int nPicSetLastPos = nPos + nMetaLen;

			while(m_pFile->GetPosition() < nPicSetLastPos)
			{
				//get key
				byte picTag[2];
				nRead = m_pFile->Read(picTag, 2);
#ifdef _DEBUG
				nDbgCurrPos = m_pFile->GetPosition();
#endif // _DEBUG
				short nTagLen = 0;
				nTagLen = ConvertBERLen(2);
				if (picTag[0] == 0x32 && picTag[1] == 0x18)
				{
					//found it
					ASSERT(nTagLen == 1);
					nSeePosOut = m_pFile->GetPosition();
					nRead = m_pFile->Read(&bAFDOut, 1);
					return AWERR_SUCCESS;
				}
				m_pFile->Seek(nTagLen, CFile64::current);
			}
			
			return AWERR_NOTFINDAFD;
			
		}
		else
		{
			m_pFile->Seek(nMetaLen, CFile64::current);
			nMetasetReaded += nMetaLen;

		}
	}

	return AWERR_NOTFINDAFD;
}

int CAFDWriter::GetAFDInfo( CString& strFileIn, byte& bAFDOut, INT64& nSeekPosOut )
{
	int nRet = OpenMXF(strFileIn);
	if (nRet != AWERR_SUCCESS)
	{
		return nRet;
	}

	nRet = GetAFDInfo(bAFDOut, nSeekPosOut);

	CloseMXF();

	return nRet;

}

int CAFDWriter::OpenMXF( CString& strSrcFile )
{
	CloseMXF();
	BOOL nRet = FALSE;
	m_pFile = new CFile64;
	if (m_pFile)
	{
		nRet = m_pFile->Open(strSrcFile, CFile64::modeRead|CFile64::shareDenyNone);
		if (!nRet)
		{
			delete m_pFile;
			m_pFile = NULL;
		}
		else
		{
			m_nFileLen = m_pFile->GetLength();

		}
	}

	return nRet? AWERR_SUCCESS:AWERR_FILEOPEN;
}

void CAFDWriter::CloseMXF()
{
	if (m_pFile)
	{
		m_pFile->Close();
		delete m_pFile;
		m_pFile = NULL;
		m_nFileLen = 0;
	}
}

INT64 CAFDWriter::ConvertBERLen(const int nDataLen )
{
	byte berLen[BERLENBYTE];
	ZeroMemory(berLen, BERLENBYTE);
	if(nDataLen > BERLENBYTE)
	{
		WriteLog(TEXT("BER长度溢出！"));
		return -1;
	}

	int nRead = m_pFile->Read(berLen, nDataLen);
	ASSERT(nRead == nDataLen);

	INT64 nRetLen = 0;
	for (int i = 0; i < nDataLen; i++)
	{
		nRetLen += berLen[i] <<(8*(nDataLen-i-1));
	}

	return nRetLen;
}

int CAFDWriter::WriteAFD( CString& strSrcFileIn, CString& strDesFileIn, byte bAFDIn, const INT64 nSrcAFDPosIn )
{
	if (_access(strSrcFileIn, 04) == -1)
	{
		WriteLog(TEXT("错误：源文件无法访问."));
		return AWERR_FILEOPEN;
	}
	
	if (strSrcFileIn.CompareNoCase(strDesFileIn) != 0)
	{
		//copy file
		if (!CopyFile(strSrcFileIn, strDesFileIn, FALSE))
		{
			WriteLog(TEXT("文件拷贝失败，请检查目标文件路径"));
			return AWERR_FILECOPY;
		}
	}

	CFile64 desFile;
	if (!desFile.Open(strDesFileIn, CFile64::modeReadWrite, NULL))
	{
		WriteLog(TEXT("目标文件打开失败！"));
		return AWERR_FILEOPEN;
	}

	int nRet = AWERR_SUCCESS;
	try
	{
		desFile.Seek(nSrcAFDPosIn, CFile64::begin);
		desFile.Write((void*)(&bAFDIn), 1);
		WriteLog(TEXT("AFD信息写入成功"));
	}
	catch (CFileException* e)
	{
		char szBuff[512];
		e->GetErrorMessage(szBuff, 510);
		WriteLog(TEXT("AFD信息写入失败, err = %s"),szBuff);
		nRet = AWERR_FILEWRITE;
	}
	
	desFile.Flush();
	desFile.Close();
	return nRet;
}


