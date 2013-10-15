#pragma once

#include "TxObjectBase.h"
#include "TestTxChildObj.h"
// CTestTxObj

class CTestTxObj : public CTxObjectBase
{
	DECLARE_DYNAMIC(CTestTxObj)

public:
	CTestTxObj();
	virtual ~CTestTxObj();

	CTestTxChildObj m_childObj;
	BOOL m_bMouseEnter;

virtual void Draw( CDC* pDC );
virtual HRESULT DefaultMsgProc(UINT nMsg, WPARAM wparam, LPARAM lparam);

};


