#include "CThreadPool.h"

ThreadPool::ThreadPool(DWORD dwNum) : _lThreadNum(0), _lRunningNum(0) 
{
	InitializeCriticalSection(&_csThreadVector);				//sys
	InitializeCriticalSection(&_csWorkQueue);					//sys
	_EventComplete = CreateEvent(0, false, false, NULL);		//sys
	_EventEnd = CreateEvent(0, true, false, NULL);				//sys
	_SemaphoreCall = CreateSemaphore(0, 0, 0x7FFFFFFF, NULL);	//sys
	_SemaphoreDel = CreateSemaphore(0, 0, 0x7FFFFFFF, NULL);	//sys
	assert(_SemaphoreCall != INVALID_HANDLE_VALUE);				//sys
	assert(_EventComplete != INVALID_HANDLE_VALUE);				//sys
	assert(_EventEnd != INVALID_HANDLE_VALUE);					//sys
	assert(_SemaphoreDel != INVALID_HANDLE_VALUE);				//sys
	AdjustSize(dwNum <= 0 ? 4 : dwNum);
}

ThreadPool::~ThreadPool()
{
	DeleteCriticalSection(&_csWorkQueue);		//sys
	CloseHandle(_EventEnd);						//sys
	CloseHandle(_EventComplete);				//sys
	CloseHandle(_SemaphoreCall);				//sys
	CloseHandle(_SemaphoreDel);					//sys
	vector<ThreadItem*>::iterator iter;
	for (iter = _ThreadVector.begin() ; iter != _ThreadVector.end() ; iter++)
	{
		if(*iter)
		{
			delete *iter;
		}
	}
	DeleteCriticalSection(&_csThreadVector);	//sys
}

int ThreadPool::AdjustSize(int iNum)
{
	if(iNum > 0)
	{
		ThreadItem *pNew;
		EnterCriticalSection(&_csThreadVector);//sys 进入临界区
		for (int _i = 0 ; _i < iNum ; _i++)
		{
			_ThreadVector.push_back(pNew = new ThreadItem(this)); 
			assert(pNew);//sys
			pNew->_Handle = CreateThread(NULL, 0, DefaultJobProc, pNew, 0, NULL);	//创建线程
			SetThreadPriority(pNew->_Handle, THREAD_PRIORITY_BELOW_NORMAL);			//设置线程优先级，这里是比一般线程优先级低
			assert(pNew->_Handle); 
		}
		LeaveCriticalSection(&_csThreadVector);//sys 离开临界区
	}
	else
	{
		iNum *= -1;
		ReleaseSemaphore(_SemaphoreDel, iNum > _lThreadNum ? _lThreadNum : iNum, NULL); //sys 释放信号量
	}
	return (int)_lThreadNum;
}

void ThreadPool::Call(void (*pFunc)(void *), void *pPara)
{
	assert(pFunc);
	EnterCriticalSection(&_csWorkQueue);//sys 进入临界区
	_JobQueue.push(new JobItem(pFunc, pPara));
	LeaveCriticalSection(&_csWorkQueue);//sys 离开临界区
	ReleaseSemaphore(_SemaphoreCall, 1, NULL);//sys 释放信号量
}

inline void ThreadPool::Call(ThreadJob * p, void *pPara)
{
	Call(CallProc, new CallProcPara(p, pPara));
}

bool ThreadPool::EndAndWait(DWORD dwWaitTime)
{
	SetEvent(_EventEnd);
	return WaitForSingleObject(_EventComplete, dwWaitTime) == WAIT_OBJECT_0;
}

inline void ThreadPool::End()
{
	SetEvent(_EventEnd);
}

inline DWORD ThreadPool::Size()
{
	return (DWORD)_lThreadNum;
}

inline DWORD ThreadPool::GetRunningSize()
{
	return (DWORD)_lRunningNum;
}

bool ThreadPool::IsRunning()
{
	return _lRunningNum > 0;
}

DWORD WINAPI ThreadPool::DefaultJobProc(LPVOID lpParameter)
{
	ThreadItem *pThread = static_cast<ThreadItem*>(lpParameter);
	assert(pThread);
	ThreadPool *pThreadPoolObj = pThread->_pThis;//所属的线程池
	assert(pThreadPoolObj);
	InterlockedIncrement(&pThreadPoolObj->_lThreadNum);//sys 交错增量
	HANDLE hWaitHandle[3];
	hWaitHandle[0] = pThreadPoolObj->_SemaphoreCall;
	hWaitHandle[1] = pThreadPoolObj->_SemaphoreDel;
	hWaitHandle[2] = pThreadPoolObj->_EventEnd;
	JobItem *pJob;
	bool fHasJob;
	for( ; ; )
	{
		DWORD wr = WaitForMultipleObjects(3, hWaitHandle, false, INFINITE);
		//响应删除线程信号
		if(wr == WAIT_OBJECT_0 + 1) 
			break;
		//从队列里取得用户作业
		EnterCriticalSection(&pThreadPoolObj->_csWorkQueue);
		if(fHasJob = !pThreadPoolObj->_JobQueue.empty())
		{
			pJob = pThreadPoolObj->_JobQueue.front();
			pThreadPoolObj->_JobQueue.pop();
			assert(pJob);
		}
		LeaveCriticalSection(&pThreadPoolObj->_csWorkQueue);
		//受到结束线程信号 确定是否结束线程(结束线程信号 && 是否还有工作)
		if(wr == WAIT_OBJECT_0 + 2 && !fHasJob) 
			break;
		if(fHasJob && pJob)
		{
			InterlockedIncrement(&pThreadPoolObj->_lRunningNum);
			pThread->_dwLastBeginTime = GetTickCount();
			pThread->_dwCount++;
			pThread->_fIsRunning = true;
			pJob->_pFunc(pJob->_pPara); //运行用户作业
			delete pJob; 
			pThread->_fIsRunning = false;
			InterlockedDecrement(&pThreadPoolObj->_lRunningNum);
		}
	}
	//删除自身结构
	EnterCriticalSection(&pThreadPoolObj->_csThreadVector);
	pThreadPoolObj->_ThreadVector.erase(find(pThreadPoolObj->_ThreadVector.begin(), pThreadPoolObj->_ThreadVector.end(), pThread));
	LeaveCriticalSection(&pThreadPoolObj->_csThreadVector);
	delete pThread;
	InterlockedDecrement(&pThreadPoolObj->_lThreadNum);
	if(!pThreadPoolObj->_lThreadNum) //所有线程结束
		SetEvent(pThreadPoolObj->_EventComplete);
	return 0;
}

void ThreadPool::CallProc(void *pPara) 
{
	CallProcPara *cp = static_cast<CallProcPara *>(pPara);
	assert(cp);
	if(cp)
	{
		cp->_pObj->DoJob(cp->_pPara);
		delete cp;
	}
}
