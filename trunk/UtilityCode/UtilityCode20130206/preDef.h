/********************************************************************
	created:	2012/11/21
	created:	2012:11
	filename: 	preDef.h
	file base:	preDef
	file ext:	h
	author:		tian_yx
	purpose:	
*********************************************************************/
#pragma  once

#include <vector>
#include <map>
#include <list>

typedef ULONG64 ULONG64_PTR, *PULONG64_PTR;
#define MAKELONG64KEY(a, b)      ((ULONG64)(((ULONG)((ULONG_PTR)(a<b? a:b) & 0xffffffff)) | ((ULONG64)((ULONG)((ULONG_PTR)(a<b? b:a) & 0xffffffff))) << 32))
#define MAKELONG64(a, b)      ((ULONG64)(((ULONG)((ULONG_PTR)(a) & 0xffffffff)) | ((ULONG64)((ULONG)((ULONG_PTR)(b) & 0xffffffff))) << 32))
#define LOWLONG64(l)           ((ULONG64)((ULONG64_PTR)(l) & 0xffffffff))
#define HILONG64(l)           ((ULONG64)((ULONG64_PTR)(l) >> 32))

#define	TxMACRO_WriteLog g_log.Print

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

//template col header
#define TEMPLATE_DB_COL_NUM 9

enum ENUM_TEMPCOL_TYPE
{
	TRCOLTYPE_INDEX = 0,
	TRCOLTYPE_PGMNAME,
	TRCOLTYPE_PGMID,
	TRCOLTYPE_CHANNELID,
	TRCOLTYPE_TOTALLENTH,
	TRCOLTYPE_FIRSTIME,		//string above
	TRCOLTYPE_TYPE,
	TRCOLTYPE_STATE,
	TRCOLTYPE_UNKNOW = -1
};

enum ENUM_MONTYPE
{
	MONTYPE_INDEX = 0,
	MONTYPE_TIMEREMAIN,
	MONTYPE_STATE,
	MONTYPE_CLIPNAME,
	MONTYPE_PGMCODE,
	MONTYPE_CHNAME,
	MONTYPE_TOTALLENGTH,
	MONTYPE_FIRSTTIME

};


enum ENUM_TRANSSTATE
{
	//type in db
	TRANSSTATE_READY = 0,
	TRANSSTATE_GOING,
	TRANSSTATE_SUCCESS,
	TRANSSTATE_CHECKSUM,
	TRANSSTATE_ERROR,
	TRANSSTATE_FTPERROR,
	TRANSSTATE_CHECKSUMERROR,
	TRANSSTATE_DBREGERROR,
	TRANSSTATE_DEVICEERR,

	//custom type
	TRANSSTATE_TRANSNONETIMESAFE,
	TRANSSTATE_TRANSNONETIMEUNSAFE,
	TRANSSTATE_TRANSNONETIMEOUT,
	TRANSSTATE_TRANSDOORCLOSED,
	TRANSSTATE_UNKNOW
};


struct ST_TMP_COLINFO
{
	int nColPos;	//position in list 
	ENUM_TEMPCOL_TYPE nPosInType; //position in ENUMMONTYPE
	int nColWidth;
	CString strTitle;
	int nEditMode;  //{ Mxt_ReadOnly= 0, Mxt_Text, Mxt_TimeCode, Mxt_Combo };
	int nMinWidth;
	BOOL bWidthFixed;
	ST_TMP_COLINFO()
	{
		nPosInType = TRCOLTYPE_UNKNOW;
		nColWidth = 0;
		nColPos = -1;
		nEditMode = 0;
		bWidthFixed = FALSE;
		nMinWidth = 100;
	}
};
typedef std::vector<ST_TMP_COLINFO> VECTMPCOLUMN;
typedef std::vector<CString> VECSTRING;
typedef std::vector<CString> VECSHOWNSTRING;
typedef std::vector<int> VECINT;

class CTrMonItem;
typedef std::vector<CTrMonItem> VECTMPITEM;
typedef std::list<CTrMonItem> LSTTMPITEM;
typedef std::map<int, CTrMonItem> MAPTMPLSTITEM;



#define  TXFONTFOR_ALL	   0
#define  TXFONTFOR_CAPTION 1
#define  TXFONTFOR_LIST	   2
#define  TXFONTFOR_TIME	   3


#define ALERTTYPE_FLASH_SAFE 0
#define ALERTTYPE_FLASH_UNSAFE 1
#define ALERTTYPE_FLASH_ERROR 2


