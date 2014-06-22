#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <winsock.h>
#include <map>
#include <string>

using namespace std;

class Connection
{
protected:
	Connection(){}
public:
	~Connection(){}

	static Connection& Instance()
	{
		static Connection gs_Connection;
		return gs_Connection;
	}

	void ProcessConnection();

	void ProcessMsg();

	SOCKET& GetServerSocket();

private:
	SOCKET m_ServerSocket;
	map<SOCKET, string> m_mapClientSocket;
};

#endif//_CONNECTION_H_