#if !defined(AFX_MEMDC_H__75FED539_1CA2_4C25_920E_ABD04348B248__INCLUDED_)
#define AFX_MEMDC_H__75FED539_1CA2_4C25_920E_ABD04348B248__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MemDC.h : header file
//
//注意:本类本不适合绘制动画时动态创建
/////////////////////////////////////////////////////////////////////////////

class CMemDC : public CDC
{
// Construction
public:
	CMemDC(CDC* pDc, long width = 0,long heigh = 0);

// Attributes
public:

// Implementation
public:
	BOOL OpenMemDC(CDC* pDc, long width,long heigh);
	HBITMAP CloseMemDC();
	virtual ~CMemDC();

private:
	CBitmap  m_bitmap;      
    CBitmap* m_pOldBitmap;
	UINT m_bmpHeigh;
	UINT m_bmpWidth;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMDC_H__75FED539_1CA2_4C25_920E_ABD04348B248__INCLUDED_)
