// FileInfoExtract_WMV.h: interface for the CFileInfoExtract_WMV class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEINFOEXTRACT_WMV_H__47568D2E_5D04_4AE9_A135_DDC43290F2D7__INCLUDED_)
#define AFX_FILEINFOEXTRACT_WMV_H__47568D2E_5D04_4AE9_A135_DDC43290F2D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileInfoExtract.h"

class CFileInfoExtract_WMV : public CFileInfoExtract  
{
public:
	CFileInfoExtract_WMV();
	virtual ~CFileInfoExtract_WMV();
   	virtual BOOL    AnalyzeFile();
protected:
	void GenerateGUID(GUID guid,CString &strGUID);
};

#endif // !defined(AFX_FILEINFOEXTRACT_WMV_H__47568D2E_5D04_4AE9_A135_DDC43290F2D7__INCLUDED_)
