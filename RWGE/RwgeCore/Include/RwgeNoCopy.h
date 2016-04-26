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
	FORCE_INLINE NoCopy(const NoCopy<T> &) {};
	FORCE_INLINE NoCopy<T>& operator=(const NoCopy<T> &) { return *this; };
};