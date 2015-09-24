#pragma once

#include <Windows.h>

#include "RwgeClasses.h"
#include "AppConfig.h"
#include "Singleton.h"

class Application : public Singleton<Application> {
public:
	Application();
	~Application();

	void SetDelegate(AppDelegate* pDelegate);
	void Run();

	HINSTANCE GetHandle();

private:
	static LRESULT CALLBACK AppWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	void Initialize();
	void Update();
	void Cleanup();

private:
	HINSTANCE m_hInstance;

	AppDelegate* m_pDelegate;

	Graphics* m_pGraphics;
	InputManager* m_pInputManager;

	LARGE_INTEGER m_Frequency;		// 系统时钟频率
	LARGE_INTEGER m_LastCount;
	LARGE_INTEGER m_CurrentCount;
	float m_DeltaTime;

	LARGE_INTEGER m_StartUpdateCount;
	LARGE_INTEGER m_EndUpdateCount;
	float m_UpdateTime;
	float m_MinIntervalTime;
};

