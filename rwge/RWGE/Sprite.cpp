#include "Sprite.h"

#include "Graphics.h"

#include <fstream>

using namespace std;

Sprite::Sprite() {
	//m_ModelHead.meshNum = 1;
	//m_Meshes = new Mesh[m_ModelHead.meshNum];
}

Sprite::~Sprite() {

}

Sprite* Sprite::Load(const char* filePath) {
	Sprite* sprite = new Sprite();

	ifstream modelFile;
	modelFile.open(filePath, ios::in | ios::binary);

	// 获取模型文件首字段的数据
	modelFile.read((char*)&sprite->m_ModelHead, sizeof(MaxModelHead));

	// 获取骨骼数据
	int boneDataLength = 4 * 4 * sprite->m_ModelHead.frameNum * sprite->m_ModelHead.boneNum;
	sprite->m_BoneData = new float[boneDataLength];
	modelFile.read((char*)sprite->m_BoneData, sizeof(float)* boneDataLength);

	// 获取网格数据
	sprite->m_Meshes = new Mesh[sprite->m_ModelHead.meshNum];
	for (int i = 0; i < sprite->m_ModelHead.meshNum; ++i) {
		// 获取网格首字段的数据
		MaxMeshHead head;
		modelFile.read((char*)&head, sizeof(MaxMeshHead));

		MaxVertex* vertexData = new MaxVertex[head.vertexNum];
		unsigned short* indexData = new unsigned short[head.triangleNum * 3];

		modelFile.read((char*)vertexData, sizeof(MaxVertex)* head.vertexNum);
		modelFile.read((char*)indexData, sizeof(unsigned short)* 3 * head.triangleNum);

		sprite->m_Meshes[i] = Mesh(head, vertexData, indexData, sprite);
	}

	modelFile.close();

	sprite->InitAnimation();

	return sprite;
}

Mesh* Sprite::GetMeshes() {
	return m_Meshes;
}

int Sprite::GetMeshNum() {
	return m_ModelHead.meshNum;
}

float* Sprite::GetBoneData() {
	return m_BoneData;
}

int Sprite::GetBoneNum() {
	return m_ModelHead.boneNum;
}

int Sprite::GetFrameNum() {
	return m_ModelHead.frameNum;
}

void Sprite::Initialize() {
	for (int i = 0; i < m_ModelHead.meshNum; ++i) {
		m_Meshes[i].Initialize();
	}
}

void Sprite::Update(float deltaTime) {
	m_Animation[m_AnimationID].Update(deltaTime);

	for (int i = 0; i < m_ModelHead.meshNum; ++i) {
		m_Meshes[i].Update(m_Animation[m_AnimationID].GetFrameIndex());
	}
}

void Sprite::Cleanup() {

}

void Sprite::Draw() {
	for (int i = 0; i < m_ModelHead.meshNum; ++i) {
		m_Meshes[i].Draw();
	}
}

void Sprite::SetAnimation(int animationID) {
	m_AnimationID = animationID;
}

void Sprite::PlayAnimation(int animationID, bool loop) {
	m_AnimationID = animationID;

	m_Animation[m_AnimationID].Play(loop);
}

void Sprite::InitAnimation() {
	m_Animation = new Animation[ANIMATION_NUM];

	m_Animation[ANIMATION_STAND].Set(6, 40);
	m_Animation[ANIMATION_STAND_WEAPON].Set(745, 40);
	m_Animation[ANIMATION_SPECIAL_STAND].Set(104, 95);
	m_Animation[ANIMATION_SPECTAL_STAND_WEAPON].Set(207, 71);
	m_Animation[ANIMATION_RUN].Set(286, 22);
	m_Animation[ANIMATION_RUN_WEAPON].Set(316, 22);
	m_Animation[ANIMATION_HIT].Set(346, 17);
	m_Animation[ANIMATION_HIT_WEAPON].Set(370, 17);
	m_Animation[ANIMATION_ATTACK].Set(394, 16);
	m_Animation[ANIMATION_ATTACK_WEAPON].Set(421, 31);
	m_Animation[ANIMATION_DEAD].Set(465, 502);
	m_Animation[ANIMATION_SKILL_FNYJ01].Set(508, 15);
	m_Animation[ANIMATION_SKILL_FNYJ02].Set(535, 20);
	m_Animation[ANIMATION_SKILL_FNYJ03].Set(566, 27);
	m_Animation[ANIMATION_SKILL_FKYJ].Set(605, 24);
	m_Animation[ANIMATION_SKILL_FYJT].Set(637, 22);
	m_Animation[ANIMATION_SKILL_FRLW].Set(666, 30);
	m_Animation[ANIMATION_STAND_ATTACK].Set(700, 38);
	m_Animation[ANIMATION_SKILL_JS].Set(815, 25);
	m_Animation[ANIMATION_SKILL_HYCJ].Set(845, 35);

	m_AnimationID = ANIMATION_STAND;
}