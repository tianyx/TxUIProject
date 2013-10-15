#if !defined(AFX_DBPROGRESSCTRL_H__57B9FD0B_AC42_48BE_84E4_CDEB104A24D4__INCLUDED_)
#define AFX_DBPROGRESSCTRL_H__57B9FD0B_AC42_48BE_84E4_CDEB104A24D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DBProgressCtrl.h : header file
//

#include "DBStretchImage.h"

/////////////////////////////////////////////////////////////////////////////
// CDBProgressCtrl window

class CDBProgressCtrl : public CProgressCtrl
{
// Construction
public:
	CDBProgressCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDBProgressCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void LoadImages(LPCTSTR lpszBmpBk, LPCTSTR lpszBmpPadding = NULL);
	virtual ~CDBProgressCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDBProgressCtrl)
	afx_msg void OnPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void DrawPadding(CDC *pDC, CRect rect);
	DBStretchImage m_imagePadding;
	DBStretchImage m_imageBk;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBPROGRESSCTRL_H__57B9FD0B_AC42_48BE_84E4_CDEB104A24D4__INCLUDED_)
