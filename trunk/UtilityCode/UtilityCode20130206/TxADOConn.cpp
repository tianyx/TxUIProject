#include "StdAfx.h"
#include "TxADOConn.h"
#include "FGlobal.h"

CTxADODBMgr* CTxADODBMgr::m_spDBMgr = NULL;

//////////////////////////////////////////////////////////////////////////

CTxADORecordSet::CTxADORecordSet( DWDBKEY dwKey /*= DBKEY_ANY*/ )
{
	CoInitialize(NULL);
	m_apConn = CTxADODBMgr::GetADODBMgr()->GetADOConnection(dwKey);
	if (!m_apConn || m_apConn->GetState() == adStateClosed)
	{
		ASSERT(FALSE);
	}
	else
	{
		m_apRecordSet.CreateInstance(__uuidof(Recordset));
		if (!m_apRecordSet)
		{
			ASSERT(FALSE);
			TRACE("create Recordset Error!");
		}
		
	}
}

CTxADORecordSet::~CTxADORecordSet(void)
{
	if (m_apRecordSet && m_apRecordSet->GetState() != adStateClosed)
	{
		m_apRecordSet->Close();
	}

}

BOOL CTxADORecordSet::ExecuteSQL( CString& strSqlIn )
{
	BOOL bRet = FALSE;
	if (!m_apRecordSet)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	if (m_apRecordSet->GetState() != adStateClosed)
	{
		m_apRecordSet->Close();
	}

	try
	{
	 	HRESULT hr = m_apRecordSet->Open(_variant_t(strSqlIn),_variant_t((IDispatch *)m_apConn), adOpenKeyset,
			adLockOptimistic,adCmdText);
		if (FAILED(hr))
		{
			ASSERT(FALSE);
		}
		else
		{
			bRet = TRUE;
		}
	}
	catch(_com_error &e)
	{
		CString str = (TCHAR*)e.Description();
		CFWriteLog(0, TEXT("Open RecordsetErr %s, %s"), str, strSqlIn);
	}

	return bRet;

}

long CTxADORecordSet::GetRecordCount()
{
	if (m_apRecordSet->GetState() != adStateOpen)
	{
		ASSERT(FALSE);
		return 0;
	}
	
	long nCountNum = 0;
	nCountNum = m_apRecordSet->RecordCount;

	if(nCountNum < 0)
	{
		//可能是因为没有主键造成的
		nCountNum = 0;
		try
		{
			if(m_apRecordSet->BOF)
			{
				//说明记录集为空
				nCountNum = 0;
			}
			else
			{
				m_apRecordSet->MoveFirst();
				while(!m_apRecordSet->TXADOEOF)
				{
					nCountNum++;
					m_apRecordSet->MoveNext();
				}
			}
		}
		catch (...)
		{
			return 0;
		}
	}
	return nCountNum;
}

BOOL CTxADORecordSet::IsEOF()
{
	if (!m_apRecordSet || m_apRecordSet->GetState() != adStateOpen)
	{
		return TRUE;
	}


	return m_apRecordSet->TXADOEOF;
}

BOOL CTxADORecordSet::MoveNext()
{
	if (!m_apRecordSet || m_apRecordSet->GetState() != adStateOpen)
	{
		return FALSE;
	}


	if (!m_apRecordSet->TXADOEOF)
	{
		m_apRecordSet->MoveNext();
		return TRUE;
	}

	return FALSE;
}

BOOL CTxADORecordSet::MoveFirst()
{
	if (!m_apRecordSet || m_apRecordSet->GetState() != adStateOpen)
	{
		return FALSE;
	}

	
	if (!m_apRecordSet->BOF)
	{
		m_apRecordSet->MoveFirst();
		return TRUE;
	}

	return FALSE;
}

CTxStrConvert CTxADORecordSet::GetVal( LPCTSTR szColNameIn, LPCTSTR def /*= TEXT("")*/ )
{
	CTxStrConvert valOut(def);
	if (!m_apRecordSet || m_apRecordSet->GetState() != adStateOpen)
	{
		return valOut;
	}

	try
	{
		FieldsPtr fieldsPtr =  m_apRecordSet->GetFields();
		if (fieldsPtr)
		{
			FieldPtr fPtr= fieldsPtr->GetItem(_variant_t(szColNameIn));
			if (fPtr)
			{
				valOut.SetVal((LPCTSTR)((_bstr_t)(fPtr->GetValue())));
			}

		}
	}
	catch(_com_error &e)
	{
		CString strErr = (TCHAR*)e.Description();
		TRACE( TEXT("Get Filed Val Error col =%s, %s"), szColNameIn, strErr);
	}

	return valOut;
}


//////////////////////////////////////////////////////////////////////////

CTxADOCommand::CTxADOCommand( DWDBKEY dwKey /*= DBKEY_ANY*/ )
{
	CoInitialize(NULL);
	m_apConn = CTxADODBMgr::GetADODBMgr()->GetADOConnection(dwKey);
	if (!m_apConn || m_apConn->GetState() == adStateClosed)
	{
		ASSERT(FALSE);
	}
	else
	{
		m_apCommand.CreateInstance(__uuidof(Command));
		if (!m_apCommand)
		{
			ASSERT(FALSE);
			TRACE("create command Error!");
		}
		else
		{
			m_apCommand->ActiveConnection = m_apConn;
		}

	}
}

CTxADOCommand::~CTxADOCommand( void )
{
}

BOOL CTxADOCommand::ExecuteSQL( CString& strSqlIn )
{
	if (m_apConn->GetState() == adStateClosed)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	
	BOOL bRet = FALSE;
	BSTR btr = strSqlIn.AllocSysString();
	CComBSTR bstrSql(btr);
	::SysFreeString(btr);
	try
	{

		VARIANT varRecordsAffected;
		varRecordsAffected.vt = VT_INT;
		VARIANT paramSQl;
		paramSQl.vt = VT_BSTR;
		paramSQl.bstrVal = bstrSql;
		m_apCommand->CommandText = paramSQl.bstrVal;
		m_apCommand->CommandType = adCmdText;
		m_apCommand->Execute(&varRecordsAffected, NULL, adCmdText|adExecuteNoRecords);
		bRet = TRUE;
	
	}
	catch(_com_error &e)
	{
		CString str = (TCHAR*)e.Description();
		CFWriteLog(0, TEXT("execute commandErr %s, %s"), str, strSqlIn);
	}

	return bRet;
}

BOOL CTxADOCommand::ExecuteSQLs( VECSTRINGS& vSqlIn )
{
	if (m_apConn->GetState() == adStateClosed)
	{
		ASSERT(FALSE);
		return FALSE;
	}


	BOOL bRet = FALSE;
	try
	{
		m_apConn->BeginTrans();
		for (size_t i = 0; i < vSqlIn.size(); ++i)
		{
			BSTR btr = vSqlIn[i].AllocSysString();
			CComBSTR bstrSql(btr);
			::SysFreeString(btr);
			VARIANT varRecordsAffected;
			VARIANT paramSQl;
			paramSQl.vt = VT_BSTR;
			paramSQl.bstrVal = bstrSql;
			m_apCommand->Execute(&varRecordsAffected, & paramSQl, adCmdText|adExecuteNoRecords);
		}
		
		m_apConn->CommitTrans();
		bRet = TRUE;

	}
	catch(_com_error &e)
	{
		m_apConn->RollbackTrans();
		CString strerr = (TCHAR*)e.Description();
		CFWriteLog(0, TEXT("execute batch commandErr %s"), strerr);
	}

	return bRet;
}


//////////////////////////////////////////////////////////////////////////
BOOL CTxADODBMgr::OpenDB( const DWORD dwDBKey, LPCTSTR szConnStr )
{
	if (szConnStr == NULL|| dwDBKey == DBKEY_ANY)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	BOOL bRet = TRUE;
	CAutoLock lock(&m_csdbLock);
	MAPADOCONNS::iterator itf = m_mapAdoConns.find(dwDBKey);
	if (itf != m_mapAdoConns.end())
	{
		//found it
		if(itf->second.strConnStr.CompareNoCase(szConnStr) != 0)
		{
			//not match!!!
			CFWriteLog(0, TEXT("different dbstr using same key!!!"));
			ASSERT(FALSE);
			bRet = FALSE;
		}
	}
	else
	{
		ST_ADODBCONNINFO info;
		info.pConn = ConnectDB(szConnStr);
		if (info.pConn == NULL)
		{
			ASSERT(FALSE);
			bRet = FALSE;
		}
		else
		{
			info.dwKey = dwDBKey;
			info.strConnStr = szConnStr;
			m_mapAdoConns[dwDBKey] = info;
		}
		
	}

	return bRet;
}

BOOL CTxADODBMgr::OpenDB( const DWORD dwDBkey, LPCTSTR szSvrName, LPCTSTR szDBName, LPCTSTR szUser, LPCTSTR szPw )
{
	CString strConn;
	strConn.Format(TEXT("Provider=SQLOLEDB; Server=%s;" "Database=%s; uid=%s; pwd=%s;"), szSvrName, szDBName, szUser, szPw);
	return OpenDB(dwDBkey, strConn);
}

BOOL CTxADODBMgr::CloseDB( const DWORD dwDBkey )
{
	CAutoLock lock(&m_csdbLock);
	MAPADOCONNS::iterator itf = m_mapAdoConns.find(dwDBkey);
	if (itf != m_mapAdoConns.end())
	{
		_Connection* pConn = itf->second.pConn;
		m_mapAdoConns.erase(itf);

		if (pConn)
		{
			try
			{
				pConn->Close();
				pConn->Release();
			}
			catch(_com_error e)
			{
				ASSERT(FALSE);
			}
		}
	}

	return TRUE;
}

void CTxADODBMgr::ReleaseDBMgr()
{
	if (m_spDBMgr)
	{
		delete m_spDBMgr;
		m_spDBMgr = NULL;
	}
	
}

_Connection* CTxADODBMgr::GetADOConnection( const DWORD dwDBKey /*= DBKEY_ANY*/ )
{
	_Connection* pRet = NULL;
	if (dwDBKey == DBKEY_ANY)
	{
		//return first db
		CAutoLock lock(&m_csdbLock);
		if (m_mapAdoConns.begin() != m_mapAdoConns.end())
		{
			ST_ADODBCONNINFO& infoTmp = m_mapAdoConns.begin()->second;
			if (infoTmp.pConn == NULL || infoTmp.pConn->GetState()== adStateClosed)
			{
				//reopen
				if (infoTmp.pConn)
				{
					infoTmp.pConn->Release();
				}
				infoTmp.pConn= ConnectDB(infoTmp.strConnStr);	
			}
			pRet = infoTmp.pConn;
		}
	}
	else
	{
		CAutoLock lock(&m_csdbLock);
		MAPADOCONNS::iterator itf = m_mapAdoConns.find(dwDBKey);
		if (itf != m_mapAdoConns.end())
		{
			ST_ADODBCONNINFO& infoTmp = itf->second;
			if (infoTmp.pConn == NULL || infoTmp.pConn->GetState()== adStateClosed)
			{
				//reopen
				if (infoTmp.pConn)
				{
					infoTmp.pConn->Release();
				}
				infoTmp.pConn= ConnectDB(infoTmp.strConnStr);
				pRet = infoTmp.pConn;
			}
		}
	}

	if (pRet)
	{
		pRet->AddRef();
	}
	else
	{
		ASSERT(FALSE);
	}

	return pRet;
}

void CTxADODBMgr::Clear()
{
	CAutoLock lock(&m_csdbLock);
	MAPADOCONNS::iterator itb = m_mapAdoConns.begin();
	for (; itb != m_mapAdoConns.end(); ++itb)
	{
		_Connection* pConn = itb->second.pConn;	
		if (pConn)
		{
			try
			{
				pConn->Close();
				pConn->Release();
			}
			catch(_com_error e)
			{
				ASSERT(FALSE);
			}
		}
	}

	m_mapAdoConns.clear();
}

CTxADODBMgr* CTxADODBMgr::GetADODBMgr()
{
	if (m_spDBMgr == NULL)
	{
		m_spDBMgr = new CTxADODBMgr;
	}

	return m_spDBMgr;
}

_Connection* CTxADODBMgr::ConnectDB( LPCTSTR szConnStr )
{
	CoInitialize(NULL);

	_Connection* pRetConn = NULL;
	try{
		_ConnectionPtr apConnection;
		HRESULT hr = apConnection.CreateInstance(__uuidof(Connection));
		ASSERT(hr == S_OK);
		if (apConnection)
		{
			apConnection->Open(szConnStr, "", "", adOpenUnspecified);
			pRetConn = apConnection;
			pRetConn->AddRef();
		}
		else
		{
			ASSERT(FALSE);
			CFWriteLog(0, TEXT("open db error:%d, %s"),GetLastError(), szConnStr);
		}
	}
	catch(_com_error e){
		CString str((TCHAR*)e.Description());
		CFWriteLog(0, TEXT("open db error:%s, %s"),str, szConnStr);
	}

	return pRetConn;

}

