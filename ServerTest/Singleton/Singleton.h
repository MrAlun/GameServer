/************************************************************************
不可控单件的定义
静态对象的创建和销毁由编译器决定
************************************************************************/
#pragma once

template<typename T>
class CSingleton
{
public:
	static T* Instance() {return &m_pIntance;}

protected:
	CSingleton(){}
	~CSingleton(){}
	static T m_pIntance;
};

template<typename T>
T CSingleton<T>::m_pIntance;