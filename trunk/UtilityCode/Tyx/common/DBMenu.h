// DBMenu.h: interface for the DBMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUSPAWN_H__BB6F2F01_91FA_11D1_8B78_0000B43382FE__INCLUDED_)
#define AFX_MENUSPAWN_H__BB6F2F01_91FA_11D1_8B78_0000B43382FE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "MemDC.h"
#include <afxtempl.h>

class DBMenu  
{
public:
	struct ToolBarData	
	{ // struct for toolbar resource; guess you already know it;(for standard toolbar resource))
		WORD wVersion;
		WORD wWidth;			
		WORD wHeight;			
		WORD wItemCount;		
	};

	struct DRAWITEMDATA
	{
		TCHAR	cText[128];
		int		iImageIndex;
		BOOL	bCustomImg;
		UINT	nCustomImgID;
		DRAWITEMDATA()
		{
			bCustomImg = FALSE;
			nCustomImgID = 0;
		}

	};
public:
	BOOL OnMenuChar(UINT nChar, CMenu* pMenu, LRESULT * pResult);
	BOOL MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	BOOL DrawItem(LPDRAWITEMSTRUCT lp);
	void InitMenuPopup(CMenu * pMenu);
	void UninitMenuPopup(HMENU hMenu);
	BOOL AddToolBarResource(UINT nIDToolBar, UINT nIDBitmap = -1);
	void DoUpdate(CMenu * pMenu, CWnd * pParent);
	BOOL AddCustomBitmap(CMenu* pMenuIn,  UINT nMenuIDIn, UINT nBitmapIn , BOOL bPos = FALSE);

	DBMenu();
	~DBMenu();

private:	//function members
	
	DRAWITEMDATA * AddOwnerDrawItem(LPCTSTR lpszText, const UINT nIDItem);
	int GetImageIndex(UINT nIDItem);
	BOOL IsSpawnMenu(CMenu * pMenu);
	void DrawText(CDC *pDC, CString& strText, CRect &rect);
	void DrawImage(CDC* pDC, CRect& rect, UINT state, CImageList& imageList, int index);
	void DrawFrame(CDC *pDC);
	void DrawCustomImg(CDC* pDC, CRect& rect, UINT state, UINT imgID);


private:	//data members
	
	
	COLORREF m_clrMenuText, m_clrMenuTextSel, m_clrMenuTextGray;
	COLORREF m_clrMenuFaceDark;
	COLORREF m_clrBtnFace, m_clrBtnHilight, m_clrBtnBorder;
	
	CRect m_rcBorders;
	int m_iGapTextAwayImage;
	HFONT	m_hFont;

	CUIntArray m_itemIDArray;
	CImageList m_imageList;
	CImageList m_imageListState;
	CSize m_szImage;

	CPtrList m_listItemDataPtr;
};

#endif // !defined(AFX_MENUSPAWN_H__BB6F2F01_91FA_11D1_8B78_0000B43382FE__INCLUDED_)
