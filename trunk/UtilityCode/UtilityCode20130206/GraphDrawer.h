#pragma once
#include "MBCCommonDef.h"

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

private:
	int GetMaxHeight(VECDRAWOBJS& vObjs);
	void RelayoutLineObjs(VECDRAWOBJS& vObjs, const  CPoint& ptLeftTop, double dZoom, int nIntervalX);

	VECDRAWOBJS m_vObjClient;
	VECDRAWOBJS m_vObjSvr;
	VECDRAWOBJS m_vObjEndMaster;
	VECDRAWOBJS m_vObjEndSlave;
	VECDRAWOBJS m_vObjEndBack;

public:
	CSize m_szGraphSize; //the 1:1 graph size
	int nLastGraphUniqueId;
	Bitmap m_bmpCache;
	double m_dZoom;
};


