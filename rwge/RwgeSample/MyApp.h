#pragma once

#include <AppDelegate.h>

#include "Controller.h"

class Sprite;
class Light;

class MyApp : public AppDelegate {
public:
	MyApp();
	~MyApp();

	void Initialize();
	void Update(float deltaTime);
	void Cleanup();

private:
	Sprite* m_pSprite;

	Light* m_pPointLight;
	Light* m_pDirectionalLight;
	Light* m_pSpotLight;

	Controller* m_pController;
};

