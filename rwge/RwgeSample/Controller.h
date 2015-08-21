#pragma once

#include <InputListener.h>
#include <Sprite.h>
#include <Light.h>
#include <Camera.h>
#include <InputManager.h>

class Controller : public InputListener {
public:
	Controller(Sprite* pSprite, Light* pPointLight, Light* pDirectionalLight, Light* pSpotLight);
	~Controller();

	void OnKeyUp(unsigned int key);
	void OnKeyDown(unsigned int key);

	void Update(float deltaTime);

private:
	void OnWalkKeyDown(unsigned int direction);
	void OnWalkKeyUp(unsigned int direction);

	void OnWeaponKeyDown();
	void OnLightKeyDown();
	void OnAttackKeyDown();
	void OnSkillKeyDown(unsigned int skillID);


private:
	InputManager* m_pInputManager;

	Sprite* m_pSprite;
	unsigned short m_ActionState;
	bool m_UseWeapon;

	Light* m_pPointLight;
	Light* m_pDirectionalLight;
	Light* m_pSpotLight;
	unsigned short m_LightType;

	Camera* m_pCamera;
};

