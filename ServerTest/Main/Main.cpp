#include <winsock.h>
#include <stdio.h>
#include <iostream>
#include "../Net/Connection.h"

int main()
{
	Connection g_Conn;
	g_Conn.Init(5150);
	int a = g_Conn.Run();

	Connection g_Conn1;
	g_Conn1.Init(5151);
	int b = g_Conn1.Run();

// 	WaitForSingleObject((HANDLE)a, INFINITE);
// 	WaitForSingleObject((HANDLE)b, INFINITE);

	while (true)
	{
		int a;
		cin>>a;
	}

	return 0;
}