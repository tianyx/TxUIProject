// AVIInfoExtract.h: interface for the CAVIInfoExtract class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVIINFOEXTRACT_H__4AE8D9CE_EBB5_49EE_A58D_E9627DD4F9AF__INCLUDED_)
#define AFX_AVIINFOEXTRACT_H__4AE8D9CE_EBB5_49EE_A58D_E9627DD4F9AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SequenceHeader.h"
#include "FileInfoExtract.h"
#include "JS_FileDrive.h"

class _JS_Fix CAVIInfoExtract : public CFileInfoExtract  
{
public:
	CAVIInfoExtract();
	virtual ~CAVIInfoExtract();
	virtual BOOL AnalyzeFile();
};

#endif // !defined(AFX_AVIINFOEXTRACT_H__4AE8D9CE_EBB5_49EE_A58D_E9627DD4F9AF__INCLUDED_)
