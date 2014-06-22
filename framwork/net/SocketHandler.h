#ifndef _SOCKET_HANDLER_H_
#define _SOCKET_HANDLER_H_

#include <Windows.h>

class CSocketHandler
{
private:
	CSocketHandler(){}
public:
	~CSocketHandler(){}

	SOCKET CreateSocket();

	int BindAddr(SOCKET nSocket, const char* strIpAddr, u_short nPort);
};

#endif//_SOCKET_HANDLER_H_