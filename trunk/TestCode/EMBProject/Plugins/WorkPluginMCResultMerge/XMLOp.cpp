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
//	AfxMessageBox("New CXMLOp");
	CoInitialize(NULL);
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
	m_strXML = " ";
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
	if (XMLDocument.CreateInstance(__uuidof(FCVSXML4::DOMDocument)) != S_OK)
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


BOOL CXMLOp::LoadXMLFromFile(CString strXMLFile)
{
	if (strXMLFile.IsEmpty())
	{
		return FALSE;
	}
	CFileFind FileFind;
	if (!FileFind.FindFile(strXMLFile))
	{
		return FALSE;
	}
	if(XMLDocumentIsValidate())
	{
		CleanXMLDocument();
	}
	//
	HRESULT hr = XMLDocument.CreateInstance(__uuidof(FCVSXML4::DOMDocument));
	if (hr != S_OK)
	{
		::CoInitialize(NULL);
		if (XMLDocument.CreateInstance(__uuidof(FCVSXML4::DOMDocument)) != S_OK)
		{
			return FALSE;
		}
	}
	
	BOOL bRet = FALSE;
	try
	{
		bRet = XMLDocument->load(_bstr_t(strXMLFile));
		m_strXML = (LPCTSTR)XMLDocument->xml;
	}
	catch (...)
	{
		return FALSE;
	}
	return bRet;
}

BOOL CXMLOp::CreateUpdateConnectRoot(CString& strErr)
{
	if(XMLDocumentIsValidate())
	{
		strErr.Format("XML对象已经存在");
		return FALSE;
	}
	if (XMLDocument.CreateInstance(__uuidof(FCVSXML4::DOMDocument)) != S_OK)
	{
		strErr.Format("创建XML对象出错");
		return FALSE;
	}
	FCVSXML4::IXMLDOMProcessingInstructionPtr pPI=NULL; 
	XMLDocument->raw_createProcessingInstruction(_bstr_t(L"xml"),_bstr_t(L"version='1.0' encoding='GB2312'"),&pPI);
	XMLDocument->appendChild(pPI);
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
	if (XMLDocument.CreateInstance(__uuidof(FCVSXML4::DOMDocument)) != S_OK)
	{
		strErr.Format("创建XML对象出错");
		return FALSE;
	}
	FCVSXML4::IXMLDOMProcessingInstructionPtr pPI=NULL; 
	XMLDocument->raw_createProcessingInstruction(_bstr_t(L"xml"),_bstr_t(L"version='1.0' encoding='GB2312'"),&pPI);
	XMLDocument->appendChild(pPI);
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
	if (XMLDocument.CreateInstance(__uuidof(FCVSXML4::DOMDocument)) != S_OK)
	{
		strErr.Format("创建XML对象出错");
		return FALSE;
	}
	FCVSXML4::IXMLDOMProcessingInstructionPtr pPI=NULL; 
	XMLDocument->raw_createProcessingInstruction(_bstr_t(L"xml"),_bstr_t(L"version='1.0' encoding='GB2312'"),&pPI);
	XMLDocument->appendChild(pPI);
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
	if (XMLDocument.CreateInstance(__uuidof(FCVSXML4::DOMDocument)) != S_OK)
	{
		strErr.Format("创建XML对象出错");
		return FALSE;
	}
	FCVSXML4::IXMLDOMProcessingInstructionPtr pPI=NULL; 
	XMLDocument->raw_createProcessingInstruction(_bstr_t(L"xml"),_bstr_t(L"version='1.0' encoding='GB2312'"),&pPI);
	XMLDocument->appendChild(pPI);
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
	if (XMLDocument.CreateInstance(__uuidof(FCVSXML4::DOMDocument)) != S_OK)
	{
		strErr.Format("创建XML对象出错");
		return FALSE;
	}
	FCVSXML4::IXMLDOMProcessingInstructionPtr pPI=NULL; 
	XMLDocument->raw_createProcessingInstruction(_bstr_t(L"xml"),_bstr_t(L"version='1.0' encoding='GB2312'"),&pPI);
	XMLDocument->appendChild(pPI);
	Root = XMLDocument->createElement(_bstr_t(strRoot));
	XMLDocument->documentElement = Root;
	CurrentNode = Root;
	m_strXML = (LPCTSTR)XMLDocument->xml;
	m_bRootIsReady = TRUE;
	return TRUE;
}

FCVSXML4::IXMLDOMElementPtr CXMLOp::CreateRootEx(CString strRoot,CString& strErr)
{
	strRoot.TrimLeft();
	strRoot.TrimRight();
	if(XMLDocumentIsValidate())
	{
		strErr.Format("XML对象已经存在");
		return FALSE;
	}
	if (XMLDocument.CreateInstance(__uuidof(FCVSXML4::DOMDocument)) != S_OK)
	{
		strErr.Format("创建XML对象出错");
		return FALSE;
	}
	FCVSXML4::IXMLDOMProcessingInstructionPtr pPI=NULL; 
	XMLDocument->raw_createProcessingInstruction(_bstr_t(L"xml"),_bstr_t(L"version='1.0' encoding='GB2312'"),&pPI);
	XMLDocument->appendChild(pPI);
	Root = XMLDocument->createElement(_bstr_t(strRoot));
	XMLDocument->documentElement = Root;
	CurrentNode = Root;
	m_strXML = (LPCTSTR)XMLDocument->xml;
	m_bRootIsReady = TRUE;
	return Root;
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
		FCVSXML4::IXMLDOMElementPtr Node;
		Node = FindNodeByNodeName(strNode,XMLDocument->documentElement->childNodes);
		CurrentNode = Node;
	}
	return TRUE;
}

BOOL CXMLOp::SetCurrentNodeEx(FCVSXML4::IXMLDOMElementPtr NodeCurrent)
{
	if (!XMLDocumentIsValidate())
	{
		return FALSE;
	}
	CurrentNode = NodeCurrent;
	return TRUE;
}

BOOL CXMLOp::AppendNode(FCVSXML4::IXMLDOMElementPtr NodeTemp,CString& strErr,CString strParentNode /* =  */)
{
	if (!XMLDocumentIsValidate() || NodeTemp == NULL)
	{
		strErr.Format("XML对象不存在");
		return FALSE;
	}
	FCVSXML4::IXMLDOMElementPtr ParentNode;
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

BOOL CXMLOp::AppendNodeEx(FCVSXML4::IXMLDOMElementPtr NodeTemp,CString& strErr,FCVSXML4::IXMLDOMElementPtr NodeParent)
{
	if (!XMLDocumentIsValidate() || NodeTemp == NULL)
	{
		strErr.Format("XML对象不存在");
		return FALSE;
	}
	NodeParent->appendChild(NodeTemp);
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
	FCVSXML4::IXMLDOMElementPtr Node;
	FCVSXML4::IXMLDOMElementPtr ParentNode;
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

FCVSXML4::IXMLDOMElementPtr CXMLOp::CreateNodeEx(CString strNode,CString& strErr)
{
	if (!XMLDocumentIsValidate())
	{
		strErr.Format("XML对象不存在");
		return FALSE;
	}
	strNode.TrimLeft();
	strNode.TrimRight();
	FCVSXML4::IXMLDOMElementPtr Node;
	Node = XMLDocument->createElement(_bstr_t(strNode));
	m_strXML = (LPCTSTR)XMLDocument->xml;
	return Node;
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

BOOL CXMLOp::SetNodeCDATASection(CString strNode,CString strCDATAValue,CString strErr)
{
	if (!XMLDocumentIsValidate())
	{
		strErr.Format("XML对象不存在");
		return FALSE;
	}
	strCDATAValue.TrimRight();
	strCDATAValue.TrimLeft();
	strNode.TrimLeft();
	strNode.TrimRight();
	if (strNode.IsEmpty())
	{
		return FALSE;
	}
	else
	{
		if (!NodeExists(strNode))
		{
			strErr.Format("父节点不存在");
			return FALSE;
		}
		FCVSXML4::IXMLDOMElementPtr Node = FindNodeByNodeName(strNode,XMLDocument->documentElement->childNodes);
		CString strParentNodeName = (char*)(_bstr_t)Node->nodeName;
		FCVSXML4::IXMLDOMNodePtr   DOMNodePtr; 
		DOMNodePtr = XMLDocument->createCDATASection(_bstr_t(strCDATAValue));
		Node->appendChild(DOMNodePtr);
	}
	m_strXML = (LPCTSTR)XMLDocument->xml;
	return TRUE;
}

BOOL CXMLOp::SetNodeValue(CString strNode,CString strValue,CString& strErr)
{
	if (!XMLDocumentIsValidate())
	{
		strErr.Format("XML对象不存在");
		return FALSE;
	}
	strValue.TrimRight();
	strValue.TrimLeft();
	strNode.TrimLeft();
	strNode.TrimRight();
	if (strNode.IsEmpty())
	{
		return FALSE;
	}
	else
	{
		if (!NodeExists(strNode))
		{
			strErr.Format("父节点不存在");
			return FALSE;
		}
		FCVSXML4::IXMLDOMElementPtr Node = FindNodeByNodeName(strNode,XMLDocument->documentElement->childNodes);
		CString strParentNodeName = (char*)(_bstr_t)Node->nodeName;
		Node->put_text(_bstr_t(strValue));
	}
	m_strXML = (LPCTSTR)XMLDocument->xml;
	return TRUE;
}

BOOL CXMLOp::SetNodeValueEx(FCVSXML4::IXMLDOMElementPtr Node,CString strValue,CString& strErr)
{
	if (!XMLDocumentIsValidate())
	{
		strErr.Format("XML对象不存在");
		return FALSE;
	}
	strValue.TrimRight();
	strValue.TrimLeft();
	//if (!Node)
	if (Node!=NULL)
	{
		return FALSE;
	}
	else
	{
		CString strParentNodeName = (char*)(_bstr_t)Node->nodeName;
		Node->put_text(_bstr_t(strValue));
	}
	m_strXML = (LPCTSTR)XMLDocument->xml;
	return TRUE;
}

BOOL CXMLOp::SetAttributeEx(CString strAttr,CString strValue,CString& strErr,FCVSXML4::IXMLDOMElementPtr Node)
{
	if (!XMLDocumentIsValidate())
	{
		strErr.Format("XML对象不存在");
		return FALSE;
	}
	strAttr.TrimRight();
	strAttr.TrimLeft();
	strValue.TrimRight();
	strValue.TrimLeft();
	//if (!Node)
	if (Node!=NULL)
	{
		strErr.Format("XML节点错误");
		return FALSE;
	}
	Node->setAttribute(_bstr_t(strAttr),_bstr_t(strValue));
	return TRUE;
}

BOOL CXMLOp::SetAttribute(CString strAttr,CString strValue,CString& strErr,CString strNode)
{
	if (!XMLDocumentIsValidate())
	{
		strErr.Format("XML对象不存在");
		return FALSE;
	}
	strAttr.TrimRight();
	strAttr.TrimLeft();
	strValue.TrimRight();
	strValue.TrimLeft();
	strNode.TrimLeft();
	strNode.TrimRight();
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
		FCVSXML4::IXMLDOMElementPtr Node = FindNodeByNodeName(strNode,XMLDocument->documentElement->childNodes);
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
		FCVSXML4::IXMLDOMElementPtr Node;
		Node = FindNodeByNodeName(strNode,XMLDocument->documentElement->childNodes);
		CString str = (LPCTSTR)Node->GetnodeName();
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
	FCVSXML4::IXMLDOMElementPtr Node;
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

FCVSXML4::IXMLDOMNodePtr CXMLOp::FindNodeByNodeName(CString strNode,FCVSXML4::IXMLDOMNodeListPtr pNodeList)
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
	FCVSXML4::IXMLDOMNodePtr Node;
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

BOOL CXMLOp::GetResultInfos(structResult &stResult)
{
	FCVSXML4::IXMLDOMElementPtr ResultInfoNode;
	CString strValue = "";
	CString strErr = "";
	CString strNode = "";
	GetAttribute("TskID",stResult.strTskID,strErr,"TskInfo");
	GetAttribute("SecID",strValue,strErr,"TskInfo");
	stResult.nSecID = _ttoi(strValue);
	GetAttribute("SOM",stResult.strSOM,strErr,"TskInfo");
	GetAttribute("EOM",stResult.strEOM,strErr,"TskInfo");
	GetAttribute("nTotalSectionCount",stResult.strTotalSectionCount,strErr,"TskInfo");
	ResultInfoNode = FindNodeByNodeName("ResultInfo",XMLDocument->documentElement->childNodes);
	if (ResultInfoNode)
	{
		FCVSXML4::IXMLDOMNodeListPtr pNodeList = ResultInfoNode->GetchildNodes();
		FCVSXML4::IXMLDOMNodePtr Node;
		int N = pNodeList->Getlength();
		for (int i = 0 ; i < pNodeList->Getlength() ; i ++)
		{
			Node = pNodeList->Getitem(i);
			if (Node)
			{
				ResultData resultData;
				strValue = "";
				strNode = (LPCTSTR)Node->GetnodeName();
				GetAttribute("DetectType",strValue,strErr,strNode);
				resultData.strDetectType = strValue;
				GetAttribute("DetectTypeName",strValue,strErr,strNode);
				resultData.strDetectTypeName = strValue;
				GetAttribute("FrameStart",strValue,strErr,strNode);
				resultData.strFrameStart.Format("%I64d",/*_atoi64(stResult.strSOM)+*/_atoi64(strValue));
				GetAttribute("FrameEnd",strValue,strErr,strNode);
				resultData.strFrameEnd.Format("%I64d",/*_atoi64(stResult.strSOM)+*/_atoi64(strValue));
				GetAttribute("FrameDuration",strValue,strErr,strNode);
				resultData.strFrameDuration = strValue;
				stResult.vecResultDatas.push_back(resultData);
			}
		}
	}
	return TRUE;
}

BOOL CXMLOp::GetDetectResultInfo(CStringArray& strDetectTypeArr,CStringArray& strTypeNameArr,CStringArray& strStartArr,CStringArray& strEndArr,CStringArray& strDurationArr,CStringArray& strnDurationArr)
{
	FCVSXML4::IXMLDOMElementPtr ResultInfoNode;
	CString strValue = "";
	CString strErr = "";
	CString strNode = "";
	ResultInfoNode = FindNodeByNodeName("ResultInfo",XMLDocument->documentElement->childNodes);
	if (ResultInfoNode)
	{
		FCVSXML4::IXMLDOMNodeListPtr pNodeList = ResultInfoNode->GetchildNodes();
		FCVSXML4::IXMLDOMNodePtr Node;
		int N = pNodeList->Getlength();
		for (int i = 0 ; i < pNodeList->Getlength() ; i ++)
		{
			Node = pNodeList->Getitem(i);
			if (Node)
			{
				strValue = "";
				strNode = (LPCTSTR)Node->GetnodeName();
				GetAttribute("DetectType",strValue,strErr,strNode);
				strDetectTypeArr.Add(strValue);
				GetAttribute("DetectTypeName",strValue,strErr,strNode);
				strTypeNameArr.Add(strValue);
				GetAttribute("FrameStart",strValue,strErr,strNode);
				strStartArr.Add(strValue);
				GetAttribute("FrameEnd",strValue,strErr,strNode);
				strEndArr.Add(strValue);
				GetAttribute("TimeCodeDuration",strValue,strErr,strNode);
				strDurationArr.Add(strValue);
				GetAttribute("FrameDuration",strValue,strErr,strNode);
				strnDurationArr.Add(strValue);
			}
		}
	}
	return TRUE;
}

BOOL CXMLOp::GetDetectFileInfo(CStringArray& strNameArr, CStringArray &strValueArr)
{
	FCVSXML4::IXMLDOMElementPtr ResultInfoNode;
	CString strValue = "";
	CString strErr = "";
	CString strNode = "";
	ResultInfoNode = FindNodeByNodeName("FileInfo",XMLDocument->documentElement->childNodes);
	if (ResultInfoNode)
	{
		FCVSXML4::IXMLDOMNodeListPtr pNodeList = ResultInfoNode->GetchildNodes();
		FCVSXML4::IXMLDOMNodePtr Node;
		int N = pNodeList->Getlength();
		for (int i = 0 ; i < pNodeList->Getlength() ; i ++)
		{
			Node = pNodeList->Getitem(i);
			if (Node)
			{
				strValue = "";
				strNode = (LPCTSTR)Node->GetnodeName();
				GetAttribute("ColName",strValue,strErr,strNode);
				strNameArr.Add(strValue);
				GetAttribute("ColValue",strValue,strErr,strNode);
				strValueArr.Add(strValue);
			}
		}
	}
	return TRUE;
}

BOOL CXMLOp::GetFileDuration(long &lFileDuration)
{
	FCVSXML4::IXMLDOMElementPtr ResultInfoNode;
	CString strValue = "";
	CString strColName = "";
	CString strErr = "";
	CString strNode = "";
	ResultInfoNode = FindNodeByNodeName("FileInfo",XMLDocument->documentElement->childNodes);
	if (ResultInfoNode)
	{
		FCVSXML4::IXMLDOMNodeListPtr pNodeList = ResultInfoNode->GetchildNodes();
		FCVSXML4::IXMLDOMNodePtr Node;
		int N = pNodeList->Getlength();
		for (int i = 0 ; i < pNodeList->Getlength() ; i ++)
		{
			Node = pNodeList->Getitem(i);
			if (Node)
			{
				strColName = "";
				strValue = "";
				strNode = (LPCTSTR)Node->GetnodeName();
				GetAttribute("ColName",strColName,strErr,strNode);
				strColName.TrimLeft();
				strColName.TrimRight();
				if (strColName.Compare("FileDuration") == 0)
				{
					GetAttribute("ColValue",strValue,strErr,strNode);
					strValue.TrimLeft();
					strValue.TrimRight();
					lFileDuration = atol(strValue);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
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