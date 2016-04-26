#include "RwgeMesh.h"


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::SetMaterial(RMaterial* pMaterial)
{
	m_pMaterial = pMaterial;
}

RMaterial* Mesh::GetMaterialPtr() const
{
	return m_pMaterial;
}

void Mesh::AddRenderPrimitive(RenderPrimitive* pPrimitive)
{
	m_listPrimitives.push_back(pPrimitive);
}

std::list<RenderPrimitive*> Mesh::GetRenderPrimitives()
{
	return m_listPrimitives;
}
