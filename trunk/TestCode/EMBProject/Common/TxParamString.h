#pragma once
#include <vector>
#include <map>
#include "StrConvert.h"
using namespace std;
struct ST_MARKITEM;
typedef map<CString, CString> MAPKEYATTRIB;
typedef map<CString, ST_MARKITEM> MAPKEYVALUE;

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


class CTxParamString 
{

public:
	CTxParamString(LPCTSTR szParamIn);
	CTxParamString(const ST_MARKITEM& kvDataIn);
	virtual ~CTxParamString(void);

	CTxParamString(const CTxParamString& other);
	CTxParamString&  operator =(const CTxParamString& other);
	bool operator <(const CTxParamString& other);
	bool operator ==(const CTxParamString& other);
	operator CString(){return m_strRealString;}
	BOOL InitLayout();
	void Clear();
	//root format like directory. ".\\folder1", "."means root
	BOOL GoToPath(LPCTSTR szRoot);
	//
	CTxStrConvert GetElemVal(LPCTSTR szKey);
	CTxStrConvert GetAttribVal(LPCTSTR szKey, LPCTSTR szAttribKey);
	//BOOL AddPath(LPCTSTR szKey);
	BOOL SetElemVal(LPCTSTR szKey, CTxStrConvert& valIn);
	BOOL SetAttribVal(LPCTSTR szKey, LPCTSTR szAttribKey, CTxStrConvert& valIn );
	//must call this if what write back to m_strRealString after set value
	BOOL UpdateData(BOOL bToReal = TRUE);

private:
	ST_MARKITEM* FindNode(VECSTRINGS& vPaths);
private:
	BOOL m_bInited;
	CString m_strRealString;
	ST_MARKITEM m_kvData;
	VECSTRINGS m_vecCurrPath;
};
