#pragma once

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "RwgeClasses.h"
#include "Singleton.h"

class Graphics : public Singleton<Graphics> {
public:
	Graphics();
	~Graphics();

	void Initialize();
	void Update(float deltaTime);
	void Cleanup();

	IDirect3DDevice9* GetD3D9Device();
	SceneManager* GetSceneManager();

private:
	void InitWindow();
	void InitD3D9();
	void InitSceneManager();
	

private:
	Window* m_pWindow;

	IDirect3D9* m_pD3D9;
	IDirect3DDevice9* m_pDevice;
	int m_VertexProcType;

	SceneManager* m_pSceneManager;
};

