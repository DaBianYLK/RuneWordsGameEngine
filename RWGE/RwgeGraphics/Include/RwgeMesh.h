/*--------------------------------------------------------------------------------------------------------------------*\
   【CREATE】
	AUTH :	大便一箩筐																			   DATE : 2016-05-23
	DESC :	
	1.	一个模型中材质相同的可渲染单元组成了一个Mesh
\*--------------------------------------------------------------------------------------------------------------------*/


#pragma once

#include <list>
#include <RwgeObject.h>

class RMaterial;
class RRenderUnit;

class RMesh : public RObject
{
public:
	RMesh();
	~RMesh();
	
	void SetMaterial(RMaterial* pMaterial);
	RMaterial* GetMaterial() const;

	void AddRenderUnit(RRenderUnit* pPrimitive);
	const std::list<RRenderUnit*>& GetRenderUnits();

private:
	RMaterial* m_pMaterial;
	std::list<RRenderUnit*> m_listPrimitives;
};

