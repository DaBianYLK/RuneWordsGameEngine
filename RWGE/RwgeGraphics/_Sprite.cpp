//#include "Sprite.h"
//
//#include "Graphics.h"
//#include "Mesh.h"
//#include "Animation.h"
//
//#include <fstream>
//
//using namespace std;
//
//Sprite::Sprite()
//{
//	m_NodeType = SceneNodeType::SpriteNode;
//	
//	m_Meshes = nullptr;
//	m_Animations = nullptr;
//	m_AnimationID = 0;
//	m_AnimationNum = 0;
//
//	ZeroMemory(&m_ModelHead, sizeof(MaxModelHead));
//	m_BoneData = nullptr;
//}
//
//Sprite::~Sprite()
//{
//
//}
//
//Sprite* Sprite::Load(const char* filePath)
//{
//	Sprite* pSprite = new Sprite();
//
//	ifstream modelFile;
//	modelFile.open(filePath, ios::in | ios::binary);
//
//	// 获取模型文件首字段的数据
//	modelFile.read((char*)&pSprite->m_ModelHead, sizeof(MaxModelHead));
//
//	// 获取骨骼数据
//	int boneDataLength = 4 * 4 * pSprite->m_ModelHead.frameNum * pSprite->m_ModelHead.boneNum;
//	pSprite->m_BoneData = new float[boneDataLength];
//	modelFile.read((char*)pSprite->m_BoneData, sizeof(float) * boneDataLength);
//
//	// 获取网格数据
//	pSprite->m_Meshes = new Mesh[pSprite->m_ModelHead.meshNum];
//	for (unsigned int i = 0; i < pSprite->m_ModelHead.meshNum; ++i)
//{
//		// 获取网格首字段的数据
//		MaxMeshHead head;
//		modelFile.read((char*)&head, sizeof(MaxMeshHead));
//
//		MaxVertex* vertexData = new MaxVertex[head.vertexNum];
//		unsigned short* indexData = new unsigned short[head.triangleNum * 3];
//
//		modelFile.read((char*)vertexData, sizeof(MaxVertex)* head.vertexNum);
//		modelFile.read((char*)indexData, sizeof(unsigned short)* 3 * head.triangleNum);
//
//		pSprite->m_Meshes[i] = Mesh(head, vertexData, indexData, pSprite);
//	}
//
//	// 获取动画数据
//	unsigned int animationNum = 0;
//	modelFile.read((char*)&animationNum, sizeof(unsigned int));
//
//	MaxAnimation* animationData = new MaxAnimation[animationNum];
//
//	modelFile.read((char*)animationData, sizeof(MaxAnimation) * animationNum);
//
//	modelFile.close();
//
//	pSprite->m_AnimationNum = animationNum;
//	pSprite->m_Animations = new Animation[animationNum]; 
//	pSprite->m_AnimationID = 0;
//
//	for (unsigned int i = 0; i < animationNum; ++i)
//{
//		pSprite->m_Animations[i].Set(animationData[i].startFrame, animationData[i].frameNum);
//	}
//
//	delete animationData;
//
//	return pSprite;
//}
//
//Sprite* Sprite::CreatePanel(float x, float y, float z, float length, float width)
//{
//	Sprite* pSprite = new Sprite();
//
//	D3DXVECTOR3 position(x, y, z);
//	pSprite->m_Meshes = Mesh::CreatePanel(position, length, width);
//
//	pSprite->m_ModelHead.meshNum = 1;
//
//	return pSprite;
//}
//
//Mesh* Sprite::GetMeshes()
//{
//	return m_Meshes;
//}
//
//int Sprite::GetMeshNum()
//{
//	return m_ModelHead.meshNum;
//}
//
//float* Sprite::GetBoneData()
//{
//	return m_BoneData;
//}
//
//unsigned int Sprite::GetBoneNum()
//{
//	return m_ModelHead.boneNum;
//}
//
//int Sprite::GetFrameNum()
//{
//	return m_ModelHead.frameNum;
//}
//
//void Sprite::Update(float deltaTime)
//{
//	if (m_Animations && m_AnimationID < m_AnimationNum)
//{
//		m_Animations[m_AnimationID].Update(deltaTime);
//
//#ifndef SHADER_ANIMATION
//		if (m_ModelHead.boneNum > 0)
//{
//			for (int i = 0; i < m_ModelHead.meshNum; ++i)
//{
//				m_Meshes[i].Update(m_Animations[m_AnimationID].GetFrameIndex());
//			}
//		}
//#endif
//	}
//}
//
//void Sprite::Cleanup()
//{
//
//}
//
//void Sprite::Draw()
//{
//#ifdef SHADER_ANIMATION
//	for (unsigned int i = 0; i < m_ModelHead.meshNum; ++i)
//{
//		if (m_Animations && m_AnimationID < m_AnimationNum)
//{
//			m_Meshes[i].Draw(m_Animations[m_AnimationID].GetFrameIndex());
//		}
//		else
//{
//			m_Meshes[i].Draw(-1);
//		}
//	}
//#else
//	for (unsigned int i = 0; i < m_ModelHead.meshNum; ++i)
//{
//		m_Meshes[i].Draw();
//	}
//#endif
//}
//
//int Sprite::GetAnimationNum()
//{
//	return m_AnimationNum;
//}
//
//void Sprite::SetAnimation(int animationID)
//{
//	if (m_Animations && animationID < m_AnimationNum)
//{
//		m_AnimationID = animationID;
//	}
//}
//
//void Sprite::PlayAnimation(int animationID, bool loop)
//{
//	if (m_Animations && animationID < m_AnimationNum)
//{
//		m_AnimationID = animationID;
//
//		m_Animations[m_AnimationID].Play(loop);
//	}
//}
//
//bool Sprite::IsCurrentAnimationPlaying()
//{
//	if (m_Animations)
//{
//		return m_Animations[m_AnimationID].IsPlaying();
//	}
//	
//	return false;
//}
//
////void Sprite::InitAnimation()
//{
////	m_Animations = new Animation[ANIMATION_NUM];
////
////	m_Animations[ANIMATION_STAND].Set(6, 40);
////	m_Animations[ANIMATION_STAND_WEAPON].Set(745, 40);
////	m_Animations[ANIMATION_SPECIAL_STAND].Set(104, 95);
////	m_Animations[ANIMATION_SPECTAL_STAND_WEAPON].Set(207, 71);
////	m_Animations[ANIMATION_RUN].Set(286, 22);
////	m_Animations[ANIMATION_RUN_WEAPON].Set(316, 22);
////	m_Animations[ANIMATION_HIT].Set(346, 17);
////	m_Animations[ANIMATION_HIT_WEAPON].Set(370, 17);
////	m_Animations[ANIMATION_ATTACK].Set(394, 16);
////	m_Animations[ANIMATION_ATTACK_WEAPON].Set(421, 31);
////	m_Animations[ANIMATION_DEAD].Set(465, 37);
////	m_Animations[ANIMATION_SKILL_FNYJ01].Set(508, 15);
////	m_Animations[ANIMATION_SKILL_FNYJ02].Set(535, 20);
////	m_Animations[ANIMATION_SKILL_FNYJ03].Set(566, 27);
////	m_Animations[ANIMATION_SKILL_FKYJ].Set(605, 24);
////	m_Animations[ANIMATION_SKILL_FYJT].Set(637, 22);
////	m_Animations[ANIMATION_SKILL_FRLW].Set(666, 30);
////	m_Animations[ANIMATION_STAND_ATTACK].Set(700, 38);
////	m_Animations[ANIMATION_SKILL_JS].Set(815, 25);
////	m_Animations[ANIMATION_SKILL_HYCJ].Set(845, 35);
////
////	m_AnimationID = ANIMATION_STAND;
////}