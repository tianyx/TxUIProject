// CallLuaTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CallLuaTest.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "LuaStrategy.h"

// 唯一的应用程序对象

CWinApp theApp;

using namespace std;


class CTestCall: public IStrategyRuntimeCallback
{
	virtual BOOL OnStrategyRunMessage(int nMsgType, CString& strValIn)
	{
		if (nMsgType == 1)
		{
			printf("\n%s", strValIn);
		}
	}

};


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
	}

	char szbuff[256];
	GetModuleFileNameA(NULL, szbuff, 256);
	std::string wsPath = szbuff;
	size_t nPos = wsPath.rfind('\\');
	CString strLuaFile = wsPath.substr(0, nPos).c_str();
	strLuaFile += TEXT("\\test.lua");

	CTime tmNow(1397126052);
	CString strtm = tmNow.Format(TEXT("%Y-%m-%d %H:%M:%S"));

	CLuaStrategy stg;
	VECPARTOLUA vPar;
	ST_PARAMTOLUA par;
	par.nType = 1;
	par.strVal = "11";
	par.strKey = TEXT("key1");

	vPar.push_back(par);

	par.nType = 1;
	par.strVal = "22";
	par.strKey = TEXT("key2");
	vPar.push_back(par);

	stg.StrategyFromFile(TEXT("test"), strLuaFile);
	stg.RunStrategyUseTable(TEXT("StgCallFunc"), vPar);

	system("pause");
	return nRetCode;
}
