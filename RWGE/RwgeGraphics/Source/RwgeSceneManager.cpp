#include "RwgeSceneManager.h"

#include <RwgeLog.h>
#include "RwgeCamera.h"
#include "RwgeD3d9Viewport.h"
#include "RwgeModel.h"
#include "RwgeLight.h"
#include "RwgeD3d9RenderQueue.h"

using namespace std;

RSceneManager::RSceneManager() :
	m_pRoot(new RSceneNode()), 
	m_pLight(nullptr), 
	m_pActiveCamera(nullptr), 
	m_bSceneChanged(false)
{
	m_pRoot->m_pSceneManager = this;
}

RSceneManager::~RSceneManager()
{

}

RSceneNode* RSceneManager::GetSceneRoot() const
{
	return m_pRoot;
}

void RSceneManager::RenderScene(RCamera* pCamera, RD3d9Viewport* pViewport, RD3d9RenderQueue& renderQueue)
{
	m_pActiveCamera = pCamera;

	// 遍历场景树，更新所有的场景节点
	m_pRoot->UpdateSelfAndAllChildren();

	// 遍历场景树，将模型加入到渲染队列
	// ToDo：这里需要实现空间划分树与视锥体裁剪
	renderQueue.Clear();
	renderQueue.SetCamera(pCamera);
	renderQueue.SetLight(m_pLight);
	if (m_bSceneChanged)
	{
		renderQueue.NeedUpdateCachedMaterialShader();
	}
	renderQueue.SetSceneKey(GetSceneKey());
	FindModelsInSceneTree(m_pRoot, renderQueue);
}

void RSceneManager::SetLight(RLight* pLight)
{
	m_pLight = pLight;
	
	m_bSceneChanged = true;
}

const RLight* RSceneManager::GetLight() const
{
	return m_pLight;
}

RCamera* RSceneManager::GetActiveCamera()
{
	return m_pActiveCamera;
}

const SceneKey& RSceneManager::GetSceneKey()
{
	if (m_bSceneChanged)
	{
		if (m_pLight)
		{
			m_SceneKey.SetLightTypeKey(m_pLight->GetLightType());
		}
		else
		{
			m_SceneKey.SetLightTypeKey(ELT_NoLight);
		}
		
		m_bSceneChanged = false;
	}

	return m_SceneKey;
}

void RSceneManager::FindModelsInSceneTree(RSceneNode* pNode, RD3d9RenderQueue& renderQueue)
{
	// 场景节点的插入操作保证了场景树中不可能存在空节点，因此不需要做指针检查

	if (pNode->m_NodeType == RSceneNode::ENT_Model)
	{
		renderQueue.InsertModel(reinterpret_cast<RModel*>(pNode));
	}

	for (RSceneNode* pChildNode : pNode->m_listChildren)
	{
		FindModelsInSceneTree(pChildNode, renderQueue);
	}
}
