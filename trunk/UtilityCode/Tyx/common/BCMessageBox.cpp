// MessageBox.cpp : implementation file
//

#include "stdafx.h"
#include "BCMessageBox.h"
#include "global.h"
#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HWND CMessageBox::m_hWndLast = NULL;
CMutex g_MsgBoxMutex(FALSE, TEXT("{BBB9CB83-F1F0-4095-9563-93E2613E4E00}"), NULL);
/////////////////////////////////////////////////////////////////////////////
// CMessageBox dialog
IMPLEMENT_DYNAMIC(CMessageBox, CDlgCommonContainer)

CMessageBox::CMessageBox(LPCTSTR szMsg,UINT unType,DWORD dwWaitTime,CWnd* pParent /*=NULL*/)
	: CDlgCommonContainer(CMessageBox::IDD, pParent)
{
	m_dwWaitTime = dwWaitTime;
	m_unType = unType;
	//{{AFX_DATA_INIT(CMessageBox)
	m_strPrompt = szMsg;
	m_bCenterScreen = FALSE;
	m_pImgCurrIcn = &m_imgInfoIcon;
	m_crfTitleClr =RGB(0,0,204);
	m_bTopMost = TRUE;
	m_static.m_nTextStyle = DT_EDITCONTROL|DT_WORDBREAK|DT_NOPREFIX;
	SetDlgType(TYPE_CLOSE);

	if (pParent == NULL)
	{
		g_log.Print("no parent dlg found, msg = %s", szMsg);
	}
	//}}AFX_DATA_INIT
}


void CMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDlgCommonContainer::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessageBox)
	DDX_Control(pDX, IDC_STATIC_ICON, m_staticIcon);
	DDX_Control(pDX, IDC_STATIC_MSG, m_static);
	DDX_Text(pDX, IDC_STATIC_MSG, m_strPrompt);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessageBox, CDlgCommonContainer)
	//{{AFX_MSG_MAP(CMessageBox)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageBox message handlers

BOOL CMessageBox::OnInitDialog() 
{
	if (m_hWndLast)
	{
		::SendMessage(m_hWndLast, WM_CLOSE, 0, 0);
		m_hWndLast = NULL;
	}
	m_btnOK.LoadBitmaps(button_common, FALSE);
	m_btnCancel.LoadBitmaps(button_common, FALSE);
	m_imgErrIcon.LoadBitmap(DlgMsg_Icn_Err, 10,20);
	m_imgInfoIcon.LoadBitmap(DlgMsg_Icn_Info, 10,20);
	m_imgQAIcon.LoadBitmap(DlgMsg_Icn_QA, 10,20);

	CDlgCommonContainer::OnInitDialog();
	
	//m_static.SetGradientText();

	//set msg is showing
	g_globalInfo.bMsgOnShow = TRUE;
	// TODO: Add extra initialization here
	m_hWndLast = this->GetSafeHwnd();

	m_btnOK.SubclassDlgItem(IDOK,this);
	m_btnOK.ShowWindow( SW_HIDE );

	m_btnCancel.SubclassDlgItem(IDCANCEL,this);
	m_btnCancel.ShowWindow( SW_HIDE );

// 	CRect rcBtnSize(0,0, BTN_COMMON2_WIDTH, BTN_COMMON2_HEIGTH);
// 	m_btnOK.MoveWindow(rcBtnSize);
// 	m_btnCancel.MoveWindow(rcBtnSize);
	
	SetIcon();
	m_staticIcon.ShowWindow(SW_HIDE);

	m_static.SetWindowText( m_strPrompt );
	//m_static.SetHDStaticClr(RGB(255,255,255));
	CRect rcWnd (0,0, 388, 252);
// 	GetWindowRect(&rcWnd);

	CRect rcTextOld, rcTextNew;
	//m_static.GetWindowRect(&rcStcRect);
	rcTextOld.left = m_imgInfoIcon.Width() +40;
	rcTextOld.right = rcWnd.right- 40;
	rcTextOld.top = 100;
	rcTextOld.bottom = rcWnd.Height() - 100;
	//ScreenToClient(&rcTextOld);
	rcTextNew = rcTextOld;

	//计算文本控件的位置
// 	CDC *pDC = GetDC();
// 	ASSERT(pDC != NULL);
// 	CFont * pFont  = GetFont();
// 	if (pFont == NULL)
// 		pFont = &g_fontStc;
// 	CFont *pFontOld = pDC->SelectObject(pFont);
// 
// 	pDC->DrawText(m_strPrompt, &rcTextNew, DT_CALCRECT | DT_WORDBREAK);
// 	if (rcTextNew.Height() > rcTextOld.Height())
// 	{
// 		rcTextNew.right += 500;
// 		pDC->DrawText(m_strPrompt, &rcTextNew, DT_CALCRECT | DT_WORDBREAK);
// 	}

	//调整消息框大小
	rcWnd.right += (rcTextNew.right - rcTextOld.right);
	MoveWindow(rcWnd);
	//调整静态文本大小
	m_static.MoveWindow(rcTextNew);
	
// 	pDC->SelectObject(pFontOld);
// 	ReleaseDC(pDC);
	

	//调整按钮位置
	CRect rcClient;
	GetClientRect(&rcClient);
	CRect rcBtn;
	CSize szBtn;

	UINT unBtnType = m_unType&0x0f;

#define  BOMBTN_OFFSET 3
	if( unBtnType==MB_OKCANCEL ){

		m_btnOK.GetWindowRect(&rcBtn);
		szBtn = rcBtn.Size();
		rcBtn.right = rcClient.Width()/2 - 30;
		rcBtn.left = rcBtn.right - szBtn.cx;
		rcBtn.bottom = rcClient.bottom - BOMBTN_OFFSET;
		rcBtn.top = rcBtn.bottom - szBtn.cy; 
		m_btnOK.MoveWindow(&rcBtn);
		m_btnOK.ShowWindow(SW_SHOW);

		m_btnCancel.GetWindowRect(&rcBtn);
		szBtn = rcBtn.Size();
		rcBtn.left = rcClient.Width()/2 + 30;
		rcBtn.right = rcBtn.left + szBtn.cx;
		rcBtn.bottom = rcClient.bottom - BOMBTN_OFFSET;
		rcBtn.top = rcBtn.bottom - szBtn.cy; 
		m_btnCancel.MoveWindow(&rcBtn);
		m_btnCancel.ShowWindow(SW_SHOW);
	}else{
		m_btnOK.GetWindowRect(&rcBtn);
		szBtn = rcBtn.Size();
		rcBtn.left = (rcClient.Width() - szBtn.cx)/2;
		rcBtn.right = rcBtn.left + szBtn.cx;
		rcBtn.bottom = rcClient.bottom - BOMBTN_OFFSET;
		rcBtn.top = rcBtn.bottom - szBtn.cy; 
		m_btnOK.MoveWindow(&rcBtn);
		m_btnOK.ShowWindow(SW_SHOW);
	}

	CenterWindow();
	if (m_bCenterScreen)
	{
		int nCx =  GetSystemMetrics(SM_CXSCREEN);
		int nCY=  GetSystemMetrics(SM_CYSCREEN);
		CRect rcWnd;
		GetWindowRect(rcWnd);
		rcWnd.MoveToXY((nCx - rcWnd.Width())/2,( nCY - rcWnd.Height())/2);
		MoveWindow(rcWnd);
	}
	ReLayout();
	SetForegroundWindow();

	if (m_bTopMost)
	{
		SetWindowPos(&wndTopMost, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE);
	}
	SetActiveWindow();

	CDlgMovedBase::ReLayout();

	if( m_dwWaitTime != INFINITE )
		SetTimer( 1000,m_dwWaitTime,NULL );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMessageBox::SetIcon()
{
	UINT unIconType = m_unType&0x00f0;
	if( unIconType==MB_ICONEXCLAMATION )
	{
		m_hIcon = LoadIcon( NULL,IDI_EXCLAMATION );
		m_pImgCurrIcn = &m_imgErrIcon;
	}
	else
	{
		if( unIconType==MB_ICONQUESTION )
		{
			m_hIcon = LoadIcon( NULL,IDI_QUESTION );
			m_pImgCurrIcn = &m_imgQAIcon;
		}
		else
		{
			m_hIcon = LoadIcon( NULL,IDI_INFORMATION );
			m_pImgCurrIcn = &m_imgInfoIcon;
		}
	}

	m_staticIcon.SetIcon( m_hIcon );
}

void CMessageBox::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CDlgCommonContainer::OnTimer(nIDEvent);

	if( nIDEvent == 1000 ){
		KillTimer( 1000 );
		OnCancel();
	}
}

void CMessageBox::CenterScreen()
{
	m_bCenterScreen = TRUE;
}

CMessageBox::~CMessageBox( void )
{
	g_globalInfo.bMsgOnShow = FALSE;
}
BOOL CMessageBox::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//  	CRect rcThis;
//  	GetClientRect(rcThis);
// 	//pDC->FillSolidRect(rcThis, RGB(0,0,0));
// 	m_imgBk.Render(pDC, rcThis.TopLeft(), rcThis.Size());
// 	
// 	CPoint ptIcn(20, 70);
// 	m_pImgCurrIcn->Render(pDC, ptIcn, CSize(m_pImgCurrIcn->Width(), m_pImgCurrIcn->Height()));
// 
// 	CRgn rgn;
// 	rgn.CreateRoundRectRgn(rcThis.left, rcThis.top, rcThis.right,rcThis.bottom, 5, 5);
// 	CBrush brush;
// 	brush.CreateSolidBrush(RGB(92,101,115));
// 	pDC->FrameRgn(&rgn, &brush, 1,1);
// 	brush.DeleteObject();
	//dc.SelectStockObject(BLACK_PEN);
// 	CPaintDC dc(this);
// 	dc.SetTextColor(m_crfTitleClr);
// 	CRect rcTitle(10,5, rcThis.right -20, 50);
// 	//draw default
// 	dc.DrawText(g_globalInfo.commonStr.strEZWorking, -1,rcTitle,DT_EDITCONTROL|DT_WORDBREAK|DT_END_ELLIPSIS|DT_LEFT|DT_TOP|DT_NOPREFIX);

	//return TRUE;
	return CDlgCommonContainer::OnEraseBkgnd(pDC);
}

HBRUSH CMessageBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDlgCommonContainer::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if( nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CMessageBox::OnSize(UINT nType, int cx, int cy)
{
	CDlgCommonContainer::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	ReLayout();
}

void CMessageBox::ReLayout()
{
	if (!m_static.GetSafeHwnd())
	{
		return;
	}

	CRect rcThis;
	GetClientRect(rcThis);
	CRgn rgn;
	rgn.CreateRoundRectRgn(rcThis.left, rcThis.top, rcThis.right,rcThis.bottom, 5, 5);
	SetWindowRgn(rgn, TRUE);
}
void CMessageBox::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_hWndLast = NULL;
	CDlgCommonContainer::PostNcDestroy();
}

void CMessageBox::SetSpecialHeader( CString& strSpecialIn )
{
	m_static.SetSpecialHeader(strSpecialIn);
}

void CMessageBox::WaitForLastBoxClose( DWORD dwTimeOut /*= 5000*/ )
{
	g_MsgBoxMutex.Lock(5000);
	int nTimeCount = 0;
	while(CMessageBox::m_hWndLast != NULL)
	{
		Sleep(500);
		nTimeCount += 500;
		if (dwTimeOut < nTimeCount)
		{
			break;
		}
	}
	g_MsgBoxMutex.Unlock();
}

BOOL CMessageBox::IsShowing()
{
	return m_hWndLast != NULL;
}	
