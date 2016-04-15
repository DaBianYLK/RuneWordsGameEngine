#pragma once

#include "RwgeSceneNode.h"
#include "RwgeAnimation.h"
#include <list>
#include <map>
#include <d3dx9.h>

class Mesh;

struct Bone
{
	std::string			strName;
	D3DXMATRIX			transform;
	Bone*				pParent;
	std::list<Bone*>	plistChildren;
};

class Model : public SceneNode
{
	friend class ModelFactory;

private:
	Model();

public:
	~Model();

	void AddMesh(Mesh* pMesh);
	std::list<Mesh*>& GetMeshes();

private:
	std::list<Mesh*>					m_listMeshes;
	std::map<std::string, Bone*>		m_mapBones;				// <骨骼名， 骨骼>
	std::map<std::string, Animation*>	m_mapAnimations;		// <动画名， 动画>
};

