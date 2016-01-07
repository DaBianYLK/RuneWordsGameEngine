#pragma once

#include "AppConfig.h"
#include "VertexShader.h"

class RwgeVertexShader : public VertexShader {
public:
	RwgeVertexShader();
	~RwgeVertexShader();

	void SetViewTransform(const D3DXMATRIX* pMatrix);
	void SetWorldTransform(const D3DXMATRIX* pMatrix);
	void SetWorldViewProjectionTransform(const D3DXMATRIX* pMatrix);

#ifdef SHADER_ANIMATION
	void SetModelTransform(const D3DXMATRIX* matrices, unsigned int num);
#endif

	void SetMaterial(const D3DMATERIAL9* pMaterial);
	void SetLight(const D3DLIGHT9* pLight);

private:
	D3DXHANDLE m_hViewMatrix;
	D3DXHANDLE m_hWorldMatrix;
	D3DXHANDLE m_hWorldViewProjectionMatrix;

#ifdef SHADER_ANIMATION
	D3DXHANDLE m_hModelMatrices;
#endif

	D3DXHANDLE m_hMaterial;
	D3DXHANDLE m_hLight;
};

