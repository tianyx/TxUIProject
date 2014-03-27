// FileInfoExtract_Hik.h: interface for the CFileInfoExtract_Hik class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEINFOEXTRACT_HIK_H__53C95882_0E9A_406F_BB41_06DAC456BB5C__INCLUDED_)
#define AFX_FILEINFOEXTRACT_HIK_H__53C95882_0E9A_406F_BB41_06DAC456BB5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileInfoExtract.h"

class _JS_Fix CFileInfoExtract_Hik : public CFileInfoExtract  
{
public:
	CFileInfoExtract_Hik();
	virtual ~CFileInfoExtract_Hik();
	BOOL    AnalyzeFile();

};

#endif // !defined(AFX_FILEINFOEXTRACT_HIK_H__53C95882_0E9A_406F_BB41_06DAC456BB5C__INCLUDED_)
