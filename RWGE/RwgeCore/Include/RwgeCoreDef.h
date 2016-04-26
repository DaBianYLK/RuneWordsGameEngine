/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-01-28
	DESC :	用于存放全局的定义
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#if defined(_WIN32) || defined(_WIN64)
#	define FORCE_INLINE		__forceinline
#else
#	define FORCE_INLINE		inline
#endif

// 虽然delete中已经包含了指针是否为0的判断，但某些情况下写操作的开销会远大于判断的开销
// 为了避免额外的写操作，在外层再多判一次
#define SafeDelete(ptr) if (ptr) { delete ptr; ptr = nullptr; }

