#pragma once

#include "Mesh.h"
#include "Animation.h"

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

class Sprite {
public:
	Sprite();
	~Sprite();

	static Sprite* Load(const char* filePath);

	Mesh* GetMeshes();
	int GetMeshNum();
	float* GetBoneData();
	int GetBoneNum();
	int GetFrameNum();

	void Initialize();
	void Update(float deltaTime);
	void Cleanup();
	void Draw();

	void SetAnimation(int animationID);
	void PlayAnimation(int animationID, bool loop = false);

private:
	void InitAnimation();

private:
	Mesh* m_Meshes;
	Animation* m_Animation;
	int m_AnimationID;

	// 用于从model文件中加载数据
	MaxModelHead m_ModelHead;
	float* m_BoneData;
};