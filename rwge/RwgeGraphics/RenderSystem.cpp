#include "RenderSystem.h"

#include "GraphicsDefinitions.h"

RenderSystem::RenderSystem()
{
	Init();
}

RenderSystem::~RenderSystem()
{
	Release();
}

bool RenderSystem::Init()
{
	m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pD3D9)
	{
		ErrorBox("Initialize Direct3D-9 failed.");
		return false;
	}
}

bool RenderSystem::Release()
{
	D3D9SafeRelease(m_pD3D9);
}

IDirect3D9* RenderSystem::GetD3D9Ptr() const
{
	return m_pD3D9;
}

RenderTarget* RenderSystem::CreateRenderTarget(DisplayWindow& window)
{

}

bool RenderSystem::RemoveRenderTarget(RenderTarget* pTarget)
{
}

RenderTarget* RenderSystem::GetActiveRenderTarget()
{
}

