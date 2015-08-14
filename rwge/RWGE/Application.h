#pragma once

#include <Windows.h>

#include "RwgeClasses.h"

class Application {
public:
	Application(AppDelegate* pDelegate);
	~Application();

	void Run();

private:
	void Initialize();
	void Cleanup();

private:
	Graphics* m_pGraphics;
	InputManager* m_pInputManager;


};

