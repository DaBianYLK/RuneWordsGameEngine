#include "Mesh.h"


Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}

Material* Mesh::GetMaterialPtr() const
{
	return m_pMaterial;
}

const std::list<RenderPrimitive*> Mesh::GetRenderPrimitives() const
{
	return m_listPrimitives;
}
