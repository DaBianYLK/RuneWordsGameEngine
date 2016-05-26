#include "RwgeInputManager.h"

#include "RwgeApplication.h"
#include "RwgeAppWindow.h"
#include "RwgeInputListener.h"
#include "RwgeMessageDef.h"

using namespace std;
using namespace RwgeAppWindow;

RInputManager::RInputManager()
{
	RwgeZeroMemory(m_bIsKeyDown, sizeof(bool) * MAX_KEY_COUNT);
}

RInputManager::~RInputManager()
{

}

LRESULT CALLBACK RInputManager::HandleMessage(HWND hWnd, UINT u32Message, WPARAM wParam, LPARAM lParam)
{
	switch (u32Message)
	{
	case WM_DESTROY:
	case WM_CLOSE:
		if (hWnd == RApplication::GetInstance().GetMainWindow()->GetHandle())
		{
			PostQuitMessage(0);
		}
		return 0;

	case WM_KEYDOWN:
		OnKeyDown(hWnd, static_cast<unsigned int>(wParam));
		return 0;

	case WM_KEYUP:
		OnKeyUp(hWnd, static_cast<unsigned int>(wParam));
		return 0;

	case WM_MOVE:
		OnWindowMove(hWnd, wParam, lParam);
		return 0;

	case WM_SIZE:
		OnWindowResize(hWnd, wParam, lParam);
		return 0;

	default:
		// All other messages pass to the message handler in the system class.
		return DefWindowProc(hWnd, u32Message, wParam, lParam);
	}
}

void RInputManager::OnKeyUp(HWND hWnd, UINT u32Key)
{
	m_bIsKeyDown[u32Key] = false;

	for (RwgeInput::KeyBoardListener* pListener : m_listKeyBoardListeners)
	{
		pListener->OnKeyUp(hWnd, u32Key);
	}

	RAppWindow* pWindow = GetAppWindow(hWnd);
	if (pWindow != nullptr)
	{
		pWindow->OnKeyUp(u32Key);
	}
}

void RInputManager::OnKeyDown(HWND hWnd, UINT u32Key)
{
	m_bIsKeyDown[u32Key] = true;

	for (RwgeInput::KeyBoardListener* pListener : m_listKeyBoardListeners)
	{
		pListener->OnKeyDown(hWnd, u32Key);
	}

	RAppWindow* pWindow = GetAppWindow(hWnd);
	if (pWindow != nullptr)
	{
		pWindow->OnKeyDown(u32Key);
	}
}

bool RInputManager::IsKeyDown(UINT u32Key)
{
	return m_bIsKeyDown[u32Key];
}

void RInputManager::OnWindowMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	RAppWindow* pWindow = GetAppWindow(hWnd);
	if (pWindow != nullptr)
	{
		pWindow->OnMove(LOWORD(lParam), HIWORD(lParam));
	}
}

void RInputManager::OnWindowResize(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	RAppWindow* pWindow = GetAppWindow(hWnd);
	if (pWindow != nullptr)
	{
		if (wParam == SIZE_TRUE_FULLSCREEN)
		{
			pWindow->OnResize(LOWORD(lParam), HIWORD(lParam), EDM_TrueFullScreen);
		}
		else if (wParam == SIZE_FAKE_FULLSCREEN)
		{
			pWindow->OnResize(LOWORD(lParam), HIWORD(lParam), EDM_FakeFullScreen);
		}
		else
		{
			pWindow->OnResize(LOWORD(lParam), HIWORD(lParam), EDM_Windowed);
		}
	}
}

void RInputManager::RegKeyBoardListener(RwgeInput::KeyBoardListener* pListener)
{
#ifdef _DEBUG
	for (RwgeInput::KeyBoardListener* pOldListener : m_listKeyBoardListeners)
	{
		RwgeAssert(pListener == pOldListener);
	}
#endif // _DEBUG

	m_listKeyBoardListeners.push_back(pListener);
}

void RInputManager::DeRegKeyBoardListener(RwgeInput::KeyBoardListener* pListener)
{
#ifdef _DEBUG
	bool bRegistered = false;
	for (RwgeInput::KeyBoardListener* pOldListener : m_listKeyBoardListeners)
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

void RInputManager::RegAppWindow(RAppWindow* pWindow)
{
	RwgeAssert(m_mapAppWindows.insert(make_pair(pWindow->GetHandle(), pWindow)).second);
}

void RInputManager::DeRegAppWindow(RAppWindow* pWindow)
{
	m_mapAppWindows.erase(pWindow->GetHandle());
}

RAppWindow* RInputManager::GetAppWindow(HWND hWnd)
{
	map<HWND, RAppWindow*>::iterator itWindow = m_mapAppWindows.find(hWnd);
	if (itWindow == m_mapAppWindows.end())
	{
		return nullptr;
	}

	return itWindow->second;
}
