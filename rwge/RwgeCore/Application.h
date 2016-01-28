#pragma once

#include <Windows.h>
#include "Singleton.h"
#include "FPSController.h"

class AppDelegate;
class InputManager;
class RenderSystem;

class Application : public Singleton<Application>
{
public:
	Application();
	~Application();

	void SetDelegate(AppDelegate* pDelegate);
	void Run();

	HINSTANCE GetHandle() const;
	float GetTimeSinceLastFrame() const;

private:
	static LRESULT CALLBACK AppWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	void Initialize();
	void Release();
	void Update();

private:
	HINSTANCE m_hInstance;
	FPSController m_FPSController;

	AppDelegate* m_pDelegate;
	RenderSystem* m_pRenderSystem;
	InputManager* m_pInputManager;
};

