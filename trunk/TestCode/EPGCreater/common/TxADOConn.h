/********************************************************************
	created:	2013/12/06
	created:	$2013:12:6
	filename: 	TxADOConn.h
	file base:	TxADOConn
	file ext:	h
	author:		tianyx
	
	purpose:	ado conn class
*********************************************************************/
#pragma once
#include <map>
#include "AutoCritSec.h"
#include "StrConvert.h"
#import "..\commondll\msado15.dll" rename_namespace("TXADODB") rename("EOF", "TXADOEOF") 

using namespace std;
using namespace TXADODB;

typedef DWORD DWDBKEY;
#define DBKEY_ANY -1

//////////////////////////////////////////////////////////////////////////
//use CTxADORecordSet to get a record set;
class CTxADORecordSet
{
public:
	CTxADORecordSet(DWDBKEY dwKey = DBKEY_ANY);
	~CTxADORecordSet(void);

public:
	
	BOOL ExecuteSQL(CString& strSqlIn);

	//after open a record set
	long GetRecordCount();
	BOOL IsEOF();
	BOOL MoveNext();
	BOOL MoveFirst();
	CTxStrConvert GetVal(LPCTSTR szColNameIn, LPCTSTR def = TEXT(""));
	BOOL SetVal(LPCTSTR szColNameIn, CTxStrConvert valIn);

private:
	_ConnectionPtr m_apConn;
	_RecordsetPtr m_apRecordSet;
};


//////////////////////////////////////////////////////////////////////////
//use CTxADOCommand to execute a command sql
class CTxADOCommand
{
public:
	CTxADOCommand(DWDBKEY dwKey = DBKEY_ANY);
	~CTxADOCommand(void);

	BOOL ExecuteSQL(CString& strSqlIn);

	BOOL ExecuteSQLs(VECSTRINGS& vSqlIn);

	//only for single sql executing.
	BOOL BeginTrans();
	BOOL CommitTrans();
	BOOL RollBackTrans();


private:
	_ConnectionPtr m_apConn;
	_CommandPtr m_apCommand;
};


struct ST_ADODBCONNINFO
{
	DWDBKEY dwKey;
	CString strConnStr;
	_Connection* pConn;

	ST_ADODBCONNINFO()
	{
		dwKey  = -1;
		pConn = NULL;
	}
};
typedef map<DWORD, ST_ADODBCONNINFO> MAPADOCONNS;

class CTxADODBMgr
{
friend class CTxADORecordSet;
friend class CTxADOCommand;

private:
	CTxADODBMgr(){}
	~CTxADODBMgr(){Clear();}
public:
	BOOL OpenDB(const DWORD dwDBKey, LPCTSTR szConnStr);
	BOOL OpenDB(const DWORD dwDBkey, LPCTSTR szSvrName, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPw);
	
	BOOL CloseDB(const DWORD dwDBkey);
	
protected:
	//the returned ptr must be released by caller
	_Connection* GetADOConnection(const DWORD dwDBKey = DBKEY_ANY);

private:
	void Clear();
	_Connection* ConnectDB(LPCTSTR szConnStr);
public:
	static CTxADODBMgr* GetADODBMgr();
	static void ReleaseDBMgr();

private:
	static CTxADODBMgr* m_spDBMgr;
	CAutoCritSec m_csdbLock;
	MAPADOCONNS m_mapAdoConns;

};