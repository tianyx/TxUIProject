// MemDC.cpp : implementation file
//

#include "stdafx.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemDC


CMemDC::CMemDC(CDC* pDc, long width,long heigh)
{   
	m_pOldBitmap = NULL;
	if(heigh >0 && width > 0)
	{
		OpenMemDC(pDc, width, heigh);
	}
}

CMemDC::~CMemDC()
{
	if (!GetSafeHdc())
	{
		return;
	}
	if(m_pOldBitmap)
	{
		SelectObject(m_pOldBitmap);
		m_pOldBitmap = NULL;
	}
	m_bitmap.DeleteObject();
	DeleteDC();
}
/////////////////////////////////////////////////////////////////////////////
// CMemDC message handlers

BOOL CMemDC::OpenMemDC( CDC* pDC, long width,long heigh )
{
	BOOL bRet = FALSE;
	try
	{
		const UINT nMax = 10240;
		ASSERT((width > 0) && (heigh > 0) && (heigh < nMax) && (width < nMax));
		if(heigh <=0 || width <= 0 || width >= nMax || heigh >= nMax)
		{
			ASSERT(FALSE);
			throw(0);
		
		}

		//CDC *pDC = CDC::FromHandle(::GetDC(GetDesktopWindow()));
		if (pDC == NULL)
		{
			_RPT1(0,"createdc Err=%d", GetLastError());
			ASSERT(FALSE);
		}
		ASSERT(pDC != NULL);
		//CreateCompatibleDC(pDC);
		HDC hdc = ::CreateCompatibleDC(pDC->GetSafeHdc());
		
		if (hdc == NULL)
		{
			_RPT1(0,"\n dc == 0,last err =%d",GetLastError());
			ASSERT(FALSE);
			//ASSERT(FALSE);
			return FALSE;
		}
		else
		{
			 Attach(hdc); 
		}
		m_bmpWidth = width;
		m_bmpHeigh = heigh;
		//m_bitmap.CreateCompatibleBitmap(pDC, m_bmpWidth, m_bmpHeigh);

		if (m_bitmap.CreateBitmap(m_bmpWidth, m_bmpHeigh, 1,32,0))
		{
			m_pOldBitmap = SelectObject(&m_bitmap);
			bRet = TRUE;
		}
		else
		{
			ASSERT(FALSE);
			bRet = FALSE;
		}
		//::ReleaseDC(GetDesktopWindow(), pDC->m_hDC);
	}
	catch (...)
	{
		ASSERT(FALSE);
	}
	return bRet;
}

HBITMAP CMemDC::CloseMemDC()
{
	HBITMAP hRet = NULL;
	try
	{
		if(m_pOldBitmap)
		{
			SelectObject(m_pOldBitmap);
			m_pOldBitmap = NULL;
		}

		hRet = (HBITMAP)(m_bitmap.Detach());
	}
	catch(...)
	{
	}
	return hRet;
}
