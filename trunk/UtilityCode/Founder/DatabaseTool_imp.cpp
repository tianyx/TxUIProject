#include "..\stdafx.h"
#include "DatabaseTool.h"
#include "FileTool.h"

#include <sql.h>
#include <sqlext.h>
#pragma comment(lib, "odbc32.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
	
NAMESPACE_MARRCO
#pragma warning( disable : 4996 )
extern"C" MARRCO_DECLSPEC CDbConnectionMgr* GetGlobalDBConnectionMgr()
{
	static CLibraryTool libTool;
	static CDbConnectionMgr s_dbConnectionMgr;	

	if ( CFilePath().GetFileExt().CompareNoCase( _T("exe")) == 0 )
	{
		// exe module
		//	static CXmlConfigMgr configMgr;
		return &s_dbConnectionMgr;
	}

	// dll module
	static BOOL bInit = FALSE;
	static CDbConnectionMgr *pDbConnectionMgr = &s_dbConnectionMgr;
	if ( !bInit )
	{
		libTool.InitOleLibrary();

#ifdef _UNICODE
		CString strExePath = _wpgmptr;
#else
		CString strExePath = _pgmptr;
#endif
		HINSTANCE hInst = ::LoadLibrary( strExePath );
		if ( hInst )
		{
			typedef CDbConnectionMgr* (*GETGLOBALCONNECTIONMGR)();
			GETGLOBALCONNECTIONMGR pfn = (GETGLOBALCONNECTIONMGR)::GetProcAddress( hInst , "GetGlobalDBConnectionMgr" );
			if ( pfn )
			{
				pDbConnectionMgr = pfn();				
			}
			else
			{
				//	static CXmlConfigMgr configMgr;
				pDbConnectionMgr = &s_dbConnectionMgr;
			}
			
			FreeLibrary( hInst );
			
		}	
		
		bInit = TRUE;
	}
	
	return pDbConnectionMgr;
}

CDbConnectionMgr::CDbConnectionMgr()
{
	m_spDefaultConnectionPtr = NULL;
}

BOOL CDbConnectionMgr::Lookup(const CString& strConnectionName, int& nIndex)
{
	BOOL bLookup = FALSE;
	CSingleLock smartLock( GetCriticalSection(), TRUE );

	nIndex = -1;

	for ( int i = 0; i < m_dataArray.GetSize(); i++)
	{				
		if ( m_dataArray[i].strConnectionName == strConnectionName)
		{
			bLookup = TRUE;
			nIndex = i;
			break;			
		}
		
	}

	return bLookup;
	
};

HRESULT	CDbConnectionMgr::SetConnectionInfoByName( CString strConnectionName, tagConnectionInfo& pInfo )
{
	REGISTER_FUNC( HRESULT	CDbConnectionMgr::GetConnectionInfoByName( CString& strConnectionName, tagConnectionInfo*& pInfo ) );
	HRESULT hr = E_FAIL;
	CSingleLock smartLock( GetCriticalSection(), TRUE );

	try
	{
		if ( strConnectionName.IsEmpty() )
		{
			VERIFY_SUCCEEDED( GetDefaultConnectionName( strConnectionName ) );
		}

		VERIFY_TRUE( !strConnectionName.IsEmpty() );
		int nIndex = -1;
		VERIFY_SUCCEEDED( Lookup( strConnectionName, nIndex) );
		VERIFY_TRUE( nIndex >= 0 && nIndex < m_dataArray.GetSize() );
		m_dataArray.SetAt(nIndex, pInfo);	

		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CDbConnectionMgr::GetConnectionInfoByName( CString strConnectionName, tagConnectionInfo& pInfo )
{
	REGISTER_FUNC( HRESULT	CDbConnectionMgr::GetConnectionInfoByName( CString& strConnectionName, tagConnectionInfo*& pInfo ) );
	HRESULT hr = E_FAIL;
	CSingleLock smartLock( GetCriticalSection(), TRUE );

	try
	{
		if ( strConnectionName.IsEmpty() )
		{
			VERIFY_SUCCEEDED( GetDefaultConnectionName( strConnectionName ) );
		}

		VERIFY_TRUE( !strConnectionName.IsEmpty() );
		int nIndex = -1;
		VERIFY_SUCCEEDED( Lookup( strConnectionName, nIndex) );
		VERIFY_TRUE( nIndex >= 0 && nIndex < m_dataArray.GetSize() );
		pInfo = m_dataArray[nIndex];	
		
		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CDbConnectionMgr::GetConnectionPtrByName( CString& strConnectionName, ADODB::_ConnectionPtr& spConnectionPtr )
{
	REGISTER_FUNC( CDbConnectionMgr::GetConnectionByName( CString& strConnectionName, ADODB::_ConnectionPtr& spConnectionPtr ) );
	HRESULT hr = E_FAIL;
	CSingleLock smartLock( GetCriticalSection(), TRUE );

	if ( spConnectionPtr != NULL )
	{
		spConnectionPtr.Release();
	}

	try
	{
		if ( strConnectionName.IsEmpty() )
		{
			VERIFY_SUCCEEDED( GetDefaultConnectionPtr( spConnectionPtr) );
		}
		else
		{
			int nIndex = -1;
			VERIFY_SUCCEEDED( Lookup( strConnectionName, nIndex) );
			VERIFY_TRUE( nIndex >= 0 && nIndex < m_dataArray.GetSize() );
			spConnectionPtr = m_dataArray[nIndex].spConnection;	
			VERIFY_TRUE( spConnectionPtr != NULL );
		}


		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CDbConnectionMgr::GetDefaultConnectionName( CString& strConnectionName )
{
	CSingleLock smartLock( GetCriticalSection(), TRUE );

	HRESULT hr = S_OK;
	strConnectionName.Empty();

	if ( m_strDefaultConnectionName.IsEmpty() )
	{
		if ( m_dataArray.GetSize() > 0 )
		{
			strConnectionName = m_dataArray[0].strConnectionName;
			m_strDefaultConnectionName = m_dataArray[0].strConnectionName;
			hr = S_OK;
		}
		else
		{
			hr = E_FAIL;
		}
	}
	else
	{
		strConnectionName = m_strDefaultConnectionName;
	}	
	
	return hr;
}
	
HRESULT	CDbConnectionMgr::GetDefaultConnectionPtr( ADODB::_ConnectionPtr& spConnectionPtr )
{
	REGISTER_FUNC( CDbConnectionMgr::GetDefaultConnectionPtr( ADODB::_ConnectionPtr& spConnectionPtr ) );

	HRESULT hr = E_FAIL;
	
	if ( spConnectionPtr != NULL )
	{
		spConnectionPtr.Release();
	}

	try
	{
		
			// 得到默认链接的名称
			int nIndex = -1;
			CString strDefaultConnectionName;			
			CSingleLock smartLock( GetCriticalSection(), TRUE );
			VERIFY_SUCCEEDED( GetDefaultConnectionName( strDefaultConnectionName ) );
			VERIFY_SUCCEEDED( GetConnectionPtrByName( strDefaultConnectionName, spConnectionPtr) );
			m_spDefaultConnectionPtr = spConnectionPtr;
			hr = S_OK;				
	}
	my_catch;

	return hr;
}

HRESULT	CDbConnectionMgr::SetDefaultConnectionName( const CString& strConnectionName )
{
	CSingleLock smartLock( GetCriticalSection(), TRUE );

	HRESULT hr = E_FAIL;
	int nIndex = -1;

	if ( SUCCEEDED( Lookup( strConnectionName, nIndex)) )
	{
		m_strDefaultConnectionName = strConnectionName;
		hr = S_OK;
	}
	
	
	return hr;
}
/////////////////////////////////////////////////////////
// implementation of class DBTool
//
CDbConnectionTool::CDbConnectionTool( )
{
	m_spConnection = NULL;	
}

CDbConnectionTool::~CDbConnectionTool()
{
	
}

HRESULT	CDbConnectionTool::GetSqlServers( CStringArray& strSqlsArray )
{
	REGISTER_FUNC( HRESULT	CDbConnectionTool::GetSqlServers( CStringArray& strSqlsArray ) );
	HRESULT hr = E_FAIL;
	int nRet = 0;

	CStringTool stringTool;
	const int MAX_RET_LENGTH = 4824;

	SQLHENV       hSQLEnv = NULL;
	SQLHDBC       hSQLHdbc = NULL;

	strSqlsArray.RemoveAll();

	try
	{
		
		short		  sConnStrOut = 0;		
		
		nRet = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hSQLEnv);
		VERIFY_TRUE( nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO);
		
		
		nRet = SQLSetEnvAttr(hSQLEnv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
		VERIFY_TRUE ( nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO) ;
			
		
		nRet = SQLAllocHandle(SQL_HANDLE_DBC, hSQLEnv, &hSQLHdbc);
			
		VERIFY_TRUE (nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO) ;
				
		CString strConnStrOut;
		
		nRet = SQLBrowseConnect(hSQLHdbc, (SQLTCHAR *)_T("Driver={SQL Server}"), SQL_NTS,
						(SQLTCHAR *)( strConnStrOut.GetBuffer(MAX_RET_LENGTH)), MAX_RET_LENGTH, &sConnStrOut);
		strConnStrOut.ReleaseBuffer();		

		CString strSqlServers;
		VERIFY_SUCCEEDED( stringTool.GetBetween( strConnStrOut, _T("{"), _T("}"), strSqlServers) );
		VERIFY_SUCCEEDED( stringTool.SplitString2Array( _T(","), strSqlServers, strSqlsArray));

		hr = S_OK;
	}
	my_catch;

	if ( hSQLHdbc )
	{
		SQLDisconnect(hSQLHdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, hSQLHdbc);
	}
	
	if ( hSQLEnv )
	{
		SQLFreeHandle(SQL_HANDLE_ENV, hSQLEnv);
	}
	
	return hr;

}

HRESULT	CDbConnectionTool::GetDataBases( CStringArray& strDataBaseArray, LPCTSTR lpszServerName, LPCTSTR lpszUser, LPCTSTR lpszPsw ) 
{
	REGISTER_FUNC( HRESULT	CDbConnectionTool::GetDataBases( CStringArray& strDataBaseArray, LPCTSTR lpszServerName, LPCTSTR lpszUser, LPCTSTR lpszPsw )  );
	HRESULT hr = E_FAIL;
	int nRet = 0;

	CStringTool stringTool;
	const int MAX_RET_LENGTH = 4824;

	SQLHENV       hSQLEnv = NULL;
	SQLHDBC       hSQLHdbc = NULL;

	CString strSvrName = lpszServerName;
	CString strUser = lpszUser;
	CString strPsw = lpszPsw;

	strDataBaseArray.RemoveAll();

	try
	{
		if ( strSvrName.IsEmpty() )
		{
			VERIFY_TRUE( IsConnected() );
			strSvrName = m_strServerName;
			strUser = m_strUser;
			strPsw = m_strPsw;
		}
		else
		{
			VERIFY_TRUE( !strUser.IsEmpty() );
		}

		short		  sConnStrOut = 0;		

		nRet = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hSQLEnv);
		VERIFY_TRUE( nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO);


		nRet = SQLSetEnvAttr(hSQLEnv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);
		VERIFY_TRUE ( nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO) ;


		nRet = SQLAllocHandle(SQL_HANDLE_DBC, hSQLEnv, &hSQLHdbc);

		VERIFY_TRUE (nRet == SQL_SUCCESS || nRet == SQL_SUCCESS_WITH_INFO) ;

		CString strConnStrOut;
		CString szInputParam;
		szInputParam.Format( _T("Driver={SQL Server};SERVER=%s;UID=%s;PWD=%s"), strSvrName, strUser, strPsw);

		nRet = SQLBrowseConnect(hSQLHdbc, (SQLTCHAR *)(LPCTSTR)szInputParam, SQL_NTS,
			(SQLTCHAR *)( strConnStrOut.GetBuffer(MAX_RET_LENGTH)), MAX_RET_LENGTH, &sConnStrOut);
		strConnStrOut.ReleaseBuffer();		

		VERIFY_TRUE( stringTool.IsPrefix( _T("*DATABASE"), strConnStrOut));

		CString strDataBase;
		VERIFY_SUCCEEDED( stringTool.GetBetween( strConnStrOut, _T("{"), _T("}"), strDataBase) );
		VERIFY_SUCCEEDED( stringTool.SplitString2Array( _T(","), strDataBase, strDataBaseArray));

		hr = S_OK;
	}
	my_catch;

	if ( hSQLHdbc )
	{
		SQLDisconnect(hSQLHdbc);
		SQLFreeHandle(SQL_HANDLE_DBC, hSQLHdbc);
	}

	if ( hSQLEnv )
	{
		SQLFreeHandle(SQL_HANDLE_ENV, hSQLEnv);
	}

	return hr;

}

HRESULT	CDbConnectionTool::TestConnectSqlServer( const CString& strComputerName, const CString& strUser, const CString& strPsw)
{
	REGISTER_FUNC( HRESULT	CDbConnectionTool::TestConnectSqlServer( const CString& strComputerName, const CString& strUser, const CString& strPsw) );
	HRESULT  hr = E_FAIL;
	CString strConnectionString;
	
	try
	{
		ADODB::_ConnectionPtr spConnection;
		VERIFY_SUCCEEDED( spConnection.CreateInstance( __uuidof( ADODB::Connection) ) );


		strConnectionString.Format( _T("Provider=SQLOLEDB; Data Source=%s; User ID=%s; Password=%s;"),
			strComputerName, strUser, strPsw);

		spConnection->ConnectionString = (LPCTSTR)strConnectionString;

		VERIFY_SUCCEEDED( spConnection->Open( _bstr_t(_T("")), _bstr_t(_T("")), _bstr_t(_T("")), ADODB::adConnectUnspecified) );
		VERIFY_SUCCEEDED( spConnection->Close() );

		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CDbConnectionTool::Connect( const CString& strComputerName, const CString& strDbSvrName, const CString& strUser, const CString& strPsw, const CString& strServerType, const CString& strConnectionName, BOOL bManaged)
{
	REGISTER_FUNC( Connect( const CString& strComputerName, const CString& strDbSvrName, const CString& strUser, const CString& strPsw) );
	HRESULT  hr = E_FAIL;
	CString strConnectionString;

	m_strConnectionName = strConnectionName;
	m_bManaged = bManaged;

	try
	{
		m_strServerName = strComputerName;
		m_strDataBase = strDbSvrName;
		m_strUser = strUser;
		m_strPsw = strPsw;
		if ( m_bManaged )
		{
			_VERIFY_TRUE( !m_strConnectionName.IsEmpty(), _T("invalid parameters; strConnectionName must not be empty when bManaged is true") );
		}
		
		VERIFY_TRUE( !IsConnected() );		
		VERIFY_SUCCEEDED( m_spConnection.CreateInstance( __uuidof( ADODB::Connection) ) );

		if ( strServerType.CompareNoCase(_T("ORACLE")) == 0 )
		{
		//	strConnectionString.Format( _T("Provider=MSDAORA.1;Data Source=%s;User ID=%s;Password=%s;Persist Security Info=True;"),
			strConnectionString.Format( _T("Provider=OraOLEDB.Oracle.1;Data Source=%s;User ID=%s;Password=%s;Persist Security Info=True;"),
				strDbSvrName, strUser, strPsw);			
		}
		if ( strServerType.CompareNoCase( _T("SQLSERVER") ) == 0 )
		{
			strConnectionString.Format( _T("Provider=SQLOLEDB; Data Source=%s; Initial Catalog=%s; User ID=%s; Password=%s;"),
				strComputerName, strDbSvrName, strUser, strPsw);
		}
		else
		if ( strServerType.CompareNoCase( _T("ACCESS") ) == 0  )
		{
			strConnectionString.Format( _T("Provider=Microsoft.Jet.OLEDB.4.0; Data Source=%s; User ID=%s; Password=%s;"),			
				strDbSvrName, strUser, strPsw);
		}
		else
		if ( strServerType.CompareNoCase( _T("ACCESS_2007") ) == 0 )
		{			
			strConnectionString.Format( _T("Provider=Microsoft.ACE.OLEDB.12.0; Data Source=%s; User ID=%s; Password=%s;"),
				strDbSvrName, strUser, strPsw);

		}
		
		m_spConnection->ConnectionString = (LPCTSTR)strConnectionString;
		
		VERIFY_SUCCEEDED( m_spConnection->Open( _bstr_t(_T("")), _bstr_t(_T("")), _bstr_t(_T("")), ADODB::adConnectUnspecified) );
		

		if ( m_bManaged )
		{
			tagConnectionInfo ConnectionInfo;
			ConnectionInfo.spConnection = GetConnectionPtr();
			ConnectionInfo.pDbConnectionTool = this;
			ConnectionInfo.strConnectionString = strConnectionString;
			GetConnectionName( ConnectionInfo.strConnectionName );
			GetGlobalDBConnectionMgr()->Add( ConnectionInfo );
		}

		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CDbConnectionTool::DisConnect()
{
	REGISTER_FUNC( CDbConnectionTool::DisConnect() );
	HRESULT hr = E_FAIL;

	try
	{
		if ( m_spConnection != NULL )
		{				
			VERIFY_SUCCEEDED( m_spConnection->Close() );		
		}
		
		hr = S_OK;
	}
	my_catch;

	return hr;
}

BOOL CDbConnectionTool::IsConnected()
{
	return ( m_spConnection != NULL && m_spConnection->State != NULL );
}

/*
CDbConnectionTool& CDbConnectionTool::operator=( const CDbConnectionTool& rhs )
{
	m_spConnection = rhs.m_spConnection;
	m_strConnectionName = rhs.m_strConnectionName;
	m_bManaged = rhs.m_bManaged;

	return *this;
}
*/

ADODB::_ConnectionPtr CDbConnectionTool::GetConnectionPtr()
{
	return m_spConnection;
}

HRESULT CDbConnectionTool::GetConnectionName(CString& strConnectionName)
{
	strConnectionName = m_strConnectionName;
	return strConnectionName.IsEmpty() ? E_FAIL : S_OK;
}


///////////////////////////////////////////////////////////////////////////
//
//
CDbRecordSetTool::CDbRecordSetTool( LPCTSTR lpszConnectionName ) : m_spRecordset(NULL), m_spConnection(NULL), m_bInit( FALSE )
{
	int nIndex = -1;
	CDbConnectionTool *pConnectionTool = NULL;
	CString strConnectionName = lpszConnectionName;
	
	m_bInit = SUCCEEDED( GetGlobalDBConnectionMgr()->GetConnectionPtrByName( strConnectionName, m_spConnection ) );

}

CDbRecordSetTool::tagFieldInfo::tagFieldInfo()
{
	dataType = ADODB::adEmpty;
}

CDbRecordSetTool::tagFieldInfo::tagFieldInfo(const tagFieldInfo& rhs)
{
	dataType = rhs.dataType;
	fieldSize = 0;
	strFieldName = rhs.strFieldName;
}

CString CDbRecordSetTool::tagFieldInfo::DataType2String(  )
{
	CString strType;

	switch( dataType )
	{
	case ADODB:: adEmpty:
		strType = _T("adEmpty");
		break;
			
	case ADODB:: adTinyInt:
		strType = _T("adTinyInt");
		break;

	case ADODB:: adSmallInt:
		strType = _T("adSmallInt");
		break;

	case ADODB:: adInteger:
		strType = _T("adInteger");
		break;

	case ADODB:: adBigInt:
		strType = _T("adBigInt");
		break;

	case ADODB:: adUnsignedTinyInt:
		strType = _T("adUnsignedTinyInt");
		break;

	case ADODB:: adUnsignedSmallInt:
		strType = _T("adUnsignedSmallInt");
		break;

	case ADODB:: adUnsignedInt:
		strType = _T("adUnsignedInt");
		break;

	case ADODB:: adUnsignedBigInt:
		strType = _T("adUnsignedBigInt");
		break;

	case ADODB:: adSingle:
		strType = _T("adSingle");
		break;
			
	case ADODB:: adDouble:
		strType = _T("adDouble");
		break;

	case ADODB:: adCurrency:
		strType = _T("adCurrency");
		break;

	case ADODB:: adDecimal:
		strType = _T("adDecimal");
		break;

	case ADODB:: adNumeric:
		strType = _T("adNumeric");
		break;

	case ADODB:: adBoolean:
		strType = _T("adBoolean");
		break;

	case ADODB:: adError:
		strType = _T("adError");
		break;
			
	case ADODB:: adUserDefined:
		strType = _T("adUserDefined");
		break;

	case ADODB:: adVariant:
		strType = _T("adVariant");
		break;

	case ADODB:: adIDispatch:
		strType = _T("adIDispatch");
		break;

	case ADODB:: adIUnknown:
		strType = _T("adIUnknown");
		break;

	case ADODB:: adGUID:
		strType = _T("adGUID");
		break;

	case ADODB:: adDate:
		strType = _T("adDate");
		break;

	case ADODB:: adDBDate:
		strType = _T("adDBDate");
		break;
			
	case ADODB:: adDBTime:
		strType = _T("adDBTime");
		break;

	case ADODB:: adDBTimeStamp:
		strType = _T("adDBTimeStamp");
		break;

	case ADODB:: adBSTR:
		strType = _T("adBSTR");
		break;
			
	case ADODB:: adChar:
		strType = _T("adChar");
		break;

	case ADODB:: adVarChar:
		strType = _T("adVarChar");
		break;
			
	case ADODB:: adLongVarChar:
		strType = _T("adLongVarChar");
		break;

	case ADODB:: adWChar:
		strType = _T("adWChar");
		break;

	case ADODB:: adVarWChar:
		strType = _T("adVarWChar");
		break;

	case ADODB:: adLongVarWChar:
		strType = _T("adLongVarWChar");
		break;

	case ADODB:: adBinary:
		strType = _T("adBinary");
		break;

	case ADODB:: adVarBinary:
		strType = _T("adVarBinary");
		break;

	case ADODB:: adLongVarBinary:
		strType = _T("adLongVarBinary");
		break;

	case ADODB:: adChapter:
		strType = _T("adChapter");
		break;

	case ADODB:: adFileTime:
		strType = _T("adFileTime");
		break;

	case ADODB:: adPropVariant:
		strType = _T("adPropVariant");
		break;

	case ADODB:: adVarNumeric:
		strType = _T("adVarNumeric");
		break;

	case ADODB:: adArray:
		strType = _T("adArray");
		break;

	default:
		strType = _T("UnKnown");


	}

	CString strTemp;
	strTemp.Format( _T("size:%u;"), fieldSize);
	strType += _T("(") + strTemp;
	strTemp.Format( _T("attr:%u"), fieldAttribute);
	strType += strTemp + _T(")");

	return strType;
}

bool CDbRecordSetTool::tagFieldInfo::operator==( const CDbRecordSetTool::tagFieldInfo& rhs )
{
	return ( rhs.strFieldName.CompareNoCase(strFieldName) == 0 &&
		rhs.dataType == dataType &&
		rhs.fieldAttribute == fieldAttribute &&
		rhs.fieldSize == fieldSize );
}

bool CDbRecordSetTool::tagFieldInfo::operator!=( const CDbRecordSetTool::tagFieldInfo& rhs )
{
	return ( ! this->operator ==( rhs) );
}

CDbRecordSetTool::CDbRecordSetTool( ADODB::_ConnectionPtr	spConnection ) : m_spRecordset(NULL), m_bInit( FALSE )
{
	m_spConnection = spConnection;
	m_bInit = (m_spConnection != NULL);

}

CDbRecordSetTool::~CDbRecordSetTool()
{

}

CTime CDbRecordSetTool::GetSvrTime()
{
	CTime time = CTime::GetCurrentTime();
	
	CString strSQL;
	strSQL = _T("Select 'dd' = GetDate()");



	if( FAILED( Execute(strSQL)) )
	{
		
	}

	if( !IsEOF() )
	{		
		time = (this->operator[](_T("dd"))).GetAsTime();
	}

	return time;

}

HRESULT	CDbRecordSetTool::OnConnectionErr()
{
	try
	{
		tagConnectionInfo info;
		GetGlobalDBConnectionMgr()->GetConnectionInfoByName( m_strConnectionName, info );
		if ( info.nLastConnectErrorTime == 0 )
		{
			m_bInit = FALSE;
			info.nLastConnectErrorTime = GetTickCount();
			GetGlobalDBConnectionMgr()->SetConnectionInfoByName( m_strConnectionName, info );
		}
		CheckStatus();
	}
	my_catch;

	return S_OK;
}

HRESULT	CDbRecordSetTool::CheckStatus( )
{
	HRESULT hr = E_FAIL;

	try
	{
		tagConnectionInfo info;
		GetGlobalDBConnectionMgr()->GetConnectionInfoByName( m_strConnectionName, info );
		
		if ( info.nLastConnectErrorTime == 0 )
		{
			return S_OK;			
		}
		else
		{
			if ( (unsigned int)(GetTickCount() - info.nLastConnectErrorTime) >= (unsigned int)10000L )
			{
				info.nLastConnectErrorTime = 0;
				GetGlobalDBConnectionMgr()->SetConnectionInfoByName( m_strConnectionName, info );
				
				ADODB::_ConnectionPtr spConnectionNew;
				spConnectionNew.CreateInstance( __uuidof( ADODB::Connection) );
				spConnectionNew->ConnectionString = (LPCTSTR)info.strConnectionString;
				info.spConnection = spConnectionNew;
				info.spConnection->Open(_bstr_t(_T("")), _bstr_t(_T("")), _bstr_t(_T("")), ADODB::adConnectUnspecified);

				GetGlobalDBConnectionMgr()->SetConnectionInfoByName( m_strConnectionName, info );
				m_spConnection = info.spConnection;
				m_bInit = TRUE;				
			}
		}

		hr = S_OK;
	}
	my_catch;

	return hr;

}

BOOL CDbRecordSetTool::IsInit()
{
	if ( !m_bInit )
	{
		SetExceptionDescription( _T("Database connection is not initialized.") );
	}

	return m_bInit ;
}

HRESULT	CDbRecordSetTool::Execute( CString strSQL )
{
	REGISTER_FUNC( CDbRecordSetTool::Execute( const CString& strSQL ) );
	HRESULT hr = E_FAIL;
	BSTR bstrSQL = NULL;

	try
	{
		GetGlobalDBConnectionMgr()->GetConnectionPtrByName( m_strConnectionName, m_spConnection ) ;

		VERIFY_TRUE( IsInit() );	
		bstrSQL = strSQL.AllocSysString();
		if ( m_spRecordset != NULL )
		{
			m_spRecordset.Release();
		}
		m_spRecordset.CreateInstance( __uuidof( ADODB::Recordset) );
		m_spRecordset = m_spConnection->Execute( bstrSQL, NULL, ADODB::adCmdText) ;

		hr = S_OK;
	}
	catch( _com_error& e)
	{
		hr = e.Error();
		this->SetExceptionDescription( e.Description());

	}
	catch( ... )
	{
		hr = E_FAIL;
	}

	if ( bstrSQL )
	{
		::SysFreeString( bstrSQL );
		bstrSQL = NULL;
	}

	if ( FAILED(hr) )
	{
		long lState = m_spConnection->GetState();
		if ( hr == E_FAIL || lState == 0 )
		{
			OnConnectionErr();			
		}
	}

	return hr;

}

HRESULT		CDbRecordSetTool::Execute(CString strStoreProcName,CString strInputXml,CString& strOutputXml)
{
	REGISTER_FUNC( CDbRecordSetTool::Execute( const CString& strStoreProcName, const CString& strInXML,CString& strOutXml) );
	HRESULT hr = E_FAIL;
	try
	{
		VERIFY_TRUE( IsInit() );			

		ADODB::_CommandPtr spCommand;

		spCommand.CreateInstance( __uuidof( ADODB::Command ) );
		spCommand->ActiveConnection = m_spConnection;
		spCommand->CommandText = _bstr_t(strStoreProcName);
		spCommand->CommandType = ADODB::adCmdStoredProc;
		spCommand->Parameters->Append( spCommand->CreateParameter( _bstr_t("@docXml"), ADODB::adLongVarChar, ADODB::adParamInput, strInputXml.GetLength() + 1, strInputXml.AllocSysString()) );
		ADODB::_RecordsetPtr pRecordset = spCommand->Execute( NULL, NULL, ADODB::adCmdStoredProc );

		if(pRecordset != NULL)
		{
			strOutputXml =  pRecordset->Fields->GetItem("xmlOutput")->GetValue().bstrVal;   
		}

		hr = S_OK;
	}
	my_catch;	

	return hr;

}

HRESULT	CDbRecordSetTool::MakeSqlString( CString& strSQL )
{
	REGISTER_FUNC( CDbRecordSetTool::MakeSqlString( CString& strSQL ) );
	HRESULT hr = E_FAIL;
	
	try
	{		
		int nLength = strSQL.GetLength();
		unsigned char *p = (unsigned char*)strSQL.GetBuffer(0);
		unsigned int flag = 0;
		unsigned int Count = 1;
		
		for (int i=0; i < nLength; i++, p++ )
		{
			if ( flag == 0 )
			{
				if (  (*p) >0x80 )
				{
					flag=1;
				}
				else if (*p == _T('\'') )
				{			
					CString strHead = strSQL.Left( i + Count );
					CString strEnd  = strSQL.Right( strSQL.GetLength() - i - Count );
					strSQL = strHead + _T("'") + strEnd;
					Count++;
				}
			}
			else
			{
				flag=0;
			}
		}
		
		hr = S_OK;
	}
	my_catch;
	
	return hr;
	
}

CDataConvertor	CDbRecordSetTool::operator[]( LPCSTR lpszColumnName)
{
	CString strValue;
	HRESULT hr = GetValue( CString(lpszColumnName), strValue );
	return CDataConvertor( strValue, hr );
}

CDataConvertor	CDbRecordSetTool::operator[]( LPCWSTR lpszColumnName)
{
	CString strValue;
	HRESULT hr = GetValue( lpszColumnName, strValue );
	return CDataConvertor( strValue, hr );
}

HRESULT	CDbRecordSetTool::GetValue( const CString& strName, CString& strValue)
{
	REGISTER_FUNC( CDbRecordSetTool::GetValue( const CString& strName, CString& strValue) );
	CStringTool st;
	HRESULT hr = E_FAIL;	

	strValue.Empty();

	try
	{
		do
		{
			if ( !IsInit() )
			{ break;}

			if ( m_spRecordset == NULL )
			{ break;}

			_variant_t vtName = (LPCTSTR)strName;
			_variant_t vtValue;

			if ( FAILED( m_spRecordset->get_Collect( vtName, &vtValue) ) )
			{ break;}

			if ( FAILED ( st.Variant2String( vtValue, strValue) ) )
			{ break;}
			hr = S_OK;
		}while(0); // yes, while(0)


	}
	my_catch;

	return hr;
}

HRESULT	CDbRecordSetTool::MoveNext()
{
	REGISTER_FUNC( CDbRecordSetTool::MoveNext() );
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( IsInit() );
		VERIFY_SUCCEEDED( m_spRecordset->MoveNext() );
		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CDbRecordSetTool::MovePrevious()
{
	REGISTER_FUNC( CDbRecordSetTool::MovePrevious() );
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( IsInit() );
		VERIFY_SUCCEEDED( m_spRecordset->MovePrevious() );

	}
	my_catch;

	return hr;
}

HRESULT CDbRecordSetTool::MoveFirst()
{
	REGISTER_FUNC( CDbRecordSetTool::MoveFirst() );
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( IsInit() );
		VERIFY_SUCCEEDED( m_spRecordset->MoveFirst() );
		hr = S_OK;
	}
	my_catch;

	return hr;
}
	
HRESULT CDbRecordSetTool::MoveLast()
{
	REGISTER_FUNC( CDbRecordSetTool::MoveLast() );
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( IsInit() );
		VERIFY_SUCCEEDED( m_spRecordset->MoveLast() );

		hr = S_OK;
	}
	my_catch;

	return hr;
}

BOOL CDbRecordSetTool::IsEOF()
{
	REGISTER_FUNC( CDbRecordSetTool::IsEOF() );
	VARIANT_BOOL vb = VARIANT_TRUE;

	try
	{
		VERIFY_TRUE( IsInit() );
		VERIFY_SUCCEEDED( m_spRecordset->get_ADOEOF( &vb ) );
	}
	my_catch;
	
	return ( vb != VARIANT_FALSE );

}

HRESULT	CDbRecordSetTool::GetAllViews( CStringArray& strViewsArray )
{
	REGISTER_FUNC( HRESULT	CDbRecordSetTool::GetAllViews( CStringArray& strViewsArray ) );
	HRESULT hr = E_FAIL;

	strViewsArray.RemoveAll();

	try
	{
		CString strSQL = _T("SELECT * ")
			_T("FROM sys.sysobjects ")
			_T("WHERE (type = 'V') ")
			_T("ORDER BY name ");
		// The query above may return several rows per each stored procedure 
		// if the definition text is longer than 4000 characters.
		VERIFY_SUCCEEDED( Execute( strSQL) );

		while( !IsEOF())
		{
			tagProcedureInfo info;

			strViewsArray.Add( this->operator []( _T("name") ).GetAsString() );
			MoveNext();
		}

		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT	CDbRecordSetTool::GetAllTableName( CStringArray& strTableArray )
{
	REGISTER_FUNC(HRESULT	CDbRecordSetTool::GetAllTableName( CStringArray& strTableArray ));
	HRESULT hr = E_FAIL;
	CStringTool stringTool;
	strTableArray.RemoveAll();

	try
	{
		VERIFY_TRUE( IsInit() );	

		ADODB::_RecordsetPtr spRecordset;
		
		VERIFY_SUCCEEDED( spRecordset.CreateInstance( __uuidof( ADODB::Recordset) ) );
		
		spRecordset = m_spConnection->OpenSchema( ADODB::adSchemaTables );   		

		while( spRecordset->ADOEOF != VARIANT_TRUE )     
		{   
			CString strTableName;
			CString strTableType;

			_variant_t vDBTableName= spRecordset->GetCollect("TABLE_NAME");         
			_variant_t VDBTableType = spRecordset->GetCollect("TABLE_TYPE");     

			stringTool.Variant2String( VDBTableType, strTableType) ;
			if ( strTableType == _T("TABLE") )    
			{     				
				stringTool.Variant2String( vDBTableName, strTableName );
				strTableArray.Add( strTableName );			 
			}   
  
			spRecordset->MoveNext();     
		}     
		
		VERIFY_SUCCEEDED( spRecordset->Close() );//关闭记录集   

		hr = S_OK;
	}
	my_catch;

	return hr;
}


HRESULT	CDbRecordSetTool::GetAllFields( CString strTableName, CArray< tagFieldInfo, tagFieldInfo&>& strFieldsArray )
{
	REGISTER_FUNC( HRESULT	CDbRecordSetTool::GetAllFields( CString strTableName, CStringArray& strFieldsArray ) );
	HRESULT hr = E_FAIL;
	CStringTool stringTool;
	BSTR bstrTableName = NULL;

	strFieldsArray.RemoveAll();
	try
	{
		VERIFY_TRUE( IsInit() );	
		VERIFY_TRUE( !strTableName.IsEmpty() );

		ADODB::_RecordsetPtr spRecordset;

		VERIFY_SUCCEEDED( spRecordset.CreateInstance( __uuidof( ADODB::Recordset) ) );
		
		bstrTableName = strTableName.AllocSysString();
		spRecordset = m_spConnection->Execute( bstrTableName, NULL, ADODB::adCmdTable) ;

		VERIFY_TRUE( spRecordset != NULL );

		long lFieldsCount = spRecordset->GetFields()->GetCount();

		VERIFY_TRUE( lFieldsCount > 0 );

		for ( long i = 0; i < lFieldsCount; i++)
		{
			ADODB::FieldPtr spField;
			tagFieldInfo fieldInfo;
			spField = spRecordset->GetFields()->GetItem( i );
			VERIFY_TRUE( spField != NULL );
			_variant_t bstrName = spField->GetName();		
			stringTool.Variant2String( bstrName, fieldInfo.strFieldName );
			fieldInfo.dataType = spField->GetType();
			fieldInfo.fieldSize = spField->GetDefinedSize();
			fieldInfo.fieldAttribute = spField->GetAttributes();

			//

			strFieldsArray.Add( fieldInfo );
		}

		hr = S_OK;		
	}
	my_catch;

	if ( bstrTableName )
	{
		::SysFreeString( bstrTableName );
	}

	return hr;
}

HRESULT	CDbRecordSetTool::GetAllProcedures( CProcedureInfoArray& strFieldsArray )
{
	REGISTER_FUNC( HRESULT	CDbRecordSetTool::GetAllProcedures( CProcedureInfoArray& strFieldsArray ) );
	HRESULT hr = E_FAIL;

	strFieldsArray.RemoveAll();

	try
	{
		CString strSQL = _T("select su.name, so.name  AS name1, sc.text ")
								_T("from sysobjects so, syscomments sc, sysusers su ")
								_T("where xtype='P' and so.id=sc.id and so.uid=su.uid ")
								_T("order by name1, su.name, sc.colid ");
		// The query above may return several rows per each stored procedure 
		// if the definition text is longer than 4000 characters.
		VERIFY_SUCCEEDED( Execute( strSQL) );
		
		CString strPreName;

		while( !IsEOF())
		{
			tagProcedureInfo info;

			info.strName = this->operator []( _T("name1") ).GetAsString();
			info.strText = this->operator []( _T("text") ).GetAsString();

			// 对于一个较长的存储过程
			// 其内容是分成几部分返回的
			// 例如 sp_xxx			第一部分
			//		sp_xxx			第二部分
			// 要把这几部分加起来才能形成一个完整的 存储过程
			if ( info.strName.IsEmpty() )
			{
				continue;
			}
			if ( info.strName == strPreName )
			{
				strFieldsArray[ strFieldsArray.GetSize() - 1 ].strText += info.strText; 
			}
			else
			{
				strFieldsArray.Add( info );
			}

			strPreName = info.strName;
			
			
			MoveNext();
		}

		hr = S_OK;
	}
	my_catch;

	return hr;

}

NAMESPACE_MARRCO_END


