#include "StdAfx.h"
#include "LuaStrategy.h"

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

BOOL CLuaStrategy::RunStrategy(LPCTSTR szLuaFuncName, VECPARTOLUA& vparIn, int& nRetVal)
{
	if (m_strStgContent.IsEmpty())
	{
		ASSERT(FALSE);
		return FALSE;
	}

// 	//load lua func file
// 	int nRet = luaL_loadstring(m_aLState, m_strStgContent);
// 	if (nRet != LUA_OK)
// 	{
// 		CFWriteLog(0, TEXT("lua load func error:%s"), lua_tostring(m_aLState, -1));
// 		lua_pop(m_aLState, 1); /* pop error message from the stack */
// 		ASSERT(FALSE);
// 		return FALSE;
// 	}


// 	//set c callback function
// 	lua_pushcfunction(m_aLState, StrategyLuaCallFunc);
// 	lua_setglobal(m_aLState, TEXT("FuncCallbackToC"));

	//get lua func
	int nTop = lua_gettop(m_aLState);

	lua_getglobal(m_aLState, szLuaFuncName);
	
	//set param
	lua_createtable(m_aLState, vparIn.size()+1, 0);

	//push param to stack
	for (size_t i = 0; i < vparIn.size(); ++i)
	{
		ST_PARAMTOLUA parRef = vparIn[i];
		//push param to stack
		for (size_t i = 0; i < vparIn.size(); ++i)
		{
			ST_PARAMTOLUA parRef = vparIn[i];
			lua_pushstring(m_aLState,parRef.strKey);    //push key  
			lua_pushstring(m_aLState,parRef.strVal);  //push Value  
			lua_settable(m_aLState,-3);     // pop key,value and update table
		}
	}

	//push callback obj
	lua_pushstring(m_aLState, luaparamkey_callbkobj);    
	lua_pushinteger(m_aLState, (int)this);  
	lua_settable(m_aLState,-3);    

	int nRet = lua_pcall(m_aLState, 1, 1, 0);
	if (nRet != LUA_OK)
	{
		CFWriteLog(0, TEXT("lua call func error:%s"), lua_tostring(m_aLState, -1));
	}
	else
	{
		nRetVal = lua_tointeger(m_aLState, -1);
		CFWriteLog(0, TEXT("lua stg returned :%d"), nRetVal);

	}

	lua_settop(m_aLState, nTop);

	return nRet == LUA_OK;
}

BOOL CLuaStrategy::InitStrategyFromString( CString& strStgName, CString& strLuaContentIn, IStrategyRuntimeCallback* pCallbackIn )
{
	m_strStgContent = strLuaContentIn;
	m_strStgName = strStgName;
	m_pIRecall = pCallbackIn;
	//CAutoLuaState aLState;
	luaL_openlibs(m_aLState);

	//load lua func file
	int nRet = luaL_loadstring(m_aLState, m_strStgContent);
	if (nRet != LUA_OK)
	{
		CFWriteLog(TEXT("%s"), lua_tostring(m_aLState, -1));
		lua_pop(m_aLState, 1); /* pop error message from the stack */
		ASSERT(FALSE);
		return FALSE;
	}

	//pre init stg func
	nRet = lua_pcall(m_aLState, 0,0,0);
	if (nRet != LUA_OK)
	{
		CFWriteLog(0, TEXT("lua load func error:%s"), lua_tostring(m_aLState, -1));
		lua_pop(m_aLState, 1); /* pop error message from the stack */
		ASSERT(FALSE);
		return FALSE;
	}

	//set c callback function
	lua_pushcfunction(m_aLState, StrategyLuaCallFunc);
	lua_setglobal(m_aLState, TEXT("FuncCallbackToC"));

	return TRUE;

}

BOOL CLuaStrategy::InitStrategyFromFile( CString& strStgName, CString& strFilePathIn, IStrategyRuntimeCallback* pCallbackIn )
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
	CString strBuff =pBuff;
	if (pBuff)
	{
		delete pBuff;
	}
	
	return InitStrategyFromString(strStgName, strBuff, pCallbackIn);

}
