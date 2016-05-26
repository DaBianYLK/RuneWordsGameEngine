#include "RwgeAppWindow.h"

#include "RwgeAssert.h"
#include "RwgeMessageDef.h"
#include <RwgeD3d9RenderTarget.h>
#include <RwgeD3d9RenderSystem.h>

using namespace std;
using namespace RwgeAppWindow;

const int DefaultWidth = 800;
const int DefaultHeight = 600;

RAppWindow::RAppWindow(HINSTANCE hAppInstance, const char* strName) :
	m_strName(strName),
	m_bShow(false),
	m_DisplayMode(EDM_Windowed),
	m_pRenderTarget(nullptr)
{
	// ==================== 设置窗口的默认大小 ====================
	HWND hDesk = GetDesktopWindow();
	RECT rect;
	GetWindowRect(hDesk, &rect);

	if (rect.right <= DefaultWidth || rect.bottom <= DefaultHeight)
	{
		m_s32X = 0;
		m_s32Y = 0;
		m_s32Width = rect.right;
		m_s32Height = rect.bottom;
	}
	else
	{
		m_s32X = (rect.right - DefaultWidth) / 2;
		m_s32Y = (rect.bottom - DefaultHeight) / 2;
		m_s32Width = DefaultWidth;
		m_s32Height = DefaultHeight;
	}

	// ==================== 创建窗口 ====================
	m_hWnd = CreateWindow(
		TEXT("RuneWordsGameEngine"),	// 窗口别名
		m_strName.c_str(),				// 窗口标题
		WS_OVERLAPPEDWINDOW,			// 窗口风格
		m_s32X,							// 窗口左上角X坐标（基于屏幕像素）
		m_s32Y,							// 窗口左上角Y坐标（基于屏幕像素）
		m_s32Width,						// 窗口宽度
		m_s32Height,					// 窗口高度
		nullptr,						// 父窗口
		nullptr,						// 菜单
		hAppInstance,					// 程序实例的句柄
		nullptr);						// 参数

	if (m_hWnd == nullptr)
	{
		RwgeErrorBox(TEXT("Create window failed!\nWindowName: %s\nApplication Instance: %d"), strName, hAppInstance);
	}
}

RAppWindow::~RAppWindow()
{
	OnDestroy();

	DestroyWindow(m_hWnd);
}

void RAppWindow::Resize(int s32X, int s32Y, int s32Width, int s32Height, EDisplayMode mode /* = EDM_Windowed */)
{
	switch (mode)
	{
	default:
	case EDM_Windowed:
		{
			SetWindowPos(
				m_hWnd,				// 窗口句柄
				nullptr,			// 窗口的显示层次，例：HWND_TOP
				s32X,				// 窗口左上角X坐标（基于屏幕像素）
				s32Y,				// 窗口左上角Y坐标（基于屏幕像素）
				s32Width,			// 窗口宽度
				s32Height,			// 窗口高度
				0);					// 其他操作，如显示、隐藏、刷新窗口等，例：SWP_SHOWWINDOW

			if (m_pRenderTarget != nullptr)
			{
				m_pRenderTarget->Resize(s32Width, s32Height, mode);
			}
		}
		break;

	case EDM_FakeFullScreen :
		// 非独占全屏模式需要使用Windows API设置
		{
			RECT rect;
			GetWindowRect(GetDesktopWindow(), &rect);

			// 隐藏窗口标题栏与边框
			SetWindowLong(
				m_hWnd,
				GWL_STYLE,				// 窗口属性
				WS_OVERLAPPED);			// 属性值

			SetWindowPos(
				m_hWnd,					// 窗口句柄
				HWND_TOPMOST,			// 窗口的显示层次
				0,						// 窗口左上角X坐标（基于屏幕像素）
				0,						// 窗口左上角Y坐标（基于屏幕像素）
				rect.right,				// 窗口宽度
				rect.bottom,			// 窗口高度
				SWP_NOSENDCHANGING);	// 其他操作，如显示、隐藏、刷新窗口等，例：SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE

			if (m_pRenderTarget != nullptr)
			{
				m_pRenderTarget->Resize(rect.right, rect.bottom, mode);
			}

			// 发送消息，调用监听器函数
			SendMessage(m_hWnd, WM_SIZE, WPARAM(SIZE_FAKE_FULLSCREEN), MAKELONG(rect.right, rect.bottom));
			SendMessage(m_hWnd, WM_MOVE, 0, 0);
		}
		break;

	case EDM_TrueFullScreen:
		// 独占全屏模式通过调用D3D API实现，
		{
			if (m_listWindowListeners.empty())
			{
				RwgeErrorBox(TEXT("There is no registered D3D Device. Set \"True Full Screen\" failed."));
			}

			RECT rect;
			GetWindowRect(GetDesktopWindow(), &rect);

			if (m_pRenderTarget != nullptr)
			{
				m_pRenderTarget->Resize(rect.right, rect.bottom, mode);
			}

			// 发送消息，调用监听器函数
			SendMessage(m_hWnd, WM_SIZE, WPARAM(SIZE_TRUE_FULLSCREEN), MAKELONG(rect.right, rect.bottom));
			SendMessage(m_hWnd, WM_MOVE, 0, 0);
		}
		break;
	}
}

void RAppWindow::Show(bool bShow /* = true */)
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
	}
}

void RAppWindow::OnShow()
{
	if (!m_bShow)
	{
		m_bShow = true;

		for (WindowListener* pWindowListener : m_listWindowListeners)
		{
			pWindowListener->OnShow();
		}
	}
}

void RAppWindow::OnHide()
{
	if (m_bShow)
	{
		m_bShow = false;

		for (WindowListener* pWindowListener : m_listWindowListeners)
		{
			pWindowListener->OnHide();
		}
	}
}

void RAppWindow::OnDestroy()
{
	for (WindowListener* pWindowListener : m_listWindowListeners)
	{
		pWindowListener->OnDestroy();
	}
}

void RAppWindow::OnMove(int s32X, int s32Y)
{
	if (m_s32X != s32X || m_s32Y != s32Y)
	{
		m_s32X = s32X;
		m_s32Y = s32Y;

		for (WindowListener* pWindowListener : m_listWindowListeners)
		{
			pWindowListener->OnMove(s32X, s32Y);
		}
	}
}

void RAppWindow::OnResize(int s32Width, int s32Height, RwgeAppWindow::EDisplayMode mode)
{
	if (m_s32Width != s32Width || m_s32Height != s32Height || m_DisplayMode != mode)
	{
		m_s32Width = s32Width;
		m_s32Height = s32Height;
		m_DisplayMode = mode;

		for (WindowListener* pWindowListener : m_listWindowListeners)
		{
			pWindowListener->OnResize(s32Width, s32Height, mode);
		}
	}
}

void RAppWindow::RegWindowListener(WindowListener* pListener)
{
#ifdef _DEBUG
	for (WindowListener* pOldListener : m_listWindowListeners)
	{
		RwgeAssert(pListener != pOldListener);
	}
#endif	// _DEBUG

	m_listWindowListeners.push_back(pListener);
}

void RAppWindow::DeRegWindowListener(WindowListener* pListener)
{
#ifdef _DEBUG
	bool bRegistered = false;
	for (WindowListener* pOldListener : m_listWindowListeners)
	{
		if (pListener == pOldListener)
		{
			bRegistered = true;
			break;
		}
	}
	RwgeAssert(bRegistered);
#endif // _DEBUG

	m_listWindowListeners.remove(pListener);
}

void RAppWindow::OnKeyUp(unsigned int u32Key)
{
	for (KeyBoardListener* pListener : m_listKeyBoardListeners)
	{
		pListener->OnKeyUp(u32Key);
	}
}

void RAppWindow::OnKeyDown(unsigned int u32Key)
{
	for (KeyBoardListener* pListener : m_listKeyBoardListeners)
	{
		pListener->OnKeyDown(u32Key);
	}
}

void RAppWindow::RegKeyBoardListener(KeyBoardListener* pListener)
{
#ifdef _DEBUG
	for (KeyBoardListener* pOldListener : m_listKeyBoardListeners)
	{
		RwgeAssert(pListener != pOldListener);
	}
#endif	// _DEBUG

	m_listKeyBoardListeners.push_back(pListener);
}

void RAppWindow::DeRegKeyBoardListener(KeyBoardListener* pListener)
{
#ifdef _DEBUG
	bool bRegistered = false;
	for (KeyBoardListener* pOldListener : m_listKeyBoardListeners)
	{
		if (pListener == pOldListener)
		{
			bRegistered = true;
			break;
		}
	}
	RwgeAssert(bRegistered);
#endif // _DEBUG

	m_listKeyBoardListeners.remove(pListener);
}

void RAppWindow::AddViewport(RD3d9Viewport* pViewport)
{
	m_pRenderTarget->AddViewport(pViewport);
}

void RAppWindow::RemoveViewport(RD3d9Viewport* pViewport)
{
	m_pRenderTarget->RemoveViewport(pViewport);
}

void RAppWindow::SetCamera(RCamera* pCamera)
{
	m_pRenderTarget->SetDefaultCamera(pCamera);
}
