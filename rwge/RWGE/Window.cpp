#include "Window.h"

#include "Application.h"
#include "InputManager.h"
#include "Viewport.h"

Window::Window() {

}

Window::~Window() {

}

void Window::Initialize() {
	HINSTANCE hInstance = Application::GetInstance()->GetHandle();

	// Create window
	m_hWnd = CreateWindow("MainWindow", AppConfig::winName, WS_OVERLAPPEDWINDOW,
		AppConfig::winX, AppConfig::winY, AppConfig::winWidth, AppConfig::winHeight,
		NULL, NULL, hInstance, NULL);

	m_FullScreen = AppConfig::fullScreen;
	if (m_FullScreen) {
		HWND hDesk = GetDesktopWindow();
		RECT rect;
		GetWindowRect(hDesk, &rect);

		m_X = 0;
		m_Y = 0;
		m_Width = rect.right;
		m_Height = rect.bottom;

		SetWindowLong(m_hWnd, GWL_STYLE, WS_BORDER);
		SetWindowPos(m_hWnd, HWND_TOPMOST, m_X, m_Y, m_Width, m_Height, SWP_SHOWWINDOW);
	}
	else {
		m_X = AppConfig::winX;
		m_Y = AppConfig::winY;
		m_Width = AppConfig::winWidth;
		m_Height = AppConfig::winHeight;
	}

	ShowWindow(m_hWnd, SW_SHOW);
}

void Window::Update(float deltaTime) {
	m_pViewport->Update(deltaTime);
}

void Window::Resize(int x, int y, int width, int height) {

}

void Window::Cleanup() {

}

Viewport* Window::CreateViewport(Camera* pCamera) {
	m_pViewport = new Viewport(pCamera);

	return m_pViewport;
}

HWND Window::GetHandle() {
	return m_hWnd;
}

void Window::GetSize(int& x, int&y, int& width, int& height) {
	x = m_X;
	y = m_Y;
	width = m_Width;
	height = m_Height;
}

int Window::GetX() {
	return m_X;
}

int Window::GetY() {
	return m_Y;
}

int Window::GetWidth() {
	return m_Width;
}

int Window::GetHeight() {
	return m_Height;
}

bool Window::IsFullScreen() {
	return m_FullScreen;
}

Viewport* Window::GetViewport() {
	return m_pViewport;
}