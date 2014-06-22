#include <winsock.h>
#include <stdio.h>
#include "../Net/Server.h"

#pragma comment(lib, "ws2_32.lib")

CServer g_Server;

int main()
{
	g_Server.Init();
	g_Server.Run();

	return 0;
}