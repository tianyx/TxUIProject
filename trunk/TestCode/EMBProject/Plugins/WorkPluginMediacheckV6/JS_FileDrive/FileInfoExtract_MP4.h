// FileInfoExtract_MP4.h: interface for the CFileInfoExtract_MP4 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEINFOEXTRACT_MP4_H__1FD881AB_05D1_4A8D_B805_667040C3E116__INCLUDED_)
#define AFX_FILEINFOEXTRACT_MP4_H__1FD881AB_05D1_4A8D_B805_667040C3E116__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileInfoExtract.h"

class _JS_Fix CFileInfoExtract_MP4 : public CFileInfoExtract  
{
public:
	CFileInfoExtract_MP4();
	virtual ~CFileInfoExtract_MP4();
	virtual BOOL    AnalyzeFile();
};

#endif // !defined(AFX_FILEINFOEXTRACT_MP4_H__1FD881AB_05D1_4A8D_B805_667040C3E116__INCLUDED_)
