#include "SceneManager.h"

#include "Camera.h"
#include "Viewport.h"
#include "RenderSystem.h"
#include "Model.h"
#include "Mesh.h"
#include "MathDefinitions.h"
#include "ShaderManager.h"
#include <LogUtil.h>

using namespace std;

SceneManager::SceneManager() :
	m_pRoot(new SceneNode()), 
	m_pLight(nullptr), 
	m_pActiveCamera(nullptr), 
	m_bEnvironmentChanged(false)
{
	m_pRoot->m_pSceneManager = this;
}

SceneManager::~SceneManager()
{

}

SceneNode* SceneManager::GetSceneRoot() const
{
	return m_pRoot;
}

void SceneManager::RenderScene(Viewport* pViewport)
{
	m_pActiveCamera = pViewport->GetCamera();

	RenderTarget* pRenderTarget = pViewport->GetRenderTarget();

	bool bSceneChanged = pRenderTarget->GetActiveSceneManager() != this;
	if (bSceneChanged)
	{
		pRenderTarget->SetActiveSceneManager(this);
	}

	// �������������������еĳ����ڵ�
	m_pRoot->UpdateSelfAndAllChildren();

	// ToDo����׶��ü�

	// ����ȾͼԪ������Ⱦ����
	SetupRenderQueue(m_pActiveCamera, RenderSystem::GetInstance().GetRenderQueuePtr(), m_bEnvironmentChanged || bSceneChanged);

	RenderSystem::GetInstance().RenderScene(this);
}

void SceneManager::AddModel(Model* pModel)
{
	m_mapModels.insert(make_pair(pModel, pModel));
}

void SceneManager::RemoveModel(Model* pModel)
{
	m_mapModels.erase(pModel);
}

void SceneManager::AddModelBySceneNode(SceneNode* pNode)
{
	if (pNode->m_NodeType == SceneNode::NT_Model)
	{
		AddModel(reinterpret_cast<Model*>(pNode));
	}

	for (auto pChild : pNode->m_listChildren)
	{
		AddModelBySceneNode(pChild);
	}
}

void SceneManager::RemoveModelBySceneNode(SceneNode* pNode)
{
	if (pNode->m_NodeType == SceneNode::NT_Model)
	{
		RemoveModel(reinterpret_cast<Model*>(pNode));
	}

	for (auto pChild : pNode->m_listChildren)
	{
		RemoveModelBySceneNode(pChild);
	}
}

void SceneManager::SetLight(Light* pLight)
{
	m_pLight = pLight;
}

Light* SceneManager::GetLight()
{
	return m_pLight;
}

Camera* SceneManager::GetActiveCamera()
{
	return m_pActiveCamera;
}

void SceneManager::SetupRenderQueue(Camera* pCamera, RenderQueue* pRenderQueue, bool bNeedUpdateShader)
{
	pRenderQueue->Clear();

	RenderState renderState;

	D3DXMATRIX viewProjMatrix;
	D3DXMatrixMultiply(&viewProjMatrix, pCamera->GetViewTransform(), pCamera->GetProjectionTransform());

	// auto = pair<Model*, Model*>
	for (auto& pair : m_mapModels)
	{
		// m_mapModels�е�Key��Value��ֵ��ͬ������ָ��ͬһ��Model
		Model* pModel = pair.first;

		renderState.transformWorld = pModel->GetWorldTransform();

		// ����WVP�任����
		D3DXMatrixMultiply(&renderState.transformWVP, &renderState.transformWorld, &viewProjMatrix);

		// ʹ��ģ�;�������������ƽ��������ȣ�������׶��ü���ģ����������ľ�����Խ��ƿ�Ϊ��ȣ�
		renderState.fDepth = RwgeMath::Distance2(pCamera->GetWorldPosition(), pModel->GetWorldPosition());

		// auto = Mesh*
		for (auto pMesh : pModel->GetMeshes())
		{
			Material* pMaterial = pMesh->GetMaterialPtr();
			const list<RenderPrimitive*> listPrimitives = pMesh->GetRenderPrimitives();

			renderState.pMaterial = pMaterial;

			// ���Ҫ����shader
			if (pMaterial->GetShaderType() == nullptr || bNeedUpdateShader)
			{
				unsigned long long u64ShaderKey = ShaderManager::GetShaderKey(pMaterial->GetMaterialKey(), ShaderManager::GetEnvironmentKey(m_pLight));
				pMaterial->SetShaderType(ShaderManager::GetInstance().GetShaderType(u64ShaderKey));
			}

			// auto = RenderPrimitive*
			for (auto pPrimitive : listPrimitives)
			{
				pRenderQueue->AddRenderPrimitive(pPrimitive, renderState);
			}
		}
	}
}
