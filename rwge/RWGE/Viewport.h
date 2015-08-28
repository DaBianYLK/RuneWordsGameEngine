#pragma once

#include <d3d9.h>

#include "RwgeClasses.h"

class Viewport {
	friend class Window;

private:
	Viewport(Camera* pCamera);
	~Viewport();

public:
	void Update(float deltaTime);

	void Clear();
	void SetBackgroundColor(const D3DCOLOR& color);

private:
	IDirect3DDevice9* m_pDevice;
	RwgeVertexShader* m_pVertexShader;

	Camera* m_pCamera;
	D3DCOLOR m_BackgroundColor;
};

