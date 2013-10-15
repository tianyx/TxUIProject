// DBFrame.cpp : implementation file
//

#include "stdafx.h"
#include "DBFrame.h"
#include "MemDC.h"
#include "SkinFiles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CFont g_fontBase;

//系统按钮提示字符串
//---------------------------
const CString strSysCloseBtnTip = TEXT("Close");
const CString strSysMaxBtnTip = TEXT("Maximize  window");
const CString strSysMinBtnTip = TEXT("Minimize window");
//---------------------------

/////////////////////////////////////////////////////////////////////////////
// CDBFrame

IMPLEMENT_DYNCREATE(CDBFrame, CFrameWnd)

CDBFrame::CDBFrame()
	:m_rcSysBorder(2, 1, 3, 5),
	m_szRoundCorner(2, 2),
	m_rcSysCloseBtn(20, 1, 51, 21),
	m_rcSysMaxBtn(51, 1, 92, 21),
	m_rcSysMinBtn(92, 1, 123, 21)
{
	m_bNcActivate = TRUE;
	
	//装载标题栏位图
	m_imageCaption.LoadBitmap(DBFrame_Caption, 10, 70);//激活时用
	m_imageCaptionNA.LoadBitmap(DBFrame_Caption, 10, 70);//不激活时用
	m_imageTitleBk.LoadBitmap(DBFrame_TitleBk, 50, 100);
	//装载边框线位图
	m_imageBorders[LEFT].LoadBitmap(DBFrame_SysBorderLeft, 10, 20, FALSE);
	m_imageBorders[RIGHT].LoadBitmap(DBFrame_SysBorderRight, 10, 20, FALSE);
	m_imageBorders[BOTTOM].LoadBitmap(DBFrame_SysBorderBottom, 10, 20);

	m_imageLeftHeader.LoadBitmap(TEXT("skins\\capture_left.bmp"),5,10);
}

CDBFrame::~CDBFrame()
{
}

/////////////////////////////////////////////////////////////////////////////
// CDBFrame message handlers

BEGIN_MESSAGE_MAP(CDBFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CDBFrame)
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
	ON_COMMAND(ID_SC_CLOSE, OnSCClose)
	ON_COMMAND(ID_SC_MAX, OnSCMax)
	ON_COMMAND(ID_SC_MIN, OnSCMin)
	ON_COMMAND(ID_SC_RESTORE, OnSCRestore)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_WM_NCACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDBFrame message handlers
	
//窗口尺寸改变时调用设置系统按钮位置
void CDBFrame::OnSetSysButtonPos()
{
	DWORD dwStyle = GetStyle();
	if (!(dwStyle & WS_SYSMENU))
		return;

	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//关闭按钮
	CRect rcSysCloseBtn(m_rcSysCloseBtn);
	rcSysCloseBtn.OffsetRect(rcWindow.Width()
		- m_rcSysCloseBtn.left*2 - m_rcSysCloseBtn.Width(),
		0);
	rcSysCloseBtn.OffsetRect(rcWindow.TopLeft());
	ScreenToClient(&rcSysCloseBtn);

	m_sysCloseBtn.Move(rcSysCloseBtn.TopLeft()); //注意：CDBButton中的调用总是相对于客户区
	m_sysToolTipCtrl.SetToolRect(this, ID_SC_CLOSE, &rcSysCloseBtn);


	if (!(dwStyle & WS_MINIMIZEBOX)
		&& !(dwStyle & WS_MAXIMIZEBOX))
		return;

	//最大化按钮
	CRect rcSysMaxBtn(m_rcSysMaxBtn);
	rcSysMaxBtn.OffsetRect(rcWindow.Width()
		- m_rcSysMaxBtn.left*2 - m_rcSysMaxBtn.Width(),
		0);
	rcSysMaxBtn.OffsetRect(rcWindow.TopLeft());
	ScreenToClient(&rcSysMaxBtn);

	m_sysMaxBtn.Move(rcSysMaxBtn.TopLeft()); //注意：CDBButton中的调用总是相对于客户区
	m_sysToolTipCtrl.SetToolRect(this, ID_SC_MAX, &rcSysMaxBtn);

	//最小化按钮
	CRect rcSysMinBtn(m_rcSysMinBtn);
	rcSysMinBtn.OffsetRect(rcWindow.Width()
		- m_rcSysMinBtn.left*2 - m_rcSysMinBtn.Width(),
		0);
	rcSysMinBtn.OffsetRect(rcWindow.TopLeft());
	ScreenToClient(&rcSysMinBtn);

	m_sysMinBtn.Move(rcSysMinBtn.TopLeft()); //注意：CDBButton中的调用总是相对于客户区
	m_sysToolTipCtrl.SetToolRect(this, ID_SC_MIN, &rcSysMinBtn);
}
	
//留下标题栏与边框空间
void CDBFrame::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	int iCxBorderOld = 0, iCyBorderOld = 0;//原边框尺寸
	DWORD dwStyle = GetStyle();
	if (dwStyle & WS_THICKFRAME)
	{
		//iCxBorderOld = GetSystemMetrics(SM_CXSIZEFRAME);
		//iCyBorderOld = GetSystemMetrics(SM_CYSIZEFRAME);
	}

	lpncsp->rgrc[0].top += (m_imageCaption.Height()
		+ m_rcSysBorder.top - iCyBorderOld);	
	lpncsp->rgrc[0].left += (m_rcSysBorder.left - iCxBorderOld);
	lpncsp->rgrc[0].right -= (m_rcSysBorder.right - iCxBorderOld);
	lpncsp->rgrc[0].bottom -= (m_rcSysBorder.bottom - iCyBorderOld);

	//CFrameWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CDBFrame::OnNcPaint() 
{
	if (IsIconic()) {
		return;
	}
	CDC *pDC;
	pDC = GetWindowDC();
	if (pDC == NULL)	//获取DC失败,则不画
		return;
	
	CRect rect;
	GetWindowRect(&rect);
	CMemDC memDC(pDC, rect.Width(), m_imageCaption.Height() + m_rcSysBorder.top);
	DrawCaption(&memDC);
	DrawSysButtons(&memDC);
	pDC->BitBlt(0, 0, rect.Width(), m_imageCaption.Height() + m_rcSysBorder.top,
				&memDC, 0, 0, SRCCOPY);
	
	DrawBorder(pDC);
	
	ReleaseDC(pDC);
}
//绘制窗口边框
void CDBFrame::DrawBorder(CDC *pDC)
{
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	rcWindow.OffsetRect(-rcWindow.left, -rcWindow.top);
	
	int iCaptionHeight = m_rcSysBorder.top + m_imageCaption.Height();
	m_imageBorders[LEFT].Render(pDC, CPoint(0, iCaptionHeight),
		CSize(m_rcSysBorder.left, rcWindow.Height() - iCaptionHeight));

	m_imageBorders[RIGHT].Render(pDC,
		CPoint(rcWindow.Width() - m_rcSysBorder.right, iCaptionHeight),
		CSize(m_rcSysBorder.right, rcWindow.Height() - iCaptionHeight));

	m_imageBorders[BOTTOM].Render(pDC,
		CPoint(0, rcWindow.Height() - m_rcSysBorder.bottom),
		CSize(rcWindow.Width(), m_rcSysBorder.bottom));

 	CRgn rgn;
 	CRect rcWnd;
	GetWindowRect(rcWnd);
	rcWnd.OffsetRect(-rcWnd.left, -rcWnd.top);
 	rgn.CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, 2,2);
	CBrush brush;
	brush.CreateSolidBrush(RGB(171,186,227));
	pDC->FrameRgn(&rgn, &brush, 1,1);


}
//绘制标题栏
void CDBFrame::DrawCaption(CDC *pDC)
{
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//画标题栏位图
	CRect rcCaption(0,//m_rcSysBorder.left,
					m_rcSysBorder.top,
					rcWindow.Width(),// - m_rcSysBorder.right,
					m_rcSysBorder.top + m_imageCaption.Height());

	if (m_bNcActivate)
		m_imageCaption.Render(pDC, rcCaption.TopLeft(), rcCaption.Size());
	else
		m_imageCaptionNA.Render(pDC, rcCaption.TopLeft(), rcCaption.Size());

	
	CRect rcTitle;
	GetTitleRect(rcTitle);
	DrawTitle(pDC, rcTitle);
}

void CDBFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	if (nState == WA_INACTIVE)
		m_bNcActivate = FALSE;
	else
		m_bNcActivate = TRUE;
	//更新
	if (m_sysToolTipCtrl.m_hWnd != NULL) //OnCreate必须被调用过
		SendMessage(WM_NCPAINT,0,0);
}

void CDBFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);

	OnSetWndRgn();

	//决定显示最大化按钮还是恢复按钮
	if (nType == SIZE_MAXIMIZED && !m_sysMaxBtn.m_bMaximized)//窗口被最大化
	{

		if (!m_sysMaxBtn.m_bMinimized)
			m_sysMaxBtn.Exchange();
		m_sysMaxBtn.m_bMaximized = TRUE;
		m_sysMaxBtn.m_bMinimized = FALSE;
		//更新按钮
		SendMessage(WM_NCPAINT, 0, 0);

		m_sysToolTipCtrl.UpdateTipText(TEXT("Restore"),this,ID_SC_MAX);
	}

	if (nType == SIZE_MINIMIZED
		&& !m_sysMaxBtn.m_bMinimized)//窗口被最小化
	{
		if (!m_sysMaxBtn.m_bMaximized)
			m_sysMaxBtn.Exchange();
		m_sysMaxBtn.m_bMaximized = FALSE;
		m_sysMaxBtn.m_bMinimized = TRUE;
		//更新按钮
		SendMessage(WM_NCPAINT, 0, 0);
	}
	
	if (nType == SIZE_RESTORED
		&& (m_sysMaxBtn.m_bMaximized || m_sysMaxBtn.m_bMinimized))//窗口被还原
	{
		m_sysMaxBtn.Exchange();
		m_sysMaxBtn.m_bMaximized = FALSE;
		m_sysMaxBtn.m_bMinimized = FALSE;	
		//更新按钮
		SendMessage(WM_NCPAINT, 0, 0);

		m_sysToolTipCtrl.UpdateTipText(TEXT("Maximize  window"),this,ID_SC_MAX);
	}
}


LRESULT CDBFrame::OnNcHitTest(CPoint point) 
{
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	CRect rcCaption(rcWindow.left + m_rcSysBorder.left,
		rcWindow.top + m_rcSysBorder.top,
		rcWindow.left + rcWindow.Width() - m_rcSysBorder.right,
		rcWindow.top + m_rcSysBorder.top + m_imageCaption.Height());

	if (rcCaption.PtInRect(point))
		return HTCAPTION;
	CRect rcClient;
	GetClientRect(&rcClient);
	ClientToScreen(&rcClient);
	if(rcClient.PtInRect(point))
		return HTCLIENT;
	return CFrameWnd::OnNcHitTest(point);
}

//系统关闭按钮
void CDBFrame::OnSCClose() 
{
	PostMessage(WM_CLOSE);
}

//系统最大化按钮
void CDBFrame::OnSCMax() 
{
	ShowWindow(SW_SHOWMAXIMIZED);
}

//系统最小化按钮
void CDBFrame::OnSCMin() 
{
	ShowWindow(SW_SHOWMINNOACTIVE);
}

//系统还原按钮
void CDBFrame::OnSCRestore() 
{
	ShowWindow(SW_RESTORE);
}

void CDBFrame::DrawSysButtons(CDC *pDC)
{
	//更新系统按钮位置
	OnSetSysButtonPos();
	
	DWORD dwStyle = GetStyle();
	if (!(dwStyle & WS_SYSMENU))
		return;

	//绘制系统按钮
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//关闭按钮
	CSize szBtnBmp = m_sysCloseBtn.GetImageSize();
	CPoint ptOrg(rcWindow.Width() - szBtnBmp.cx - m_rcSysCloseBtn.left,
				 m_rcSysBorder.top);

	ptOrg.Offset(rcWindow.TopLeft());
	ScreenToClient(&ptOrg);

	m_sysCloseBtn.DrawButton(ptOrg, pDC); //注意：CDBButton类中的调用总是相对于客户区

	if (!(dwStyle & WS_MINIMIZEBOX)
		&& !(dwStyle & WS_MAXIMIZEBOX))
		return;
	
	//最大化按钮
	szBtnBmp = m_sysMaxBtn.GetImageSize();
	ptOrg.Offset(-szBtnBmp.cx, 0);
	m_sysMaxBtn.DrawButton(ptOrg, pDC); //注意：CDBButton类中的调用总是相对于客户区

	//最小化按钮
	szBtnBmp = m_sysMinBtn.GetImageSize();
	ptOrg.Offset(-szBtnBmp.cx, 0);
	m_sysMinBtn.DrawButton(ptOrg, pDC); //注意：CDBButton类中的调用总是相对于客户区
}

int CDBFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
		
		//去掉系统原来的标题栏和窗体边框
	DWORD dwStyle = GetStyle();
	dwStyle &= ~WS_CAPTION;
	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);
	DWORD dwExStyle = GetExStyle();
	dwExStyle &= ~WS_EX_DLGMODALFRAME;
	SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle);

	//促使窗口调用OnNcCalcSize函数
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOZORDER
		| SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	//因自定义标题栏，需调整对话框高度
	CRect rcDlg;
	GetWindowRect(&rcDlg);
	SetWindowPos(0, 0, 0, rcDlg.Width(),
		rcDlg.Height() - ::GetSystemMetrics(SM_CYCAPTION)
		+ m_rcSysBorder.top + m_rcSysBorder.bottom
		+ m_imageCaption.Height(),
		SWP_NOZORDER | SWP_NOMOVE);

	//创建工具提示,用于系统按钮
	m_sysToolTipCtrl.Create(this);
	m_sysToolTipCtrl.SetDelayTime(250);

	//创建系统按钮
	dwStyle = GetStyle();
	if (dwStyle & WS_SYSMENU)
	{
		m_sysCloseBtn.Create(CRect(0, 0,
			m_rcSysCloseBtn.Width(), m_rcSysCloseBtn.Height()),
			this, ID_SC_CLOSE, DBFrame_SysCloseBtn);
		
		m_sysToolTipCtrl.AddTool(this, strSysCloseBtnTip,
			&CRect(0, 0, m_rcSysCloseBtn.Width(), m_rcSysCloseBtn.Height()),
			ID_SC_CLOSE);

		if (dwStyle & WS_MINIMIZEBOX
			|| dwStyle & WS_MAXIMIZEBOX)
		{
			m_sysMaxBtn.Create(CRect(0, 0,
				m_rcSysMaxBtn.Width(), m_rcSysMaxBtn.Height()),
				this, ID_SC_MAX, ID_SC_RESTORE, DBFrame_SysMaxBtn,
				DBFrame_SysRestoreBtn);
			if (!(dwStyle & WS_MAXIMIZEBOX))
				m_sysMaxBtn.EnableBtn(FALSE);
			m_sysToolTipCtrl.AddTool(this, strSysMaxBtnTip,
				&CRect(0, 0, m_rcSysMaxBtn.Width(), m_rcSysMaxBtn.Height()),
				ID_SC_MAX);


			
			m_sysMinBtn.Create(CRect(0, 0, 
				m_rcSysMinBtn.Width(), m_rcSysMinBtn.Height()),
				this, ID_SC_MIN, DBFrame_SysMinBtn);
			if (!(dwStyle & WS_MINIMIZEBOX))
				m_sysMinBtn.EnableBtn(FALSE);
			m_sysToolTipCtrl.AddTool(this, strSysMinBtnTip,
				&CRect(0, 0, m_rcSysMinBtn.Width(), m_rcSysMinBtn.Height()),
				ID_SC_MIN);
		}
	}
	
	//设置系统按钮位置
	OnSetSysButtonPos();

	return 0;
}

void CDBFrame::GetTitleRect(CRect &rect)
{
	ASSERT(::IsWindow(m_hWnd));
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	
	rect.left = (rcWindow.Width() - m_imageTitleBk.Width())/2;
	rect.top = 1;
	rect.right = rect.left + m_imageTitleBk.Width();
	rect.bottom = m_imageTitleBk.Height();
}

void CDBFrame::DrawTitle(CDC *pDC, CRect rect)
{
//	m_imageTitleBk.Render(pDC, rect.TopLeft(), rect.Size());
	CRect rcLeftCapture = rect;
	rcLeftCapture.left  = 5;

	rcLeftCapture.right = rcLeftCapture.left + m_imageLeftHeader.Width();
		


	m_imageLeftHeader.Render(pDC,rcLeftCapture.TopLeft(),rcLeftCapture.Size());


	CString strText;
	GetWindowText(strText);
	CFont *pOldFont=pDC->SelectObject(&g_fontBase);
	int iModeOld = pDC->SetBkMode(TRANSPARENT);
	
	//字体绘制位置垂直方向位于标题栏中间
	rect.top += 12;
//	pDC->DrawText(strText, &rect, DT_CENTER);
	
	pDC->SetBkMode(iModeOld);
	pDC->SelectObject(pOldFont);
}

void CDBFrame::OnSetWndRgn()
{
	/*
	//创建凸出的部分区域
	CRgn rgnAdd;
	CRect rcRgnAdd;
	GetTitleRect(rcRgnAdd);
	rcRgnAdd.bottom = rcRgnAdd.top + 3;
	rgnAdd.CreateRectRgnIndirect(rcRgnAdd);
	*/

	CRect rcRgn;
	GetWindowRect(&rcRgn);
	rcRgn.OffsetRect(-rcRgn.left, -rcRgn.top);
	rcRgn.top;
	m_rgnWnd.DeleteObject();
	m_rgnWnd.CreateRoundRectRgn(rcRgn.left, rcRgn.top, rcRgn.right,
		rcRgn.bottom, m_szRoundCorner.cx, m_szRoundCorner.cy);
	
	/*
	//合并区域
	m_rgnWnd.CombineRgn(&m_rgnWnd, &rgnAdd, RGN_OR);
	*/

	SetWindowRgn(m_rgnWnd, TRUE);

}

LRESULT CDBFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//派遣鼠标消息给系统按钮与工具提示控件
	const MSG *pMsg = GetCurrentMessage();
	DWORD dwStyle = GetStyle();
	if (dwStyle & WS_SYSMENU && pMsg != NULL)
	{
		m_sysCloseBtn.RelayEvent(pMsg);
		if (dwStyle & WS_MINIMIZEBOX
			|| dwStyle & WS_MAXIMIZEBOX)
		{
			m_sysMaxBtn.RelayEvent(pMsg);
			m_sysMinBtn.RelayEvent(pMsg);
		}
	}

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

BOOL CDBFrame::PreTranslateMessage(MSG* pMsg)
{
	DWORD dwStyle = GetStyle();
	if (dwStyle & WS_SYSMENU && pMsg != NULL
		&& ::IsWindow(m_sysToolTipCtrl.GetSafeHwnd()))
		m_sysToolTipCtrl.RelayEvent(const_cast<MSG *>(pMsg));

	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CDBFrame::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	CFrameWnd::OnGetMinMaxInfo(lpMMI);

	RECT rect;
	SystemParametersInfo(SPI_GETWORKAREA,0,(PVOID)(&rect),0);
	lpMMI->ptMaxPosition.x = rect.left- 1;
	lpMMI->ptMaxPosition.y = rect.top - 1;
	lpMMI->ptMaxSize.x = rect.right - rect.left + 2;
	lpMMI->ptMaxSize.y = rect.bottom - rect.top + 2;
	lpMMI->ptMaxTrackSize.x = 5000;
	lpMMI->ptMaxTrackSize.y = 5000;
}

BOOL CDBFrame::OnNcActivate(BOOL bActive)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//{added by tianyx 2009-6-12--15:32 for prevent from vista drawing frame border.
	return TRUE;
	//}ended by tianyx

	//return CFrameWnd::OnNcActivate(bActive);
}

void CDBFrame::EnableMinSizeBtn( BOOL bEnable /*= TRUE*/ )
{
	m_sysMinBtn.EnableBtn(bEnable);	
}