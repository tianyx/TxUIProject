#pragma once
#include "TxWndContainer.h"

#include "TestTxObj.h"
#include "TestScrollView.h"
// CTestContainer

class CTestContainer : public CTxWndContainer
{
	DECLARE_DYNAMIC(CTestContainer)

public:
	CTestContainer();
	virtual ~CTestContainer();

	CTestScrollView m_obj;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


