/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	global.cpp
	file base:	global
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
#include "stdafx.h"
#include "global.h"
#include "Log.h"

CFont					g_fontBase;
Log						g_log;
LOGFONT					g_logfont;
// CDlgDebugLog*			g_pdlgDebugLog = NULL;
BOOL					g_bStopStun = FALSE;
HD_GLOBAL_INFO g_globalInfo;

CString AfxSplitPathName(LPCTSTR lpszPath, int nSplit)
{
	CString strResult;

	char szPath[800],szDrive[800],szFileName[800],szExt[800];
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

CString AfxGetAppPath()
{
	char szTemp[MAX_PATH];
	GetModuleFileName(NULL,szTemp,sizeof(szTemp));
	return AfxSplitPathName(szTemp,DRIVE|PATH);
}


BOOL GetVersionIsHigherThanXp()
{

	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	//
	// If that fails, try using the OSVERSIONINFO structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return FALSE;
	}
	if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 6) 
	{
		return TRUE;
	}
	return FALSE;
}



CString GetFullFilePath(const CString& strFile )
{
	CString strTmpFileOut = AfxGetAppPath();
	//strTmpFileOut.TrimRight(TEXT("\\"));
	strTmpFileOut += strFile;
	return strTmpFileOut;
}

CString GetFullFilePathOCX( const CString& strFile )
{
	CString strTmpFileOut = AfxGetOcxPath();
	//strTmpFileOut.TrimRight(TEXT("\\"));
	strTmpFileOut += strFile;
	return strTmpFileOut;
}

CString AfxGetOcxPath()
{
	CWinApp* pApp = AfxGetApp();
	if(pApp && pApp->m_pszHelpFilePath != NULL)
	{
		return AfxSplitPathName(AfxGetApp()->m_pszHelpFilePath,0x0001|0x0002);
	}
	else
	{
		return NULL;
	}

}

CRect GetCenterRect( CRect rc1, CRect rc2 )
{
	CRect rcRet;
	int nOffX = (rc1.Width() - rc2.Width()) /2;
	rcRet.left = rc1.left + nOffX;
	rcRet.right = rcRet.left + rc2.Width();
	int nOffY = (rc1.Height() - rc2.Height())/2;
	rcRet.top = rc1.top + nOffY;
	rcRet.bottom = rcRet.top + rc2.Height();
	return rcRet;
}

void TxUTModifyState( UINT& nStateInOut, UINT stateRem, UINT stateAdd )
{
	if (stateRem != 0)
	{
		nStateInOut &=  ~stateRem;
	}

	if (stateAdd != 0)
	{
		nStateInOut |= stateAdd;
	}
}

BOOL TxUTHasState( UINT nStateOrg, UINT stateToCheck )
{
	return (nStateOrg & stateToCheck)!= 0;
}


