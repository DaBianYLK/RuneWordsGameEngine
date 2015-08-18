#include "Viewport.h"

#include "Graphics.h"
#include "Camera.h"

Viewport::Viewport(Camera* pCamera) {
	m_pDevice = Graphics::GetInstance()->GetD3D9Device();
	m_pCamera = pCamera;

}

Viewport::~Viewport() {
	
}

void Viewport::Update(float deltaTime) {
	m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_BackgroundColor, 1.0f, 0);

	m_pDevice->SetTransform(D3DTS_VIEW, m_pCamera->GetViewMatrix());
}

void Viewport::Clear() {
	m_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, m_BackgroundColor, 1.0f, 0);
}

void Viewport::SetBackgroundColor(const D3DCOLOR& color) {
	m_BackgroundColor = color;
}