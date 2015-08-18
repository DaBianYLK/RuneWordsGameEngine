#include "InputManager.h"

#include "InputListener.h"

InputManager::InputManager() {
	memset(m_IsKeyDown, 0, sizeof(bool)* 256);
}

InputManager::~InputManager() {

}

void InputManager::Initialize() {

}

void InputManager::Update(float deltaTime) {

}

void InputManager::Cleanup() {

}

LRESULT CALLBACK InputManager::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam) {
	switch (umsg) {
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
		KeyDown((unsigned int)wparam);
		return 0;

		// Check if a key has been released on the keyboard.
	case WM_KEYUP:
		// If a key is released then send it to the input object so it can unset the state for that key.
		KeyUp((unsigned int)wparam);
		return 0;

		// All other messages pass to the message handler in the system class.
	default:
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
}

void InputManager::KeyUp(unsigned int key) {
	m_IsKeyDown[key] = false;

	std::list<InputListener*>::iterator listener = m_InputListeners.begin();
	while (listener != m_InputListeners.end()) {
		(*listener)->OnKeyUp(key);

		listener++;
	}
}

void InputManager::KeyDown(unsigned int key) {
	m_IsKeyDown[key] = true;

	std::list<InputListener*>::iterator listener = m_InputListeners.begin();
	while (listener != m_InputListeners.end()) {
		(*listener)->OnKeyDown(key);

		listener++;
	}
}

bool InputManager::IsKeyDown(unsigned int key) {
	return m_IsKeyDown[key];
}

void InputManager::addListener(InputListener* listener) {
	m_InputListeners.push_back(listener);
}