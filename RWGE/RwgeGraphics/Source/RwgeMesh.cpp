#include "RwgeMesh.h"


RMesh::RMesh()
{
}


RMesh::~RMesh()
{
}

void RMesh::SetMaterial(RMaterial* pMaterial)
{
	m_pMaterial = pMaterial;
}

RMaterial* RMesh::GetMaterial() const
{
	return m_pMaterial;
}

void RMesh::AddRenderUnit(RRenderUnit* pPrimitive)
{
	m_listPrimitives.push_back(pPrimitive);
}

const std::list<RRenderUnit*>& RMesh::GetRenderUnits()
{
	return m_listPrimitives;
}
