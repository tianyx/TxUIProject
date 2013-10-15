// TestMC_RDM_Sender.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "winsock2.h"
#pragma comment(lib, "Ws2_32.lib")
int _tmain(int argc, _TCHAR* argv[])
{
	WSAStartup(MAKEWORD(2,2), NULL);
	SOCKET        s;
	SOCKADDR_IN   salocal, sasession;
	int           dwSessionPort;

	s = socket (AF_INET, SOCK_RDM, IPPROTO_PGM);

	salocal.sin_family = AF_INET;
	salocal.sin_port   = htons (0);    // Port is ignored here
	salocal.sin_addr.s_addr = htonl (INADDR_ANY);

	bind (s, (SOCKADDR *)&salocal, sizeof(salocal));

	//
	// Set all relevant sender socket options here
	//

	//
	// Now, connect <entity type="hellip"/>
	// Setting the connection port (dwSessionPort) has relevance, and
	// can be used to multiplex multiple sessions to the same
	// multicast group address over different ports
	//
	dwSessionPort = 51001;
	sasession.sin_family = AF_INET;
	sasession.sin_port   = htons (dwSessionPort);
	sasession.sin_addr.s_addr = inet_addr ("235.6.7.8");

	connect (s, (SOCKADDR *)&sasession, sizeof(sasession));

	//
	// We're now ready to send data!
	//
	LONG        error;
	//:
	int SendLength = 0;
	char pSendBuffer[256];
	static int nSendData = 0;
	while(1)
	{
		nSendData++;
		sprintf(pSendBuffer, "snd = %d\r\n", nSendData);
		SendLength = strlen(pSendBuffer);
		error = send (s, pSendBuffer, SendLength, 0);
		if (error == SOCKET_ERROR)
		{
			fprintf (stderr, "send() failed: Error = %d\n",
				WSAGetLastError());
			break;
		}
		else
		{
			printf(pSendBuffer);
		}
		Sleep(1000);

	}
	WSACleanup();

	return 0;
}

