
#include "stdafx.h"
#include "CcType_Timecode.h"
#include <math.h>

namespace Coco
{


inline int _HEX2BCD(char bcd) { return ( (bcd / 10) << 4)  + bcd % 10; }
inline int _BCD2HEX(char hex) { return (hex >> 4) * 10 + (hex & 0xF) ; }

void __fastcall timecode_t::GetAsString(char* lpTimeCode)const
	{
		int hh,mm,ss,ff;
		GetAsDetail(hh,mm,ss,ff);
		::sprintf(lpTimeCode,"%02d:%02d:%02d:%02d", hh,mm,ss,ff);
		if (Sign < 0)
		{
			lpTimeCode[0] = '-';
		}
	};


void __fastcall timecode_t::GetAsString(unsigned short* lpTimeCode)const
	{
		int hh,mm,ss,ff;
		GetAsDetail(hh,mm,ss,ff);
		::swprintf((wchar_t*)lpTimeCode,L"%02d:%02d:%02d:%02d", hh,mm,ss,ff);
		if (Sign < 0)
		{
			lpTimeCode[0] = wchar_t('-');
		}

	};

extern "C" char* __stdcall glGetThreadSafeBuffer(int nSize);

const CString __fastcall timecode_t::ToString(char* pBuf,int nOP)const
	{
		char buff[32];
		if (pBuf==0)
		{
			pBuf = buff;
		}

		GetAsString(pBuf);

		return pBuf;
	}

const unsigned short* __fastcall timecode_t::ToStringW(unsigned short* pBuf,int nOP)const
	{
		wchar_t buff[32];
		if (pBuf==0)
		{
			pBuf = (unsigned short*)buff;
		}
		GetAsString(pBuf);
		
		return pBuf;
	}

void __fastcall timecode_t::SetAsFrames(const char* lpTimeCode)
	{
		if (0== lpTimeCode)
		{
			return;
		}

		int hh=0,mm=0,ss=0,ff=0;
	
		char Buf[32]; 
		
		::strncpy(Buf,lpTimeCode,sizeof(Buf));
		Buf[sizeof(Buf)-1]=0;

		CString strBuff(Buf);
		strBuff.Replace('\t', ' ');
		strBuff.Replace('\\', ' ');	
		strBuff.Replace(':',  ' ');
		strBuff.Replace(',',  ' ');
		strBuff.Replace('.',  ' ');
		//aString::Replace(Buf,'-',  ' ');
		strBuff.Replace('/',  ' ');
	
		::sscanf(strBuff,"%d %d %d %d",&hh,&mm,&ss,&ff);

		m_nFrames = hh * FRAMES_PER_SEC * 3600L + mm * FRAMES_PER_SEC * 60L + ss * FRAMES_PER_SEC + ff;
		if (Buf[0] == '-')
			m_nFrames *= -1;

	};
/*

void __fastcall timecode_t::SetAsFrames(const unsigned short* lpTimeCode)
	{
		if (0== lpTimeCode)
		{
			return;
		}
		
		int hh=0,mm=0,ss=0,ff=0;
		
		wchar_t Buf[32]; 
		
		wString::strncpy(Buf,(wchar_t*)lpTimeCode,32);

		wString::Pack(Buf);
		wString::Replace(Buf,L'\t', L' ');
		wString::Replace(Buf,L'\\', L' ');	
		wString::Replace(Buf,L':',  L' ');
		wString::Replace(Buf,L',',  L' ');
		wString::Replace(Buf,L'.',  L' ');
		//wString::Replace(Buf,L'-',  L' ');
		wString::Replace(Buf,L'/',  L' ');
		
		::swscanf(Buf,L"%d %d %d %d",&hh,&mm,&ss,&ff);
		
		m_nFrames = hh * FRAMES_PER_SEC * 3600L + mm * FRAMES_PER_SEC * 60L + ss * FRAMES_PER_SEC + ff;
		if (Buf[0] == L'-')
			m_nFrames *= -1;
		
	};*/

void __fastcall timecode_t::GetAsDetail(int& hh, int& mm, int& ss, int& ff, int* Sign)const
	{
		if (Sign)
		{
			*Sign = (m_nFrames>=0) ? 1 : -1;
		}

		int  F = ::abs(m_nFrames);

		ff = (F % FRAMES_PER_SEC);	F /=FRAMES_PER_SEC;
		ss = (F % 60);	F /=60;
		mm = (F % 60);	F /=60;
		hh = F;
	};


void __fastcall timecode_t::GetAsDetail_BCD(int& hh, int& mm, int& ss, int& ff,int* Sign)const
	{
		GetAsDetail(hh,mm,ss,ff,Sign);
		hh = _HEX2BCD((char)hh);
		mm = _HEX2BCD((char)mm);
		ss = _HEX2BCD((char)ss);
		ff = _HEX2BCD((char)ff);
	};


void __fastcall timecode_t::GetAsDetail(char& hh, char& mm, char& ss, char& ff)const
	{
		int h,m,s,f;
		GetAsDetail(h,m,s,f);
		hh = (char)h; mm=(char)m; ss=(char)s; ff=(char)f;
	};

void __fastcall timecode_t::GetAsDetail_BCD(char& hh, char& mm, char& ss, char& ff)const
	{
		int h,m,s,f;
		GetAsDetail_BCD(h,m,s,f);
		hh = (char)h; mm=(char)m; ss=(char)s; ff=(char)f;
	};

int __fastcall timecode_t::GetHour()const
	{
		int hh,mm,ss,ff;
		GetAsDetail(hh,mm,ss,ff);
		return hh;
	};

int __fastcall timecode_t::GetFrame()const
	{
		int hh,mm,ss,ff;
		GetAsDetail(hh,mm,ss,ff);
		return ff;
	};

int __fastcall timecode_t::GetMinute()const
	{
		int hh,mm,ss,ff;
		GetAsDetail(hh,mm,ss,ff);
		return mm;
	};

int __fastcall timecode_t::GetSecond()const
	{
		int hh,mm,ss,ff;
		GetAsDetail(hh,mm,ss,ff);
		return ss;
	};

int __fastcall timecode_t::GetSign()const
	{
		int hh,mm,ss,ff,Sign;
		GetAsDetail(hh,mm,ss,ff,&Sign);
		return Sign;
	};

int __fastcall timecode_t::GetMills()const
	{
		return GetFrame() * (1000 / FRAMES_PER_SEC);
	};

int __fastcall timecode_t::GetDay()const
	{
		return GetHour() / 24;
	}

void __fastcall timecode_t::SetAsDetail(int h,int m,int s,int f, int Sign)
	{

		enum	{ _HMIN = -10000000		};
		enum	{ _HMAX =  10000000		};
	
		if (h < _HMIN) h = _HMIN;
		if (h > _HMAX) h = _HMAX;

		Sign = (Sign != -1) ? 1 : -1;


		h = abs(h);
		m = abs(m) % 60;	
		s = abs(s) % 60 ;
		f = abs(f) % FRAMES_PER_SEC;
		
		m_nFrames = Sign* ( FRAMES_PER_SEC*h*3600L + FRAMES_PER_SEC*m*60L + FRAMES_PER_SEC*s + f );
		
	}	

void __fastcall timecode_t::SetAsDetail_BCD(int h,int m,int s,int f,int Sign)
	{
		SetAsDetail(_BCD2HEX((char)h),_BCD2HEX((char)m),_BCD2HEX((char)s),_BCD2HEX((char)f),Sign);
	};


}
