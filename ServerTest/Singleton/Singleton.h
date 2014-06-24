/************************************************************************
���ɿص����Ķ���
��̬����Ĵ����������ɱ���������
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