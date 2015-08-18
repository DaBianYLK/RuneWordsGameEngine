#pragma once

#include <d3d9.h>

#include "RwgeClasses.h"

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	void Initialize();
	void Draw();
	void Update(float deltaTime);
	void Cleanup();

	Scene* GetScene();
	Camera* GetCamera();

private:
	IDirect3DDevice9* m_pDevice;

	Scene* m_pScene;

	Camera* m_pCamera;
};

