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

void RenderSystem::RenderOneFrame(float fDeltaTime)
{
	// 注意，此处auto需要加上引用，否则会创建副本
	for (auto& renderTarget : m_RenderTargetList)
	{
		m_pActiveRenderTarget = &renderTarget;

		renderTarget.Update();
	}
}

bool RenderSystem::Init()
{
	m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pD3D9)
	{
		ErrorBox("Initialize Direct3D-9 failed.");
		return false;
	}

	return true;
}

bool RenderSystem::Release()
{
	D3D9SafeRelease(m_pD3D9);

	return true;
}

IDirect3D9* RenderSystem::GetD3D9Ptr() const
{
	return m_pD3D9;
}

RenderTarget* RenderSystem::CreateRenderTarget(DisplayWindow& window)
{
	m_RenderTargetList.push_back(RenderTarget(window));
	m_pActiveRenderTarget = &m_RenderTargetList.back();

	return m_pActiveRenderTarget;
}

bool RenderSystem::RemoveRenderTarget(RenderTarget* pTarget)
{
	ASSERT(pTarget);

	if (pTarget == m_pActiveRenderTarget)
	{
		m_pActiveRenderTarget = nullptr;
	}

	m_RenderTargetList.remove(*m_pActiveRenderTarget);

	return true;
}

RenderTarget* RenderSystem::GetActiveRenderTarget() const
{
	return m_pActiveRenderTarget;
}

RenderQueue* RenderSystem::GetRenderQueuePtr()
{
	return &m_RenderQueue;
}
