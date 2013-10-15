// DCTool.h: interface for the CDCTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DCTOOL_H__F4E02083_7CD0_4236_9292_37E7DBFDCB65__INCLUDED_)
#define AFX_DCTOOL_H__F4E02083_7CD0_4236_9292_37E7DBFDCB65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include<GdiPlus.h>

class Gdiplus::Image;

struct tagBKBmpData
{
	tagBKBmpData( int nData1, int nData2, int nData3, int nData4, int nData5, int nData6)
	{
		nLeftWidth	= nData1;
		nMiddleWidth= nData2;
		nRightWidth = nData3;
		nTopHeight	= nData4;
		nMiddleHeight= nData5;
	    nBottomHeight= nData6;

	}

	tagBKBmpData()
	{
		nLeftWidth	= 0;
		nMiddleWidth= 0;
		nRightWidth = 0;
		nTopHeight	= 0;
		nMiddleHeight= 0;
	    nBottomHeight= 0;
	}
	BOOL operator== ( tagBKBmpData& rhs)
	{
		return( nLeftWidth == rhs.nLeftWidth && nMiddleWidth == rhs.nMiddleWidth && nRightWidth == rhs.nRightWidth &&
				nTopHeight == rhs.nTopHeight && nMiddleHeight == rhs.nMiddleHeight && nBottomHeight == rhs.nBottomHeight);
	}

	int nLeftWidth;
	int nMiddleWidth;
	int nRightWidth;
	int nTopHeight;
	int nMiddleHeight;
	int nBottomHeight;
};

class CDCTool  
{
	
public:

	//					�����pStream->Release();
	HRESULT				Resource2IStream( HINSTANCE hResourceHandle, UINT nID, LPCTSTR lpszType, IStream** pStram );
	HRESULT				Buffer2IStream( byte* pBuffer, DWORD dwLength, IStream** pStream );

	HRESULT				SetLayerWindowBackground(CWnd* pWnd, Gdiplus::Image *pImage, double dMagnify = 1.0);
	//					��ָ����ͼƬ����ΪLayerWindow�ı���, strImageType �� nImageID ����Դ����, dMagnify �Ŵ���
	//					[example:] SetLayerWindowBackground( hWnd, BK_IMAGE, _T("PNG") );
	HRESULT				SetLayerWindowBackground( CWnd* pWnd, UINT nImageID, const CString& strImageType, double dMagnify = 1.0);
	
	//					you have to call delete to release pImage; 
	HRESULT				ImageFromResource( UINT nID, const CString& strResourceType, Gdiplus::Image * &pImg);
	HRESULT				ImageFromBuffer( byte* pBuffer, DWORD dwLength, Gdiplus::Image **pImg);


	HRESULT				ConvertToGrayScale(CBitmap& bitmap);
	//					����Bitmap�Լ�ָ����͸��ɫ����HRGN���ú������ص� HRGN ��Ҫ���� DeleteObject ɾ��
	HRGN				BitmapToRegion(HBITMAP hBmp, COLORREF cTransparentColor = 0, COLORREF cTolerance = 0x101010 );

	//					��ָ������������ʾ�ַ������ַ����ھ��������ڴ�ֱ�����Ͼ�����ʾ
	HRESULT				DrawText(CDC& dc, const CString& strText, CRect rectTarget,  UINT uFormat);

	//					��rectTargetָ���������ڻ���ͼƬ
	//					rectSrc ��ʾͼƬ������
	//					tagBKBmpData ���� rectSrc �������������
	HRESULT				DrawBkPicture(CDC& dc, CBitmap& bitmap, CRect rectTarget, CRect rectSrc = CRect(0,0,0,0), tagBKBmpData bmpData = tagBKBmpData(0,0,0,0,0,0));

	//
	CDCTool();
	virtual ~CDCTool();

};


#endif // !defined(AFX_DCTOOL_H__F4E02083_7CD0_4236_9292_37E7DBFDCB65__INCLUDED_)
