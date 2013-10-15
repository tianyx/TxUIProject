#include "StdAfx.h"
#include "MBCGraphDrawBaseObj.h"
#include "FGlobal.h"
#include "MBCCommonDef.h"
#include "StrConvert.h"
#include "GDIDrawFunc.h"
#include "TxFontLoader.h"


Bitmap* CMBCGDrawBaseObj::m_spbmpCache = NULL;
int CMBCGDrawBaseObj::m_snCacheRef = 0;
CMBCGDrawBaseObj::CMBCGDrawBaseObj(void)
{
	m_nGraphState = graphSate_none;
	m_snCacheRef++;
	if (!m_spbmpCache)
	{
		m_spbmpCache = new Bitmap(g_nScreenCX, g_nScreenCY,PixelFormat32bppARGB);
	}

	m_ArrLineClr = Color::Green;
}

CMBCGDrawBaseObj::~CMBCGDrawBaseObj(void)
{
	m_snCacheRef--;
	if (m_snCacheRef == 0)
	{
		if (m_spbmpCache)
		{
			delete m_spbmpCache;
			m_spbmpCache = NULL;
		}
	}
}


BOOL CMBCGDrawBaseObj::Init( ST_OBJSTATEINFO& objInfo, const int nGraphState )
{
	m_pObjInfo = &objInfo;
	m_nGraphState = nGraphState;
	m_nObjType = objInfo.nObjType;
	m_nObjState = objInfo.nSelfState;
	m_nRemoteState = objInfo.remoteInfo.nRemoteState;
	m_strObjName = objInfo.strName;
	m_vDrawData.clear();
	m_lineInInfo.vaddrs.clear();
	m_lineOutInfo.vaddrs.clear();
	m_mapLineInObjInfo.clear();
	m_mapLineOutObjInfo.clear();
	return TRUE;
}

void CMBCGDrawBaseObj::SaveLineOutObj( CMBCGDrawBaseObj* pObj , SOCKADDR_IN& addr)
{
	m_mapLineOutObjInfo[pObj] = addr;
}	

BOOL CMBCGDrawBaseObj::CanConnectIn( CMBCGDrawBaseObj* pObjIn )
{
	
	BOOL bFound = FALSE;
	const ST_LINEENDINFO& lineOutOther =  pObjIn->GetLineOutInfo();
	for (size_t i = 0; i < lineOutOther.vaddrs.size(); ++i)
	{
		const SOCKADDR_IN& addrOutOther = lineOutOther.vaddrs[i];
		for (size_t j= 0; j < m_lineInInfo.vaddrs.size(); ++j)
		{
			if (addrOutOther.sin_family == AF_INET && addrOutOther == m_lineInInfo.vaddrs[j])
			{
				//found a link
				pObjIn->SaveLineOutObj(this, (SOCKADDR_IN)addrOutOther);
				bFound = TRUE;
				break;
			}
		}

		if (bFound)
		{
			break;
		}
	}

	return bFound;
}

BOOL CMBCGDrawBaseObj::ResetDrawData()
{
	//init base info
	m_vDrawData.clear();
	m_lineInInfo.vaddrs.clear();
	m_lineOutInfo.vaddrs.clear();
	// title
	ST_GDRAWITEM item;
	m_vDrawData.push_back(item.Init(TEXT("名称:"), m_strObjName));
	m_vDrawData.push_back(item.Init(TEXT("类型:"), MBCObjType2String(m_nObjType)));
	m_vDrawData.push_back(item.Init(TEXT("状态:"), MBCState2String(m_nObjState)));
	m_vDrawData.push_back(item.Init(TEXT("监控对象状态:"), m_nObjState == MBCSTATE_OK? MBCState2String(m_nRemoteState):TEXT("--")));
	return TRUE;
}

CSize& CMBCGDrawBaseObj::ReCalcNeedSize()
{
	m_szNeeded.cx = g_nDefMBCDrawItemCX;
	m_szNeeded.cy = m_vDrawData.size() * g_nDefMBCDrawItemCY;

	return m_szNeeded;
}

void CMBCGDrawBaseObj::SetDrawRect( CPoint& ptTopLeft, double dZoom )
{
	m_ptLeftTop = ptTopLeft;
	m_dZoom = dZoom;
}

void CMBCGDrawBaseObj::Draw( Graphics& gcIn )
{
	if (!m_spbmpCache || m_vDrawData.size() == 0)
	{
		return;
	}
	Graphics gc(m_spbmpCache);

	Rect rcSrc(0,0, m_szNeeded.cx, m_szNeeded.cy);
	Rect rcDes(0, 0, m_dZoom * m_szNeeded.cx, m_dZoom * m_szNeeded.cy);
	GraphicsContainer gcon = gc.BeginContainer(rcDes, rcSrc, UnitPixel);
	//start draw
	Color clrBk = Color::Transparent;
	SolidBrush brush(clrBk);
	gc.FillRectangle(&brush, rcSrc);

	Color clrPen = Color::Blue;
	clrPen = GetStateColor(m_nObjState);

	Pen pen(clrPen, 4.0);
	gc.DrawRectangle(&pen, rcSrc);
	Rect rcSrcFill(rcSrc);
	rcSrcFill.Inflate(-2,-2);
	brush.SetColor(Color::LightGray);
	gc.FillRectangle(&brush, rcSrcFill);
	Color clrText = Color::Black;
	SolidBrush brushText(clrText);
	StringFormat formatTxt;
	formatTxt.SetAlignment(StringAlignmentNear);
	formatTxt.SetLineAlignment(StringAlignmentCenter);
	formatTxt.SetTrimming(StringTrimmingEllipsisCharacter);
	formatTxt.SetFormatFlags(StringFormatFlagsNoWrap);
	int nInterval = rcSrcFill.Height /m_vDrawData.size();
	Rect rcText(rcSrcFill.X, rcSrcFill.Y, rcSrcFill.Width, rcSrcFill.Y + nInterval);
	RectF rfText = Rect2RectF(rcText);
	for (size_t i = 0; i < m_vDrawData.size(); ++i)
	{
		ST_GDRAWITEM& drawItem = m_vDrawData[i];
		std::wstring wstrText = CStr2W(drawItem.strTitle + drawItem.strText);
		gc.DrawString(wstrText.c_str(),-1, g_pFontLoader->GetFont(0), rfText, &formatTxt, &brushText);
		rcText.Offset(0, nInterval);
		rfText = Rect2RectF(rcText);
	}
	
	gc.EndContainer(gcon);

	gcIn.DrawImage(m_spbmpCache, m_ptLeftTop.x, m_ptLeftTop.y, rcDes.X, rcDes.Y, rcDes.Width+1, rcDes.Height+1,UnitPixel);

	//draw line
	if (m_nObjType == MBCOBJTYPE_ENDBACK
		&&m_nObjState != MBCSTATE_OK)
	{
		return;
	}
	Pen arrLinePen(m_ArrLineClr, 2.0);
	GraphicsPath strokePath;

	AdjustableArrowCap myCap(6.0f,4.0f,TRUE);
	
	arrLinePen.SetCustomEndCap(&myCap);
	MAPLINEENDOBJS::iterator itb = m_mapLineOutObjInfo.begin();
	MAPLINEENDOBJS::iterator ite = m_mapLineOutObjInfo.end();
	Rect rcSelf(m_ptLeftTop.x, m_ptLeftTop.y, rcDes.Width, rcDes.Height);
	for (; itb != ite; ++itb)
	{
		CMBCGDrawBaseObj* pOutObj = itb->first;
		if (pOutObj)
		{
			Rect rcOut = pOutObj->GetDrawRect();
			Point ptFrom, ptTo;
			CalcLineEnds(rcSelf, rcOut, ptFrom, ptTo );
			gcIn.DrawLine(&arrLinePen, ptFrom, ptTo);

		}

	}
	


}

Rect CMBCGDrawBaseObj::GetDrawRect()
{
	return Rect(m_ptLeftTop.x, m_ptLeftTop.y, m_dZoom *m_szNeeded.cx, m_dZoom* m_szNeeded.cy);
}

CString MBCObjType2String( int nObjTypeIn )
{
	CString strRet;
	switch (nObjTypeIn)
	{
	case MBCOBJTYPE_CLIENT:
		strRet = TEXT("客户端");
		break;
	case MBCOBJTYPE_SVR:
		strRet = TEXT("服务端");
		break;
	case MBCOBJTYPE_ENDMASTER:
		strRet = TEXT("多播点(主)");
		break;
	case MBCOBJTYPE_ENDSLAVE:
		strRet = TEXT("多播点(备)");
		break;

	case MBCOBJTYPE_ENDBACK:
		strRet = TEXT("多播点(垫片)");
		break;
	default:
		break;
	}
	return strRet;
}

CString MBCState2String( int nStateIn )
{
	CString strRet = TEXT("--");
	switch(nStateIn)
	{
	case MBCSTATE_OK:
		strRet = TEXT("正常");
		break;
	case MBCSTATE_STOP:
		strRet = TEXT("停止");
		break;

	case MBCSTATE_NETERR:
		strRet = TEXT("网络异常");
		break;
	case MBCSTATE_NOTRESPOND:
		strRet = TEXT("未响应");
		break;
	case MBCSTATE_STREAMDOWN:
		strRet = TEXT("断流");
		break;
	default:
		break;
	}

	return strRet;
}

Color GetStateColor( int nObjStateIn )
{
	Color clrOut = Color::Black;
	switch(nObjStateIn)
	{
	case MBCSTATE_OK:

		break;
	case MBCSTATE_STOP:
		clrOut = Color::Blue;
		break;

	case MBCSTATE_NETERR:
		clrOut = Color::OrangeRed;
		break;
	case MBCSTATE_NOTRESPOND:
		clrOut = Color::MediumVioletRed;
		break;
	case MBCSTATE_STREAMDOWN:
		clrOut = Color::IndianRed;
		break;
	default:
		break;
	}

	return clrOut;
}

BOOL CalcLineEnds( const Rect& rcFrom, const Rect& rcTo, Point& ptFrom, Point& ptTo )
{
	ptFrom.X = (rcFrom.GetLeft() + rcFrom.GetRight())/2;
	ptTo.X = (rcTo.GetLeft() + rcTo.GetRight())/2;

	if (rcFrom.GetBottom() < rcTo.GetTop())
	{
		//from.bottom --->to.top
		ptFrom.Y = rcFrom.GetBottom();
		ptTo.Y = rcTo.GetTop();

	}
	else if (rcFrom.GetTop() > rcTo.GetBottom())
	{
		//from.top---> to.bottom
		ptFrom.Y = rcFrom.GetTop();
		ptTo.Y = rcTo.GetBottom();

	}
	else
	{
		ASSERT(FALSE);
		//center -->center
		ptFrom.Y = (rcFrom.GetTop() + rcFrom.GetBottom())/2;
		ptTo.Y = (rcTo.GetTop() + rcTo.GetBottom())/2;

	}

	return TRUE;
}
