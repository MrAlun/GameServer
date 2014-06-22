/************************************************************************
���ɿص����Ķ���
��̬����Ĵ����������ɱ���������
************************************************************************/
#pragma once

template<typename T>
class CSingleton
{
public:
	static T*		Instance( void ) {return &m_pIntance;}

protected:
	CSingleton( void ){}
	~CSingleton( void ){}
	static T m_pIntance;
};

template<typename T>
T CSingleton<T>::m_pIntance;