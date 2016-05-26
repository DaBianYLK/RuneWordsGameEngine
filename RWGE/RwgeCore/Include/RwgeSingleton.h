/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��
	AUTH :	���һ���																			   DATE : 2016-01-08
	DESC :	
	1.	������ģ�����Ҫ�����Ǳ�֤������Ψһ�ԣ���������ʱһ�������������ͬ���͵ĵ������󣬷������ᱨ�����Բ���
		Ҫ��������Ĺ��캯������Ϊ˽�к���
	2.	�������ĵ�����������������ɿ��������й�����������Ҫ���е��õ����Ĺ��������������������ʱ��ִ����Ч�Լ��

   ��UPDATE��
	AUTH :	���һ���																			   DATE : 2016-04-26
	DESC :
	1.	��������ģʽ�ĵ���ģ��SingletonLazyMode���״�ʹ��ʱ���쵥����ÿ�ε���ִ����Ч�Լ��
		ToDo����Ҫͨ��MemoryBarrier ��˫����֤����֤�̰߳�ȫ
	2.	���Ӷ���ģʽ�ĵ���ģ��SingletonEagerMode�������ʼ��ʱ���쵥�����������������ڼ�һֱ���ڣ�����ʱ��ִ�м��
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
		// ���캯��ִ��ʱ������ָ��һ��ҪΪ�գ���֤������Ψһ��
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
		// ָ����Ч���ɿ��������б�֤
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
		// ���캯��ִ��ʱ������ָ��һ��ҪΪ�գ���֤������Ψһ��
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