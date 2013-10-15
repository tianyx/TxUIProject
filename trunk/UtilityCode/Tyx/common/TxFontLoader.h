/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxFontLoader.h
	file base:	TxFontLoader
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once

#include <vector>
#include "GdiPlusNewHeader.h"
using namespace Gdiplus;
struct  ST_TXFONT
{
	Gdiplus::Font* pFont;
	CFont* pCFont;
	ST_TXFONT()
	{
		pFont = NULL;
		pCFont = NULL;
	}
};
typedef std::vector<ST_TXFONT> VECFONTS;
class CTxFontLoader
{
public:
	CTxFontLoader(void);
	~CTxFontLoader(void);
	CFont* GetCFont(int nId);
	Gdiplus::Font* GetFont(int nId);
	int AddFont(LOGFONT& lf);
	int AddFont(LPCTSTR szFamilyName, int nHeight, BOOL bBlod = FALSE);
	void Release();
private:
	VECFONTS m_vFonts;
};
