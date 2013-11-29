/********************************************************************
	created:	2013/11/08
	created:	8:11:2013   18:01
	filename: 	TxParamString.h
	author:		tianyx
	
	purpose:	xml string paser
*********************************************************************/
#pragma once
#include <vector>
#include <map>
#include "StrConvert.h"
using namespace std;
struct ST_MARKITEM;
typedef map<CString, CString> MAPKEYATTRIB;
typedef map<CString, ST_MARKITEM> MAPKEYVALUE;


//defined for vc6 STL
#ifdef VC6DEFINE
struct ST_CHILDMAP
{
	MAPKEYVALUE* pChildItem;
	ST_CHILDMAP();

	~ST_CHILDMAP()
	{

	}
	ST_CHILDMAP(ST_CHILDMAP& other);


	ST_CHILDMAP& operator =(const ST_CHILDMAP& other);


	operator MAPKEYVALUE&(){return *pChildItem;}
	ST_MARKITEM& operator [](const CString& key);
//  	MAPKEYVALUE::iterator begin(){return (*pChildItem).begin();}
//  	MAPKEYVALUE::iterator end(){return (*pChildItem).end();}
	void clear();
	//void insert(MAPKEYVALUE::iterator& itb, MAPKEYVALUE::iterator& ite){return (*pChildItem).insert(itb, ite);}
};
struct ST_MARKITEM
{
	BOOL bIsPath;
	CString strKey;
	CString strVal;
	ST_CHILDMAP mapChildItem;
	MAPKEYATTRIB mapAttrib;
public:
	ST_MARKITEM()
	{
		bIsPath = FALSE;
	}
	
	ST_MARKITEM(const ST_MARKITEM& other)
	{
		if (&other == this)
		{
			return;
		}
		bIsPath = other.bIsPath;
		strKey = other.strKey;
		strVal = other.strVal;
		mapChildItem = other.mapChildItem;
		mapAttrib = mapAttrib;
	}
	ST_MARKITEM& operator =(const ST_MARKITEM& other)
	{
		if (&other == this)
		{
			return *this;
		}
		bIsPath = other.bIsPath;
		strKey = other.strKey;
		strVal = other.strVal;
		mapChildItem = other.mapChildItem;
		mapAttrib = mapAttrib;
	}
	
};

#else
struct ST_MARKITEM
{

	BOOL bIsPath;
	CString strKey;
	CString strVal;
	MAPKEYVALUE mapChildItem;
	MAPKEYATTRIB mapAttrib;
	ST_MARKITEM()
	{
		bIsPath = FALSE;
	}
};
#endif // _DEBUG



class CTxParamString 
{

public:
	CTxParamString(LPCTSTR szParamIn = NULL);
	CTxParamString(const ST_MARKITEM& kvDataIn);
	virtual ~CTxParamString(void);

	CTxParamString(const CTxParamString& other);
	CTxParamString&  operator =(const CTxParamString& other);
	bool operator <(const CTxParamString& other);
	bool operator ==(const CTxParamString& other);
	operator CString(){return m_strRealString;}
	BOOL IsEmpty(){return m_strRealString.IsEmpty();}

	void Clear();
	
	//*Description: go into a node, must pass a full path
	//*Input Param: szRoot:root format like directory. ".\\folder1", "."means root, NULL means root
	//*Return Param: not used now
	//*History: 
	BOOL GoToPath(LPCTSTR szRoot = NULL);
	
	
	//*Description: go into a node, pass in a sub-node name of current node.
	//*Input Param: szKey: node name, format like "edoc_main"
	//*Return Param: 
	//*History: 
	BOOL GoIntoKey(LPCTSTR szKey);

	//jump out of current node.
	BOOL OutofKey();

	
	//*Description: get node value
	//*Input Param: szKey: if null, return the current node value, else return value of sub-node of current node 
	//*Return Param: 
	//*History: 
	CTxStrConvert GetElemVal(LPCTSTR szKey);

	
	//*Description: get node attribrate value
	//*Input Param: szKey: sub node name, if null, means current node
	//				szAttribKey: attribrate name, must not be null
	//*Return Param: 
	//*History: 
	CTxStrConvert GetAttribVal(LPCTSTR szKey, LPCTSTR szAttribKey);

	//BOOL AddPath(LPCTSTR szKey);

	
	//*Description: set node value, note that if set node value, you can't set sub-node to this node
	//*Input Param: see GetElemVal
	//*Return Param: 
	//*History: 
	BOOL SetElemVal(LPCTSTR szKey, CTxStrConvert& valIn);

	//*Description: set node attribrate value
	//*Input Param: see GetElemVal
	//*Return Param: 
	//*History: 
	BOOL SetAttribVal(LPCTSTR szKey, LPCTSTR szAttribKey, CTxStrConvert& valIn );

	
	//*Description: return the sub xml string of a node
	//*Input Param: szRoot: full path of a node
	//*Return Param: 
	//*History: 
	BOOL GetSubNodeString(LPCTSTR szRoot, CTxParamString& subParamOut);
	BOOL SetSubNodeString(LPCTSTR szRoot, CTxParamString& subParamIn);

	//
	
	//*Description:write back to the xml string, must call this if what write back to m_strRealString after use Setxxx func
	//*Input Param: not used now
	//*Return Param: 
	//*History: 
	BOOL UpdateData(BOOL bToReal = TRUE);
private:
	BOOL InitLayout();

private:
	ST_MARKITEM* FindNode(VECSTRINGS& vPaths);
private:
	BOOL m_bInited;
	CString m_strRealString;
	VECSTRINGS m_vecCurrPath;
public:
	ST_MARKITEM m_kvData;

};


//template class for auto update struct to xml string 
//type T must implement the function ToString
template<typename T>
struct ST_AUTOUPDATEPARAM : public T
{
	ST_AUTOUPDATEPARAM(CString& strAttach):m_strAttach(strAttach)
	{
		
	}
	virtual ~ST_AUTOUPDATEPARAM()
	{
		T::ToString(m_strAttach);
	}
	
	CString& m_strAttach;
};