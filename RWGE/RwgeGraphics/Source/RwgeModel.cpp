#include "RwgeModel.h"


RModel::RModel() : RSceneNode()
{
	m_NodeType = ENT_Model;
}

RModel::~RModel()
{

}

void RModel::AddMesh(RMesh* pMesh)
{
	m_listMeshes.push_back(pMesh);
}

std::list<RMesh*>& RModel::GetMeshes()
{
	return m_listMeshes;
}
