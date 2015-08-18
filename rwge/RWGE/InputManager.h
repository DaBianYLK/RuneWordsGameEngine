#pragma once

#include <Windows.h>
#include <list>

#include "RwgeClasses.h"
#include "Singleton.h"

class InputManager : public Singleton<InputManager> {
public:
	InputManager();
	~InputManager();

	void Initialize();
	void Update(float deltaTime);
	void Cleanup();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

	void KeyUp(unsigned int key);
	void KeyDown(unsigned int key);
	bool IsKeyDown(unsigned int key);

	void addListener(InputListener* listener);

private:
	bool m_IsKeyDown[256];

	std::list<InputListener*> m_InputListeners;
};

