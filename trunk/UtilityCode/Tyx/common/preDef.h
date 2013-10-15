#pragma  once

#include <vector>
#include <map>

typedef ULONG64 ULONG64_PTR, *PULONG64_PTR;
#define MAKELONG64KEY(a, b)      ((ULONG64)(((ULONG)((ULONG_PTR)(a<b? a:b) & 0xffffffff)) | ((ULONG64)((ULONG)((ULONG_PTR)(a<b? b:a) & 0xffffffff))) << 32))
#define MAKELONG64(a, b)      ((ULONG64)(((ULONG)((ULONG_PTR)(a) & 0xffffffff)) | ((ULONG64)((ULONG)((ULONG_PTR)(b) & 0xffffffff))) << 32))
#define LOWLONG64(l)           ((ULONG64)((ULONG64_PTR)(l) & 0xffffffff))
#define HILONG64(l)           ((ULONG64)((ULONG64_PTR)(l) >> 32))

#define	TxMACRO_WriteLog glReportDbg

#define TXMARCO_ENSURE(x) if (!x){ASSERT(FALSE); return;}
#define TXMARCO_ENSURE2(x) if (!x){ASSERT(FALSE); return FALSE;} 


struct ST_DBCHANNEL
{
	CString strChID;
	CString strChName;
};

typedef std::map<CString, CString> MAPCHANNELS;

enum ENUM_TXBMPSTRETCHTYPE
{
	TXBMP_STRETCH_NONE	= 0,
	TXBMP_STRETCH_LR	= 1,
	TXBMP_STRETCH_TB	= 2,
	TXBMP_STRETCH_MID_LR =3,
	TXBMP_STRETCH_MID_TB =4,
	TXBMP_STRETCH_MID_LRTB	= 5
};

