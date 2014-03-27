// XMLOp.cpp: implementation of the CXMLOp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XMLOp.h"
//#include "RuntimeClassFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CXMLOp::CXMLOp()
{
	m_bRootIsReady = FALSE;
	m_ReadDecNum = 0;
//	AfxMessageBox("New CXMLOp");
	OleInitialize(NULL);
}

CXMLOp::~CXMLOp()
{
	if (Root)
	{
		Root.Release();
	}
	if (CurrentNode)
	{
		CurrentNode.Release();
	}
	if (XMLDocument)
	{
		XMLDocument.Release();
	}
	if (CurRoot)
	{
		CurRoot.Release();
	}
	
	m_strXML = "";
}


BOOL CXMLOp::AnalysisXml(CString strXml)
{
	CString str = m_strXML;
// 	MSXML2::IXMLDOMElementPtr Node = FindNodeByNodeName(XMLDocument->documentElement->childNodes,"Data");
// 	CString strName = (char*)(_bstr_t)Node->nodeName;
	AfxMessageBox(m_strXML);
	return TRUE;
}

CString CXMLOp::GetXMLAsString()
{
	if (!m_strXML.IsEmpty())
	{
		return m_strXML;
	}
	return (LPCTSTR)XMLDocument->xml;
}

BOOL CXMLOp::LoadXMLFromString(CString strXML)
{
	if (strXML.IsEmpty())
	{
		return FALSE;
	}
	if(XMLDocumentIsValidate())
	{
		CleanXMLDocument();
	}
	if (XMLDocument.CreateInstance(__uuidof(MSXML2::DOMDocument)) != S_OK)
	{
		return FALSE;
	}
	try
	{
		XMLDocument->loadXML(_bstr_t(strXML));
		m_strXML = strXML;
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CXMLOp::CreateUpdateConnectRoot(CString& strErr)
{
	if(XMLDocumentIsValidate())
	{
		strErr.Format("XML对象已经存在");
		return FALSE;
	}
	if (XMLDocument.CreateInstance(__uuidof(MSXML2::DOMDocument)) != S_OK)
	{
		strErr.Format("创建XML对象出错");
		return FALSE;
	}
	Root = XMLDocument->createElement(L"UpDateConnection");
	XMLDocument->documentElement = Root;
	CurrentNode = Root;
	m_strXML = (LPCTSTR)XMLDocument->xml;
	m_bRootIsReady = TRUE;
	return TRUE;
}

BOOL CXMLOp::CreateOperationRoot(CString& strErr)
{
	if(XMLDocumentIsValidate())
	{
		strErr.Format("XML对象已经存在");
		return FALSE;
	}
	if (XMLDocument.CreateInstance(__uuidof(MSXML2::DOMDocument)) != S_OK)
	{
		strErr.Format("创建XML对象出错");
		return FALSE;
	}
	Root = XMLDocument->createElement(L"TaskOperation");
	XMLDocument->documentElement = Root;
	CurrentNode = Root;
	m_strXML = (LPCTSTR)XMLDocument->xml;
	m_bRootIsReady = TRUE;
	return TRUE;
}

BOOL CXMLOp::CreateOperationReRoot(CString& strErr)
{
	if(XMLDocumentIsValidate())
	{
		strErr.Format("XML对象已经存在");
		return FALSE;
	}
	if (XMLDocument.CreateInstance(__uuidof(MSXML2::DOMDocument)) != S_OK)
	{
		strErr.Format("创建XML对象出错");
		return FALSE;
	}
	Root = XMLDocument->createElement(L"TaskOperation_Return");
	XMLDocument->documentElement = Root;
	CurrentNode = Root;
	m_strXML = (LPCTSTR)XMLDocument->xml;
	m_bRootIsReady = TRUE;
	return TRUE;
}

BOOL CXMLOp::CreateBroadCastRoot(CString& strErr)
{
	if(XMLDocumentIsValidate())
	{
		strErr.Format("XML对象已经存在");
		return FALSE;
	}
	if (XMLDocument.CreateInstance(__uuidof(MSXML2::DOMDocument)) != S_OK)
	{
		strErr.Format("创建XML对象出错");
		return FALSE;
	}
	Root = XMLDocument->createElement(L"TaskBroadCast");
	XMLDocument->documentElement = Root;
	CurrentNode = Root;
	m_strXML = (LPCTSTR)XMLDocument->xml;
	m_bRootIsReady = TRUE;
	return TRUE;
}

BOOL CXMLOp::CreateRoot(CString strRoot,CString& strErr)
{
	strRoot.TrimLeft();
	strRoot.TrimRight();
	if(XMLDocumentIsValidate())
	{
		strErr.Format("XML对象已经存在");
		return FALSE;
	}
	if (XMLDocument.CreateInstance(__uuidof(MSXML2::DOMDocument)) != S_OK)
	{
		strErr.Format("创建XML对象出错");
		return FALSE;
	}
	Root = XMLDocument->createElement(_bstr_t(strRoot));
	XMLDocument->documentElement = Root;
	CurrentNode = Root;
	m_strXML = (LPCTSTR)XMLDocument->xml;
	m_bRootIsReady = TRUE;
	return TRUE;
}

BOOL CXMLOp::SetCurrentNode(CString strNode)
{
	if (!XMLDocumentIsValidate())
	{
		return FALSE;
	}
	strNode.TrimLeft();
	strNode.TrimRight();
	if (!NodeExists(strNode))
    {
		return FALSE;
    }
	if (strNode.CompareNoCase("Root") == 0)
	{
		CurrentNode = Root;
	}
	else
	{
		MSXML2::IXMLDOMElementPtr Node;
		Node = FindNodeByNodeName(strNode,XMLDocument->documentElement->childNodes);
		CurrentNode = Node;
	}
	return TRUE;
}

BOOL CXMLOp::AppendNode(MSXML2::IXMLDOMElementPtr NodeTemp,CString& strErr,CString strParentNode /* =  */)
{
	if (!XMLDocumentIsValidate() || NodeTemp == NULL)
	{
		strErr.Format("XML对象不存在");
		return FALSE;
	}
	MSXML2::IXMLDOMElementPtr ParentNode;
	if(strParentNode.IsEmpty())
	{
		CurrentNode->appendChild(NodeTemp);
	}
	else if (strParentNode.CompareNoCase("Root") == 0)
	{
		Root->appendChild(NodeTemp);
	}
	else
	{
		ParentNode = FindNodeByNodeName(strParentNode,XMLDocument->documentElement->childNodes);
		if (ParentNode)
		{
			ParentNode->appendChild(NodeTemp);
		}
		else
		{
			strErr.Format("父节点不存在");
			return FALSE;
		}
	}
	m_strXML = (LPCTSTR)XMLDocument->xml;
	return TRUE;
}

BOOL CXMLOp::CreateNode(CString strNode,CString& strErr,CString strParentNode)
{
	if (!XMLDocumentIsValidate())
	{
		strErr.Format("XML对象不存在");
		return FALSE;
	}
	strNode.TrimLeft();
	strNode.TrimRight();
	MSXML2::IXMLDOMElementPtr Node;
	MSXML2::IXMLDOMElementPtr ParentNode;
    if (NodeExists(strNode))
    {
		strErr.Format("该节点已经存在");
		return FALSE;
    }
	Node = XMLDocument->createElement(_bstr_t(strNode));
	if(strParentNode.IsEmpty())
	{
		CurrentNode->appendChild(Node);
	}
	else if (strParentNode.CompareNoCase("Root") == 0)
	{
		Root->appendChild(Node);
	}
	else
	{
		ParentNode = FindNodeByNodeName(strParentNode,XMLDocument->documentElement->childNodes);
		if (ParentNode)
		{
			ParentNode->appendChild(Node);
		}
		else
		{
			strErr.Format("父节点不存在");
			return FALSE;
		}
	}
	m_strXML = (LPCTSTR)XMLDocument->xml;
	return TRUE;
}

CString CXMLOp::GetRootNodeName()
{
	return (char*)(_bstr_t)Root->nodeName;
}

_bstr_t CXMLOp::GetFistTagName()
{
	if (XMLDocument)
	{
		return XMLDocument->documentElement ? XMLDocument->documentElement->tagName : "";
	}
	return "";
}

CString CXMLOp::GetCurrentNodeName()
{
	return (char*)(_bstr_t)CurrentNode->nodeName;
}

BOOL CXMLOp::SetAttribute(CString strAttr,CString strValue,CString& strErr,CString strNode)
{
	if (!XMLDocumentIsValidate())
	{
		strErr.Format("XML对象不存在");
		return FALSE;
	}
	if (strValue.IsEmpty())
	{
		TRACE("SetAttribute Value值为空");
		return FALSE;
	}
// 	strAttr.TrimRight();
// 	strAttr.TrimLeft();
// 	strValue.TrimRight();
// 	strValue.TrimLeft();
// 	strNode.TrimLeft();
// 	strNode.TrimRight();
	if (strNode.IsEmpty())
	{
		if(AttributeExists(strAttr,GetCurrentNodeName()))
		{
			strAttr.Format("该属性已经存在");
			return FALSE;
		}
		CurrentNode->setAttribute(_bstr_t(strAttr),_bstr_t(strValue));
	}
	else
	{
		if (!NodeExists(strNode))
		{
			strErr.Format("父节点不存在");
			return FALSE;
		}
		MSXML2::IXMLDOMElementPtr Node = FindNodeByNodeName(strNode,XMLDocument->documentElement->childNodes);
		CString strParentNodeName = (char*)(_bstr_t)Node->nodeName;
		if (AttributeExists(strAttr,strParentNodeName))
		{
			strErr.Format("该属性已存在");
			return FALSE;
		}
		Node->setAttribute(_bstr_t(strAttr),_bstr_t(strValue));
	}
	m_strXML = (LPCTSTR)XMLDocument->xml;
	return TRUE;
}

CString CXMLOp::GetMsgID(CString strMsg /* =  */)
{
// 	CString strErr = "";
// 	CString strRet = "";
// 	if (strMsg.IsEmpty())
// 	{
// 		GetAttribute("MsgID",strRet,strErr,"TaskInfo");
// 	}
// 	else
// 	{
// 		LoadXMLFromString(strMsg);
// 		GetAttribute("MsgID",strRet,strErr,"TaskInfo");
// 	}
// 	return strRet;

	CString strErr = "";
	CString strRet = "";
	int nRet = 0;
	if (!strMsg.IsEmpty())
	{
		LoadXMLFromString(strMsg);
	}
	nRet = GetAttribute("MsgID",strRet,strErr,"TaskInfo");
	if (!nRet)
	{
		nRet = GetAttribute("MsgID",strRet,strErr,"Info");
	}
	return nRet ? strRet : "";
}

CString CXMLOp::GetTskID(CString strMsg /* =  */)
{
	CString strErr = "";
	CString strRet = "";
	int nRet = 0;
	if (!strMsg.IsEmpty())
	{
		LoadXMLFromString(strMsg);
	}
	nRet = GetAttribute("TskID",strRet,strErr,"TaskData");
	if (!nRet)
	{
		nRet = GetAttribute("TskID",strRet,strErr,"Info");
		if (!nRet)
		{
			nRet = GetAttribute("TskID",strRet,strErr,"TskInfo");
		}
	}
	return nRet ? strRet : "";
}

BOOL CXMLOp::GetAttribute(CString strAttr,CString& strValue,CString& strErr,CString strNode /* =  */)
{
	if (!XMLDocumentIsValidate())
	{
		strErr.Format("XML对象不存在 ");
		return FALSE;
	}
	_variant_t v;
	VariantInit(&v);
	strAttr.TrimLeft();
	strAttr.TrimRight();
	strNode.TrimRight();
	strNode.TrimLeft();
	if (strNode.IsEmpty())
	{
		v = CurrentNode->getAttribute(_bstr_t(strAttr));
		if (v.vt == VT_NULL)
		{
			strErr.Format("该属性不存在");
			VariantClear(&v);
			return FALSE;
		}
		strValue = v.bstrVal;
	}
	else
	{
		MSXML2::IXMLDOMElementPtr Node = NULL;//wangjun
		Node = FindNodeByNodeName(strNode,XMLDocument->documentElement->childNodes);
		if (Node == NULL)
		{
			strErr.Format("该节点不存在");
			return FALSE;
		}
		v = Node->getAttribute(_bstr_t(strAttr));
		if (v.vt == VT_NULL)
		{
			strErr.Format("该属性不存在");
			VariantClear(&v);
			return FALSE;
		}
		strValue = v.bstrVal;
	}
	VariantClear(&v);
	return TRUE;
}

BOOL CXMLOp::AttributeExists(CString strAttr,CString strNode)
{
	strAttr.TrimLeft();
	strAttr.TrimRight();
	strNode.TrimLeft();
	strNode.TrimRight();
	if (strAttr.IsEmpty() || strNode.IsEmpty())
	{
		return FALSE;
	}
	if (!NodeExists(strNode))
	{
		return FALSE;
	}
	CString strValue,strErr;
	if(!GetAttribute(strAttr,strValue,strErr,strNode))
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CXMLOp::NodeExists(CString strNode)
{
	strNode.TrimLeft();
	strNode.TrimRight();
	MSXML2::IXMLDOMElementPtr Node;
	Node = FindNodeByNodeName(strNode,XMLDocument->documentElement->childNodes);
	if (Node)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CXMLOp::XMLDocumentIsValidate()
{
	if (XMLDocument)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CXMLOp::CleanXMLDocument()
{
	if (Root)
	{
		Root.Release();
	}
	if (CurrentNode)
	{
		CurrentNode.Release();
	}
	if (XMLDocument)
	{
		XMLDocument.Release();
	}
	m_strXML = "";
	m_bRootIsReady = FALSE;
	return TRUE;
}

MSXML2::IXMLDOMNodePtr CXMLOp::FindNodeByNodeName(CString strNode,MSXML2::IXMLDOMNodeListPtr pNodeList)
{
	if (pNodeList == NULL)
	{
		pNodeList = XMLDocument->documentElement->childNodes;
	}
	strNode.TrimLeft();
	strNode.TrimRight();
	if (strNode.IsEmpty())
	{
		return NULL;
	}
	MSXML2::IXMLDOMNodePtr Node;
	int N = pNodeList->Getlength();
	for (int i = 0 ; i < pNodeList->Getlength() ; i ++)
	{
		if (Node)
		{
			break;
		}
		Node = pNodeList->Getitem(i);
		CString str = (LPCTSTR)Node->GetnodeName();
		if (str.CompareNoCase(strNode) == 0)
		{
			break;
		}
		else
		{
			Node = FindNodeByNodeName(strNode,Node->GetchildNodes());
		}
	}
	return Node;
}

BOOL CXMLOp::SaveXML(CString strPath)
{
	BOOL bRe = TRUE;
	HRESULT hr = E_FAIL;
	try
	{
		hr = XMLDocument->save(_bstr_t(strPath));
		if (hr != S_OK)
		{
			bRe = FALSE;
		}
	}
	catch(...)
	{
        bRe = FALSE;
	}
	return bRe;
}

BOOL CXMLOp::SetCurNodeValue(CString strValue)
{
	try
	{
		CurrentNode->Puttext(_bstr_t(strValue));
	}
	catch (...)
	{
		return FALSE;
	}
    
	return TRUE;
}

BOOL CXMLOp::CreateCurNodeAndAttribute(CString strNode,CString& strErr,CString strParentNode,CString strAttr,CString strValue)
{
    if (!XMLDocumentIsValidate())
	{
		strErr.Format("XML对象不存在");
		return FALSE;
	}
	strNode.TrimLeft();
	strNode.TrimRight();
	MSXML2::IXMLDOMElementPtr Node;
	MSXML2::IXMLDOMElementPtr ParentNode;
    
	Node = XMLDocument->createElement(_bstr_t(strNode));
	if(strAttr.GetLength() > 0)
	{
		Node->setAttribute(_bstr_t(strAttr),_bstr_t(strValue));
	}
	if(strParentNode.IsEmpty())
	{
		CurrentNode->appendChild(Node);
	}
	else if (strParentNode.CompareNoCase("Root") == 0)
	{
		Root->appendChild(Node);
	}
	else
	{
		ParentNode = FindNodeByNodeName(strParentNode,XMLDocument->documentElement->childNodes);
		if (ParentNode)
		{
			ParentNode->appendChild(Node);
		}
		else
		{
			strErr.Format("父节点不存在");
			return FALSE;
		}
	}
	m_strXML = (LPCTSTR)XMLDocument->xml;

	CurrentNode = Node;
	return TRUE;
}

BOOL CXMLOp::SetCurNodeAttribute(CString strAttr,CString strValue)
{
	try
	{
        CurrentNode->setAttribute(_bstr_t(strAttr),_bstr_t(strValue));
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CXMLOp::CreateCurNodeAndSetValue(CString strNode, CString &strErr,CString strValue)
{
    if (!XMLDocumentIsValidate())
	{
		strErr.Format("XML对象不存在");
		return FALSE;
	}
	strNode.TrimLeft();
	strNode.TrimRight();
	MSXML2::IXMLDOMElementPtr Node;
	MSXML2::IXMLDOMElementPtr ParentNode;

	ParentNode = CurrentNode;
    
	Node = XMLDocument->createElement(_bstr_t(strNode));
	if(strValue.GetLength() > 0)
		Node->Puttext(_bstr_t(strValue));
	if(ParentNode != NULL)
	    ParentNode->appendChild(Node);
	else
	{
		strErr.Format("当前节点为空!");
		return FALSE;
	}

	CurrentNode = Node;
	
	m_strXML = (LPCTSTR)XMLDocument->xml;
	return TRUE;
}

BOOL CXMLOp::CreateCurRootNodeAndSetValue(CString strNode, CString &strErr,CString strValue)
{
    if (!XMLDocumentIsValidate())
	{
		strErr.Format("XML对象不存在");
		return FALSE;
	}
	strNode.TrimLeft();
	strNode.TrimRight();
	MSXML2::IXMLDOMElementPtr Node;
	MSXML2::IXMLDOMElementPtr ParentNode;
	
	ParentNode = CurRoot;
    
	Node = XMLDocument->createElement(_bstr_t(strNode));
	if(strValue.GetLength() > 0)
		Node->Puttext(_bstr_t(strValue));
	if(ParentNode != NULL)
		ParentNode->appendChild(Node);
	else
	{
		strErr.Format("当前节点为空!");
		return FALSE;
	}
	
	CurrentNode = Node;
	
	m_strXML = (LPCTSTR)XMLDocument->xml;
	return TRUE;
}

void CXMLOp::SetCurrentRoot()
{
    CurRoot = CurrentNode;
}

BOOL CXMLOp::AddDetectToXml(CString DetectName,tagDetectPara DetParam)
{
	if (!XMLDocumentIsValidate())
	{
		return FALSE;
	}

	MSXML2::IXMLDOMElementPtr DetectParamNode;
	MSXML2::IXMLDOMElementPtr VideoParamNode;
	MSXML2::IXMLDOMElementPtr AudioParamNode;
	MSXML2::IXMLDOMElementPtr Node;

	DetectParamNode = XMLDocument->createElement(_bstr_t("DetectParam"));
	DetectParamNode->setAttribute(_bstr_t("DetectName"),_bstr_t(DetectName));

	Root->appendChild(DetectParamNode);

	VideoParamNode = XMLDocument->createElement(_bstr_t("VideoParam"));
	DetectParamNode->appendChild(VideoParamNode);
	AudioParamNode = XMLDocument->createElement(_bstr_t("AudioParam"));
	DetectParamNode->appendChild(AudioParamNode);

	Node = XMLDocument->createElement(_bstr_t("TOP_SAFE_SECTION"));
	Node->Puttext(_bstr_t(DetParam.TOP_SAFE_SECTION));
	VideoParamNode->appendChild(Node);

	Node = XMLDocument->createElement(_bstr_t("BOTTOM_SAFE_SECTION"));
	Node->Puttext(_bstr_t(DetParam.BOTTOM_SAFE_SECTION));
	VideoParamNode->appendChild(Node);

	Node = XMLDocument->createElement(_bstr_t("LEFT_SAFE_SECTION"));
	Node->Puttext(_bstr_t(DetParam.LEFT_SAFE_SECTION));
	VideoParamNode->appendChild(Node);

	Node = XMLDocument->createElement(_bstr_t("RIGHT_SAFE_SECTION"));
	Node->Puttext(_bstr_t(DetParam.RIGHT_SAFE_SECTION));
	VideoParamNode->appendChild(Node);

	Node = XMLDocument->createElement(_bstr_t("BLACK_SCENE_THRESHOLD"));
	Node->Puttext(_bstr_t(DetParam.BLACK_SCENE_THRESHOLD));
	VideoParamNode->appendChild(Node);
	
	Node = XMLDocument->createElement(_bstr_t("COLOR_SCENE_PERCENTAGE"));
	Node->Puttext(_bstr_t(DetParam.COLOR_SCENE_PERCENTAGE));
	VideoParamNode->appendChild(Node);

    Node = XMLDocument->createElement(_bstr_t("COLOR_STRIP_PERCENTAGE"));
	Node->Puttext(_bstr_t(DetParam.COLOR_STRIP_PERCENTAGE));
	VideoParamNode->appendChild(Node);
	
	Node = XMLDocument->createElement(_bstr_t("STATIC_FRAME_PERCENTAGE"));
	Node->Puttext(_bstr_t(DetParam.STATIC_FRAME_PERCENTAGE));
	VideoParamNode->appendChild(Node);

	Node = XMLDocument->createElement(_bstr_t("LINE_COUNTS_FOR_STRIP_DETECT"));
	Node->Puttext(_bstr_t(DetParam.LINE_COUNTS_FOR_STRIP_DETECT));
	VideoParamNode->appendChild(Node);
	
	Node = XMLDocument->createElement(_bstr_t("AMBIT_DEEMED_TO_SAME_PIXEL"));
	Node->Puttext(_bstr_t(DetParam.AMBIT_DEEMED_TO_SAME_PIXEL));
	VideoParamNode->appendChild(Node);

	Node = XMLDocument->createElement(_bstr_t("UNDULATE_AMBIT_OF_Y"));
	Node->Puttext(_bstr_t(DetParam.UNDULATE_AMBIT_OF_Y));
	VideoParamNode->appendChild(Node);
	
	Node = XMLDocument->createElement(_bstr_t("UNDULATE_AMBIT_OF_U"));
	Node->Puttext(_bstr_t(DetParam.UNDULATE_AMBIT_OF_U));
	VideoParamNode->appendChild(Node);

	Node = XMLDocument->createElement(_bstr_t("UNDULATE_AMBIT_OF_V"));
	Node->Puttext(_bstr_t(DetParam.UNDULATE_AMBIT_OF_V));
	VideoParamNode->appendChild(Node);

	Node = XMLDocument->createElement(_bstr_t("AUDIO_Low_THRESHOLD"));
	Node->Puttext(_bstr_t(DetParam.AUDIO_LOW_THRESHOLD));
	AudioParamNode->appendChild(Node);

	Node = XMLDocument->createElement(_bstr_t("AUDIO_Low_PERIOD"));
	Node->Puttext(_bstr_t(DetParam.AUDIO_LOW_PERIOD));
	AudioParamNode->appendChild(Node);

	Node = XMLDocument->createElement(_bstr_t("AUDIO_High_THRESHOLD"));
	Node->Puttext(_bstr_t(DetParam.AUDIO_HIGH_THRESHOLD));
	AudioParamNode->appendChild(Node);

	Node = XMLDocument->createElement(_bstr_t("AUDIO_High_PERIOD"));
	Node->Puttext(_bstr_t(DetParam.AUDIO_HIGH_PERIOD));
	AudioParamNode->appendChild(Node);

	Node = XMLDocument->createElement(_bstr_t("AUDIO_Mute_THRESHOLD"));
	Node->Puttext(_bstr_t(DetParam.AUDIO_MUTE_THRESHOLD));
	AudioParamNode->appendChild(Node);

	Node = XMLDocument->createElement(_bstr_t("AUDIO_Mute_PERIOD"));
	Node->Puttext(_bstr_t(DetParam.AUDIO_MUTE_PERIOD));
	AudioParamNode->appendChild(Node);

	Node = XMLDocument->createElement(_bstr_t("LOUDNESS_LOW_THRESHOLD"));
	Node->Puttext(_bstr_t(DetParam.LOUDNESS_LOW_THRESHOLD));
	AudioParamNode->appendChild(Node);
	
	Node = XMLDocument->createElement(_bstr_t("LOUDNESS_HIGH_THRESHOLD"));
	Node->Puttext(_bstr_t(DetParam.LOUDNESS_HIGH_THRESHOLD));
	AudioParamNode->appendChild(Node);

	m_strXML = (LPCTSTR)XMLDocument->xml;

    return TRUE;
}

bool CXMLOp::GetFirstDetectParam(CString &strDecName,tagDetectPara& DetParam,CString& strErr)
{
    if (!XMLDocumentIsValidate())
	{
		strErr.Format("XML对象不存在 ");
		return FALSE;
	}

	CurNodeList = XMLDocument->documentElement->childNodes;

	CurRoot = FindNodeByNodeName("DetectParam",Root);

	MSXML2::IXMLDOMElementPtr RootNode = NULL;
	MSXML2::IXMLDOMNodePtr VideoNode = NULL;
	MSXML2::IXMLDOMNodePtr AudioNode = NULL;
	MSXML2::IXMLDOMNodePtr Node = NULL;
	int N = CurNodeList->Getlength();
	if(N <= 0)
		return false;

	_variant_t v;
	VariantInit(&v);

	m_ReadDecNum = 0;
	RootNode = CurNodeList->Getitem(0);
	
	v = RootNode->getAttribute(_bstr_t("DetectName"));
	if (v.vt == VT_NULL)
	{
		strErr.Format("该属性不存在");
		VariantClear(&v);
		return FALSE;
	}

	//得到算法名称
	strDecName = v.bstrVal;

	VideoNode = RootNode->selectSingleNode(_bstr_t("VideoParam"));

	Node = VideoNode->selectSingleNode(_bstr_t("TOP_SAFE_SECTION"));
	Node->get_nodeTypedValue(&v);

	if (v.vt == VT_NULL)
	{
		strErr.Format("TOP_SAFE_SECTION节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.TOP_SAFE_SECTION = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("BOTTOM_SAFE_SECTION"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("BOTTOM_SAFE_SECTION节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.BOTTOM_SAFE_SECTION = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("LEFT_SAFE_SECTION"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("LEFT_SAFE_SECTION节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.LEFT_SAFE_SECTION = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("RIGHT_SAFE_SECTION"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("RIGHT_SAFE_SECTION节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.RIGHT_SAFE_SECTION = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("BLACK_SCENE_THRESHOLD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("BLACK_SCENE_THRESHOLD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.BLACK_SCENE_THRESHOLD = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("COLOR_SCENE_PERCENTAGE"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("COLOR_SCENE_PERCENTAGE节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.COLOR_SCENE_PERCENTAGE = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("COLOR_STRIP_PERCENTAGE"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("COLOR_STRIP_PERCENTAGE节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.COLOR_STRIP_PERCENTAGE = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("STATIC_FRAME_PERCENTAGE"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("STATIC_FRAME_PERCENTAGE节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.STATIC_FRAME_PERCENTAGE = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("LINE_COUNTS_FOR_STRIP_DETECT"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("LINE_COUNTS_FOR_STRIP_DETECT节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.LINE_COUNTS_FOR_STRIP_DETECT = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("AMBIT_DEEMED_TO_SAME_PIXEL"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("AMBIT_DEEMED_TO_SAME_PIXEL节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.AMBIT_DEEMED_TO_SAME_PIXEL = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("UNDULATE_AMBIT_OF_Y"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("UNDULATE_AMBIT_OF_Y节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.UNDULATE_AMBIT_OF_Y = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("UNDULATE_AMBIT_OF_U"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("UNDULATE_AMBIT_OF_U节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.UNDULATE_AMBIT_OF_U = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("UNDULATE_AMBIT_OF_V"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("UNDULATE_AMBIT_OF_V节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.UNDULATE_AMBIT_OF_V = v.bstrVal;

	//音频配置
	AudioNode = RootNode->selectSingleNode(_bstr_t("AudioParam"));
	if(AudioNode == NULL)
	{
		return FALSE;
	}

	Node = AudioNode->selectSingleNode(_bstr_t("AUDIO_Low_THRESHOLD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("AUDIO_Low_THRESHOLD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.AUDIO_LOW_THRESHOLD = v.bstrVal;

	Node = AudioNode->selectSingleNode(_bstr_t("AUDIO_Low_PERIOD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("AUDIO_Low_PERIOD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.AUDIO_LOW_PERIOD = v.bstrVal;

	Node = AudioNode->selectSingleNode(_bstr_t("AUDIO_High_THRESHOLD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("AUDIO_High_THRESHOLD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.AUDIO_HIGH_THRESHOLD = v.bstrVal;

	Node = AudioNode->selectSingleNode(_bstr_t("AUDIO_High_PERIOD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("AUDIO_High_PERIOD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.AUDIO_HIGH_PERIOD = v.bstrVal;

	Node = AudioNode->selectSingleNode(_bstr_t("AUDIO_Mute_THRESHOLD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("AUDIO_Mute_THRESHOLD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.AUDIO_MUTE_THRESHOLD = v.bstrVal;

	Node = AudioNode->selectSingleNode(_bstr_t("AUDIO_Mute_PERIOD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("AUDIO_Mute_PERIOD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.AUDIO_MUTE_PERIOD = v.bstrVal;

	Node = AudioNode->selectSingleNode(_bstr_t("LOUDNESS_LOW_THRESHOLD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("LOUDNESS_LOW_THRESHOLD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.LOUDNESS_LOW_THRESHOLD = v.bstrVal;
	
	Node = AudioNode->selectSingleNode(_bstr_t("LOUDNESS_HIGH_THRESHOLD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("LOUDNESS_HIGH_THRESHOLD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.LOUDNESS_HIGH_THRESHOLD = v.bstrVal;

    return true;
}

bool CXMLOp::GetNextDetectParam(CString &strDecName, tagDetectPara &DetParam, CString &strErr)
{
    if (!XMLDocumentIsValidate())
	{
		strErr.Format("XML对象不存在 ");
		return FALSE;
	}

	CurNodeList = XMLDocument->documentElement->childNodes;

	CurRoot = FindNodeByNodeName("DetectParam",Root);

	MSXML2::IXMLDOMElementPtr RootNode = NULL;
	MSXML2::IXMLDOMNodePtr VideoNode = NULL;
	MSXML2::IXMLDOMNodePtr AudioNode = NULL;
	MSXML2::IXMLDOMNodePtr Node = NULL;
	int N = CurNodeList->Getlength();
	if(N <= 0)
		return false;

	if((m_ReadDecNum + 1) >= N)
	{
		return false;
	}

	_variant_t v;
	VariantInit(&v);

	m_ReadDecNum++;
	RootNode = CurNodeList->Getitem(m_ReadDecNum);
	if(RootNode == NULL)
		return false;
	
	v = RootNode->getAttribute(_bstr_t("DetectName"));
	if (v.vt == VT_NULL)
	{
		strErr.Format("该属性不存在");
		VariantClear(&v);
		return FALSE;
	}

	//得到算法名称
	strDecName = v.bstrVal;

	VideoNode = RootNode->selectSingleNode(_bstr_t("VideoParam"));

	Node = VideoNode->selectSingleNode(_bstr_t("TOP_SAFE_SECTION"));
	Node->get_nodeTypedValue(&v);

	if (v.vt == VT_NULL)
	{
		strErr.Format("TOP_SAFE_SECTION节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.TOP_SAFE_SECTION = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("BOTTOM_SAFE_SECTION"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("BOTTOM_SAFE_SECTION节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.BOTTOM_SAFE_SECTION = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("LEFT_SAFE_SECTION"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("LEFT_SAFE_SECTION节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.LEFT_SAFE_SECTION = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("RIGHT_SAFE_SECTION"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("RIGHT_SAFE_SECTION节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.RIGHT_SAFE_SECTION = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("BLACK_SCENE_THRESHOLD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("BLACK_SCENE_THRESHOLD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.BLACK_SCENE_THRESHOLD = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("COLOR_SCENE_PERCENTAGE"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("COLOR_SCENE_PERCENTAGE节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.COLOR_SCENE_PERCENTAGE = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("COLOR_STRIP_PERCENTAGE"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("COLOR_STRIP_PERCENTAGE节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.COLOR_STRIP_PERCENTAGE = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("STATIC_FRAME_PERCENTAGE"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("STATIC_FRAME_PERCENTAGE节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.STATIC_FRAME_PERCENTAGE = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("LINE_COUNTS_FOR_STRIP_DETECT"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("LINE_COUNTS_FOR_STRIP_DETECT节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.LINE_COUNTS_FOR_STRIP_DETECT = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("AMBIT_DEEMED_TO_SAME_PIXEL"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("AMBIT_DEEMED_TO_SAME_PIXEL节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.AMBIT_DEEMED_TO_SAME_PIXEL = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("UNDULATE_AMBIT_OF_Y"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("UNDULATE_AMBIT_OF_Y节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.UNDULATE_AMBIT_OF_Y = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("UNDULATE_AMBIT_OF_U"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("UNDULATE_AMBIT_OF_U节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.UNDULATE_AMBIT_OF_U = v.bstrVal;

	Node = VideoNode->selectSingleNode(_bstr_t("UNDULATE_AMBIT_OF_V"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("UNDULATE_AMBIT_OF_V节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.UNDULATE_AMBIT_OF_V = v.bstrVal;

	//音频配置
	AudioNode = RootNode->selectSingleNode(_bstr_t("AudioParam"));
	if(AudioNode == NULL)
	{
		return FALSE;
	}

	Node = AudioNode->selectSingleNode(_bstr_t("AUDIO_Low_THRESHOLD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("AUDIO_Low_THRESHOLD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.AUDIO_LOW_THRESHOLD = v.bstrVal;

	Node = AudioNode->selectSingleNode(_bstr_t("AUDIO_Low_PERIOD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("AUDIO_Low_PERIOD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.AUDIO_LOW_PERIOD = v.bstrVal;

	Node = AudioNode->selectSingleNode(_bstr_t("AUDIO_High_THRESHOLD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("AUDIO_High_THRESHOLD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.AUDIO_HIGH_THRESHOLD = v.bstrVal;

	Node = AudioNode->selectSingleNode(_bstr_t("AUDIO_High_PERIOD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("AUDIO_High_PERIOD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.AUDIO_HIGH_PERIOD = v.bstrVal;

	Node = AudioNode->selectSingleNode(_bstr_t("AUDIO_Mute_THRESHOLD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("AUDIO_Mute_THRESHOLD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.AUDIO_MUTE_THRESHOLD = v.bstrVal;

	Node = AudioNode->selectSingleNode(_bstr_t("AUDIO_Mute_PERIOD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("AUDIO_Mute_PERIOD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.AUDIO_MUTE_PERIOD = v.bstrVal;

	Node = AudioNode->selectSingleNode(_bstr_t("LOUDNESS_LOW_THRESHOLD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("LOUDNESS_LOW_THRESHOLD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.LOUDNESS_LOW_THRESHOLD = v.bstrVal;
	
	Node = AudioNode->selectSingleNode(_bstr_t("LOUDNESS_HIGH_THRESHOLD"));
	Node->get_nodeTypedValue(&v);
	
	if (v.vt == VT_NULL)
	{
		strErr.Format("LOUDNESS_HIGH_THRESHOLD节点不存在");
		VariantClear(&v);
		return FALSE;
	}
    DetParam.LOUDNESS_HIGH_THRESHOLD = v.bstrVal;

    return true;
}
