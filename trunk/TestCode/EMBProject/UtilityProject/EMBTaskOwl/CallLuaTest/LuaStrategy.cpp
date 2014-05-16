#include "StdAfx.h"
#include "LuaStrategy.h"
#include "AutoLuaState.h"

//func called by lua
static int StrategyLuaCallFunc (lua_State *L)
{
	int nPar1 = lua_tointeger(L, 1);
	int nType = lua_tointeger(L, 2);
	CString strInfo = lua_tostring(L, 3);
	CLuaStrategy* pObj = (CLuaStrategy*) nPar1;
	if (pObj && pObj->m_pIRecall)
	{
		pObj->m_pIRecall->OnStrategyRunMessage(nType, strInfo);
	}

	//lua_pushinteger(L, 0);
	//no ret value, return 0
	return 0;
}


CLuaStrategy::CLuaStrategy(void)
{
	m_pIRecall = NULL;
}

CLuaStrategy::~CLuaStrategy(void)
{
}

BOOL CLuaStrategy::StrategyFromFile(CString strStgName, CString strFilePathIn )
{
	if (strStgName.IsEmpty() || strFilePathIn.IsEmpty())
	{
		return FALSE;
	}

	CFile file;
	if(!file.Open(strFilePathIn, CFile::modeRead, NULL))
	{
		return FALSE;
	}
	INT64 nFileLen = file.GetLength();
	char* pBuff = new char[nFileLen+2];
	ZeroMemory(pBuff, sizeof(nFileLen+2));
	file.Read(pBuff, nFileLen);
	file.Close();
	m_strStgContent =pBuff;
	if (pBuff)
	{
		delete pBuff;
	}

	return TRUE;
}

BOOL CLuaStrategy::RunStrategy(LPCTSTR szLuaFuncName, VECPARTOLUA& vparIn)
{
	if (m_strStgContent.IsEmpty())
	{
		return FALSE;
	}

	CAutoLuaState aLState;
	luaL_openlibs(aLState);

	//load lua func file
	int nRet = luaL_loadstring(aLState, m_strStgContent);
	if (nRet != LUA_OK)
	{
		fprintf(stderr, "%s\n", lua_tostring(aLState, -1));
		lua_pop(aLState, 1); /* pop error message from the stack */
		ASSERT(FALSE);
		return FALSE;
	}
	nRet = lua_pcall(aLState, 0,0,0);


	//set c callback function
	lua_pushcfunction(aLState, StrategyLuaCallFunc);
	lua_setglobal(aLState, TEXT("FuncCallbackToC"));

	//get lua func
	
	lua_getglobal(aLState, szLuaFuncName);

	int nParNum = vparIn.size();
	//push param to stack
	for (size_t i = 0; i < vparIn.size(); ++i)
	{
		ST_PARAMTOLUA parRef = vparIn[i];
		if (parRef.nType == toluapartype_int32)
		{
			//push first param
			int nVal = atoi(parRef.strVal);
			lua_pushinteger(aLState, nVal);

		}
		else if (parRef.nType == toluapartype_int64)
		{
			//push first param
			INT64 nVal = _atoi64(parRef.strVal);
			lua_pushinteger(aLState, nVal);

		}
		else if (parRef.nType == toluapartype_string)
		{
			//push first param
			lua_pushstring(aLState, parRef.strVal);
		}
		else
		{
			ASSERT(FALSE);
			return FALSE;
		}

	}

	nRet =lua_pcall(aLState, nParNum, 1, 0);
	if (nRet != LUA_OK)
	{
		printf( TEXT("lua call func error:%s"), lua_tostring(aLState, -1));
		return FALSE;
	}
	nRet = lua_tointeger(aLState, -1);

	return nRet == 0;
}

BOOL CLuaStrategy::RunStrategyUseTable(LPCTSTR szLuaFuncName, VECPARTOLUA& vparIn)
{
	if (m_strStgContent.IsEmpty())
	{
		return FALSE;
	}

	CAutoLuaState aLState;
	luaL_openlibs(aLState);

	//load lua func file
	int nRet = luaL_loadstring(aLState, m_strStgContent);
	if (nRet != LUA_OK)
	{
		fprintf(stderr, "%s\n", lua_tostring(aLState, -1));
		lua_pop(aLState, 1); /* pop error message from the stack */
		ASSERT(FALSE);
		return FALSE;
	}
	nRet = lua_pcall(aLState, 0,0,0);


	//set c callback function
// 	lua_pushcfunction(aLState, StrategyLuaCallFunc);
// 	lua_setglobal(aLState, TEXT("FuncCallbackToC"));

	//push lua func
	lua_getglobal(aLState, szLuaFuncName);

	int nParNum = vparIn.size();
	lua_createtable(aLState, nParNum, 0);

	//push param to stack
	for (size_t i = 0; i < vparIn.size(); ++i)
	{
		ST_PARAMTOLUA parRef = vparIn[i];
		lua_pushstring(aLState,parRef.strKey);    //压入key  
		lua_pushinteger(aLState, 166);  //压入Value  
		lua_settable(aLState,-3);     // 上面压入的key,value 被弹出
	}

	nRet =lua_pcall(aLState, 1, 1, 0);
	if (nRet != LUA_OK)
	{
		printf( TEXT("lua call func error:%s"), lua_tostring(aLState, -1));
		return FALSE;
	}
	nRet = lua_tointeger(aLState, -1);

	return nRet == 0;
}