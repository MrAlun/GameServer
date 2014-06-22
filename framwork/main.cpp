#pragma comment(lib,"ws2_32.lib")
#include "net/CSocket.h"
#include "thread/CThreadPool.h"

#define  PORT 4000
#define  IP_ADDRESS "127.0.0.1"

void DoJob(void *pPara)
{
	cout<<"DoJob"<<endl;
}

int main(int argc, char* argv[])
{
	WSADATA  Ws;
	if ( WSAStartup(MAKEWORD(2,2), &Ws) != 0 )
	{
		cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;
		return -1;
	} 

	CSocket mysocket;
	if ( false == mysocket.init() )
	{
		cout<<"Create Socket Failed!"<<endl;
		return -1;
	}
	if ( false == mysocket.connect(AF_INET, IP_ADDRESS, PORT) )
	{
		cout<<"Connect Failed!"<<endl;
		return -1;
	}
	else
	{
		cout<<"Connect Success!"<<endl;
	}
	//SOCKET clientSocket = mysocket.accept_connect();

	ThreadPool threadPool(1);
	threadPool.Call(&DoJob);

	fd_set fdread;
	int ret;

	while(true)
	{
// 		FD_ZERO(&fdread);
// 		FD_SET(mysocket.get_sock(), &fdread);
// 		if (SOCKET_ERROR == (ret = select(0, &fdread, NULL, NULL, NULL)))
// 		{
// 			continue;
// 		}
// 
// 		if (ret > 0)
// 		{
// 			if (FD_ISSET(mysocket.get_sock(), &fdread))
// 			{
// 				
// 			}
// 		}
	}


	WSACleanup();
	return 0;
}