#include "RwgeWindow.h"

#include "RwgeAssert.h"

const char* DefaultName = "Rune Words Game Engine";
const int DefaultWidth = 1440;
const int DefaultHeight = 900;

RWindow::RWindow(HINSTANCE hAppInstance, bool bFullScreen)
{
	m_hWnd = nullptr;
	m_strName = DefaultName;
	SetDefaultSize();
	m_bFullScreen = bFullScreen;
	m_bShow = false;

	Init(hAppInstance);
}

RWindow::RWindow(HINSTANCE hAppInstance, const char* strName, bool bFullScreen)
{
	m_hWnd = nullptr;
	m_strName = strName;
	SetDefaultSize();
	m_bFullScreen = bFullScreen;
	m_bShow = false;

	Init(hAppInstance);
}

RWindow::RWindow(HINSTANCE hAppInstance, int x, int y, int width, int height)
{
	m_hWnd = nullptr;
	m_strName = DefaultName;
	m_nX = x;
	m_nY = y;
	m_nWidth = width;
	m_nHeight = height;
	m_bFullScreen = false;
	m_bShow = false;

	Init(hAppInstance);
}

RWindow::RWindow(HINSTANCE hAppInstance, const char* strName, int x, int y, int width, int height)
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

RWindow::~RWindow()
{
	Release();
}

void RWindow::SetDefaultSize()
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

void RWindow::Init(HINSTANCE hAppInstance)
{
	ASSERT(!m_hWnd);

	// Create window
	m_hWnd = CreateWindowA(	
		"MainWindow",				// ���ڱ���
		m_strName.c_str(),			// ���ڱ���
		WS_OVERLAPPEDWINDOW,		// ���ڷ��
		m_nX,						// �������Ͻ�X���꣨������Ļ���أ�
		m_nY,						// �������Ͻ�Y���꣨������Ļ���أ�
		m_nWidth,					// ���ڿ��
		m_nHeight,					// ���ڸ߶�
		nullptr,					// ������
		nullptr,					// �˵�
		hAppInstance,				// ����ʵ���ľ��
		nullptr);					// ����

	if (m_bFullScreen)
	{
		// ����ȫ���Ĺ����л������ʾ
		SetFullScreen();
	}
	else
	{
		ShowWindow(m_hWnd, SW_SHOW);
	}
}

void RWindow::SetFullScreen()
{
	HWND hDesk = GetDesktopWindow();
	RECT rect;
	GetWindowRect(hDesk, &rect);

	m_nX = 0;
	m_nY = 0;
	m_nWidth = rect.right;
	m_nHeight = rect.bottom;

	SetWindowLong(
		m_hWnd, 
		GWL_STYLE,			// ��������
		WS_OVERLAPPED);		// ����ֵ
	SetWindowPos(m_hWnd, HWND_TOPMOST, m_nX, m_nY, m_nWidth, m_nHeight, 0);	// ���һ��������SWP_NOSIZE | SWP_NOMOVE

	m_bFullScreen = true;
}

void RWindow::Resize(int x, int y, int width, int height)
{
	SetWindowPos(
		m_hWnd,				// ���ھ��
		HWND_TOP,			// ���ڵ���ʾ���
		x,					// �������Ͻ�X���꣨������Ļ���أ�
		y,					// �������Ͻ�Y���꣨������Ļ���أ�
		width,				// ���ڿ��
		height,				// ���ڸ߶�
		SWP_SHOWWINDOW);	// ��������������ʾ�����ء�ˢ�´��ڵ�

	m_nX = x;
	m_nY = y;
	m_nWidth = width;
	m_nHeight = height;

	m_bFullScreen = false;
}

void RWindow::Show(bool bShow /* = true */)
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

void RWindow::Hide()
{
	Show(false);
}

const std::string& RWindow::GetName() const
{
	return m_strName;
}

void RWindow::Release()
{
	DestroyWindow(m_hWnd);
	m_hWnd = nullptr;
}

HWND RWindow::GetHandle() const
{
	return m_hWnd;
}

void RWindow::GetSize(int& x, int&y, int& width, int& height) const
{
	x = m_nX;
	y = m_nY;
	width = m_nWidth;
	height = m_nHeight;
}

int RWindow::GetX() const
{
	return m_nX;
}

int RWindow::GetY() const
{
	return m_nY;
}

int RWindow::GetWidth() const
{
	return m_nWidth;
}

int RWindow::GetHeight() const
{
	return m_nHeight;
}

bool RWindow::IsFullScreen() const
{
	return m_bFullScreen;
}
