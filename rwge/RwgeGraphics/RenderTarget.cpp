#include "RenderTarget.h"

#include "DisplayWindow.h"
#include <AssertUtil.h>

using namespace std;

RenderTarget::RenderTarget(DisplayWindow& window) : 
	D3D9Device					(window),
	m_pWindow					(&window),
	m_pActiveViewport			(nullptr), 
	m_IndexStreamBuffer			(*this, m_uIndexStreamBufferSize), 
	m_ShaderManager				(this),
	m_VertexDeclarationManager	(this)
{
	m_listVertexStreamBuffers.push_back(VertexStreamBuffer(*this, m_uVertexStreamBufferSize));
}

RenderTarget::~RenderTarget()
{
	
}

bool RenderTarget::Release()
{
	// clear����ֻ�����ñ�־����������ջ���
	// ����һ���յ���ʱ���������swap���������ڴ棬��������ʱ��������������ͷ��ڴ�(�����Ҫ�����ͷſ���ͨ���������޶���ʱ������������)
	ViewportList emptyList;
	m_ViewportList.swap(emptyList);
	
	return __super::Release();
}

Viewport* RenderTarget::CreateViewport()
{
	m_ViewportList.emplace_back(Viewport(m_pDevice, 0, 0, m_pWindow->GetWidth(), m_pWindow->GetHeight()));
	m_pActiveViewport = &m_ViewportList.back();
	m_pActiveViewport->m_pRenderTarget = this;

	return m_pActiveViewport;
}

bool RenderTarget::RemoveViewport(Viewport* pViewport)
{
	ASSERT(pViewport);

	if (pViewport == m_pActiveViewport)
	{
		m_pActiveViewport = nullptr;
	}

	m_ViewportList.remove(*pViewport);

	return true;
}

void RenderTarget::Update()
{
	// ע�⣬�˴�auto��Ҫ�������ã�����ᴴ������
	for (auto& viewport : m_ViewportList)
	{
		m_pActiveViewport = &viewport;

		viewport.Update();
	}
}

void RenderTarget::SetActiveSceneManager(SceneManager* pSceneManager)
{
	m_pActiveSceneManager = pSceneManager;
}

SceneManager* RenderTarget::GetActiveSceneManager() const
{
	return m_pActiveSceneManager;
}

ShaderManager* RenderTarget::GetShaderManager()
{
	return &m_ShaderManager;
}

void RenderTarget::ApplyRenderState(const RenderState& renderState)
{
	Material* pCurrentMaterial = m_CurrentRenderState.pMaterial;
	Material* pNewMaterial = renderState.pMaterial;

	ShaderProgram* pNewShader = pNewMaterial->GetShaderProgram();

	// �����ǰ����Ϊ�գ���ֱ�Ӱ��²���
	if (pCurrentMaterial == nullptr)
	{
		pNewShader->Begin();
		pNewMaterial->BindTextures(pNewShader);
		pNewMaterial->BindConstants(pNewShader);
	}
	// �����ǰ�������²��ʲ�ͬ
	else if (pCurrentMaterial != pNewMaterial)
	{
		ShaderProgram* pCurrentShader = pCurrentMaterial->GetShaderProgram();

		// �����ǰ��ɫ��Ϊ�գ�ֱ�Ӱ�����ɫ����������ʵ�����ܳ�������Ϊ�в���һ����shader��
		if (pCurrentShader == nullptr)
		{
			pNewShader->Begin();
			pNewMaterial->BindTextures(pNewShader);
			pNewMaterial->BindConstants(pNewShader);
		}
		// �����ǰ��ɫ��������ɫ����ͬ���ͽ��ǰ��ɫ����������ɫ��
		else if (pCurrentShader != pNewShader)
		{
			pCurrentShader->End();

			pNewShader->Begin();
			pNewMaterial->BindTextures(pNewShader);
			pNewMaterial->BindConstants(pNewShader);
		}
		// �����ǰ��ɫ��������ɫ����ͬ����ֻ������ͳ���
		else
		{
			pNewMaterial->BindTextures(pNewShader);
			pNewMaterial->BindConstants(pNewShader);
		}
	}
	// �����ǰ�������²�����ͬ�������κβ���

	// ���ñ任����
	pNewShader->SetTransform(&renderState.transform);

	// ��������Ⱦ״̬Ϊ��ǰ��Ⱦ״̬
	m_CurrentRenderState = renderState;
}

void RenderTarget::ResetRenderState()
{
	Material* pMaterial = m_CurrentRenderState.pMaterial;

	if (pMaterial != nullptr)
	{
		ShaderProgram* pShader = pMaterial->GetShaderProgram();

		if (pShader != nullptr)
		{
			pShader->End();
		}
	}

	m_CurrentRenderState = RenderState();
}

void RenderTarget::ApplyVertexDeclaration(VertexDeclaration* pVertexDeclaration)
{
	if (pVertexDeclaration != m_pCurrentVertexDeclaration)
	{
		pVertexDeclaration->Enable();

		m_pCurrentVertexDeclaration = pVertexDeclaration;
	}
}
