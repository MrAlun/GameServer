#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <map>
#include <string>
#include <Platform.h>
using namespace std;

#if defined(_WIN32_PLATFROM_)
#include <winsock.h>

typedef SOCKET HSocket;

#pragma comment(lib, "ws2_32.lib")
#endif

#if defined(_LINUX_PLATFROM_)
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

typedef int HSocket;

#define SOCKET_ERROR (-1)
#define INVALID_SOCKET 0
#endif


class Connection
{
public:
	Connection(){}
	~Connection(){}

	bool Init(u_short nPort);

	int Run();

	void ProcessConnection();

	void ProcessReceiveMsg();

	HSocket& GetServerSocket();

private:
	HSocket m_ServerSocket;
	std::map<HSocket, string> m_mapClientSocket;
};

#endif//_CONNECTION_H_