#include "RwgeInputManager.h"

#include "RwgeInputListener.h"
#include "RwgeApplication.h"

RInputManager::RInputManager()
{
	memset(m_IsKeyDown, 0, sizeof(bool)* 256);
}

RInputManager::~RInputManager()
{

}

void RInputManager::Initialize()
{

}

void RInputManager::Update(float deltaTime)
{

}

void RInputManager::Cleanup()
{

}

LRESULT CALLBACK RInputManager::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch (umsg)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

		// Check if the window is being closed.
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		// If a key is pressed send it to the input object so it can record that state.
		KeyDown(static_cast<unsigned int>(wparam));
		return 0;

		// Check if a key has been released on the keyboard.
	case WM_KEYUP:
		// If a key is released then send it to the input object so it can unset the state for that key.
		KeyUp(static_cast<unsigned int>(wparam));
		return 0;

		// All other messages pass to the message handler in the system class.
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

void RInputManager::KeyUp(unsigned int key)
{
	m_IsKeyDown[key] = false;

	std::list<RInputListener*>::iterator listener = m_InputListeners.begin();
	while (listener != m_InputListeners.end())
	{
		(*listener)->OnKeyUp(key);

		++listener;
	}
}

void RInputManager::KeyDown(unsigned int key)
{
	m_IsKeyDown[key] = true;

	std::list<RInputListener*>::iterator listener = m_InputListeners.begin();
	while (listener != m_InputListeners.end())
	{
		(*listener)->OnKeyDown(key);

		++listener;
	}
}

bool RInputManager::IsKeyDown(unsigned int key)
{
	return m_IsKeyDown[key];
}

void RInputManager::AddListener(RInputListener* listener)
{
	m_InputListeners.push_back(listener);
}