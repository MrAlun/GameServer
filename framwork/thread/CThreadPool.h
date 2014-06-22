#ifndef _ThreadPool_H_
#define _ThreadPool_H_
#pragma warning(disable: 4530)
#pragma warning(disable: 4786)
#include <cassert>
#include <vector>
#include <queue>
#include <windows.h>
using namespace std;

class ThreadJob //工作基类
{
public:
	//供线程池调用的虚函数
	virtual void DoJob(void *pPara) = 0;
};

class ThreadPool
{
public:
	ThreadPool(DWORD dwNum = 4/* dwNum 线程池规模 */);
	~ThreadPool();
	int AdjustSize(int iNum);											//调整线程池规模
	void Call(void (*pFunc)(void *), void *pPara = NULL);
	inline void ThreadPool::Call(ThreadJob * p, void *pPara = NULL);
	bool EndAndWait(DWORD dwWaitTime = INFINITE);						//结束线程池, 并同步等待
	inline void End();													//结束线程池
	inline DWORD Size();
	inline DWORD GetRunningSize();
	bool IsRunning();

protected:
	static DWORD WINAPI DefaultJobProc(LPVOID lpParameter = NULL);		//工作线程
	static void CallProc(void *pPara);									//调用用户对象虚函数
	
	struct CallProcPara //用户对象结构
	{
		ThreadJob* _pObj;//用户对象 
		void *_pPara;//用户参数
		CallProcPara(ThreadJob* p, void *pPara) : _pObj(p), _pPara(pPara) { };
	};
	
	struct JobItem //用户函数结构
	{
		void (*_pFunc)(void *);//函数
		void *_pPara; //参数
		JobItem(void (*pFunc)(void *) = NULL, void *pPara = NULL) : _pFunc(pFunc), _pPara(pPara) { };
	};
	
	struct ThreadItem //线程池中的线程结构
	{
		HANDLE _Handle;								//线程句柄
		ThreadPool *_pThis;							//线程池的指针
		DWORD _dwLastBeginTime;						//最后一次运行开始时间
		DWORD _dwCount;								//运行次数
		bool _fIsRunning;
		ThreadItem(ThreadPool *pthis) : _pThis(pthis), _Handle(NULL), _dwLastBeginTime(0), _dwCount(0), _fIsRunning(false) { };
		~ThreadItem()
		{
			if(_Handle)
			{
				CloseHandle(_Handle);
				_Handle = NULL;
			}
		}
	};
	std::queue<JobItem *> _JobQueue;												//工作队列
	std::vector<ThreadItem *> _ThreadVector;										//线程数据
	CRITICAL_SECTION _csThreadVector, _csWorkQueue;									//工作队列临界, 线程数据临界
	HANDLE _EventEnd;																//结束通知
	HANDLE _EventComplete;															//完成事件
	HANDLE _SemaphoreCall;															//工作信号
	HANDLE _SemaphoreDel;															//删除线程信号
	long _lThreadNum, _lRunningNum;													//线程数, 运行的线程数
};

#endif //_ThreadPool_H_