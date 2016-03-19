#include "Model.h"


Model::Model() : SceneNode()
{
	m_NodeType = NT_Model;
}

Model::~Model()
{

}

void Model::AddMesh(Mesh* pMesh)
{
	m_listMeshes.push_back(pMesh);
}

std::list<Mesh*>& Model::GetMeshes()
{
	return m_listMeshes;
}
