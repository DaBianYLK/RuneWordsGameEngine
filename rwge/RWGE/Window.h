#pragma once

#include <Windows.h>

#include "RwgeClasses.h"

class Window {
public:
	Window();
	~Window();

	void Initialize();
	void Update(float deltaTime);
	void Resize(int x, int y, int width, int height);
	void Cleanup();

	Viewport* CreateViewport(Camera* pCamera);

	HWND GetHandle();
	void GetSize(int& x, int&y, int& width, int& height);
	int GetX();
	int GetY();
	int GetWidth();
	int GetHeight();
	bool IsFullScreen();
	Viewport* GetViewport();

private:
	HWND m_hWnd;
	Viewport* m_pViewport;

	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;
	bool m_FullScreen;
};

