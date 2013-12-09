#pragma once
interface IMBCRelayControlInterafce
{
	virtual HRESULT ChangeRelayAddr(char* szChId, SOCKADDR_IN& addrRelay) = 0;
};