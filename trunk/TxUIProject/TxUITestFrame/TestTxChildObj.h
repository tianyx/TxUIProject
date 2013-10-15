#pragma once

#include "TxObjectBase.h"
// CTestTxChildObj ÃüÁîÄ¿±ê

class CTestTxChildObj : public CTxObjectBase
{

	DECLARE_DYNAMIC(CTestTxChildObj)
public:
	CTestTxChildObj();
	virtual ~CTestTxChildObj();


	virtual void Draw( CDC* pDC );
	virtual HRESULT DefaultMsgProc(UINT nMsg, WPARAM wparam, LPARAM lparam);

};


