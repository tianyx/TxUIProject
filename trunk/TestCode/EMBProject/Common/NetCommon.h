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
//wVersionRequested = MAKEWORD(2, 2); // 异步I/O和多址广播只有在WinSock 2.0以上版本才支持
//return 0 if success, else return wsaerror
//if success, WSAStartup was called
int CheckWinsockVersion(WORD wVersionRequested);