/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��
	AUTH :	���һ���																			   DATE : 2016-01-28
	DESC :	���ڴ��ȫ�ֵĶ���
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <string.h>

#if defined(_WIN32) || defined(_WIN64)
#	define FORCE_INLINE		__forceinline
#else
#	define FORCE_INLINE		inline
#endif

// ��Ȼdelete���Ѿ�������ָ���Ƿ�Ϊ0���жϣ���ĳЩ�����д�����Ŀ�����Զ�����жϵĿ���
// Ϊ�˱�������д������������ٶ���һ��
#define RwgeSafeDelete(pObject)			if (pObject) { delete (pObject); (pObject) = nullptr; }
#define RwgeSafeDeleteArray(pArray)		if (pArray)  { delete[] (pArray); (pArray) = nullptr; }
#define RwgeSafeRelease(pObject)		if (pObject) { (pObject)->Release(); (pObject) = nullptr; }

#define RwgeEqualMemory(pDestination, pSource, u32Size)		(!memcmp((pDestination), (pSource), (u32Size)))
#define RwgeMoveMemory(pDestination, pSource, u32Size)		memmove((pDestination), (pSource), (u32Size))	// ���Ŀ��������Դ�������ص�����֤�ص����ֵ������ܹ�����������������Ч���ȼ���memcpy
#define RwgeCopyMemory(pDestination, pSource, u32Size)		memcpy((pDestination), (pSource), (u32Size))
#define RwgeFillMemory(pDestination, u32Size, s32Data)		memset((pDestination), (s32Data), (u32Size))
#define RwgeZeroMemory(pDestination, u32Size)				memset((pDestination), 0, (u32Size))

#define RwgeSymbolToStringBegin(symbolValue)		switch (symbolValue)
#define RwgeSymbolToString(symbol)					case symbol:	return #symbol
#define RwgeSymbolToStringDefault()					default:		return "Undefined"


