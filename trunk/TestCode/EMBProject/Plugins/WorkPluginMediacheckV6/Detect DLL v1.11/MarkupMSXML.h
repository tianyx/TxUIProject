// MarkupMSXML.h: interface for the CMarkupMSXML class.
//
// Markup Release 9.0
// Copyright (C) 1999-2007 First Objective Software, Inc. All rights reserved
// Go to www.firstobject.com for the latest CMarkup and EDOM documentation
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.

#if !defined(AFX_MARKUPMSXML_H__948A2705_9E68_11D2_A0BF_00105A27C570__INCLUDED_)
#define AFX_MARKUPMSXML_H__948A2705_9E68_11D2_A0BF_00105A27C570__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#pragma warning(disable:4996) // suppress VS 2005 deprecated function warnings
#pragma warning(disable:4786)
#endif // _MSC_VER > 1000

#if defined(MARKUP_MSXML4)
#import <msxml4.dll> no_function_mapping
#define MSXMLNS MSXML2
#elif defined(MARKUP_MSXML3)
#import <msxml3.dll> no_function_mapping
#define MSXMLNS MSXML2
#else
#import <msxml.dll>
#define MSXMLNS MSXML
#endif

class CMarkupMSXML  
{
public:
#if defined(MARKUP_MSXML3) || defined(MARKUP_MSXML4)
	MSXMLNS::IXMLDOMDocument2Ptr m_pDOMDoc;
#else
	MSXMLNS::IXMLDOMDocumentPtr m_pDOMDoc;
#endif
	CMarkupMSXML();
	CMarkupMSXML( LPCTSTR szDoc );
	virtual ~CMarkupMSXML();

	static _bstr_t ToBSTR( LPCTSTR pszText );
	static CString FromBSTR( const _bstr_t& bstrText );
	static _variant_t ToVARIANT( LPCTSTR pszText );
	static CString FromVARIANT( const _variant_t& varText );

	// Navigate
	BOOL Load( LPCTSTR szFileName );
	BOOL SetDoc( LPCTSTR szDoc );
	BOOL FindElem( LPCTSTR szName=NULL );
	BOOL FindChildElem( LPCTSTR szName=NULL );
	BOOL IntoElem();
	BOOL OutOfElem();
	void ResetChildPos() { if ( m_pChild ) m_pChild.Release(); };
	void ResetMainPos() { ResetChildPos(); if ( m_pMain ) m_pMain.Release(); };
	void ResetPos() { ResetMainPos(); m_pParent = m_pDOMDoc; };
	BOOL SetMainPosPtr( MSXMLNS::IXMLDOMNodePtr pMain );
	CString GetTagName() const { return x_GetTagName(m_pMain); };
	CString GetChildTagName() const { return x_GetTagName(m_pChild); };
	CString GetData() const { return x_GetData( m_pMain ); };
	CString GetChildData() const { return x_GetData(m_pChild); };
	CString GetAttrib( LPCTSTR szAttrib ) const { return x_GetAttrib( m_pMain, szAttrib ); };
	CString GetChildAttrib( LPCTSTR szAttrib ) const { return x_GetAttrib( m_pChild, szAttrib ); };
	CString GetAttribName( int n ) const;
	int FindNode( int nType=0 );
	int GetNodeType() { return x_GetNodeType( m_pMain ); };
	CString GetError() const { return m_strError; };

	enum MarkupNodeType
	{
		MNT_ELEMENT					= 1,  // 0x01
		MNT_TEXT					= 2,  // 0x02
		MNT_WHITESPACE				= 4,  // 0x04
		MNT_CDATA_SECTION			= 8,  // 0x08
		MNT_PROCESSING_INSTRUCTION	= 16, // 0x10
		MNT_COMMENT					= 32, // 0x20
		MNT_DOCUMENT_TYPE			= 64, // 0x40
		MNT_EXCLUDE_WHITESPACE		= 123,// 0x7b
	};

	// Create
	BOOL Save( LPCTSTR szFileName );
	CString GetDoc() const;
	BOOL AddElem( LPCTSTR szName, LPCTSTR szData=NULL ) { return x_AddElem(szName,szData,false,false); };
	BOOL InsertElem( LPCTSTR szName, LPCTSTR szData=NULL ) { return x_AddElem(szName,szData,true,false); };
	BOOL AddChildElem( LPCTSTR szName, LPCTSTR szData=NULL ) { return x_AddElem(szName,szData,false,true); };
	BOOL InsertChildElem( LPCTSTR szName, LPCTSTR szData=NULL ) { return x_AddElem(szName,szData,true,true); };
	BOOL AddElem( LPCTSTR szName, int nValue ) { return x_AddElem(szName,nValue,false,false); };
	BOOL InsertElem( LPCTSTR szName, int nValue ) { return x_AddElem(szName,nValue,true,false); };
	BOOL AddChildElem( LPCTSTR szName, int nValue ) { return x_AddElem(szName,nValue,false,true); };
	BOOL InsertChildElem( LPCTSTR szName, int nValue ) { return x_AddElem(szName,nValue,true,true); };
	BOOL AddAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_pMain,szAttrib,szValue); };
	BOOL AddChildAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_pChild,szAttrib,szValue); };
	BOOL AddAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_pMain,szAttrib,nValue); };
	BOOL AddChildAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_pChild,szAttrib,nValue); };
	BOOL AddSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,false,false); };
	BOOL InsertSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,true,false); };
	CString GetSubDoc() const { return x_GetSubDoc(m_pMain); };
	BOOL AddChildSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,false,true); };
	BOOL InsertChildSubDoc( LPCTSTR szSubDoc ) { return x_AddSubDoc(szSubDoc,true,true); };
	CString GetChildSubDoc() const { return x_GetSubDoc(m_pChild); };
	void SetDefaultNamespace( LPCTSTR szNamespace ) { m_strDefaultNamespace = szNamespace?szNamespace:_T(""); };
	BOOL AddNode( int nType, LPCTSTR szText ) { return x_AddNode(nType,szText,false); };
	BOOL InsertNode( int nType, LPCTSTR szText ) { return x_AddNode(nType,szText,true); };

	// Modify
	BOOL RemoveElem();
	BOOL RemoveChildElem();
	BOOL RemoveNode();
	BOOL SetAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_pMain,szAttrib,szValue); };
	BOOL SetChildAttrib( LPCTSTR szAttrib, LPCTSTR szValue ) { return x_SetAttrib(m_pChild,szAttrib,szValue); };
	BOOL SetAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_pMain,szAttrib,nValue); };
	BOOL SetChildAttrib( LPCTSTR szAttrib, int nValue ) { return x_SetAttrib(m_pChild,szAttrib,nValue); };
	BOOL SetData( LPCTSTR szData, int nCDATA=0 ) { return x_SetData(m_pMain,szData,nCDATA); };
	BOOL SetChildData( LPCTSTR szData, int nCDATA=0 ) { return x_SetData(m_pChild,szData,nCDATA); };

protected:
	MSXMLNS::IXMLDOMNodePtr m_pParent;
	MSXMLNS::IXMLDOMNodePtr m_pMain;
	MSXMLNS::IXMLDOMNodePtr m_pChild;
	CString m_strError;
	CString m_strDefaultNamespace;

	HRESULT x_CreateInstance();
	BOOL x_ParseError();
	MSXMLNS::IXMLDOMNodePtr x_FindElem( MSXMLNS::IXMLDOMNodePtr pParent, MSXMLNS::IXMLDOMNodePtr pNode, LPCTSTR szPath );
	CString x_GetPath( MSXMLNS::IXMLDOMNodePtr pNode ) const;
	CString x_GetTagName( MSXMLNS::IXMLDOMNodePtr pNode ) const;
	CString x_GetData( MSXMLNS::IXMLDOMNodePtr pNode ) const;
	CString x_GetAttrib( MSXMLNS::IXMLDOMNodePtr pNode, LPCTSTR szAttrib ) const;
	void CMarkupMSXML::x_Insert( MSXMLNS::IXMLDOMNodePtr pParent, MSXMLNS::IXMLDOMNodePtr pNext, MSXMLNS::IXMLDOMNodePtr pNew );
	BOOL x_AddElem( LPCTSTR szName, LPCTSTR szData, BOOL bInsert, BOOL bAddChild );
	BOOL x_AddElem( LPCTSTR szName, int nValue, BOOL bInsert, BOOL bAddChild );
	CString x_GetSubDoc( MSXMLNS::IXMLDOMNodePtr pNode ) const;
	BOOL x_AddSubDoc( LPCTSTR szSubDoc, BOOL bInsert, BOOL bAddChild );
	BOOL x_SetAttrib( MSXMLNS::IXMLDOMNodePtr pNode, LPCTSTR szAttrib, LPCTSTR szValue );
	BOOL x_SetAttrib( MSXMLNS::IXMLDOMNodePtr pNode, LPCTSTR szAttrib, int nValue );
	BOOL x_CreateNode( CString& strNode, int nNodeType, LPCTSTR szText );
	BOOL x_AddNode( int nNodeType, LPCTSTR szText, BOOL bInsert );
	int x_GetNodeType( MSXMLNS::IXMLDOMNodePtr pNode ) const;
	BOOL x_SetData( MSXMLNS::IXMLDOMNodePtr& pNode, LPCTSTR szData, int nCDATA );
};

#endif // !defined(AFX_MARKUPMSXML_H__948A2705_9E68_11D2_A0BF_00105A27C570__INCLUDED_)
