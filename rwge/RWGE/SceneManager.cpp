#include "SceneManager.h"

#include <d3d9.h>
#include <stack>

#include "AppConfig.h"
#include "Graphics.h"
#include "Camera.h"
#include "Sprite.h"

using namespace std;

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {

}

void SceneManager::Initialize() {
	m_pDevice = Graphics::GetInstance()->GetD3D9Device();

	D3DXVECTOR3 position(AppConfig::cameraPosX, AppConfig::cameraPosY, AppConfig::cameraPosZ);
	D3DXVECTOR3 rightAxis(AppConfig::cameraRightAxisX, AppConfig::cameraRightAxisY, AppConfig::cameraRightAxisZ);
	D3DXVECTOR3 upAxis(AppConfig::cameraUpAxisX, AppConfig::cameraUpAxisY, AppConfig::cameraUpAxisZ);
	D3DXVECTOR3 lookAxis(AppConfig::cameraLookAxisX, AppConfig::cameraLookAxisY, AppConfig::cameraLookAxisZ);
	m_pCamera = new Camera(position, rightAxis, upAxis, lookAxis, 
						   AppConfig::cameraFovy, AppConfig::cameraAspect, AppConfig::cameraLookNear, AppConfig::cameraLookFar);

	m_pSceneRootNode = new SceneNode();
}

void SceneManager::Draw(float deltaTime) {
	m_pDevice->BeginScene();

	// 遍历场景树
	list<SceneNode*>::iterator pNodeIterator = m_pSceneRootNode->m_pChildren.begin();
	while (pNodeIterator != m_pSceneRootNode->m_pChildren.end()) {
		TraversalSceneNode(*pNodeIterator, deltaTime);

		pNodeIterator++;
	}

	m_pDevice->EndScene();
}

void SceneManager::Cleanup() {

}

SceneNode* SceneManager::GetSceneRootNode() {
	return m_pSceneRootNode;
}

Camera* SceneManager::GetCamera() {
	return m_pCamera;
}

void SceneManager::TraversalSceneNode(SceneNode* pNode, float deltaTime) {
	D3DXMATRIX* pTransformMatrix = pNode->GetTransformMatrix();

	if (!m_pTransformMatrices.empty()) {
		D3DXMatrixMultiply(pTransformMatrix, pTransformMatrix, m_pTransformMatrices.top());
	}

	m_pTransformMatrices.push(pTransformMatrix);

	// 如果当前节点为精灵，则进行绘制
	if (pNode->m_NodeType == SceneNode::Type::SpriteNode) {
		Sprite* pSprite = static_cast<Sprite*>(pNode);

		pSprite->Update(deltaTime);
		pSprite->Draw();
	}

	// 遍历子节点
	list<SceneNode*>::iterator pNodeIterator = pNode->m_pChildren.begin();
	while (pNodeIterator != pNode->m_pChildren.end()) {
		TraversalSceneNode(*pNodeIterator, deltaTime);

		pNodeIterator++;
	}

	m_pTransformMatrices.pop();
}