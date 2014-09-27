// #include <stdio.h>
// 
// int main(void)
// {
// 	int a = 3;
// 
// 	int *one_ptr = &a; //相当于int *one_ptr；one_ptr = &a;
// 
// 	int **two_ptr = &one_ptr; //相当于int **two_ptr; two_ptr = &one_ptr;
// 
// 	printf("&a = %p, one_ptr = %p, *two_ptr = %p\n", &a, one_ptr, *two_ptr);
// 	printf("&one_ptr = %p, *two_ptr = %p\n", &one_ptr, two_ptr);
// 	printf("a = %d, *one_ptr = %d, **two_ptr = %d\n", a, *one_ptr, **two_ptr);
// 
// 	while (true)
// 	{
// 
// 	}
// 
// 	return 0;
// }

// #include "sock_wrap.h"
// #include "lightthread.h"
// 
// void proc(void* param)
// {
// 	while (true)
// 	{
// 
// 	}
// }
// 
// int main(void)
// {
// 	CLightThread::CreateThread(proc, NULL);
// 	return 0;
// }

// #include <windows.h>
// #include <stdio.h>
// #include <iostream>
// 
// #define MAX_SEM_COUNT 10
// #define THREADCOUNT 15
// 
// HANDLE ghSemaphore;
// 
// DWORD WINAPI ThreadProc( LPVOID );
// 
// using namespace std;
// 
// int main()
// {
// 	HANDLE aThread[THREADCOUNT];
// 	DWORD ThreadID;
// 	int i;
// 
// 	// Create a semaphore with initial and max counts of MAX_SEM_COUNT
// 
// 	ghSemaphore = CreateSemaphore( 
// 		NULL,           // default security attributes
// 		MAX_SEM_COUNT,  // initial count
// 		MAX_SEM_COUNT,  // maximum count
// 		NULL);          // unnamed semaphore
// 
// 	if (ghSemaphore == NULL) 
// 	{
// 		printf("CreateSemaphore error: %d\n", GetLastError());
// 		return -1;
// 	}
// 
// 	// Create worker threads
// 
// 	for( i=0; i < THREADCOUNT; i++ )
// 	{
// 		aThread[i] = CreateThread( 
// 			NULL,       // default security attributes
// 			0,          // default stack size
// 			(LPTHREAD_START_ROUTINE) ThreadProc, 
// 			NULL,       // no thread function arguments
// 			0,          // default creation flags
// 			&ThreadID); // receive thread identifier
// 
// 		if( aThread[i] == NULL )
// 		{
// 			printf("CreateThread error: %d\n", GetLastError());
// 			return -1;
// 		}
// 	}
// 
// 	// Wait for all threads to terminate
// 
// 	WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, INFINITE);
// 
// 	// Close thread and semaphore handles
// 
// 	for( i=0; i < THREADCOUNT; i++ )
// 		CloseHandle(aThread[i]);
// 
// 	CloseHandle(ghSemaphore);
// 	int a;
// 	cin>>a;
// 	return 0;
// }
// 
// DWORD WINAPI ThreadProc( LPVOID lpParam )
// {
// 	DWORD dwWaitResult; 
// 	BOOL bContinue=TRUE;
// 
// 	while(bContinue)
// 	{
// 		// Try to enter the semaphore gate.
// 
// 		dwWaitResult = WaitForSingleObject( 
// 			ghSemaphore,   // handle to semaphore
// 			0L);           // zero-second time-out interval
// 
// 		switch (dwWaitResult) 
// 		{ 
// 			// The semaphore object was signaled.
// 		case WAIT_OBJECT_0: 
// 			// TODO: Perform task
// 			printf("Thread %d: wait succeeded\n", GetCurrentThreadId());
// 			bContinue=FALSE;            
// 
// 			// Simulate thread spending time on task
// 			Sleep(5);
// 
// 			// Relase the semaphore when task is finished
// 
// 			if (!ReleaseSemaphore( 
// 				ghSemaphore,  // handle to semaphore
// 				1,            // increase count by one
// 				NULL) )       // not interested in previous count
// 			{
// 				printf("ReleaseSemaphore error: %d\n", GetLastError());
// 			}
// 			break; 
// 
// 			// The semaphore was nonsignaled, so a time-out occurred.
// 		case WAIT_TIMEOUT: 
// 			printf("Thread %d: wait timed out\n", GetCurrentThreadId());
// 			break; 
// 		}
// 	}
// 	return TRUE;
// }

#include <stdio.h>

typedef struct
{
	int i;
	int num;
	int state;
} task;

int cb(task *t)
{
	switch (t->state) {
	case 0:
		for (;;) {
			t->num = 1;
			for (t->i = 0; t->i < 20; t->i++) {
				t->state = __LINE__ + 2;
				return t->num;
	case __LINE__:
		t->num += 1;
			}
		}
	}
}

int main()
{
	task t;
	int i;

	t.state = 0;

	for (i = 0; i < 100; i++) {
		printf("%d ", cb(&t));
	}
	return 0;
}