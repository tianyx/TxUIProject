/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxFontLoader.cpp
	file base:	TxFontLoader
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "TxFontLoader.h"

CTxFontLoader::CTxFontLoader(void)
{
}

CTxFontLoader::~CTxFontLoader(void)
{
}

int CTxFontLoader::AddFont( LOGFONT& lf )
{
	ST_TXFONT txFont;
	txFont.pCFont =new CFont;
	txFont.pCFont->CreateFontIndirect(&lf);

	CString m_strGCFontName = lf.lfFaceName;
	//draw countdown text
	CComBSTR btrFontName(m_strGCFontName);
	FontFamily  fontFamily(btrFontName);
	txFont.pFont = new Gdiplus::Font(&fontFamily, -lf.lfHeight, lf.lfWeight == 700?FontStyleBold:FontStyleRegular, UnitPixel);
	m_vFonts.push_back(txFont);
	
	return m_vFonts.size()-1;

}

int CTxFontLoader::AddFont( LPCTSTR szFamilyName, int nHeight , BOOL bBlod/* = FALSE*/)
{
	LOGFONT lf = g_logfont;
	StrCpy(lf.lfFaceName, szFamilyName);
	lf.lfHeight = -nHeight;
	if (bBlod)
	{
		lf.lfWeight = 700;
	}
	return AddFont(lf);
}


CFont* CTxFontLoader::GetCFont( int nId )
{
	if (nId < 0 || nId >= m_vFonts.size())
	{
		ASSERT(FALSE);
		return m_vFonts[0].pCFont;
	}

	return m_vFonts[nId].pCFont;
}

void CTxFontLoader::Release()
{
	for (int i = 0; i < m_vFonts.size(); i++)
	{
		delete m_vFonts[i].pCFont;
		delete m_vFonts[i].pFont;
	}

	m_vFonts.clear();
}

Gdiplus::Font* CTxFontLoader::GetFont( int nId )
{
	if (nId < 0 || nId >= m_vFonts.size())
	{
		ASSERT(FALSE);
		return m_vFonts[0].pFont;
	}

	return m_vFonts[nId].pFont;
}
