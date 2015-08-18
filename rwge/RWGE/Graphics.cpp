#include "Graphics.h"

#include "AppConfig.h"
#include "Application.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Window.h"
#include "Mesh.h"

Graphics::Graphics() {
	m_pDevice = NULL;
	m_pSceneManager = NULL;
}

Graphics::~Graphics() {

}

void Graphics::Initialize() {
	InitWindow();
	InitD3D9();
	InitSceneManager();

	Mesh::SetDevice(m_pDevice);

	m_pWindow->CreateViewport(m_pSceneManager->GetCamera());

	m_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	m_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	m_pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);


	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)m_pWindow->GetWidth() / (float)m_pWindow->GetHeight(),
		1.0f,
		1000.0f);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &proj);
}

void Graphics::Update(float deltaTime) {
	if (m_pDevice && m_pSceneManager) {
		m_pWindow->Update(deltaTime);

		m_pSceneManager->Update(deltaTime);
		m_pSceneManager->Draw();

		// Swap the back and front buffers.
		m_pDevice->Present(0, 0, 0, 0);
	}
}

void Graphics::Cleanup() {

}

IDirect3DDevice9* Graphics::GetD3D9Device() {
	return m_pDevice;
}

SceneManager* Graphics::GetSceneManager() {
	return m_pSceneManager;
}

void Graphics::InitWindow() {
	m_pWindow = new Window();
	m_pWindow->Initialize();
}

void Graphics::InitD3D9() {
	m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pD3D9) {
		MessageBox(0, "Initialize Direct3D-9 failed.", 0, 0);
		return;
	}

	D3DCAPS9 caps;
	m_pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT, AppConfig::deviceType, &caps);


	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		m_VertexProcType = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else {
		m_VertexProcType = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	HWND hWnd = m_pWindow->GetHandle();

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = m_pWindow->GetWidth();
	d3dpp.BackBufferHeight = m_pWindow->GetHeight();
	d3dpp.BackBufferFormat = AppConfig::backBufferFormat;
	d3dpp.BackBufferCount = AppConfig::backBufferCount;
	d3dpp.MultiSampleType = AppConfig::multiSampleType;
	d3dpp.MultiSampleQuality = AppConfig::multiSampleQuality;
	d3dpp.SwapEffect = AppConfig::swapEffect;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = !m_pWindow->IsFullScreen();
	d3dpp.EnableAutoDepthStencil = AppConfig::enableAutoDepthStencil;
	d3dpp.AutoDepthStencilFormat = AppConfig::autoDepthStencilFormat;
	d3dpp.Flags = AppConfig::flags;
	d3dpp.FullScreen_RefreshRateInHz = AppConfig::fullScreenRefreshRateInHz;
	d3dpp.PresentationInterval = AppConfig::presentationInterval;

	bool result = m_pD3D9->CreateDevice(
		D3DADAPTER_DEFAULT,				// primary adapter
		AppConfig::deviceType,			// device type
		hWnd,							// window associated with device
		m_VertexProcType,				// vertex processing
		&d3dpp,							// present parameters
		&m_pDevice);					// return created device

	if (FAILED(result)) {
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = AppConfig::autoDepthStencilFormat2;

		result = m_pD3D9->CreateDevice(
			D3DADAPTER_DEFAULT,
			AppConfig::deviceType,
			hWnd,
			m_VertexProcType,
			&d3dpp,
			&m_pDevice);

		if (FAILED(result)) {
			m_pD3D9->Release(); // done with d3d9 object
			MessageBox(0, "Create Direct3D-9 device failed.", 0, 0);
			return;
		}
	}

	m_pD3D9->Release(); // done with d3d9 object
}

void Graphics::InitSceneManager() {
	m_pSceneManager = new SceneManager();
	m_pSceneManager->Initialize();
}