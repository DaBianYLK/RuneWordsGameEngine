#include "Viewport.h"

#include "AppConfig.h"
#include "Graphics.h"
#include "Camera.h"

Viewport::Viewport(Camera* pCamera)
{
	m_pDevice = Graphics::GetInstance()->GetD3D9Device();
	m_pVertexShader = Graphics::GetInstance()->GetVertexShader();

	m_pCamera = pCamera;

	#ifndef RWGE_SHADER_ENABLED
		m_pDevice->SetTransform(D3DTS_PROJECTION, pCamera->GetProjectionMatrix());
	#endif
}

Viewport::~Viewport()
{
	
}

void Viewport::Update(float deltaTime)
{
	m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_BackgroundColor, 1.0f, 0);

	#ifdef RWGE_SHADER_ENABLED
		m_pViewMatrix = m_pCamera->GetViewMatrix();
		D3DXMatrixMultiply(&m_ViewProjectionMatrix, m_pViewMatrix, m_pCamera->GetProjectionMatrix());
	#else
		m_pDevice->SetTransform(D3DTS_VIEW, m_pCamera->GetViewMatrix());
	#endif
}

void Viewport::Clear()
{
	m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_BackgroundColor, 1.0f, 0);
}

void Viewport::SetBackgroundColor(const D3DCOLOR& color)
{
	m_BackgroundColor = color;
}

D3DXMATRIX* Viewport::GetViewTransform()
{
	return m_pViewMatrix;
}

D3DXMATRIX* Viewport::GetViewportTransform()
{
	return &m_ViewProjectionMatrix;
}

Camera* Viewport::GetCamera()
{
	return m_pCamera;
}