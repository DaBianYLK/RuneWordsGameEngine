/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-05-24
	DESC :	
	1.	模型由多个网格、骨骼以及动画组成
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include "RwgeSceneNode.h"
#include "RwgeAnimation.h"
#include <list>
#include <map>
#include <d3dx9.h>

class RMesh;

struct Bone
{
	std::string			strName;
	D3DXMATRIX			transform;
	Bone*				pParent;
	std::list<Bone*>	plistChildren;
};

class RModel : public RSceneNode
{
	friend class ModelFactory;

private:
	RModel();

public:
	~RModel();

	void AddMesh(RMesh* pMesh);
	std::list<RMesh*>& GetMeshes();

private:
	std::list<RMesh*>					m_listMeshes;
	std::map<std::string, Bone*>		m_mapBones;				// <骨骼名， 骨骼>
	std::map<std::string, Animation*>	m_mapAnimations;		// <动画名， 动画>
};

