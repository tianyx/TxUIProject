#pragma once

#include <vector>
#include "File64.h"
extern HWND g_hNotifier;

#define MSG_LOGNOTIFY WM_USER+0617
#define LINE_BUFFER_SIZE 1024 

#define AFDRATIO43 0x0
#define AFDRATIO169 0x4

//modified by tianyx  4:7:2013   14:30
//modify afd byte according to the rule of k2 and mediainfo.
//end modified by tianyx
//#define MAKEAFDBYTEFROMINT(x,ratio)  ((x/1000)<<6 + ((x%1000)/100)<<5 +(x%100/10)<<4 +(x%10)<<3 + ratio)
#define MAKEAFDBYTEFROMINT(x,ratio)  ((x/1000)<<3 + ((x%1000)/100)<<2 +(x%100/10)<<1 +(x%10))

//#define MAKEAFDBYTEFROMSTRING(x, ratio)  (((x[0]-'0')<<6) + ((x[1]-'0')<<5) +((x[2]-'0')<<4) +((x[3]-'0')<<3) + ratio)
#define MAKEAFDBYTEFROMSTRING(x, ratio)  (((x[0]-'0')<<3) + ((x[1]-'0')<<2) +((x[2]-'0')<<1) +((x[3]-'0')))

//#define MAKESTRINGFROMAFDBYTE(szOut, bt) szOut[0] = (bt>>6)%2+'0';szOut[1]= (bt>>5)%2 +'0';szOut[2]= (bt>>4)%2 +'0'; szOut[3]= (bt>>3)%2 +'0'
#define MAKESTRINGFROMAFDBYTE(szOut, bt) szOut[0] = (bt>>3)%2+'0';szOut[1]= (bt>>2)%2 +'0';szOut[2]= (bt>>1)%2 +'0'; szOut[3]= bt%2 +'0'
enum enum_AWERR
{
	AWERR_SUCCESS = 0,
	AWERR_NOTFINDAFD = 10,
	AWERR_FILEOPEN = -100,
	AWERR_FILEINVALID,
	AWERR_BODYKEYREAD,
	AWERR_NOFILEMETA,
	AWERR_FILECOPY,
	AWERR_FILEWRITE
	
};

struct STHeadPackPos 
{
	INT64 nBegin;
	INT64 nEnd;
	STHeadPackPos()
	{
		nBegin = -1;
		nEnd = 0;
	}
};

class CAFDWriter
{
public:
	CAFDWriter(void);
	~CAFDWriter(void);

	int GetAFDInfo(CString& strFileIn,  byte& bAFDOut, INT64& nSeekPosOut);
	int WriteAFD(CString& strSrcFileIn, CString& strDesFileIn, byte bAFDIn, const INT64 nSrcAFDPosIn);

protected:
	int GetAFDInfo( byte& bAFDOut, INT64& nSeePosOut);
	CFile64* m_pFile;
	INT64 m_nFileLen;
	INT64 ConvertBERLen(const int nDataLen);
	int OpenMXF(CString& strSrcFile);
	void CloseMXF();

};

void WriteLog( LPCTSTR format,...);
