#pragma once
#include "TxLogManager.h"
#include "TxADOConn.h"
#include <map>
using namespace std;
typedef map<DWORD, DWORD> MAPLOGDB;

class CTxDBLogManager: public CTxLogManager
{
private:
	CTxDBLogManager(void);
	virtual ~CTxDBLogManager(void);

public:
	DWORD AddNewDBConn(DWORD dwLogKeyIn,DWORD dwDBKeyIn, CString& strDBConn);
	virtual void CheckLog();
private:
	MAPLOGDB m_mapdbs;
	static CTxDBLogManager* m_spDBLogMgr;
public:
	static CTxDBLogManager* GetTxDBLogMgr();
	//release the logmanager
	static  void ReleaseTxDBLogMgr();

};
