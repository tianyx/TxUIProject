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
#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

CFont	g_fontBase;
LOGFONT	g_logfont;


CTxFontLoader::CTxFontLoader()
{
	if (::SystemParametersInfo(SPI_GETICONTITLELOGFONT,
		sizeof (g_logfont), &g_logfont, 0))
	{
		g_logfont.lfHeight = -12;
		//		VERIFY(g_fontBase.CreateFontIndirect(&logfont));
		//		logfont.lfHeight = -30;
		//		VERIFY(g_fontBig.CreateFontIndirect(&logfont));

	}
	else
	{
		memset(&g_logfont, 0, sizeof(g_logfont));
		g_logfont.lfHeight = -12;
		g_logfont.lfCharSet = DEFAULT_CHARSET;
		g_logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		g_logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		g_logfont.lfQuality = DEFAULT_QUALITY;
		g_logfont.lfPitchAndFamily = DEFAULT_PITCH;
		StrCpy(g_logfont.lfFaceName, TEXT("system"));
		//VERIFY(g_fontBase.CreateFontIndirect(&g_logfont));
		//VERIFY(g_fontBig.CreateFontIndirect(&g_logfont));

	}

	CString strfontLanTxt= TEXT("Arial");
	StrCpy(g_logfont.lfFaceName, strfontLanTxt);
	g_fontBase.CreateFontIndirect(&g_logfont);

	AddFont(g_logfont);

}

CTxFontLoader::~CTxFontLoader()
{
	Release();
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
