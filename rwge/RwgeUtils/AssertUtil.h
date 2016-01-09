#pragma once

#include "Windows.h"
#include <stdio.h>

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
		MessageBoxA(nullptr, AssertMessage, "Rune Words Game Engine", 0); \
	}

// Show Error Box
#define ErrorBox(message, ...)\
	{\
		char ErrorTitle[256]; \
		sprintf_s(ErrorTitle, \
				  "Error:\n"); \
		char ErrorMessage[256]; \
		sprintf_s(ErrorMessage, \
				  message, \
				  __VA_ARGS__); \
		strcat_s(ErrorTitle, ErrorMessage); \
		MessageBoxA(nullptr, ErrorTitle, "Rune Words Game Engine", 0); \
	}