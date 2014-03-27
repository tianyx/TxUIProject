// InfoExtract_ASF.h: interface for the CInfoExtract_ASF class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOEXTRACT_ASF_H__18C6364D_1BDE_404E_BF38_7D8510A711AC__INCLUDED_)
#define AFX_INFOEXTRACT_ASF_H__18C6364D_1BDE_404E_BF38_7D8510A711AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FileInfoExtract.h"

class CFileInfoExtract_ASF : public CFileInfoExtract 
{
public:
	CFileInfoExtract_ASF();
	virtual ~CFileInfoExtract_ASF();
	virtual BOOL    AnalyzeFile();

protected:
	void AnalyzeVC_1Data(const BYTE* pBuffer,__int64 llSize);
	void AnalyzeCodecInfo(const BYTE *pBuffer);
	void AnalyzeAudioInfo(const BYTE* pBuffer);
	void AnalyzeVideoInfo(const BYTE* pBuffer);
	void AnalyzeFileProperties(const BYTE *pBuffer);
protected:
	//VC-1 Start Code
	enum{
		Sequence_Start_Code    = 0x0000010F,
		Sequence_End_Code      = 0x0000010A,
        Entry_Point_Start_Code = 0x0000010E,
		Frame_Data_Code        = 0x0000011D,
		Field_Data_Code        = 0x0000011C,
		Sequence_Data_Code     = 0x0000011F,
        Entry_Point_Data_Code  = 0x0000011E,
	};

	int m_VideoStreamId;
	int m_AudioStreamId;

};

#endif // !defined(AFX_INFOEXTRACT_ASF_H__18C6364D_1BDE_404E_BF38_7D8510A711AC__INCLUDED_)
