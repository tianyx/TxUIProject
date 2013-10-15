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


//    CDbConnectionTool ͨ������ȫ�ֱ���
class CDbConnectionTool : public CErrReport
{
public:
		// bManaged �����ó�TRUE ʱ�����뱣֤�ö������������������ڼ���Ч
	CDbConnectionTool();		
	virtual ~CDbConnectionTool();
		
public:
	//					�õ����������е� SQL Server
	HRESULT				GetSqlServers( CStringArray& strSqlsArray );

	//					�õ�ָ�����ݿ�������ϵ��������ݿ�
	//					��� �ѳɹ����� Connect�� �� lpszServerName��lpszUser ����Ϊ��
	HRESULT				GetDataBases( CStringArray& strDataBaseArray, LPCTSTR lpszServerName = _T(""), LPCTSTR lpszUser = _T(""), LPCTSTR lpszPsw = _T("") ) ;

	HRESULT				TestConnectSqlServer( const CString& strComputerName, const CString& strUser, const CString& strPsw);
public:
	//					strServerType : SQLSERVER		sql server ϵ��
	//									ACCESS			microsoft Access     mdb ���ݿ�
	//									ACCESS_2007		microsoft Access 2007  accdb ���ݿ�
	HRESULT				Connect( const CString& strComputerName, const CString& strDbSvrName, const CString& strUser, const CString& strPsw, const CString& strServerType = _T("SQLSERVER"), const CString& strConnectionName = _T(""), BOOL bManaged = FALSE );

	HRESULT				DisConnect();
	HRESULT				GetConnectionName(CString& strConnectionName);
	BOOL				IsConnected();
		
	ADODB::_ConnectionPtr GetConnectionPtr();
		
private:
		// copy constructor �Լ� operator= ������ ����������û�ж���
		// �������κοͻ����ø÷���
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
		CString						strName;				// �洢��������
		CString						strText;				// �洢���̵�����
	};

	struct tagFieldInfo
	{
		CString						strFieldName;			// �ֶ�����
		ADODB::DataTypeEnum			dataType;				// �ֶ����ͣ� char ,int ��
		ADODB::ADO_LONGPTR			fieldSize;				// �ֶγ���, varchar(50) ��
		long						fieldAttribute;			// �ֶ����ԣ����Ƿ�����Ϊ�յ�
															// fieldAttribute �� ADODB::FieldAttributeEnum �����
		

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
	
	//						�ƶ�����һ����¼
	HRESULT					MoveNext();
	
	//						�ƶ���ǰһ����¼
	HRESULT					MovePrevious();
	
	//						ע�⣺MoveFirst ��Щʱ��ᵼ�� SQL ��䱻����ִ��һ��
	HRESULT					MoveFirst();
	
	//						�ƶ������һ����¼
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

	//						�õ����еı�
	HRESULT					GetAllTableName( CStringArray& strTableArray );

	//						�õ�������ͼ
	HRESULT					GetAllViews( CStringArray& strTableArray );

	//						�õ�ָ����������ֶ�
	HRESULT					GetAllFields( CString strTableName, CArray< tagFieldInfo, tagFieldInfo&>& strFieldsArray );

	//						�õ����еĴ洢����
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
