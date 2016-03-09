#include "SceneManager.h"

#include "Camera.h"
#include "Viewport.h"
#include "RenderSystem.h"
#include "Model.h"
#include "Mesh.h"
#include "MathDefinitions.h"

using namespace std;

SceneManager::SceneManager() :
	m_pRoot(new SceneNode()), 
	m_pLight(nullptr)
{

}

SceneManager::~SceneManager()
{

}

SceneNode* SceneManager::GetSceneRoot() const
{
	return m_pRoot;
}

void SceneManager::VisitScene(Viewport* pViewport)
{
	m_pActiveCamera = pViewport->GetCamera();

	// 遍历场景树，更新所有的场景节点
	m_pRoot->UpdateSelfAndAllChildren();

	// ToDo：视锥体裁剪

	// 将渲染图元加入渲染队列
	UpdateRenderQueue(m_pActiveCamera, RenderSystem::GetInstance().GetRenderQueuePtr());
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

void SceneManager::UpdateRenderQueue(Camera* pCamera, RenderQueue* pRenderQueue)
{
	pRenderQueue->Clear();

	RenderState renderState;

	D3DXMATRIX viewProjMatrix;
	D3DXMatrixMultiply(&viewProjMatrix, pCamera->GetViewTransform(), pCamera->GetProjectionTransform());

	// auto = pair<Model*, Model*>
	for (auto& pair : m_mapModels)
	{
		// m_mapModels中的Key和Value的值相同，它们指向同一个Model
		Model* pModel = pair.first;

		// 计算WVP变换矩阵
		D3DXMatrixMultiply(&renderState.transform, &pModel->GetWorldTransform(), &viewProjMatrix);

		// 使用模型距离摄像机距离的平方代替深度（经过视锥体裁剪后，模型与摄像机的距离可以近似看为深度）
		renderState.fDepth = RwgeMath::Distance2(pCamera->GetWorldPosition(), pModel->GetWorldPosition());

		// auto = Mesh*
		for (auto pMesh : pModel->GetMeshes())
		{
			Material* pMaterial = pMesh->GetMaterialPtr();
			const list<RenderPrimitive*> listPrimitives = pMesh->GetRenderPrimitives();

			// 只要材质与上一个材质不同时才计算ShaderKey
			if (pMaterial != renderState.pMaterial)
			{
				renderState.u64ShaderKey = ShaderManager::GetShaderProgramKey(pMaterial, m_pLight);
				renderState.pMaterial = pMaterial;
			}

			// auto = RenderPrimitive*
			for (auto pPrimitive : listPrimitives)
			{
				pRenderQueue->AddRenderPrimitive(pPrimitive, renderState);
			}
		}
	}
}
