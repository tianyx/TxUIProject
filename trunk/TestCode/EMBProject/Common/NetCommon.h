/********************************************************************
	created:	2013/07/03
	created:	2013:7
	filename: 	NetCommon.h
	file base:	NetCommon
	file ext:	h
	author:		tian_yx
	purpose:	for net function
*********************************************************************/
#pragma once

class NetCommon
{
public:
	NetCommon(void);
	~NetCommon(void);
};

//multi broadcast must use winsock2 	
//wVersionRequested = MAKEWORD(2, 2); // �첽I/O�Ͷ�ַ�㲥ֻ����WinSock 2.0���ϰ汾��֧��
//return 0 if success, else return wsaerror
//if success, WSAStartup was called
int CheckWinsockVersion(WORD wVersionRequested);