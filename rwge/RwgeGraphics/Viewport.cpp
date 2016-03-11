#include "Viewport.h"

#include "Camera.h"
#include "D3DX9Extension.h"
#include "RenderTarget.h"

using namespace D3DX9Extension;

Viewport::Viewport(IDirect3DDevice9* pDevice, unsigned uX, unsigned uY, unsigned uWidth, unsigned uHeight): 
	m_pCamera			(nullptr),
	m_BackgroundColor	(D3DColorBlack)
{
	m_pDevice = pDevice;
	m_D3D9ViewportParam = {uX, uY, uWidth, uHeight, 0.0f, 1.0f};
	m_Rect = {uX, uY, uX + uWidth, uY + uHeight};
}

Viewport::~Viewport()
{
	
}

void Viewport::Update()
{
	Clear();

	if (m_pCamera)
	{
		m_pCamera->RenderScene(this);
	}
}

void Viewport::Enable() const
{
	m_pDevice->SetViewport(&m_D3D9ViewportParam);
}

void Viewport::Clear() const
{
	m_pDevice->Clear(0, &m_Rect, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_BackgroundColor, m_D3D9ViewportParam.MaxZ, 0);
}

void Viewport::SetRect(unsigned uX, unsigned uY, unsigned uWidth, unsigned uHeight)
{
	m_D3D9ViewportParam = { uX, uY, uWidth, uHeight, 0.0f, 1.0f };
	m_Rect = { uX, uY, uX + uWidth, uY + uHeight };
}

void Viewport::SetBackgroundColor(const D3DCOLOR& color)
{
	m_BackgroundColor = color;
}

void Viewport::SetCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}

Camera* Viewport::GetCamera() const
{
	return m_pCamera;
}

RenderTarget* Viewport::GetRenderTarget() const
{
	return m_pRenderTarget;
}
