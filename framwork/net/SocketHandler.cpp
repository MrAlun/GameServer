#include "SocketHandler.h"
#include <iostream>

using namespace std;

SOCKET CSocketHandler::CreateSocket()
{
	int nRet = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (0 != nRet)
	{
		cout<<"Bind Socket Failed::"<<GetLastError()<<endl;
	}
	return nRet;
}

int CSocketHandler::BindAddr(SOCKET nSocket, const char* strIpAddr, u_short nPort)
{
	struct sockaddr_in LocalAddr;
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_addr.s_addr = inet_addr(strIpAddr);
	LocalAddr.sin_port = htons(nPort);
	memset(LocalAddr.sin_zero, 0x00, 8);

	int nRet = bind(nSocket, (struct sockaddr*)&LocalAddr, sizeof(LocalAddr));
	if (0 != nRet)
	{
		cout<<"Bind Socket Failed::"<<GetLastError()<<endl;
	}
	return nRet;
}