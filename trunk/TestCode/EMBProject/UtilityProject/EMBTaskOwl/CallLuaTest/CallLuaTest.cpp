// CallLuaTest.cpp : �������̨Ӧ�ó������ڵ㡣
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

// Ψһ��Ӧ�ó������

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

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
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
