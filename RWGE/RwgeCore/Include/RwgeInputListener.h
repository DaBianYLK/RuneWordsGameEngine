/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-04-25
	DESC :	�û�����ļ�������������Ӧ�û������¼�
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
