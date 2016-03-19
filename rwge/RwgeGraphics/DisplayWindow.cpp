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
	m_nX = x;
	m_nY = y;
	m_nWidth = width;
	m_nHeight = height;
	m_bFullScreen = false;

	Init(hAppInstance);
}

DisplayWindow::DisplayWindow(HINSTANCE hAppInstance, const char* strName, int x, int y, int width, int height)
{
	m_hWnd = nullptr;
	m_strName = strName;
	m_nX = x;
	m_nY = y;
	m_nWidth = width;
	m_nHeight = height;
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
		m_nX = 0;
		m_nY = 0;
		m_nWidth = rect.right;
		m_nHeight = rect.bottom;
	}
	else
	{
		m_nX = (rect.right - DefaultWidth) / 2;
		m_nY = (rect.bottom - DefaultHeight) / 2;
		m_nWidth = DefaultWidth;
		m_nHeight = DefaultHeight;
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
		m_nX,						// 窗口左上角X坐标（基于屏幕像素）
		m_nY,						// 窗口左上角Y坐标（基于屏幕像素）
		m_nWidth,					// 窗口宽度
		m_nHeight,					// 窗口高度
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

	m_nX = 0;
	m_nY = 0;
	m_nWidth = rect.right;
	m_nHeight = rect.bottom;

	SetWindowLong(m_hWnd, GWL_STYLE, WS_BORDER);
	SetWindowPos(m_hWnd, HWND_TOPMOST, m_nX, m_nY, m_nWidth, m_nHeight, SWP_SHOWWINDOW);

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

	m_nX = x;
	m_nY = y;
	m_nWidth = width;
	m_nHeight = height;

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

const std::string& DisplayWindow::GetName() const
{
	return m_strName;
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
	x = m_nX;
	y = m_nY;
	width = m_nWidth;
	height = m_nHeight;
}

int DisplayWindow::GetX() const
{
	return m_nX;
}

int DisplayWindow::GetY() const
{
	return m_nY;
}

int DisplayWindow::GetWidth() const
{
	return m_nWidth;
}

int DisplayWindow::GetHeight() const
{
	return m_nHeight;
}

bool DisplayWindow::IsFullScreen() const
{
	return m_bFullScreen;
}
