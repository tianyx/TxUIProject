// XmlConfigTool_imp.cpp: implementation of the CXmlConfigTool class.
//
//////////////////////////////////////////////////////////////////////
#include "..\stdafx.h"
#include "XmlConfigTool.h"
#include "FileTool.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#pragma warning(disable:4996)

extern"C" MARRCO_DECLSPEC CXmlConfigMgr* GetGlobalConfigMgr()
{
	static CXmlConfigMgr g_configMgr;

	if ( CFilePath().GetFileExt().CompareNoCase( _T("exe")) == 0 )
	{
		// exe module
		return &g_configMgr;
	}

	// dll module
	static BOOL bInit = FALSE;
	static CXmlConfigMgr *pConfigMgr = NULL;
	if ( !bInit )
	{
#ifdef _UNICODE
		CString strExePath = _wpgmptr;
#else
		CString strExePath = _pgmptr;
#endif
		HINSTANCE hInst = ::LoadLibrary( strExePath );
		if ( hInst )
		{
			typedef CXmlConfigMgr* (*GETGLOBALCONFIGMGR)();
			GETGLOBALCONFIGMGR pfn = (GETGLOBALCONFIGMGR)::GetProcAddress( hInst , "GetGlobalConfigMgr" );
			if ( pfn )
			{
				pConfigMgr = pfn();				
			}
			else
			{
			//	static CXmlConfigMgr configMgr;
				pConfigMgr = &g_configMgr;
			}

			FreeLibrary( hInst );

		}	
		
		bInit = TRUE;
	}

	return pConfigMgr;


}


CXmlConfigTool::~CXmlConfigTool()
{

}

CXmlConfigTool::CXmlConfigTool( const CString& strSection, const CString& strFileName, const CString& strEncoding )
{
	REGISTER_FUNC( CXmlConfigTool::CXmlConfigTool( const CString& strSection, const CString& strFileName ) );
	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection(), TRUE );
		
	try
	{		
		CString _strFileName;
		if ( strFileName.IsEmpty() )
		{
			CFilePath filePath;
			_strFileName = filePath.GetFilePath() + _T("\\") + filePath.GetFileNameWithoutExt() + _T(".xml");
		}
		else
		{
			CFilePath filePath;
			if ( filePath.IsAbsolutePath(strFileName) )
			{
				_strFileName = strFileName;
			}
			else
			{
				WARNING_IF_FALSE( filePath.SetRelativePathName( strFileName ) );
				_strFileName = filePath.GetFullPathName();							
			}
			
		}
		_strFileName.MakeLower();
		
		_tagConfigData configData;
		configData.strFileName = _strFileName;
		configData.strSection = strSection;
		
		int nIndex = -1;
		if ( GetGlobalConfigMgr()->Lookup( configData.strFileName, nIndex ))
		{
			GetGlobalConfigMgr()->GetAt( nIndex, m_configData );
			m_configData.strSection = _T("<") + strSection + _T(">");
		}
		else
		{
			m_configData.strFileName = _strFileName;
			m_configData.strSection = _T("<") + strSection + _T(">");
			if ( FAILED(m_configData.xmlTool.LoadFromFile( _strFileName ) ) )
			{
				WARNING_WHEN( FAILED( m_configData.xmlTool.CreateXML( _T("config"), _T("1.0"), strEncoding ) ) );
			}
			GetGlobalConfigMgr()->Add( m_configData );
		}
		
		WARNING_WHEN( !CFilePath::IsPathNameValid( m_configData.strFileName ) );

		WARNING_WHEN( FAILED( m_configData.xmlTool.SelectDocumentNode( m_configData.xmlTool ) ) );

		WARNING_WHEN( FAILED( m_configData.xmlTool.SelectChildNode( strSection, m_configData.xmlTool ) ) );		
	}
	my_catch;
	
}


CXmlConfigTool CXmlConfigTool::operator[](const CString& strSubSection)
{
	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection() , TRUE );
	CXmlConfigTool configTool;
	configTool.m_configData.strFileName = m_configData.strFileName;
	configTool.m_configData.strSection += m_configData.strSection + _T("<") + strSubSection + _T(">");	
	if ( !m_configData.xmlTool.IsStatusOK() )
	{
		m_configData.xmlTool.SelectDocumentNode( m_configData.xmlTool );
		m_configData.xmlTool.SelectChildNode( m_configData.strSection, m_configData.xmlTool);
	}
	m_configData.xmlTool.SelectChildNode( strSubSection, configTool.m_configData.xmlTool );
	return configTool;

}

int CXmlConfigTool::GetAsInt(int nDefaultValue)
{
	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection() , TRUE );
	int nValue = nDefaultValue;	
	m_configData.xmlTool.GetValue( nValue, nDefaultValue );
	return nValue;
}

HRESULT	CXmlConfigTool::SetAsInt(int nValue)
{
	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection() , TRUE );

	if ( !m_configData.xmlTool.IsStatusOK() )
	{	
		m_configData.xmlTool.SelectDocumentNode( m_configData.xmlTool );
		m_configData.xmlTool.SelectChildNode( m_configData.strSection, m_configData.xmlTool, CXmlTool::CREATE_IF_NOT_EXISTING);		
	}
	
	return m_configData.xmlTool.SetValue( nValue );	
}

bool CXmlConfigTool::GetAsBool( bool defaultBoolValue )
{
	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection() , TRUE );
	bool bValue = defaultBoolValue;
	m_configData.xmlTool.GetValue( bValue, defaultBoolValue );
	return bValue;
}

HRESULT	CXmlConfigTool::SetAsBool( bool bBoolValue )
{
	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection() , TRUE );
	
	if ( !m_configData.xmlTool.IsStatusOK() )
	{	
		m_configData.xmlTool.SelectDocumentNode( m_configData.xmlTool );
		m_configData.xmlTool.SelectChildNode( m_configData.strSection, m_configData.xmlTool, CXmlTool::CREATE_IF_NOT_EXISTING);		
	}
	
	return m_configData.xmlTool.SetValue( bBoolValue );	

}

double CXmlConfigTool::GetAsDouble(double dDefaultValue)
{
	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection() , TRUE );
	double dValue;
	m_configData.xmlTool.GetValue( dValue, dDefaultValue );
	return dValue;
}

HRESULT CXmlConfigTool::SetAsDouble(double dValue)
{
	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection() , TRUE );
	if ( !m_configData.xmlTool.IsStatusOK() )
	{	
		m_configData.xmlTool.SelectDocumentNode( m_configData.xmlTool );
		m_configData.xmlTool.SelectChildNode( m_configData.strSection, m_configData.xmlTool, CXmlTool::CREATE_IF_NOT_EXISTING);		
	}
	return m_configData.xmlTool.SetValue( dValue )	;
}

CTime CXmlConfigTool::GetAsTime(CTime timeDefault)
{
	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection() , TRUE );
	CTime time;
	m_configData.xmlTool.GetValue( time, timeDefault );
	return time;

}

HRESULT CXmlConfigTool::SetAsTime(CTime timeValue)
{
	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection() , TRUE );
	if ( !m_configData.xmlTool.IsStatusOK() )
	{	
		m_configData.xmlTool.SelectDocumentNode( m_configData.xmlTool );
		m_configData.xmlTool.SelectChildNode( m_configData.strSection, m_configData.xmlTool, CXmlTool::CREATE_IF_NOT_EXISTING);		
	}
	return m_configData.xmlTool.SetValue( timeValue );
}

CString	CXmlConfigTool::GetAsString( const CString& strDefault)
{
	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection() , TRUE );
	CString strValue;
	m_configData.xmlTool.GetValue( strValue, strDefault );
	return strValue;
}

HRESULT	CXmlConfigTool::SetAsString( const CString& strValue)
{
	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection() , TRUE );
	if ( !m_configData.xmlTool.IsStatusOK() )
	{	
		m_configData.xmlTool.SelectDocumentNode( m_configData.xmlTool );
		m_configData.xmlTool.SelectChildNode( m_configData.strSection, m_configData.xmlTool, CXmlTool::CREATE_IF_NOT_EXISTING);		
	}
	return m_configData.xmlTool.SetValue( strValue );
}

HRESULT	CXmlConfigTool::GetAsStringArray( CStringArray& strArray )
{
	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection() , TRUE );
	CString strValue;
	HRESULT hr = E_FAIL;

	try
	{
		CString strTagName;
		m_configData.xmlTool.GetTagName( strTagName );
		CXmlTool xmlTool = m_configData.xmlTool;

		do
		{
			if ( SUCCEEDED( xmlTool.GetValue( strValue ) ) )
			{
				hr = S_OK;
				strArray.Add( strValue );
			}

		}while( SUCCEEDED( xmlTool.MoveToNextSibling(strTagName)) );
		

	}
	my_catch;
	
	return hr;

}

HRESULT	CXmlConfigTool::GetXmlTool( CXmlTool& xmlTool )
{
	if ( !m_configData.xmlTool.IsStatusOK() )
	{
		m_configData.xmlTool.SelectDocumentNode( m_configData.xmlTool );
		m_configData.xmlTool.SelectChildNode( m_configData.strSection, m_configData.xmlTool, CXmlTool::CREATE_IF_NOT_EXISTING);		
	}
	
	xmlTool = m_configData.xmlTool;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
CXmlConfigMgr::CXmlConfigMgr()
{	
}

CXmlConfigMgr::~CXmlConfigMgr()
{
	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection(), TRUE );
	
	for ( int i = 0; i < GetGlobalConfigMgr()->GetDataArray()->GetSize(); i++ )
	{
		GetGlobalConfigMgr()->GetDataArray()->GetAt(i).xmlTool.SaveToFile( GetGlobalConfigMgr()->GetDataArray()->GetAt(i).strFileName );

	}

	GetGlobalConfigMgr()->RemoveAll();
		
}

BOOL CXmlConfigMgr::Lookup(const CString& strConfigFilePath, int& nIndex )
{
	BOOL bFound = FALSE;
	nIndex = -1;

	CSingleLock smartLock( GetGlobalConfigMgr()->GetCriticalSection(), TRUE );

	for ( int i = 0; i < GetGlobalConfigMgr()->GetDataArray()->GetSize(); i++ )
	{
		CXmlConfigTool::_tagConfigData& _data = GetGlobalConfigMgr()->GetDataArray()->GetAt(i);
		if ( _data.strFileName.CompareNoCase( strConfigFilePath ) == 0 )
		{
			nIndex = i;
			bFound = TRUE;
			break;
		}
		
	}
	
	return bFound;
}
