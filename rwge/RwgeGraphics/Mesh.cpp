#include "Mesh.h"


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

void Mesh::SetMaterial(Material* pMaterial)
{
	m_pMaterial = pMaterial;
}

Material* Mesh::GetMaterialPtr() const
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
