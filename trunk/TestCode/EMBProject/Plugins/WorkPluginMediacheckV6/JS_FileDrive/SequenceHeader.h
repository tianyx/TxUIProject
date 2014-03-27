#pragma once
#include "JS_FileDrive.h"
// video elementary stream sequence header --
// used to create video media type
class _JS_Fix SequenceHeader
{
public:
	SequenceHeader();
	~SequenceHeader();
	bool   Parse(const BYTE* pData, long cBytes);

	long   Length()        { return m_cBytes;	}
	long   Width()         { return m_width; }
	long   Height()        { return m_height; }
	__int64 FrameTime()    { return m_tFrame; }
	double FrameRate();
	long   XPels()         { return m_XPelsPerMeter; }
	long   YPels()         { return m_YPelsPerMeter; }
	long   Bitrate()       { return m_Bitrate; }
	long   vbvBufferSize() { return m_vbvBufferSize;}
	
    std::string GetAspectRatio()  
	{
		std::string strAR;
		switch (m_idxAspect)
		{
		   case 0x0001:
			   strAR = "1:1";
			   break;
	       case 0x0002:
			   strAR = "4:3";
			   break;
		   case 0x0003:
			   strAR = "16:9";
			   break;
		   default:
			   strAR = "";
			   break;
		}
		return strAR;
	}
	const BYTE* Header()       { return m_pHeader; }

private:
	long CheckLength(const BYTE* pHeader, long cBytes);

private:
	// raw header
	BYTE*   m_pHeader;
	long    m_cBytes;

	// parsed info
	long    m_width;
	long    m_height;
	long    m_XPelsPerMeter;
	long    m_YPelsPerMeter;
	long    m_idxAspect;
	__int64 m_tFrame;
	long    m_Bitrate;
	long    m_vbvBufferSize;
	long    m_lFrameRateIndex;
private:
	// static conversion tables
	static const double FrameRates[];
	static const LONG PictureTimes[];
	static const LONG AspectRatios[];
};