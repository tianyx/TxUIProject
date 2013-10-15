#pragma once


// Database Tool Class
#pragma warning(disable:4146)
#import "C:\Program Files\Common Files\System\ado\msado15.dll" rename_namespace("ADODB") rename("EOF","ADOEOF")


NAMESPACE_MARRCO

class CDbConnectionTool;
struct tagConnectionInfo
{
	tagConnectionInfo()
	{
		spConnection = NULL;		
		pDbConnectionTool = NULL;
		nLastConnectErrorTime = 0;
	}

	ADODB::_ConnectionPtr	spConnection;	
	CString					strConnectionString;
	CString					strConnectionName;
	CDbConnectionTool		*pDbConnectionTool;
	__int64					nLastConnectErrorTime;
};


//    CDbConnectionTool 通常创建全局变量
class CDbConnectionTool : public CErrReport
{
public:
		// bManaged 被设置成TRUE 时，必须保证该对象在整个程序运行期间有效
	CDbConnectionTool();		
	virtual ~CDbConnectionTool();
		
public:
	//					得到网络上所有的 SQL Server
	HRESULT				GetSqlServers( CStringArray& strSqlsArray );

	//					得到指定数据库服务器上的所有数据库
	//					如果 已成功调用 Connect， 则 lpszServerName、lpszUser 可以为空
	HRESULT				GetDataBases( CStringArray& strDataBaseArray, LPCTSTR lpszServerName = _T(""), LPCTSTR lpszUser = _T(""), LPCTSTR lpszPsw = _T("") ) ;

	HRESULT				TestConnectSqlServer( const CString& strComputerName, const CString& strUser, const CString& strPsw);
public:
	//					strServerType : SQLSERVER		sql server 系列
	//									ACCESS			microsoft Access     mdb 数据库
	//									ACCESS_2007		microsoft Access 2007  accdb 数据库
	HRESULT				Connect( const CString& strComputerName, const CString& strDbSvrName, const CString& strUser, const CString& strPsw, const CString& strServerType = _T("SQLSERVER"), const CString& strConnectionName = _T(""), BOOL bManaged = FALSE );

	HRESULT				DisConnect();
	HRESULT				GetConnectionName(CString& strConnectionName);
	BOOL				IsConnected();
		
	ADODB::_ConnectionPtr GetConnectionPtr();
		
private:
		// copy constructor 以及 operator= 操作符 仅有声明，没有定义
		// 不允许任何客户调用该方法
	CDbConnectionTool( const CDbConnectionTool& rhs );
	CDbConnectionTool& operator=( const CDbConnectionTool& rhs );
		
private:
	ADODB::_ConnectionPtr	m_spConnection;	
	CString					m_strConnectionName;
	BOOL					m_bManaged;
	CString					m_strServerName;
	CString					m_strDataBase;
	CString					m_strUser;
	CString					m_strPsw;
};


class CDbConnectionMgr : public CDataMgr<tagConnectionInfo, tagConnectionInfo&>
{
public:
	CDbConnectionMgr();
	BOOL				Lookup(const CString& strConnectionName, int& nIndex);
	HRESULT				GetConnectionPtrByName( CString& strConnectionName, ADODB::_ConnectionPtr& spConnectionPtr );
	HRESULT				SetConnectionInfoByName( CString strConnectionName, tagConnectionInfo& pInfo );
	HRESULT				GetConnectionInfoByName( CString strConnectionName, tagConnectionInfo& pInfo );
	HRESULT				GetDefaultConnectionName( CString& strConnectionName );
	HRESULT				SetDefaultConnectionName( const CString& strConnectionName );
	HRESULT				GetDefaultConnectionPtr( ADODB::_ConnectionPtr& spConnectionPtr );
	
protected:
	CString				m_strDefaultConnectionName;
	ADODB::_ConnectionPtr	m_spDefaultConnectionPtr;	
};

extern"C" MARRCO_DECLSPEC CDbConnectionMgr* GetGlobalDBConnectionMgr();



class CDbRecordSetTool : public CErrReport
{
public:
	struct tagProcedureInfo
	{
		CString						strName;				// 存储过程名称
		CString						strText;				// 存储过程的内容
	};

	struct tagFieldInfo
	{
		CString						strFieldName;			// 字段名称
		ADODB::DataTypeEnum			dataType;				// 字段类型， char ,int 等
		ADODB::ADO_LONGPTR			fieldSize;				// 字段长度, varchar(50) 等
		long						fieldAttribute;			// 字段属性，如是否允许为空等
															// fieldAttribute 是 ADODB::FieldAttributeEnum 的组合
		

		tagFieldInfo();
		tagFieldInfo(const tagFieldInfo&);

		CString DataType2String(  );
		bool operator==( const tagFieldInfo& rhs );
		bool operator!=( const tagFieldInfo& rhs );
	};

	typedef CArray< CDbRecordSetTool::tagFieldInfo, CDbRecordSetTool::tagFieldInfo& >  CFieldInfoArray;
	typedef CArray< CDbRecordSetTool::tagProcedureInfo, CDbRecordSetTool::tagProcedureInfo&> CProcedureInfoArray;

public:
	CDbRecordSetTool( LPCTSTR lpszConnectionName = NULL );
	CDbRecordSetTool( ADODB::_ConnectionPtr	spConnection );
	virtual ~CDbRecordSetTool();
	
public:	
	
	HRESULT					Execute( CString strSQL );
	
	HRESULT					Execute(CString strStoreProcName,CString strInXML,CString& strOutXml);
	
	//						移动到下一条记录
	HRESULT					MoveNext();
	
	//						移动到前一条记录
	HRESULT					MovePrevious();
	
	//						注意：MoveFirst 有些时候会导致 SQL 语句被重新执行一次
	HRESULT					MoveFirst();
	
	//						移动到最后一条记录
	HRESULT					MoveLast();
	
	BOOL					IsEOF();
	
	HRESULT					MakeSqlString( CString& strSQL );

	HRESULT					CheckStatus( );
	
	HRESULT					OnConnectionErr();

	BOOL					IsInit();
	
	CTime					GetSvrTime();

	CDataConvertor			operator[]( LPCSTR lpszColumnName);
	CDataConvertor			operator[]( LPCWSTR lpszColumnName);
	
public:

	//						得到所有的表
	HRESULT					GetAllTableName( CStringArray& strTableArray );

	//						得到所有视图
	HRESULT					GetAllViews( CStringArray& strTableArray );

	//						得到指定表的所有字段
	HRESULT					GetAllFields( CString strTableName, CArray< tagFieldInfo, tagFieldInfo&>& strFieldsArray );

	//						得到所有的存储过程
	//						Requirements: Microsoft SQL Server 2000 or later 
	HRESULT					GetAllProcedures( CProcedureInfoArray& strFieldsArray );

protected:
	HRESULT					GetValue( const CString& strName, CString& strValue);
	
private:
	CString					m_strConnectionName;
	BOOL					m_bInit;
	ADODB::_RecordsetPtr	m_spRecordset;
	ADODB::_ConnectionPtr	m_spConnection;
};

typedef CDbRecordSetTool::CFieldInfoArray		CFieldInfoArray;
typedef CDbRecordSetTool::CProcedureInfoArray	CProcedureInfoArray;

NAMESPACE_MARRCO_END
