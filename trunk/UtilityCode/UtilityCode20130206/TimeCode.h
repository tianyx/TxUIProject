// TimeCode.h: interface for the CTimeCode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIMECODE_H__9BF4D4F4_ED8A_11D3_9400_00104B04B842__INCLUDED_)
#define AFX_TIMECODE_H__9BF4D4F4_ED8A_11D3_9400_00104B04B842__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTimeCode  
{
public:

	CTimeCode();
	CTimeCode(const CTimeCode& srcTimeCode);
	
	CTimeCode(BYTE byteHour,
		      BYTE byteMinute,
			  BYTE byteSecond,
			  BYTE byteFrame);
	
	CTimeCode(DWORD dwTotalFrames);
	CTimeCode(CString& strTimeCode);
//	virtual ~CTimeCode();

	CTimeCode& operator = (const CTimeCode& tcOther);
	CTimeCode& operator = (const DWORD dwTotalFrames);
	CTimeCode& operator = (const CString& strTimeCode);

	
	BOOL operator == (CTimeCode& tcOther);
	BOOL operator > (CTimeCode& tcOther);
	BOOL operator < (CTimeCode& tcOther);

	CTimeCode operator + (CTimeCode& tcAdded);
	CTimeCode operator - (CTimeCode& tcSubtracted);

	void operator+=(CTimeCode& tcAdded);
	void operator-=(CTimeCode& tcSubtracted);

	void SetWithDetails(BYTE byteHour,
						BYTE byteMinute,
						BYTE byteSecond,
						BYTE byteFrame);

	void SetWithTotalFrames(DWORD dwTotalFrames);
	void SetWithString(CString strTimeCode);

	void GetDetails(BYTE& byteHour,
					BYTE& byteMinute,
					BYTE& byteSecond,
					BYTE& byteFrame);

	DWORD GetTotalFrames();
	CString ConvertToString();

public:

	char m_byteHour;
	char m_byteMinute;
	char m_byteSecond;
	char m_byteFrame;

};

#endif // !defined(AFX_TIMECODE_H__9BF4D4F4_ED8A_11D3_9400_00104B04B842__INCLUDED_)
