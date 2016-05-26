/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-01-08
	DESC :	
	1.	本单例模板的主要作用是保证单例的唯一性：程序运行时一定不会出现两个同类型的单例对象，否则程序会报错，所以不需
		要将派生类的构造函数设置为私有函数
	2.	所派生的单例对象的生命周期由开发者自行管理（开发者需要自行调用单例的构造和析构函数），调用时不执行有效性检查

   【UPDATE】
	AUTH :	大便一箩筐																			   DATE : 2016-04-26
	DESC :
	1.	增加懒汉模式的单例模板SingletonLazyMode：首次使用时构造单例，每次调用执行有效性检查
		ToDo：需要通过MemoryBarrier 和双重验证锁保证线程安全
	2.	增加饿汉模式的单例模板SingletonEagerMode：程序初始化时构造单例，整个程序运行期间一直存在，调用时不执行检查
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include "RwgeAssert.h"
#include "RwgeNoCopy.h"
#include <typeinfo>

template<typename T>
class Singleton : public NoCopy< Singleton<T> >
{
protected:
	Singleton()
	{
		// 构造函数执行时，单例指针一定要为空，保证单例的唯一性
		if (m_pInstance != nullptr)
		{
			RwgeErrorBox(TEXT("%s : Singleton can't be created twice."), typeid(T).name());
		}
		
		m_pInstance = static_cast<T*>(this);
	}

	virtual ~Singleton()
	{
		if (m_pInstance == nullptr)
		{
			RwgeErrorBox(TEXT("%s : Singleton can't be released before created."), typeid(T).name());
		}

		m_pInstance = nullptr;
	}

public:
	FORCE_INLINE static T& GetInstance()
	{
		// 指针有效性由开发者自行保证
		return *m_pInstance;
	}

protected:
	static T* m_pInstance;
};

template<class T>
T* Singleton<T>::m_pInstance = nullptr;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class SingletonLazyMode : public NoCopy< SingletonLazyMode<T> >
{
protected:
	SingletonLazyMode()
	{
		// 构造函数执行时，单例指针一定要为空，保证单例的唯一性
		if (m_pInstance != nullptr)
		{
			RwgeErrorBox(TEXT("%s : Singleton can't be created twice."), typeid(T).name());
		}
	}

	~SingletonLazyMode()
	{
		if (m_pInstance == nullptr)
		{
			RwgeErrorBox(TEXT("%s : Singleton can't be released before created."), typeid(T).name());
		}
	}

public:
	FORCE_INLINE static T& GetInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new T;
		}

		return *m_pInstance;
	}

protected:
	static T* m_pInstance;
};

template<class T>
T* SingletonLazyMode<T>::m_pInstance = nullptr;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class SingletonEagerMode : public NoCopy< SingletonEagerMode<T> >
{
protected:
	SingletonEagerMode()
	{
		if (m_bHasCreated)
		{
			RwgeErrorBox(TEXT("%s : Singleton can't be created twice."), typeid(T).name());
		}

		m_bHasCreated = true;
	}

	~SingletonEagerMode()
	{
		if (!m_bHasCreated)
		{
			RwgeErrorBox(TEXT("%s : Singleton can't be released before created."), typeid(T).name());
		}

		m_bHasCreated = false;
	}

public:
	FORCE_INLINE static T& GetInstance()
	{
		return m_Instance;
	}

protected:
	static T m_Instance;
	static bool m_bHasCreated;
};

template<class T>
T SingletonEagerMode<T>::m_Instance;

template<class T>
bool SingletonEagerMode<T>::m_bHasCreated = false;