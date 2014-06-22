#include "Server.h"
#include <winsock.h>
#include <map>
#include <string>
#include "NetDefine.h"
#include <iostream>
#include <stdio.h>
#include "Connection.h"

bool CServer::Init()
{
	WSADATA     wsaData;
	WSAStartup(0x0202, &wsaData);

	SOCKET& ServerSocket = Connection::Instance().GetServerSocket();

	/* ����socket */
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == ServerSocket)
	{
		cout<<"Create Socket Failed::"<<GetLastError()<<endl;
		return false;
	}

	/* socket�󶨵�ַ */
	SOCKADDR_IN local;
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	int nRet = bind(ServerSocket, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));
	if (0 != nRet)
	{
		cout<<"Bind Socket Failed::"<<GetLastError()<<endl;
		return false;
	}

	/* socket���� */
	nRet = listen(ServerSocket, LISTENCOUNT);
	if (0 != nRet)
	{
		cout<<"listen Socket Failed::"<<GetLastError()<<endl;
		return false;
	}

	return true;
}

void WorkerThread(void* lpParameter)
{
	while (TRUE)
	{
		Connection::Instance().ProcessMsg();
	}
}

void CServer::Run()
{
	RunThread(&(WorkerThread), NULL);
	while (TRUE)
	{
		Connection::Instance().ProcessConnection();
	}
}