/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-01-09
	DESC :	Debug工具
	1.	Assert只在Debug模式下执行，ErrorBox在Debug和Release模式下均执行
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <Windows.h>
#include <tchar.h>
#include <csignal>
#include <stdio.h>

#ifdef _DEBUG
// Show Assert Box when expression is FALSE
#	define RwgeAssert(expression) \
		if (!(expression)) \
			{ \
				TCHAR AssertMessage[256]; \
				_stprintf_s(AssertMessage, \
						  TEXT("Assert Failed!\nExpression: %s\nFile: %s\nLine: %d"), \
						  TEXT(#expression),\
						  TEXT(__FILE__), \
						  TEXT(__LINE__)); \
				InterruptWindow(AssertMessage); \
			}
#else
#	define RwgeAssert(expression)
#endif

// Show Error Box
#define RwgeErrorBox(message, ...)\
	{\
		TCHAR ErrorTitle[256]; \
		_stprintf_s(ErrorTitle, TEXT("[ERROR]\nFile: %s\nLine: %d\n"), TEXT(__FILE__), TEXT(__LINE__)); \
		TCHAR ErrorMessage[256]; \
		_stprintf_s(ErrorMessage, message, __VA_ARGS__); \
		_tcscat_s(ErrorTitle, ErrorMessage); \
		InterruptWindow(ErrorTitle); \
	}

inline void InterruptWindow(const TCHAR* message)
{
	switch (MessageBox(GetActiveWindow(), message, TEXT("Rune Words Game Engine"), MB_ABORTRETRYIGNORE))
	{
	case IDABORT:
		raise(SIGABRT); 
		exit(3); 
		break;

	case IDRETRY:
		_asm int 3			// int 3 为CPU中断指令
		break;

	case IDIGNORE:
		ShowWindow(GetActiveWindow(), SW_SHOW); 
		break; 
	};
}
	