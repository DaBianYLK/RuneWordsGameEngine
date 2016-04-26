#include "RwgeAppWindow.h"

#include "RwgeAssert.h"
#include "RwgeMessageDef.h"

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
	// ==================== ���ô��ڵ�Ĭ�ϴ�С ====================
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

	// ==================== �������� ====================
	m_hWnd = CreateWindow(
		TEXT("RuneWordsGameEngine"),	// ���ڱ���
		m_strName.c_str(),				// ���ڱ���
		WS_OVERLAPPEDWINDOW,			// ���ڷ��
		m_s32X,							// �������Ͻ�X���꣨������Ļ���أ�
		m_s32Y,							// �������Ͻ�Y���꣨������Ļ���أ�
		m_s32Width,						// ���ڿ��
		m_s32Height,					// ���ڸ߶�
		nullptr,						// ������
		nullptr,						// �˵�
		hAppInstance,					// ����ʵ���ľ��
		nullptr);						// ����

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
	case EDM_Windowed :
		SetWindowPos(
			m_hWnd,				// ���ھ��
			nullptr,			// ���ڵ���ʾ��Σ�����HWND_TOP
			s32X,				// �������Ͻ�X���꣨������Ļ���أ�
			s32Y,				// �������Ͻ�Y���꣨������Ļ���أ�
			s32Width,			// ���ڿ��
			s32Height,			// ���ڸ߶�
			0);					// ��������������ʾ�����ء�ˢ�´��ڵȣ�����SWP_SHOWWINDOW
		break;

	case EDM_FakeFullScreen :
		// �Ƕ�ռȫ��ģʽ��Ҫʹ��Windows API���ô���
		{
			RECT rect;
			GetWindowRect(GetDesktopWindow(), &rect);

			// ���ش��ڱ�������߿�
			SetWindowLong(
				m_hWnd,
				GWL_STYLE,				// ��������
				WS_OVERLAPPED);			// ����ֵ

			SetWindowPos(
				m_hWnd,					// ���ھ��
				HWND_TOPMOST,			// ���ڵ���ʾ���
				0,						// �������Ͻ�X���꣨������Ļ���أ�
				0,						// �������Ͻ�Y���꣨������Ļ���أ�
				rect.right,				// ���ڿ��
				rect.bottom,			// ���ڸ߶�
				SWP_NOSENDCHANGING);	// ��������������ʾ�����ء�ˢ�´��ڵȣ�����SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE

			SendMessage(m_hWnd, WM_SIZE, WPARAM(SIZE_FAKE_FULLSCREEN), MAKELONG(rect.right, rect.bottom));
			SendMessage(m_hWnd, WM_MOVE, 0, 0);
		}
		break;

	case EDM_TrueFullScreen:
		// ��ռȫ��ģʽͨ���ڻص������е���D3D APIʵ�֣�������Ϣ��������Device����
		{
			if (m_listWindowListeners.empty())
			{
				RwgeErrorBox(TEXT("There is no registered D3D Device. Set \"True Full Screen\" failed."));
			}

			RECT rect;
			GetWindowRect(GetDesktopWindow(), &rect);

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

void RAppWindow::SetRenderTarget(RD3dRenderTarget* pRenderTarget)
{
	m_pRenderTarget = pRenderTarget;
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
