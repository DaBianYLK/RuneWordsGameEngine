#include "SceneManager.h"

#include "Camera.h"
#include "Viewport.h"

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

void SceneManager::RenderScene(Camera* pCamera, Viewport* pViewport)
{
	// 遍历场景树，更新所有的场景节点
	m_pRoot->UpdateSelfAndAllChildren();

}