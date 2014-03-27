// FileInfoExtract_MPG.h: interface for the CFileInfoExtract_MPG class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEINFOEXTRACT_MPG_H__35669BE6_71D9_45D0_9E23_4F0D26150EF4__INCLUDED_)
#define AFX_FILEINFOEXTRACT_MPG_H__35669BE6_71D9_45D0_9E23_4F0D26150EF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileInfoExtract.h"
#include "SequenceHeader.h"
#include <list>
using namespace std ;

class _JS_Fix CFileInfoExtract_MPG : public CFileInfoExtract  
{
public:
	CFileInfoExtract_MPG();
	virtual ~CFileInfoExtract_MPG();
	
	VOID FileEndAnalyse(BYTE* pBuf,LONG offset1, LONG offset2,LONG rem);
	BOOL AnalyzeFile();
	
	
	SequenceHeader m_SequenceHeader;
	
protected:
	bool AddStreamID(ULONG ID);
	bool RemoveStreamID(ULONG ID);
	bool FindStreamID(ULONG ID);
	long FindStartCode(const BYTE* pData,long nLen,DWORD code);

	long PES_Packet(const BYTE* pBuf,long nLen);
	long Pack_Header(const BYTE* pBuf,long nLen);
//	bool System_Header(const BYTE* pBuf,long nLen);
	
public:
	struct Time{
		INT hours;
		INT minutes;
		INT seconds;
	};

protected:
	list<ULONG> m_StreamIDList;

};

#endif // !defined(AFX_FILEINFOEXTRACT_MPG_H__35669BE6_71D9_45D0_9E23_4F0D26150EF4__INCLUDED_)
