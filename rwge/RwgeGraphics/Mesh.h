#pragma once

#include <list>

class Material;
class RenderPrimitive;

class Mesh
{
public:
	Mesh();
	~Mesh();

	Material* GetMaterialPtr() const;
	const std::list<RenderPrimitive*> GetRenderPrimitives() const;

private:
	Material* m_pMaterial;
	std::list<RenderPrimitive*> m_listPrimitives;
};

