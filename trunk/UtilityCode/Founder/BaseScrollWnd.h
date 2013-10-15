#pragma once


// CBaseScrollWnd

class CBaseScrollWnd : public CWnd
{
	DECLARE_DYNAMIC(CBaseScrollWnd)

public:
	CBaseScrollWnd();
	virtual ~CBaseScrollWnd();

public:
	BOOL		Create( LPCTSTR lpszWindowName, CWnd *pParent, CRect rc, UINT nID, DWORD dwStyle);
	virtual	void	OnDraw(CDC *pDC, CRect rectClipBox = CRect(0,0,0,0)  ) ;	

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};


