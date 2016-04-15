#include "RwgeApplication.h"

#include "RwgeAppDelegate.h"
#include "RwgeInputManager.h"
#include "RwgeRenderSystem.h"
#include "RwgeLog.h"
#include "RwgeWindow.h"
#include <tchar.h>

using namespace std;

RApplication::RApplication(): 
	m_pDelegate		(nullptr),
	m_pRenderSystem	(nullptr),
	m_pInputManager	(nullptr)
{
	m_hInstance = GetModuleHandle(nullptr);
}


RApplication::~RApplication()
{

}

void RApplication::SetDelegate(RAppDelegate* pDelegate)
{
	m_pDelegate = pDelegate;
}

void RApplication::Run()
{
	// 初始化应用程序
	Initialize();

	// 启动主循环
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Update();
		}
	}

	Release();
}

HINSTANCE RApplication::GetHandle() const
{
	return m_hInstance;
}

float RApplication::GetTimeSinceLastFrame() const
{
	return m_FPSController.GetTimeSinceLastFrame();
}

RWindow* RApplication::CreateDisplayWindow()
{
	RWindow* pWindow = new RWindow(m_hInstance);

	m_mapDisplayWindows.insert(make_pair(pWindow->GetName(), pWindow));

	return pWindow;
}

RWindow* RApplication::CreateDisplayWindow(const char* strName, bool bFullScreen /* = false */)
{
	RWindow* pWindow = new RWindow(m_hInstance, strName, bFullScreen);

	m_mapDisplayWindows.insert(make_pair(pWindow->GetName(), pWindow));

	return pWindow;
}

RWindow* RApplication::CreateDisplayWindow(const char* strName, int x, int y, int width, int height)
{
	RWindow* pWindow = new RWindow(m_hInstance, strName, x, y, width, height);

	m_mapDisplayWindows.insert(make_pair(pWindow->GetName(), pWindow));

	return pWindow;
}

RWindow* RApplication::GetDisplayWindow(const char* strName)
{
	return m_mapDisplayWindows.find(strName)->second;
}

float RApplication::GetCurrentFPS() const
{
	return m_FPSController.GetCurrentFPS();
}

LRESULT CALLBACK RApplication::AppWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return RInputManager::GetInstance().MessageHandler(hwnd, umessage, wparam, lparam);
}

void RApplication::Initialize()
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = AppWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = "MainWindow";
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	RegisterClassEx(&wcex);

	// 初始化各个单例模块
	m_pRenderSystem = new RenderSystem();
	m_pInputManager = new RInputManager();
	
	m_pDelegate->Initialize();
}

void RApplication::Update()
{
	m_FPSController.FrameStart();

	float fDeltaTime = m_FPSController.GetTimeSinceLastFrame();

	// 更新各个子模块
	m_pInputManager->Update(fDeltaTime);
	m_pDelegate->Update(fDeltaTime);
	m_pRenderSystem->RenderOneFrame(fDeltaTime);

	m_FPSController.FrameEnd();
}

void RApplication::Release()
{
	
}
