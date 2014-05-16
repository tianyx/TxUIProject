#include "StdAfx.h"
#include "AutoLuaState.h"

CAutoLuaState::CAutoLuaState(void)
{
	m_pLS = luaL_newstate();
	if (!m_pLS)
	{
		ASSERT(FALSE);
	}
}

CAutoLuaState::~CAutoLuaState(void)
{
	if (m_pLS)
	{
		lua_close(m_pLS);
		m_pLS = NULL;
	}
}
