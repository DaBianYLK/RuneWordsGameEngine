#include "Controller.h"

#include <Graphics.h>
#include <SceneManager.h>
#include <AppConfig.h>

#define POINT_LIGHT			0
#define DIRECTIONAL_LIGHT	1
#define SPOT_LIGHT			2

#define ACTION_STAND	0
#define ACTION_RUN		1
#define ACTION_ATTACK	2

#define DIRECTION_UP	0
#define DIRECTION_DOWN	1
#define DIRECTION_LEFT	2
#define DIRECTION_RIGHT	3

#define SKILL_0		0
#define SKILL_1		1
#define SKILL_2		2
#define SKILL_3		3
#define SKILL_4		4
#define SKILL_5		5
#define SKILL_6		6
#define SKILL_7		7

Controller::Controller(Sprite* pSprite, Light* pPointLight, Light* pDirectionalLight, Light* pSpotLight) {
	m_pInputManager = InputManager::GetInstance();

	m_pSprite = pSprite; 
	m_ActionState = ACTION_STAND;
	m_pSprite->PlayAnimation(ANIMATION_STAND, true);

	m_pPointLight = pPointLight;
	m_pDirectionalLight = pDirectionalLight;
	m_pSpotLight = pSpotLight;

	m_LightType = POINT_LIGHT;
	m_pPointLight->Enable();

	m_pCameraYawPivot = new SceneNode();
	m_pCameraYawPivot->SetPositionY(80.0f);
	m_pSprite->AttachChild(m_pCameraYawPivot);

	m_pCameraPitchPivot = new SceneNode();
	m_pCameraYawPivot->AttachChild(m_pCameraPitchPivot);

	m_CameraDistance = 150.0f;
	m_CameraMinDistance = 100.0f;
	m_CameraMaxDistance = 500.0f;

	m_pCamera = Graphics::GetInstance()->GetSceneManager()->GetCamera();
	m_pCamera->SetPosition(0.0f, m_CameraDistance, -m_CameraDistance * 2.0f);
	D3DXVECTOR3 rightAxis(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 upAxis(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 lookAxis(0.0f, -0.5f, 1.0f);
	m_pCamera->SetLookAxes(rightAxis, upAxis, lookAxis);
	m_pCameraPitchPivot->AttachChild(m_pCamera);
}


Controller::~Controller() {

}

void Controller::OnKeyUp(unsigned int key) {
	switch (key) {
	case 'W':
		OnWalkKeyUp(DIRECTION_UP);
		break;

	case 'A':
		OnWalkKeyUp(DIRECTION_LEFT);
		break;

	case 'S':
		OnWalkKeyUp(DIRECTION_DOWN);
		break;

	case 'D':
		OnWalkKeyUp(DIRECTION_RIGHT);
		break;

	default:
		break;
	}
}

void Controller::OnKeyDown(unsigned int key) {
	switch (key) {
	case 'P':
		OnLightKeyDown();
		break;

	case 'W':
		OnWalkKeyDown(DIRECTION_UP);
		break;

	case 'A':
		OnWalkKeyDown(DIRECTION_LEFT);
		break;

	case 'S':
		OnWalkKeyDown(DIRECTION_DOWN);
		break;

	case 'D':
		OnWalkKeyDown(DIRECTION_RIGHT);
		break;

	case 'Y':
		OnSkillKeyDown(SKILL_0);
		break;

	case 'U':
		OnSkillKeyDown(SKILL_1);
		break;

	case 'I':
		OnSkillKeyDown(SKILL_2);
		break;

	case 'O':
		OnSkillKeyDown(SKILL_3);
		break;

	case 'H':
		OnSkillKeyDown(SKILL_4);
		break;

	case 'J':
		OnSkillKeyDown(SKILL_5);
		break;

	case 'K':
		OnSkillKeyDown(SKILL_6);
		break;

	case 'L':
		OnSkillKeyDown(SKILL_7);
		break;

	case 'F':
		OnWeaponKeyDown();
		break;

	case VK_SPACE:
		OnAttackKeyDown();
		break;

	default:
		break;
	}
}

void Controller::Update(float deltaTime) {
	if (!m_pSprite->IsCurrentAnimationPlaying()) {
		if (m_UseWeapon) {
			m_pSprite->PlayAnimation(ANIMATION_STAND_WEAPON, true);
		}
		else {
			m_pSprite->PlayAnimation(ANIMATION_STAND, true);
		}

		m_ActionState = ACTION_STAND;
	}

	if (m_pInputManager->IsKeyDown('Q')) {
		m_pCameraYawPivot->Yaw(AppConfig::cameraRotateSpeed);
	}
	if (m_pInputManager->IsKeyDown('E')) {
		m_pCameraYawPivot->Yaw(-AppConfig::cameraRotateSpeed);
	}
	if (m_pInputManager->IsKeyDown('Z')) {
		m_pCameraPitchPivot->Pitch(-AppConfig::cameraRotateSpeed);
	}
	if (m_pInputManager->IsKeyDown('X')) {
		m_pCameraPitchPivot->Pitch(AppConfig::cameraRotateSpeed);
	}
	if (m_pInputManager->IsKeyDown('C')) {
		if (m_CameraDistance < m_CameraMaxDistance) {
			m_CameraDistance += AppConfig::cameraMoveSpeed;

			m_pCamera->SetPosition(0.0f, m_CameraDistance, -m_CameraDistance * 2.0f);
		}
	}
	if (m_pInputManager->IsKeyDown('V')) {
		if (m_CameraDistance > m_CameraMinDistance) {
			m_CameraDistance -= AppConfig::cameraMoveSpeed;

			m_pCamera->SetPosition(0.0f, m_CameraDistance, -m_CameraDistance * 2.0f);
		}
	}
}

void Controller::OnWalkKeyDown(unsigned int direction) {
	// 只有角色站立时才可以控制角色移动
	if (m_ActionState == ACTION_STAND) {
		if (m_UseWeapon) {
			m_pSprite->PlayAnimation(ANIMATION_RUN_WEAPON, true);
		}
		else {
			m_pSprite->PlayAnimation(ANIMATION_RUN, true);
		}

		m_ActionState = ACTION_RUN;
	}
}

void Controller::OnWalkKeyUp(unsigned int direction) {
	if (m_ActionState == ACTION_RUN) {
		if (m_UseWeapon) {
			m_pSprite->PlayAnimation(ANIMATION_STAND_WEAPON, true);
		}
		else {
			m_pSprite->PlayAnimation(ANIMATION_STAND, true);
		}
		m_ActionState = ACTION_STAND;
	}
}

void Controller::OnWeaponKeyDown() {
	m_UseWeapon = !m_UseWeapon;

	if (m_UseWeapon) {
		if (m_ActionState == ACTION_STAND) {
			m_pSprite->PlayAnimation(ANIMATION_STAND_WEAPON, true);
		}
		else if (m_ActionState == ACTION_RUN) {
			m_pSprite->PlayAnimation(ANIMATION_RUN_WEAPON, true);
		}
	}
	else {
		if (m_ActionState == ACTION_STAND) {
			m_pSprite->PlayAnimation(ANIMATION_STAND, true);
		}
		else if (m_ActionState == ACTION_RUN) {
			m_pSprite->PlayAnimation(ANIMATION_RUN, true);
		}
	}
}

void Controller::OnLightKeyDown() {
	switch (m_LightType) {
	case POINT_LIGHT:
		m_pPointLight->Disable();
		m_pDirectionalLight->Enable();
		m_pSpotLight->Disable();
		m_LightType = DIRECTIONAL_LIGHT;
		break;

	case DIRECTIONAL_LIGHT:
		m_pPointLight->Disable();
		m_pDirectionalLight->Disable();
		m_pSpotLight->Enable();
		m_LightType = SPOT_LIGHT;
		break;

	case SPOT_LIGHT:
		m_pPointLight->Enable();
		m_pDirectionalLight->Disable();
		m_pSpotLight->Disable();
		m_LightType = POINT_LIGHT;
		break;

	default:
		m_pPointLight->Enable();
		m_pDirectionalLight->Disable();
		m_pSpotLight->Disable();
		m_LightType = POINT_LIGHT;
		break;
	}
}

void Controller::OnAttackKeyDown() {
	if (m_ActionState == ACTION_STAND || m_ActionState == ACTION_RUN) {
		if (m_UseWeapon) {
			m_pSprite->PlayAnimation(ANIMATION_ATTACK_WEAPON, false);
		}
		else {
			m_pSprite->PlayAnimation(ANIMATION_ATTACK, false);
		}

		m_ActionState = ACTION_ATTACK;
	}
}

void Controller::OnSkillKeyDown(unsigned int skillID) {
	if (m_ActionState == ACTION_STAND || m_ActionState == ACTION_RUN) {
		switch (skillID) {
		case SKILL_0:
			m_pSprite->PlayAnimation(ANIMATION_SKILL_FNYJ01, false);
			break;

		case SKILL_1:
			m_pSprite->PlayAnimation(ANIMATION_SKILL_FNYJ02, false);
			break;

		case SKILL_2:
			m_pSprite->PlayAnimation(ANIMATION_SKILL_FNYJ03, false);
			break;

		case SKILL_3:
			m_pSprite->PlayAnimation(ANIMATION_SKILL_FKYJ, false);
			break;

		case SKILL_4:
			m_pSprite->PlayAnimation(ANIMATION_SKILL_FYJT, false);
			break;

		case SKILL_5:
			m_pSprite->PlayAnimation(ANIMATION_SKILL_FRLW, false);
			break;

		case SKILL_6:
			m_pSprite->PlayAnimation(ANIMATION_SKILL_JS, false);
			break;

		case SKILL_7:
			m_pSprite->PlayAnimation(ANIMATION_SKILL_HYCJ, false);
			break;

		default:
			m_pSprite->PlayAnimation(ANIMATION_SKILL_FNYJ01, false);
			break;
		}

		m_ActionState = ACTION_ATTACK;
	}
}