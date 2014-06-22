#ifndef _ThreadPool_H_
#define _ThreadPool_H_
#pragma warning(disable: 4530)
#pragma warning(disable: 4786)
#include <cassert>
#include <vector>
#include <queue>
#include <windows.h>
using namespace std;

class ThreadJob //��������
{
public:
	//���̳߳ص��õ��麯��
	virtual void DoJob(void *pPara) = 0;
};

class ThreadPool
{
public:
	ThreadPool(DWORD dwNum = 4/* dwNum �̳߳ع�ģ */);
	~ThreadPool();
	int AdjustSize(int iNum);											//�����̳߳ع�ģ
	void Call(void (*pFunc)(void *), void *pPara = NULL);
	inline void ThreadPool::Call(ThreadJob * p, void *pPara = NULL);
	bool EndAndWait(DWORD dwWaitTime = INFINITE);						//�����̳߳�, ��ͬ���ȴ�
	inline void End();													//�����̳߳�
	inline DWORD Size();
	inline DWORD GetRunningSize();
	bool IsRunning();

protected:
	static DWORD WINAPI DefaultJobProc(LPVOID lpParameter = NULL);		//�����߳�
	static void CallProc(void *pPara);									//�����û������麯��
	
	struct CallProcPara //�û�����ṹ
	{
		ThreadJob* _pObj;//�û����� 
		void *_pPara;//�û�����
		CallProcPara(ThreadJob* p, void *pPara) : _pObj(p), _pPara(pPara) { };
	};
	
	struct JobItem //�û������ṹ
	{
		void (*_pFunc)(void *);//����
		void *_pPara; //����
		JobItem(void (*pFunc)(void *) = NULL, void *pPara = NULL) : _pFunc(pFunc), _pPara(pPara) { };
	};
	
	struct ThreadItem //�̳߳��е��߳̽ṹ
	{
		HANDLE _Handle;								//�߳̾��
		ThreadPool *_pThis;							//�̳߳ص�ָ��
		DWORD _dwLastBeginTime;						//���һ�����п�ʼʱ��
		DWORD _dwCount;								//���д���
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
	std::queue<JobItem *> _JobQueue;												//��������
	std::vector<ThreadItem *> _ThreadVector;										//�߳�����
	CRITICAL_SECTION _csThreadVector, _csWorkQueue;									//���������ٽ�, �߳������ٽ�
	HANDLE _EventEnd;																//����֪ͨ
	HANDLE _EventComplete;															//����¼�
	HANDLE _SemaphoreCall;															//�����ź�
	HANDLE _SemaphoreDel;															//ɾ���߳��ź�
	long _lThreadNum, _lRunningNum;													//�߳���, ���е��߳���
};

#endif //_ThreadPool_H_