/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-04-26
	DESC :	继承此模板的类不可复制
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
	// 不允许生成副本
	FORCE_INLINE NoCopy(const NoCopy<T> &) { RwgeErrorBox("[%s] can't be copied.", typeid(T).name()); };
	FORCE_INLINE virtual NoCopy<T>& operator=(const NoCopy<T> &) { RwgeErrorBox("[%s] can't be copied.", typeid(T).name()); return *this; };
};