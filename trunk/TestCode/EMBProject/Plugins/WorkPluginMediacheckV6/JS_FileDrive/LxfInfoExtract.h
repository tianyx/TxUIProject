// LxfInfoExtract.h: interface for the CLxfInfoExtract class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LXFINFOEXTRACT_H__CA26F085_C324_4735_968D_F2CE7D95BDCF__INCLUDED_)
#define AFX_LXFINFOEXTRACT_H__CA26F085_C324_4735_968D_F2CE7D95BDCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SequenceHeader.h"
#include "FileInfoExtract.h"

class _JS_Fix CLxfInfoExtract : public CFileInfoExtract  
{
public:
	SequenceHeader aa;
	VOID PacketAnalyze_Video(BYTE *pBuf, LONG offset);
	VOID PacketAnalyze_Audio(BYTE *pBuf, LONG offset);
	VOID SequenceAnalyze_Video(BYTE* pBuf,LONG offset);
	CLxfInfoExtract();
	virtual ~CLxfInfoExtract();
	BOOL AnalyzeFile();
public:
	VOID FileDuration(BYTE *pBuf, LONG offset);
	int m_VersionTrans;
	int m_Version;

	BOOL SequenceAnalyze_Audio(BYTE* pBuf,LONG offset);
	    static const long SampleRates[2][4];
		static const double FrameRates[9];
 		static const long LxfReadHead;
 		static const long LxfReadEnd;

private:
	BOOL bTransClip;

};
	

#endif // !defined(AFX_LXFINFOEXTRACT_H__CA26F085_C324_4735_968D_F2CE7D95BDCF__INCLUDED_)
