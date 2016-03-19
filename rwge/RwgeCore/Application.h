#pragma once

#include <Windows.h>
#include <Singleton.h>
#include "FPSController.h"
#include <map>
#include <string>

class AppDelegate;
class InputManager;
class RenderSystem;
class DisplayWindow;

class Application : public Singleton<Application>
{
public:
	Application();
	~Application();

	void SetDelegate(AppDelegate* pDelegate);
	void Run();

	HINSTANCE GetHandle() const;
	float GetTimeSinceLastFrame() const;

	DisplayWindow* CreateDisplayWindow();
	DisplayWindow* CreateDisplayWindow(const char* strName, bool bFullScreen = false);
	DisplayWindow* CreateDisplayWindow(const char* strName, int x, int y, int width, int height);

	DisplayWindow* GetDisplayWindow(const char* strName);

	float GetCurrentFPS() const;

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

	std::map<std::string, DisplayWindow*> m_mapDisplayWindows;
};

