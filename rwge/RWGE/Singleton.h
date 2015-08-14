#pragma once

#define NULL 0

template<class T> 
class Singleton {
public:
	static T* GetInstance() {
		if (!m_pInstance) {
			m_pInstance = new T();
		}

		return m_pInstance;
	}

protected:
	Singleton() {
		if (m_pInstance) {
			// ±¨´í
		}
	}

	~Singleton() {
		m_pInstance = NULL;
	}

private:
	static T* m_pInstance;
};

template<class T> T* Singleton<T>::m_pInstance = NULL;

