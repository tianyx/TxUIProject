// DBStretchImage.h: interface for the DBStretchImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBSTRETCHIMAGE_H__0E093D54_A71E_4A41_A7C1_8043746506D4__INCLUDED_)
#define AFX_DBSTRETCHIMAGE_H__0E093D54_A71E_4A41_A7C1_8043746506D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
#include "MemDC.h"

struct  BMPDIVPART
{
	CBitmap* m_pbmpMiddle;
	CBitmap* m_pbmpStart;
	CBitmap* m_pbmpEnd;
	int nWidth;
	int nHeight;

	BMPDIVPART()
	{
		m_pbmpMiddle = NULL;
		m_pbmpStart = NULL;
		m_pbmpEnd = NULL;
		nWidth = 0;
		nHeight = 0;
	}

}; 

struct BMPDIVKEY
{
	int nPosFir;
	int nPosSec;
	BOOL bHStretch;
	BOOL bEdgeStretch;
	CString strPath;
	int nBtnPart; //-1 for not use, -2 for resource, 0, 1,2,3 for normal, over, sel, disable 

	BMPDIVKEY()
	{
		nPosFir = 2;
		nPosSec = 4;
		bHStretch = TRUE;
		bEdgeStretch = FALSE;
		nBtnPart = -1;

	}

	bool operator ==(const BMPDIVKEY& other) const
	{
		if (&other == this)
		{
			return true;
		}
		else
		{
			return nPosFir == other.nPosFir && nPosSec == other.nPosSec
				&& bHStretch == other.bHStretch && strPath == other.strPath
				&& nBtnPart == other.nBtnPart;
		}
	}

	bool operator <(const BMPDIVKEY& other) const
	{
		if (&other == this)
		{
			return false;
		}
		else
		{
			return strPath < other.strPath ;
		}
	}
};

typedef std::pair<BMPDIVKEY, BMPDIVPART> IMGDIVPAIR;
typedef std::vector<IMGDIVPAIR> MAP_IMGDIVDATA;

class DBStretchImage  
{
public:
	struct STRENTCH{
		public:
			static HBITMAP ScaleBitmap( HDC hDC, HBITMAP hBmp, WORD wNewWidth, WORD wNewHeight, BOOL fastScale );
			
		private:
			// helper function prototypes 
			static BITMAPINFO *PrepareRGBBitmapInfo(WORD wWidth, 
				WORD wHeight); 
			static int * CreateCoeffInt(int nLen, int nNewLen, BOOL bShrink);
			static float * CreateCoeff(int nLen, int nNewLen, BOOL bShrink);
			static void ShrinkDataInt( BYTE *pInBuff, WORD wWidth, WORD wHeight, 
				BYTE *pOutBuff, WORD wNewWidth, WORD wNewHeight); 
			static void EnlargeDataInt( BYTE *pInBuff, WORD wWidth, WORD wHeight, 
				BYTE *pOutBuff, WORD wNewWidth, WORD wNewHeight); 
			static void ShrinkData( BYTE *pInBuff, WORD wWidth, WORD wHeight, 
				BYTE *pOutBuff, WORD wNewWidth, WORD wNewHeight); 
			static void EnlargeData( BYTE *pInBuff, WORD wWidth, WORD wHeight, 
				BYTE *pOutBuff, WORD wNewWidth, WORD wNewHeight); 
	};

public:
	HBITMAP		m_hBmpDib;

	BOOL LoadDibSection(LPCTSTR lpszBmpPath);
	BOOL IsEmpty();
	void SetBitmap(CBitmap *pBitmap);
	int Height() const;
	int Width() const;
	void Render(CDC *pDC,CPoint pt,CSize size);
	void EdgeStretchRender(CDC *pDC,CPoint pt,CSize size);
	void RenderTransparent(CDC *pDC,CPoint pt,CSize size);
	
	void LoadBitmap(UINT nIDResource, 
		int iDividePosFir, int iDividePosSec, BOOL bHStretch = TRUE);

	void LoadBitmap(LPCSTR lpszBitmap, 
		int iDividePosFir, int iDividePosSec, BOOL bHStretch = TRUE, BOOL bEdgeStretch = FALSE);

	void LoadBitmap(HBITMAP hBmp, 
		int iDividePosFir, int iDividePosSec, BOOL bHStretch = TRUE, BOOL bEdgeStretch = FALSE);

	
	void LoadBitmap(int nBtnPart, LPCSTR lpszBitmap, 
		int iDividePosFir, int iDividePosSec, BOOL bHStretch = TRUE, BOOL bEdgeStretch = FALSE);


	//////////////////////////////////////////////////////////////////////////

	DBStretchImage();
	virtual ~DBStretchImage();

	//clean all bitmap cache
	void Reset();

private:

	BOOL GetStaticBmpData(BMPDIVKEY& keyIn, BMPDIVPART& dataOut);


private:
	BOOL m_bHStretch;
	CSize m_szSrcBmp;
// 	CBitmap m_bmpMiddle;
// 	CBitmap m_bmpStart;
// 	CBitmap m_bmpEnd;
	int m_iDividePosFir;
	int m_iDividePosSec;
	CString m_strBmpName;
	BOOL m_bEdgeStretch;


	BMPDIVPART m_bmpDivData;
	BMPDIVKEY  m_divKey;
	//static image list;
	static BOOL m_sbInited;
	static MAP_IMGDIVDATA* m_spmap_ImgDivData;
	static BOOL SaveToDivBitmap(BMPDIVKEY& keyIn, BMPDIVPART& dataOut);

public:
	static void InitStaticData();
	static void ReleaseStaticData();

};

#endif // !defined(AFX_DBSTRETCHIMAGE_H__0E093D54_A71E_4A41_A7C1_8043746506D4__INCLUDED_)
