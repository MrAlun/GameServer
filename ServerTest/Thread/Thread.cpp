#include "Thread.h"
#include "../Platform/Platform.h"

#if defined(_WIN32_PLATFROM_)
#include <process.h> /* _beginthread, _endthread */
#endif

#if defined(_WIN32_PLATFROM_)
void CThread::RunThread(void (*pFun)(void*), void* pParam)
{
	_beginthread(pFun, 0, pParam);
}
#endif