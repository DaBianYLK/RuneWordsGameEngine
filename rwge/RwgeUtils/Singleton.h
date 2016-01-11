#pragma once

#include "AssertUtil.h"

/*
���ڱ�����ģ��ļ���˵����
1. ������ģ�����Ҫ�����Ǳ�֤������Ψһ�ԣ���������ʱһ�������������ͬ���͵ĵ������󣬷������ᱨ�����Բ���Ҫ��������Ĺ��캯������Ϊ˽�к�����
2. �������ĵ�����������������ɿ��������й�����������Ҫ���е��õ����Ĺ��������������
3. ��ģ��֧�ֳ�ʼ���빹�캯�����룬����ʹ��ʱӦ��ȷ���õ����������Ѿ���ʼ��
*/

template<class T>
class Singleton
{
protected:
	Singleton()
	{
		// ���캯��ִ��ʱ������ָ��һ��ҪΪ��
		ASSERT(!m_pInstance);
		m_pInstance = static_cast<T*>(this);
	}

	~Singleton()
	{
		// ��������ִ��ʱ������ָ��һ������Ϊ��
		ASSERT(m_pInstance);
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
		// ���ʹ����δ��ʼ���ĵ��������Ͷ��Ծ���
		ASSERT(m_pInstance);
		return m_pInstance;
	}

	static T& GetInstance()
	{
		// ���ʹ����δ��ʼ���ĵ��������Ͷ��Ծ���
		ASSERT(m_pInstance);
		return *m_pInstance;
	}

protected:
	static T*	m_pInstance;
};

template<class T>
T* Singleton<T>::m_pInstance = nullptr;

