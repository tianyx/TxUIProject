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
	BOOL InitLayout();
	void Clear();
	//root format like directory. ".\\folder1", "."means root, NULL means root
	BOOL GoToPath(LPCTSTR szRoot = NULL);
	//key format like "edoc_main"
	BOOL GoIntoKey(LPCTSTR szKey);

	BOOL OutofKey();

	//
	CTxStrConvert GetElemVal(LPCTSTR szKey);
	CTxStrConvert GetAttribVal(LPCTSTR szKey, LPCTSTR szAttribKey);
	//BOOL AddPath(LPCTSTR szKey);
	BOOL SetElemVal(LPCTSTR szKey, CTxStrConvert& valIn);
	BOOL SetAttribVal(LPCTSTR szKey, LPCTSTR szAttribKey, CTxStrConvert& valIn );

	BOOL GetSubNodeString(LPCTSTR szRoot, CTxParamString& subParamOut);
	BOOL SetSubNodeString(LPCTSTR szRoot, CTxParamString& subParamIn);

	//must call this if what write back to m_strRealString after set value
	BOOL UpdateData(BOOL bToReal = TRUE);

private:
	ST_MARKITEM* FindNode(VECSTRINGS& vPaths);
private:
	BOOL m_bInited;
	CString m_strRealString;
	VECSTRINGS m_vecCurrPath;
public:
	ST_MARKITEM m_kvData;

};

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