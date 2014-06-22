#pragma once 
#include "windows.h" 

class locker 
{ 
public: 
	locker() 
	{ 
		::InitializeCriticalSection(&cs); 
	} 
	~locker() 
	{ 
		::DeleteCriticalSection(&cs); 
	} 
	void lock() const 
	{ 
		::EnterCriticalSection(&cs); 
	} 
	void unlock() const 
	{ 
		::LeaveCriticalSection(&cs); 
	} 
private: 
	mutable ::CRITICAL_SECTION cs; 
};