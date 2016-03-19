#include "RenderSystem.h"

#include "GraphicsDefinitions.h"
#include "SceneManager.h"
#include "Light.h"
#include "RenderPrimitive.h"
#include "Camera.h"

RenderSystem::RenderSystem()
{
	InitD3DD9();
}

RenderSystem::~RenderSystem()
{
	Release();
}

bool RenderSystem::InitD3DD9()
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

void RenderSystem::RenderOneFrame(float fDeltaTime)
{
	// 注意，此处auto需要加上引用，否则会创建副本
	for (auto pRenderTarget : m_listRenderTarget)
	{
		m_pActiveRenderTarget = pRenderTarget;

		pRenderTarget->Update();
	}
}

void RenderSystem::RenderScene(SceneManager* pSceneManager)
{
	// 设置着色器共享常量
	Shader* pShader = m_pActiveRenderTarget->GetShader();

	//static bool bLighted = false;
	//if (!bLighted)
	{
		void* pLightData;
		unsigned char uSize;
		pSceneManager->GetLight()->GetConstantBuffer(pLightData, uSize);
		pShader->SetLight(pLightData, uSize);

	//	bLighted = true;
	}

	D3DXVECTOR3 viewOppoisteDirection = -(pSceneManager->GetActiveCamera()->GetDirection());
	pShader->SetViewOppositeDirection(&viewOppoisteDirection);

	// 遍历渲染队列，并执行绘制函数
	DrawRenderQueue();
}

void RenderSystem::DrawRenderQueue()
{
	m_pActiveRenderTarget->GetDevicePtr()->BeginScene();

	// 先画不透明组中的图元
	//auto = pair <RenderStateKey, std::list<RenderPrimitive*>>
	for (auto& renderOperation : m_RenderQueue.m_OpaqueGroup)
	{
		ApplyRenderState(renderOperation.first);

		// auto = RenderPrimitive*
		for (auto pPrimitive : renderOperation.second)
		{
			DrawPrimitive(*pPrimitive);
		}
	}

	// 再画半透明组中的图元
	//auto = pair <RenderStateKey, std::list<RenderPrimitive*>>
	for (auto& renderOperation : m_RenderQueue.m_TranslucentGroup)
	{
		ApplyRenderState(renderOperation.first);

		// auto = RenderPrimitive*
		for (auto pPrimitive : renderOperation.second)
		{
			DrawPrimitive(*pPrimitive);
		}
	}

	m_pActiveRenderTarget->GetDevicePtr()->EndScene();

	//ResetRenderState();
}

void RenderSystem::ApplyRenderState(const RenderState& renderState)
{
	m_pActiveRenderTarget->ApplyRenderState(renderState);
}

void RenderSystem::ResetRenderState()
{
	m_pActiveRenderTarget->ResetRenderState();
}

void RenderSystem::DrawPrimitive(const RenderPrimitive& primitive)
{
	m_pActiveRenderTarget->DrawPrimitive(primitive);
}

IDirect3D9* RenderSystem::GetD3D9Ptr() const
{
	return m_pD3D9;
}

RenderTarget* RenderSystem::CreateRenderTarget(const DisplayWindow* pWindow)
{
	m_pActiveRenderTarget = new RenderTarget(pWindow);

	m_listRenderTarget.push_back(m_pActiveRenderTarget);

	return m_pActiveRenderTarget;
}

bool RenderSystem::RemoveRenderTarget(RenderTarget* pTarget)
{
	ASSERT(pTarget);

	if (pTarget == m_pActiveRenderTarget)
	{
		m_pActiveRenderTarget = nullptr;
	}

	m_listRenderTarget.remove(pTarget);

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
