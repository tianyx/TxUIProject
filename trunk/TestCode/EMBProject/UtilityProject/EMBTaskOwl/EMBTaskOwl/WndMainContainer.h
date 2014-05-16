#pragma once


// CWndMainContainer


class CWndMainContainer : public CWnd
{
	DECLARE_DYNAMIC(CWndMainContainer)

public:
	CWndMainContainer();
	virtual ~CWndMainContainer();

	CSplitterWnd    m_wndView;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


