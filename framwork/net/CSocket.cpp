#include "CSocket.h"

CSocket::CSocket() : m_server_socket(0)
{

}

CSocket::~CSocket()
{
	closesocket(m_server_socket);
}

bool CSocket::init()
{
	try
	{
		m_server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if ( INVALID_SOCKET == m_server_socket )
		{
			throw "Create Socket Failed!";
			return false;
		}
	}
	catch (const char* s)
	{
		cout<<s<<endl;
		return false;
	}
	return true;
}

bool CSocket::connect(short domain, const char* ipAddr, UINT16 port, int backlog)
{
	try
	{
		sockaddr_in serverLocalAddr;
		serverLocalAddr.sin_family = domain;
		serverLocalAddr.sin_addr.s_addr = inet_addr(ipAddr);
		serverLocalAddr.sin_port = htons(port);
		memset(serverLocalAddr.sin_zero, 0x00, 8);
		if ( 0 != bind(m_server_socket, (struct sockaddr*)&serverLocalAddr, sizeof(serverLocalAddr)) )
		{
			throw "Bind Socket Failed!";
			return false;
		}
		if ( 0 >= backlog )
		{
			throw "Backlog Error!";
			return false;
		}
		if ( 0 != listen(m_server_socket, backlog) )
		{
			throw "Listen Socket Failed!";
			return false;
		}
	}
	catch (const char* s)
	{
		cout<<s<<endl;
		return false;
	}
	return true;
}

SOCKET CSocket::accept_connect()
{
	SOCKET clientSocket;
	sockaddr_in clientAddr;
	int addrLen = sizeof(clientAddr);
	clientSocket = accept(m_server_socket, (struct sockaddr*)&clientAddr, &addrLen);
	if ( SOCKET_ERROR == clientSocket )
	{
		cout<<"Accept Client Connect Failed!"<<endl;
		return SOCKET_ERROR;
	}
	return clientSocket;
}

void CSocket::run()
{

}

SOCKET CSocket::get_sock()
{
	return m_server_socket;
}