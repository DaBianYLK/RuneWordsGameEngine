#include "RwgeD3d9RenderQueue.h"

#include "RwgeModel.h"
#include <RwgeCamera.h>
#include <RwgeMesh.h>
#include <RwgeGraphics.h>
#include "RwgeMaterial.h"
#include "RwgeD3d9ShaderManager.h"

using namespace std;

RD3d9RenderQueue::RD3d9RenderQueue() :
	m_pViewTransform(nullptr),
	m_pProjectionTransform(nullptr),
	m_bNeedUpdateCachedMaterialShader(false)
{

}


RD3d9RenderQueue::~RD3d9RenderQueue()
{
}

void RD3d9RenderQueue::SetCamera(const RCamera* pCamera)
{
	m_ViewOppositeDirection = -(pCamera->GetDirection());
	m_pCameraPosition = &(pCamera->GetWorldPosition());

	m_pViewTransform = pCamera->GetViewTransform();
	m_pProjectionTransform = pCamera->GetProjectionTransform();

	D3DXMatrixMultiply(&m_ViewProjTransform, m_pViewTransform, m_pProjectionTransform);
}

void RD3d9RenderQueue::InsertModel(RModel* pModel)
{
	RenderState				renderState;

	const D3DXMATRIX* pWorldTransform = &(pModel->GetWorldTransform());

	// ʹ��ģ�;�������������ƽ���������ƽ����������׶��ü���ģ����������ľ�����Խ��ƿ�Ϊ��ȣ�ʵ����ʹ�ÿ���Ⱦ��Ԫ�����ĵ����жϾ��������
	float f32DepthSquare = RwgeMath::Distance2(*(m_pCameraPosition), pModel->GetWorldPosition());

	for (RMesh* pMesh : pModel->GetMeshes())
	{
		renderState.pMaterial = pMesh->GetMaterial();
		const list<RRenderUnit*>& listPrimitives = pMesh->GetRenderUnits();

		// ���Ҫ����shader
		if (m_bNeedUpdateCachedMaterialShader || renderState.pMaterial->GetCachedShader() == nullptr)
		{
			renderState.pShader = RD3d9ShaderManager::GetInstance().GetShader(RShaderKey(renderState.pMaterial->GetMaterialKey(), m_SceneKey, m_GlobalKey));
			renderState.pMaterial->SetCachedShader(renderState.pShader);
		}
		else
		{
			renderState.pShader = renderState.pMaterial->GetCachedShader();
		}

		for (RRenderUnit* pPrimitive : listPrimitives)
		{
			pPrimitive->SetWorldTransform(pWorldTransform);

			if (renderState.pMaterial->GetBlendMode() == EBM_Opaque || renderState.pMaterial->GetBlendMode() == EBM_Masked)
			{
				RenderUnitWithDepth renderPrimitiveWidthDepth(f32DepthSquare, pPrimitive);
				m_mapOpaqueGroup[renderState].insert(renderPrimitiveWidthDepth);
			}
			else
			{
				RenderStateWithDepth renderStateWithDepth(renderState);
				m_mapTranslucentGroup[renderStateWithDepth].push_back(pPrimitive);
			}
		}
	}
}

void RD3d9RenderQueue::Clear()
{
	m_mapOpaqueGroup.clear();
	m_mapTranslucentGroup.clear();
}