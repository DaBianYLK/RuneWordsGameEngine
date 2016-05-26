/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-01-28
	DESC :	用于存放全局的定义
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#	define FORCE_INLINE		__forceinline
#else
#	define FORCE_INLINE		inline
#endif

// 虽然delete中已经包含了指针是否为0的判断，但某些情况下写操作的开销会远大于判断的开销
// 为了避免额外的写操作，在外层再多判一次
#define RwgeSafeDelete(pObject)			if (pObject) { delete (pObject); (pObject) = nullptr; }
#define RwgeSafeDeleteArray(pArray)		if (pArray)  { delete[] (pArray); (pArray) = nullptr; }
#define RwgeSafeRelease(pObject)		if (pObject) { (pObject)->Release(); (pObject) = nullptr; }

#define RwgeEqualMemory(pDestination, pSource, u32Size)		(!memcmp((pDestination), (pSource), (u32Size)))
#define RwgeMoveMemory(pDestination, pSource, u32Size)		memmove((pDestination), (pSource), (u32Size))	// 如果目标区域与源区域有重叠，保证重叠部分的数据能够正常被拷贝，否则效果等价与memcpy
#define RwgeCopyMemory(pDestination, pSource, u32Size)		memcpy((pDestination), (pSource), (u32Size))
#define RwgeFillMemory(pDestination, u32Size, s32Data)		memset((pDestination), (s32Data), (u32Size))
#define RwgeZeroMemory(pDestination, u32Size)				memset((pDestination), 0, (u32Size))

#define RwgeSymbolToStringBegin(symbolValue)		switch (symbolValue)
#define RwgeSymbolToString(symbol)					case symbol:	return #symbol
#define RwgeSymbolToStringDefault()					default:		return "Undefined"


