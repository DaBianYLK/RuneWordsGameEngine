#include "RwgeApplication.h"

#include "RwgeInputManager.h"
#include "RwgeD3d9RenderSystem.h"
#include "RwgeLog.h"
#include "RwgeAppWindow.h"

using namespace std;

RApplication::AppDelegate* RApplication::m_pDelegate = nullptr;

RApplication::RApplication() :
	m_pRenderSystem	(nullptr),
	m_pInputManager	(nullptr)
{
	m_hInstance = GetModuleHandle(nullptr);

	// ================ 初始化各个模块 ================
	m_pRenderSystem = new RD3d9RenderSystem();
	m_pInputManager = new RInputManager();

	// ================ 注册窗口类 ================
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
	wcex.lpszClassName = "RuneWordsGameEngine";
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	RegisterClassEx(&wcex);

	m_pDelegate->OnCreate();
}


RApplication::~RApplication()
{
	m_pDelegate->OnDestroy();
}

void RApplication::SetDelegate(AppDelegate* pDelegate)
{
	m_pDelegate = pDelegate;
}

void RApplication::Run()
{
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
			UpdateFrame();
		}
	}
}

HINSTANCE RApplication::GetHandle() const
{
	return m_hInstance;
}

float RApplication::GetTimeSinceLastFrame() const
{
	return m_FPSController.GetTimeSinceLastFrame();
}

RAppWindow* RApplication::CreateAppWindow(const char* pName)
{
	RAppWindow* pWindow = new RAppWindow(m_hInstance, pName);

	m_mapAppWindows.insert(make_pair(pWindow->GetName(), pWindow));
	m_pInputManager->RegAppWindow(pWindow);		// 在输入管理器注册窗口

	return pWindow;
}

RAppWindow* RApplication::GetAppWindow(const char* pName)
{
	map<string, RAppWindow*>::iterator itWindow = m_mapAppWindows.find(pName);
	if (itWindow == m_mapAppWindows.end())
	{
		return nullptr;
	}

	return itWindow->second;
}

RAppWindow* RApplication::GetPrimaryWindow()
{
	if (m_mapAppWindows.empty())
	{
		return nullptr;
	}

	return m_mapAppWindows.begin()->second;
}

bool RApplication::DestroyAppWindow(const char* pName)
{
	map<string, RAppWindow*>::iterator itWindow = m_mapAppWindows.find(pName);
	if (itWindow == m_mapAppWindows.end())
	{
		return false;
	}
	
	delete itWindow->second;
	m_mapAppWindows.erase(itWindow);

	return true;
}

float RApplication::GetCurrentFPS() const
{
	return m_FPSController.GetCurrentFPS();
}

LRESULT CALLBACK RApplication::AppWndProc(HWND hWnd, UINT u32Message, WPARAM wParam, LPARAM lParam)
{
	return RInputManager::GetInstance().HandleMessage(hWnd, u32Message, wParam, lParam);
}

void RApplication::UpdateFrame()
{
	m_FPSController.FrameStart();

	float f32DeltaTime = m_FPSController.GetTimeSinceLastFrame();

	// ================ 更新各个模块 ================
	m_pDelegate->OnUpdateFrame(f32DeltaTime);
	m_pRenderSystem->RenderOneFrame(f32DeltaTime);

	m_FPSController.FrameEnd();
}