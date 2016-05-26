/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��
	AUTH :	���һ���																			   DATE : 2016-04-26
	DESC :	�̳д�ģ����಻�ɸ���
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include "RwgeAssert.h"
#include "RwgeCoreDef.h"
#include <typeinfo>

template<typename T>
class NoCopy
{
public:
	NoCopy() {};
	virtual ~NoCopy() {};

private:
	// ���������ɸ���
	FORCE_INLINE NoCopy(const NoCopy<T> &) { RwgeErrorBox("[%s] can't be copied.", typeid(T).name()); };
	FORCE_INLINE virtual NoCopy<T>& operator=(const NoCopy<T> &) { RwgeErrorBox("[%s] can't be copied.", typeid(T).name()); return *this; };
};