/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxListCtrl.h
	file base:	TxListCtrl
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once

#include "TxScrollWnd.h"
#include "TxListHeader.h"

// CTxListCtrl

class CTxListCtrl : public CTxScrollWnd
{
	DECLARE_DYNAMIC(CTxListCtrl)

public:
	CTxListCtrl();
	virtual ~CTxListCtrl();

	CTxListHeader& GetListHeader(){return m_headerCtrl;}

	virtual void TxGetClientRect(CRect& rcClient);
	virtual void SetScrollViewSize(CSize szView);
protected:
	CTxListHeader m_headerCtrl;
protected:
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


