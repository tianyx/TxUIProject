/********************************************************************
created:	2010/02/23
created:	23:2:2010   17:50
filename: 	IMPackRes.cpp
file path:	Common
file base:	IMPackRes
file ext:	cpp
author:		tian_yx

purpose:	
*********************************************************************/
#include "stdafx.h"
#include "IMPackRes.h"
#include "IMSerialize.h"
#include "Log.h"
#include "io.h"

CString SplitAppPathName(LPCTSTR lpszPath, int nSplit)
{
	CString strResult;

	TCHAR szPath[800],szDrive[800],szFileName[800],szExt[800];
	_splitpath(lpszPath,szDrive,szPath,szFileName,szExt);

	if(nSplit & DRIVE)
		strResult += szDrive;
	if(nSplit & PATH)
		strResult += szPath;
	if(nSplit & FNAME)
		strResult += szFileName;
	if(nSplit & EXT)
		strResult += szExt;

	return strResult;
}

CString GetOcxPath()
{
	ASSERT(AfxGetApp()->m_pszHelpFilePath != NULL);
	return SplitAppPathName(AfxGetApp()->m_pszHelpFilePath,0x0001|0x0002);

}


CString GetAppPath()
{
#ifdef USED_IN_OCX_TYPE
	return GetOcxPath();
#else
	TCHAR szTemp[MAX_PATH];
	GetModuleFileName(NULL,szTemp,sizeof(szTemp)/sizeof(TCHAR));
	return SplitAppPathName(szTemp,DRIVE|PATH);

#endif
}





BOOL IsResType(CString& strNameIn, CString& strType)
{
	BOOL bRet = FALSE;
	int nPos = strNameIn.ReverseFind(L'.');
	if (!(nPos < 0))
	{
		strType = strNameIn.Mid(nPos+1);
		strType.MakeUpper();
		if (strType == TEXT("BMP") 
			|| strType == TEXT("GIF") 
			|| strType == TEXT("JPG") 
			||strType == TEXT("DLL")
			)
		{
			bRet = TRUE;
		}
	}
	return bRet;
}

void GetFilesInFolder(CString& strFolder, RESDATA& dataOut, LPCTSTR strPrePathIn, BOOL bSearchInSubFolder = TRUE)
{
	CFileFind finder;
	// build a string with wildcards
	CString strWildcard(strFolder);
	strWildcard.TrimRight(TEXT("\\"));
	strWildcard += TEXT("\\*.*");

	CString strAppPath = GetAppPath();
	strAppPath.TrimRight(TEXT("\\"));

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!
		if (finder.IsDots())
			continue;
		// if it's a directory, recursively search it
		if (finder.IsDirectory() && bSearchInSubFolder)
		{
			CString strPath = finder.GetFilePath();
			
			CString strPreSubPath = strPrePathIn;
			strPreSubPath += TEXT("\\");
			strPreSubPath += finder.GetFileName();
			strPreSubPath += TEXT("\\");
			GetFilesInFolder(strPath, dataOut, strPreSubPath, TRUE);
		}

		CString strName = finder.GetFileName();
		CString strType;
		if (IsResType(strName, strType))
		{
			if (strPrePathIn != NULL)
			{
				strName = strPrePathIn + strName;
			}
			//save to vec
			dataOut.push_back(RESPAIR(strType, strName));
		}
	}

	finder.Close();

}

BOOL GetPackFileList( RESDATA& dataOut )
{
	dataOut.clear();
// 	dataOut.push_back(RESPAIR(TEXT("bmp"), TEXT("Skins\\CommonButton.bmp")));
// 	dataOut.push_back(RESPAIR(TEXT("bmp"), TEXT("Skins\\caption.bmp")));
// 	dataOut.push_back(RESPAIR(TEXT("bmp"), TEXT("Skins\\SysMinBtn.bmp")));
	CString strAppPath = GetAppPath();

	//get files in app path
	GetFilesInFolder(strAppPath, dataOut, NULL, FALSE);

	//get files in \\Skins 
	CString strSkinsFolder = strAppPath + TEXT("skins");
	GetFilesInFolder(strSkinsFolder, dataOut, TEXT("skins\\"),  TRUE);

	//get files in \\language 
	CString strLanFolder = strAppPath + TEXT("language");
	GetFilesInFolder(strLanFolder, dataOut, TEXT("language\\"), TRUE);



	return TRUE;
}

void PackRes()
{

	// TODO: 在此添加控件通知处理程序代码
	CString strAppPath = GetAppPath();
	RESDATA resData;
	GetPackFileList(resData);
	
	CString strResFile = strAppPath + TEXT("ImPackData.bin");

	CFile packfile;
	BOOL bOpen =packfile.Open(strResFile, CFile::modeCreate|CFile::modeWrite);
	if (bOpen)
	{
		//write ver;
		int nVer = 1;
		packfile.Write(&nVer, sizeof(nVer));
		//write file count
		int nFileCount = resData.size();
		packfile.Write(&nFileCount, sizeof(nFileCount));

		BYTE nBuffer[2048];
		for (size_t i = 0; i < resData.size(); i++)
		{
			CFile subFile;
			CString strSubFileName = strAppPath + resData[i].second;
			if (subFile.Open(strSubFileName, CFile::modeRead))
			{
				//write file info
				CIMSerialize arr; //in mode
				arr<< resData[i].second;  //pathname
				long nLength = (long)(subFile.GetLength());
				ASSERT(nLength > 0);
				arr<<nLength;  //file length
				long nBufferSize = 2048;
				arr.WriteBuffer(&nBuffer, nBufferSize);
				packfile.Write(&nBuffer, nBufferSize);
				int nRead = subFile.Read(&nBuffer, 2048);
				while(nRead > 0)
				{
					packfile.Write(nBuffer, nRead);
					nRead = subFile.Read(&nBuffer, 2048);
				}
				subFile.Close();
			}
		}
		packfile.Flush();
		packfile.Close();
	}
}

BOOL ExtractRes()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strSource;
	strSource.Format(TEXT("#%d"), IDR_IMRESDATA);
	BOOL bREs = IS_INTRESOURCE(IDR_IMRESDATA);
	HINSTANCE hIns = AfxFindResourceHandle(MAKEINTRESOURCE(IDR_IMRESDATA),TEXT("IMRES"));

	if (!hIns)
	{
		return FALSE;
	}
	HRSRC hRes =FindResource(hIns, MAKEINTRESOURCE(IDR_IMRESDATA),TEXT("IMRES"));
	if (!hRes)
	{
		return FALSE;
	}
	HGLOBAL hData = LoadResource(hIns, hRes);
	if (!hData)
	{
		return FALSE;
	}
	long dataSize = SizeofResource(hIns, hRes);
	CString strAppPath = GetAppPath();

	//create common file path
	CString strPathSkin = strAppPath + TEXT("skins");
	CreateDirectory(strPathSkin, NULL);
	CString strPathFace = strPathSkin + TEXT("\\faces");
	CreateDirectory(strPathFace, NULL);
	CString strLanPath = strAppPath + TEXT("language");
	CreateDirectory(strLanPath, NULL);

//	g_log.Print(TEXT("\n res hData  = %x"), hData);

	void* ptrRes = 	LockResource(hData);
	CIMSerialize arrOut(ptrRes, dataSize);
//	g_log.Print(TEXT("\n res size  = %d"), dataSize);
//	g_log.Print(TEXT("\n res addr  = %x"), ptrRes);

	int nVer = 0; //get Ver;
	arrOut >> nVer;
//	g_log.Print(TEXT("\n res ver  = %d"), nVer);
	int nFileCount = 0;
	arrOut >> nFileCount;
//	g_log.Print(TEXT("\n res file count = %d"), nFileCount);
	for (int i = 0; i < nFileCount; i++)
	{
		CString strSubFile;
		arrOut>>strSubFile;
		long nFileSize = 0;
		arrOut >> nFileSize;
		CFile subfile;
		CString strSubFileName = strAppPath + strSubFile;
		BOOL bOpen = subfile.Open(strSubFileName, CFile::modeCreate|CFile::modeWrite);
		if (bOpen)
		{
			LONG nWrited = arrOut.WriteToFile(&subfile, nFileSize);
			if (nWrited == 0)
			{
				ASSERT(FALSE);
				subfile.Close();
				break;
			}
			subfile.Flush();
			subfile.Close();
		}
		else
		{
			ASSERT(FALSE);
			break;
		}

	}

	GlobalUnlock(hData);

	//reg dll
	CString strImgViewDll = TEXT("ATLImgViewer.dll");
	CString strImgViewDllFull =strAppPath + TEXT("ATLImgViewer.dll");

	//strImgViewDll = TEXT("regsvr32.exe \"") +strImgViewDll;
	//strImgViewDll += TEXT("\"");
	//strImgViewDll += TEXT(" -s");
	if (_access(strImgViewDllFull,00) != -1)
	{
		//AfxMessageBox(strImgViewDllFull);

		ShellExecute(NULL, NULL, TEXT("regsvr32.exe"), strImgViewDll, strAppPath, SW_HIDE);
	}

	return TRUE;

}
