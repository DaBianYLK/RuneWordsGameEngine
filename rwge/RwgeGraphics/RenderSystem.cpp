#include "RenderSystem.h"

#include "GraphicsDefinitions.h"
#include "SceneManager.h"
#include "Light.h"
#include "Camera.h"

RenderSystem::RenderSystem()
{
	InitD3DD9();
}

RenderSystem::~RenderSystem()
{
	Release();
}

void RenderSystem::RenderOneFrame(float fDeltaTime)
{
	// ע�⣬�˴�auto��Ҫ�������ã�����ᴴ������
	for (auto& renderTarget : m_RenderTargetList)
	{
		m_pActiveRenderTarget = &renderTarget;

		renderTarget.Update();
	}
}

void RenderSystem::RenderScene(SceneManager* pSceneManager)
{
	// ������ɫ��������
	ShaderProgram* pShader = m_pActiveRenderTarget->m_ShaderManager.GetFirstShaderProgram();

	unsigned char* pLight;
	unsigned char uSize;
	pSceneManager->GetLight()->GetConstantBuffer(pLight, uSize);
	pShader->SetLight(pLight, uSize);

	D3DXVECTOR3 viewOppoisteDirection = -pSceneManager->GetActiveCamera()->GetDirection();
	pShader->SetViewOppositeDirection(&viewOppoisteDirection);

	// ������Ⱦ���У���ִ�л��ƺ���
	VisitRenderQueue();
}

void RenderSystem::VisitRenderQueue()
{
	// �Ȼ���͸�����е�ͼԪ
	//auto = pair <RenderStateKey, std::list<RenderPrimitive*>>
	for (auto& renderOperation : m_RenderQueue.m_OpaqueGroup)
	{
		ApplyRenderState(renderOperation.first);
	}

	// �ٻ���͸�����е�ͼԪ
	//auto = pair <RenderStateKey, std::list<RenderPrimitive*>>
	for (auto& renderOperation : m_RenderQueue.m_TranslucentGroup)
	{

	}
}

void RenderSystem::ApplyRenderState(const RenderState& renderState)
{
	if (m_CurrentRenderState.u64ShaderKey != renderState.u64ShaderKey)
	{

	}
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
