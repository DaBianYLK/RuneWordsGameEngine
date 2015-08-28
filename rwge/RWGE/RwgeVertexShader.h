#pragma once

#include "VertexShader.h"

class RwgeVertexShader : public VertexShader {
public:
	RwgeVertexShader();
	~RwgeVertexShader();

	void SetViewTransform(const D3DXMATRIX* pMatrix);
	void SetViewProjectionTransform(const D3DXMATRIX* pMatrix);
	//void SetWorldViewProjectionTransform(const D3DXMATRIX* pMatrix);

	void SetMaterial(const D3DMATERIAL9* pMaterial);
	void SetLight(const D3DLIGHT9* pLight);

private:
	D3DXHANDLE m_hViewMatrix;
	D3DXHANDLE m_hViewProjectionMatrix;

	D3DXHANDLE m_hMaterialAmbient;
	D3DXHANDLE m_hMaterialDiffuse;

	D3DXHANDLE m_hLightDiffuse;
	D3DXHANDLE m_hLightAmbient;
	D3DXHANDLE m_hLightDirection;
};

