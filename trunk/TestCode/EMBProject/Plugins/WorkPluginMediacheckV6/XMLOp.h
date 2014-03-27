// XMLOp.h: interface for the CXMLOp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLOP_H__CB82664E_3C35_4A0C_86C8_B599D9E06F88__INCLUDED_)
#define AFX_XMLOP_H__CB82664E_3C35_4A0C_86C8_B599D9E06F88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "RuntimeClassFactory.h"
//#include "NetMsgDef.h"

#include "FCVSServerConfig.h"

#import <msxml4.dll>
using namespace MSXML2;


class CXMLOp  
{
private:
	MSXML2::IXMLDOMDocumentPtr XMLDocument;
	MSXML2::IXMLDOMElementPtr Root;
	MSXML2::IXMLDOMElementPtr CurrentNode;
	MSXML2::IXMLDOMElementPtr CurRoot;

	MSXML2::IXMLDOMNodeListPtr CurNodeList;
	BOOL m_bRootIsReady;

	int  m_ReadDecNum;
public:
	CString m_strXML;
public:
	CXMLOp();
	virtual ~CXMLOp();
	BOOL AnalysisXml(CString strXml);
	BOOL CreateUpdateConnectRoot(CString& strErr);
	BOOL CreateOperationRoot(CString& strErr);
	BOOL CreateOperationReRoot(CString& strErr);
	BOOL CreateBroadCastRoot(CString& strErr);
	BOOL CreateRoot(CString strRoot,CString& strErr);
	BOOL CreateNode(CString strNode,CString& strErr,CString strParentNode = "Root");
	BOOL AppendNode(MSXML2::IXMLDOMElementPtr NodeTemp,CString& strErr,CString strParentNode = "Root");
	BOOL SetCurrentNode(CString strNode = "Root");
	CString GetRootNodeName();
	CString GetCurrentNodeName();
	BOOL SetAttribute(CString strAttr,CString strValue,CString& strErr,CString strNode = "");
	BOOL GetAttribute(CString strAttr,CString& strValue,CString& strErr,CString strNode = "");
	BOOL AttributeExists(CString strAttr,CString strNode);
	BOOL NodeExists(CString strNode);
	BOOL XMLDocumentIsValidate();
	BOOL CleanXMLDocument();
	CString GetXMLAsString();
	BOOL LoadXMLFromString(CString strXML);
	_bstr_t GetFistTagName();
	CString GetMsgID(CString strMsg = "");
	CString GetTskID(CString strMsg = "");
	BOOL SaveXML(CString strPath);
public:
	bool GetNextDetectParam(CString &strDecName,tagDetectPara& DetParam,CString& strErr);
	bool GetFirstDetectParam(CString& strDecName,tagDetectPara& DetParam,CString& strErr);
	BOOL AddDetectToXml(CString DetectName,tagDetectPara DetParam);
	void SetCurrentRoot();
	BOOL CreateCurRootNodeAndSetValue(CString strNode, CString &strErr,CString strValue = "");
	BOOL CreateCurNodeAndSetValue(CString strNode, CString &strErr,CString strValue = "");
	BOOL SetCurNodeAttribute(CString strAttr,CString strValue);
	BOOL CreateCurNodeAndAttribute(CString strNode,CString& strErr,CString strParentNode,CString strAttr = "",CString strValue = "");
	BOOL SetCurNodeValue(CString strValue);
	MSXML2::IXMLDOMNodePtr FindNodeByNodeName(CString strNode,MSXML2::IXMLDOMNodeListPtr pNodeList = NULL);
};


#endif // !defined(AFX_XMLOP_H__CB82664E_3C35_4A0C_86C8_B599D9E06F88__INCLUDED_)
