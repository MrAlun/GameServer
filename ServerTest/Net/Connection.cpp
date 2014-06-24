#include "Connection.h"
#include <iostream>
#include <stdio.h>
#include <process.h>

#define MSGSIZE    1024

bool Connection::Init(u_short nPort)
{
	WSADATA     wsaData;
	WSAStartup(0x0202, &wsaData);

	/* 创建socket */
	m_ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_ServerSocket)
	{
		cout<<"Create Socket Failed::"<<GetLastError()<<endl;
		return false;
	}

	/* socket绑定地址 */
	SOCKADDR_IN local;
	local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_family = AF_INET;
	local.sin_port = htons(nPort);
	int nRet = bind(m_ServerSocket, (struct sockaddr *)&local, sizeof(SOCKADDR_IN));
	if (0 != nRet)
	{
		cout<<"Bind Socket Failed::"<<GetLastError()<<endl;
		return false;
	}

	/* socket监听 */
	nRet = listen(m_ServerSocket, 64);
	if (0 != nRet)
	{
		cout<<"listen Socket Failed::"<<GetLastError()<<endl;
		return false;
	}

// 	unsigned long iMode = 1;
// 	ioctlsocket(m_ServerSocket, FIONBIO, &iMode);

	return true;
}

void AcceptThread(void* lpParameter)
{
	Connection* pConn = (Connection*)lpParameter;
	while (true)
	{	
		pConn->ProcessConnection();
	}
	_endthread();
}

void ReceiveThread(void* lpParameter)
{
	Connection* pConn = (Connection*)lpParameter;
	while (true)
	{	
		pConn->ProcessReceiveMsg();
	}
	_endthread();
}

int Connection::Run()
{
	_beginthread(AcceptThread, 0, (void*)this);
	return _beginthread(ReceiveThread, 0, (void*)this);
}

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
	HSocket CientSocket = accept(m_ServerSocket, (struct sockaddr *)&ClientAddr, &AddrLen);
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

void Connection::ProcessReceiveMsg()
{
	fd_set fdread;
	FD_ZERO(&fdread);

//	struct timeval tv = {1, 0};
	char szMessage[MSGSIZE];


	map<HSocket, string>::iterator it = m_mapClientSocket.begin();
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

HSocket& Connection::GetServerSocket()
{
	return m_ServerSocket;
}
