/*--------------------------------------------------------------------------------------------------------------------*\
   ��CREATE��
	AUTH :	���һ���																			   DATE : 2016-05-23
	DESC :	
	1.	һ��ģ���в�����ͬ�Ŀ���Ⱦ��Ԫ�����һ��Mesh
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

