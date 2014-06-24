#include <WINSOCK2.H>
#include <stdio.h>
#include <iostream>

using namespace std;

#define SERVER_ADDRESS "127.0.0.1"
#define PORT           5150
#define MSGSIZE        1024

#pragma comment(lib, "ws2_32.lib")

int main()
{
	u_short nPort = 0;
	cin>>nPort;

	WSADATA     wsaData;
	SOCKET      sClient;
	SOCKADDR_IN server;
	char        szMessage[MSGSIZE];
	int         ret;

	// Initialize Windows socket library
	WSAStartup(0x0202, &wsaData);

	// Create client socket
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Connect to server
	memset(&server, 0, sizeof(SOCKADDR_IN));
	server.sin_family = AF_INET;
	server.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDRESS);
	server.sin_port = htons(nPort);

	connect(sClient, (struct sockaddr *)&server, sizeof(SOCKADDR_IN));

	int a = 1;
	while (TRUE)
	{
		Sleep(3000);
		printf("Send:");
		_snprintf(szMessage, MSGSIZE - 1, "send id : [%d]", a);
		a++;

		// Send message
		send(sClient, szMessage, strlen(szMessage), 0);

		// Receive message
		ret = recv(sClient, szMessage, MSGSIZE, 0);
		szMessage[ret] = '\0';

		printf("Received [%d bytes]: '%s'\n", ret, szMessage);
	}

	// Clean up
	closesocket(sClient);
	WSACleanup();
	return 0;
}
