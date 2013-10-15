// DBMenu.cpp: implementation of the DBMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBMenu.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DBMenu::DBMenu()
{
	m_clrMenuText = GetSysColor(COLOR_MENUTEXT);
	m_clrMenuTextSel = GetSysColor(COLOR_MENUTEXT);
	m_clrMenuTextGray = GetSysColor(COLOR_GRAYTEXT);

	m_clrMenuFaceDark = RGB(193,222,255);

	m_clrBtnFace = RGB(245, 246, 250);//RGB(243,253,255);
	m_clrBtnHilight = RGB(180, 208, 243);
	m_clrBtnBorder = RGB(75, 85, 137);//RGB(123, 151, 220);

	m_szImage = CSize(16, 16);
	m_iGapTextAwayImage = 6;

	m_rcBorders.SetRect(2, 3, 3, 3);

	//装载菜单状态图标
	CBitmap bmpTemp;
	bmpTemp.LoadBitmap(IDB_DBMENU_STATE);
	m_imageListState.Create(16, 16, ILC_COLOR8 | ILC_MASK, 2, 1);
	m_imageListState.Add(&bmpTemp, RGB(192, 192, 192));

	//创建菜单字体
	NONCLIENTMETRICS ncm;
	memset(&ncm, 0, sizeof(ncm));
	ncm.cbSize = sizeof(ncm);
	::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, (PVOID) &ncm, 0);
	m_hFont = ::CreateFontIndirect(&ncm.lfMenuFont);
}

DBMenu::~DBMenu()
{
	POSITION pos = m_listItemDataPtr.GetHeadPosition();
	ASSERT(pos == NULL); //测试是否有内存泄露
	while (pos != NULL)
		delete (DRAWITEMDATA *)m_listItemDataPtr.GetNext(pos);
	
	if (m_hFont)
		::DeleteObject((HGDIOBJ)m_hFont);
}

//从工具条中添加图像资源,nIDBitmap=-1表示加载默认的工具条位图
BOOL DBMenu::AddToolBarResource(UINT nIDToolBar,UINT nIDBitmap/* = -1*/)
{
	HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(nIDToolBar), RT_TOOLBAR);
	if (hRsrc == NULL)
		return FALSE;

	HGLOBAL hGlb = ::LoadResource(NULL, hRsrc);
	if (hGlb == NULL)
		return FALSE;

	ToolBarData* pTBData = (ToolBarData *)::LockResource(hGlb);
	if (pTBData == NULL)
		return FALSE;
	ASSERT(pTBData->wVersion == 1);	
	WORD* pIDArray = (WORD *)(pTBData + 1);
	ASSERT (pIDArray != NULL);
	int nCount = pTBData->wItemCount;
	ASSERT (nCount > 0);
	//创建图象列表m_imageList
	if (m_imageList.GetSafeHandle() == NULL)
	{
		m_szImage.cx = (int) pTBData->wWidth;
		m_szImage.cy = (int) pTBData->wHeight;
		if (!m_imageList.Create(m_szImage.cx, m_szImage.cy, ILC_COLOR24|ILC_MASK, pTBData->wItemCount, 0))
			return FALSE;
	}

	//图标资源加入m_imageList,对应的ID加入m_itemIDArray
	CBitmap bmpToolBar;
	bmpToolBar.LoadBitmap(nIDBitmap == -1 ? nIDToolBar : nIDBitmap);
	m_imageList.Add(&bmpToolBar, RGB(192, 192, 192));
	for(int i = 0; i < nCount; i++)
	{
		if(pIDArray[i] != ID_SEPARATOR)
			m_itemIDArray.Add(pIDArray[i]);
	}
	ASSERT (m_imageList.GetImageCount() == m_itemIDArray.GetSize());
	
	//释放资源
	::UnlockResource(hGlb);
    ::FreeResource(hGlb);
	return TRUE;
}

//将普通菜单初始化为自画菜单
void DBMenu::InitMenuPopup(CMenu * pMenu)
{
	ASSERT(pMenu);
	ASSERT (::IsMenu(pMenu->m_hMenu));

	int nItem = pMenu->GetMenuItemCount();
	while ((--nItem) >= 0)
	{
		UINT nIDItem = pMenu->GetMenuItemID(nItem);

		MENUITEMINFO menuItemInfo;
		ZeroMemory(&menuItemInfo, sizeof (MENUITEMINFO));
		menuItemInfo.cbSize = sizeof (MENUITEMINFO);
		menuItemInfo.fMask = MIIM_DATA | MIIM_TYPE|MIIM_SUBMENU;
		if (pMenu->GetMenuItemInfo(nItem, &menuItemInfo, TRUE))
		{
			if (!(menuItemInfo.fType & MFT_OWNERDRAW)
				&& menuItemInfo.dwItemData == NULL)
			{
				CString strText;
				pMenu->GetMenuString(nItem, strText, MF_BYPOSITION);
				menuItemInfo.dwItemData = (DWORD) AddOwnerDrawItem(strText, nIDItem);

				int uType = menuItemInfo.fType & ~MFT_STRING;
				//uType &= ~MFT_SEPARATOR;
				uType &= ~MFT_BITMAP;
				uType |= MFT_OWNERDRAW;
				menuItemInfo.fType = uType;
				SetMenuItemInfo(pMenu->m_hMenu, nItem, TRUE, &menuItemInfo);
			}

			if (menuItemInfo.hSubMenu != NULL)
			{
				CMenu* pSubMenu = CMenu::FromHandle(menuItemInfo.hSubMenu);
				if (pSubMenu)
				{
					InitMenuPopup(pSubMenu);
				}
			}
		}		
	}
}

//释放将普通菜单初始化为自画菜单添加的资源
void DBMenu::UninitMenuPopup(HMENU hMenu)
{
	ASSERT (::IsMenu(hMenu));
	CMenu *pMenu = CMenu::FromHandle(hMenu);

	int nItem = pMenu->GetMenuItemCount();
	while ((--nItem) >= 0)
	{
		UINT nIDItem = pMenu->GetMenuItemID(nItem);

		MENUITEMINFO menuItemInfo;
		ZeroMemory(&menuItemInfo, sizeof (MENUITEMINFO));
		menuItemInfo.cbSize = sizeof (MENUITEMINFO);
		menuItemInfo.fMask = MIIM_DATA | MIIM_TYPE|MIIM_SUBMENU;
		if (pMenu->GetMenuItemInfo(nItem, &menuItemInfo, TRUE))
		{
			if ((menuItemInfo.fType & MFT_OWNERDRAW)
				&& menuItemInfo.dwItemData != NULL)
			{
				DRAWITEMDATA * pData = (DRAWITEMDATA *)menuItemInfo.dwItemData;
				int uType = menuItemInfo.fType & ~MFT_OWNERDRAW;
				if (nIDItem == 0)
					uType |= MFT_SEPARATOR;
				else
					uType |= MFT_STRING;
				menuItemInfo.fType = uType;
				menuItemInfo.fMask = MIIM_TYPE | MIIM_DATA;
				menuItemInfo.dwItemData = 0;

				if (SetMenuItemInfo(pMenu->m_hMenu, nItem, TRUE, &menuItemInfo))
				{
					if (nIDItem != 0)
						pMenu->ModifyMenu(nItem, MF_BYPOSITION | MF_STRING,	nIDItem, pData->cText);
					POSITION pos = m_listItemDataPtr.Find((void *)pData);
					if (pos != NULL)
					{
						delete pData;
						m_listItemDataPtr.RemoveAt(pos);
					}
				}
			}
			if (menuItemInfo.hSubMenu != NULL)
			{
				UninitMenuPopup(menuItemInfo.hSubMenu);
			}
		}		
	}
}

DBMenu::DRAWITEMDATA * DBMenu::AddOwnerDrawItem(LPCTSTR lpszText, const UINT nIDItem)
{
	DRAWITEMDATA *pItemData = new DRAWITEMDATA;
	ASSERT (pItemData != NULL);

	CString strText(lpszText);
	if (strText.GetLength() > 127)
		strText.Insert(124, TEXT("..."));
	strText = strText.Left(127);
	lstrcpy(pItemData->cText, strText);

	pItemData->iImageIndex = GetImageIndex(nIDItem);

	m_listItemDataPtr.AddTail(pItemData);
	return pItemData;
}

int DBMenu::GetImageIndex(UINT nIDItem)
{
	if (m_imageList.GetSafeHandle() == NULL)
		return -1;

	const int iImageCount = m_imageList.GetImageCount();
	for (int i = 0; i < iImageCount; i++)
		if (m_itemIDArray[i] == nIDItem)
			return i;

	return -1;
}

BOOL DBMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	if (lpDrawItemStruct != NULL && lpDrawItemStruct->CtlType == ODT_MENU)
	{
		UINT nIDItem = lpDrawItemStruct->itemID;
		UINT state = lpDrawItemStruct->itemState;
		CRect rect(lpDrawItemStruct->rcItem);
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		CMemDC memDC(pDC,rect.Width(), rect.Height());
		memDC.SetWindowOrg(rect.left, rect.top);
		DRAWITEMDATA* pItemData = (DRAWITEMDATA*) lpDrawItemStruct->itemData;
		
		if (pItemData)
		{	
			CRect rcLeft(rect.left, rect.top, rect.left + m_szImage.cx
								+ m_iGapTextAwayImage, rect.bottom);
			CRect rcRight(rcLeft.right, rect.top, rect.right, rect.bottom);
			CString strText(pItemData->cText);

			CFont* pOldFont = memDC.SelectObject(CFont::FromHandle(m_hFont));
			int nOldBkMode = memDC.SetBkMode(TRANSPARENT);

			//画整个菜单边框
			DrawFrame(pDC);	//这里的dc不能为内存dc
			
			//自画菜单

			if (nIDItem == 0)//画分割线
			{
				
				memDC.FillSolidRect(&rcLeft, m_clrMenuFaceDark);
				memDC.FillSolidRect(rcLeft.right - 1, rect.top, 
									1, rect.Height(), RGB(239,243,1245));
				memDC.FillSolidRect(&rcRight, m_clrBtnFace);
				
				//画线
				CRect rcBound(rcRight);
				if (rcBound.Height() % 2 == 1)
					rcBound.bottom += 1;
				rcBound.DeflateRect( m_iGapTextAwayImage / 2, rcBound.Height() / 2 - 1,
									0, rcBound.Height() / 2 - 1);
				memDC.Draw3dRect(&rcBound, m_clrMenuTextGray, RGB(255, 255, 255));			
			}
			else //画普通菜单项
			{
				//定义绘制菜单所需参数
				CRect rcImage(CPoint(rcLeft.left + m_rcBorders.left, rcLeft.top
								+ (rcLeft.Height() - m_szImage.cy) / 2), m_szImage);

				CRect rcText(rcRight);
				rcText.DeflateRect(m_iGapTextAwayImage / 2, m_rcBorders.top,
								   12 + m_rcBorders.right, m_rcBorders.bottom);
				
				BOOL bRatioCheck = FALSE;
				MENUITEMINFO menuItemInfo;
				ZeroMemory(&menuItemInfo, sizeof (MENUITEMINFO));
				menuItemInfo.cbSize = sizeof (MENUITEMINFO);
				menuItemInfo.fMask = MIIM_TYPE | MIIM_CHECKMARKS;
				if (GetMenuItemInfo((HMENU)lpDrawItemStruct->hwndItem, nIDItem, FALSE, &menuItemInfo))
				{
					if (menuItemInfo.fType & MFT_RADIOCHECK || menuItemInfo.hbmpChecked)
						bRatioCheck = TRUE;
				}

				//按相应的状态绘制菜单
				if (state & ODS_SELECTED) //菜单项处于选择状态
				{
					if (state & ODS_GRAYED)
					{	
						memDC.FillSolidRect(&rect, m_clrBtnHilight);
						memDC.FrameRect(&rect, &CBrush(m_clrBtnBorder));
						
						//绘制图标
						if (pItemData->bCustomImg)
						{
							DrawCustomImg(&memDC, rcImage, ODS_SELECTED, pItemData->nCustomImgID);
						}
						else
						{
							if (state & ODS_CHECKED)
							{
								if (bRatioCheck)
									DrawImage(&memDC, rcImage, ODS_GRAYED, m_imageListState, 1);
								else
									DrawImage(&memDC, rcImage, ODS_GRAYED, m_imageListState, 0);
							}
							else if (m_imageList.GetSafeHandle() != NULL && pItemData->iImageIndex != -1)
								DrawImage(&memDC, rcImage, ODS_GRAYED, m_imageList, pItemData->iImageIndex);
						}
						
						
						//绘制"disable"文本
						memDC.SetTextColor(RGB(255, 255, 255));
						rcText.OffsetRect(1, 1);
						DrawText(&memDC, strText, rcText);
						rcText.OffsetRect(-1, -1);
						memDC.SetTextColor(m_clrMenuTextGray);
						DrawText(&memDC, strText, rcText);
					}
					else
					{
						//绘制背景，边框
						memDC.FillSolidRect(&rect, m_clrBtnHilight);
						memDC.FrameRect(&rect, &CBrush(m_clrBtnBorder));
						
						//绘制图标
						
						if (pItemData->bCustomImg)
						{
							DrawCustomImg(&memDC, rcImage, 0, pItemData->nCustomImgID);
						}
						else
						{
							if (state & ODS_CHECKED)
							{
								if (bRatioCheck)
									DrawImage(&memDC, rcImage, ODS_SELECTED, m_imageListState, 1);
								else
									DrawImage(&memDC, rcImage, ODS_SELECTED, m_imageListState, 0);
							}
							else if (m_imageList.GetSafeHandle() != NULL && pItemData->iImageIndex != -1)
								DrawImage(&memDC, rcImage, ODS_SELECTED, m_imageList, pItemData->iImageIndex);

						}
						
						//绘制文本
						memDC.SetTextColor(m_clrMenuTextSel);
						DrawText(&memDC, strText, rcText);
					}
				}
				else //菜单项处于正常状态
				{
					if (state & ODS_GRAYED)
					{	
						memDC.FillSolidRect(&rcLeft, m_clrMenuFaceDark);
						memDC.FillSolidRect(rcLeft.right - 1, rect.top, 
											1, rect.Height(), RGB(239,243,245));
						memDC.FillSolidRect(&rcRight, m_clrBtnFace);						
						
						//绘制图标
						if (pItemData->bCustomImg)
						{
							DrawCustomImg(&memDC, rcImage, 0, pItemData->nCustomImgID);
						}
						else
						{
							if (state & ODS_CHECKED)
							{
								if (bRatioCheck)
									DrawImage(&memDC, rcImage, ODS_GRAYED, m_imageListState, 1);
								else
									DrawImage(&memDC, rcImage, ODS_GRAYED, m_imageListState, 0);
							}
							else if (m_imageList.GetSafeHandle() != NULL && pItemData->iImageIndex != -1)
								DrawImage(&memDC, rcImage, ODS_GRAYED, m_imageList, pItemData->iImageIndex);

						}

						
						//绘制"disable"文本
						memDC.SetTextColor(RGB(255, 255, 255));
						rcText.OffsetRect(1, 1);
						DrawText(&memDC, strText, rcText);
						rcText.OffsetRect(-1, -1);
						memDC.SetTextColor(m_clrMenuTextGray);
						DrawText(&memDC, strText, rcText);
					}
					else
					{
						memDC.FillSolidRect(&rcLeft, m_clrMenuFaceDark);
						memDC.FillSolidRect(rcLeft.right - 1, rect.top, 
											1, rect.Height(), RGB(239,243,245));
						memDC.FillSolidRect(&rcRight, m_clrBtnFace);
						
						//绘制图标
						if (pItemData->bCustomImg)
						{
							DrawCustomImg(&memDC, rcImage, 0, pItemData->nCustomImgID);
						}
						else
						{
							if (state & ODS_CHECKED)
							{
								if (bRatioCheck)
									DrawImage(&memDC, rcImage, 0, m_imageListState, 1);
								else
									DrawImage(&memDC, rcImage, 0, m_imageListState, 0);
							}
							else if (m_imageList.GetSafeHandle() != NULL && pItemData->iImageIndex != -1)
								DrawImage(&memDC, rcImage, ODS_SELECTED, m_imageList, pItemData->iImageIndex);

						}
						
						//绘制文本
						memDC.SetTextColor(m_clrMenuText);
						DrawText(&memDC, strText, rcText);
					}
				}	
			}		
			memDC.SelectObject(pOldFont);
			memDC.SetBkMode(nOldBkMode);
			pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(),
						&memDC, rect.left, rect.top, SRCCOPY);
		}
		return TRUE;
	}
	return FALSE;
}

void DBMenu::DrawText(CDC* pDC, CString& strText, CRect& rect)
{
	CString strLeft(strText), strRight;
	int iPos = strText.Find('\t');
	if (iPos != -1)
	{
		strLeft = strText.Left(iPos);
		strRight = strText.Mid(iPos + 1);
	}
	pDC->DrawText(strLeft, &rect, DT_LEFT | DT_VCENTER 
				  | DT_SINGLELINE | DT_EXPANDTABS);
	pDC->DrawText(strRight, &rect, DT_RIGHT | DT_VCENTER 
				  | DT_SINGLELINE | DT_EXPANDTABS);
}

void DBMenu::DrawImage(CDC *pDC, CRect &rect, UINT state, CImageList &imageList, int index)
{
	if (index == -1 || imageList.GetSafeHandle() == NULL)
		return;

	if (state & ODS_SELECTED)
	{	
		imageList.DrawIndirect(pDC, index, rect.TopLeft(), rect.Size(),
								 CPoint (0, 0), ILD_NORMAL, SRCCOPY,
								 CLR_NONE, CLR_DEFAULT);
	}
	else if (state & ODS_GRAYED)
	{
		HICON hIcon = imageList.ExtractIcon(index);
		
		pDC->DrawState(rect.TopLeft(), rect.Size(), hIcon, DST_ICON 
					   | DSS_DISABLED, (CBrush*) NULL);	
		DestroyIcon(hIcon);
	}
	else
	{
		imageList.DrawIndirect(pDC, index, rect.TopLeft(), rect.Size(),
								 CPoint (0, 0), ILD_BLEND25 , SRCCOPY,
								 CLR_NONE, RGB(255, 255, 255));
	}
}


BOOL DBMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (lpMeasureItemStruct->CtlType == ODT_MENU)
	{			
		UINT nIDItem = lpMeasureItemStruct->itemID;
		DRAWITEMDATA * pItemData = (DRAWITEMDATA *) lpMeasureItemStruct->itemData;
		if (pItemData)
		{
			if (nIDItem == 0) //为分割线
			{
				lpMeasureItemStruct->itemWidth  = 10;
				lpMeasureItemStruct->itemHeight = 5;
			}
			else
			{
				CString strText(pItemData->cText);
				CClientDC dc(AfxGetMainWnd());
				CFont *pFont = CFont::FromHandle(m_hFont);
				CFont *pOldFont = dc.SelectObject(pFont);
				CSize szText = dc.GetOutputTabbedTextExtent(strText, 0, NULL);
				if (!strText.IsEmpty())
				{
					lpMeasureItemStruct->itemHeight = max(m_szImage.cy, szText.cy) + m_rcBorders.top
														+ m_rcBorders.bottom;
					lpMeasureItemStruct->itemWidth  = szText.cx + m_szImage.cx + m_iGapTextAwayImage
														+ 12 + m_rcBorders.left + m_rcBorders.right;
					dc.SelectObject(pOldFont);
				}
				else	//设置空白项的默认宽度
				{
					lpMeasureItemStruct->itemHeight = max(m_szImage.cy, szText.cy) + m_rcBorders.top
														+ m_rcBorders.bottom;
					lpMeasureItemStruct->itemWidth  = 10;
				}
			}
			return TRUE;
		}
	}
	return FALSE;
}

void DBMenu::DrawFrame(CDC *pDC)
{
	return;

	ASSERT (pDC != NULL);
	CWnd* pWnd = pDC->GetWindow();
	if (pWnd != NULL)
	{
		CDC* pWinDC = pWnd->GetWindowDC();

		CRect rect;
		pWnd->GetWindowRect(&rect);
		
		CRect rcClient;
		pWnd->GetClientRect(&rcClient);
		pWnd->ClientToScreen(&rcClient);
		rcClient.OffsetRect(-rect.left, -rect.top);
		
		CRect rcBound(rect);
		rcBound.OffsetRect(-rect.left, -rect.top);

		pWinDC->Draw3dRect(&rcBound, RGB(191,213,249), RGB(75,134,190));
		rcBound.DeflateRect(1, 1);
		pWinDC->Draw3dRect(&rcBound, RGB(239,243,245), RGB(151,169,216));
		rcBound.DeflateRect(1, 1);
		pWinDC->Draw3dRect(&rcBound, RGB(239,243,245), RGB(191,213,249));
		rcBound.DeflateRect(1, 1);
		
		pWinDC->ExcludeClipRect(&rcClient);
		CRect rcClip(rcBound.left, rcBound.top,
					 rcClient.left + m_szImage.cx + 4
					 + m_iGapTextAwayImage / 2, rcBound.bottom);
		CRgn rgnClip;
		rgnClip.CreateRectRgnIndirect(&rcClip);
		pWinDC->SelectClipRgn(&rgnClip, RGN_AND);
		pWinDC->FillSolidRect(&rcBound, m_clrMenuFaceDark);
		pWinDC->SelectClipRgn(NULL);
		
		pWinDC->ExcludeClipRect(&rcClient);	
		pWinDC->SelectClipRgn(&rgnClip, RGN_DIFF);
		pWinDC->FillSolidRect(&rcBound, m_clrBtnFace);
		pWinDC->SelectClipRgn(NULL);
		
		pWnd->ReleaseDC(pWinDC);
	}
}

//父窗口为非框架窗口时更新菜单
void DBMenu::DoUpdate(CMenu * pMenu, CWnd * pParent)
{
	ASSERT(pMenu);
	ASSERT(pParent);

	int nItem = pMenu->GetMenuItemCount();
	CCmdUI state;
	state.m_pMenu = pMenu;
	state.m_nIndex = nItem - 1;
	state.m_nIndexMax = nItem;

	while ((--nItem) >= 0)
	{
		UINT nIDItem = pMenu->GetMenuItemID(nItem);
		if (nIDItem == (UINT) -1)
		{
			CMenu* menuPops = pMenu->GetSubMenu(nItem);
			if (menuPops)
				DoUpdate(menuPops, pParent);
		}
		else
		{
			if (nIDItem != 0)
			{
				state.m_nID = nIDItem;
				state.DoUpdate(pParent, TRUE);
			}
		}
		state.m_nIndex = nItem - 1;
	}
}

//给定一个字符检查是否为快捷键，通常在所有者窗口OnMenuChar中调用
BOOL DBMenu::OnMenuChar(UINT nChar, CMenu* pMenu, LRESULT * pResult)
{
	ASSERT(pMenu);
	ASSERT(pMenu->GetSafeHmenu());
	if (!IsSpawnMenu(pMenu))
		return FALSE;
	
	*pResult = 0;
	int nCount = pMenu->GetMenuItemCount();
	MENUITEMINFO menuItemInfo;
	ZeroMemory(&menuItemInfo, sizeof(MENUITEMINFO));
	menuItemInfo.cbSize = sizeof(MENUITEMINFO);
	menuItemInfo.fMask = MIIM_DATA | MIIM_TYPE | MIIM_ID;
	for (int index = 0; index < nCount; index++)
	{	
		if (pMenu->GetMenuItemInfo(index, &menuItemInfo, TRUE))
		{
			if (menuItemInfo.fType & MFT_OWNERDRAW)
			{
				DRAWITEMDATA * pItemData = (DRAWITEMDATA *) menuItemInfo.dwItemData;
				if (pItemData)
				{
					CString strItem(pItemData->cText);
					int iPos = strItem.Find('&');
					if (iPos >= 0)
					{
						strItem = strItem.Mid(iPos + 1, 1);//抽出加速字符
						if (strItem.CompareNoCase(CString(char(nChar))) == 0)
						{
							*pResult = MAKELONG(index, 2);
							break;
						}
					}
				}
			}
		}
	}
	return TRUE;
}

BOOL DBMenu::IsSpawnMenu(CMenu* pMenu)
{
	ASSERT(pMenu);
	ASSERT(pMenu->GetSafeHmenu());

	MENUITEMINFO menuItemInfo;
	ZeroMemory(&menuItemInfo, sizeof(MENUITEMINFO));
	menuItemInfo.cbSize = sizeof(MENUITEMINFO);
	menuItemInfo.fMask = MIIM_DATA | MIIM_TYPE;

	int nCount = pMenu->GetMenuItemCount();
	for (int i = 0; i < nCount; i++)
	{
		if (pMenu->GetMenuItemInfo(i, &menuItemInfo, TRUE))
			if (menuItemInfo.fType & MFT_OWNERDRAW)
				if (menuItemInfo.dwItemData)
					return TRUE;
	}
	return FALSE;
}

BOOL DBMenu::AddCustomBitmap( CMenu* pMenuIn,  UINT nMenuIDIn, UINT nBitmapIn , BOOL bPos)
{
	BOOL bRet = FALSE;
	if (!pMenuIn)
	{
		return bRet;
	}

	MENUITEMINFO menuItemInfo;

	ZeroMemory(&menuItemInfo, sizeof(MENUITEMINFO));
	menuItemInfo.cbSize = sizeof (MENUITEMINFO);
	menuItemInfo.fMask = MIIM_DATA;
	if(pMenuIn->GetMenuItemInfo(nMenuIDIn, &menuItemInfo, bPos))
	{
		((DBMenu::DRAWITEMDATA*) (menuItemInfo.dwItemData))->bCustomImg = TRUE;
		((DBMenu::DRAWITEMDATA*) (menuItemInfo.dwItemData))->nCustomImgID = nBitmapIn;
		bRet = TRUE;
	}

	return bRet;
}

void DBMenu::DrawCustomImg( CDC* pDC, CRect& rect, UINT state, UINT imgID )
{
	CImageList tmpImgList;
	CBitmap bmpTemp;
	bmpTemp.LoadBitmap(imgID);
	tmpImgList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 1, 1);
	tmpImgList.Add(&bmpTemp, RGB(192, 192, 192));
	DrawImage(pDC, rect,state, tmpImgList, 0);

}