#include "StdAfx.h"
#include "ADXmlLoader.h"
#include "CADXMLDOMDocument.h"
#include "CADXMLDOMNode.h"
#include "CADXMLDOMNodeList.h"
#include "CADXMLElement.h"
#include "io.h"

//////////////////////////////////////////////////////////////////////////
BOOL LoadADSegment(CADXMLDOMNode& adNodeIn, ST_ADPACKDATA& dataOut);
BOOL LoadADItem(CADXMLDOMNode& adNodeIn, ST_ADITEMDATA& dataOut);

void TLWriteLog(LPCTSTR format, ...){}
CADXmlLoader::CADXmlLoader(void)
{
}

CADXmlLoader::~CADXmlLoader(void)
{
}


BOOL CADXmlLoader::LoadADPackListFromFile( CString& strXmlFileIn, ST_ADPACK& dataOut )
{
	if (_access(strXmlFileIn, 04) == -1)
	{
		ASSERT(FALSE);
		TLWriteLog("无法读取文件:%s", strXmlFileIn);
		return FALSE;
	}

	CADXMLDOMDocument adDoc;
 	ADXML::IXMLDOMDocument2* pdoc = NULL;
 //	HRESULT hr = CoCreateInstance(__uuidof(ADXML::DOMDocument), NULL, CLSCTX_INPROC_SERVER, __uuidof(ADXML::IXMLDOMDocument), reinterpret_cast<void**>(&pdoc));
	if(!adDoc.CreateDispatch(__uuidof(ADXML::DOMDocument)))
	{
		ASSERT(FALSE);
		return FALSE;
	}
	COleVariant var(strXmlFileIn);
	if(!adDoc.load(var))
	{
		ASSERT(FALSE);
		TLWriteLog("xml加载失败!");
		return FALSE;
	}

	CADXMLDOMNode spotNode = adDoc.selectSingleNode("SpotListInfo");
	if (!spotNode.m_lpDispatch)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	CADXMLDOMNodeList adSpotNodeList = spotNode.get_childNodes();
	if (!adSpotNodeList.m_lpDispatch)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	BOOL bRet = TRUE;
	int nSpotLen = adSpotNodeList.get_length();
	ASSERT(nSpotLen > 3);
	for (int i = 0; i < nSpotLen; i++)
	{
		CADXMLDOMNode adNode = adSpotNodeList.get_item(i);
		CString strNodeName = adNode.get_nodeName();

		if (strNodeName.Compare("Date") == 0)
		{
			dataOut.strDate = adNode.get_nodeTypedValue();
		}
		else if (strNodeName.Compare("PackageTypeName") == 0)
		{
			dataOut.strPkTypeName = adNode.get_nodeTypedValue();
		}
		else if (strNodeName.Compare("ChannelName") == 0)
		{
			dataOut.strChName =  adNode.get_nodeTypedValue();
		}
		else if (strNodeName.Compare("SegmentInfo") == 0)
		{
			ST_ADPACKDATA tmpPackData;
			bRet = LoadADSegment(adNode, tmpPackData);
			if (bRet)
			{
				dataOut.vecAdPackData.push_back(tmpPackData);
			}
			else
			{
				ASSERT(FALSE);
				break;
			}
		}
		else
		{
			ASSERT(FALSE);
			TLWriteLog("find unknown node: %s", strNodeName);
		}
	}
	

	return bRet;
}

BOOL LoadADSegment( CADXMLDOMNode& adNodeIn, ST_ADPACKDATA& dataOut )
{
	CADXMLDOMNodeList segNodeList = adNodeIn.get_childNodes();
	if (!segNodeList.m_lpDispatch)
	{
		ASSERT(FALSE);
		TLWriteLog("not find segment info");
		return FALSE;
	}

	BOOL bRet = TRUE;
	int nLen = segNodeList.get_length();
	for (int i = 0; i < nLen; i++)
	{
		CADXMLDOMNode segNode = segNodeList.get_item(i);
		CString strNodeName = segNode.get_nodeName();
		if (strNodeName.Compare("SegmentName") == 0)
		{
			dataOut.strSegmentName = segNode.get_nodeTypedValue();
		}
		else if (strNodeName.Compare("Creater") == 0)
		{
			dataOut.strCreater = segNode.get_nodeTypedValue();
		}
		else if (strNodeName.Compare("Item") == 0)
		{
			ST_ADITEMDATA tmpItem;
			bRet = LoadADItem(segNode, tmpItem);
			if (bRet)
			{
				dataOut.mapAdItem[tmpItem.nIdx] = tmpItem;
			}
			else
			{
				ASSERT(FALSE);
				break;
			}
		}
	}

	return bRet;
}

BOOL LoadADItem( CADXMLDOMNode& adNodeIn, ST_ADITEMDATA& dataOut )
{
	CADXMLDOMNodeList itNodeList = adNodeIn.get_childNodes();
	if (!itNodeList.m_lpDispatch)
	{
		ASSERT(FALSE);
		TLWriteLog("not find segment info");
		return FALSE;
	}

	BOOL bRet = TRUE;
	int nLen = itNodeList.get_length();
	for (int i = 0; i < nLen; i++)
	{
		CADXMLDOMNode itNode = itNodeList.get_item(i);
		CString strNodeName = itNode.get_nodeName();
		CString strNodeVal = itNode.get_nodeTypedValue();
		if (strNodeName.Compare("Index") == 0)
		{
			dataOut.nIdx = atoi(strNodeVal);
		}
		else if (strNodeName.Compare("ProgramName") == 0)
		{
			dataOut.ProgramName = strNodeVal;
		}
		else if (strNodeName.Compare("strSom") == 0)
		{
			dataOut.nSom = atoi(strNodeVal);

		}
		else if (strNodeName.Compare("Duration") == 0)
		{
			dataOut.nDuration = atoi(strNodeVal);
		}
		else if (strNodeName.Compare("ClipType") == 0)
		{
			dataOut.nClipType = atoi(strNodeVal);
		}
		else if (strNodeName.Compare("TCType") == 0)
		{
			dataOut.nTCType = atoi(strNodeVal);
		}
	}

	return bRet;
}
