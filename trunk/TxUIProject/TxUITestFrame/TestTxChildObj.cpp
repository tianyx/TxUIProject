// TestTxChildObj.cpp : 实现文件
//

#include "stdafx.h"
#include "TxUITestFrame.h"
#include "TestTxChildObj.h"


// CTestTxChildObj
IMPLEMENT_DYNAMIC(CTestTxChildObj, CTxObjectBase)

CTestTxChildObj::CTestTxChildObj()
{
}

CTestTxChildObj::~CTestTxChildObj()
{
}

void CTestTxChildObj::Draw( CDC* pDC )
{
	if (!IsVisable())
	{
		return;
	}
	CRect rcThis =GetClientRect();

	COLORREF clrBk = RGB(0,0,100);
	if (IsInState(TS_MOUSEOVER))
	{
		clrBk = RGB(0,100,0);
	}

	pDC->FillSolidRect(rcThis, clrBk);

}

HRESULT CTestTxChildObj::DefaultMsgProc( UINT nMsg, WPARAM wparam, LPARAM lparam )
{
	return TRUE;
}


// CTestTxChildObj 成员函数
