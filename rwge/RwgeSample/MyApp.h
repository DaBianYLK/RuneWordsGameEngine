#pragma once

#include "AppDelegate.h"

class Sprite;

class MyApp : public AppDelegate {
public:
	MyApp();
	~MyApp();

	void Initialize();
	void Update(float deltaTime);
	void Cleanup();

private:
	Sprite* m_pSprite;
};

