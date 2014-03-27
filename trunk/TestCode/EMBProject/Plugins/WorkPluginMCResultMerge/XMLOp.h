// XMLOp.h: interface for the CXMLOp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XMLOP_H__CB82664E_3C35_4A0C_86C8_B599D9E06F88__INCLUDED_)
#define AFX_XMLOP_H__CB82664E_3C35_4A0C_86C8_B599D9E06F88__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include <vector>
#include <functional>
#include <algorithm>
using namespace std;
#import "..\commondll\msxml4.dll" rename_namespace("FCVSXML4")

using namespace FCVSXML4;

struct ResultData
{
	CString strDetectType;
	CString strDetectTypeName;
	CString strFrameStart;
	CString strFrameEnd;
	CString strFrameDuration;
};

typedef struct _structResult
{
	CString strFileParh;
	CString strTskID;
	int nSecID;
	CString strSOM;
	CString strEOM;
	CString strTotalSectionCount;
	vector<ResultData> vecResultDatas;
	bool operator < (const _structResult &m)const {	return nSecID < m.nSecID;}

}structResult;

class CXMLOp  
{
private:
	FCVSXML4::IXMLDOMDocumentPtr XMLDocument;
	FCVSXML4::IXMLDOMElementPtr Root;
	FCVSXML4::IXMLDOMElementPtr CurrentNode;
	BOOL m_bRootIsReady;
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
	FCVSXML4::IXMLDOMElementPtr CreateRootEx(CString strRoot,CString& strErr);
	BOOL CreateNode(CString strNode,CString& strErr,CString strParentNode = "Root");
	FCVSXML4::IXMLDOMElementPtr CreateNodeEx(CString strNode,CString& strErr);
	BOOL SetAttributeEx(CString strAttr,CString strValue,CString& strErr,FCVSXML4::IXMLDOMElementPtr Node);
	BOOL SetNodeValue(CString strNode,CString strValue,CString& strErr);
	BOOL SetNodeValueEx(FCVSXML4::IXMLDOMElementPtr Node,CString strValue,CString& strErr);
	BOOL SetNodeCDATASection(CString strNode,CString strCDATAValue,CString strErr);
	BOOL AppendNode(FCVSXML4::IXMLDOMElementPtr NodeTemp,CString& strErr,CString strParentNode = "Root");
	BOOL AppendNodeEx(FCVSXML4::IXMLDOMElementPtr NodeTemp,CString& strErr,FCVSXML4::IXMLDOMElementPtr NodeParent);
	BOOL SetCurrentNode(CString strNode = "Root");
	BOOL SetCurrentNodeEx(FCVSXML4::IXMLDOMElementPtr NodeCurrent);
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
	BOOL LoadXMLFromFile(CString strXMLFile);
	_bstr_t GetFistTagName();
	CString GetMsgID(CString strMsg = "");
	CString GetTskID(CString strMsg = "");
	BOOL SaveXML(CString strPath);
	BOOL GetDetectResultInfo(CStringArray& strDetectTypeArr,CStringArray& strTypeNameArr,CStringArray& strStartArr,CStringArray& strEndArr,CStringArray& strDurationArr,CStringArray& strnDurationArr);
	BOOL GetDetectFileInfo(CStringArray& strNameArr,CStringArray& strValueArr);
	BOOL GetFileDuration(long& lFileDuration);
	BOOL GetResultInfos(structResult &stResult);
public:
	FCVSXML4::IXMLDOMNodePtr FindNodeByNodeName(CString strNode,FCVSXML4::IXMLDOMNodeListPtr pNodeList = NULL);
};


#endif // !defined(AFX_XMLOP_H__CB82664E_3C35_4A0C_86C8_B599D9E06F88__INCLUDED_)
