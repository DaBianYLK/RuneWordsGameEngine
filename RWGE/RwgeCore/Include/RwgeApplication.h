#pragma once

#include <Windows.h>
#include <RwgeSingleton.h>
#include "RwgeFpsController.h"
#include <map>
#include <string>

class RAppDelegate;
class RInputManager;
class RenderSystem;
class RWindow;

class RApplication : public Singleton<RApplication>
{
public:
	RApplication();
	~RApplication();

	void SetDelegate(RAppDelegate* pDelegate);
	void Run();

	HINSTANCE GetHandle() const;
	float GetTimeSinceLastFrame() const;

	RWindow* CreateDisplayWindow();
	RWindow* CreateDisplayWindow(const char* strName, bool bFullScreen = false);
	RWindow* CreateDisplayWindow(const char* strName, int x, int y, int width, int height);

	RWindow* GetDisplayWindow(const char* strName);

	float GetCurrentFPS() const;

private:
	static LRESULT CALLBACK AppWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
	void Initialize();
	void Release();
	void Update();

private:
	HINSTANCE m_hInstance;
	RFpsController m_FPSController;

	RAppDelegate* m_pDelegate;
	RenderSystem* m_pRenderSystem;
	RInputManager* m_pInputManager;

	std::map<std::string, RWindow*> m_mapDisplayWindows;
};

