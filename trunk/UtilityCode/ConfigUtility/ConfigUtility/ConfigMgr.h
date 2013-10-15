#pragma once
#include <vector>
#include <map>
using namespace std;
using namespace std::tr1;

//the layout of cfg file
typedef wstring SROOT;
typedef wstring SSUB;
typedef wstring SSUB2;
typedef map<SSUB2, wstring> MAPSSUB2;
struct ST_MAPSSUB2
{
	MAPSSUB2 m_data;
	operator MAPSSUB2& ()
	{
		return m_data;
	}
};
typedef map<SSUB, ST_MAPSSUB2> MAPSSUB;
struct ST_MAPSSUB
{
	MAPSSUB m_data;
	operator MAPSSUB&()
	{
		return m_data;
	}
};
typedef map<SROOT, ST_MAPSSUB> MAPSROOT;

//the layout of cfg's content 
typedef wstring CFGSECTION;
typedef wstring CFGKEY;
typedef wstring CFGVALUE;
typedef vector<CFGVALUE> VECCFGVALUE;

enum ENUMVALUETYPE
{
	CFGVALTYPE_NONE = 0,
	CFGVALTYPE_STR,
	CFGVALTYPE_NUMBER,
	CFGVALTYPE_BOOL,
	CFGVALTYPE_VEC,
	CFGVALTYPE_MAP
};

enum ENUMCOMAPRETYPE
{
	COMPTYPE_NONE = 0,
	COMPTYPE_TYPEDIFF,
	COMPTYPE_EQUAL,
	COMPTYPE_LOSS,
	COMPTYPE_DIFF,
	COMPTYPE_EXTRA,
};

struct ST_CFGVALUE;
typedef map<CFGKEY, ST_CFGVALUE> MAPCFGKEY;

struct ST_CFGVALUE 
{
	ENUMVALUETYPE nType;
 	CFGKEY		wskey;
 	CFGVALUE	wsOrgData;
 
//  	shared_ptr<CFGVALUE> pwsVal;
//  	shared_ptr<MAPCFGKEY> mapVal;
//  	shared_ptr<VECCFGVALUE> vecVal;
	CFGVALUE wsVal;
	MAPCFGKEY mapVal;
	VECCFGVALUE vecVal;

	//for compare 
	ENUMCOMAPRETYPE nCompType;

	ST_CFGVALUE()
	{
		nCompType = COMPTYPE_NONE;
		nType = CFGVALTYPE_NONE;
	}
		
};

typedef map<CFGSECTION, MAPCFGKEY> MAPCFGSECTION;

//
typedef void (*FINDCFGFILECALLBACK)(int nLevel, LPARAM lparam);

class CConfigMgr
{
public:
	CConfigMgr(void);
	~CConfigMgr(void);

	BOOL ResetData();
	BOOL ReloadCfg();
	BOOL GetCfgContent(wstring& swFile, MAPCFGSECTION& dataOut);
	int Compare(wstring& swFileLeft, wstring& swFileRight);
	void ClearResult();


private:
	//compare and fill compare type in left, and add loss key in left.
	ENUMCOMAPRETYPE CompareLeft(MAPCFGSECTION& mapValLeft, MAPCFGSECTION& mapValRight);
	ENUMCOMAPRETYPE CompareLeft(MAPCFGKEY& mapValLeft, MAPCFGKEY& mapValRight);
	ENUMCOMAPRETYPE CompareLeft(ST_CFGVALUE& stValLeft, ST_CFGVALUE& stValRight);
	ENUMCOMAPRETYPE CompareLeft(VECCFGVALUE& vecValLeft, VECCFGVALUE& vecValRight);
	ENUMCOMAPRETYPE CompareLeft(CFGVALUE& wsValLeft, CFGVALUE& wsValRight);

	BOOL ExtractLine(wstring& strline, ST_CFGVALUE& valOut);
	BOOL ExtractValue(ST_CFGVALUE& valOut);
	BOOL IsSection(wstring& strlineInOut);
	//note that at line begin with ";" or "//"
	BOOL IsNote(wstring& strLineIn);
	//filter note that begin with "//" at line end.
	void FilterEndNote(wstring& strLineInOut);
public:
	MAPSROOT m_mapCfgRoot;
	MAPCFGSECTION m_mapValLeft;
	MAPCFGSECTION m_mapValRight;

private:
	int m_nCurrLine;
	void WriteLog( LPCTSTR format,...);
};


void FindCfgCallback(CFileFind& ffind, int nLevel, LPARAM lparam);
