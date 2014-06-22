#pragma once 
template <class T>  
class singleton 
{  
public:  
	static inline T& instance() 
	{  
		static T _instance;  
		return _instance;  
	}  
};