// TimeCode.cpp: implementation of the CTimeCode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TimeCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTimeCode::CTimeCode()
{

}

CTimeCode::CTimeCode(const CTimeCode& srcTimeCode)
{
	m_byteHour   = srcTimeCode.m_byteHour;
	m_byteMinute = srcTimeCode.m_byteMinute;
	m_byteSecond = srcTimeCode.m_byteSecond;
	m_byteFrame  = srcTimeCode.m_byteFrame;
}

CTimeCode::CTimeCode(BYTE byteHour, BYTE byteMinute, BYTE byteSecond, BYTE byteFrame)
{
	m_byteHour   = byteHour;
	m_byteMinute = byteMinute;
	m_byteSecond = byteSecond;
	m_byteFrame  = byteFrame;
}

CTimeCode::CTimeCode(DWORD dwTotalFrames)
{
	DWORD dwSecond = dwTotalFrames/25;
	m_byteFrame	   = (BYTE)(dwTotalFrames - dwSecond*25);
	m_byteHour	   = (BYTE)(dwSecond/3600);
	dwSecond	   = dwSecond%3600;
	m_byteMinute   = (BYTE)(dwSecond/60);
	m_byteSecond   = (BYTE)(dwSecond%60);
}

CTimeCode::CTimeCode(CString& strTimeCode)
{
	m_byteHour   = (strTimeCode[0] - '0') * 10 + (strTimeCode[1] - '0');
	m_byteMinute = (strTimeCode[3] - '0') * 10 + (strTimeCode[4] - '0');
	m_byteSecond = (strTimeCode[6] - '0') * 10 + (strTimeCode[7] - '0');
	m_byteFrame  = (strTimeCode[9] - '0') * 10 + (strTimeCode[10] - '0');
}
/*
CTimeCode::~CTimeCode()
{

}
*/
CTimeCode& CTimeCode::operator =(const CTimeCode& tcOther)
{
	m_byteHour   = tcOther.m_byteHour;
	m_byteMinute = tcOther.m_byteMinute;
	m_byteSecond = tcOther.m_byteSecond;
	m_byteFrame  = tcOther.m_byteFrame;
	return *this;
}

CTimeCode& CTimeCode::operator = (const DWORD dwTotalFrames)
{
	DWORD dwSecond = dwTotalFrames/25;
	m_byteFrame	   = (BYTE)(dwTotalFrames - dwSecond*25);
	m_byteHour	   = (BYTE)(dwSecond/3600);
	dwSecond	   = dwSecond%3600;
	m_byteMinute   = (BYTE)(dwSecond/60);
	m_byteSecond   = (BYTE)(dwSecond%60);
	return *this;
}

CTimeCode& CTimeCode::operator = (const CString& strTimeCode)
{
	if(strTimeCode.GetLength() > 0)
	{
		m_byteHour   = (strTimeCode[0] - '0') * 10 + (strTimeCode[1] - '0');
		m_byteMinute = (strTimeCode[3] - '0') * 10 + (strTimeCode[4] - '0');
		m_byteSecond = (strTimeCode[6] - '0') * 10 + (strTimeCode[7] - '0');
		m_byteFrame  = (strTimeCode[9] - '0') * 10 + (strTimeCode[10] - '0');
	}
	return *this;
}

BOOL CTimeCode::operator == (CTimeCode& tcOther)
{
	return ((m_byteHour == tcOther.m_byteHour) && (m_byteMinute == tcOther.m_byteMinute)
		&& (m_byteSecond == tcOther.m_byteSecond) && (m_byteFrame == tcOther.m_byteFrame));
}

BOOL CTimeCode::operator > (CTimeCode& tcOther)
{
	BOOL bRet = FALSE;

	CTimeCode tcOp1 = tcOther;
	if ((tcOp1 - *this).m_byteHour < 0)
		bRet = TRUE;

	return bRet;

/*	if(m_byteHour > tcOther.m_byteHour)
		return TRUE;
	
	else if(m_byteMinute > tcOther.m_byteMinute)
		return TRUE;

	else if(m_byteSecond > tcOther.m_byteSecond)
		return TRUE;

	else if(m_byteFrame > tcOther.m_byteFrame)
		return TRUE;

	else return FALSE;
*/
}

BOOL CTimeCode::operator < (CTimeCode& tcOther)
{
	BOOL bRet = FALSE;

	CTimeCode tcOp1 = *this;
	if ((tcOp1 - tcOther).m_byteHour < 0)
		bRet = TRUE;

	return bRet;
}

CTimeCode CTimeCode::operator-(CTimeCode& tcSubtracted)
{
	CTimeCode tcOp1(*this);

	tcOp1.m_byteFrame -= tcSubtracted.m_byteFrame;
	if(tcOp1.m_byteFrame < 0)
	{
		tcOp1.m_byteFrame += 25;
		tcOp1.m_byteSecond -= 1;
	}
	
	tcOp1.m_byteSecond -= tcSubtracted.m_byteSecond;
	if(tcOp1.m_byteSecond < 0)
	{
		tcOp1.m_byteSecond += 60;
		tcOp1.m_byteMinute -= 1;
	}
	
	tcOp1.m_byteMinute -= tcSubtracted.m_byteMinute;
	if(tcOp1.m_byteMinute < 0)
	{
		tcOp1.m_byteMinute += 60;
		tcOp1.m_byteHour -= 1;
	}

	tcOp1.m_byteHour -= tcSubtracted.m_byteHour;

	return tcOp1;
}

CTimeCode CTimeCode::operator+(CTimeCode& tcAdded)
{
	CTimeCode tcOp1(*this);
	tcOp1.m_byteFrame += tcAdded.m_byteFrame;
	if(tcOp1.m_byteFrame >= 25)
	{
		tcOp1.m_byteFrame -= 25;
		tcOp1.m_byteSecond += 1;
	}
	
	tcOp1.m_byteSecond += tcAdded.m_byteSecond;
	if(tcOp1.m_byteSecond >= 60)
	{
		tcOp1.m_byteSecond -= 60;
		tcOp1.m_byteMinute += 1;
	}
	
	tcOp1.m_byteMinute += tcAdded.m_byteMinute;
	if(tcOp1.m_byteMinute >= 60)
	{
		tcOp1.m_byteMinute -= 60;
		tcOp1.m_byteHour += 1;
	}
	
	tcOp1.m_byteHour += tcAdded.m_byteHour;

	return tcOp1;
}

void CTimeCode::operator += (CTimeCode& tcAdded)
{
	m_byteFrame += tcAdded.m_byteFrame;
	if(m_byteFrame >= 25)
	{
		m_byteFrame -= 25;
		m_byteSecond += 1;
	}
	
	m_byteSecond += tcAdded.m_byteSecond;
	if(m_byteSecond >= 60)
	{
		m_byteSecond -= 60;
		m_byteMinute += 1;
	}
	
	m_byteMinute += tcAdded.m_byteMinute;
	if(m_byteMinute >= 60)
	{
		m_byteMinute -= 60;
		m_byteHour += 1;
	}
	
	m_byteHour += tcAdded.m_byteHour;
}

void CTimeCode::operator -= (CTimeCode& tcSubtracted)
{
	m_byteFrame -= tcSubtracted.m_byteFrame;
	if(m_byteFrame < 0)
	{
		m_byteFrame += 25;
		m_byteSecond -= 1;
	}
	
	m_byteSecond -= tcSubtracted.m_byteSecond;
	if(m_byteSecond < 0)
	{
		m_byteSecond += 60;
		m_byteMinute -= 1;
	}
	
	m_byteMinute -= tcSubtracted.m_byteMinute;
	if(m_byteMinute < 0)
	{
		m_byteMinute += 60;
		m_byteHour -= 1;
	}

	m_byteHour -= tcSubtracted.m_byteHour;
}

void CTimeCode::SetWithDetails(BYTE byteHour,
					           BYTE byteMinute,
						       BYTE byteSecond,
						       BYTE byteFrame)
{
	m_byteHour   = byteHour;
	m_byteMinute = byteMinute;
	m_byteSecond = byteSecond;
	m_byteFrame  = byteFrame;
}

void CTimeCode::SetWithTotalFrames(DWORD dwTotalFrames)
{
	DWORD dwSecond = dwTotalFrames/25;
	m_byteFrame	   = (BYTE)(dwTotalFrames - dwSecond*25);
	m_byteHour	   = (BYTE)(dwSecond/3600);
	dwSecond	   = dwSecond%3600;
	m_byteMinute   = (BYTE)(dwSecond/60);
	m_byteSecond   = (BYTE)(dwSecond%60);
}

void CTimeCode::SetWithString(CString strTimeCode)
{
	m_byteHour   = (strTimeCode[0] - '0') * 10 + (strTimeCode[1] - '0');
	m_byteMinute = (strTimeCode[3] - '0') * 10 + (strTimeCode[4] - '0');
	m_byteSecond = (strTimeCode[6] - '0') * 10 + (strTimeCode[7] - '0');
	m_byteFrame  = (strTimeCode[9] - '0') * 10 + (strTimeCode[10] - '0');
}

void CTimeCode::GetDetails(BYTE& byteHour, BYTE& byteMinute, BYTE& byteSecond, BYTE& byteFrame)
{
	byteHour = m_byteHour;
	byteMinute = m_byteMinute;
	byteSecond = m_byteSecond;
	byteFrame = m_byteFrame;
}

DWORD CTimeCode::GetTotalFrames()
{
	DWORD dwTotalFrames;
	dwTotalFrames  = (DWORD)(((m_byteHour*60+m_byteMinute)*60+m_byteSecond)*25)+m_byteFrame;
	return dwTotalFrames;
}

CString CTimeCode::ConvertToString()
{
	CString strTimeCode; 
	strTimeCode.Format("%02d:%02d:%02d:%02d", m_byteHour,m_byteMinute,m_byteSecond,m_byteFrame);
	return strTimeCode;
}

