#pragma once
#include "MBCCommonDef.h"
#include "AutoCritSec.h"
#include "MBCGraphDrawBaseObj.h"
using namespace  std;

typedef vector<CMBCGDrawBaseObj*> VECDRAWOBJS;



class CGraphDrawer
{
public:
	CGraphDrawer();
	virtual ~CGraphDrawer(void);
	
	HRESULT ResetDrawData(ST_GRAPHSTATE& gsStateInfo);
	void ReLayoutGraph(double dZoom = -1.0);

	void DrawGraph(HDC hdcIn, CPoint& ptLeftTop, CSize& szSizeIn );

	void ClearDrawer();

	//set dZoom < 0 to not use this param
	//pt in client view coordinate
	BOOL HitTestObject(CPoint ptIn, ST_OBJSTATEINFO& objInfoOut);

private:
	int GetMaxHeight(VECDRAWOBJS& vObjs);
	void RelayoutLineObjs(VECDRAWOBJS& vObjs, const  CPoint& ptLeftTop, double dZoom, int nIntervalX);

	VECDRAWOBJS m_vObjClient;
	VECDRAWOBJS m_vObjSvr;
	VECDRAWOBJS m_vObjEndMaster;
	VECDRAWOBJS m_vObjEndSlave;
	VECDRAWOBJS m_vObjEndBack;

	CAutoCritSec m_csData;

public:
	CSize m_szGraphSize; //the 1:1 graph size
	CPoint m_ptLastDrawOrg;
	int nLastGraphUniqueId;
	Bitmap m_bmpCache;
	double m_dZoom;
};


