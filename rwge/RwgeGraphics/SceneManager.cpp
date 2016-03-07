#include "SceneManager.h"

#include "Camera.h"
#include "Viewport.h"
#include "RenderSystem.h"
#include "Model.h"
#include "Mesh.h"
#include "MathDefinitions.h"

using namespace std;

SceneManager::SceneManager() :
	m_pRoot(new SceneNode())
{

}

SceneManager::~SceneManager()
{

}

SceneNode* SceneManager::GetSceneRoot() const
{
	return m_pRoot;
}

void SceneManager::RenderScene(Viewport* pViewport, RenderQueue* pRenderQueue)
{
	// �������������������еĳ����ڵ�
	m_pRoot->UpdateSelfAndAllChildren();

	// ToDo����׶��ü�

	// ����ȾͼԪ������Ⱦ����
	PutPrimitiveIntoRenderQueue(pViewport->GetCamera(), pRenderQueue);
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

void SceneManager::PutPrimitiveIntoRenderQueue(Camera* pCamera, RenderQueue* pRenderQueue)
{
	pRenderQueue->Clear();

	// auto = pair<Model*, Model*>
	for (auto& pair : m_mapModels)
	{
		// m_mapModels�е�Key��Value��ֵ��ͬ������ָ��ͬһ��Model
		Model* pModel = pair.first;

		// ʹ��ģ�;�������������ƽ��������ȣ�������׶��ü���ģ����������ľ�����Խ��ƿ�Ϊ��ȣ�
		float fDepth2 = RwgeMath::Distance2(pCamera->GetWorldPosition(), pModel->GetWorldPosition());

		// auto = Mesh*
		for (auto pMesh : pModel->GetMeshes())
		{
			Material* pMaterial = pMesh->GetMaterialPtr();
			const list<RenderPrimitive*> listPrimitives = pMesh->GetRenderPrimitives();

			// auto = RenderPrimitive*
			for (auto pPrimitive : listPrimitives)
			{
				pRenderQueue->AddRenderPrimitive(pPrimitive, pMaterial, fDepth2);
			}
		}
	}
}
