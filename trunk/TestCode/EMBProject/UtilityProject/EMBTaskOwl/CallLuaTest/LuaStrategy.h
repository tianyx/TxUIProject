#pragma once
#include <vector>

using namespace std;

#define toluapartype_int32		1
#define toluapartype_int64		2
#define toluapartype_string		3

struct ST_PARAMTOLUA
{
	int nType;
	CString strKey;
	CString strVal;

	ST_PARAMTOLUA()
	{
		nType = toluapartype_int32;
	}

};

typedef vector<ST_PARAMTOLUA> VECPARTOLUA;

interface IStrategyRuntimeCallback
{
	virtual BOOL OnStrategyRunMessage(int nMsgType, CString& strValIn) = 0;
};

class CLuaStrategy
{
public:
	CLuaStrategy(void);
	~CLuaStrategy(void);

	BOOL StrategyFromFile(CString strStgName, CString strFilePathIn);
	BOOL RunStrategy(LPCTSTR szLuaFuncName, VECPARTOLUA& vparIn);
	BOOL RunStrategyUseTable(LPCTSTR szLuaFuncName, VECPARTOLUA& vparIn);

	IStrategyRuntimeCallback* m_pIRecall;

	CString m_strStgName;
	CString m_strStgContent;
};
