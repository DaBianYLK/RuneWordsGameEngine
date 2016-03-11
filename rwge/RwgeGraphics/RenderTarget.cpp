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
	// clear函数只会重置标志，而不会清空缓存
	// 创建一个空的临时链表和利用swap函数交换内存，再利用临时链表的析构函数释放内存(如果需要立即释放可以通过大括号限定临时变量的作用域)
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
	// 注意，此处auto需要加上引用，否则会创建副本
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

	// 如果当前材质为空，则直接绑定新材质
	if (pCurrentMaterial == nullptr)
	{
		pNewShader->Begin();
		pNewMaterial->BindTextures(pNewShader);
		pNewMaterial->BindConstants(pNewShader);
	}
	// 如果当前材质与新材质不同
	else if (pCurrentMaterial != pNewMaterial)
	{
		ShaderProgram* pCurrentShader = pCurrentMaterial->GetShaderProgram();

		// 如果当前着色器为空，直接绑定新着色器（这里其实不可能成立，因为有材质一定有shader）
		if (pCurrentShader == nullptr)
		{
			pNewShader->Begin();
			pNewMaterial->BindTextures(pNewShader);
			pNewMaterial->BindConstants(pNewShader);
		}
		// 如果当前着色器与新着色器不同，就解绑当前着色器，绑定新着色器
		else if (pCurrentShader != pNewShader)
		{
			pCurrentShader->End();

			pNewShader->Begin();
			pNewMaterial->BindTextures(pNewShader);
			pNewMaterial->BindConstants(pNewShader);
		}
		// 如果当前着色器与新着色器相同，则只绑定纹理和常量
		else
		{
			pNewMaterial->BindTextures(pNewShader);
			pNewMaterial->BindConstants(pNewShader);
		}
	}
	// 如果当前材质与新材质相同，则不做任何操作

	// 设置变换矩阵
	pNewShader->SetTransform(&renderState.transform);

	// 设置新渲染状态为当前渲染状态
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
