/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��
	AUTH :	���һ���																			   DATE : 2016-05-24
	DESC :	
	1.	ģ���ɶ�����񡢹����Լ��������
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
	std::map<std::string, Bone*>		m_mapBones;				// <�������� ����>
	std::map<std::string, Animation*>	m_mapAnimations;		// <�������� ����>
};

