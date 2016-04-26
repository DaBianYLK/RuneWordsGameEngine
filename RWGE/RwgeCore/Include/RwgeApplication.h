/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-01-08
	DESC :	
	1.	客户端的主循环入口类，管理了程序的生命周期，开发者可以继承AppDelegate实现对程序生命周期的监听
	2.	负责引擎各个子模块的初始化与销毁工作
	3.	程序创建的第一个窗口称为PrimaryWindow，一旦它被关闭，整个程序就会退出
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <Windows.h>
#include <RwgeSingleton.h>
#include "RwgeFpsController.h"
#include <map>
#include <string>

#include "RwgeObject.h"

class RAppDelegate;
class RInputManager;
class RD3d9RenderSystem;
class RAppWindow;

class RApplication : 
	public RObject,
	public Singleton<RApplication>
{
public:
	class AppDelegate
	{
	public:
		AppDelegate()									{};
		virtual ~AppDelegate()							{};

		virtual void OnCreate()							{};
		virtual void OnUpdateFrame(float f32DeltaTime)	{};
		virtual void OnDestroy()						{};
	};

public:
	RApplication();
	~RApplication();

	static void SetDelegate(AppDelegate* pDelegate);
	void Run();

	HINSTANCE GetHandle() const;
	float GetTimeSinceLastFrame() const;

	RAppWindow* CreateAppWindow(const char* pName);
	RAppWindow* GetAppWindow(const char* pName);
	RAppWindow* GetPrimaryWindow();
	bool DestroyAppWindow(const char* pName);

	float GetCurrentFPS() const;

private:
	static LRESULT CALLBACK AppWndProc(HWND hWnd, UINT u32Message, WPARAM wParam, LPARAM lParam);
	void UpdateFrame();

private:
	HINSTANCE m_hInstance;
	RFpsController m_FPSController;

	static AppDelegate* m_pDelegate;
	RD3d9RenderSystem* m_pRenderSystem;
	RInputManager* m_pInputManager;

	std::map<std::string, RAppWindow*> m_mapAppWindows;
};

