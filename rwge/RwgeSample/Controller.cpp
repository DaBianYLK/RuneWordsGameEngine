#include "Controller.h"

#include <Graphics.h>
#include <SceneManager.h>
#include <AppConfig.h>

#include <LogUtil.h>

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

#define MATERIAL_AMBIENT	0
#define MATERIAL_DIFFUSE	1
#define MATERIAL_SPECULAR	2
#define MATERIAL_POWER		3

Controller::Controller(Sprite* pSprite, Light* pPointLight, Light* pDirectionalLight, Light* pSpotLight, Camera* pCamera) {
	m_pInputManager = InputManager::GetInstance();

	m_pSpriteNode = pSprite->GetFather();

	m_pSprite = pSprite;
	m_ActionState = ACTION_STAND;
	m_pSprite->PlayAnimation(ANIMATION_STAND, true);
	m_MoveSpeed = 360.0f;
	m_FaceRadian = 0.0f;
	m_TargetRadian = m_FaceRadian;
	m_RotateSpeed = 6.0f;
	m_pSprite->SetRotation(0.0f, 1.0f, 0.0f, m_FaceRadian + +D3DX_PI);

	m_Meshes = m_pSprite->GetMeshes();
	m_MeshNum = m_pSprite->GetMeshNum();
	m_Ambient = m_Meshes[0].GetMaterialAmbient().r;
	m_Diffuse = m_Meshes[0].GetMaterialDiffuse().r;
	m_Specular = m_Meshes[0].GetMaterialSpecular().r;
	m_Power = m_Meshes[0].GetMaterialPower();
	m_DeltaColor = 0.05f;

	m_pPointLight = pPointLight;
	m_pDirectionalLight = pDirectionalLight;
	m_pSpotLight = pSpotLight;

	m_LightType = DIRECTIONAL_LIGHT;
	m_pDirectionalLight->Enable();

	m_pCameraYawPivot = new SceneNode();
	m_pCameraYawPivot->SetPositionY(80.0f);
	m_pSpriteNode->AttachChild(m_pCameraYawPivot);

	m_pCameraPitchPivot = new SceneNode();
	m_pCameraYawPivot->AttachChild(m_pCameraPitchPivot);

	m_CameraDistance = 150.0f;
	m_CameraMinDistance = 50.0f;
	m_CameraMaxDistance = 500.0f;

	m_pCamera = pCamera;
	m_pCamera->SetPosition(0.0f, m_CameraDistance, -m_CameraDistance * 2.0f);
	D3DXVECTOR3 rightAxis(1.0f, 0.0f, 0.0f);
	D3DXVECTOR3 upAxis(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 lookAxis(0.0f, -0.5f, 1.0f);
	m_pCamera->SetLookAxes(rightAxis, upAxis, lookAxis);
	m_pCameraPitchPivot->AttachChild(m_pCamera);
	m_CameraRadian = 0.0f;
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

	case VK_NUMPAD7:
		EnhanceMaterial(MATERIAL_AMBIENT);
		break;

	case VK_NUMPAD8:
		EnhanceMaterial(MATERIAL_DIFFUSE);
		break;

	case VK_NUMPAD9:
		EnhanceMaterial(MATERIAL_SPECULAR);
		break;

	case VK_NUMPAD4:
		WeakenMaterial(MATERIAL_AMBIENT);
		break;

	case VK_NUMPAD5:
		WeakenMaterial(MATERIAL_DIFFUSE);
		break;

	case VK_NUMPAD6:
		WeakenMaterial(MATERIAL_SPECULAR);
		break;

	case VK_NUMPAD2:
		EnhanceMaterial(MATERIAL_POWER);
		break;

	case VK_NUMPAD1:
		WeakenMaterial(MATERIAL_POWER);
		break;

	default:
		break;
	}
}

void Controller::Update(float deltaTime) {
	// 人物动作控制
	if (!m_pSprite->IsCurrentAnimationPlaying()) {
		if (m_UseWeapon) {
			m_pSprite->PlayAnimation(ANIMATION_STAND_WEAPON, true);
		}
		else {
			m_pSprite->PlayAnimation(ANIMATION_STAND, true);
		}

		m_ActionState = ACTION_STAND;
	}

	// 镜头控制
	if (m_pInputManager->IsKeyDown('Q')) {
		//m_pCameraYawPivot->Yaw(-AppConfig::cameraRotateSpeed);
		m_CameraRadian -= AppConfig::cameraRotateSpeed * deltaTime;
		m_pCameraYawPivot->SetRotation(0.0f, 1.0f, 0.0f, m_CameraRadian);
	}
	else if (m_pInputManager->IsKeyDown('E')) {
		//m_pCameraYawPivot->Yaw(AppConfig::cameraRotateSpeed);
		m_CameraRadian += AppConfig::cameraRotateSpeed * deltaTime;
		m_pCameraYawPivot->SetRotation(0.0f, 1.0f, 0.0f, m_CameraRadian);
	}

	if (m_pInputManager->IsKeyDown('Z')) {
		m_pCameraPitchPivot->Pitch(-AppConfig::cameraRotateSpeed * deltaTime);
	}
	else if (m_pInputManager->IsKeyDown('X')) {
		m_pCameraPitchPivot->Pitch(AppConfig::cameraRotateSpeed * deltaTime);
	}

	if (m_pInputManager->IsKeyDown('C')) {
		if (m_CameraDistance < m_CameraMaxDistance) {
			m_CameraDistance += AppConfig::cameraMoveSpeed * deltaTime;

			m_pCamera->SetPosition(0.0f, m_CameraDistance, -m_CameraDistance * 2.0f);
		}
	}
	else if (m_pInputManager->IsKeyDown('V')) {
		if (m_CameraDistance > m_CameraMinDistance) {
			m_CameraDistance -= AppConfig::cameraMoveSpeed * deltaTime;

			m_pCamera->SetPosition(0.0f, m_CameraDistance, -m_CameraDistance * 2.0f);
		}
	}

	if (m_ActionState == ACTION_RUN) {
		// 人物移动控制
		if (m_pInputManager->IsKeyDown('Q') || m_pInputManager->IsKeyDown('E')) {
			if (m_pInputManager->IsKeyDown('W')) {
				m_TargetRadian = m_CameraRadian;
			}
			else if (m_pInputManager->IsKeyDown('A')) {
				m_TargetRadian = m_CameraRadian - D3DX_PI * 0.5f;
			}
			else if (m_pInputManager->IsKeyDown('S')) {
				m_TargetRadian = m_CameraRadian + D3DX_PI;
			}
			else if (m_pInputManager->IsKeyDown('D')) {
				m_TargetRadian = m_CameraRadian + D3DX_PI * 0.5f;
			}
		}

		// 行走过程中的人物转身控制
		float rotateRadian = m_RotateSpeed * deltaTime;
		if (m_FaceRadian != m_TargetRadian) {
			if (m_FaceRadian > m_TargetRadian + rotateRadian) {
				m_FaceRadian -= rotateRadian;
			}
			else if (m_FaceRadian < m_TargetRadian - rotateRadian) {
				m_FaceRadian += rotateRadian;
			}
			else {
				m_FaceRadian = m_TargetRadian;
			}

			m_pSprite->SetRotation(0.0f, 1.0f, 0.0f, m_FaceRadian + D3DX_PI);
		}

		float moveLength = m_MoveSpeed * deltaTime;
		float x = moveLength * sinf(m_FaceRadian);
		float z = moveLength * cosf(m_FaceRadian);
		m_pSpriteNode->Translate(x, 0.0f, z);
	}

	if (m_ActionState == ACTION_STAND) {
		// 站立时的人物转身控制
		if (m_pInputManager->IsKeyDown('R')) {
			m_FaceRadian += m_RotateSpeed * deltaTime;
			if (m_FaceRadian > D3DX_PI * 2.0f) {
				m_FaceRadian -= D3DX_PI * 2.0f;
			}
			m_pSprite->SetRotation(0.0f, 1.0f, 0.0f, m_FaceRadian + D3DX_PI);
		}
		else if (m_pInputManager->IsKeyDown('T')) {
			m_FaceRadian -= m_RotateSpeed * deltaTime;
			if (m_FaceRadian < 0.0f) {
				m_FaceRadian += D3DX_PI * 2.0f;
			}
			m_pSprite->SetRotation(0.0f, 1.0f, 0.0f, m_FaceRadian + D3DX_PI);
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

	switch (direction) {
	case DIRECTION_UP:
		m_TargetRadian = m_CameraRadian;
		break;

	case DIRECTION_DOWN:
		m_TargetRadian = m_CameraRadian + D3DX_PI;
		break;

	case DIRECTION_LEFT:
		m_TargetRadian = m_CameraRadian - D3DX_PI * 0.5f;
		break;

	case DIRECTION_RIGHT:
		m_TargetRadian = m_CameraRadian + D3DX_PI * 0.5f;
		break;

	default:
		break;
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

void Controller::EnhanceMaterial(unsigned int attributeID) {
	D3DXCOLOR white(1.0f, 1.0f, 1.0f, 1.0f);

	switch (attributeID) {
	case MATERIAL_AMBIENT:
		for (unsigned int i = 0; i < m_MeshNum; ++i) {
			m_Ambient += m_DeltaColor;
			m_Meshes[i].SetMaterialAmbient(white * m_Ambient);
		}
		break;

	case MATERIAL_DIFFUSE:
		for (unsigned int i = 0; i < m_MeshNum; ++i) {
			m_Diffuse += m_DeltaColor;
			m_Meshes[i].SetMaterialDiffuse(white * m_Diffuse);
		}
		break;

	case MATERIAL_SPECULAR:
		for (unsigned int i = 0; i < m_MeshNum; ++i) {
			m_Specular += m_DeltaColor;
			m_Meshes[i].SetMaterialSpecular(white * m_Specular);
		}
		break;

	case MATERIAL_POWER:
		for (unsigned int i = 0; i < m_MeshNum; ++i) {
			m_Power += m_DeltaColor;
			m_Meshes[i].SetMaterialPower(m_Power);
		}
		break;

	default:
		break;
	}
}

void Controller::WeakenMaterial(unsigned int attributeID) {
	D3DXCOLOR white(1.0f, 1.0f, 1.0f, 1.0f);

	switch (attributeID) {
	case MATERIAL_AMBIENT:
		for (unsigned int i = 0; i < m_MeshNum; ++i) {
			m_Ambient -= m_DeltaColor;
			m_Meshes[i].SetMaterialAmbient(white * m_Ambient);
		}
		break;

	case MATERIAL_DIFFUSE:
		for (unsigned int i = 0; i < m_MeshNum; ++i) {
			m_Diffuse -= m_DeltaColor;
			m_Meshes[i].SetMaterialDiffuse(white * m_Diffuse);
		}
		break;

	case MATERIAL_SPECULAR:
		for (unsigned int i = 0; i < m_MeshNum; ++i) {
			m_Specular -= m_DeltaColor;
			m_Meshes[i].SetMaterialSpecular(white * m_Specular);
		}
		break;

	case MATERIAL_POWER:
		for (unsigned int i = 0; i < m_MeshNum; ++i) {
			m_Power -= m_DeltaColor;
			m_Meshes[i].SetMaterialPower(m_Power);
		}
		break;

	default:
		break;
	}
}