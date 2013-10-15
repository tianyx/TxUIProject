/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	ClipImportMonitorDlg.cpp
	file base:	ClipImportMonitorDlg
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
// ClipImportMonitorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ClipImportMonitor.h"
#include "ClipImportMonitorDlg.h"
#include "TrCommonInfoMgr.h"
#include "UxTheme.h"
#include "GDIDrawFunc.h"
#include "TxFontLoader.h"
#include "MonDataMgr.h"
#pragma comment(lib, "UxTheme.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  IDTIMER_CURRENTTIME 1200


#define ID_BTN_HIDETITLE 8001
int g_nAlertFadeStep = 15;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDlgMovedBase
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDlgMovedBase(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgMovedBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDlgMovedBase)
END_MESSAGE_MAP()


// CClipImportMonitorDlg 对话框




CClipImportMonitorDlg::CClipImportMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDlgMovedBase(CClipImportMonitorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pImgBk = NULL;
	m_nTitleHeight = 0;
	m_strTitle = TEXT("全台网素材监控系统");
	m_nAlertType = FALSE;
	m_bmpCache.CreateBitmap(2000, 2000, 1, 32, NULL);
	m_rcAlert.SetRectEmpty();
	m_rcTime.SetRectEmpty();
	int nFont = g_FontLoader.AddFont(g_globalInfo.captionSetting.strFontFaceTime, g_globalInfo.captionSetting.nFontHeightTime, TRUE);
	m_pTimerFont = g_FontLoader.GetFont(nFont);
	m_pTitleFont = g_FontLoader.GetFont(TXFONTFOR_CAPTION);
 

}

void CClipImportMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgMovedBase::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClipImportMonitorDlg, CDlgMovedBase)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCACTIVATE()
	ON_WM_NCCALCSIZE()
	ON_BN_CLICKED(SC_CLOSE, &CClipImportMonitorDlg::OnBnClickSysClose)
	ON_BN_CLICKED(SC_MINIMIZE, &CClipImportMonitorDlg::OnBnClickSysMin)
	ON_BN_CLICKED(SC_MAXIMIZE, &CClipImportMonitorDlg::OnBnClickSysMax)
	ON_BN_CLICKED(SC_RESTORE, &CClipImportMonitorDlg::OnBnClickSysRestore)
	ON_BN_CLICKED(ID_BTN_HIDETITLE, &CClipImportMonitorDlg::OnBnClickHideTitle)

	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CClipImportMonitorDlg 消息处理程序

BOOL CClipImportMonitorDlg::OnInitDialog()
{
	::SetWindowTheme(this->GetSafeHwnd(), L"",L"");
	SetWindowText(m_strTitle);
	m_btnMin.LoadBitmap("skins\\sysbtnmin.bmp", TXBMP_STRETCH_NONE);
	m_btnMax.LoadBitmap("skins\\sysbtnmax.bmp", TXBMP_STRETCH_NONE);
	m_btnClose.LoadBitmap("skins\\sysbtnclose.bmp", TXBMP_STRETCH_NONE);
	m_btnRestore.LoadBitmap("skins\\sysbtnrestore.bmp", TXBMP_STRETCH_NONE);
	m_btnHideTitle.LoadBitmap("skins\\btnHideAlert.bmp", TXBMP_STRETCH_NONE);
	m_btnHideTitle.AddBitmap("skins\\btnShowAlert.bmp");
	m_pImgBk = CTxImageLoader::LoadBitmap("skins\\titlebk.bmp");
	m_pBmpAlert = CTxImageLoader::LoadBitmap("skins\\Alert.bmp");
	ASSERT(m_pBmpAlert != NULL);
	ASSERT(m_pImgBk != NULL);
	m_szAlertBmpSize.cx = m_pBmpAlert->GetWidth();
	m_szAlertBmpSize.cy = m_pBmpAlert->GetHeight();
	m_nAlertWidth = m_szAlertBmpSize.cx / 7;
	m_nTitleHeight = m_pImgBk->GetHeight();

	//load slash

	m_ctlSlash.InitSlash(g_nAlertFadeStep*3, m_nAlertWidth, m_szAlertBmpSize.cy);
	CRect rcBegin(0,0, m_nAlertWidth, m_szAlertBmpSize.cy);
	CRect rcDes(rcBegin);
	rcDes.MoveToX(m_nAlertWidth* 6);
	m_ctlSlash.AddSlash(m_pBmpAlert, &rcBegin, &rcDes, g_nAlertFadeStep);

	rcBegin.MoveToX(m_nAlertWidth * 1);
	rcDes.MoveToX(m_nAlertWidth* 5);
	m_ctlSlash.AddSlash(m_pBmpAlert, &rcBegin, &rcDes, g_nAlertFadeStep);

	rcBegin.MoveToX(m_nAlertWidth * 2);
	rcDes.MoveToX(m_nAlertWidth* 5);
	m_ctlSlash.AddSlash(m_pBmpAlert, &rcBegin, &rcDes, g_nAlertFadeStep);

	
	CDlgMovedBase::OnInitDialog();
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_btnClose.Create(NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,29,24), this, SC_CLOSE);
	m_btnMin.Create(NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,29,24), this, SC_MINIMIZE);
	m_btnMax.Create(NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,29,24), this, SC_MAXIMIZE);
	m_btnRestore.Create(NULL, WS_CHILD, CRect(0,0,29,24), this, SC_RESTORE);
	m_btnHideTitle.Create(NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,29,24), this, ID_BTN_HIDETITLE);
	// TODO: 在此添加额外的初始化代码
	m_pView = new CClipMonView;
	m_pView->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,  CRect(0,0, 500,500), this, 111 );
	SetWindowPos(NULL,0,0, 1024,768, SWP_NOMOVE);
	CenterWindow();
	Relayout();
	SetTimer(IDTIMER_CURRENTTIME, 1000, 0);
	m_ctlSlash.StartLoop(this, m_rcAlert, ALERTTYPE_FLASH_SAFE, g_nAlertFadeStep, 100);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CClipImportMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if (nID == SC_MAXIMIZE)
		{
			m_btnMax.ShowWindow(SW_HIDE);
			m_btnRestore.ShowWindow(SW_SHOW);
		}
		else if (nID == SC_RESTORE)
		{
			if (!IsIconic())
			{
				m_btnMax.ShowWindow(SW_SHOW);
				m_btnRestore.ShowWindow(SW_HIDE);
			}
		}
		CDlgMovedBase::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CClipImportMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDlgMovedBase::OnPaint();

	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CClipImportMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CClipImportMonitorDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	if(m_pView)
	{
		delete m_pView;
		m_pView = NULL;
	}
	CDlgMovedBase::PostNcDestroy();
}

void CClipImportMonitorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDlgMovedBase::OnSize(nType, cx, cy);
	Relayout();
	Invalidate();
	// TODO: 在此处添加消息处理程序代码
}

void CClipImportMonitorDlg::Relayout()
{
	if (!m_btnClose.GetSafeHwnd())
	{
		return;
	}
	CRect rcThis;
	GetClientRect(rcThis);
	if (rcThis.IsRectEmpty())
	{
		m_rcDragRect.SetRectEmpty();
		m_rcTime.SetRectEmpty();
		m_rcAlert.SetRectEmpty();
		m_ctlSlash.SetRect(m_rcAlert);
		return;
	}

	CRect rcCap(rcThis);
	rcCap.bottom = rcCap.top + m_nTitleHeight;
	m_rcDragRect = rcCap;
	m_rcDragRect.bottom = m_rcDragRect.top + 30;

	m_rcAlert = CRect(0,0, m_nAlertWidth, m_szAlertBmpSize.cy);
	m_rcAlert.MoveToXY(rcThis.right - 8 - m_nAlertWidth, 43);
	m_ctlSlash.SetRect(m_rcAlert);


	m_rcTime = rcCap;
	m_rcTime.right = m_rcAlert.left -5;
	m_rcTime.DeflateRect(10, 33, 0,2);
	m_rcTime.right -= 100;


	CRect rcBtnClose;
	m_btnClose.GetClientRect(rcBtnClose);
	rcBtnClose.MoveToXY( rcCap.right - rcBtnClose.Width() -5,  6);
	m_btnClose.MoveWindow(rcBtnClose);

	CRect rcBtnMax;
	m_btnMax.GetClientRect(rcBtnMax);
	rcBtnMax.MoveToXY( rcBtnClose.left - rcBtnMax.Width(), rcBtnClose.top);
	m_btnMax.MoveWindow(rcBtnMax);
	m_btnRestore.MoveWindow(rcBtnMax);

	CRect rcBtnMin;
	m_btnMin.GetClientRect(rcBtnMin);
	rcBtnMin.MoveToXY(rcBtnMax.left - rcBtnMin.Width() , rcBtnMax.top);
	m_btnMin.MoveWindow(rcBtnMin);

	CRect rcBtnHide;
	m_btnHideTitle.GetClientRect(rcBtnHide);
	rcBtnHide.MoveToXY(rcBtnMin.left - rcBtnHide.Width(), rcBtnMin.top);
	m_btnHideTitle.MoveWindow(rcBtnHide);

	m_btnMin.Invalidate(TRUE);
	m_btnMax.Invalidate(TRUE);
	m_btnRestore.Invalidate(TRUE);
	m_btnClose.Invalidate(TRUE);
	m_btnHideTitle.Invalidate(TRUE);

	CRect rcView(rcThis);
	if (m_btnHideTitle.GetCurrImgIdx() == 1)
	{
		rcView.top += 33;
		m_rcTime.SetRectEmpty();
		m_rcAlert.SetRectEmpty();
	}
	else
	{
		rcView.top = rcCap.bottom;
	}
	m_pView->MoveWindow(rcView);

}

void CClipImportMonitorDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 1024;
	lpMMI->ptMinTrackSize.y = 600;
	CDlgMovedBase::OnGetMinMaxInfo(lpMMI);
}


BOOL CClipImportMonitorDlg::OnNcActivate(BOOL bActive)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	PostMessage(WM_NCPAINT, 0, 0);
	return TRUE;
	//return CDlgMovedBase::OnNcActivate(bActive);
}

void CClipImportMonitorDlg::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (bCalcValidRects)
	{
		// 		int nCap = GetSystemMetrics(SM_CYCAPTION);
		// 		int nCy = GetSystemMetrics(SM_CYSIZEFRAME);
		// 		int nCx = GetSystemMetrics(SM_CXSIZEFRAME);
		// 		lpncsp->rgrc[0].top += IM_BORDER_L_Y - nCy;   
		// 		lpncsp->rgrc[0].left += IM_BORDER_L_X - nCx;   
		// 		lpncsp->rgrc[0].bottom -= IM_BORDER_R_Y - nCy;   
		// 		lpncsp->rgrc[0].right -= IM_BORDER_R_X - nCx;   
		lpncsp->rgrc[0].top +=1;
		lpncsp->rgrc[0].left +=1;
		lpncsp->rgrc[0].right -=2;
		lpncsp->rgrc[0].bottom -=2;
	}
	//CDlgMovedBase::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CClipImportMonitorDlg::OnBnClickSysClose()
{
	//SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
	OnOK();
}

void CClipImportMonitorDlg::OnBnClickSysMin()
{
	AnimateWindow(200, AW_BLEND|AW_HIDE);
	PostMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void CClipImportMonitorDlg::OnBnClickSysMax()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}

void CClipImportMonitorDlg::OnBnClickSysRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
BOOL CClipImportMonitorDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	HGDIOBJ oldObj = memDC.SelectObject(&m_bmpCache);

	CRect rcThis;
	GetClientRect(rcThis);
	CRect rcTitle(rcThis);
	rcTitle.bottom = rcTitle.top + m_nTitleHeight;
	Graphics gc(memDC.GetSafeHdc());

	if (!m_rcAlert.IsRectEmpty())
	{
		Rect rAlert = CRect2Rect(m_rcAlert);
		gc.ExcludeClip(rAlert);
	}
	if (!m_pImgBk)
	{
		memDC.FillSolidRect(rcThis, RGB(0,0,0));
	}
	else
	{
		GPDrawStretchImage(gc, m_pImgBk, rcTitle, NULL, TXBMP_STRETCH_MID_LR);
	}

	//draw icon
	HICON hico =  GetIcon(FALSE);
	memDC.DrawIcon(4,4, hico);

	//draw title
	CRect rcCaption(rcTitle);
	rcCaption.bottom = rcCaption.top +33;
	rcCaption.right = rcCaption.left + 300;
	rcCaption.DeflateRect(35,4,0,0);
	Gdiplus::StringFormat fmt;
	fmt.SetAlignment(StringAlignmentNear);
	fmt.SetLineAlignment(StringAlignmentCenter);
	fmt.SetTrimming(StringTrimmingEllipsisCharacter);
	fmt.SetFormatFlags(StringFormatFlagsLineLimit);
	GPDrawShadowText(gc, m_strTitle, rcCaption, *m_pTitleFont, g_globalInfo.gSetting.dwBrush, g_globalInfo.gSetting.dwPen, 0, 0xc0646464, 1,1, &fmt);
	
	//draw timer
	if (!m_rcTime.IsRectEmpty())
	{
		CString strTime;
		CTime tm(time(NULL));
		strTime.Format(TEXT("当前时间: %4d-%02d-%02d %02d:%02d:%02d"), tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
		GPDrawShadowText(gc, strTime, m_rcTime, *m_pTimerFont,  g_globalInfo.captionSetting.dwClrBurshTime,  g_globalInfo.captionSetting.dwClrPenTime, 0, 0xff646464, 2,2, &fmt);
	}


	//draw alert
	
/*
	if (!(m_nAlertType > 6 || m_nAlertType < 0))
	{
		CRect rcImg(0,0, m_nAlertWidth, m_szAlertBmpSize.cy);
		rcImg.OffsetRect(m_nAlertWidth* m_nAlertType, 0);
		if (m_nAlertType == ALERTTYPE_FLASH_SAFE)
		{

		}
		else if (m_nAlertType == ALERTTYPE_FLASH_UNSAFE)
		{

		}
		else if (m_nAlertType == ALERTTYPE_FLASH_ERROR)
		{
		}
		GPDrawStretchImage(gc, m_pBmpAlert, m_rcAlert, &rcImg, TXBMP_STRETCH_NONE);
	}*/
	

	pDC->BitBlt(rcThis.left, rcThis.top, rcThis.Width(), rcThis.Height(), &memDC, 0,0, SRCCOPY);
	memDC.SelectObject(oldObj);

	return TRUE;
	return CDlgMovedBase::OnEraseBkgnd(pDC);
}

void CClipImportMonitorDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_rcDragRect.PtInRect(point))
	{
		if (m_btnMax.IsWindowVisible())
		{
			PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
		else
		{
			PostMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		}
	}
	CDlgMovedBase::OnLButtonDblClk(nFlags, point);
}

void CClipImportMonitorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == IDTIMER_CURRENTTIME)
	{
		if (!m_rcTime.IsRectEmpty())
		{
			InvalidateRect(m_rcTime);
		}

		static int nsAltertCount =0;
		nsAltertCount++;
		if (nsAltertCount == g_monDataMgr.m_nAlertCheckInterval)
		{
			CheckAlert();
			nsAltertCount = 0;
		}
	}


	CDlgMovedBase::OnTimer(nIDEvent);
}


void CClipImportMonitorDlg::OnDestroy()
{
	m_ctlSlash.EndLoop();
	CDlgMovedBase::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CClipImportMonitorDlg::CheckAlert()
{
	m_nAlertType = g_monDataMgr.CheckAlert();	
	m_ctlSlash.SetLoopPos(m_nAlertType*g_nAlertFadeStep, g_nAlertFadeStep);
	m_pView->ReDrawBkBuffer();
}

LRESULT CClipImportMonitorDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == MSG_MONDATACHANGED)
	{
		CheckAlert();
	}
	return CDlgMovedBase::WindowProc(message, wParam, lParam);
}

void CClipImportMonitorDlg::OnBnClickHideTitle()
{
	int nIdx = m_btnHideTitle.GetCurrImgIdx();
	m_btnHideTitle.SetCurrImgIdx(nIdx == 0? 1:0);
	Relayout();
}
