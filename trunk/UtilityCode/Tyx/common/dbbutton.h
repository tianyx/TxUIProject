// DBButton.h: interface for the CSCBButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__DBARG_H__)
#define __DBARG_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


/////////////////////////////////////////////////////////////////////////
// DBButton class help

//如何使用该类:
//1. 在父窗口OnPaint或OnNcPoint中调用DrawButton
//2. 在父窗口PreTranslateMessage函数中调用RelayEvent
class DBButton
{
public:
	void EnableBtn(BOOL bEnable = TRUE);
	CSize GetImageSize();

	DBButton();
	virtual ~DBButton();
	
	void RelayEvent(const MSG *pMsg);
	
	void Create(const CRect &rect, CWnd *pWndParent, UINT nID,
			UINT nIDBmpNormal, UINT nIDBmpSel, UINT nIDBmpHover,
			UINT nIDBmpDisable);

	void Create(const CRect &rect, CWnd *pWndParent, UINT nID,
			LPCTSTR lpszBmpNormal, LPCTSTR lpszBmpSel, LPCTSTR lpszBmpHover,
			LPCTSTR lpszBmpDisable);

	void Create(const CRect &rect, CWnd *pWndParent, UINT nID,
		LPCTSTR lpszBitmaps);

	void SetBitmaps(HBITMAP hBmpNormal,
		HBITMAP hBmpSel, HBITMAP hBmpHover, 
		HBITMAP hBmpDisable);

	void Move(CPoint ptTo)
	{ 
		m_rect.OffsetRect(ptTo - m_rect.TopLeft());
	}
    CRect GetRect() { ASSERT(m_nID != 0); return m_rect; }

    virtual void DrawButton(CPoint ptOrg);
    virtual void DrawButton(CPoint ptOrg, CDC *pDC);


protected:
	void Init();
	
	CBitmap m_bmps[4];
	UINT m_nID;
	BOOL m_bPushed;
	enum 
	{	
		stateNormal,
		stateSel,
		stateHover,
		stateDisable
	} m_state;

private:
	void DrawButton();

private:
	
	CRect m_rect;
	CWnd * m_pWndParent;
	static DBButton* m_pBtnCapture;
	CPoint m_ptOrgLast; //最近一次外部调用绘制函数的绘制原点
};

#endif
