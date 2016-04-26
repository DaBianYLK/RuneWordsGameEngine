/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��	
	AUTH :	���һ���																			   DATE : 2016-01-28
	DESC :
	1.	��������������ڻ�ȡ�û�������Ϣ��ַ���Ϣ
	2.	Windows ʹ�����޷�������[0, 255]�Լ����������Ϣ���б��룬�����Ӧ��ϵ����MSDN��������Virtual-Key Codes ��
	3.	��������������Ӧ�ó����յ���������Ϣ���ٰ����Ƿַ�����Ӧ�Ĵ���
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

