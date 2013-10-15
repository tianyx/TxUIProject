// TestMC_RDM_Recv.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "winsock2.h"
#pragma comment(lib, "Ws2_32.lib")

int _tmain(int argc, _TCHAR* argv[])
{

	WSADATA wsd;
	WSAStartup(MAKEWORD(2,2), &wsd);

	SOCKET        s,
		sclient;
	SOCKADDR_IN   salocal,
		sasession;
	int           sasessionsz, dwSessionPort;

	s = socket (AF_INET, SOCK_RDM, IPPROTO_PGM);

	//
	// The bind port (dwSessionPort) specified should match that
	// which the sender specified in the connect call
	//
	dwSessionPort = 51001;
	salocal.sin_family = AF_INET;
	salocal.sin_port   = htons (dwSessionPort);    
	salocal.sin_addr.s_addr = inet_addr ("235.6.7.8");

	bind (s, (SOCKADDR *)&salocal, sizeof(salocal));

	//
	// Set all relevant receiver socket options here
	//

	listen (s, 10);

	sasessionsz = sizeof(sasession);
	sclient = accept (s, (SOCKADDR *)&sasession, &sasessionsz);

	//
	// accept will return the client socket and we are now ready
	// to receive data on the new socket!
	//

	LONG        BytesRead;
	//:
	int MaxBufferSize = 256;
	char pTestBuffer[256];
	while(1)
	{
		ZeroMemory(pTestBuffer, 256);
		BytesRead = recv (sclient, pTestBuffer, MaxBufferSize, 0);
		if (BytesRead == 0)
		{
			fprintf(stdout, "Session was terminated\n");
			break;
		}
		else if (BytesRead == SOCKET_ERROR)
		{
			fprintf(stderr, "recv() failed: Error = %d\n",
				WSAGetLastError());
			break;
		}
		else
		{
			printf_s(pTestBuffer);
		}
	}

	return 0;
}

