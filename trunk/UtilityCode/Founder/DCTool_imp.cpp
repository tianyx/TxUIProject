// DCTool_imp.cpp: implementation of the CDCTool class.
//
//////////////////////////////////////////////////////////////////////

#include "../stdafx.h"
#include "DCTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDCTool::CDCTool()
{

}

CDCTool::~CDCTool()
{

}

HRESULT CDCTool::DrawBkPicture(CDC &dc, CBitmap &bkBMP, CRect rectTarget, CRect rectSrc, tagBKBmpData bmpData)
{
	// 数据初始化
	if ( rectSrc.Width() == 0 || rectSrc.Height() == 0)
	{
		BITMAP bmp;
		bkBMP.GetBitmap(&bmp);

		rectSrc.left = 0;
		rectSrc.top = 0;
		rectSrc.right = bmp.bmWidth;
		rectSrc.bottom = bmp.bmHeight;
	}

	if ( bmpData == tagBKBmpData( 0, 0, 0, 0, 0, 0) )
	{
		bmpData.nLeftWidth = rectSrc.Width() / 3;
		bmpData.nMiddleWidth = bmpData.nLeftWidth;
		bmpData.nRightWidth = bmpData.nLeftWidth;

		bmpData.nTopHeight = rectSrc.Height() / 3;
		bmpData.nMiddleHeight = bmpData.nTopHeight;
		bmpData.nBottomHeight = bmpData.nTopHeight;

	}
	// 数据合法性检查
	if ( rectSrc.Width() <= 0 || rectSrc.Height() <= 0)
	{
		return E_INVALIDARG;
	}
	if ( bmpData.nLeftWidth <= 0 || bmpData.nMiddleWidth <= 0 || bmpData.nRightWidth <= 0 )
	{
		return E_INVALIDARG;
	}
	if ( bmpData.nTopHeight <= 0 || bmpData.nMiddleHeight <= 0 || bmpData.nBottomHeight <= 0)
	{
		return E_INVALIDARG;
	}
	// Draw background	
	CDC memDC;
	CBitmap *pOldBmp = NULL;
	
	
	memDC.CreateCompatibleDC( &dc );
	pOldBmp = memDC.SelectObject( &bkBMP );
	
	
	CRect rectTargetTemp;
	CRect rectSourceTemp;

	// 上部,左
	rectTargetTemp.left		= rectTarget.left;
	rectTargetTemp.top		= rectTarget.top;
	rectTargetTemp.right	= rectTarget.left + bmpData.nLeftWidth ;
	rectTargetTemp.bottom	= rectTarget.top + bmpData.nTopHeight;	

	rectSourceTemp.left		= rectSrc.left;
	rectSourceTemp.top		= rectSrc.top;
	rectSourceTemp.right	= rectSrc.left + bmpData.nLeftWidth;
	rectSourceTemp.bottom	= rectSrc.top + bmpData.nTopHeight;
	dc.BitBlt( rectTargetTemp.left, rectTargetTemp.top, rectTargetTemp.Width(), rectTargetTemp.Height(), &memDC, rectSourceTemp.left, rectSourceTemp.top, SRCCOPY);


	// 上部,中
	rectTargetTemp.left		= rectTarget.left + bmpData.nLeftWidth;
	rectTargetTemp.top		= rectTarget.top;
	rectTargetTemp.right	= rectTarget.right - bmpData.nRightWidth;
	rectTargetTemp.bottom	= rectTarget.top + bmpData.nTopHeight;	
	
	rectSourceTemp.left		= rectSrc.left + bmpData.nLeftWidth;
	rectSourceTemp.top		= rectSrc.top;
	rectSourceTemp.right	= rectSrc.left + bmpData.nLeftWidth + bmpData.nMiddleWidth;
	rectSourceTemp.bottom	= rectSrc.top + bmpData.nTopHeight;
	dc.StretchBlt( rectTargetTemp.left, rectTargetTemp.top, rectTargetTemp.Width(), rectTargetTemp.Height(), &memDC, rectSourceTemp.left, rectSourceTemp.top, rectSourceTemp.Width(), rectSourceTemp.Height(),SRCCOPY);
	
	// 上部,右
	rectTargetTemp.left		= rectTarget.right - bmpData.nRightWidth;
	rectTargetTemp.top		= rectTarget.top;
	rectTargetTemp.right	= rectTarget.right ;
	rectTargetTemp.bottom	= rectTarget.top + bmpData.nTopHeight;	
	
	rectSourceTemp.left		= rectSrc.right - bmpData.nRightWidth;
	rectSourceTemp.top		= rectSrc.top;
	rectSourceTemp.right	= rectSrc.right;
	rectSourceTemp.bottom	= rectSrc.top + bmpData.nTopHeight;
	dc.BitBlt( rectTargetTemp.left, rectTargetTemp.top, rectTargetTemp.Width(), rectTargetTemp.Height(), &memDC, rectSourceTemp.left, rectSourceTemp.top, SRCCOPY);
	
	// 中部, 左
	rectTargetTemp.left		= rectTarget.left;
	rectTargetTemp.top		= rectTarget.top + bmpData.nTopHeight;
	rectTargetTemp.right	= rectTarget.left + bmpData.nLeftWidth ;
	rectTargetTemp.bottom	= rectTarget.bottom - bmpData.nBottomHeight;	
	
	rectSourceTemp.left		= rectSrc.left;
	rectSourceTemp.top		= rectSrc.top +  bmpData.nTopHeight ;
	rectSourceTemp.right	= rectSrc.left + bmpData.nLeftWidth;
	rectSourceTemp.bottom	= rectSrc.bottom - bmpData.nBottomHeight;
	dc.StretchBlt( rectTargetTemp.left, rectTargetTemp.top, rectTargetTemp.Width(), rectTargetTemp.Height(), &memDC, rectSourceTemp.left, rectSourceTemp.top, rectSourceTemp.Width(), rectSourceTemp.Height(),SRCCOPY);

	// 中部, 中
	rectTargetTemp.left		= rectTarget.left + bmpData.nLeftWidth;
	rectTargetTemp.top		= rectTarget.top + bmpData.nTopHeight;
	rectTargetTemp.right	= rectTarget.right - bmpData.nRightWidth;
	rectTargetTemp.bottom	= rectTarget.bottom - bmpData.nBottomHeight;	
	
	rectSourceTemp.left		= rectSrc.left + bmpData.nLeftWidth;
	rectSourceTemp.top		= rectSrc.top +  bmpData.nTopHeight ;
	rectSourceTemp.right	= rectSrc.left + bmpData.nLeftWidth + bmpData.nMiddleWidth;
	rectSourceTemp.bottom	= rectSrc.bottom - bmpData.nBottomHeight;
	dc.StretchBlt( rectTargetTemp.left, rectTargetTemp.top, rectTargetTemp.Width(), rectTargetTemp.Height(), &memDC, rectSourceTemp.left, rectSourceTemp.top, rectSourceTemp.Width(), rectSourceTemp.Height(),SRCCOPY);

	// 中部, 右
	rectTargetTemp.left		= rectTarget.right - bmpData.nRightWidth;
	rectTargetTemp.top		= rectTarget.top + bmpData.nTopHeight;
	rectTargetTemp.right	= rectTarget.right ;
	rectTargetTemp.bottom	= rectTarget.bottom - bmpData.nBottomHeight;
	
	rectSourceTemp.left		= rectSrc.right - bmpData.nRightWidth;
	rectSourceTemp.top		= rectSrc.top +  bmpData.nTopHeight ;
	rectSourceTemp.right	= rectSrc.right;
	rectSourceTemp.bottom	= rectSrc.bottom - bmpData.nBottomHeight;
	dc.StretchBlt( rectTargetTemp.left, rectTargetTemp.top, rectTargetTemp.Width(), rectTargetTemp.Height(), &memDC, rectSourceTemp.left, rectSourceTemp.top, rectSourceTemp.Width(), rectSourceTemp.Height(),SRCCOPY);

	// 下部,左
	rectTargetTemp.left		= rectTarget.left;
	rectTargetTemp.top		= rectTarget.bottom - bmpData.nBottomHeight;
	rectTargetTemp.right	= rectTarget.left + bmpData.nLeftWidth ;
	rectTargetTemp.bottom	= rectTarget.bottom;	
	
	rectSourceTemp.left		= rectSrc.left;
	rectSourceTemp.top		= rectSrc.bottom - bmpData.nBottomHeight;
	rectSourceTemp.right	= rectSrc.left + bmpData.nLeftWidth;
	rectSourceTemp.bottom	= rectSrc.bottom;
	dc.BitBlt( rectTargetTemp.left, rectTargetTemp.top, rectTargetTemp.Width(), rectTargetTemp.Height(), &memDC, rectSourceTemp.left, rectSourceTemp.top, SRCCOPY);

	// 下部,中
	rectTargetTemp.left		= rectTarget.left + bmpData.nLeftWidth;
	rectTargetTemp.top		= rectTarget.bottom - bmpData.nBottomHeight;
	rectTargetTemp.right	= rectTarget.right - bmpData.nRightWidth;
	rectTargetTemp.bottom	= rectTarget.bottom;
	
	rectSourceTemp.left		= rectSrc.left + bmpData.nLeftWidth;
	rectSourceTemp.top		= rectSrc.bottom - bmpData.nBottomHeight;
	rectSourceTemp.right	= rectSrc.left + bmpData.nLeftWidth + bmpData.nMiddleWidth;
	rectSourceTemp.bottom	= rectSrc.bottom;
	dc.StretchBlt( rectTargetTemp.left, rectTargetTemp.top, rectTargetTemp.Width(), rectTargetTemp.Height(), &memDC, rectSourceTemp.left, rectSourceTemp.top, rectSourceTemp.Width(), rectSourceTemp.Height(),SRCCOPY);
	
	// 下部,右
	rectTargetTemp.left		= rectTarget.right - bmpData.nRightWidth;
	rectTargetTemp.top		= rectTarget.bottom - bmpData.nBottomHeight;
	rectTargetTemp.right	= rectTarget.right ;
	rectTargetTemp.bottom	= rectTarget.bottom;
	
	rectSourceTemp.left		= rectSrc.right - bmpData.nRightWidth;
	rectSourceTemp.top		= rectSrc.bottom - bmpData.nBottomHeight;
	rectSourceTemp.right	= rectSrc.right;
	rectSourceTemp.bottom	= rectSrc.bottom;
	dc.BitBlt( rectTargetTemp.left, rectTargetTemp.top, rectTargetTemp.Width(), rectTargetTemp.Height(), &memDC, rectSourceTemp.left, rectSourceTemp.top, SRCCOPY);
	

	// release resource;
	memDC.SelectObject( pOldBmp );

	return S_OK;
}

HRESULT CDCTool::DrawText(CDC &dc, const CString &strText, CRect rectTarget, UINT uFormat)
{
	CRect rectTemp(rectTarget);
	dc.DrawText( strText, &rectTemp, uFormat | DT_CALCRECT);
	
	rectTarget.top += ( rectTarget.Height() - rectTemp.Height() ) / 2;
	rectTarget.left += ( rectTarget.Width() - rectTemp.Width() ) / 2;

	dc.DrawText( strText, rectTarget, uFormat);

	return S_OK;
}


HRGN CDCTool::BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor, COLORREF   cTolerance)   
{   
	HRGN   hRgn   =   NULL;   
	
	ASSERT(hBmp);   
	if   (hBmp)   
	{   
		//   Create   a   memory   DC   inside   which   we   will   scan   the   bitmap   content   
		HDC   hMemDC   =   CreateCompatibleDC(NULL);   
		ASSERT(hMemDC);   
		if   (hMemDC)   
		{   
			//   Get   bitmap   size   
			BITMAP   bm;   
			GetObject(hBmp,   sizeof(bm),   &bm);   
			
			//   Create   a   32   bits   depth   bitmap   and   select   it   into   the   memory   DC   
			BITMAPINFOHEADER   RGB32BITSBITMAPINFO   =   {   
				sizeof(BITMAPINFOHEADER), //   biSize   
					bm.bmWidth, //   biWidth;   
					bm.bmHeight, //   biHeight;   
					1, //   biPlanes;   
					32, //   biBitCount   
					BI_RGB, //   biCompression;   
					0, //   biSizeImage;   
					0, //   biXPelsPerMeter;   
					0, //   biYPelsPerMeter;   
					0, //   biClrUsed;   
					0 //   biClrImportant;   
			};   
			VOID   *   pbits32;   
			HBITMAP   hbm32   =   CreateDIBSection(hMemDC,   (BITMAPINFO   *)&RGB32BITSBITMAPINFO,   DIB_RGB_COLORS,   &pbits32,   NULL,   0);   
			ASSERT(hbm32);   
			if   (hbm32)   
			{   
				HBITMAP   holdBmp   =   (HBITMAP)SelectObject(hMemDC,   hbm32);   
				
				//   Create   a   DC   just   to   copy   the   bitmap   into   the   memory   D   
				HDC   hDC   =   CreateCompatibleDC(hMemDC);   
				ASSERT(hDC);   
				if   (hDC)   
				{   
					//   Get   how   many   bytes   per   row   we   have   for   the   bitmap   bits   (rounded   up   to   32   bits   
					BITMAP   bm32;   
					VERIFY(GetObject(hbm32,   sizeof(bm32),   &bm32));   
					while   (bm32.bmWidthBytes   %   4)   
						bm32.bmWidthBytes++;   
					
					//   Copy   the   bitmap   into   the   memory   D   
					HBITMAP   holdBmp   =   (HBITMAP)SelectObject(hDC,   hBmp);   
					VERIFY(BitBlt(hMemDC,   0,   0,   bm.bmWidth,   bm.bmHeight,   hDC,   0,   0,   SRCCOPY));   
					
					//   For   better   performances,   we   will   use   the   ExtCreateRegion()   function   to   create   th   
					//   region.   This   function   take   a   RGNDATA   structure   on   entry.   We   will   add   rectangles   b   
					//   amount   of   ALLOC_UNIT   number   in   this   structure   
#define   ALLOC_UNIT 100   
					DWORD   maxRects   =   ALLOC_UNIT;   
					HANDLE   hData   =   GlobalAlloc(GMEM_MOVEABLE,   sizeof(RGNDATAHEADER)   +   (sizeof(RECT)   *   maxRects));   
					RGNDATA   *pData   =   (RGNDATA   *)GlobalLock(hData);   
					pData->rdh.dwSize   =   sizeof(RGNDATAHEADER);   
					pData->rdh.iType   =   RDH_RECTANGLES;   
					pData->rdh.nCount   =   pData->rdh.nRgnSize   =   0;   
					SetRect(&pData->rdh.rcBound,   MAXLONG,   MAXLONG,   0,   0);   
					
					//   Keep   on   hand   highest   and   lowest   values   for   the   "transparent"   pixel   
					BYTE   lr   =   GetRValue(cTransparentColor);   
					BYTE   lg   =   GetGValue(cTransparentColor);   
					BYTE   lb   =   GetBValue(cTransparentColor);   
					BYTE   hr   =   min(0xff,   lr   +   GetRValue(cTolerance));   
					BYTE   hg   =   min(0xff,   lg   +   GetGValue(cTolerance));   
					BYTE   hb   =   min(0xff,   lb   +   GetBValue(cTolerance));   
					
					//   Scan   each   bitmap   row   from   bottom   to   top   (the   bitmap   is   inverted   vertically   
					BYTE   *p32   =   (BYTE   *)bm32.bmBits   +   (bm32.bmHeight   -   1)   *   bm32.bmWidthBytes;   
					for   (int   y   =   0;   y   <   bm.bmHeight;   y++)   
					{   
						//   Scan   each   bitmap   pixel   from   left   to   righ   
						for   (int   x   =   0;   x   <   bm.bmWidth;   x++)   
						{   
							//   Search   for   a   continuous   range   of   "non   transparent   pixels"   
							int   x0   =   x;   
							LONG   *p   =   (LONG   *)p32   +   x;   
							while   (x   <   bm.bmWidth)   
							{   
								BYTE   b   =   GetRValue(*p);   
								if   (b   >=   lr   &&   b   <=   hr)   
								{   
									b   =   GetGValue(*p);   
									if   (b   >=   lg   &&   b   <=   hg)   
									{   
										b   =   GetBValue(*p);   
										if   (b   >=   lb   &&   b   <=   hb)   
											//   This   pixel   is   "transparent"   
											break;   
									}   
								}   
								p++;   
								x++;   
							}   
							
							if   (x   >   x0)   
							{   
								//   Add   the   pixels   (x0,   y)   to   (x,   y+1)   as   a   new   rectangle   in   the   regio   
								if   (pData->rdh.nCount   >=   maxRects)   
								{   
									GlobalUnlock(hData);   
									maxRects   +=   ALLOC_UNIT;   
									VERIFY(hData   =   GlobalReAlloc(hData,   sizeof(RGNDATAHEADER)   +   (sizeof(RECT)   *   maxRects),   GMEM_MOVEABLE));   
									pData   =   (RGNDATA   *)GlobalLock(hData);   
									ASSERT(pData);   
								}   
								RECT   *pr   =   (RECT   *)&pData->Buffer;   
								SetRect(&pr[pData->rdh.nCount],   x0,   y,   x,   y+1);   
								if   (x0   <   pData->rdh.rcBound.left)   
									pData->rdh.rcBound.left   =   x0;   
								if   (y   <   pData->rdh.rcBound.top)   
									pData->rdh.rcBound.top   =   y;   
								if   (x   >   pData->rdh.rcBound.right)   
									pData->rdh.rcBound.right   =   x;   
								if   (y+1   >   pData->rdh.rcBound.bottom)   
									pData->rdh.rcBound.bottom   =   y+1;   
								pData->rdh.nCount++;   
								
								//   On   Windows98,   ExtCreateRegion()   may   fail   if   the   number   of   rectangles   is   to   
								//   large   (ie:   >   4000).   Therefore,   we   have   to   create   the   region   by   multiple   steps   
								if   (pData->rdh.nCount   ==   2000)   
								{   
									HRGN   h   =   ExtCreateRegion(NULL,   sizeof(RGNDATAHEADER)   +   (sizeof(RECT)   *   maxRects),   pData);   
									ASSERT(h);   
									if   (hRgn)   
									{   
										CombineRgn(hRgn,   hRgn,   h,   RGN_OR);   
										DeleteObject(h);   
									}   
									else   
										hRgn   =   h;   
									pData->rdh.nCount   =   0;   
									SetRect(&pData->rdh.rcBound,   MAXLONG,   MAXLONG,   0,   0);   
								}   
							}   
						}   
						
						//   Go   to   next   row   (remember,   the   bitmap   is   inverted   vertically   
						p32   -=   bm32.bmWidthBytes;   
					}   
					
					//   Create   or   extend   the   region   with   the   remaining   rectangle   
					HRGN   h   =   ExtCreateRegion(NULL,   sizeof(RGNDATAHEADER)   +   (sizeof(RECT)   *   maxRects),   pData);   
					ASSERT(h);   
					if   (hRgn)   
					{   
						CombineRgn(hRgn,   hRgn,   h,   RGN_OR);   
						DeleteObject(h);   
					}   
					else   
						hRgn   =   h;   
					
					//   Clean   u   
					SelectObject(hDC,   holdBmp);   
					DeleteDC(hDC);   
				}   
				
				DeleteObject(SelectObject(hMemDC,   holdBmp));   
			}   
			
			DeleteDC(hMemDC);   
		}   
	}   
	
	return   hRgn;   
}   

HRESULT CDCTool::ConvertToGrayScale(CBitmap &bitmap)
{
	HRESULT hr = E_FAIL;
	
	return hr;
	
}

HRESULT CDCTool::Buffer2IStream( byte* pBuffer, DWORD dwLength, IStream** pStream )
{
	REGISTER_FUNC( HRESULT CDCTool::Buffer2IStream( byte* pBuffer, DWORD dwLength, IStream** pStream ) );
	HRESULT hr = E_FAIL;

	try
	{
		VERIFY_TRUE( pBuffer != NULL && dwLength > 0 );
		HGLOBAL hMem = GlobalAlloc( GMEM_FIXED, dwLength); 
		BYTE* pmem = (BYTE*)GlobalLock( hMem ); 
		memcpy( pmem, pBuffer, dwLength ); 

		CreateStreamOnHGlobal( hMem, TRUE, pStream ); 				
		GlobalUnlock( hMem ); 		
		
		hr = S_OK;
	}
	my_catch;

	return hr;

}

HRESULT	CDCTool::Resource2IStream( HINSTANCE hResourceHandle, UINT nID, LPCTSTR lpszType, IStream** pStram )
{
	REGISTER_FUNC( HRESULT	CDCTool::Resource2IStream( HINSTANCE hResourceHandle, UINT nID, LPCTSTR lpszType, IStream** pStram ) );
	HRESULT hr = E_FAIL;

	try
	{
		HRSRC hRsrc = ::FindResource( hResourceHandle, MAKEINTRESOURCE(nID), lpszType);   //   type 
		VERIFY_TRUE( hRsrc != NULL ); 
		
		DWORD dwLen = SizeofResource( hResourceHandle, hRsrc ); 
		BYTE* lpRsrc = (BYTE*)LoadResource( hResourceHandle, hRsrc); 
		VERIFY_TRUE( lpRsrc != NULL );		
		Buffer2IStream( lpRsrc, dwLen, pStram );
		FreeResource( lpRsrc ); 

		hr = S_OK;
	}
	my_catch;

	return hr;
}

HRESULT CDCTool::ImageFromBuffer( byte* pBuffer, DWORD dwLength, Gdiplus::Image **pImg )
{
	REGISTER_FUNC( HRESULT CDCTool::ImageFromBuffer(UINT nID, const CString &strResourceType, Gdiplus::Image *&pImg) );
	HRESULT hr = E_FAIL;
	IStream *pStream = NULL;

	try
	{
		VERIFY_SUCCEEDED( Buffer2IStream( pBuffer, dwLength, &pStream) );
		*pImg = Gdiplus::Image::FromStream( pStream );		
		VERIFY_TRUE( *pImg != NULL );	

		hr = S_OK;
	}
	my_catch;

	if ( pStream )
	{
		pStream->Release();
	}

	return hr;

}

HRESULT CDCTool::ImageFromResource(UINT nID, const CString &strResourceType, Gdiplus::Image *&pImg)
{
	SET_FUNC_NAME( _T("CDCTool::ImageFromResource(UINT nID, const CString &strResourcename, Image *&pImg)") );
	HRESULT hr = E_FAIL;
	IStream* pStm = NULL;
	HINSTANCE hInst = AfxGetResourceHandle();

	pImg = NULL;

	try
	{		
		VERIFY_TRUE( hInst != NULL );
		VERIFY_SUCCEEDED( Resource2IStream( hInst, nID, strResourceType, &pStm) );
		pImg = Gdiplus::Image::FromStream( pStm );		
		VERIFY_TRUE( pImg != NULL );		
		hr = S_OK;
	}
	my_catch;

	if ( pStm )
	{
		pStm->Release();
	}
	return hr;

}

HRESULT CDCTool::SetLayerWindowBackground(CWnd* pWnd, UINT nImageID, const CString &strImageType, double dMagnify)
{
	SET_FUNC_NAME( _T("CDCTool::SetLayerWindowBackground(HWND hWnd, UINT nImageID, const CString &strImageType)") );
	HRESULT hr = E_FAIL;
	Gdiplus::Image* pImage = NULL;

	try
	{
		VERIFY_TRUE( pWnd != NULL );
		VERIFY_TRUE( nImageID != NULL );
		VERIFY_SUCCEEDED( ImageFromResource( nImageID, strImageType, pImage) );
		VERIFY_TRUE( pImage != NULL );
		VERIFY_SUCCEEDED( SetLayerWindowBackground( pWnd, pImage, dMagnify) );
		hr = S_OK;
	}
	my_catch;

	// free resource
	if ( pImage != NULL )
	{
		delete pImage;
	}

	return hr;
}

HRESULT CDCTool::SetLayerWindowBackground(CWnd *pWnd, Gdiplus::Image *pImage, double dMagnify)
{
	SET_FUNC_NAME( _T("CDCTool::SetLayerWindowBackground(CWnd *pWnd, Gdiplus::Image *pImage)") );
	HRESULT hr = E_FAIL;
	
	try
	{
		VERIFY_TRUE( pWnd != NULL && IsWindow( pWnd->GetSafeHwnd()) );
		VERIFY_TRUE( pImage != NULL );
		VERIFY_TRUE( SetWindowLong( pWnd->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong( pWnd->GetSafeHwnd(), GWL_EXSTYLE) & ~WS_EX_LAYERED ) != 0 );

		CRect rectWin;
		pWnd->GetWindowRect( &rectWin );

		CClientDC dc( pWnd );
		CBitmap bmp;
		CBitmap *pOldBmp = NULL;
		CDC dcMemory;
		CDC* pDcScreen = NULL;
		
		int nWidth = 0;
		int nHeight = 0;
		
		VERIFY_TRUE( (nWidth = pImage->GetWidth()) != 0 );	// yes, =, not ==
		VERIFY_TRUE( (nHeight = pImage->GetHeight()) != 0 ); // yes, =, not ==
		
		pWnd->MoveWindow( rectWin.left, rectWin.top,  static_cast<int>(nWidth * dMagnify), static_cast<int>(nHeight * dMagnify));
		pWnd->GetWindowRect( &rectWin );

		pDcScreen = pWnd->GetDC();
		dcMemory.CreateCompatibleDC( &dc );
		bmp.CreateCompatibleBitmap( &dc, static_cast<int>(nWidth * dMagnify), static_cast<int>(nHeight * dMagnify) );
		pOldBmp = dcMemory.SelectObject( &bmp );
		Gdiplus::Graphics graphic( dcMemory.GetSafeHdc() );		
		
		
		Gdiplus::Point points[] = { Gdiplus::Point(0, 0), Gdiplus::Point( static_cast<int>(nWidth * dMagnify), 0), Gdiplus::Point(0, static_cast<int>(nHeight * dMagnify)) };
		graphic.DrawImage( pImage, points, 3);
				
		POINT point = { 0, 0};
		BLENDFUNCTION blend;
		
		POINT ptDst = { rectWin.left, rectWin.top};
		SIZE sizeDst = { rectWin.Width(), rectWin.Height() };
		// 
		
		VERIFY_TRUE( SetWindowLong( pWnd->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong( pWnd->GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED ) != 0 );
		
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags  = 0;
		blend.SourceConstantAlpha = 192;
		blend.AlphaFormat = AC_SRC_ALPHA;
		UpdateLayeredWindow( pWnd->GetSafeHwnd(), pDcScreen->GetSafeHdc(), &ptDst, &sizeDst, dcMemory.GetSafeHdc(),
			&point, 0, &blend, LWA_ALPHA);		
		
		// 
		dcMemory.SelectObject( pOldBmp );
		
		pWnd->ReleaseDC( pDcScreen );
		pDcScreen = NULL;
		
		hr = S_OK;
	}
	my_catch;

	return hr;

}


