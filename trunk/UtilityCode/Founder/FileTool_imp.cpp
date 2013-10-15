// FileTool_imp.cpp: implementation of the CFileTool class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include <stack>
#include "BaseTool.h"
#include "FileTool.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilePath::CFilePath( DWORD nModuleFlag )
{
#pragma warning( disable : 4996 )

//#ifndef _WINDLL 
//#ifdef UNICODE
//	SetFullPathName( _wpgmptr );
//#else
//	SetFullPathName( _pgmptr );
//#endif // !UNICODE	
//#else

	static HMODULE hModule = NULL;
	if ( hModule == NULL )
	{
		BOOL b =  GetModuleHandleEx(
			GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
			(LPCTSTR)(&hModule),
			&hModule
			);
	}
	CString strTemp;
	GetModuleFileName( (nModuleFlag == CURRENT_MODULE ? hModule : NULL ), strTemp.GetBuffer( MAX_PATH * sizeof(TCHAR)), MAX_PATH * sizeof(TCHAR) );
	strTemp.ReleaseBuffer();
	SetFullPathName( strTemp );

//#endif
#pragma warning( default : 4996 )

}

CFilePath::CFilePath( LPCTSTR lpszPathName)
{
	SetFullPathName( lpszPathName );
}

CFilePath::~CFilePath()
{

}

CString CFilePath::GetFilePath()
{
	return m_strFilePath;
}

CString CFilePath::GetFileName()
{
	return m_strFileName;
}

CString CFilePath::GetFullPathName()
{
	return m_strFullPathName;
}

BOOL CFilePath::SetFullPathName(LPCTSTR lpszFullPathName)
{
	using namespace std;
	const BOOL FROM_STRING_END(TRUE);
	CStringTool strTool;
	CString strFullPathName;
	stack<CString> stringStack;
		
	strFullPathName = lpszFullPathName;
	int nPos = -1;

	while( (nPos = strFullPathName.FindOneOf( _T("/\\") ) ) != -1 )
	{
		CString strTemp = strFullPathName.Left( nPos + 1 );
		strFullPathName = strFullPathName.Right( strFullPathName.GetLength() - nPos - 1 );

		if ( strTemp == _T("..\\") || strTemp == _T("../") )
		{
			if ( stringStack.size() > 1 )
			{
				stringStack.pop();
			}
		}
		else
		{
			stringStack.push( strTemp );
		}
		
	}
	// 
	while ( stringStack.size() > 0 )
	{
		strFullPathName = stringStack.top() + strFullPathName;
		stringStack.pop();
	}

	//
	m_strFullPathName = strFullPathName;
	strTool.SplitString( _T("/\\"), m_strFullPathName, m_strFilePath, m_strFileName, FROM_STRING_END);	
	
	BOOL bValid = IsPathNameValid();

	if ( !bValid )
	{
		m_strFullPathName.Empty();
	}

	return bValid;
}

BOOL CFilePath::SetRelativePathName(LPCTSTR lpszRelativePathName)
{
	CString strRelativePathName = lpszRelativePathName;
	strRelativePathName.TrimLeft( _T("/\\") );

	SetFullPathName( m_strFilePath + _T("\\") + strRelativePathName );
	return IsPathNameValid();
}

CString CFilePath::GetRelativePath(LPCTSTR lpszRoot)
{
	const BOOL NOT_SENSITIVE(FALSE);
	CString strRoot;
	CString strFullPathName;
	CString strRelativePath;
	CStringTool strTool;
	
	strRoot = ( lpszRoot == NULL ) ? m_strFilePath : lpszRoot;
	strFullPathName = m_strFullPathName;
	strRoot.Replace( _T('/'), _T('\\'));
	strFullPathName.Replace( _T("/"), _T("\\") );
	

	if ( strTool.IsPrefix( strRoot, strFullPathName, NOT_SENSITIVE ) )
	{		
		strRelativePath = m_strFullPathName.Right( m_strFullPathName.GetLength() - strRoot.GetLength() );
		strRelativePath.TrimLeft(_T("/"));
		strRelativePath.TrimLeft(_T("\\"));
		strRelativePath.TrimRight(_T("/"));
		strRelativePath.TrimRight(_T("\\"));
		return strRelativePath;
	}

	return _T("");
}

CString CFilePath::GetFileExt()
{
	CString strFileName, strFileExt;
	const BOOL FROM_STRING_END(TRUE);
	CStringTool strTool;
		
	strTool.SplitString( _T("."), m_strFileName, strFileName, strFileExt, FROM_STRING_END);
	return strFileExt;

}

CString CFilePath::GetFileNameWithoutExt()
{
	CString strFileName, strFileExt;
	const BOOL FROM_STRING_END(TRUE);
	CStringTool strTool;
	
	strTool.SplitString( _T("."), m_strFileName, strFileName, strFileExt, FROM_STRING_END);
	return strFileName;
}

BOOL CFilePath::IsAbsolutePath(const CString& strPath)
{
	BOOL bAbsolute = FALSE;
	if ( strPath.GetLength() >= 2 )
	{
		bAbsolute = ( _istalpha(strPath.GetAt(0)) && strPath.GetAt(1) == ':' );
	}

	return bAbsolute;

}

BOOL CFilePath::IsPathNameValid( const CString& strFileFullPath )
{
	CFilePath filePath;
	filePath.SetFullPathName( strFileFullPath );
	return filePath.IsPathNameValid();
}

BOOL CFilePath::IsPathNameValid()
{
	const BOOL PATHNAME_INVALID(FALSE);
	const BOOL PATHNAME_VALID(TRUE);

	CStringArray strArray;
	CStringTool strTool;

	strTool.SplitString2Array( _T("/\\"), m_strFullPathName, strArray );

	if ( strArray.GetSize() <= 0 )
	{
		return PATHNAME_INVALID;
	}

	// 
	CString strRoot = strArray[0];
	if ( strRoot.GetLength() != 2 || !_istalpha(strRoot.GetAt(0)) || strRoot.GetAt(1) != _T(':') )
	{
		return PATHNAME_INVALID;
	}

	for ( int i = 1; i < strArray.GetSize(); i++ )
	{
		if ( strArray[i].FindOneOf(_T(":*?\"<>|")) != -1 )
		{
			return PATHNAME_INVALID; 
		}
		if ( strArray[i].IsEmpty() && i != strArray.GetSize() - 1 )
		{
			return PATHNAME_INVALID; 
		}

	}

	return PATHNAME_VALID; 
}

BOOL CFilePath::IsExist()
{
	const BOOL FILEPATH_NOT_EXIST(FALSE);
	const BOOL FILEPATH_EXIST(TRUE);
	
	return ( GetFileAttributes( m_strFullPathName ) == 0xFFFFFFFF ) ? FILEPATH_NOT_EXIST : FILEPATH_EXIST;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HRESULT CFileSystemTool::GetFiles(const CString& strWildcard, CStringArray& strArrayFiles, DWORD dwFlag)
{
	HRESULT hr = E_FAIL;
	CStringTool stringTool;
	CString strRoot;
	CString str = strWildcard;			
	CString str1;
	CString str2;
	CString strSubWildcard;
	CString strWildcardTemp;
	CString strWhiteSpace;
	CString strWildcardNew;
	const BOOL SPLIT_FROM_STRING_END(TRUE);
	CStringArray strRootArray;

	hr = ( stringTool.SplitString( _T("/\\"), strWildcard, str1, str2, strWhiteSpace, SPLIT_FROM_STRING_END)) ;

	if ( SUCCEEDED(hr))
	{
		strRootArray.Add( str1 );
		strWildcardTemp = str2;
	}
	else
	{
		GetDiskPartition( strRootArray );
		strWildcardTemp = strWildcard;
	}
	
	for ( int k = 0; k < strRootArray.GetSize(); k++ )
	{
		CFileFind finder;
		strWildcardNew = strRootArray[k] + _T("\\") + strWildcardTemp;
		// build a string with wildcards
		BOOL bWorking = finder.FindFile(strWildcardNew);
		
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			
			// skip . and .. files; otherwise, we'd
			// recur infinitely!
			
			if (finder.IsDirectory())
			{
				continue;
			}
			
			strArrayFiles.Add( finder.GetFilePath() );
			// if it's a directory, recursively search it
			
			
		}
		
		finder.Close();
		
		if ( (dwFlag & SEARCH_SUB_FOLDER) != 0 )
		{
			CStringArray strArrayFolders;			
			
			hr = ( stringTool.SplitString( _T("/\\"), strWildcardNew, str1, str2, strWhiteSpace, SPLIT_FROM_STRING_END)) ;

			strRoot =  ( SUCCEEDED(hr) ? str1 : _T(""));
			
			GetFolders( strRoot, strArrayFolders);
			
			for ( int i = 0; i < strArrayFolders.GetSize(); i++)
			{
				strSubWildcard = ( SUCCEEDED(hr) ? (strArrayFolders[i] + _T("\\") + str2) : (strArrayFolders[i] + _T("\\") + strWildcardNew));
				GetFiles( strSubWildcard, strArrayFiles, dwFlag);
				
			}		
		}
	}

	return S_OK;
}

HRESULT CFileSystemTool::GetFolders( const CString& strRoot, CStringArray& strArrayFolders, DWORD dwFlag)
{	
	// build a string with wildcards
	CStringArray strRootArray;
	CString strRootPath(strRoot);
	CString strWildcard;
	strRootPath.TrimRight( _T('/'));
	strRootPath.TrimRight( _T('\\'));

	strRootArray.RemoveAll();

	if ( strRootPath.IsEmpty() )
	{
		GetDiskPartition( strRootArray );
		if ( dwFlag == DONOT_SEARCH_SUB_FOLDER )
		{
			return S_OK;
		}
	}
	else
	{		
		strRootArray.Add(strRootPath) ;
	}	
	
	for ( int i = 0; i < strRootArray.GetSize(); i++ )
	{		
		CFileFind finder;

		// start working for files
		strWildcard = strRootArray[i] + _T("\\*.*") ;
		BOOL bWorking = finder.FindFile( strWildcard );
		
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			
			// skip . and .. files; otherwise, we'd
			// recur infinitely!
			
			if (finder.IsDots())
				continue;
			
			// if it's a directory, recursively search it
			
			if (finder.IsDirectory())
			{
				CString str = finder.GetFilePath();
				strArrayFolders.Add( str );
				
				if ( (dwFlag & SEARCH_SUB_FOLDER) != 0 )
				{
					GetFolders( str, strArrayFolders, dwFlag );
				}
			}
		}
		
		finder.Close();
	}

	return S_OK;

}

HRESULT	CFileSystemTool::SelectFolder(CString& strFolder)
{
	REGISTER_FUNC( CFileSystemTool::SelectFolder(CString& strFolder) );

	HRESULT hr = E_FAIL;
	strFolder.Empty();

	try
	{
		BROWSEINFO bi;
		TCHAR pBuffer[MAX_PATH];
		
		bi.hwndOwner = NULL;
		bi.pidlRoot = NULL;
		bi.pszDisplayName = pBuffer;
		bi.lpszTitle = _T("选择路径");
		bi.ulFlags = 0;
		bi.lpfn = NULL;
		bi.iImage = NULL ;
		
		LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);

		VERIFY_TRUE( pIDList != NULL );
		
		SHGetPathFromIDList(pIDList, pBuffer);
			
		strFolder = pBuffer;
		
		LPMALLOC lpMalloc = NULL;
		if( SUCCEEDED(SHGetMalloc(&lpMalloc)) )
		{
			//释放内存
			lpMalloc->Free(pIDList);
			lpMalloc->Release();
		}
		

		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT CFileSystemTool::GetDiskPartition(CStringArray &strPartitionArray, DWORD dwFlag)
{
	for ( int i = 0; i < 26; i++ )
	{
		CString strRoot;
		strRoot.Format( _T("%c:\\"), _T('a') + i )	;

		DWORD dwAttr = GetFileAttributes( strRoot );   
		if ( dwAttr != 0xFFFFFFFF && ( dwAttr & FILE_ATTRIBUTE_DIRECTORY) != 0 )   
		{
			UINT nDriveType = GetDriveType( strRoot );
			if ( nDriveType == DRIVE_REMOVABLE || nDriveType == DRIVE_FIXED /* || nDriveType == DRIVE_REMOTE */)
			{
				strRoot.TrimRight( _T("\\") );
				strPartitionArray.Add(strRoot);
			}
			
		}

	}

	return S_OK;
}
