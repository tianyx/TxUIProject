/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxSlashCtrl.cpp
	file base:	TxSlashCtrl
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "TxSlashCtrl.h"
#include "GDIDrawFunc.h"

CTxSlashCtrl::CTxSlashCtrl(void)
{
	m_nSlashCount = 0;
	m_szCacheSize = CSize(0,0);
	m_nLoopBegin = 0;
	m_nLoopCount = 0;
	m_pParent = NULL;
	m_rcRect.SetRectEmpty();
	m_nSlashInterval = 1000;
	m_hThread  = NULL;
	m_bInited = FALSE;
	m_pBmpCache = NULL;
	m_nTotalFrame = 0;
	m_nFrameWidth = 0;
	m_nFrameHeight = 0;
	m_bInLoop = FALSE;
	m_nCurrentFrame = 0;
	InitializeCriticalSection(&m_lock);

}

CTxSlashCtrl::~CTxSlashCtrl(void)
{
	if (m_hThread)
	{
		EndLoop();
	}
	if (m_pBmpCache)
	{
		delete m_pBmpCache;
		m_pBmpCache = NULL;
	}
	DeleteCriticalSection(&m_lock);
	
}

void CTxSlashCtrl::InitSlash( int nTotalFrame, int nFrameWidth, int nFrameHeight )
{
	if (m_pBmpCache)
	{
		delete m_pBmpCache;
		m_pBmpCache = NULL;
	}
	m_nTotalFrame = nTotalFrame;
	m_nFrameWidth = nFrameWidth;
	m_nFrameHeight = nFrameHeight;
	m_pBmpCache = new Bitmap(nFrameWidth* nTotalFrame, nFrameHeight, PixelFormat32bppARGB);
	ASSERT(m_pBmpCache && m_pBmpCache->GetLastStatus() == Ok);
	m_bInited = TRUE;
}

int CTxSlashCtrl::AddSlash( Bitmap* pImgIn, CRect* prcFirstFrame, CRect* prcEndFrame, int nNumToSplit )
{
	if (!m_bInited || nNumToSplit < 2)
	{
		ASSERT(FALSE);
		return -1;
	}
	int nOffset = m_vSlashBmp.size();
	if (nOffset + nNumToSplit > m_nTotalFrame)
	{
		ASSERT(FALSE);
		return -1;
	}

	int nStartPos = m_nFrameWidth * nOffset;
	CRect rcDes(nStartPos, 0, nStartPos + m_nFrameWidth, m_nFrameHeight);
	for (int i = 0; i < nNumToSplit; i++)
	{
		double percent = ((double)i)/nNumToSplit;
		GPCreateFlashFrame(*pImgIn, *prcFirstFrame, *prcEndFrame, *m_pBmpCache, rcDes, percent);
		m_vSlashBmp.push_back(CRect2Rect(rcDes));
		rcDes.OffsetRect(m_nFrameWidth, 0);
	}
	
	return nStartPos;
}

BOOL CTxSlashCtrl::StartLoop( CWnd* pParent, CRect& rcToDraw, int nLoopStart, int nLoopCount, int nSlashInterval )
{
	if (!m_bInited || rcToDraw.IsRectEmpty() || nLoopStart >= m_vSlashBmp.size() || nLoopStart + nLoopCount > m_vSlashBmp.size())
	{
		ASSERT(FALSE);
		return FALSE;
	}

	if (m_bInLoop)
	{
		EndLoop();
	}
	m_pParent = pParent;
	m_rcRect = rcToDraw;
	m_nLoopBegin = nLoopStart;
	m_nLoopCount = nLoopCount;
	m_nSlashInterval = nSlashInterval;
	m_nCurrentFrame = nLoopStart;
	m_hThread = CreateThread(NULL, 0, SlashDrawThreadProc, (void*)this, 0, NULL);

	return TRUE;
}

BOOL CTxSlashCtrl::EndLoop()
{
	m_pParent = NULL;
	m_rcRect.SetRectEmpty();
	if (m_hThread)
	{
		Sleep(100);
		ASSERT(!m_bInLoop);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}

	return TRUE;
}

void CTxSlashCtrl::Draw()
{
	Lock();
	if (!m_bInited  ||!m_pBmpCache ||!m_pParent ||!m_pParent->IsWindowVisible() || m_rcRect.IsRectEmpty()
		||m_nCurrentFrame >= m_vSlashBmp.size())
	{
		
	}
	else
	{
		CClientDC dc(m_pParent);
		Graphics gc(dc.GetSafeHdc());
		Rect rSrc = m_vSlashBmp[m_nCurrentFrame];
		Rect rDes = CRect2Rect(m_rcRect);
		gc.DrawImage(m_pBmpCache, rDes,rSrc.X, rSrc.Y, rSrc.Width, rSrc.Height, UnitPixel);

	}

	UnLock();
}

void CTxSlashCtrl::SetRect( CRect rcDraw )
{
	if (rcDraw != m_rcRect)
	{
		if (m_pParent && !m_rcRect.IsRectEmpty())
		{
			m_pParent->InvalidateRect(m_rcRect);
		}
		Lock();
		m_rcRect = rcDraw;
		UnLock();
	}
}

BOOL CTxSlashCtrl::SetLoopPos( int nStart, int nCount )
{
	if (nStart != m_nLoopBegin || nCount != m_nLoopCount)
	{
		Lock();
		m_nLoopBegin = nStart;
		m_nLoopCount = nCount;
		m_nCurrentFrame = nStart;
		UnLock();
	}

	return TRUE;
}

BOOL CTxSlashCtrl::Lock()
{
	int nCount = 0;
	while(TryEnterCriticalSection(&m_lock) == 0)
	{
		if (nCount> 100)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		nCount++;
		Sleep(10);
	}

	return TRUE;
}

void CTxSlashCtrl::UnLock()
{
	LeaveCriticalSection(&m_lock);

}

void CTxSlashCtrl::CallDraw()
{
	static int nDeta =1;
	if (m_nCurrentFrame >= m_nLoopBegin + m_nLoopCount-1)
	{
		nDeta = -1;
	}
	if (m_nCurrentFrame <= m_nLoopBegin)
	{
		nDeta = 1;
	}
	m_nCurrentFrame += nDeta;
	Draw();
}

DWORD __stdcall SlashDrawThreadProc( LPVOID pParam )
{
	CTxSlashCtrl* pCtrl = (CTxSlashCtrl*)pParam;
	while (pCtrl->m_pParent)
	{
		pCtrl->CallDraw();
		Sleep(pCtrl->m_nSlashInterval);
	}

	return 0;
}
