#ifndef _THREAD_H_
#define _THREAD_H_

#pragma once 
#include <windows.h> 
#include <functional> 
using namespace std; 

#include "CSingleton.h" 
#include "Clocker.h" 

namespace CThread 
{
	// 线程结构体
	struct thread; 
	// 线程执行函数
	DWORD WINAPI ThreadFunc(void* thrd); 
	// 获取当前线程的数据
	thread* get_current_thread_data();
	//
	void interruption_point();


	// #define TLS_OUT_OF_INDEXES 0xFFFFFFFF 
	DWORD current_thread_tls_key = TLS_OUT_OF_INDEXES; 

	// 线程空间单例
	struct tls_alloc_once : public singleton<tls_alloc_once>
	{ 
		locker lock; 
		tls_alloc_once() 
		{ 
			lock.lock(); 
			current_thread_tls_key = TlsAlloc(); 
			lock.unlock(); 
		} 
	}; 

	struct interrupt_exception {}; 

	struct thread_data 
	{ 
		bool interrupted; 
		typedef function<void()> f_type; 
		f_type _f; 
		thread_data() : interrupted(false) {} 

		template<typename F> 
		thread_data(F f) : _f(f), interrupted(false) {} 

		void run()
		{ 
			if(_f) _f(); 
		} 
	}; 

	struct thread
	{ 
		thread_data _data; 
		HANDLE _h; 
		DWORD _id; 
		thread(){} 
		thread(thread_data data) : _data(data){} 
		void start(){ _h = CreateThread(NULL, 0, ThreadFunc, (void*)this, 0, &_id); } 
		void join(){ ::WaitForSingleObject(_h, INFINITE); } 
		void operator=(thread_data data){ _data = data; } 
		void interrupt(){ _data.interrupted = true; } 
	};

	DWORD WINAPI ThreadFunc(void* thrd) 
	{ 
		tls_alloc_once::instance(); 
		if (current_thread_tls_key == TLS_OUT_OF_INDEXES) 
		{
			throw std::exception("tls alloc error");
		}

		if (!::TlsSetValue(current_thread_tls_key, thrd)) 
		{
			throw std::exception("tls setvalue error"); 
		}

		try 
		{ 
			static_cast<thread*>(thrd)->_data.run(); 
		}
		catch(interrupt_exception&)
		{

		} 
		return 0; 
	}

	void interruption_point() 
	{ 
		thread* thrd = get_current_thread_data(); 
		if (!thrd) 
		{
			throw std::exception("no thread, wth"); 
		}
		if (thrd->_data.interrupted) 
		{ 
			thrd->_data.interrupted = false; 
			throw interrupt_exception(); 
		} 
	} 
	thread* get_current_thread_data()
	{ 
		if (current_thread_tls_key == TLS_OUT_OF_INDEXES) 
		{ 
			return NULL; 
		} 
		return (thread*)TlsGetValue(current_thread_tls_key); 
	} 

}; // end of namespace thread 
#endif //_THREAD_H_