#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <stack>

class Viewport;
class SceneNode;
class Camera;
class RwgeVertexShader;

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	void Initialize();
	void Draw(float deltaTime);
	void Cleanup();

	void SetViewport(Viewport* pViewport);

	SceneNode* GetSceneRootNode();
	Camera* GetCamera();

private:
	void TraversalSceneNode(SceneNode* pNode, float deltaTime);

private:
	IDirect3DDevice9* m_pDevice;
	RwgeVertexShader* m_pVertexShader;
	Viewport* m_pViewport;

	SceneNode* m_pSceneRootNode;
	std::stack<D3DXMATRIX*> m_pTransformMatrices;

	Camera* m_pCamera;

	D3DXMATRIX m_WorldViewProjectionMatrix;
};

