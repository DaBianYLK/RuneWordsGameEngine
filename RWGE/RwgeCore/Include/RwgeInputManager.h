/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】	
	AUTH :	大便一箩筐																			   DATE : 2016-01-28
	DESC :
	1.	输入管理器，用于获取用户输入信息与分发消息
	2.	Windows 使用了无符号整数[0, 255]对键盘与鼠标消息进行编码，具体对应关系请在MSDN中搜索“Virtual-Key Codes ”
	3.	输入管理器会监听应用程序收到的所有消息，再把它们分发给相应的窗口
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <RwgeSingleton.h>
#include <list>
#include <map>
#include "RwgeObject.h"

class RAppWindow;

namespace RwgeInput
{
	class KeyBoardListener;
}

#define MAX_KEY_COUNT	256

class RInputManager : 
	public RObject,
	public Singleton<RInputManager>
{
public:
	RInputManager();
	~RInputManager();

	LRESULT CALLBACK HandleMessage(HWND hWnd, UINT u32Message, WPARAM wParam, LPARAM lParam);

	void OnKeyUp(HWND hWnd, UINT u32Key);
	void OnKeyDown(HWND hWnd, UINT u32Key);
	bool IsKeyDown(UINT u32Key);

	void OnWindowMove(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void OnWindowResize(HWND hWnd, WPARAM wParam, LPARAM lParam);

	void RegKeyBoardListener(RwgeInput::KeyBoardListener* pListener);
	void DeRegKeyBoardListener(RwgeInput::KeyBoardListener* pListener);

	void RegAppWindow(RAppWindow* pWindow);
	void DeRegAppWindow(RAppWindow* pWindow);
	RAppWindow* GetAppWindow(HWND hWnd);

private:
	bool m_bIsKeyDown[MAX_KEY_COUNT];

	std::list<RwgeInput::KeyBoardListener*>	m_listKeyBoardListeners;
	std::map<HWND, RAppWindow*>		m_mapAppWindows;
};

