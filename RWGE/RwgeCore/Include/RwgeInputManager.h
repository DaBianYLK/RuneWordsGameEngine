#pragma once

#include <list>
#include <RwgeSingleton.h>

class RInputListener;

class RInputManager : public Singleton<RInputManager>
{
public:
	RInputManager();
	~RInputManager();

	void Initialize();
	void Update(float deltaTime);
	void Cleanup();

	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);

	void KeyUp(unsigned int key);
	void KeyDown(unsigned int key);
	bool IsKeyDown(unsigned int key);

	void AddListener(RInputListener* listener);

private:
	bool m_IsKeyDown[256];

	std::list<RInputListener*> m_InputListeners;
};

