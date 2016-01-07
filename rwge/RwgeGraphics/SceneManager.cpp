#include "SceneManager.h"

#include <d3d9.h>
#include <stack>

#include "AppConfig.h"
#include "Graphics.h"
#include "Camera.h"
#include "Sprite.h"
#include "Viewport.h"
#include "RwgeVertexShader.h"
#include "Window.h"

using namespace std;

SceneManager::SceneManager() {

}

SceneManager::~SceneManager() {

}

void SceneManager::Initialize() {
	m_pDevice = Graphics::GetInstance()->GetD3D9Device();
	m_pVertexShader = Graphics::GetInstance()->GetVertexShader();
	m_pViewport = Graphics::GetInstance()->GetWindow()->GetViewport();

	m_pSceneRootNode = new SceneNode();
}

void SceneManager::Draw(float deltaTime) {
	m_pDevice->BeginScene();

	// 将视图变换矩阵传入着色器
	m_pVertexShader->SetViewTransform(m_pViewport->GetViewTransform());

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

void SceneManager::SetViewport(Viewport* pViewport) {
	m_pViewport = pViewport;
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

	D3DXMatrixMultiply(&m_WorldViewProjectionMatrix, pTransformMatrix, m_pViewport->GetViewportTransform());
	m_pVertexShader->SetWorldTransform(pTransformMatrix);
	m_pVertexShader->SetWorldViewProjectionTransform(&m_WorldViewProjectionMatrix);

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