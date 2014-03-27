// FileInfoExtract_3GP.h: interface for the CFileInfoExtract_3GP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEINFOEXTRACT_3GP_H__146882DC_15B8_4FC5_941A_B99AB19CDEE7__INCLUDED_)
#define AFX_FILEINFOEXTRACT_3GP_H__146882DC_15B8_4FC5_941A_B99AB19CDEE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileInfoExtract.h"

class _JS_Fix CFileInfoExtract_3GP : public CFileInfoExtract  
{
public:
	CFileInfoExtract_3GP();
	virtual ~CFileInfoExtract_3GP();
	virtual BOOL    AnalyzeFile();
};

#endif // !defined(AFX_FILEINFOEXTRACT_3GP_H__146882DC_15B8_4FC5_941A_B99AB19CDEE7__INCLUDED_)
