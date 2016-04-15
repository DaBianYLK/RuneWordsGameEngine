#pragma once

#include "RwgeAssert.h"
#include <typeinfo>

/*
���ڱ�����ģ��ļ���˵����
1. ������ģ�����Ҫ�����Ǳ�֤������Ψһ�ԣ���������ʱһ�������������ͬ���͵ĵ������󣬷������ᱨ�����Բ���Ҫ��������Ĺ��캯������Ϊ˽�к�����
2. �������ĵ�����������������ɿ��������й�����������Ҫ���е��õ����Ĺ��������������
3. ��ģ��֧�ֳ�ʼ���빹�캯�����룬����ʹ��ʱӦ��ȷ���õ����������Ѿ���ʼ��
*/

template<typename T>
class Singleton
{
protected:
	Singleton()
	{
		// ���캯��ִ��ʱ������ָ��һ��ҪΪ�գ���֤������Ψһ��
		if (m_pInstance != nullptr)
		{
			ErrorBox("%s : Singleton instance can't be initialized twice.", typeid(T).name());
		}
		
		m_pInstance = static_cast<T*>(this);
	}

	~Singleton()
	{
		if (m_pInstance == nullptr)
		{
			ErrorBox("%s : Singleton instance can't be released before initialization.", typeid(T).name());
		}

		m_pInstance = nullptr;
	}


private:
	// ���������ɵ����ĸ���
	Singleton(const Singleton<T> &) {};
	Singleton& operator=(const Singleton<T> &) 
	{ 
		return *m_pInstance; 
	};

public:
	static T* GetInstancePtr()
	{
		if (m_pInstance == nullptr)
		{
			ErrorBox("%s : Singleton instance is used before initialization.", typeid(T).name());
		}
		
		return m_pInstance;
	}

	static T& GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			ErrorBox("%s : Singleton instance is used before initialization.", typeid(T).name());
		}

		return *m_pInstance;
	}

protected:
	static T*	m_pInstance;
};

template<class T>
T* Singleton<T>::m_pInstance = nullptr;

