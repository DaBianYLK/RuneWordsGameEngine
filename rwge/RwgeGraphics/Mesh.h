#pragma once

#include <list>

class Material;
class RenderPrimitive;

class Mesh
{
public:
	Mesh();
	~Mesh();
	
	void SetMaterial(Material* pMaterial);
	Material* GetMaterialPtr() const;

	void AddRenderPrimitive(RenderPrimitive* pPrimitive);
	std::list<RenderPrimitive*> GetRenderPrimitives();

private:
	Material* m_pMaterial;
	std::list<RenderPrimitive*> m_listPrimitives;
};

