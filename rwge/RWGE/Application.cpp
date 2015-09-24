#include "Application.h"

#include "AppConfig.h"
#include "AppDelegate.h"
#include "Graphics.h"
#include "InputManager.h"
#include "LogUtil.h"

Application::Application() {
	m_hInstance = GetModuleHandle(0);
	m_MinIntervalTime = 1.0f / AppConfig::maxFPS;
}


Application::~Application() {

}

void Application::SetDelegate(AppDelegate* pDelegate) {
	m_pDelegate = pDelegate;
}

void Application::Run() {
	// ��ʼ��Ӧ�ó���
	Initialize();

	// ������ѭ��
	MSG msg = { 0 };
	while (WM_QUIT != msg.message) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Update();
		}
	}

	// ����Ӧ�ó���
	Cleanup();
}

HINSTANCE Application::GetHandle() {
	return m_hInstance;
}

LRESULT CALLBACK Application::AppWndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam) {
	return InputManager::GetInstance()->MessageHandler(hwnd, umessage, wparam, lparam);
}

void Application::Initialize() {
	QueryPerformanceFrequency(&m_Frequency);	// ��ȡϵͳʱ��Ƶ�ʣ�ÿ�����д�����
	QueryPerformanceCounter(&m_LastCount);		// ��ȡ��ǰϵͳʱ�Ӽ���

	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = AppWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "MainWindow";
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&wcex);

	m_pGraphics = Graphics::GetInstance();
	m_pGraphics->Initialize();

	m_pInputManager = InputManager::GetInstance();
	
	m_pDelegate->Initialize();
}

void Application::Update() {
	// ������һ�α����������õ�ʱ�̵���������ȥ��ʱ��
	QueryPerformanceCounter(&m_CurrentCount);		// ��ȡ��ǰϵͳʱ�Ӽ���
	m_DeltaTime = (float)(m_CurrentCount.QuadPart - m_LastCount.QuadPart) / m_Frequency.QuadPart;
	m_LastCount = m_CurrentCount;

	// ��¼��ʼ����ʱ��ϵͳʱ�Ӽ���
	QueryPerformanceCounter(&m_StartUpdateCount);

	// ���¸�����ģ��
	m_pGraphics->Update(m_DeltaTime);
	m_pInputManager->Update(m_DeltaTime);
	m_pDelegate->Update(m_DeltaTime);

	// ��¼��������ʱ��ϵͳʱ�Ӽ���
	QueryPerformanceCounter(&m_EndUpdateCount);

	if (AppConfig::lockFPS) {
		// �������ʹ�õ�ʱ��
		m_UpdateTime = (float)(m_EndUpdateCount.QuadPart - m_StartUpdateCount.QuadPart) / m_Frequency.QuadPart;
		if (m_MinIntervalTime > m_UpdateTime) {
			Sleep((int)(m_MinIntervalTime - m_UpdateTime) * 1000);
		}
	}

	LOG("FPS = %d", (int)(1.0f / m_DeltaTime));
}

void Application::Cleanup() {
	LogUtil::GetInstance()->Cleanup();
}
