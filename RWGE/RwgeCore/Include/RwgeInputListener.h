/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-04-25
	DESC :	用户输入的监听器，用于响应用户输入事件
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <windef.h>

namespace RwgeInput
{
	class KeyBoardListener
	{
	public:
		KeyBoardListener()					{};
		virtual ~KeyBoardListener()			{};

		virtual void OnKeyUp(HWND hWnd, UINT u32Key)	{};
		virtual void OnKeyDown(HWND hWnd, UINT u32Key)	{};
	};
}
