#include "StdAfx.h"
#include "NetCommon.h"

NetCommon::NetCommon(void)
{
}

NetCommon::~NetCommon(void)
{
}

int CheckWinsockVersion(WORD wVersionRequested)
{
	WSADATA wsaData;
	int err;

	err = WSAStartup(wVersionRequested, &wsaData);

	if (err==0) 
	{

		if (wsaData.wVersion== wVersionRequested ) 
		{
			// 确认WinSock DLL支持2.0
			return 0; /* WinSock DLL可接受，成功返回 */
		}

		WSACleanup();

		err = WSAVERNOTSUPPORTED; /* 不支持，失败返回 */

	}

	/* Tell the user that we couldn't find a usable WinSock DLL.*/

	TRACE("WinSock DLL does not support requested API version.\n");

	return err;

}
