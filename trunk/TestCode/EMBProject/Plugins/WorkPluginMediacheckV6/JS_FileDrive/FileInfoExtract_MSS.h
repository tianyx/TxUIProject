// FileInfoExtract_PNCL.h: interface for the CFileInfoExtract_PNCL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEINFOEXTRACT_PNCL_H__1953FE4E_FFFC_4861_B8DC_BECB079BDCEB__INCLUDED_)
#define AFX_FILEINFOEXTRACT_PNCL_H__1953FE4E_FFFC_4861_B8DC_BECB079BDCEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FileInfoExtract.h"
#include "SequenceHeader.h"

class _JS_Fix CFileInfoExtract_MSS : public CFileInfoExtract 
{
public:
	CFileInfoExtract_MSS();
	virtual ~CFileInfoExtract_MSS();

	VOID FileEndAnalyse(BYTE* pBuf,LONG offset1, LONG offset2,LONG rem);
	BOOL AnalyzeFile();


	SequenceHeader m_SequenceHeader;
    
protected:
	bool AnalyPSStream(const BYTE* pBuf,long nLen);
	long FindStartCode(const BYTE* pData,long nLen,DWORD code);

public:
	struct Time{
		INT hours;
		INT minutes;
		INT seconds;
		INT picNums;
	};
     Time m_FirstGOPTime;
};

#endif // !defined(AFX_FILEINFOEXTRACT_PNCL_H__1953FE4E_FFFC_4861_B8DC_BECB079BDCEB__INCLUDED_)
