#pragma once
#include "GdiPlusNewHeader.h"
#include <vector>
#include <map>
#include "MBCCommonDef.h"
using namespace Gdiplus;
 
class CMBCGDrawBaseObj;
struct ST_GDRAWITEM
{
	CString strTitle;
	CString strText;
	//for line
	CMBCGDrawBaseObj* pLineOutObj;
	CString strLineText;

	ST_GDRAWITEM()
	{
		pLineOutObj = NULL;
	}
	ST_GDRAWITEM& Init(LPCTSTR szTitleIn, LPCTSTR szTextIn, CMBCGDrawBaseObj* pObjIn = NULL, LPCTSTR szLineIn = NULL)
	{
		strTitle = szTitleIn;
		strText = szTextIn;
		pLineOutObj = pObjIn;
		strLineText = szLineIn;
		return *this;
	}
};

typedef std::vector<ST_GDRAWITEM> VECDRAWDATA;
typedef std::map<CMBCGDrawBaseObj*, SOCKADDR_IN>MAPLINEENDOBJS;

struct ST_LINEENDINFO
{
	std::vector<SOCKADDR_IN> vaddrs;
};

class CMBCGDrawBaseObj
{
public:
	CMBCGDrawBaseObj();
	virtual ~CMBCGDrawBaseObj(void);

	virtual BOOL Init(ST_OBJSTATEINFO& objInfo, const int nGraphState);
	virtual BOOL ResetDrawData();

	virtual BOOL CanConnectIn(CMBCGDrawBaseObj* pObjIn);
	virtual void Draw(Graphics& gc);

	CSize& ReCalcNeedSize();
	void SetDrawRect(CPoint& ptTopLeft, double dZoom);
	Rect GetDrawRect();

protected:
	virtual const ST_LINEENDINFO& GetLineOutInfo(){return m_lineOutInfo;}
	virtual void SaveLineOutObj(CMBCGDrawBaseObj* pObj, SOCKADDR_IN& addr);

	ST_LINEENDINFO m_lineOutInfo;
	ST_LINEENDINFO m_lineInInfo;

	MAPLINEENDOBJS m_mapLineInObjInfo;
	MAPLINEENDOBJS m_mapLineOutObjInfo;

public:
	int m_nGraphState;
	int m_nObjType;
	int m_nObjState;
	int m_nRemoteState;
	CString m_strObjName;
	ST_OBJSTATEINFO* m_pObjInfo;

	//for draw
	VECDRAWDATA m_vDrawData;
	CPoint m_ptLeftTop;
	CSize m_szNeeded;
	double m_dZoom; // 0--1.0

	ARGB m_ArrLineClr;
	ARGB m_ClrBackGround;

	static Bitmap* m_spbmpCache;
private:
	static int m_snCacheRef;
};

CString MBCObjType2String(int nObjTypeIn);
CString MBCState2String(int nStateIn);
Color GetStateColor(int nObjStateIn);
BOOL CalcLineEnds(const Rect& rcFrom, const Rect& rcTo, Point& ptFrom, Point& ptTo);