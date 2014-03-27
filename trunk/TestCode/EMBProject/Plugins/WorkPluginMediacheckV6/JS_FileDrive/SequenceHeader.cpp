#include "stdafx.h"
#include "SequenceHeader.h"

// --- video sequence header ------------------------
#define UNITS 10000000

SequenceHeader::SequenceHeader()
: m_pHeader(NULL),
m_cBytes(0)
{
	m_lFrameRateIndex = 0;
}

SequenceHeader::~SequenceHeader()
{
	delete[] m_pHeader;
	m_pHeader = NULL;
}

bool 
SequenceHeader::Parse(const BYTE* pData, long cBytes)
{
	// calculate length and check that it is all present
	// in this buffer
	m_cBytes = CheckLength(pData,cBytes);
	if (!m_cBytes)
	{
		return false;
	}

	// validate and extract info

	// check marker bit
	if (!(pData[10] & 0x20)) {
		return false;
	}

	// 12-bits each of width and height
	m_width = (pData[4] << 4) + ((pData[5] & 0xF0) >> 4);
	m_height = ((pData[5] & 0x0F) << 8) + pData[6];

	m_lFrameRateIndex = pData[7] & 0x0F; //picture_rate
	if (m_lFrameRateIndex == 0)
	{
		return false;
	}
	// check for scrambling bit
	if (m_lFrameRateIndex > 8)
	{
		m_lFrameRateIndex &= 0x7;
	}
	m_tFrame = PictureTimes[m_lFrameRateIndex];

	m_idxAspect = (pData[7] & 0xF0) >> 4;
	if (m_idxAspect == 0)
	{
		return false;
	}
	m_XPelsPerMeter = 2000;
	m_YPelsPerMeter = MulDiv(m_XPelsPerMeter, AspectRatios[m_idxAspect], 10000);

	m_Bitrate = (pData[8] << 10) + (pData[9] << 2) + (pData[10] >> 6);
	if (m_Bitrate == 0x3FFFF)
	{
		m_Bitrate = 0;  // variable
	} else {
		m_Bitrate *= 400;
	}
	m_vbvBufferSize =(((pData[10] & 0x1F) << 5) + (pData[11] >> 3));

	// store raw data
	if (m_pHeader)
	{
		delete[] m_pHeader;
		m_pHeader = NULL;
	}
	m_pHeader = new BYTE[m_cBytes];
	CopyMemory(m_pHeader, pData, m_cBytes);
	return true;
}

long 
SequenceHeader::CheckLength(const BYTE* pHeader, long cBytes)
{
	// the length depends on which of the intra or inter
	// matrices are present.
	// To simplify this sample, we only accept a sequence
	// header that is entirely within a PES packet.
	long cHdr = 0;
	if (cBytes >= 12)
	{
		if ((pHeader[11] & 3) == 0)
		{
			// neither present
			cHdr = 12;
		} else if ((pHeader[11] & 3) == 1)
		{
			// intra present
			cHdr = 12 + 64;
		} else if (cBytes >= (12 + 64)) 
		{
			if ((pHeader[11 + 64] & 0x1) == 1)
			{
				// both
				cHdr = 12 + 64 + 64;
			} else{
				// just inter
				cHdr = 12 + 64;
			}
		}
	}
	if (cHdr > cBytes)
	{
		return 0;
	} else {
		return cHdr;
	}
}
//map index into a picture rate
const double 
SequenceHeader::FrameRates[]={0,23.976,24,25,29.97,30,50,59.94,60};
// map index into a frame duration in REFERENCE_TIME units
// static
const LONG 
SequenceHeader::PictureTimes[] = { 0,
(LONG)((double)UNITS / 23.976),
(LONG)((double)UNITS / 24),
(LONG)((double)UNITS / 25),
(LONG)((double)UNITS / 29.97),
(LONG)((double)UNITS / 30),
(LONG)((double)UNITS / 50),
(LONG)((double)UNITS / 59.94),
(LONG)((double)UNITS / 60)
};

// convert aspect ratio index into a pels-per-meter value
// static
const LONG 
SequenceHeader::AspectRatios[] = { 0,
393700,
(LONG)(393700.0 * 0.6735),
(LONG)(393700.0 * 0.7031),
(LONG)(393700.0 * 0.7615),
(LONG)(393700.0 * 0.8055),
(LONG)(393700.0 * 0.8437),
(LONG)(393700.0 * 0.8935),
(LONG)(393700.0 * 0.9375),
(LONG)(393700.0 * 0.9815),
(LONG)(393700.0 * 1.0255),
(LONG)(393700.0 * 1.0695),
(LONG)(393700.0 * 1.1250),
(LONG)(393700.0 * 1.1575),
(LONG)(393700.0 * 1.2015),
0
};

double  SequenceHeader::FrameRate()
{ 
	return FrameRates[m_lFrameRateIndex];
}