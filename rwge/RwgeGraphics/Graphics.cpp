#include "Graphics.h"

#include "AppConfig.h"
#include "Application.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "DisplayWindow.h"
#include "Mesh.h"

Graphics::Graphics()
{
	m_pDevice = nullptr;
	m_pSceneManager = nullptr;
}

Graphics::~Graphics()
{

}

void Graphics::Init()
{
	InitWindow();
	InitD3D9();

	m_pVertexShader = new RwgeVertexShader();
	m_pPixelShader = new RwgePixelShader();

	InitSceneManager();

	Mesh::SetDevice(m_pDevice);
	Mesh::SetVertexShader(m_pVertexShader);
	Mesh::SetPixelShader(m_pPixelShader);
	D3DVERTEXELEMENT9 declaration[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
#ifdef SHADER_ANIMATION
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
#endif
		D3DDECL_END()
	};
	Mesh::SetVertexDeclaration(declaration);
}

void Graphics::Update(float deltaTime)
{
	if (m_pDevice && m_pSceneManager)
{
		#ifdef RWGE_SHADER_ENABLED
			m_pVertexShader->Enable();
			m_pPixelShader->Enable();
		#endif

		m_pWindow->Update(deltaTime);

		m_pSceneManager->Draw(deltaTime);

		// Swap the back and front buffers.
		m_pDevice->Present(0, 0, 0, 0);
	}
}

void Graphics::Cleanup()
{

}

IDirect3DDevice9* Graphics::GetD3D9Device() const
{
	return m_pDevice;
}

SceneManager* Graphics::GetSceneManager() const
{
	return m_pSceneManager;
}

DisplayWindow* Graphics::GetWindow() const
{
	return m_pWindow;
}

RwgeVertexShader* Graphics::GetVertexShader() const
{
	return m_pVertexShader;
}

void Graphics::InitWindow()
{
	m_pWindow = new DisplayWindow();
	m_pWindow->Initialize();
}

void Graphics::InitD3D9()
{
	
}

void Graphics::InitSceneManager()
{
	m_pSceneManager = new SceneManager();
	m_pSceneManager->Initialize();
}