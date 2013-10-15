#if !defined(AFX_RESIZABLEPROPERTYSHEET_H__045C069C_F9F3_4741_8BB8_E4A3C3FF6952__INCLUDED_)
#define AFX_RESIZABLEPROPERTYSHEET_H__045C069C_F9F3_4741_8BB8_E4A3C3FF6952__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResizablePropertySheet.h : header file
// wang xin wei 2008.03.11
//

/////////////////////////////////////////////////////////////////////////////
// CResizablePropertySheet
/////////////////////////////////////////////////////////////////////////////

// 说明, 派生类可以改写(override)以下函数
//
// virtual HRESULT			InitControl();
// 派生类改写此函数，在属性框显示之前对如 TabCtrl, IDOK, IDCANNEL ,IDHELP, ID_APPLY_NOW 按钮
// 同时，派生类可在此函数中创建自定义控件
//
// virtual HRESULT			InitCustomControlPosData();
// 派生类改写此函数用于记录自定义控件的位置信息
//
// virtual HRESULT			RecalculatelayoutCustomControl();
// 派生类改写此函数，当属性框改变大小时，调整自定义控件的位置
////////////////////////////////////////////////////////////////////////////////


// 接口
// SetPageTitle(int nPageIndex, const CString& strPageTitle);
// SetPageTitle(CPropertyPage* pPage, const CString& strPageTitle);
// 设置属性页的标题
// 
class CResizablePropertySheet : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(CResizablePropertySheet)

protected:
	struct tagPositionData
	{
		tagPositionData()
		{
			nLeftMargin = 0;
			nRightMargin = 0;
			nTopMargin = 0;
			nBottomMargin = 0;
		}
		void RecalPosData( CRect& rectOutter, CRect &rectInner)
		{
			nLeftMargin = rectInner.left - rectOutter.left;
			nRightMargin = rectOutter.right - rectInner.right;
			nTopMargin = rectInner.top - rectOutter.top;
			nBottomMargin = rectOutter.bottom - rectInner.bottom;

		}

		int nLeftMargin;
		int nRightMargin;
		int nTopMargin;
		int nBottomMargin;
	};

	struct  tagSheetData
	{
		tagPositionData tab;
		tagPositionData page;
		tagPositionData btnOK;
		tagPositionData btnCancel;
		tagPositionData btnHelp;
		tagPositionData btnApplyNow;

	}m_SheetData;
// Construction
public:
	CResizablePropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CResizablePropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CResizablePropertySheet();
// Attributes
public:

// Operations
public:
	HRESULT					SetPageTitle(int nPageIndex, const CString& strPageTitle);
	HRESULT					SetPageTitle(CPropertyPage* pPage, const CString& strPageTitle);
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizablePropertySheet)
	public:
	virtual BOOL			OnInitDialog();
	protected:
	virtual BOOL			OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
protected:
	virtual HRESULT			InitControl();
	virtual HRESULT			InitCustomControlPosData();
	virtual HRESULT			Recalculatelayout();
	virtual HRESULT			RecalculatelayoutCustomControl();

protected:
	HRESULT					InitControlPosData();	
	//{{AFX_MSG(CResizablePropertySheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

/////////////////////////////////////////////////////////////////////////////
// CResizablePropertySheetEx

class CResizablePropertySheetEx : public CResizablePropertySheet
{
	DECLARE_DYNAMIC(CResizablePropertySheetEx)
		
		// Construction
public:
	CResizablePropertySheetEx(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CResizablePropertySheetEx(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	
	// Attributes
public:
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResizablePropertySheetEx)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	BOOL OnInitDialog();
	HRESULT InitControl();
	virtual ~CResizablePropertySheetEx();
	
protected:
	CButton		m_btnOK;
	CButton		m_btnCancel;
	CButton		m_btnApply;
	CButton		m_btnHelp;
	// Generated message map functions
protected:
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CResizablePropertySheetEx)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HRESULT CreateButton();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_RESIZABLEPROPERTYSHEET_H__045C069C_F9F3_4741_8BB8_E4A3C3FF6952__INCLUDED_)
