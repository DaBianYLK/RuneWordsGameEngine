#include "Model.h"


Model::Model() : SceneNode()
{
	m_NodeType = NT_Model;
}



Model::~Model()
{

}

const std::list<Mesh*>& Model::GetMeshes()
{
	return m_listMeshes;
}