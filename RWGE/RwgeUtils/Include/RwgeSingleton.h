#pragma once

#include "RwgeAssert.h"
#include <typeinfo>

/*
关于本单例模板的几点说明：
1. 本单例模板的主要作用是保证单例的唯一性（程序运行时一定不会出现两个同类型的单例对象，否则程序会报错，所以不需要将派生类的构造函数设置为私有函数）
2. 所派生的单例对象的生命周期由开发者自行管理（开发者需要自行调用单例的构造和析构函数）
3. 本模板支持初始化与构造函数分离，所以使用时应该确保该单例存在且已经初始化
*/

template<typename T>
class Singleton
{
protected:
	Singleton()
	{
		// 构造函数执行时，单例指针一定要为空，保证单例的唯一性
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
	// 不允许生成单例的副本
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

