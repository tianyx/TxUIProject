// BuildHelper_imp.cpp: implementation of the CBuildVersionHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "BaseDefine.h"
#include "BaseTool.h"
#include "BuildHelper.h"
#include "FileTool.h"
#include <atlconv.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment( lib, "Version.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuildVersionHelper::CBuildVersionHelper()
{
	USES_CONVERSION;
	CStringTool strTool;
	CString strBuildTime = A2T(__DATE__);
	CStringArray strArray;
	
	strTool.SplitString2Array( _T(" "), strBuildTime, strArray );
	if ( strArray.GetSize() >= 3 )
	{
		m_strBuildTime_Month = strArray[0];
		m_strBuildTime_Day = strArray[1];
		m_strBuildTime_Year = strArray[2];
	}

	TCHAR* lpszMonth[] = { _T("Jan"), _T("Feb"), _T("Mar"), _T("Apr"), _T("May"), _T("June"), _T("July"), _T("Aug"), 
	_T("Sept"), _T("Oct"), _T("Nov"), _T("Dec")};

	for ( int i = 0; i < sizeof(lpszMonth) / sizeof( lpszMonth[0]); i++)
	{
		if ( m_strBuildTime_Month.CompareNoCase( lpszMonth[i]) == 0 )
		{
			m_strBuildTime_Month.Format( _T("%02d"), i + 1 );
		}

	}

	strArray.RemoveAll();
	
	strBuildTime = __TIME__;
	strTool.SplitString2Array( _T(":"), strBuildTime, strArray );

	if ( strArray.GetSize() >= 3 )
	{
		m_strBuildTime_Hour = strArray[0];
		m_strBuildTime_Minute = strArray[1];
		m_strBuildTime_Second = strArray[2];
	}
	//	
	// 得到文件版本信息
	
	DWORD dwSize = GetFileVersionInfoSize( CFilePath().GetFullPathName(), NULL );
	byte* pBlock = new byte[dwSize];
	GetFileVersionInfo( CFilePath().GetFullPathName(), NULL, dwSize, pBlock );

	byte* pVerValue = NULL;
	
	UINT nSize = 0;
	
	VerQueryValue( pBlock, _T("\\VarFileInfo\\Translation"),
		(LPVOID*)&pVerValue, &nSize );

	CString strSubBlock;
	CString strTranslation;
	CString strTemp;
	strTemp.Format( _T("000%x"), *((unsigned short int *)pVerValue));
	strTranslation = strTemp.Right(4);
	strTemp.Format( _T("000%x"), *((unsigned short int *)&pVerValue[2]) );
	
	strTranslation += strTemp.Right(4);

	//080404b0为中文，040904E4为英文

	LPCTSTR lpszVersionInfo[] = {  _T("Comments"), _T("InternalName"), _T("ProductName"),
								   _T("CompanyName"), _T("LegalCopyright"), _T("ProductVersion"), 
								   _T("FileDescription"), _T("LegalTrademarks"), _T("PrivateBuild"),
								   _T("FileVersion"), _T("OriginalFilename"), _T("SpecialBuild ") };

	CString* pString[] = { &m_strComments, &m_strInternalName, &m_strProductName,
						   &m_strCompanyName, &m_strLegalCopyright, &m_strProductVersion,
						   &m_strFileDescription, &m_strLegalTradeMarks, &m_strPrivateBuild,
						   &m_strFileVersion, &m_strOriginalFileName, &m_strSpecialBuild };

	for ( int i = 0; i < sizeof(lpszVersionInfo) / sizeof(LPCTSTR); i++ )
	{
		strSubBlock = _T("\\StringFileInfo\\") + strTranslation + CString(_T("\\")) + (lpszVersionInfo[i]); 
		BOOL bQuery = VerQueryValue( pBlock, strSubBlock.GetBufferSetLength(256),(LPVOID*)&pVerValue, &nSize );
		strSubBlock.ReleaseBuffer();
		*( pString[i]) = bQuery ? (TCHAR*)pVerValue : _T("");
	}

	delete []pBlock;
	//
}

CBuildVersionHelper::~CBuildVersionHelper()
{

}

CString CBuildVersionHelper::Format(const CString &strFormat)
{
	CString str = strFormat;
	str.Replace( _T("%y"), m_strBuildTime_Year);
	str.Replace( _T("%m"), m_strBuildTime_Month);
	str.Replace( _T("%d"), m_strBuildTime_Day);
	str.Replace( _T("%H"), m_strBuildTime_Hour);
	str.Replace( _T("%M"), m_strBuildTime_Minute);
	str.Replace( _T("%S"), m_strBuildTime_Second);

	return str;
}
