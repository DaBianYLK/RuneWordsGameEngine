#pragma once

#include "Windows.h"
#include <stdio.h>
#include <csignal>

// Show Assert Box when expression is FALSE
#define ASSERT(expression) \
	if (!expression) \
	{ \
		char AssertMessage[256]; \
		sprintf_s(AssertMessage, \
				  "Assert Failed!\nExpression: %s\nFile: %s\nLine: %d", \
				  #expression,\
				  __FILE__, \
				  __LINE__); \
		InterruptWindow(AssertMessage); \
	}

// Show Error Box
#define ErrorBox(message, ...)\
	{\
		char ErrorTitle[256]; \
		sprintf_s(ErrorTitle, \
				  "[ERROR]\n"); \
		char ErrorMessage[256]; \
		sprintf_s(ErrorMessage, \
				  message, \
				  __VA_ARGS__); \
		strcat_s(ErrorTitle, ErrorMessage); \
		InterruptWindow(ErrorTitle); \
	}

inline void InterruptWindow(const char* message)
{
	switch (MessageBoxA(GetActiveWindow(), message, "Rune Words Game Engine", MB_ABORTRETRYIGNORE))
	{
	case IDABORT:
		raise(SIGABRT); 
		exit(3); 
		break;

	case IDRETRY:
		_asm int 3
		break;

	case IDIGNORE:
		ShowWindow(GetActiveWindow(), SW_SHOW); 
		break; 
	};
}
	