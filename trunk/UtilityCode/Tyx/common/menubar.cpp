// MenuBar.cpp : implementation file
//

#include "stdafx.h"
#include "MenuBar.h"
#include "MemDC.h"
#include "SkinFiles.h"
#include "LanManager.h"
#include "Resource.h"
#include "global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const UINT TIMER_EVENT_ID = 1000;
extern CFont g_fontBase;

/////////////////////////////////////////////////////////////////////////////
// CMenuBar

CMenuBar::CMenuBar()
{
	//创建背景画刷
	HBITMAP hBmp = (HBITMAP)::LoadImage(NULL, MenuBar_Bkgnd, 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	ASSERT(hBmp != NULL);
	m_bruBkgnd.CreatePatternBrush(CBitmap::FromHandle(hBmp));
	
	BITMAP bmpInfo;
	CBitmap::FromHandle(hBmp)->GetBitmap(&bmpInfo);
	m_iHeight = bmpInfo.bmHeight;

	::DeleteObject(hBmp);

	//装载按钮位图
	CString strBmpFile;
	for (int i = 0; i < MENU_ITEM_COUNT; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			strBmpFile.Format(MenuBar_Button, 9, j);
			HBITMAP hBmp = (HBITMAP)::LoadImage(NULL, strBmpFile, 
				IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			ASSERT (hBmp != NULL);
			m_bmpsBtn[i][j].Attach(hBmp);
		}
	}

	m_bmpsBtn[0][0].GetBitmap(&bmpInfo); //按钮必须尺寸相同
	m_szButton.cx = bmpInfo.bmWidth;
	m_szButton.cy = bmpInfo.bmHeight;
}

CMenuBar::~CMenuBar()
{
	::DestroyMenu(m_hMenu);
}


BEGIN_MESSAGE_MAP(CMenuBar, CToolBar)
	//{{AFX_MSG_MAP(CMenuBar)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(TBN_DROPDOWN, OnTBDropDown)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnDrawTBItem)
	ON_MESSAGE(UM_LANGUAGE_CHANDGED, OnLanguageChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuBar message handlers

int CMenuBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetSizes(m_szButton, m_szButton - CSize(7, 6));
	SetHeight(m_iHeight);
	GetToolBarCtrl().SetButtonWidth(m_szButton.cx, m_szButton.cx);
	
	CFrameWnd *pFrame = GetDockingFrame();
	ASSERT(pFrame != NULL);
	CMenu *pMenu = pFrame->GetMenu();
	ASSERT(pMenu != NULL && ::IsMenu(pMenu->m_hMenu));
	m_hMenu = pMenu->m_hMenu;

	//添加按钮
	int count = pMenu->GetMenuItemCount();

	// Commented by Song Haifeng on 03/12/2009 15:00
	/****************************************************************************
	ASSERT (count == MENU_ITEM_COUNT);
	****************************************************************************/
	// End commented 03/12/2009 15:00

	UINT *lpIDArray = new UINT [count];
	int i = 0;
	for (i = 0; i < count; i++)
	{
		if (pMenu->GetMenuItemID(i) == -1)
			lpIDArray[i] = i + 1;
		else 
			lpIDArray[i] = pMenu->GetMenuItemID(i);
	}
	SetButtons(lpIDArray, i);
	
	//设置按钮属性
	for (i = 0; i < count; i++)
	{
		CString strText;
		pMenu->GetMenuString(i, strText, MF_BYPOSITION);
		SetButtonText(i, strText);
		
		UINT nStyle = TBSTYLE_BUTTON;
		if (pMenu->GetMenuItemID(i) == -1)
			nStyle |= TBSTYLE_DROPDOWN;
		SetButtonStyle(i, nStyle);
		GetToolBarCtrl().EnableButton(lpIDArray[i]);
	}

	VERIFY(pFrame->SetMenu(NULL));
	delete [] lpIDArray;


	if (!g_lanManager.m_bRead)
	{
		if (g_lanManager.GetDefLanIndex() == g_lanManager.GetCurLanIndex())
			LoadLanguage(FALSE);
	}
	else
		LoadLanguage(TRUE);


	return 0;
}

void CMenuBar::OnDrawTBItem(NMHDR * pNotifyStruct, LRESULT *result)
{
    LPNMTBCUSTOMDRAW pNMTBCustomDraw = (LPNMTBCUSTOMDRAW)pNotifyStruct;
	
	CDC *pDC = CDC::FromHandle(pNMTBCustomDraw->nmcd.hdc);
    
	if (pNMTBCustomDraw->nmcd.dwDrawStage == CDDS_PREPAINT)
		*result = CDRF_NOTIFYITEMDRAW;
	else if (pNMTBCustomDraw->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{   
		const int iItem = CommandToIndex(pNMTBCustomDraw->nmcd.dwItemSpec);
		ASSERT(iItem < GetToolBarCtrl().GetButtonCount()
			   && iItem >= 0);
		const CRect rcItem =  pNMTBCustomDraw->nmcd.rc;

		CString strText = GetButtonText(iItem);
		ASSERT(!strText.IsEmpty());

		if (pNMTBCustomDraw->nmcd.uItemState & CDIS_SELECTED)
        {
			CMemDC memDC(pDC, m_szButton.cx, m_szButton.cy);
			//Background		
			memDC.DrawState(CPoint(0, 0),
							m_szButton,
							&m_bmpsBtn[iItem][2],
							DST_BITMAP | DSS_NORMAL);
			//Text
			CFont *pOldFont = memDC.SelectObject(&g_fontBase);
			CRect rcText(0/*rcItem.Height()*/,
						 0,
						 rcItem.Width(),
						 rcItem.Height());
			ASSERT(!rcText.IsRectEmpty());
			rcText.OffsetRect(1, 1);
			int iModeBkOld = memDC.SetBkMode(TRANSPARENT);
			memDC.DrawText(strText, rcText,
						   DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			memDC.SetBkMode(iModeBkOld);
			memDC.SelectObject(pOldFont);

			pDC->BitBlt(rcItem.left, rcItem.top, m_szButton.cx, m_szButton.cy,
						&memDC, 0, 0, SRCCOPY);

		}
		else if (pNMTBCustomDraw->nmcd.uItemState & CDIS_HOT)
		{   
			CMemDC memDC(pDC, m_szButton.cx, m_szButton.cy);
			//Background		
			memDC.DrawState(CPoint(0, 0),
							m_szButton,
							&m_bmpsBtn[iItem][1],
							DST_BITMAP | DSS_NORMAL);
			//Text
			CFont *pOldFont = memDC.SelectObject(&g_fontBase);
			CRect rcText(0/*rcItem.Height()*/,
						 0,
						 rcItem.Width(),
						 rcItem.Height());
			ASSERT(!rcText.IsRectEmpty());
			int iModeBkOld = memDC.SetBkMode(TRANSPARENT);
			memDC.DrawText(strText, rcText,
						   DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			memDC.SetBkMode(iModeBkOld);
			memDC.SelectObject(pOldFont);

			pDC->BitBlt(rcItem.left, rcItem.top, m_szButton.cx, m_szButton.cy,
						&memDC, 0, 0, SRCCOPY);
		}
		else
		{
			CMemDC memDC(pDC, m_szButton.cx, m_szButton.cy);
			//Background		
			memDC.DrawState(CPoint(0, 0),
							m_szButton,
							&m_bmpsBtn[iItem][0],
							DST_BITMAP | DSS_NORMAL);
			//Text
			CFont *pOldFont = memDC.SelectObject(&g_fontBase);
			CRect rcText(0/*rcItem.Height()*/,
						 0,
						 rcItem.Width(),
						 rcItem.Height());
			ASSERT(!rcText.IsRectEmpty());
			int iModeBkOld = memDC.SetBkMode(TRANSPARENT);
			memDC.DrawText(strText, rcText,
						   DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			memDC.SetBkMode(iModeBkOld);
			memDC.SelectObject(pOldFont);

			pDC->BitBlt(rcItem.left, rcItem.top, m_szButton.cx, m_szButton.cy,
						&memDC, 0, 0, SRCCOPY);
		}

		*result=CDRF_SKIPDEFAULT;
	}
}

void CMenuBar::OnTBDropDown(NMHDR * pNotifyStruct, LRESULT *result)
{
	NMTOOLBAR *pNmToolBar = (NMTOOLBAR *)pNotifyStruct;
	int nIDItem = pNmToolBar->iItem;
	int iItem = pNmToolBar->iItem - 1; //pNmToolBar->iItem表示命令ID

	HMENU hSubMenu = ::GetSubMenu(m_hMenu, iItem);
	if (hSubMenu != NULL)
	{
		CRect rcButton;
		GetToolBarCtrl().GetRect(pNmToolBar->iItem, &rcButton);
		ClientToScreen(&rcButton);
		m_itemDrop = iItem;
		TPMPARAMS stTpm;
		stTpm.cbSize = sizeof (stTpm);
		stTpm.rcExclude = rcButton;
		GetToolBarCtrl().PressButton(nIDItem);
		CFrameWnd *pFrame = GetParentFrame();
		ASSERT(pFrame != NULL &&::IsWindow(pFrame->m_hWnd));

		SetTimer(TIMER_EVENT_ID, 100, NULL);
		TrackPopupMenuEx(hSubMenu,
					 	 TPM_VERTICAL|TPM_LEFTALIGN,
						 rcButton.left,
						 rcButton.bottom,
						 pFrame->m_hWnd, &stTpm);
		KillTimer(TIMER_EVENT_ID);

		if (GetToolBarCtrl().IsButtonPressed(nIDItem))
			GetToolBarCtrl().PressButton(nIDItem, FALSE);

        *result = TBDDRET_DEFAULT;
	}
	else
	    *result = TBDDRET_NODEFAULT;
}

void CMenuBar::OnUpdateCmdUI(CFrameWnd *pTarget, BOOL bDisableIfNoHndler)
{
	CToolBar::OnUpdateCmdUI(pTarget, FALSE);
}

BOOL CMenuBar::OnEraseBkgnd(CDC* pDC) 
{
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	ScreenToClient(&rcWindow);
	
	CRect rcClient;
	GetClientRect(&rcClient);
	
	CPoint ptOrg = rcWindow.TopLeft() - rcClient.TopLeft();
	CPoint ptOldOrg = pDC->SetBrushOrg(ptOrg); 
	pDC->FillRect(&rcClient, &m_bruBkgnd);
	pDC->SetBrushOrg(ptOldOrg);
	
	return TRUE;
}

void CMenuBar::OnNcPaint() 
{
	CWindowDC wdc(this);
	
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	
	CRect rcClient;
	GetClientRect(&rcClient);
	ClientToScreen(&rcClient);
	rcClient.OffsetRect(-rcWindow.left, -rcWindow.top);

	rcWindow.OffsetRect(-rcWindow.left, -rcWindow.top);
	
	wdc.ExcludeClipRect(rcClient);
	wdc.FillRect(rcWindow, &m_bruBkgnd);
	wdc.FillSolidRect(&rcWindow, RGB(0,0, 255));
}


void CMenuBar::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TIMER_EVENT_ID)
	{
		int index;
		CPoint pt;
		GetCursorPos(&pt);
		
		if (WindowFromPoint(pt) != this)
			return;

		ScreenToClient(&pt);
		CRect rcClient;
		GetClientRect(&rcClient);
		
		if (!rcClient.PtInRect(pt))
			return;
		if ((index = GetToolBarCtrl().HitTest(&pt)) < 0)
			return;
		if (index == m_itemDrop)
			return;
		if (GetMenuItemID(m_hMenu, index) != -1)
			return;

		GetCursorPos(&pt);
		mouse_event(MOUSEEVENTF_LEFTDOWN, pt.x, pt.y, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, pt.x, pt.y, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTDOWN, pt.x, pt.y, 0, 0);
	}

	CToolBar::OnTimer(nIDEvent);
}

LRESULT CMenuBar::OnLanguageChanged(WPARAM, LPARAM)
{

	LoadLanguage(TRUE);
	return 0;
}

void CMenuBar::LoadLanguage(BOOL bRead)
{
	if (m_hMenu == NULL)
	{
		ASSERT(0);
		return;
	}
return;
	if(bRead)
	{
		g_lanManager.MenuReadLan(TEXT("MainFrameMenu"), CMenu::FromHandle(m_hMenu));
		
		//改换按钮文字
		CMenu *pMenu = CMenu::FromHandle(m_hMenu);
		ASSERT(pMenu != NULL);
		int count = pMenu->GetMenuItemCount();
		ASSERT (count == MENU_ITEM_COUNT);
		ASSERT(GetToolBarCtrl().GetButtonCount() == count);
		for (int i = 0; i < count; i++)
		{
			CString strText;
			pMenu->GetMenuString(i, strText, MF_BYPOSITION);
			SetButtonText(i, strText);
		}

	}
	else
	{
		g_lanManager.MenuWriteLan(TEXT("MainFrameMenu"), CMenu::FromHandle(m_hMenu));
	}
}