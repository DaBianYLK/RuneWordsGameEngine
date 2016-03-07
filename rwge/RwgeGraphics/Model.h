#pragma once

#include "SceneNode.h"
#include "Animation.h"
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
public:
	Model();
	~Model();

	const std::list<Mesh*>& GetMeshes();

private:
	std::list<Mesh*>					m_listMeshes;
	std::map<std::string, Bone*>		m_mapBones;				// <骨骼名， 骨骼>
	std::map<std::string, Animation*>	m_mapAnimations;		// <动画名， 动画>
};

