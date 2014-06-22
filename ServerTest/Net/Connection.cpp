#include "Connection.h"
#include <iostream>
#include <stdio.h>
#include <winsock.h>
#include "NetDefine.h"

void Connection::ProcessConnection()
{
	fd_set fdread;
	FD_ZERO(&fdread);
	FD_SET(m_ServerSocket, &fdread);
	int ret = select(0, &fdread, NULL, NULL, NULL);
	if (ret == 0)
	{
		return;
	}
	struct sockaddr_in ClientAddr;
	int AddrLen = sizeof(ClientAddr);
	SOCKET CientSocket = accept(m_ServerSocket, (struct sockaddr *)&ClientAddr, &AddrLen);
	if (INVALID_SOCKET == CientSocket)
	{
		cout<<"Accept Failed::"<<GetLastError()<<endl;
		return;
	}
	char ServerName[64];
	memset(ServerName, 0, 64);
	int Result = _snprintf(ServerName, sizeof(ServerName),"%s:%d", inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));
	if (Result == sizeof(ServerName) || Result < 0)
	{
		printf("warning:sting will be truncated");
		ServerName[sizeof(ServerName) - 1] = 0;
	}
	printf("Accepted client:%s \n", ServerName);
	m_mapClientSocket[CientSocket] = ServerName;
}

void Connection::ProcessMsg()
{
	fd_set fdread;
	FD_ZERO(&fdread);

//	struct timeval tv = {1, 0};
	char szMessage[MSGSIZE];


	map<SOCKET, string>::iterator it = m_mapClientSocket.begin();
	for ( ; it != m_mapClientSocket.end() ; ++it)
	{
		FD_SET(it->first, &fdread);
	}

	int ret = select(0, &fdread, NULL, NULL, NULL);
	if (ret == 0)
	{
		return;
	}

	it = m_mapClientSocket.begin();
	for ( ; it != m_mapClientSocket.end() ; )
	{
		if (FD_ISSET(it->first, &fdread))
		{
			ret = recv(it->first, szMessage, MSGSIZE, 0);
			if (ret == 0 || (ret == SOCKET_ERROR && WSAGetLastError() == WSAECONNRESET))
			{
				printf("Closes client:%s \n", it->second.c_str());
				closesocket(it->first);
				m_mapClientSocket.erase(it++);
			}
			else
			{
				szMessage[ret] = '\0';
				printf("client[%s]:%s \n", it->second.c_str(), szMessage);
				send(it->first, szMessage, strlen(szMessage), 0);
				++it;
			}
		}
		else
		{
			++it;
		}
	}
}

SOCKET& Connection::GetServerSocket()
{
	return m_ServerSocket;
}
