#pragma once

//WMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMW
//  Title		: Timecode
//  Author		: Guan Lei
//	CreateTime	: 2002 / 4 / 26
//	Copyright	: IDM, Founder
//WMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMWMW


namespace Coco
{

	
#pragma pack(push,4)

typedef struct DYNAMIC_TIME
{
	__int64	m_Time;
	__int64	m_Counter;

}DYNAMIC_TIME;

typedef __int64 counter_t;

#pragma pack(pop)



#pragma pack(push,4)


class timecode_t
{
	public:
		enum	{ FRAMES_PER_SEC = 25	};
		
		typedef timecode_t	_Myt;
		typedef const timecode_t _CMyt;


	public:
		int	m_nFrames;

	// 构造
	public:
		inline timecode_t()						{ m_nFrames = 0;			};
		inline timecode_t(_CMyt& tc)			{ m_nFrames = tc.m_nFrames;	}
		inline timecode_t(int nFrames)			{ m_nFrames = nFrames;		};

		inline timecode_t(int hh,int mm,int ss,int ff)		{ SetAsDetail(hh,mm,ss,ff); }
		inline timecode_t(const char* lpFrames)				{ SetAsFrames(lpFrames);	};
		inline timecode_t(const unsigned short* lpFrames)	{ SetAsFrames(lpFrames);	}
	
	// 比较
	public:
		inline bool operator ==(_CMyt& tc)const	{ return m_nFrames == tc.m_nFrames; }
		inline bool operator !=(_CMyt& tc)const	{ return m_nFrames != tc.m_nFrames; }
		
		inline bool operator >=(_CMyt& tc)const	{ return m_nFrames >= tc.m_nFrames; }
		inline bool operator <=(_CMyt& tc)const	{ return m_nFrames <= tc.m_nFrames; }

		inline bool operator > (_CMyt& tc)const	{ return m_nFrames >  tc.m_nFrames; }		
		inline bool operator < (_CMyt& tc)const	{ return m_nFrames <  tc.m_nFrames; }

	// 	
	public:
		inline int		GetAsFrames()const					{ return m_nFrames;}
		void __fastcall	SetAsFrames(const char* lpFrames);
		void __fastcall SetAsFrames(const unsigned short* lpFrames);

		inline int		GetAsMills()const			{ return (int) ( (__int64)m_nFrames*1000/FRAMES_PER_SEC ); }
		inline void		SetAsMills(int nMills)		{ m_nFrames = (int)( (__int64)nMills*FRAMES_PER_SEC/1000 ); }

		void __fastcall GetAsString(char* lpTimeCode)const;
		void __fastcall GetAsString(unsigned short* lpTimeCode)const;

		inline __int64  GetAsFileTimeSpan()const	{ return (__int64)m_nFrames*(10000000L/FRAMES_PER_SEC); }
		inline void SetAsFileTimeSpan(__int64 lTime){ SetAsMills((int)(lTime/10000L)); }


	// 
	public:
		void __fastcall GetAsDetail(int& hh, int& mm, int& ss, int& ff,int* Sign=0)const;
		void __fastcall GetAsDetail_BCD(int& hh, int& mm, int& ss, int& ff, int* Sign=0)const;
		void __fastcall GetAsDetail(char& hh, char& mm, char& ss, char& ff)const;
		void __fastcall GetAsDetail_BCD(char& hh, char& mm, char& ss, char& ff)const;

		void __fastcall SetAsDetail(int h,int m,int s,int f,int Sign=1);
		void __fastcall SetAsDetail_BCD(int h,int m,int s,int f,int Sign=1);

		int __fastcall GetFrame()const;
		int __fastcall GetSecond()const;
		int __fastcall GetMinute()const;
		int __fastcall GetHour()const;
		int __fastcall GetSign()const;
		int __fastcall GetMills()const;
		int __fastcall GetDay()const;

		inline void SetHour(int nh)		{ int h,m,s,f,sign; GetAsDetail(h,m,s,f,&sign); SetAsDetail(nh,m,s,f,sign);};
		inline void SetMinute(int nm)	{ int h,m,s,f,sign; GetAsDetail(h,m,s,f,&sign); SetAsDetail(h,nm,s,f,sign);};
		inline void SetSecond(int ns)	{ int h,m,s,f,sign; GetAsDetail(h,m,s,f,&sign); SetAsDetail(h,m,ns,f,sign);};
		inline void SetFrame(int nf)	{ int h,m,s,f,sign; GetAsDetail(h,m,s,f,&sign); SetAsDetail(h,m,s,nf,sign);};
		inline void SetSign(int nSign)	{ int h,m,s,f;		GetAsDetail(h,m,s,f);		SetAsDetail(h,m,s,f,nSign);};
		inline void SetMills(int nMills){ SetFrame(nMills * FRAMES_PER_SEC / 1000);}
		inline void SetDay(int nDay)	{ 
					int h,m,s,f,sign; 
					GetAsDetail(h,m,s,f,&sign); 
					h=nDay*24 + (h % 24);
					SetAsDetail(h,m,s,f,sign);
				};
		
		__declspec(property(get=GetHour,	put=SetHour))	int Hour;
		__declspec(property(get=GetMinute,	put=SetMinute))	int Minute;
		__declspec(property(get=GetSecond,	put=SetSecond))	int Second;
		__declspec(property(get=GetFrame,	put=SetFrame))	int Frame;
		__declspec(property(get=GetSign,	put=SetSign))	int Sign;
		__declspec(property(get=GetMills,	put=SetMills))	int Mills;
		__declspec(property(get=GetDay,		put=SetDay))	int Day;


	//=
	public:
		inline void operator=(_CMyt& tc)	{ m_nFrames = tc.m_nFrames; }
		inline void operator=(int nFrames)		{ m_nFrames = nFrames; }

		inline void operator=(const char* lpTC)				{ SetAsFrames(lpTC);}
		inline void operator=(const unsigned short* lpTC)	{ SetAsFrames(lpTC);}
	// +-
	public:
		inline void operator+=(_CMyt& tc)		{ m_nFrames += tc.m_nFrames; }
		inline void operator-=(_CMyt& tc)		{ m_nFrames -= tc.m_nFrames; }
		
		inline _Myt operator+(_CMyt& tc)const	{ return m_nFrames + tc.m_nFrames;}
		inline _Myt operator-(_CMyt& tc)const	{ return m_nFrames - tc.m_nFrames;}
		
		inline void operator--()				{ m_nFrames--; }
		inline void operator++()				{ m_nFrames++; }

		
	public:
		const CString __fastcall				ToString(char* pBuf=0,int nOp=0)const;
		const unsigned short* __fastcall	ToStringW(unsigned short* pBuf=0,int nOp=0)const;
};


#pragma pack(pop)


}
