#ifndef _THREAD_H_
#define _THREAD_H_

class CThread
{
protected:
	CThread(){}
	~CThread(){}
public:
	virtual void Run() = 0;
protected:
	void RunThread(void (*pFun)(void*), void* pParam);
};

#endif//_THREAD_H_