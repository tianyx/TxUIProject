#pragma once

class CDlg_PopupBase : public CDialogEx
{
public:
	CDlg_PopupBase(UINT nID, CWnd* pParent) : CDialogEx( nID, pParent  )
	{

	};
	virtual ~CDlg_PopupBase(){};
public:
	virtual HRESULT	GetDialogSize( CSize& size){ return E_NOTIMPL; };

};


// CPopupWnd

class CPopupWnd : public CWnd
{
	DECLARE_DYNAMIC(CPopupWnd)

public:
	static CPopupWnd* GetPopupWnd();

protected:
	CPopupWnd();

public:
	virtual ~CPopupWnd();

	HWND	m_hTargetWnd;
	CRect	m_rectTarget;
public:
	template<class T>
	HRESULT	CommonCreate( HWND hWnd, typename T* pDialog, UINT nAutoCloseTime = 3000 /*毫秒*/ )
	{
		REGISTER_FUNC( HRESULT	CPopupWnd::CommonCreate( CRect rect ) );
		HRESULT hr = E_FAIL;
		m_hTargetWnd = hWnd;
		CRect rect;
		

		try
		{
			VERIFY_TRUE( m_hTargetWnd && ::IsWindow(m_hTargetWnd));
			::GetWindowRect( m_hTargetWnd, &m_rectTarget );
			rect = m_rectTarget;
			CWnd* pWndFocus = GetFocus();
			CWnd* pWndForeground = CWnd::GetForegroundWindow();
			m_pDialog = pDialog;

			CString strClassName = ::AfxRegisterWndClass(CS_SAVEBITS, ::LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_BTNFACE + 1), NULL);

			
			DWORD dwStyleEx = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;

			dwStyleEx |= WS_EX_LAYERED;		

			m_bSetFocus = FALSE;
			
			CRect rectDummy(0, 0, 0, 0);

			VERIFY_TRUE( CWnd::CreateEx(dwStyleEx, strClassName, _T(""), WS_POPUP , rectDummy, NULL, 0));

			SetLayeredWindowAttributes( RGB(0,128,128), 240, LWA_COLORKEY | LWA_ALPHA );

			pDialog->Create( T::IDD, this);
			// 获取对话框的大小
			CSize sizeDlg = GetContentDlgSize( pDialog, T::IDD );
			CDlg_PopupBase *pDlgTemp = dynamic_cast<CDlg_PopupBase*>(pDialog);
			if ( pDlgTemp )
			{
				CSize sizeTmp;
				if ( SUCCEEDED( pDlgTemp->GetDialogSize( sizeTmp ) ) )
				{
					sizeDlg = sizeTmp;
				}
			}

			rectDummy = CRect(0, 0, sizeDlg.cx + m_marginInfo.nLeftMargin + m_marginInfo.nRightMargin, sizeDlg.cy + m_marginInfo.nBottomMargin + m_marginInfo.nTopMargin);

			pDialog->ModifyStyle( WS_POPUP | WS_BORDER | DS_MODALFRAME | WS_THICKFRAME , WS_CHILD );
			::SetWindowLong( pDialog->GetSafeHwnd(), GWL_STYLE, ::GetWindowLong(pDialog->GetSafeHwnd(), GWL_STYLE) & ~DS_MODALFRAME );
			pDialog->SetParent(this);

			
			pDialog->MoveWindow( m_marginInfo.nLeftMargin, m_marginInfo.nTopMargin, sizeDlg.cx , sizeDlg.cy );

			pDialog->ShowWindow( TRUE );

			// 得到 rect 的中心坐标
			int x0 = rect.left + rect.Width() / 2;
			int y0 = rect.top + rect.Height() / 2 - sizeDlg.cy - m_marginInfo.nTopMargin - m_marginInfo.nBottomMargin;
			//
			// 得到目标坐标
			switch( m_nMode)
			{
			case MODE_TOP_LEFT:
				x0 += m_offsetInfo.nTopLeftMode_offset;
				break;

			}

			SetWindowPos( &wndTopMost, x0, y0, sizeDlg.cx + 20, sizeDlg.cy + m_marginInfo.nTopMargin + m_marginInfo.nBottomMargin, SWP_NOACTIVATE | SWP_SHOWWINDOW );
			SetTimer( 1, nAutoCloseTime, NULL );
			SetTimer( 2, 50, NULL );

			if ( pWndForeground && pWndForeground->GetSafeHwnd() != NULL)
			{
				pWndForeground->SetForegroundWindow();
			}

			if ( pWndFocus && pWndFocus->GetSafeHwnd() != NULL)
			{
				pWndFocus->SetFocus();
			}
			m_bSetFocus = TRUE;

			hr = S_OK;
		}
		my_catch;

		return hr;
	}

protected:
	CSize		GetContentDlgSize( CDialog* pDlg, UINT nID );

protected:
	CDialog*	m_pDialog;
	BOOL		m_bSetFocus;
	struct		Margin_Info
	{
		Margin_Info()
		{
			nLeftMargin = 10;
			nTopMargin = 10;
			nRightMargin = 10;
			nBottomMargin = 25;
		}
		int		nLeftMargin;
		int		nTopMargin;
		int		nRightMargin;
		int		nBottomMargin;
	}m_marginInfo;

	struct		Offset_info
	{
		Offset_info()
		{
			nTopLeftMode_offset = -28;
		}

		int		nTopLeftMode_offset;

	}m_offsetInfo;

	int		m_nMode;
	enum	{ MODE_TOP_LEFT = 0};

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnNcDestroy();
};


