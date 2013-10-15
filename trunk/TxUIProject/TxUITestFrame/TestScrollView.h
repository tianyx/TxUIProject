#pragma once

#include "TxScrollView.h"
// CTestScrollView

class CTestScrollView : public CTxScrollView
{
	DECLARE_DYNAMIC(CTestScrollView)

public:
	CTestScrollView();
	virtual ~CTestScrollView();

	virtual void DrawView(CDC* pDC);
	//interface
	virtual HRESULT DefaultMsgProc(UINT nMsg, WPARAM wparam, LPARAM lparam);


};


