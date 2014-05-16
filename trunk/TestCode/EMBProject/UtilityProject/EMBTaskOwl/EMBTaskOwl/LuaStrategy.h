#pragma once
#include <vector>
#include "AutoLuaState.h"
#include "StrConvert.h"

using namespace std;

#define toluapartype_int32		1
#define toluapartype_int64		2
#define toluapartype_string		3

#define luaparamkey_playtime TEXT("playtime")
#define luaparamkey_chid	 TEXT("chid")
#define luaparamkey_duration TEXT("duration")
#define luaparamkey_cliptype	 TEXT("cliptype")
#define luaparamkey_callbkobj TEXT("callbkobj")

struct ST_PARAMTOLUA
{
	int nType;
	CString strKey;
	CString strVal;

	ST_PARAMTOLUA()
	{
		nType = toluapartype_int32;
	}

	ST_PARAMTOLUA(int nTypeIn, LPCTSTR strKey, CTxStrConvert valIn)
	{
		nType = nTypeIn;
		strVal = valIn.GetAsString();
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

	BOOL InitStrategyFromString(CString& strStgName, CString& strLuaContentIn, IStrategyRuntimeCallback* pCallbackIn);
	BOOL RunStrategy(LPCTSTR szLuaFuncName, VECPARTOLUA& vparIn, int& nRetVal);
	BOOL InitStrategyFromFile(CString& strStgName, CString& strFilePathIn, IStrategyRuntimeCallback* pCallbackIn);

	IStrategyRuntimeCallback* m_pIRecall;

	CString m_strStgName;
	CString m_strStgContent;
	CAutoLuaState m_aLState;
};
