#pragma once

#include "AssertUtil.h"

/*
关于本单例模板的几点说明：
1. 本单例模板的主要作用是保证单例的唯一性（程序运行时一定不会出现两个同类型的单例对象，否则程序会报错，所以不需要将派生类的构造函数设置为私有函数）
2. 所派生的单例对象的生命周期由开发者自行管理（开发者需要自行调用单例的构造和析构函数）
3. 本模板支持初始化与构造函数分离，所以使用时应该确保该单例存在且已经初始化
*/

template<class T>
class Singleton
{
protected:
	Singleton()
	{
		// 构造函数执行时，单例指针一定要为空
		ASSERT(!m_pInstance);
		m_pInstance = static_cast<T*>(this);
	}

	~Singleton()
	{
		// 析构函数执行时，单例指针一定不能为空
		ASSERT(m_pInstance);
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
		// 如果使用了未初始化的单例，发送断言警告
		ASSERT(m_pInstance);
		return m_pInstance;
	}

	static T& GetInstance()
	{
		// 如果使用了未初始化的单例，发送断言警告
		ASSERT(m_pInstance);
		return *m_pInstance;
	}

protected:
	static T*	m_pInstance;
};

template<class T>
T* Singleton<T>::m_pInstance = nullptr;

