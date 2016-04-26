#pragma once

#include <list>

class RMaterial;
class RenderPrimitive;

class Mesh
{
public:
	Mesh();
	~Mesh();
	
	void SetMaterial(RMaterial* pMaterial);
	RMaterial* GetMaterialPtr() const;

	void AddRenderPrimitive(RenderPrimitive* pPrimitive);
	std::list<RenderPrimitive*> GetRenderPrimitives();

private:
	RMaterial* m_pMaterial;
	std::list<RenderPrimitive*> m_listPrimitives;
};

