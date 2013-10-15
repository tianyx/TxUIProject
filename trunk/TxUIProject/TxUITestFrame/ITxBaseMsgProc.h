#pragma once

interface ITxBaseMsgProc
{
	virtual HRESULT DefaultMsgProc(UINT nMsg, WPARAM wparam, LPARAM lparam) = 0;
};
