#pragma once

#include <InputListener.h>
#include <Sprite.h>
#include <Light.h>
#include <Camera.h>
#include <InputManager.h>
#include <SceneNode.h>
#include <Mesh.h>

#define ANIMATION_STAND					0
#define ANIMATION_STAND_WEAPON			1
#define ANIMATION_SPECIAL_STAND			2
#define ANIMATION_SPECTAL_STAND_WEAPON	3
#define ANIMATION_RUN					4
#define ANIMATION_RUN_WEAPON			5
#define ANIMATION_HIT					6
#define ANIMATION_HIT_WEAPON			7
#define ANIMATION_ATTACK				8
#define ANIMATION_ATTACK_WEAPON			9
#define ANIMATION_DEAD					10
#define ANIMATION_SKILL_FNYJ01			11
#define ANIMATION_SKILL_FNYJ02			12
#define ANIMATION_SKILL_FNYJ03			13
#define ANIMATION_SKILL_FKYJ			14
#define ANIMATION_SKILL_FYJT			15
#define ANIMATION_SKILL_FRLW			16
#define ANIMATION_STAND_ATTACK			17
#define ANIMATION_SKILL_JS				18
#define ANIMATION_SKILL_HYCJ			19

#define ANIMATION_NUM					20

class Controller : public InputListener {
public:
	Controller(Sprite* pSprite, Light* pPointLight, Light* pDirectionalLight, Light* pSpotLight, Camera* pCamera);
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

	void EnhanceMaterial(unsigned int attributeID);
	void WeakenMaterial(unsigned int attributeID);


private:
	InputManager* m_pInputManager;

	SceneNode* m_pSpriteNode;

	Sprite* m_pSprite;
	unsigned short m_ActionState;
	bool m_UseWeapon;
	float m_MoveSpeed;
	float m_FaceRadian;
	float m_TargetRadian;
	float m_RotateSpeed;

	Mesh* m_Meshes;
	unsigned int m_MeshNum;
	float m_Ambient;
	float m_Diffuse;
	float m_Specular;
	float m_Power;
	float m_DeltaColor;

	Light* m_pPointLight;
	Light* m_pDirectionalLight;
	Light* m_pSpotLight;
	unsigned short m_LightType;

	Camera* m_pCamera;
	SceneNode* m_pCameraYawPivot;
	SceneNode* m_pCameraPitchPivot;
	float m_CameraDistance;
	float m_CameraMinDistance;
	float m_CameraMaxDistance;
	float m_CameraRadian;
};

