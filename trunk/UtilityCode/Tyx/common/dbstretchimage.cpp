// DBStretchImage.cpp: implementation of the DBStretchImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DBStretchImage.h"
#include <math.h>
#include "resource.h"
#include <algorithm>
#include "afxmt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMutex g_mutexImgDivData(NULL, TEXT("{DB8C38B4-0A33-451b-8BB9-47B2FE1CA019}"));
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL DBStretchImage::m_sbInited = FALSE;
MAP_IMGDIVDATA* DBStretchImage::m_spmap_ImgDivData = NULL;


DBStretchImage::DBStretchImage()
{
	if (!m_sbInited)
	{
		InitStaticData();
	}
	m_hBmpDib = NULL;
	m_bEdgeStretch = FALSE;
}

DBStretchImage::~DBStretchImage()
{
	::DeleteObject(m_hBmpDib);
}


//功能：装载位图
//-------------------------------
//nIDResource		位图资源ID
//iDividePosFir		第一个分割位置（位图分位三段，中间部分为填充区）
//m_iDividePosSec	第二个分割位置
//bHStretch			填充方式(水平填充或垂直填充)
//-------------------------------
void DBStretchImage::LoadBitmap(UINT nIDResource,
								int iDividePosFir, int iDividePosSec, BOOL bHStretch)
{
	ASSERT(m_bmpDivData.m_pbmpStart = NULL);

	m_divKey.bEdgeStretch = FALSE;
	m_divKey.bHStretch = bHStretch;
	m_divKey.nPosFir = iDividePosFir;
	m_divKey.nPosSec = iDividePosSec;
	m_divKey.strPath = 	MAKEINTRESOURCE(nIDResource);
	m_divKey.nBtnPart = -2;

	m_iDividePosFir = iDividePosFir;
	m_iDividePosSec = iDividePosSec;
	m_bHStretch = bHStretch;
	
	if (!GetStaticBmpData(m_divKey, m_bmpDivData))
	{
		ASSERT(FALSE);
	}
	m_szSrcBmp.cx = m_bmpDivData.nWidth;
	m_szSrcBmp.cy = m_bmpDivData.nHeight;

// 	CBitmap bitmapSrc;
// 	if( bitmapSrc.LoadBitmap(nIDResource) )
// 		SetBitmap(&bitmapSrc);
}

//功能：装载位图
//-------------------------------
//nIDResource		位图资源ID
//iDividePosFir		第一个分割位置（位图分位三段，中间部分为填充区）
//m_iDividePosSec	第二个分割位置
//bHStretch			填充方式
//-------------------------------
void DBStretchImage::LoadBitmap(LPCSTR lpszBitmap, 
								int iDividePosFir, int iDividePosSec, BOOL bHStretch, BOOL bEdgeStretch/* = FALSE*/)
{

	m_iDividePosFir = iDividePosFir;
	m_iDividePosSec = iDividePosSec;
	m_bHStretch = bHStretch;
	m_bEdgeStretch = bEdgeStretch;


	m_divKey.strPath = lpszBitmap;
	m_divKey.nPosFir = iDividePosFir;
	m_divKey.nPosSec = iDividePosSec;
	m_divKey.bHStretch = bHStretch;
	m_divKey.bEdgeStretch = bEdgeStretch;
	if (!GetStaticBmpData(m_divKey, m_bmpDivData))
	{
		ASSERT(FALSE);
		return;
	}
	m_szSrcBmp.cx = m_bmpDivData.nWidth;
	m_szSrcBmp.cy = m_bmpDivData.nHeight;


	return;


	ASSERT(FALSE);
	return;
	CBitmap& m_bmpMiddle = *m_bmpDivData.m_pbmpMiddle;
	CBitmap& m_bmpStart = *m_bmpDivData.m_pbmpStart;
	CBitmap& m_bmpEnd = *m_bmpDivData.m_pbmpEnd;

	m_strBmpName = lpszBitmap;
	ASSERT(m_bmpStart.GetSafeHandle() == NULL);
	ASSERT(m_bmpMiddle.GetSafeHandle() == NULL);
	ASSERT(m_bmpEnd.GetSafeHandle() == NULL);
	
	CBitmap bitmapSrc;

	HBITMAP hBmp = (HBITMAP)::LoadImage(NULL, IM_MACRO_GET_OCX_FILE_PATH(lpszBitmap), 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	CString fileFullPath = lpszBitmap;

#ifdef USED_IN_OCX_TYPE
	if (hBmp == NULL)
	{
		TCHAR szTemp[MAX_PATH];
		TCHAR szPath[MAX_PATH],szDrive[MAX_PATH];
		//GetModuleFileName(NULL,szTemp, sizeof(szTemp)/sizeof(TCHAR));
		_wsplitpath(AfxGetApp()->m_pszHelpFilePath,szDrive,szPath,NULL,NULL);

		CString strFilePath(szDrive);
		strFilePath += szPath;

		fileFullPath = strFilePath + lpszBitmap;
		hBmp = (HBITMAP)::LoadImage(NULL, strFilePath + lpszBitmap, 
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}
#else
	if (hBmp == NULL)
	{
		TCHAR szTemp[MAX_PATH];
		TCHAR szPath[MAX_PATH],szDrive[MAX_PATH];
		GetModuleFileName(NULL,szTemp, sizeof(szTemp)/sizeof(TCHAR));
		_splitpath(szTemp,szDrive,szPath,NULL,NULL);

		CString strFilePath(szDrive);
		strFilePath += szPath;

		fileFullPath = strFilePath + lpszBitmap;
		hBmp = (HBITMAP)::LoadImage(NULL, strFilePath + lpszBitmap, 
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

#endif // USED_IN_OCX_TYPE

	
	//ASSERT(hBmp != NULL);
	if (hBmp == NULL)
	{
		ASSERT(FALSE);
		CString strMsg;
		CString strBuff;	
		//strBuff.LoadString(IDS_STR_ERR_BMP_INVALID);
		strMsg.Format(TEXT("error load bmp = %s"), fileFullPath);
		AfxMessageBox(strMsg, MB_OK);
		exit(0);
	}

	if( bitmapSrc.Attach(hBmp))
		SetBitmap(&bitmapSrc);
	bitmapSrc.DeleteObject();
}

//功能：装载位图
//-------------------------------
//hBmp				位图句柄
//iDividePosFir		第一个分割位置（位图分位三段，中间部分为填充区）
//m_iDividePosSec	第二个分割位置
//bHStretch			填充方式
//-------------------------------
//注意：hBmp不会被删除，你必须自己释放hBmp

void DBStretchImage::LoadBitmap(HBITMAP hBmp, 
								int iDividePosFir, int iDividePosSec, BOOL bHStretch, BOOL bEdgeStretch/* = FALSE*/)
{

	ASSERT(FALSE);
	return;
	CBitmap& m_bmpMiddle = *m_bmpDivData.m_pbmpMiddle;
	CBitmap& m_bmpStart = *m_bmpDivData.m_pbmpStart;
	CBitmap& m_bmpEnd = *m_bmpDivData.m_pbmpEnd;

	ASSERT(hBmp != NULL);
	
	ASSERT(m_bmpStart.GetSafeHandle() == NULL);
	ASSERT(m_bmpMiddle.GetSafeHandle() == NULL);
	ASSERT(m_bmpEnd.GetSafeHandle() == NULL);
	
	m_iDividePosFir = iDividePosFir;
	m_iDividePosSec = iDividePosSec;
	m_bHStretch = bHStretch;
	m_bEdgeStretch = bEdgeStretch;
	SetBitmap(CBitmap::FromHandle(hBmp));
}

void DBStretchImage::LoadBitmap( int nBtnPart, LPCSTR lpszBitmap, int iDividePosFir, int iDividePosSec, BOOL bHStretch /*= TRUE*/, BOOL bEdgeStretch /*= FALSE*/ )
{

	m_divKey.nBtnPart = nBtnPart;
	m_divKey.strPath = lpszBitmap;
	m_divKey.nPosFir = iDividePosFir;
	m_divKey.nPosSec = iDividePosSec;
	m_divKey.bHStretch = bHStretch;
	m_divKey.bEdgeStretch = bEdgeStretch;

	m_iDividePosFir = iDividePosFir;
	m_iDividePosSec = iDividePosSec;
	m_bHStretch = bHStretch;
	m_bEdgeStretch = bEdgeStretch;

	if (!GetStaticBmpData(m_divKey, m_bmpDivData))
	{
		ASSERT(FALSE);
		return;
	}
	m_szSrcBmp.cx = m_bmpDivData.nWidth;
	m_szSrcBmp.cy = m_bmpDivData.nHeight;


}


//功能：绘制图象
//------------------------
//pDC	指向CDC类的指针
//pt	绘制原点
//size	绘制区尺寸
//------------------------
void DBStretchImage::Render(CDC *pDC, CPoint pt, CSize size)
{
	if (m_bEdgeStretch)
	{
		EdgeStretchRender(pDC, pt, size);
		return;
	}
	if (size.cx<=0||size.cy<=0) {
		return;
	}
	ASSERT(pDC != NULL);

	CBitmap& m_bmpMiddle = *m_bmpDivData.m_pbmpMiddle;
	CBitmap& m_bmpStart = *m_bmpDivData.m_pbmpStart;
	CBitmap& m_bmpEnd = *m_bmpDivData.m_pbmpEnd;

	if (m_hBmpDib != NULL)
	{
		CSize szOriginal(Width(), Height());
		if ((size.cx <= szOriginal.cx && size.cy <= szOriginal.cy)
			|| (size.cx >= szOriginal.cx && size.cy >= szOriginal.cy))
		{
			CDC dcMem;
			dcMem.CreateCompatibleDC(pDC);
			HBITMAP hBmpTemp = DBStretchImage::STRENTCH::ScaleBitmap(pDC->GetSafeHdc(),
				m_hBmpDib, size.cx, size.cy, TRUE); 
			ASSERT(hBmpTemp != NULL);
			CBitmap *pOldBmp = (CBitmap *)dcMem.SelectObject(CBitmap::FromHandle(hBmpTemp));
			pDC->BitBlt(pt.x, pt.y, size.cx, size.cy, 
				&dcMem, 0, 0, SRCCOPY);
			dcMem.SelectObject(pOldBmp);
			::DeleteObject(hBmpTemp);
		}
		else
		{
			CDC dcMem;
			dcMem.CreateCompatibleDC(pDC);
			CBitmap *pOldBmp = (CBitmap *)dcMem.SelectObject(CBitmap::FromHandle(m_hBmpDib));
			pDC->StretchBlt(pt.x, pt.y, size.cx, size.cy, 
				&dcMem, 0, 0, szOriginal.cx, szOriginal.cy, SRCCOPY);
			dcMem.SelectObject(pOldBmp);
		}
	}
	else
	{
		if (m_bmpStart.m_hObject == NULL)
		{
			ASSERT(FALSE);
			return;
		}
		ASSERT (m_bmpStart.m_hObject != NULL);
		ASSERT (m_bmpMiddle.m_hObject != NULL);
		ASSERT (m_bmpEnd.m_hObject != NULL);
		
		if( !m_bmpStart.m_hObject ||!m_bmpMiddle.m_hObject||!m_bmpEnd.m_hObject){
			
			CRect rect;
			rect.SetRect( pt.x,pt.y,pt.x+size.cx,pt.y+size.cy );
			pDC->FillSolidRect( rect,GetSysColor( COLOR_3DFACE ));
			return;
		}
		
		if (m_bHStretch)
		{
			CMemDC dc(pDC, size.cx,m_szSrcBmp.cy);
			if (dc.m_hDC == 0)
			{
				return;
			}
			if (!dc.GetSafeHdc())
			{
				return;
			}
			UINT uWidthBmpStart = m_iDividePosFir;//位图m_bmpStart的宽度
			UINT uWidthBmpMiddle = m_iDividePosSec - m_iDividePosFir;//位图m_bmpMiddle的宽度
			UINT uWidthBmpEnd = m_szSrcBmp.cx - m_iDividePosSec;//位图m_bmpEnd的宽度

			if (((UINT)size.cx) <= min(uWidthBmpStart, uWidthBmpEnd)*2)//位图m_bmpStart和m_bmpEnd各占一半空间
			{
				dc.DrawState(CPoint(size.cx - uWidthBmpEnd, 0),
					CSize(uWidthBmpEnd, m_szSrcBmp.cy),
					&m_bmpEnd,
					DST_BITMAP | DSS_NORMAL);
				dc.DrawState(CPoint(0, 0),
					CSize(size.cx/2, m_szSrcBmp.cy),
					&m_bmpStart,
					DST_BITMAP | DSS_NORMAL);
			}
			else if(((UINT)size.cx)<=uWidthBmpStart+uWidthBmpEnd)//m_bmpStart和m_bmpEnd中较窄者不剪裁,较宽者剪裁输出
			{
				if (uWidthBmpStart>uWidthBmpEnd)
				{
					dc.DrawState(CPoint(0,0),
						CSize(uWidthBmpStart,m_szSrcBmp.cy),
						&m_bmpStart,
						DST_BITMAP|DSS_NORMAL);
					dc.DrawState(CPoint(size.cx-uWidthBmpEnd,0),
						CSize(uWidthBmpEnd,m_szSrcBmp.cy),
						&m_bmpEnd,
						DST_BITMAP|DSS_NORMAL);
				}
				else
				{
					dc.DrawState(CPoint(size.cx-uWidthBmpEnd,0),
						CSize(uWidthBmpEnd,m_szSrcBmp.cy),
						&m_bmpEnd,
						DST_BITMAP|DSS_NORMAL);
					dc.DrawState(CPoint(0,0),
						CSize(uWidthBmpStart,m_szSrcBmp.cy),
						&m_bmpStart,
						DST_BITMAP|DSS_NORMAL);
				}
			}
			else
			{
				//画位图m_bmpStart
				dc.DrawState(CPoint(0,0),CSize(uWidthBmpStart,m_szSrcBmp.cy),&m_bmpStart,DST_BITMAP|DSS_NORMAL);
				//画位图m_bmpMiddle
				int xOrg = uWidthBmpStart;
				while (xOrg<size.cx)
				{
					dc.DrawState(CPoint(xOrg,0),
						CSize(uWidthBmpMiddle,m_szSrcBmp.cy),
						&m_bmpMiddle,
						DST_BITMAP|DSS_NORMAL);
					xOrg+=uWidthBmpMiddle;
				}
				//画位图m_bmpEnd
				dc.DrawState(CPoint(size.cx-uWidthBmpEnd,0),
					CSize(uWidthBmpEnd,
					m_szSrcBmp.cy),
					&m_bmpEnd,
					DST_BITMAP|DSS_NORMAL);
			}	
			//draw the MemDC to pDC(如果需要，则在y方向拉伸位图)
			pDC->StretchBlt(pt.x, pt.y, size.cx, size.cy,
				&dc, 0, 0, size.cx, m_szSrcBmp.cy, SRCCOPY);
		}
		else
		{
			CMemDC dc(pDC, m_szSrcBmp.cx,size.cy);
			if (!dc.GetSafeHdc())
			{
				return;
			}
/*
			CDC dc;
			dc.CreateCompatibleDC(pDC);
			CBitmap tmpBmp;
			tmpBmp.CreateBitmap(m_szSrcBmp.cx,size.cy, 1, 32, 0);
			dc.SelectObject(tmpBmp);
*/
			UINT uHeightBmpStart=m_iDividePosFir;//位图m_bmpStart的宽度
			UINT uHeightBmpMiddle=m_iDividePosSec-m_iDividePosFir;//位图m_bmpMiddle的宽度
			UINT uHeightBmpEnd=m_szSrcBmp.cy-m_iDividePosSec;//位图m_bmpEnd的宽度
			
			if (((UINT)size.cy)<=min(uHeightBmpStart,uHeightBmpEnd)*2)//位图m_bmpStart和m_bmpEnd各占一半空间
			{
				dc.DrawState(CPoint(0,size.cy-uHeightBmpEnd),
					CSize(m_szSrcBmp.cx,uHeightBmpEnd),
					&m_bmpEnd,
					DST_BITMAP|DSS_NORMAL);
				dc.DrawState(CPoint(0,0),
					CSize(m_szSrcBmp.cx,size.cy/2),
					&m_bmpStart,
					DST_BITMAP|DSS_NORMAL);
			}
			else if(((UINT)size.cy)<=uHeightBmpStart+uHeightBmpEnd)//m_bmpStart和m_bmpEnd长度较小者不剪裁,较长者剪裁输出
			{
				if (uHeightBmpStart>uHeightBmpEnd)
				{
					dc.DrawState(CPoint(0,0),CSize(m_szSrcBmp.cx,uHeightBmpStart),&m_bmpStart,DST_BITMAP|DSS_NORMAL);
					dc.DrawState(CPoint(0,size.cy-uHeightBmpEnd),CSize(m_szSrcBmp.cx,uHeightBmpEnd),&m_bmpEnd,DST_BITMAP|DSS_NORMAL);
				}
				else
				{
					dc.DrawState(CPoint(0,size.cy-uHeightBmpEnd),CSize(m_szSrcBmp.cx,uHeightBmpEnd),&m_bmpEnd,DST_BITMAP|DSS_NORMAL);
					dc.DrawState(CPoint(0,0),CSize(m_szSrcBmp.cx,uHeightBmpStart),&m_bmpStart,DST_BITMAP|DSS_NORMAL);
				}
			}
			else
			{
				//画位图m_bmpStart
				dc.DrawState(CPoint(0,0),
					CSize(m_szSrcBmp.cx,uHeightBmpStart),
					&m_bmpStart,
					DST_BITMAP|DSS_NORMAL);
				
				//画位图m_bmpMiddle
				int yOrg = uHeightBmpStart;
				while (yOrg < size.cy)
				{
					dc.DrawState(CPoint(0,yOrg),
						CSize(m_szSrcBmp.cx,uHeightBmpMiddle),
						&m_bmpMiddle,
						DST_BITMAP|DSS_NORMAL);
					yOrg += uHeightBmpMiddle;
				}
				
				//画位图m_bmpEnd
				dc.DrawState(CPoint(0,size.cy-uHeightBmpEnd),
					CSize(m_szSrcBmp.cx,uHeightBmpEnd),
					&m_bmpEnd,
					DST_BITMAP|DSS_NORMAL);
			}	
			//draw the MemDC to pDC(如果需要，则在x方向拉伸位图)
			pDC->StretchBlt(pt.x, pt.y, size.cx, size.cy,
				&dc, 0, 0, m_szSrcBmp.cx, size.cy, SRCCOPY);
		}
	}
}


//返回位图的高度
int DBStretchImage::Height() const
{
	ASSERT(m_szSrcBmp.cy != 0);
	return m_szSrcBmp.cy;
}

//返回位图的宽度
int DBStretchImage::Width() const
{
	ASSERT(m_szSrcBmp.cx != 0);
	return m_szSrcBmp.cx;
}

//功能：设置位图。注意：不改变分割点位置
//-------------------------
//pBitmap	指向位图对象的指针
//-------------------------
void DBStretchImage::SetBitmap(CBitmap *pBitmap)
{
	ASSERT(pBitmap != NULL);
	ASSERT(pBitmap->GetSafeHandle() != NULL);
	
	ASSERT(FALSE);
	return;
	CBitmap& m_bmpMiddle = *m_bmpDivData.m_pbmpMiddle;
	CBitmap& m_bmpStart = *m_bmpDivData.m_pbmpStart;
	CBitmap& m_bmpEnd = *m_bmpDivData.m_pbmpEnd;

	m_bmpStart.DeleteObject();
	m_bmpMiddle.DeleteObject();	
	m_bmpEnd.DeleteObject();
	
	//get the bitmap size.
	BITMAP bitmap;
	ZeroMemory(&bitmap, sizeof(bitmap));
	int nRet = pBitmap->GetBitmap(&bitmap);
	if (nRet == 0)
	{
		nRet = GetLastError();
		ASSERT(FALSE);
	}
	m_szSrcBmp.cx = bitmap.bmWidth;
	m_szSrcBmp.cy = bitmap.bmHeight;
	
	ASSERT (m_iDividePosSec > m_iDividePosFir);
	if (m_bHStretch)
		ASSERT (m_iDividePosSec < m_szSrcBmp.cx);
	else
		ASSERT (m_iDividePosSec < m_szSrcBmp.cy);
	
	//创建 bitmaps.
	CDC *pDC = (CWnd::FromHandle(GetDesktopWindow()))->GetDC();
	
	CDC dcSrc;
	dcSrc.CreateCompatibleDC(pDC);
	CDC dcDes;
	dcDes.CreateCompatibleDC(pDC);
	if (m_bHStretch)
	{
		m_bmpStart.CreateCompatibleBitmap(pDC,
			m_iDividePosFir, m_szSrcBmp.cy);
		m_bmpMiddle.CreateCompatibleBitmap(pDC, 
			m_iDividePosSec - m_iDividePosFir, m_szSrcBmp.cy);
		m_bmpEnd.CreateCompatibleBitmap(pDC,
			m_szSrcBmp.cx-m_iDividePosSec, m_szSrcBmp.cy);
	}
	else
	{
		m_bmpStart.CreateCompatibleBitmap(pDC,
			m_szSrcBmp.cx, m_iDividePosFir);
		m_bmpMiddle.CreateCompatibleBitmap(pDC,
			m_szSrcBmp.cx, m_iDividePosSec - m_iDividePosFir);
		m_bmpEnd.CreateCompatibleBitmap(pDC,
			m_szSrcBmp.cx, m_szSrcBmp.cy - m_iDividePosSec);
	}
	
	//创建 m_bmpStart.
	CBitmap *pOldBmpSrc = dcSrc.SelectObject(pBitmap);
	CBitmap *pOldBmpDes = dcDes.SelectObject(&m_bmpStart);
	if (m_bHStretch)
		dcDes.BitBlt(0, 0, m_iDividePosFir, m_szSrcBmp.cy,
		&dcSrc, 0, 0, SRCCOPY);
	else
		dcDes.BitBlt(0, 0, m_szSrcBmp.cx, m_iDividePosFir,
		&dcSrc, 0, 0, SRCCOPY);
	
	//创建 m_bmpMiddle.
	dcDes.SelectObject(&m_bmpMiddle);
	if (m_bHStretch)
		dcDes.BitBlt(0, 0, m_iDividePosSec - m_iDividePosFir, m_szSrcBmp.cy,
		&dcSrc, m_iDividePosFir, 0, SRCCOPY);
	else
		dcDes.BitBlt(0, 0, m_szSrcBmp.cx, m_iDividePosSec - m_iDividePosFir,
		&dcSrc, 0, m_iDividePosFir, SRCCOPY);
	
	//创建 m_bmpEnd.
	dcDes.SelectObject(&m_bmpEnd);
	if (m_bHStretch)
		dcDes.BitBlt(0, 0, m_szSrcBmp.cx - m_iDividePosSec, m_szSrcBmp.cy,
		&dcSrc, m_iDividePosSec, 0, SRCCOPY);
	else
		dcDes.BitBlt(0, 0, m_szSrcBmp.cx, m_szSrcBmp.cy - m_iDividePosSec,
		&dcSrc, 0, m_iDividePosSec, SRCCOPY);
	
	dcSrc.SelectObject(pOldBmpSrc);
	dcSrc.DeleteDC();
	dcDes.SelectObject(pOldBmpDes);
	dcDes.DeleteDC();
	
	::ReleaseDC(GetDesktopWindow(),pDC->m_hDC);
}

BOOL DBStretchImage::IsEmpty()
{
	BOOL bRet = FALSE;
	if (m_bmpDivData.m_pbmpMiddle && m_bmpDivData.m_pbmpStart &&
		m_bmpDivData.m_pbmpEnd)
	{
		CBitmap& m_bmpMiddle = *m_bmpDivData.m_pbmpMiddle;
		CBitmap& m_bmpStart = *m_bmpDivData.m_pbmpStart;
		CBitmap& m_bmpEnd = *m_bmpDivData.m_pbmpEnd;

		bRet = (m_bmpStart.m_hObject != NULL
			&& m_bmpMiddle.m_hObject != NULL
			&& m_bmpEnd.m_hObject != NULL);
	}
	
	return !bRet;

	
}


BOOL DBStretchImage::LoadDibSection(LPCTSTR lpszBmpPath)
{
	ASSERT(FALSE);
	return FALSE;
	ASSERT(m_hBmpDib == NULL);

	m_hBmpDib = (HBITMAP)::LoadImage(NULL, IM_MACRO_GET_OCX_FILE_PATH(lpszBmpPath), 
		IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	if (m_hBmpDib == NULL)
	{
		TCHAR szTemp[MAX_PATH];
		TCHAR szPath[MAX_PATH],szDrive[MAX_PATH];
		GetModuleFileName(NULL,szTemp, sizeof(szTemp)/sizeof(TCHAR));
		_splitpath(szTemp,szDrive,szPath,NULL,NULL);
		
		CString strFilePath(szDrive);
		strFilePath += szPath;

		m_hBmpDib = (HBITMAP)::LoadImage(NULL, strFilePath + lpszBmpPath, 
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	}

	ASSERT(m_hBmpDib != NULL);
	
	BITMAP bitmap;
	if (::GetObject(m_hBmpDib, sizeof (bitmap), &bitmap) == 0)
	{
		ASSERT(0);
		return FALSE;
	}
	
	m_szSrcBmp.cx = bitmap.bmWidth;
	m_szSrcBmp.cy = bitmap.bmHeight;

	return (m_hBmpDib != NULL);
}

void DBStretchImage::RenderTransparent( CDC *pDC,CPoint pt,CSize size )
{
	CMemDC memDC(pDC, size.cx, size.cy);
	CPoint ptMem(0,0);
	Render(&memDC, ptMem, size);
	pDC->TransparentBlt(pt.x,pt.y, size.cx, size.cy, &memDC, 0, 0,size.cx,size.cy,TRANS_PARENT_CLR);
}

void DBStretchImage::EdgeStretchRender( CDC *pDC,CPoint pt,CSize size )
{

	if (size.cx<=0||size.cy<=0) {
		return;
	}
	ASSERT(pDC != NULL);

	CBitmap& m_bmpMiddle = *m_bmpDivData.m_pbmpMiddle;
	CBitmap& m_bmpStart = *m_bmpDivData.m_pbmpStart;
	CBitmap& m_bmpEnd = *m_bmpDivData.m_pbmpEnd;


	if (m_hBmpDib != NULL)
	{
		CSize szOriginal(Width(), Height());
		if ((size.cx <= szOriginal.cx && size.cy <= szOriginal.cy)
			|| (size.cx >= szOriginal.cx && size.cy >= szOriginal.cy))
		{
			CDC dcMem;
			dcMem.CreateCompatibleDC(pDC);
			HBITMAP hBmpTemp = DBStretchImage::STRENTCH::ScaleBitmap(pDC->GetSafeHdc(),
				m_hBmpDib, size.cx, size.cy, TRUE); 
			ASSERT(hBmpTemp != NULL);
			CBitmap *pOldBmp = (CBitmap *)dcMem.SelectObject(CBitmap::FromHandle(hBmpTemp));
			pDC->BitBlt(pt.x, pt.y, size.cx, size.cy, 
				&dcMem, 0, 0, SRCCOPY);
			dcMem.SelectObject(pOldBmp);
			::DeleteObject(hBmpTemp);
		}
		else
		{
			CDC dcMem;
			dcMem.CreateCompatibleDC(pDC);
			CBitmap *pOldBmp = (CBitmap *)dcMem.SelectObject(CBitmap::FromHandle(m_hBmpDib));
			pDC->StretchBlt(pt.x, pt.y, size.cx, size.cy, 
				&dcMem, 0, 0, szOriginal.cx, szOriginal.cy, SRCCOPY);
			dcMem.SelectObject(pOldBmp);
		}
	}
	else
	{

 		ASSERT (m_bmpStart.m_hObject != NULL);
 		ASSERT (m_bmpMiddle.m_hObject != NULL);
 		ASSERT (m_bmpEnd.m_hObject != NULL);
		

		if( !m_bmpStart.m_hObject ||!m_bmpMiddle.m_hObject||!m_bmpEnd.m_hObject){

			CRect rect;
			rect.SetRect( pt.x,pt.y,pt.x+size.cx,pt.y+size.cy );
			pDC->FillSolidRect( rect,GetSysColor( COLOR_3DFACE ));
			return;
		}

		if (m_bHStretch)
		{
			CMemDC dc(pDC, size.cx,m_szSrcBmp.cy);
			if (dc.m_hDC == 0)
			{
				return;
			}
			if (!dc.GetSafeHdc())
			{
				return;
			}
			UINT uWidthBmpStart = m_iDividePosFir;//位图m_bmpStart的宽度
			UINT uWidthBmpMiddle = m_iDividePosSec - m_iDividePosFir;//位图m_bmpMiddle的宽度
			UINT uWidthBmpEnd = m_szSrcBmp.cx - m_iDividePosSec;//位图m_bmpEnd的宽度
			ASSERT(uWidthBmpEnd > 0);
			ASSERT(uWidthBmpStart> 0);
			if (((UINT)size.cx) <= uWidthBmpMiddle)//位图m_bmpStart和m_bmpEnd各占一半空间
			{
				dc.DrawState(pt,
					CSize(uWidthBmpMiddle, m_szSrcBmp.cy),
					&m_bmpMiddle,
					DST_BITMAP | DSS_NORMAL);
			}
			else
			{

				CRect rcMid(0,0, uWidthBmpMiddle, m_szSrcBmp.cy);
				CRect rcDraw(pt.x, pt.y, size.cx, size.cy);
				rcMid = GetCenterRect(rcDraw, rcMid);
				//draw mid
				dc.DrawState(rcMid.TopLeft(),
					rcMid.Size(),
					&m_bmpMiddle,
					DST_BITMAP|DSS_NORMAL);
				//draw left
				
				CRect rcLeft(pt.x, pt.y, rcMid.left, rcMid.bottom);
				if (!rcLeft.IsRectEmpty())
				{
					int nLeftRemain = rcLeft.Width();
					int nLeftDrawWidth = min(nLeftRemain, uWidthBmpStart);
					int nLeftPos = pt.x;
					while ( nLeftRemain > 0)
					{
						dc.DrawState(CPoint(nLeftPos, rcLeft.top),
							CSize(nLeftDrawWidth, rcLeft.Height()),&m_bmpStart,DST_BITMAP|DSS_NORMAL);
						nLeftRemain -= uWidthBmpStart;
						nLeftDrawWidth = min(nLeftRemain, uWidthBmpStart);
						nLeftPos += uWidthBmpStart;
					}
					

				}

				CRect rcRight(rcMid.right, rcMid.top, pt.x + size.cx, pt.y +size.cy);
				if (!rcRight.IsRectEmpty())
				{
					int nRightRemain = rcRight.Width();
					int nRightDrawWidth = min(nRightRemain, uWidthBmpEnd);
					int nRightPos = rcMid.right;
					while(nRightRemain > 0)
					{
						dc.DrawState(CPoint(nRightPos, rcRight.top),
							CSize(nRightDrawWidth, rcRight.Height()),
							&m_bmpEnd,
							DST_BITMAP|DSS_NORMAL);
						nRightRemain -= uWidthBmpEnd;
						nRightDrawWidth = min(nRightRemain, uWidthBmpEnd);
						nRightPos += uWidthBmpEnd;
					}
					
				}
			}
			//draw the MemDC to pDC(如果需要，则在y方向拉伸位图)
			pDC->StretchBlt(pt.x, pt.y, size.cx, size.cy,
				&dc, 0, 0, size.cx, m_szSrcBmp.cy, SRCCOPY);
		}
		else
		{
			CMemDC dc(pDC, m_szSrcBmp.cx,size.cy);
			if (!dc.GetSafeHdc())
			{
				return;
			}
			/*
			CDC dc;
			dc.CreateCompatibleDC(pDC);
			CBitmap tmpBmp;
			tmpBmp.CreateBitmap(m_szSrcBmp.cx,size.cy, 1, 32, 0);
			dc.SelectObject(tmpBmp);
			*/
			UINT uHeightBmpStart=m_iDividePosFir;//位图m_bmpStart的宽度
			UINT uHeightBmpMiddle=m_iDividePosSec-m_iDividePosFir;//位图m_bmpMiddle的宽度
			UINT uHeightBmpEnd=m_szSrcBmp.cy-m_iDividePosSec;//位图m_bmpEnd的宽度

			if (((UINT)size.cy)<=min(uHeightBmpStart,uHeightBmpEnd)*2)//位图m_bmpStart和m_bmpEnd各占一半空间
			{
				dc.DrawState(CPoint(0,size.cy-uHeightBmpEnd),
					CSize(m_szSrcBmp.cx,uHeightBmpEnd),
					&m_bmpEnd,
					DST_BITMAP|DSS_NORMAL);
				dc.DrawState(CPoint(0,0),
					CSize(m_szSrcBmp.cx,size.cy/2),
					&m_bmpStart,
					DST_BITMAP|DSS_NORMAL);
			}
			else if(((UINT)size.cy)<=uHeightBmpStart+uHeightBmpEnd)//m_bmpStart和m_bmpEnd长度较小者不剪裁,较长者剪裁输出
			{
				if (uHeightBmpStart>uHeightBmpEnd)
				{
					dc.DrawState(CPoint(0,0),CSize(m_szSrcBmp.cx,uHeightBmpStart),&m_bmpStart,DST_BITMAP|DSS_NORMAL);
					dc.DrawState(CPoint(0,size.cy-uHeightBmpEnd),CSize(m_szSrcBmp.cx,uHeightBmpEnd),&m_bmpEnd,DST_BITMAP|DSS_NORMAL);
				}
				else
				{
					dc.DrawState(CPoint(0,size.cy-uHeightBmpEnd),CSize(m_szSrcBmp.cx,uHeightBmpEnd),&m_bmpEnd,DST_BITMAP|DSS_NORMAL);
					dc.DrawState(CPoint(0,0),CSize(m_szSrcBmp.cx,uHeightBmpStart),&m_bmpStart,DST_BITMAP|DSS_NORMAL);
				}
			}
			else
			{
				//画位图m_bmpStart
				dc.DrawState(CPoint(0,0),
					CSize(m_szSrcBmp.cx,uHeightBmpStart),
					&m_bmpStart,
					DST_BITMAP|DSS_NORMAL);

				//画位图m_bmpMiddle
				int yOrg = uHeightBmpStart;
				while (yOrg < size.cy)
				{
					dc.DrawState(CPoint(0,yOrg),
						CSize(m_szSrcBmp.cx,uHeightBmpMiddle),
						&m_bmpMiddle,
						DST_BITMAP|DSS_NORMAL);
					yOrg += uHeightBmpMiddle;
				}

				//画位图m_bmpEnd
				dc.DrawState(CPoint(0,size.cy-uHeightBmpEnd),
					CSize(m_szSrcBmp.cx,uHeightBmpEnd),
					&m_bmpEnd,
					DST_BITMAP|DSS_NORMAL);
			}	
			//draw the MemDC to pDC(如果需要，则在x方向拉伸位图)
			pDC->StretchBlt(pt.x, pt.y, size.cx, size.cy,
				&dc, 0, 0, m_szSrcBmp.cx, size.cy, SRCCOPY);
		}
	}
}

void DBStretchImage::Reset()
{
	return;

	CBitmap& m_bmpMiddle = *m_bmpDivData.m_pbmpMiddle;
	CBitmap& m_bmpStart = *m_bmpDivData.m_pbmpStart;
	CBitmap& m_bmpEnd = *m_bmpDivData.m_pbmpEnd;

	if (m_hBmpDib)
	{
		DeleteObject(m_hBmpDib);
		m_hBmpDib = NULL;
		m_bmpStart.DeleteObject();
		m_bmpEnd.DeleteObject();
		m_bmpMiddle.DeleteObject();
	}
}

void DBStretchImage::InitStaticData()
{
	if (!m_sbInited)
	{
		m_spmap_ImgDivData = new MAP_IMGDIVDATA;
		m_sbInited = TRUE;
	}
}

void DBStretchImage::ReleaseStaticData()
{
	g_mutexImgDivData.Lock(1000);
	if (m_spmap_ImgDivData)
	{
		MAP_IMGDIVDATA::iterator itb = m_spmap_ImgDivData->begin();
		MAP_IMGDIVDATA::iterator ite = m_spmap_ImgDivData->end();
		for (; itb != ite; itb++)
		{
			itb->second.m_pbmpEnd->DeleteObject();
			itb->second.m_pbmpMiddle->DeleteObject();
			itb->second.m_pbmpMiddle->DeleteObject();
			delete itb->second.m_pbmpEnd;
			delete itb->second.m_pbmpMiddle;
			delete itb->second.m_pbmpStart;
		}

		m_spmap_ImgDivData->clear();
		delete m_spmap_ImgDivData;
		m_spmap_ImgDivData = NULL;
	}

	g_mutexImgDivData.Unlock();

}

BOOL DBStretchImage::GetStaticBmpData(BMPDIVKEY& keyIn,  BMPDIVPART& dataOut )
{
	g_mutexImgDivData.Lock(1000);
	BOOL bRet = FALSE;
	MAP_IMGDIVDATA::iterator itb = m_spmap_ImgDivData->begin();
	MAP_IMGDIVDATA::iterator ite = m_spmap_ImgDivData->end();
	for (; itb != ite; itb++)
	{
		if (itb->first == keyIn)
		{
			bRet = TRUE;
			dataOut = itb->second;
			break;
		}
	}
	
	if(!bRet)
	{
		bRet = SaveToDivBitmap(keyIn, dataOut);
	}
	g_mutexImgDivData.Unlock();
	return bRet;

}

BOOL DBStretchImage::SaveToDivBitmap( BMPDIVKEY& keyIn, BMPDIVPART& dataOut)
{

	CBitmap bitmapSrc;

	if (keyIn.nBtnPart == -2)
	{
		bitmapSrc.LoadBitmap(keyIn.strPath);
	}
	else
	{
		HBITMAP hBmp = (HBITMAP)::LoadImage(NULL, IM_MACRO_GET_OCX_FILE_PATH(keyIn.strPath), 
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		CString fileFullPath = keyIn.strPath;


		//ASSERT(hBmp != NULL);
		if (hBmp == NULL)
		{
			ASSERT(FALSE);
			CString strMsg;
			CString strBuff;	
			//strBuff.LoadString(IDS_STR_ERR_BMP_INVALID);
			strMsg.Format(TEXT("error load bmp = %s"), fileFullPath);
			AfxMessageBox(strMsg, MB_OK);
			exit(0);
		}

		if (keyIn.nBtnPart > -1)
		{
			CBitmap bitmapTotal;
			bitmapTotal.Attach(hBmp);
			BITMAP bitmap;
			ZeroMemory(&bitmap, sizeof(bitmap));
			int nRet = bitmapTotal.GetBitmap(&bitmap);
			if (nRet == 0)
			{
				nRet = GetLastError();
				ASSERT(FALSE);
			}
			CSize szSrcBmp;
			szSrcBmp.cx = bitmap.bmWidth;
			szSrcBmp.cy = bitmap.bmHeight;

			CDC *pDC = (CWnd::FromHandle(GetDesktopWindow()))->GetDC();

			CDC dcSrc;
			dcSrc.CreateCompatibleDC(pDC);
			CDC dcDes;
			dcDes.CreateCompatibleDC(pDC);
			bitmapSrc.CreateCompatibleBitmap(pDC, szSrcBmp.cx/4, szSrcBmp.cy);
			HGDIOBJ  pOldBmpSrc = dcSrc.SelectObject(&bitmapTotal);
			HGDIOBJ  pOldBmpDes = dcDes.SelectObject(&bitmapSrc);
			int nDivWidth = szSrcBmp.cx/4;
			dcDes.BitBlt(0, 0, nDivWidth, szSrcBmp.cy,
				&dcSrc, nDivWidth*keyIn.nBtnPart, 0, SRCCOPY);

			dcSrc.SelectObject(pOldBmpSrc);
			dcSrc.DeleteDC();
			dcDes.SelectObject(pOldBmpDes);
			dcDes.DeleteDC();
			::ReleaseDC(GetDesktopWindow(),pDC->m_hDC);
			bitmapTotal.DeleteObject();
		}
		else
		{
			bitmapSrc.Attach(hBmp);
		}
	}
	

	CBitmap* pBitmap = &bitmapSrc;

	BMPDIVPART bmpData;

	bmpData.m_pbmpStart  = new CBitmap;
	bmpData.m_pbmpMiddle = new CBitmap;
	bmpData.m_pbmpEnd = new CBitmap;

	//get the bitmap size.
	BITMAP bitmap;
	ZeroMemory(&bitmap, sizeof(bitmap));
	int nRet = pBitmap->GetBitmap(&bitmap);
	if (nRet == 0)
	{
		nRet = GetLastError();
		ASSERT(FALSE);
	}
	CSize sztmpSrcBmp;
	sztmpSrcBmp.cx = bitmap.bmWidth;
	sztmpSrcBmp.cy = bitmap.bmHeight;
	bmpData.nWidth = sztmpSrcBmp.cx;
	bmpData.nHeight = sztmpSrcBmp.cy;
	ASSERT (keyIn.nPosSec > keyIn.nPosFir);
	if (keyIn.bHStretch)
		ASSERT (keyIn.nPosSec < sztmpSrcBmp.cx);
	else
		ASSERT (keyIn.nPosSec < sztmpSrcBmp.cy);

	//创建 bitmaps.
	CDC *pDC = (CWnd::FromHandle(GetDesktopWindow()))->GetDC();

	CDC dcSrc;
	dcSrc.CreateCompatibleDC(pDC);
	CDC dcDes;
	dcDes.CreateCompatibleDC(pDC);
	if (keyIn.bHStretch)
	{
		bmpData.m_pbmpStart->CreateCompatibleBitmap(pDC,
			keyIn.nPosFir, sztmpSrcBmp.cy);
		bmpData.m_pbmpMiddle->CreateCompatibleBitmap(pDC, 
			keyIn.nPosSec - keyIn.nPosFir, sztmpSrcBmp.cy);
		bmpData.m_pbmpEnd->CreateCompatibleBitmap(pDC,
			sztmpSrcBmp.cx-keyIn.nPosSec, sztmpSrcBmp.cy);
	}
	else
	{
		bmpData.m_pbmpStart->CreateCompatibleBitmap(pDC,
			sztmpSrcBmp.cx, keyIn.nPosFir);
		bmpData.m_pbmpMiddle->CreateCompatibleBitmap(pDC,
			sztmpSrcBmp.cx, keyIn.nPosSec - keyIn.nPosFir);
		bmpData.m_pbmpEnd->CreateCompatibleBitmap(pDC,
			sztmpSrcBmp.cx, sztmpSrcBmp.cy - keyIn.nPosSec);
	}

	//创建 m_bmpStart.
	HGDIOBJ  pOldBmpSrc = dcSrc.SelectObject(pBitmap);
	HGDIOBJ  pOldBmpDes = dcDes.SelectObject(bmpData.m_pbmpStart);
	if (keyIn.bHStretch)
		dcDes.BitBlt(0, 0, keyIn.nPosFir, sztmpSrcBmp.cy,
		&dcSrc, 0, 0, SRCCOPY);
	else
		dcDes.BitBlt(0, 0, sztmpSrcBmp.cx, keyIn.nPosFir,
		&dcSrc, 0, 0, SRCCOPY);

	//创建 m_bmpMiddle.
	dcDes.SelectObject(bmpData.m_pbmpMiddle);
	if (keyIn.bHStretch)
		dcDes.BitBlt(0, 0, keyIn.nPosSec - keyIn.nPosFir, sztmpSrcBmp.cy,
		&dcSrc, keyIn.nPosFir, 0, SRCCOPY);
	else
		dcDes.BitBlt(0, 0, sztmpSrcBmp.cx, keyIn.nPosSec - keyIn.nPosFir,
		&dcSrc, 0, keyIn.nPosFir, SRCCOPY);

	//创建 m_bmpEnd.
	dcDes.SelectObject(bmpData.m_pbmpEnd);
	if (keyIn.bHStretch)
		dcDes.BitBlt(0, 0, sztmpSrcBmp.cx - keyIn.nPosSec, sztmpSrcBmp.cy,
		&dcSrc, keyIn.nPosSec, 0, SRCCOPY);
	else
		dcDes.BitBlt(0, 0, sztmpSrcBmp.cx, sztmpSrcBmp.cy - keyIn.nPosSec,
		&dcSrc, 0, keyIn.nPosSec, SRCCOPY);

	dcSrc.SelectObject(pOldBmpSrc);
	dcSrc.DeleteDC();
	dcDes.SelectObject(pOldBmpDes);
	dcDes.DeleteDC();
	::ReleaseDC(GetDesktopWindow(),pDC->m_hDC);
	bitmapSrc.DeleteObject();

	(*m_spmap_ImgDivData).push_back(IMGDIVPAIR(keyIn, bmpData));
	dataOut = bmpData;
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////// 
// 
// Functions for smooth bitmap resize 
// 
// Improvement: float calculations changed to int. 
// 
// Ivaylo Byalkov, January 24, 2000 
// e-mail: ivob@i-n.net 
// 
/////////////////////////////////////////////////////////////////////////////// 
// 
// Some bugs corrected due to large bitmap scaling and dword scanline align 
// 
// Leonardas Survila, December 30, 2003 
// e-mail: ulterior@one.lt 
// 
/////////////////////////////////////////////////////////////////////////////// 

#include <windows.h> 
#include <windowsx.h> 
#include <stdio.h> 
#include <math.h> 

#define DWORD_24BIT_WIDTHBYTES_ALIGN( width ) ((width*3) + ( width % 4 == 0 ? 0 : width % 2 == 0 ? 2 : (width+1) % 4 == 0 ? 3 : 1 )); 
#define F_DELTA 0.0001f 

/////////////////////////////////////////////////////////// 



/////////////////////////////////////////////////////////// 
// Main resize function 

HBITMAP DBStretchImage::STRENTCH::ScaleBitmap( HDC hDC, HBITMAP hBmp, WORD wNewWidth, WORD wNewHeight, BOOL fastScale ) 
{ 
	BITMAP bmp; 
	::GetObject(hBmp, sizeof(BITMAP), &bmp); 
	
	// check for valid size 
	if((bmp.bmWidth > wNewWidth 
		&& bmp.bmHeight < wNewHeight) 
		|| (bmp.bmWidth < wNewWidth 
		&& bmp.bmHeight > wNewHeight)) 
		return NULL; 
	
	BYTE *pData = (BYTE*)bmp.bmBits; 
	
	HBITMAP hResBmp = ::CreateCompatibleBitmap(hDC, wNewWidth, wNewHeight); 
	if ( hResBmp == NULL ) return NULL; 
	
	BITMAP new_bmp; 
	::GetObject(hResBmp, sizeof(BITMAP), &new_bmp); 
	
	int newmemorysize = DWORD_24BIT_WIDTHBYTES_ALIGN ( wNewWidth ); 
	newmemorysize *= wNewHeight; 
	BYTE *pData2 = new BYTE[ newmemorysize ]; 
	
	if ( fastScale ) 
	{ 
		if (bmp.bmWidth >= wNewWidth && bmp.bmHeight >= wNewHeight) 
			ShrinkDataInt( pData, (WORD)bmp.bmWidth, (WORD)bmp.bmHeight, 
			pData2, wNewWidth, wNewHeight); 
		else 
			EnlargeDataInt( pData, (WORD)bmp.bmWidth, (WORD)bmp.bmHeight, 
			pData2, wNewWidth, wNewHeight); 
	} 
	else 
	{ 
		if (bmp.bmWidth >= wNewWidth && bmp.bmHeight >= wNewHeight) 
			ShrinkData( pData, (WORD)bmp.bmWidth, (WORD)bmp.bmHeight, 
			pData2, wNewWidth, wNewHeight); 
		else 
			EnlargeData( pData, (WORD)bmp.bmWidth, (WORD)bmp.bmHeight, 
			pData2, wNewWidth, wNewHeight); 
	} 
	
	BYTE *ptraddr = pData2; 
	int caretptr = DWORD_24BIT_WIDTHBYTES_ALIGN( wNewWidth ); 
	BITMAPINFO *pbi = PrepareRGBBitmapInfo(wNewWidth, wNewHeight); 
	for ( int i=0; i<wNewHeight; i++ ) 
	{ 
		if ( i>0 ) ptraddr += caretptr; 
		::SetDIBits(hDC, hResBmp, i, 1, ptraddr, pbi, DIB_RGB_COLORS); 
	} 
	
	delete pbi; 
	delete[] pData2; 
	
	return hResBmp; 
} 

/////////////////////////////////////////////////////////// 

BITMAPINFO * DBStretchImage::STRENTCH::PrepareRGBBitmapInfo(WORD wWidth, WORD wHeight) 
{ 
	BITMAPINFO *pRes = new BITMAPINFO; 
	::ZeroMemory(pRes, sizeof(BITMAPINFO)); 
	pRes->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
	pRes->bmiHeader.biWidth = wWidth; 
	pRes->bmiHeader.biHeight = wHeight; 
	pRes->bmiHeader.biPlanes = 1; 
	pRes->bmiHeader.biBitCount = 24; 
	pRes->bmiHeader.biCompression = BI_RGB ; 
	return pRes; 
} 

/////////////////////////////////////////////////////////// 

float * DBStretchImage::STRENTCH::CreateCoeff(int nLen, int nNewLen, BOOL bShrink) 
{ 
	int nSum = 0, nSum2; 
	float *pRes = new float[2 * nLen]; 
	float *pCoeff = pRes; 
	float fNorm = (bShrink)? (float)nNewLen / nLen : 1; 
	int nDenom = (bShrink)? nLen : nNewLen; 
	
	::ZeroMemory(pRes, 2 * nLen * sizeof(float)); 
	for(int i = 0; i < nLen; i++, pCoeff += 2) 
	{ 
		nSum2 = nSum + nNewLen; 
		if(nSum2 > nLen) 
		{ 
			*pCoeff = (float)(nLen - nSum) / nDenom; 
			pCoeff[1] = (float)(nSum2 - nLen) / nDenom; 
			nSum2 -= nLen; 
		} 
		else 
		{ 
			*pCoeff = fNorm; 
			if(nSum2 == nLen) 
			{ 
				pCoeff[1] = -1; 
				nSum2 = 0; 
			} 
		} 
		nSum = nSum2; 
	} 
	
	return pRes; 
} 

/////////////////////////////////////////////////////////// 

void DBStretchImage::STRENTCH::ShrinkData(BYTE *pInBuff, 
				WORD wWidth, 
				WORD wHeight, 
				BYTE *pOutBuff, 
				WORD wNewWidth, 
				WORD wNewHeight) 
{ 
	BYTE *pLine = pInBuff, *pPix; 
	BYTE *pOutLine = pOutBuff; 
	DWORD dwInLn = DWORD_24BIT_WIDTHBYTES_ALIGN(wWidth); 
	DWORD dwOutLn = DWORD_24BIT_WIDTHBYTES_ALIGN(wNewWidth); 
	int x, y, i, ii; 
	BOOL bCrossRow, bCrossCol; 
	float *pRowCoeff = CreateCoeff(wWidth, wNewWidth, TRUE); 
	float *pColCoeff = CreateCoeff(wHeight, wNewHeight, TRUE); 
	float fTmp, *pXCoeff, *pYCoeff = pColCoeff; 
	DWORD dwBuffLn = 3 * wNewWidth * sizeof(float); 
	float *fBuff = new float[6 * wNewWidth]; 
	float *fCurrLn = fBuff, 
		*fCurrPix, 
		*fNextLn = fBuff + 3 * wNewWidth, 
		*fNextPix; 
	
	::ZeroMemory(fBuff, 2 * dwBuffLn); 
	
	y = 0; 
	while(y < wNewHeight) 
	{ 
		pPix = pLine; 
		pLine += dwInLn; 
		
		fCurrPix = fCurrLn; 
		fNextPix = fNextLn; 
		
		x = 0; 
		pXCoeff = pRowCoeff; 
		bCrossRow = pYCoeff[1] > F_DELTA; 
		while(x < wNewWidth) 
		{ 
			fTmp = *pXCoeff * *pYCoeff; 
			for(i = 0; i < 3; i++) 
				fCurrPix[i] += fTmp * pPix[i]; 
			bCrossCol = pXCoeff[1] > F_DELTA; 
			if(bCrossCol) 
			{ 
				fTmp = pXCoeff[1] * *pYCoeff; 
				for(i = 0, ii = 3; i < 3; i++, ii++) 
					fCurrPix[ii] += fTmp * pPix[i]; 
			} 
			
			if(bCrossRow) 
			{ 
				fTmp = *pXCoeff * pYCoeff[1]; 
				for(i = 0; i < 3; i++) 
					fNextPix[i] += fTmp * pPix[i]; 
				if(bCrossCol) 
				{ 
					fTmp = pXCoeff[1] * pYCoeff[1]; 
					for(i = 0, ii = 3; i < 3; i++, ii++) 
						fNextPix[ii] += fTmp * pPix[i]; 
				} 
			} 
			
			if(fabs(pXCoeff[1]) > F_DELTA) 
			{ 
				x++; 
				fCurrPix += 3; 
				fNextPix += 3; 
			} 
			
			pXCoeff += 2; 
			pPix += 3; 
		} 
		
		if(fabs(pYCoeff[1]) > F_DELTA) 
		{ 
			// set result line 
			fCurrPix = fCurrLn; 
			pPix = pOutLine; 
			for(i = 3 * wNewWidth; i > 0; i--, fCurrPix++, pPix++) 
				*pPix = (BYTE)*fCurrPix; 
			
			// prepare line buffers 
			fCurrPix = fNextLn; 
			fNextLn = fCurrLn; 
			fCurrLn = fCurrPix; 
			::ZeroMemory(fNextLn, dwBuffLn); 
			
			y++; 
			pOutLine += dwOutLn; 
		} 
		pYCoeff += 2; 
	} 
	
	delete [] pRowCoeff; 
	delete [] pColCoeff; 
	delete [] fBuff; 
} 

/////////////////////////////////////////////////////////// 

void DBStretchImage::STRENTCH::EnlargeData(BYTE *pInBuff, 
	WORD wWidth, 
	WORD wHeight, 
	BYTE *pOutBuff, 
	WORD wNewWidth, 
	WORD wNewHeight) 
{ 
	BYTE *pLine = pInBuff, 
		*pPix = pLine, 
		*pPixOld, 
		*pUpPix, 
		*pUpPixOld; 
	BYTE *pOutLine = pOutBuff, *pOutPix; 
	
	DWORD dwInLn = DWORD_24BIT_WIDTHBYTES_ALIGN(wWidth); 
	DWORD dwOutLn = DWORD_24BIT_WIDTHBYTES_ALIGN(wNewWidth); 
	
	int x, y, i; 
	BOOL bCrossRow, bCrossCol; 
	float *pRowCoeff = CreateCoeff(wNewWidth, wWidth, FALSE); 
	float *pColCoeff = CreateCoeff(wNewHeight, wHeight, FALSE); 
	float fTmp, fPtTmp[3], *pXCoeff, *pYCoeff = pColCoeff; 
	
	y = 0; 
	while(y < wHeight) 
	{ 
		bCrossRow = pYCoeff[1] > F_DELTA; 
		x = 0; 
		pXCoeff = pRowCoeff; 
		pOutPix = pOutLine; 
		pOutLine += dwOutLn; 
		pUpPix = pLine; 
		if(fabs(pYCoeff[1]) > F_DELTA) 
		{ 
			y++; 
			pLine += dwInLn; 
			pPix = pLine; 
		} 
		
		while(x < wWidth) 
		{ 
			bCrossCol = pXCoeff[1] > F_DELTA; 
			pUpPixOld = pUpPix; 
			pPixOld = pPix; 
			if(fabs(pXCoeff[1]) > F_DELTA) 
			{ 
				x++; 
				pUpPix += 3; 
				pPix += 3; 
			} 
			fTmp = *pXCoeff * *pYCoeff; 
			for(i = 0; i < 3; i++) 
				fPtTmp[i] = fTmp * pUpPixOld[i]; 
			if(bCrossCol) 
			{ 
				fTmp = pXCoeff[1] * *pYCoeff; 
				for(i = 0; i < 3; i++) 
					fPtTmp[i] += fTmp * pUpPix[i]; 
			} 
			if(bCrossRow) 
			{ 
				fTmp = *pXCoeff * pYCoeff[1]; 
				for(i = 0; i < 3; i++) 
					fPtTmp[i] += fTmp * pPixOld[i]; 
				if(bCrossCol) 
				{ 
					fTmp = pXCoeff[1] * pYCoeff[1]; 
					for(i = 0; i < 3; i++) 
						fPtTmp[i] += fTmp * pPix[i]; 
				} 
			} 
			for(i = 0; i < 3; i++, pOutPix++) 
				*pOutPix = (BYTE)fPtTmp[i]; 
			pXCoeff += 2; 
		} 
		pYCoeff += 2; 
	} 
	
	delete [] pRowCoeff; 
	delete [] pColCoeff; 
} 


/////////////////////////////////////////////////////////// 
int * DBStretchImage::STRENTCH::CreateCoeffInt(int nLen, int nNewLen, BOOL bShrink) 
{ 
	int nSum = 0, nSum2; 
	int *pRes = new int[2 * nLen]; 
	int *pCoeff = pRes; 
	int nNorm = (bShrink) 
		? (nNewLen << 12) / nLen : 0x1000; 
	int nDenom = (bShrink)? nLen : nNewLen; 
	
	::ZeroMemory(pRes, 2 * nLen * sizeof(int)); 
	for(int i = 0; i < nLen; i++, pCoeff += 2) 
	{ 
		nSum2 = nSum + nNewLen; 
		if(nSum2 > nLen) 
		{ 
			*pCoeff = ((nLen - nSum) << 12) / nDenom; 
			pCoeff[1] = ((nSum2 - nLen) << 12) / nDenom; 
			nSum2 -= nLen; 
		} 
		else 
		{ 
			*pCoeff = nNorm; 
			if(nSum2 == nLen) 
			{ 
				pCoeff[1] = -1; 
				nSum2 = 0; 
			} 
		} 
		nSum = nSum2; 
	} 
	
	return pRes; 
} 

void DBStretchImage::STRENTCH::ShrinkDataInt( 
	BYTE *pInBuff, 
	WORD wWidth, 
	WORD wHeight, 
	BYTE *pOutBuff, 
	WORD wNewWidth, 
	WORD wNewHeight) 
{ 
	BYTE *pLine = pInBuff, *pPix; 
	BYTE *pOutLine = pOutBuff; 
	DWORD dwInLn = DWORD_24BIT_WIDTHBYTES_ALIGN(wWidth); 
	DWORD dwOutLn = DWORD_24BIT_WIDTHBYTES_ALIGN(wNewWidth); 
	int x, y, i, ii; 
	BOOL bCrossRow, bCrossCol; 
	int *pRowCoeff = CreateCoeffInt(wWidth, 
		wNewWidth, 
		TRUE); 
	int *pColCoeff = CreateCoeffInt(wHeight, 
		wNewHeight, 
		TRUE); 
	int *pXCoeff, *pYCoeff = pColCoeff; 
	DWORD dwBuffLn = 3 * wNewWidth * sizeof(DWORD); 
	DWORD *pdwBuff = new DWORD[6 * wNewWidth]; 
	DWORD *pdwCurrLn = pdwBuff, 
		*pdwCurrPix, 
		*pdwNextLn = pdwBuff + 3 * wNewWidth; 
	DWORD dwTmp, *pdwNextPix; 
	
	::ZeroMemory(pdwBuff, 2 * dwBuffLn); 
	
	y = 0; 
	while(y < wNewHeight) 
	{ 
		pPix = pLine; 
		pLine += dwInLn; 
		
		pdwCurrPix = pdwCurrLn; 
		pdwNextPix = pdwNextLn; 
		
		x = 0; 
		pXCoeff = pRowCoeff; 
		bCrossRow = pYCoeff[1] > 0; 
		while(x < wNewWidth) 
		{ 
			dwTmp = *pXCoeff * *pYCoeff; 
			for(i = 0; i < 3; i++) 
				pdwCurrPix[i] += dwTmp * pPix[i]; 
			bCrossCol = pXCoeff[1] > 0; 
			if(bCrossCol) 
			{ 
				dwTmp = pXCoeff[1] * *pYCoeff; 
				for(i = 0, ii = 3; i < 3; i++, ii++) 
					pdwCurrPix[ii] += dwTmp * pPix[i]; 
			} 
			if(bCrossRow) 
			{ 
				dwTmp = *pXCoeff * pYCoeff[1]; 
				for(i = 0; i < 3; i++) 
					pdwNextPix[i] += dwTmp * pPix[i]; 
				if(bCrossCol) 
				{ 
					dwTmp = pXCoeff[1] * pYCoeff[1]; 
					for(i = 0, ii = 3; i < 3; i++, ii++) 
						pdwNextPix[ii] += dwTmp * pPix[i]; 
				} 
			} 
			if(pXCoeff[1]) 
			{ 
				x++; 
				pdwCurrPix += 3; 
				pdwNextPix += 3; 
			} 
			pXCoeff += 2; 
			pPix += 3; 
		} 
		if(pYCoeff[1]) 
		{ 
			// set result line 
			pdwCurrPix = pdwCurrLn; 
			pPix = pOutLine; 
			for(i = 3 * wNewWidth; i > 0; i--, pdwCurrPix++, pPix++) 
				*pPix = ((LPBYTE)pdwCurrPix)[3]; 
			
			// prepare line buffers 
			pdwCurrPix = pdwNextLn; 
			pdwNextLn = pdwCurrLn; 
			pdwCurrLn = pdwCurrPix; 
			::ZeroMemory(pdwNextLn, dwBuffLn); 
			
			y++; 
			pOutLine += dwOutLn; 
		} 
		pYCoeff += 2; 
	} 
	
	delete [] pRowCoeff; 
	delete [] pColCoeff; 
	delete [] pdwBuff; 
} 

/////////////////////////////////////////////////////////// 
	
void DBStretchImage::STRENTCH::EnlargeDataInt( 
	BYTE *pInBuff, 
	WORD wWidth, 
	WORD wHeight, 
	BYTE *pOutBuff, 
	WORD wNewWidth, 
	WORD wNewHeight) 
{ 
	BYTE *pLine = pInBuff, 
		*pPix = pLine, 
		*pPixOld, 
		*pUpPix, 
		*pUpPixOld; 
	BYTE *pOutLine = pOutBuff, *pOutPix; 
	DWORD dwInLn = DWORD_24BIT_WIDTHBYTES_ALIGN(wWidth); 
	DWORD dwOutLn = DWORD_24BIT_WIDTHBYTES_ALIGN(wNewWidth); 
	int x, y, i; 
	BOOL bCrossRow, bCrossCol; 
	int *pRowCoeff = CreateCoeffInt(wNewWidth, 
		wWidth, 
		FALSE); 
	int *pColCoeff = CreateCoeffInt(wNewHeight, 
		wHeight, 
		FALSE); 
	
	int *pXCoeff, *pYCoeff = pColCoeff; 
	DWORD dwTmp, dwPtTmp[3]; 
	
	y = 0; 
	while(y < wHeight ) 
	{ 
		bCrossRow = pYCoeff[1] > 0; 
		x = 0; 
		pXCoeff = pRowCoeff; 
		pOutPix = pOutLine; 
		pOutLine += dwOutLn; 
		pUpPix = pLine; 
		if(pYCoeff[1]) 
		{ 
			y++; 
			pLine += dwInLn; 
			pPix = pLine; 
		} 
		
		while(x < wWidth) 
		{ 
			bCrossCol = pXCoeff[1] > 0; 
			pUpPixOld = pUpPix; 
			pPixOld = pPix; 
			if(pXCoeff[1]) 
			{ 
				x++; 
				pUpPix += 3; 
				pPix += 3; 
			} 
			
			dwTmp = *pXCoeff * *pYCoeff; 
			
			for(i = 0; i < 3; i++) 
				dwPtTmp[i] = dwTmp * pUpPixOld[i]; 
			
			if(bCrossCol) 
			{ 
				dwTmp = pXCoeff[1] * *pYCoeff; 
				for(i = 0; i < 3; i++) 
					dwPtTmp[i] += dwTmp * pUpPix[i]; 
			} 
			
			if(bCrossRow) 
			{ 
				dwTmp = *pXCoeff * pYCoeff[1]; 
				for(i = 0; i < 3; i++) 
					dwPtTmp[i] += dwTmp * pPixOld[i]; 
				if(bCrossCol) 
				{ 
					dwTmp = pXCoeff[1] * pYCoeff[1]; 
					for(i = 0; i < 3; i++) 
						dwPtTmp[i] += dwTmp * pPix[i]; 
				} 
			} 
			
			for(i = 0; i < 3; i++, pOutPix++) 
			{ 
				*pOutPix = ((LPBYTE)(dwPtTmp + i))[3]; 
			} 
			
			pXCoeff += 2; 
		} 
		pYCoeff += 2; 
	} 
	
	delete [] pRowCoeff; 
	delete [] pColCoeff; 
} 
	
	
