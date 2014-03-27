///////////////////////////////////
//FileInfoExtract.h
//////////////////////////////////
#pragma once
#include <string>
#include "Common.h"
#include "JS_FileDrive.h"
//#define MAX_FILEPATH    512

inline DWORD SWAP_ULONG(const BYTE* pData)
{
    return DWORD((pData[0] << 24)|(pData[1] << 16)|(pData[2] << 8)|pData[3]);
}
inline __int64 SWAP_INT64(const unsigned char *src) 
{ 
	return (((__int64)SWAP_ULONG(src)) << 32) | (SWAP_ULONG(&src[4])); 
}
inline UINT SWAP_INT16(const unsigned char *src) 
{ 
	return (src[0] << 8 | src[1]); 
}
// mpeg start codes
enum
{
	    PACK_START_CODE         =   0x4C454954,
		PICTURE_START_CODE      =   0x00000100,
		SLICE_MIN_START_CODE    =   0x00000101,
		SLICE_MAX_START_CODE    =   0x000001AF,
		USER_START_CODE         =   0x000001B2,
		SEQUENCE_START_CODE     =   0x000001B3,
		SEQUENCE_EXT_CODE       =   0x000001B5,
		GOP_START_CODE          =   0x000001B8,
		PS_PACK_START_CODE      =   0x000001BA,//Program Stream pack header
		SYSTEM_START_CODE       =   0x000001BB,
		PES_PACKET_CODES        =   0x000001BC,
};

class _JS_Fix CFileInfoExtract
{
public:
	CString GetFileInfoLastError();
	BOOL Read(__int64 llOffset, long cBytes, BYTE* pBuffer);
	CFileInfoExtract(void);

	JS_VideoInfo    GetVideoInfo();
	JS_AudioInfo    GetAudioInfo();
	JS_StreamInfo   GetStreamInfo();
	BOOL            OpenFile(LPCTSTR lpFileName);
	BOOL            CloseFile();
	BOOL            Analyze();

	virtual ~CFileInfoExtract(void);
//	void            error(const char *Fmt, ...);
	virtual BOOL    AnalyzeFile();
protected:
	HANDLE          m_hFile;
	bool            m_bAnalyzing;
	std::string     m_strFileName;
	std::string     m_strError;
	JS_StreamInfo   m_StreamInfo;
	JS_AudioInfo    m_AudioInfo;
	JS_VideoInfo    m_VideoInfo;
};
