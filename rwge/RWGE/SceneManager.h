#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <stack>

#include "RwgeClasses.h"

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	void Initialize();
	void Draw(float deltaTime);
	void Cleanup();

	SceneNode* GetSceneRootNode();
	Camera* GetCamera();

private:
	void TraversalSceneNode(SceneNode* pNode, float deltaTime);

private:
	IDirect3DDevice9* m_pDevice;

	SceneNode* m_pSceneRootNode;
	std::stack<D3DXMATRIX*> m_pTransformMatrices;

	Camera* m_pCamera;
};

