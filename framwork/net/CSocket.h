#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <Windows.h>
#include <iostream>

using namespace std;

enum E_SOCKET_ERROR
{
	E_SOCKET_ERROR_DEFAULT = 0,
	E_SOCKET_ERROR_END,
};

class CSocket
{
public:
	CSocket();
	virtual ~CSocket();

	bool init();
	bool connect(short domain, const char* ipAddr, UINT16 port, int backlog = 100);
	SOCKET accept_connect();

	void run();

	SOCKET get_sock();

private:
	SOCKET m_server_socket;
};

#endif //_SOCKET_H_