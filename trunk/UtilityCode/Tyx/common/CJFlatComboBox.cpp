// CJFlatComboBox.cpp : implementation file
//
// Copyright ?1998-99 Kirk W. Stowell
//		mailto:kstowell@codejockeys.com
//		http://www.codejockeys.com/kstowell/
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. If the source code in this file is used in 
// any  commercial application then a statement along the lines of 
// "Portions Copyright ?1998 Kirk Stowell" must be included in 
// the startup banner, "About" box or printed documentation. An email 
// letting me know that you are using it would be nice as well. That's 
// not much to ask considering the amount of work that went into this.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// Expect bugs!
// 
// Please use and enjoy, and let me know of any bugs/mods/improvements 
// that you have found/implemented and I will fix/incorporate them into 
// this file. 
//
/////////////////////////////////////////////////////////////////////////////
/****************************************************************************
 *
 * $Date: 2009-07-16 17:58:24 +0800 (鏄熸湡鍥? 16 涓冩湀 2009) $
 * $Revision: 884 $
 * $Archive: /CodeJockey/CJ60Lib/CJFlatComboBox.cpp $
 *
 * $History: CJFlatComboBox.cpp $
 * 
 * *****************  Version 9  *****************
 * User: Kirk Stowell Date: 5/17/99    Time: 9:24p
 * Updated in $/CodeJockey/CJ60Lib
 * Gilberto Botaro [seafox@techno.com.br] - Added code for auto
 * completion.
 * 
 * *****************  Version 8  *****************
 * User: Kirk Stowell Date: 5/03/99    Time: 10:44p
 * Updated in $/CodeJockey/CJ60Lib
 * Fixed code ommisions.
 * 
 * *****************  Version 7  *****************
 * User: Kirk Stowell Date: 5/03/99    Time: 8:42p
 * Updated in $/CodeJockey/CJ60Lib
 * Danilo Lax [Danilo_Lax@www2.alpeng.com] - The control is painted flat
 * when the mouse leaves the box, even if the control has the focus, now
 * checking for the focus in OnTimer(.
 * 
 * Removed, unhandled msgs are processed by
 * CComboBox::PreTranslateMessage(pMsg). Used reflected msgs
 * BN_GETFOCUS/CBN_KILLFOCUS instead of WM_GETFOCUS/WM_KILLFOCUS.
 * 
 * ::GetFocus() doesn't return CFlatComboBox::m_hWnd, even if the control
 * has the focus (don't know why, maybe ::GetFocus() only returns the
 * handle to the EditField part of the ComboBox), used flag m_bHasFocus
 * instead.
 * 
 * *****************  Version 6  *****************
 * User: Kirk Stowell Date: 4/03/99    Time: 4:26p
 * Updated in $/CodeJockey/CJ60Lib
 * Added comments and cleaned up code.
 * 
 * *****************  Version 5  *****************
 * User: Kirk Stowell Date: 1/22/99    Time: 4:26p
 * Updated in $/CodeJockey/CJ60Lib
 * Fixed accelerator problem reported by YOSH YAMASHITA
 * (yosh@muj.biglobe.ne.jp) with Matt Weagle's solution of overloading
 * PreTranslateMessage().
 * 
 * *****************  Version 4  *****************
 * User: Kirk Stowell Date: 1/02/99    Time: 4:25p
 * Updated in $/CodeJockey/CJ60Lib
 * Renamed class from CJComboBox to CCJFlatComboBox, changed DRAWSTATE to
 * enum, removed method Offset(), and replaced with a direct call to
 * ::GetSystemMetrics().
 * 
 * *****************  Version 3  *****************
 * User: Kirk Stowell Date: 11/02/98   Time: 4:24p
 * Updated in $/CodeJockey/CJ60Lib
 * Added OnSetFocus(), OnKillFocus() and modified OnPaint() so the control
 * remains beveled until it actually looses focus - (Vachik
 * Hovhannissian).
 * 
 * *****************  Version 2  *****************
 * User: Kirk Stowell Date: 10/05/98   Time: 4:24p
 * Updated in $/CodeJockey/CJ60Lib
 * Fixed offset bug to use GetSystemMetrics(), instead of hard coded
 * values -
 * Todd Brannam (tbrannam@msn.com).
 * 
 * *****************  Version 1  *****************
 * User: Kirk Stowell Date: 10/02/98   Time: 4:23p
 * Created in $/CodeJockey/CJ60Lib
 * First release version.
 *
 ***************************************************************************/
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CJFlatComboBox.h"
#include "resource.h"
#include "MemDC.h"
#include "global.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCJFlatComboBox

CCJFlatComboBox::CCJFlatComboBox()
{
	m_bLBtnDown	= FALSE;
	m_bPainted	= FALSE;
	m_bHasFocus	= FALSE;
	m_bAutoComp = TRUE;

	m_clrBtnHilite  = ::GetSysColor(COLOR_BTNHILIGHT);
	m_clrBtnShadow  = ::GetSysColor(COLOR_BTNSHADOW);
	m_clrBtnFace    = RGB(239,235,239);
	m_clrArrow		= RGB(255, 255, 255);
	m_clrWindow		= RGB(255, 255, 255);
	m_clrDisable	= RGB(239,235,239);

	m_nOffset		= ::GetSystemMetrics(SM_CXHTHUMB);

	CBitmap bmpTemp;
	if(bmpTemp.LoadBitmap(IDB_BMP_COMBO))
	{	
		BITMAP bmpInfo;
		bmpTemp.GetBitmap(&bmpInfo);
		m_image.Create(bmpInfo.bmWidth/4, bmpInfo.bmHeight, 
			ILC_COLOR24, 4, 1);
		m_image.Add(&bmpTemp, RGB(0, 0, 0));
		bmpTemp.DeleteObject();
	}

	CString strBtnBk = AfxGetAppPath() + Combo_Btn_bk;
	HBITMAP hBmp = (HBITMAP)::LoadImage(NULL, strBtnBk, 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if(hBmp)
	{
		m_bmpBtnBk.Attach(hBmp);
	}
}

CCJFlatComboBox::~CCJFlatComboBox()
{
}

IMPLEMENT_DYNAMIC(CCJFlatComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CCJFlatComboBox, CComboBox)
	//{{AFX_MSG_MAP(CCJFlatComboBox)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(CBN_SETFOCUS,   OnSetFocus)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS,  OnKillFocus)
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
	//ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange) //removed by tian_yx 
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCJFlatComboBox message handlers

void CCJFlatComboBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	SetTimer(1,10,NULL);
	OnTimer(1);
	CComboBox::OnMouseMove(nFlags, point);
}

void CCJFlatComboBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = TRUE;
	CComboBox::OnLButtonDown(nFlags, point);
}

void CCJFlatComboBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = FALSE;
	Invalidate();
	CComboBox::OnLButtonUp(nFlags, point);
}

void CCJFlatComboBox::OnTimer(UINT nIDEvent) 
{
	POINT pt;
	GetCursorPos(&pt);
	CRect rcItem;
	GetWindowRect(&rcItem);

	CClientDC dc(this);
	// OnLButtonDown, show pressed.
	if (m_bLBtnDown==TRUE) {
		KillTimer (1);
		if (m_bPainted == TRUE) {
			DrawCombo(&dc, pressed);
			m_bPainted = FALSE;
		}
		return;
	}

	// If mouse leaves, show flat.
	if (!rcItem.PtInRect(pt) && !m_bHasFocus) {
		KillTimer (1);
		if (m_bPainted == TRUE) {
			DrawCombo(&dc, normal);
			m_bPainted = FALSE;
		}
		return;
	}

	// On mouse over, show raised.
	else {
		if (m_bPainted == TRUE)
			return;
		else {
			m_bPainted = TRUE;
			DrawCombo(&dc, raised);
		}
	}
	
	CComboBox::OnTimer(nIDEvent);
}

void CCJFlatComboBox::OnPaint() 
{
	//ModifyStyleEx (WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE,
	//	0, SWP_FRAMECHANGED);
	
	CPaintDC dc(this);
	CRect rcClient;
	GetClientRect(&rcClient);
	CMemDC mdc(&dc, rcClient.Width(), rcClient.Height());

	//背景
	if (IsWindowEnabled())
		mdc.FillSolidRect(rcClient, m_clrWindow);
	else
		mdc.FillSolidRect(rcClient, m_clrDisable);
	
	//边框
	CPoint pt;
	GetCursorPos(&pt);
	CRect rcItem;
	GetWindowRect(&rcItem);
	if (m_bLBtnDown)
		DrawCombo(&mdc, pressed);
	else if ((rcItem.PtInRect(pt)) || m_bHasFocus)
		DrawCombo(&mdc, raised);
	else
		DrawCombo(&mdc, normal);


	//调用DrawItem绘制文字及其它
	int iItem = GetCurSel();
	if (iItem != CB_ERR)
	{
		DRAWITEMSTRUCT drawInfo;
		drawInfo.itemID = iItem;
		drawInfo.CtlID = GetDlgCtrlID();
		drawInfo.CtlType = ODT_COMBOBOX;
		drawInfo.hDC = mdc.m_hDC;
		drawInfo.hwndItem = m_hWnd;
		drawInfo.itemAction = ODA_DRAWENTIRE;
		drawInfo.itemData = GetItemData(iItem);
		drawInfo.itemState = ODS_DEFAULT;
		CRect rect = rcClient;
		CRect rcArrow;
		GetArrowRect(&rcArrow);
		rect.right = rcArrow.left;
		rect.DeflateRect(3, 2, 0, 2);
		drawInfo.rcItem = rect;

		SendMessage(WM_DRAWITEM, drawInfo.CtlID, (LPARAM)(&drawInfo));
	}

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(),
		&mdc, 0, 0, SRCCOPY);
	
}

void CCJFlatComboBox::DrawCombo(CDC *pDC, STATE eState)
{
	ASSERT(pDC != NULL);

	CRect rcItem;
	GetClientRect(&rcItem);
	
	// Cover up dark 3D shadow.
	if (m_image.GetSafeHandle() == NULL)
	{
		pDC->Draw3dRect(rcItem, m_clrBtnShadow, m_clrBtnHilite);
		rcItem.DeflateRect(1,1);
	
		pDC->Draw3dRect( rcItem, m_clrBtnFace, m_clrBtnFace );
		rcItem.DeflateRect(1,1);
	}
	else
	{
		pDC->Draw3dRect(rcItem, RGB(123,158,189), RGB(123,158,189));
		rcItem.DeflateRect(1,1);
		pDC->Draw3dRect(rcItem, RGB(255, 255, 255), RGB(255, 255, 255));
		rcItem.DeflateRect(1, 1);
	}
	
	switch (eState)
	{
	case normal:
		break;
	case raised:
		break;
	case pressed:
		break;
	}
	
	if (!IsWindowEnabled())
	{
		pDC->Draw3dRect(rcItem, m_clrBtnFace, m_clrBtnFace);
	}
	else
	{
		pDC->Draw3dRect(rcItem, RGB(255, 255, 255), RGB(255, 255, 255));
	}
	
	DrawArrow(pDC, eState);
}

void CCJFlatComboBox::DrawArrow(CDC *pDC, STATE eState)
{   
	ASSERT(pDC != NULL);
	
	//绘制背景
	CRect rect;
    GetArrowRect(&rect);
	//pDC->FillSolidRect(rect, m_clrBtnFace);
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	HGDIOBJ oldObj = memDC.SelectObject(m_bmpBtnBk);
	pDC->BitBlt(rect.left,rect.top, rect.Width(), rect.Height(), &memDC, 0,0, SRCCOPY);

	memDC.SelectObject(oldObj);
	memDC.DeleteDC();
	//画箭头
	if (m_image.GetSafeHandle() == NULL)
	{
		CBrush brush;
		switch (eState)
		{  
		case pressed:
			pDC->Draw3dRect(rect, m_clrBtnHilite, m_clrBtnShadow);
			brush.CreateSolidBrush(m_clrArrow);
			rect.OffsetRect(1, 1);
			break;
		case raised:
			pDC->Draw3dRect(rect, m_clrBtnHilite, m_clrBtnShadow);
			brush.CreateSolidBrush(m_clrArrow);	
			break;
		default:
			pDC->Draw3dRect(rect, m_clrBtnHilite, m_clrBtnShadow);
			brush.CreateSolidBrush(m_clrArrow);	
			break;
		}
		
		CBrush *pOldBrush = pDC->SelectObject(&brush);
		CPen penArr(PS_SOLID, 1, RGB(191, 191, 191));
		CPen* pOldPen = pDC->SelectObject(&penArr);

		POINT pts[3];
		pts[0].x = rect.left + rect.Width()/2 - 10;
		pts[0].y = rect.top + rect.Height()/2 - 5;
		pts[1].x = pts[0].x + 20;
		pts[1].y = pts[0].y;
		pts[2].x = rect.left + rect.Width()/2  ;
		pts[2].y = pts[0].y + 10;
		pDC->Polygon(pts, 3);
		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);

	}
	else
	{
		switch (eState)
		{  
		case pressed:
			m_image.Draw(pDC, 2, rect.TopLeft(), ILD_NORMAL);
			break;
		case raised:
			m_image.Draw(pDC, 0, rect.TopLeft(), ILD_NORMAL);
			break;
		default:
			m_image.Draw(pDC, 1, rect.TopLeft(), ILD_NORMAL);
			break;
		}
	}
}

void CCJFlatComboBox::GetArrowRect(CRect *pRect)
{
	ASSERT(pRect != NULL);
	GetClientRect(pRect);

	if (m_image.GetSafeHandle() == NULL)
	{
		pRect->DeflateRect(1, 1);
		pRect->left = pRect->right - 20;
	}
	else
	{
		pRect->DeflateRect(2, 2);
		pRect->left = pRect->right - 20;
	}
}

void CCJFlatComboBox::OnSetFocus()
{
	m_bHasFocus = TRUE;
	CClientDC dc(this);
	DrawCombo(&dc, raised);
}

void CCJFlatComboBox::OnKillFocus() 
{
	m_bHasFocus = FALSE;
	CClientDC dc(this);
	DrawCombo(&dc, normal);
}

BOOL CCJFlatComboBox::PreTranslateMessage(MSG* pMsg) 
{
	// Catch the Alt key so we don't choke if focus 
	// is going to an owner drawn button
	if (pMsg->message == WM_SYSCHAR)
		return TRUE;
		
	return CComboBox::PreTranslateMessage(pMsg);
}

void CCJFlatComboBox::OnEditUpdate() 
{
	// if we are not to auto update the text, get outta here
	if (!m_bAutoComp) 
		return;
	
	// Get the text in the edit box
	CString str;
	GetWindowText(str);
	int nLength = str.GetLength();
	
	// Currently selected range
	DWORD dwCurSel = GetEditSel();
	WORD dStart = LOWORD(dwCurSel);
	WORD dEnd   = HIWORD(dwCurSel);
	
	// Search for, and select in, and string in the combo box that is prefixed
	// by the text in the edit box
	if (SelectString(-1, str) == CB_ERR)
	{
		SetWindowText(str);		// No text selected, so restore what was there before
		if (dwCurSel != CB_ERR)
			SetEditSel(dStart, dEnd);	//restore cursor postion
	}
	
	// Set the text selection as the additional text that we have added
	if (dEnd < nLength && dwCurSel != CB_ERR)
		SetEditSel(dStart, dEnd);
	else
		SetEditSel(nLength, -1);
}

void CCJFlatComboBox::PreSubclassWindow() 
{
	CComboBox::PreSubclassWindow();
	//去掉原有边框
	DWORD dwStyle = GetStyle();
	dwStyle &= ~WS_BORDER;
	dwStyle |= CBS_OWNERDRAWFIXED;
	dwStyle |= CBS_HASSTRINGS;
	SetWindowLong(m_hWnd, GWL_STYLE, dwStyle);

	DWORD dwExStyle = GetExStyle();
	dwExStyle &= ~WS_EX_CLIENTEDGE;
	SetWindowLong(m_hWnd, GWL_EXSTYLE, dwExStyle);

	SetItemHeight(0, 25); //for drop item
	SetItemHeight(-1, 25); //for edit text
	SetFont(&g_fontBase);

	
}

void CCJFlatComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = 25;	
}

void CCJFlatComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	if (lpDrawItemStruct->CtlType != ODT_COMBOBOX)
		return;
	
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	const CRect rcItem = lpDrawItemStruct->rcItem;
	COLORREF clrTextColor, clrBkColor;
	if (!IsWindowEnabled())
	{
		clrTextColor = GetSysColor(COLOR_WINDOWTEXT);
		clrBkColor = m_clrBtnFace;
	}
	else if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		clrTextColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
		clrBkColor = GetSysColor(COLOR_HIGHLIGHT);
	}
	else
	{
		clrTextColor = GetSysColor(COLOR_WINDOWTEXT);
		clrBkColor = GetSysColor(COLOR_WINDOW);
	}

	//背景
	CRect rcBk = rcItem;
	pDC->FillSolidRect(rcBk, clrBkColor);
	
	//焦点矩形
	//if (lpDrawItemStruct->itemState & ODS_SELECTED)
	//	pDC->DrawFocusRect(&rcBk);

	//文本
	CRect rcText = rcItem;
	CString strItem;
	if ((int)(lpDrawItemStruct->itemID) >= 0)
		GetLBText(lpDrawItemStruct->itemID, strItem);

	CFont *pOldFont = pDC->SelectObject(&g_fontBase);
	COLORREF clrTextOld = pDC->SetTextColor(clrTextColor);
	pDC->DrawText(strItem, rcText,
		DT_VCENTER | DT_SINGLELINE);
	pDC->SetTextColor(clrTextOld);
	pDC->SelectObject(pOldFont);	
}

void CCJFlatComboBox::OnSelchange() 
{
	if (GetFocus() == this)
	{
		CFrameWnd *pFrame = GetParentFrame();
		if (pFrame != NULL)
			pFrame->SetFocus();
	}
}


void CCJFlatComboBox::OnSize(UINT nType, int cx, int cy)
{
	CComboBox::OnSize(nType, cx, cy);

	COMBOBOXINFO cmbInfo;
	cmbInfo.cbSize = sizeof(cmbInfo);
	if(GetComboBoxInfo(&cmbInfo))
	{
		CWnd* pWndCmbo = CWnd::FromHandle(cmbInfo.hwndCombo);
		CRect rcCombo;
		pWndCmbo->GetClientRect(rcCombo);


		CWnd* pwndList = CWnd::FromHandle(cmbInfo.hwndList);
		CRect rcList;
		pwndList->GetClientRect(rcList);
		rcList.right += 200;
		pwndList->SetWindowPos(NULL, 0,0, rcList.Width(), rcList.Height(), SWP_NOMOVE);

		CWnd* pWndEdit = CWnd::FromHandle(cmbInfo.hwndItem);
		// 		CRect rcEdit;
		// 		pWndEdit->GetClientRect(rcEdit);

	}

	// TODO: 在此处添加消息处理程序代码
}
