/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	TxImageLoader.cpp
	file base:	TxImageLoader
	file ext:	cpp
	author:		tian_yx
	purpose:	
*********************************************************************/
#include "StdAfx.h"
#include "TxImageLoader.h"
#include "io.h"
#include "FGlobal.h"

CTxImageLoader::CTxImageLoader(void)
{
}

CTxImageLoader::~CTxImageLoader(void)
{
}

Bitmap* CTxImageLoader::LoadBitmap( LPCTSTR szFileIn )
{
	CTxImageLoader& loader = GetTxImageLoader();
	Bitmap* pImage = NULL;
	CString strFile(szFileIn);
	if (_access(strFile,  04) == -1)
	{
		CString strPath = GetAppPath().c_str();
		strPath +=TEXT("\\");
		strFile = strPath + strFile;
		if (_access(strFile, 04) == -1)
		{
			ASSERT(FALSE);
			return NULL;
		}
	}

	strFile.MakeLower();
	MAPTXBMPS::iterator itf = loader.m_mapBmps.find(strFile);
	if (itf != loader.m_mapBmps.end())
	{
		pImage = itf->second;
	}
	else
	{
		CComBSTR bstrFile(strFile);
		pImage = Bitmap::FromFile(bstrFile);
		if (!pImage || pImage->GetLastStatus() != Ok)
		{
			ASSERT(FALSE);
			CFWriteLog(TEXT("LoadBitmap"), TEXT("failed load %s"), strFile);
			if (pImage)
			{
				delete pImage;
			}
		}
		else
		{
			loader.m_mapBmps[strFile] = pImage;
		}
	}

	return pImage;
}

void CTxImageLoader::Release()
{
	CTxImageLoader& loader = GetTxImageLoader();
	MAPTXBMPS::iterator itb = loader.m_mapBmps.begin();
	MAPTXBMPS::iterator ite = loader.m_mapBmps.end();
	for (; itb != ite; itb++)
	{
		delete itb->second;
	}
	
	loader.m_mapBmps.clear();
}

CTxImageLoader& CTxImageLoader::GetTxImageLoader()
{
	static CTxImageLoader g_ImgLoader;
	return g_ImgLoader;
}
