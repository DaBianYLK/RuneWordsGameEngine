#include "Application.h"

#include "AppDelegate.h"
#include "InputManager.h"
#include "RenderSystem.h"
#include "LogUtil.h"

Application::Application(): 
	m_pDelegate		(nullptr),
	m_pRenderSystem	(nullptr),
	m_pInputManager	(nullptr)
{
	m_hInstance = GetModuleHandle(0);
}


Application::~Application()
{

}

void Application::SetDelegate(AppDelegate* pDelegate)
{
	m_pDelegate = pDelegate;
}

void Application::Run()
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

HINSTANCE Application::GetHandle() const
{
	return m_hInstance;
}

float Application::GetTimeSinceLastFrame() const
{
	return m_FPSController.GetTimeSinceLastFrame();
}

LRESULT CALLBACK Application::AppWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return InputManager::GetInstance().MessageHandler(hwnd, umessage, wparam, lparam);
}

void Application::Initialize()
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

	m_pRenderSystem = RenderSystem::GetInstancePtr();
	m_pInputManager = InputManager::GetInstancePtr();
	
	m_pDelegate->Initialize();
}

void Application::Update()
{
	m_FPSController.FrameStart();

	float fDeltaTime = m_FPSController.GetTimeSinceLastFrame();

	// 更新各个子模块
	m_pInputManager->Update(fDeltaTime);
	m_pDelegate->Update(fDeltaTime);
	m_pRenderSystem->RenderOneFrame(fDeltaTime);

	m_FPSController.FrameEnd();
}

void Application::Release()
{
	
}
