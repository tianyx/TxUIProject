/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxImageLoader.h
	file base:	TxImageLoader
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma once
#include "GdiPlusNewHeader.h"
#include <map>

using namespace Gdiplus;
typedef std::map<CString, Bitmap*> MAPTXBMPS;
class CTxImageLoader
{
private:
	CTxImageLoader(void);
	~CTxImageLoader(void);
public:

private:
	MAPTXBMPS m_mapBmps;

public:
	static CTxImageLoader& GetTxImageLoader();
	static Bitmap* LoadBitmap(LPCTSTR szFileIn);
	static void Release();

};
