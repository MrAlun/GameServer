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
		EnterCriticalSection(&_csThreadVector);//sys �����ٽ���
		for (int _i = 0 ; _i < iNum ; _i++)
		{
			_ThreadVector.push_back(pNew = new ThreadItem(this)); 
			assert(pNew);//sys
			pNew->_Handle = CreateThread(NULL, 0, DefaultJobProc, pNew, 0, NULL);	//�����߳�
			SetThreadPriority(pNew->_Handle, THREAD_PRIORITY_BELOW_NORMAL);			//�����߳����ȼ��������Ǳ�һ���߳����ȼ���
			assert(pNew->_Handle); 
		}
		LeaveCriticalSection(&_csThreadVector);//sys �뿪�ٽ���
	}
	else
	{
		iNum *= -1;
		ReleaseSemaphore(_SemaphoreDel, iNum > _lThreadNum ? _lThreadNum : iNum, NULL); //sys �ͷ��ź���
	}
	return (int)_lThreadNum;
}

void ThreadPool::Call(void (*pFunc)(void *), void *pPara)
{
	assert(pFunc);
	EnterCriticalSection(&_csWorkQueue);//sys �����ٽ���
	_JobQueue.push(new JobItem(pFunc, pPara));
	LeaveCriticalSection(&_csWorkQueue);//sys �뿪�ٽ���
	ReleaseSemaphore(_SemaphoreCall, 1, NULL);//sys �ͷ��ź���
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
	ThreadPool *pThreadPoolObj = pThread->_pThis;//�������̳߳�
	assert(pThreadPoolObj);
	InterlockedIncrement(&pThreadPoolObj->_lThreadNum);//sys ��������
	HANDLE hWaitHandle[3];
	hWaitHandle[0] = pThreadPoolObj->_SemaphoreCall;
	hWaitHandle[1] = pThreadPoolObj->_SemaphoreDel;
	hWaitHandle[2] = pThreadPoolObj->_EventEnd;
	JobItem *pJob;
	bool fHasJob;
	for( ; ; )
	{
		DWORD wr = WaitForMultipleObjects(3, hWaitHandle, false, INFINITE);
		//��Ӧɾ���߳��ź�
		if(wr == WAIT_OBJECT_0 + 1) 
			break;
		//�Ӷ�����ȡ���û���ҵ
		EnterCriticalSection(&pThreadPoolObj->_csWorkQueue);
		if(fHasJob = !pThreadPoolObj->_JobQueue.empty())
		{
			pJob = pThreadPoolObj->_JobQueue.front();
			pThreadPoolObj->_JobQueue.pop();
			assert(pJob);
		}
		LeaveCriticalSection(&pThreadPoolObj->_csWorkQueue);
		//�ܵ������߳��ź� ȷ���Ƿ�����߳�(�����߳��ź� && �Ƿ��й���)
		if(wr == WAIT_OBJECT_0 + 2 && !fHasJob) 
			break;
		if(fHasJob && pJob)
		{
			InterlockedIncrement(&pThreadPoolObj->_lRunningNum);
			pThread->_dwLastBeginTime = GetTickCount();
			pThread->_dwCount++;
			pThread->_fIsRunning = true;
			pJob->_pFunc(pJob->_pPara); //�����û���ҵ
			delete pJob; 
			pThread->_fIsRunning = false;
			InterlockedDecrement(&pThreadPoolObj->_lRunningNum);
		}
	}
	//ɾ������ṹ
	EnterCriticalSection(&pThreadPoolObj->_csThreadVector);
	pThreadPoolObj->_ThreadVector.erase(find(pThreadPoolObj->_ThreadVector.begin(), pThreadPoolObj->_ThreadVector.end(), pThread));
	LeaveCriticalSection(&pThreadPoolObj->_csThreadVector);
	delete pThread;
	InterlockedDecrement(&pThreadPoolObj->_lThreadNum);
	if(!pThreadPoolObj->_lThreadNum) //�����߳̽���
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
