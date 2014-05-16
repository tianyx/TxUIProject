#pragma once


extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};

class CAutoLuaState
{
public:
	CAutoLuaState(void);
	~CAutoLuaState(void);


	lua_State* m_pLS;

	operator lua_State*(){return m_pLS;}
};
