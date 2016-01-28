#pragma once

#include <Windows.h>
#include "GraphicsDefinitions.h"

#include "Singleton.h"
#include "RwgeVertexShader.h"
#include "RwgePixelShader.h"

class SceneManager;
class DisplayWindow;

class Graphics : public Singleton<Graphics>
{
public:
	Graphics();
	~Graphics();

	void Init();
	void Update(float deltaTime);
	void Cleanup();

	IDirect3DDevice9* GetD3D9Device() const;
	SceneManager* GetSceneManager() const;
	DisplayWindow* GetWindow() const;
	RwgeVertexShader* GetVertexShader() const;

private:
	void InitWindow();
	void InitD3D9();
	void InitSceneManager();
	

private:
	DisplayWindow* m_pWindow;

	IDirect3D9* m_pD3D9;
	IDirect3DDevice9* m_pDevice;

	SceneManager* m_pSceneManager;

	RwgeVertexShader* m_pVertexShader;
	RwgePixelShader* m_pPixelShader;
};

