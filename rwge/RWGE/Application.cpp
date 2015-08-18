#include "Application.h"

#include "AppConfig.h"
#include "AppDelegate.h"
#include "Graphics.h"
#include "InputManager.h"

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

void Application::Initialize() {
	QueryPerformanceFrequency(&m_Frequency);	// ��ȡϵͳʱ��Ƶ�ʣ�ÿ�����д�����
	QueryPerformanceCounter(&m_LastCount);		// ��ȡ��ǰϵͳʱ�Ӽ���

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

	// �������ʹ�õ�ʱ��
	m_UpdateTime = (float)(m_EndUpdateCount.QuadPart - m_StartUpdateCount.QuadPart) / m_Frequency.QuadPart;
	if (m_MinIntervalTime > m_UpdateTime) {
		Sleep((int)(m_MinIntervalTime - m_UpdateTime) * 1000);
	}
}

void Application::Cleanup() {

}
