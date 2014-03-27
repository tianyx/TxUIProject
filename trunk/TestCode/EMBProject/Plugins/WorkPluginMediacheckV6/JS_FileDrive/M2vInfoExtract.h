// M2vInfoExtract.h: interface for the CM2vInfoExtract class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_M2VINFOEXTRACT_H__D2EB4598_DE79_4831_B4E7_D06E50B0B3A6__INCLUDED_)
#define AFX_M2VINFOEXTRACT_H__D2EB4598_DE79_4831_B4E7_D06E50B0B3A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileInfoExtract.h"
#include "SequenceHeader.h"

class _JS_Fix CM2vInfoExtract : public CFileInfoExtract  
{
public:
	VOID FileEndAnalyse(BYTE* pBuf,LONG offset1, LONG offset2,LONG rem);
	BOOL AnalyzeFile();
	CM2vInfoExtract();
	virtual ~CM2vInfoExtract();
	SequenceHeader m_SequenceHeader;
	static const long M2vReadHead;
 	static const long M2vReadEnd;	
	static const double FrameRates[9];


protected:
public:
	struct Time{
		INT hours;
		INT minutes;
		INT seconds;
	};

};

#endif // !defined(AFX_M2VINFOEXTRACT_H__D2EB4598_DE79_4831_B4E7_D06E50B0B3A6__INCLUDED_)
