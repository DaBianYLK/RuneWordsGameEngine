#include "DisplayWindow.h"

#include "AssertUtil.h"

const char* DefaultName = "Rune Words Game Engine";
const int DefaultWidth = 800;
const int DefaultHeight = 600;

DisplayWindow::DisplayWindow(HINSTANCE hAppInstance, bool bFullScreen)
{
	m_hWnd = nullptr;
	m_strName = DefaultName;
	SetDefaultSize();
	m_bFullScreen = bFullScreen;

	Init(hAppInstance);
}

DisplayWindow::DisplayWindow(HINSTANCE hAppInstance, const char* strName, bool bFullScreen)
{
	m_hWnd = nullptr;
	m_strName = strName;
	SetDefaultSize();
	m_bFullScreen = bFullScreen;

	Init(hAppInstance);
}

DisplayWindow::DisplayWindow(HINSTANCE hAppInstance, int x, int y, int width, int height)
{
	m_hWnd = nullptr;
	m_strName = DefaultName;
	m_sX = x;
	m_sY = y;
	m_sWidth = width;
	m_sHeight = height;
	m_bFullScreen = false;

	Init(hAppInstance);
}

DisplayWindow::DisplayWindow(HINSTANCE hAppInstance, const char* strName, int x, int y, int width, int height)
{
	m_hWnd = nullptr;
	m_strName = strName;
	m_sX = x;
	m_sY = y;
	m_sWidth = width;
	m_sHeight = height;
	m_bFullScreen = false;

	Init(hAppInstance);
}

DisplayWindow::~DisplayWindow()
{
	Release();
}

void DisplayWindow::SetDefaultSize()
{
	HWND hDesk = GetDesktopWindow();
	RECT rect;
	GetWindowRect(hDesk, &rect);

	if (rect.right <= DefaultWidth || rect.bottom <= DefaultHeight)
	{
		m_sX = 0;
		m_sY = 0;
		m_sWidth = rect.right;
		m_sHeight = rect.bottom;
	}
	else
	{
		m_sX = (rect.right - DefaultWidth) / 2;
		m_sY = (rect.bottom - DefaultHeight) / 2;
		m_sWidth = DefaultWidth;
		m_sHeight = DefaultHeight;
	}
}

void DisplayWindow::Init(HINSTANCE hAppInstance)
{
	ASSERT(!m_hWnd);

	// Create window
	m_hWnd = CreateWindowA(	
		"MainWindow",				// 窗口别名
		m_strName.c_str(),			// 窗口标题
		WS_OVERLAPPEDWINDOW,		// 窗口风格
		m_sX,						// 窗口左上角X坐标（基于屏幕像素）
		m_sY,						// 窗口左上角Y坐标（基于屏幕像素）
		m_sWidth,					// 窗口宽度
		m_sHeight,					// 窗口高度
		nullptr,					// 父窗口
		nullptr,					// 菜单
		hAppInstance,				// 程序实例的句柄
		nullptr);					// 参数

	if (m_bFullScreen)
	{
		// 设置全屏的过程中会令窗口显示
		SetFullScreen();
	}
	else
	{
		ShowWindow(m_hWnd, SW_SHOW);
	}
}

void DisplayWindow::SetFullScreen()
{
	HWND hDesk = GetDesktopWindow();
	RECT rect;
	GetWindowRect(hDesk, &rect);

	m_sX = 0;
	m_sY = 0;
	m_sWidth = rect.right;
	m_sHeight = rect.bottom;

	SetWindowLong(m_hWnd, GWL_STYLE, WS_BORDER);
	SetWindowPos(m_hWnd, HWND_TOPMOST, m_sX, m_sY, m_sWidth, m_sHeight, SWP_SHOWWINDOW);

	m_bFullScreen = true;
}

void DisplayWindow::Resize(int x, int y, int width, int height)
{
	SetWindowPos(
		m_hWnd,				// 窗口句柄
		HWND_TOP,			// 窗口的显示层次
		x,					// 窗口左上角X坐标（基于屏幕像素）
		y,					// 窗口左上角Y坐标（基于屏幕像素）
		width,				// 窗口宽度
		height,				// 窗口高度
		SWP_SHOWWINDOW);	// 其他操作，如显示、隐藏、刷新窗口等

	m_sX = x;
	m_sY = y;
	m_sWidth = width;
	m_sHeight = height;

	m_bFullScreen = false;
}

void DisplayWindow::Show(bool bShow /* = true */)
{
	if (bShow != m_bShow)
	{
		if (bShow)
		{
			ShowWindow(m_hWnd, SW_SHOW);
		}
		else
		{
			ShowWindow(m_hWnd, SW_HIDE);
		}

		m_bShow = bShow;
	}
}

void DisplayWindow::Hide()
{
	Show(false);
}

void DisplayWindow::Release()
{
	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;
}

HWND DisplayWindow::GetHandle() const
{
	return m_hWnd;
}

void DisplayWindow::GetSize(int& x, int&y, int& width, int& height) const
{
	x = m_sX;
	y = m_sY;
	width = m_sWidth;
	height = m_sHeight;
}

int DisplayWindow::GetX() const
{
	return m_sX;
}

int DisplayWindow::GetY() const
{
	return m_sY;
}

int DisplayWindow::GetWidth() const
{
	return m_sWidth;
}

int DisplayWindow::GetHeight() const
{
	return m_sHeight;
}

bool DisplayWindow::IsFullScreen() const
{
	return m_bFullScreen;
}
