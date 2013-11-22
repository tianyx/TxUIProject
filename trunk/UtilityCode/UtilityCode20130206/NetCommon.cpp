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
			// ȷ��WinSock DLL֧��2.0
			return 0; /* WinSock DLL�ɽ��ܣ��ɹ����� */
		}

		WSACleanup();

		err = WSAVERNOTSUPPORTED; /* ��֧�֣�ʧ�ܷ��� */

	}

	/* Tell the user that we couldn't find a usable WinSock DLL.*/

	TRACE("WinSock DLL does not support requested API version.\n");

	return err;

}
