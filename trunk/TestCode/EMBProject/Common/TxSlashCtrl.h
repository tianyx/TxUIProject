/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxSlashCtrl.h
	file base:	TxSlashCtrl
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once
#include <vector>
#include "GdiPlusNewHeader.h"
using namespace Gdiplus;

typedef std::vector<Rect> VECSLASHBMPS;

DWORD __stdcall SlashDrawThreadProc(LPVOID pParam);

class CTxSlashCtrl
{
public:
	CTxSlashCtrl(void);
	~CTxSlashCtrl(void);

	void InitSlash(int nTotalFrame, int nFrameWidth, int nFrameHeight);
	int AddSlash(Bitmap* pImgIn, CRect* prcFirstFrame, CRect* prcEndFrame, int nNumToSplit);
	BOOL StartLoop(CWnd* pParent, CRect& rcToDraw, int nLoopStart, int nLoopCount, int nSlashInterval);
	BOOL EndLoop();
	BOOL Release();

	void SetRect(CRect rcDraw);
	BOOL SetLoopPos(int nStart, int nCount);

	BOOL m_bInited;
	int m_nTotalFrame;
	int m_nFrameWidth;
	int m_nFrameHeight;
	int m_nSlashCount;
	CSize m_szCacheSize;
	VECSLASHBMPS m_vSlashBmp;
	Bitmap* m_pBmpCache;

	int m_nLoopBegin;
	int m_nLoopCount;

	friend DWORD __stdcall SlashDrawThreadProc(LPVOID pParam);
protected:
	CWnd* m_pParent;
	CRect m_rcRect;
	int m_nSlashInterval;
	HANDLE m_hThread;
	BOOL m_bInLoop;
	int m_nCurrentFrame;
	void Draw();
	void CallDraw();

	CRITICAL_SECTION m_lock;
	BOOL Lock();
	void UnLock();
};

