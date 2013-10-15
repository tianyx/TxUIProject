#include "StdAfx.h"
#include "GraphDrawer.h"
#include "GDIDrawFunc.h"
#include <algorithm>
#include "MBCGDrawClientObj.h"
#include "MBCGDrawSvrObj.h"
#include "MBCGDrawEndObj.h"
#include "FGlobal.h"

using namespace std;
using namespace Gdiplus;


void ReCalcDrawObjSizeProc(CMBCGDrawBaseObj* pObj)
{
	if (pObj)
	{
		pObj->ReCalcNeedSize();
	}
}

void ResetDrawDataProc(CMBCGDrawBaseObj* pObj)
{
	if (pObj)
	{
		pObj->ResetDrawData();
	}
}

class CObjDrawcls
{
public:
	Graphics* m_pgc;
	CObjDrawcls(Graphics* pgc):m_pgc(pgc)
	{
	}

	void operator ( ) ( CMBCGDrawBaseObj* pObj ) const
	{
		if (m_pgc && pObj)
		{
			pObj->Draw(*m_pgc);
		}
	}

	
};


void DelDrawObjProc(CMBCGDrawBaseObj* pObj)
{
	if (pObj)
	{
		delete pObj;
	}
}
CGraphDrawer::CGraphDrawer(void):m_bmpCache(g_nScreenCX, g_nScreenCY, PixelFormat32bppARGB)
{
	nLastGraphUniqueId = -1;
	m_dZoom =1.0;
}

CGraphDrawer::~CGraphDrawer(void)
{
}

void CGraphDrawer::DrawGraph(HDC hdcIn, CPoint& ptLeftTop, CSize& szSizeIn )
{
	Graphics gc(&m_bmpCache);
	SolidBrush blackBrush(Color::WhiteSmoke);
	gc.FillRectangle(&blackBrush, 0, 0, g_nScreenCX, g_nScreenCY);

	CPoint ptDraw(0,0);
	if (szSizeIn.cx > m_szGraphSize.cx)
	{
		//to the center
		ptLeftTop.x = (szSizeIn.cx - m_szGraphSize.cx) /2;
	}
	if (szSizeIn.cy > m_szGraphSize.cy)
	{
		//to the center
		ptLeftTop.y = (szSizeIn.cy - m_szGraphSize.cy) /2;
	}

	Rect rcSrc(0,0,g_nScreenCX, g_nScreenCY);
	Rect rcDes(ptLeftTop.x, ptLeftTop.y, g_nScreenCX, g_nScreenCY);
	GraphicsContainer gcn = gc.BeginContainer(rcDes,rcSrc,  UnitPixel);
	// Define the rectangle.
	int x = 0;
	int y = 0;
	// Fill the rectangle.
	for_each(m_vObjClient.begin(), m_vObjClient.end(), CObjDrawcls(&gc));
	for_each(m_vObjSvr.begin(), m_vObjSvr.end(), CObjDrawcls(&gc));
	for_each(m_vObjEndSlave.begin(), m_vObjEndSlave.end(), CObjDrawcls(&gc));
	for_each(m_vObjEndMaster.begin(), m_vObjEndMaster.end(), CObjDrawcls(&gc));
	for_each(m_vObjEndBack.begin(), m_vObjEndBack.end(), CObjDrawcls(&gc));


	gc.EndContainer(gcn);
	Graphics gcIn(hdcIn);

		
	


	gcIn.DrawImage(&m_bmpCache,  0,0, 0, 0, szSizeIn.cx, szSizeIn.cy, UnitPixel);

}

HRESULT CGraphDrawer::ResetDrawData( ST_GRAPHSTATE& gsStateInfo )
{
	//calc obj type is changed?
	BOOL bReset = FALSE;
	if (nLastGraphUniqueId == -1 || nLastGraphUniqueId != gsStateInfo.nUniqueId)
	{
		//must recreate
		bReset = TRUE;
		nLastGraphUniqueId = gsStateInfo.nUniqueId;
		ClearDrawer();
		for (size_t i = 0; i < gsStateInfo.vClientInfo.size(); ++i)
		{
			CMBCGDrawClientObj* pObj = new CMBCGDrawClientObj;
			pObj->Init(gsStateInfo.vClientInfo[i], gsStateInfo.nGraphState);
			m_vObjClient.push_back(pObj);
		}

		for (size_t i = 0; i < gsStateInfo.vSvrInfo.size(); ++i)
		{
			CMBCGDrawSvrObj* pObj = new CMBCGDrawSvrObj;
			pObj->Init(gsStateInfo.vSvrInfo[i], gsStateInfo.nGraphState);
			m_vObjSvr.push_back(pObj);
		}

		for (size_t i = 0; i < gsStateInfo.vEndInfo.size(); ++i)
		{
			ST_OBJSTATEINFO& objInfo = gsStateInfo.vEndInfo[i];
			CMBCGDrawBaseObj* pObj = new CMBCGDrawEndObj;
			pObj->Init(gsStateInfo.vEndInfo[i], gsStateInfo.nGraphState);
			if (objInfo.nObjType == MBCOBJTYPE_ENDMASTER)
			{
				m_vObjEndMaster.push_back(pObj);
			}
			else if (objInfo.nObjType == MBCOBJTYPE_ENDSLAVE)
			{
				m_vObjEndSlave.push_back(pObj);
			}
			else if (objInfo.nObjType == MBCOBJTYPE_ENDBACK)
			{
				m_vObjEndBack.push_back(pObj);
			}
			else
			{
				ASSERT(FALSE);
				return FALSE;
			}
			
		}
	}
	else
	{
		size_t nClienInfoSize =gsStateInfo.vClientInfo.size();
		size_t nSvrInfoSize =gsStateInfo.vSvrInfo.size();
		size_t nEndInfoSize =gsStateInfo.vEndInfo.size();
		if (nClienInfoSize != m_vObjClient.size()
			|| nSvrInfoSize != m_vObjSvr.size()
			|| nEndInfoSize != m_vObjEndMaster.size() + m_vObjEndBack.size() + m_vObjEndSlave.size())
		{
			ASSERT(FALSE);
			return FALSE;
		}
		//graph not changed. only reInit the param
		for (size_t i = 0; i < gsStateInfo.vClientInfo.size(); ++i)
		{
			CMBCGDrawBaseObj* pObj = m_vObjClient[i];
			pObj->Init(gsStateInfo.vClientInfo[i], gsStateInfo.nGraphState);
		}

		for (size_t i = 0; i < gsStateInfo.vSvrInfo.size(); ++i)
		{
			CMBCGDrawBaseObj* pObj = m_vObjSvr[i];
			pObj->Init(gsStateInfo.vSvrInfo[i], gsStateInfo.nGraphState);
		}

		for (size_t i = 0, j = 0, k = 0, l = 0; i < gsStateInfo.vEndInfo.size(); ++i)
		{
			ST_OBJSTATEINFO& objInfo = gsStateInfo.vEndInfo[i];
			CMBCGDrawBaseObj* pObj = NULL;
				if (objInfo.nObjType == MBCOBJTYPE_ENDMASTER)
				{
					pObj = m_vObjEndMaster[j];
					++j;
				}
				else if (objInfo.nObjType == MBCOBJTYPE_ENDSLAVE)
				{
					pObj = m_vObjEndSlave[k];
					++k;
				}
				else if (objInfo.nObjType == MBCOBJTYPE_ENDBACK)
				{
					pObj = m_vObjEndBack[l];
					++l;
				}
				else
				{
					ASSERT(FALSE);
					return FALSE;
				}
				if (pObj)
				{
					pObj->Init(gsStateInfo.vEndInfo[i], gsStateInfo.nGraphState);
				}

		}

	}
	

	//gen line info
	//client to svr
	for (size_t i = 0; i < m_vObjClient.size(); ++i)
	{
		for (size_t j = 0; j < m_vObjSvr.size(); ++j)
		{
			m_vObjSvr[j]->CanConnectIn(m_vObjClient[i]);
		}
	}

	//svr to end
	for (size_t i = 0; i < m_vObjSvr.size(); ++i)
	{
		for (size_t j = 0; j < m_vObjEndMaster.size(); ++j)
		{
			m_vObjEndMaster[j]->CanConnectIn(m_vObjSvr[i]);
		}
	}

	for (size_t i = 0; i < m_vObjSvr.size(); ++i)
	{
		for (size_t j = 0; j < m_vObjEndSlave.size(); ++j)
		{
			m_vObjEndSlave[j]->CanConnectIn(m_vObjSvr[i]);
		}
	}


	for (size_t i = 0; i < m_vObjSvr.size(); ++i)
	{
		for (size_t j = 0; j < m_vObjEndMaster.size(); ++j)
		{
			m_vObjEndMaster[j]->CanConnectIn(m_vObjSvr[i]);
		}

		for (size_t k = 0; k < m_vObjEndSlave.size(); ++k)
		{
			m_vObjEndSlave[k]->CanConnectIn(m_vObjSvr[i]);
		}
	}

	//back end to master

	for (size_t i = 0; i < m_vObjEndBack.size(); ++i)
	{
		for (size_t j = 0; j < m_vObjEndMaster.size(); ++j)
		{
			m_vObjEndMaster[j]->CanConnectIn(m_vObjEndBack[i]);
		}
	}

	//init draw info

	for_each(m_vObjClient.begin(), m_vObjClient.end(), ResetDrawDataProc);
	for_each(m_vObjSvr.begin(), m_vObjSvr.end(), ResetDrawDataProc);
	for_each(m_vObjEndMaster.begin(), m_vObjEndMaster.end(), ResetDrawDataProc);
	for_each(m_vObjEndSlave.begin(), m_vObjEndSlave.end(), ResetDrawDataProc);
	for_each(m_vObjEndBack.begin(), m_vObjEndBack.end(), ResetDrawDataProc);

	
	//relayout graph
	ReLayoutGraph();

	return TRUE;
}


void CGraphDrawer::ClearDrawer()
{
	for_each(m_vObjClient.begin(), m_vObjClient.end(), DelDrawObjProc);
	for_each(m_vObjSvr.begin(), m_vObjSvr.end(), DelDrawObjProc);
	for_each(m_vObjEndMaster.begin(), m_vObjEndMaster.end(), DelDrawObjProc);
	for_each(m_vObjEndSlave.begin(), m_vObjEndSlave.end(), DelDrawObjProc);
	for_each(m_vObjEndBack.begin(), m_vObjEndBack.end(), DelDrawObjProc);

	m_vObjClient.clear();
	m_vObjSvr.clear();
	m_vObjEndSlave.clear();
	m_vObjEndMaster.clear();
	m_vObjEndBack.clear();

}

void CGraphDrawer::ReLayoutGraph(double dZoom  /*= -1.0*/)
{
	if (dZoom > 0)
	{
		dZoom = dZoom < 0.1? 0.1:dZoom;
		dZoom = dZoom > 1.0? 1.0:dZoom;
		m_dZoom = dZoom;
	}
	else
	{
		dZoom = m_dZoom;
	}


	VECDRAWOBJS tmpEnd;
	for (size_t i = 0, j = 0; i < m_vObjEndMaster.size() || j < m_vObjEndSlave.size();++i, ++j)
	{
		if (i < m_vObjEndMaster.size())
		{
			tmpEnd.push_back(m_vObjEndMaster[i]);
		}
		if (j < m_vObjEndSlave.size())
		{
			tmpEnd.push_back(m_vObjEndSlave[j]);
		}
	}
	
	int nMaxWidth = 0;
	int nMaxHeight  = 0;
	std::vector<int> vWidths;

	nMaxWidth = max( nMaxWidth,  m_vObjClient.size());
	nMaxWidth = max( nMaxWidth,  m_vObjSvr.size());
	nMaxWidth = max( nMaxWidth,  tmpEnd.size());
	//nMaxWidth = max( nMaxWidth,  m_vObjEndSlave.size());
	nMaxWidth = max( nMaxWidth,  m_vObjEndBack.size());

	int nClientHeightMax = dZoom*  GetMaxHeight(m_vObjClient);
	int nSvrHeightMax = dZoom*  GetMaxHeight(m_vObjSvr);
	int nEndMasterHeightMax = dZoom*  GetMaxHeight(m_vObjEndMaster);
	//int nEndSlaveHeightMax = dZoom*  GetMaxHeight(m_vObjEndSlave);
	int nEndBackHeightMax =dZoom*  GetMaxHeight(m_vObjEndBack);

	int nLineNum = 0;
	nClientHeightMax > 0? nLineNum++:0;
	nSvrHeightMax > 0? nLineNum++:0;
	nEndMasterHeightMax > 0? nLineNum++:0;
	//nEndSlaveHeightMax > 0? nLineNum++:0;
	nEndBackHeightMax > 0? nLineNum++:0;

	m_szGraphSize.cx =dZoom *((g_nDefMBCDrawItemCX + g_nDefMBCDrawObjIntervalCX) * nMaxWidth +g_nDefMBCDrawObjIntervalCX);
	m_szGraphSize.cy = (nClientHeightMax + nSvrHeightMax + nEndMasterHeightMax +  nEndBackHeightMax) + (g_nDefMBCDrawObjIntervalCY* (nLineNum+1));
	if (m_szGraphSize.cx == 0 || m_szGraphSize.cy == 0)
	{
		return;
	}
	nMaxWidth = m_szGraphSize.cx;
	nMaxHeight = m_dZoom * m_szGraphSize.cy;

	int nIntervalY = m_dZoom * g_nDefMBCDrawObjIntervalCY;
	
	//end back
	CPoint ptLeftTop(0,0);
	int nIntervalNum = m_vObjEndBack.size()+1;
	int nEndBackIntervalX = m_dZoom * ((m_szGraphSize.cx -((nIntervalNum-1) * g_nDefMBCDrawItemCX*dZoom)) / nIntervalNum);
	ASSERT(nEndBackIntervalX > 0);
	ptLeftTop.x =nEndBackIntervalX;
	ptLeftTop.y = nIntervalY;
	RelayoutLineObjs(m_vObjEndBack, ptLeftTop, m_dZoom, nEndBackIntervalX);

	

	
	//end master
	nIntervalNum = tmpEnd.size() +1;
	int nEndMasterIntervalX = m_dZoom * ((m_szGraphSize.cx -((nIntervalNum-1) * g_nDefMBCDrawItemCX*dZoom)) / nIntervalNum);
	ptLeftTop.x = nEndMasterIntervalX;
	ptLeftTop.y +=nEndBackHeightMax > 0? nEndBackHeightMax +nIntervalY:0;
	//RelayoutLineObjs(m_vObjEndMaster, ptLeftTop, m_dZoom, nEndMasterIntervalX);
	RelayoutLineObjs(tmpEnd, ptLeftTop, m_dZoom, nEndMasterIntervalX);

	//end slave
// 	nIntervalNum = m_vObjEndSlave.size() +1;
// 	int nEndSlaveIntervalX = m_dZoom * ((m_szGraphSize.cx -((nIntervalNum-1) * g_nDefMBCDrawItemCX)) / nIntervalNum);
// 	ptLeftTop.x = nEndSlaveIntervalX;
// 	ptLeftTop.y += nEndMasterHeightMax> 0? nEndMasterHeightMax +nIntervalY:0;
// 	RelayoutLineObjs(m_vObjEndSlave, ptLeftTop, m_dZoom, nEndSlaveIntervalX);

	//svr
	nIntervalNum = m_vObjSvr.size() +1;
	int nSvrIntervalX = m_dZoom * ((m_szGraphSize.cx -((nIntervalNum-1) * g_nDefMBCDrawItemCX*dZoom)) / nIntervalNum);
	ptLeftTop.x = nSvrIntervalX;
	ptLeftTop.y +=nEndMasterHeightMax> 0? nEndMasterHeightMax +nIntervalY:0;
	RelayoutLineObjs(m_vObjSvr, ptLeftTop, m_dZoom, nSvrIntervalX);

	//client
	nIntervalNum = m_vObjClient.size() +1;
	int nClientIntervalX = m_dZoom * ((m_szGraphSize.cx -((nIntervalNum-1) * g_nDefMBCDrawItemCX*dZoom)) / nIntervalNum);
	ptLeftTop.x = nClientIntervalX;
	ptLeftTop.y +=nSvrHeightMax> 0? nSvrHeightMax +nIntervalY:0;
	RelayoutLineObjs(m_vObjClient, ptLeftTop, m_dZoom, nClientIntervalX);
}

int CGraphDrawer::GetMaxHeight( VECDRAWOBJS& vObjs )
{
	int nMaxHeight = 0;
	for (size_t i = 0; i < vObjs.size(); ++i)
	{
		nMaxHeight = max(nMaxHeight, vObjs[i]->m_szNeeded.cy);
	}

	return nMaxHeight;
}

void CGraphDrawer::RelayoutLineObjs( VECDRAWOBJS& vObjs, const CPoint& ptLeftTop, double dZoom, int nIntervalX )
{
	CPoint ptOrg = ptLeftTop;
	for (size_t i = 0; i < vObjs.size(); ++i)
	{
		vObjs[i]->SetDrawRect(ptOrg, dZoom);
		ptOrg.Offset(nIntervalX + (dZoom *g_nDefMBCDrawItemCX) , 0);
	}
}

