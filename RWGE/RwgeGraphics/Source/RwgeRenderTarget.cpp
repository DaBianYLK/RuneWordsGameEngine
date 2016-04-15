#include "RwgeRenderTarget.h"

#include "RwgeWindow.h"
#include "RwgeShaderManager.h"
#include "RwgeVertexDeclarationType.h"
#include "RwgeRenderPrimitive.h"
#include "RwgeVertexStream.h"
#include "RwgeIndexStream.h"
#include <RwgeAssert.h>

using namespace std;

RenderTarget::RenderTarget(const RWindow* pWindow) :
	D3D9Device							(*pWindow),
	m_pWindow							(pWindow),
	m_pActiveViewport					(nullptr), 
	m_pIndexBuffer				(nullptr),
	m_pEffectPool						(nullptr),
	m_pCurrentVertexDeclaration	(nullptr),
	m_pCurrentIndexBuffer			(nullptr)
{
	HRESULT hResult = D3DXCreateEffectPool(&m_pEffectPool);
	if (FAILED(hResult))
	{
		ErrorBox("Create effect pool failed.")
	}

	m_listVertexBuffers.push_back(new VertexBuffer(*this, m_uVertexBufferSize));

	m_pIndexBuffer = new IndexBuffer(*this, m_uIndexBufferSize);
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
	//m_ViewportList.emplace_back(Viewport(m_pDevice, 0, 0, m_pWindow->GetWidth(), m_pWindow->GetHeight()));
	m_ViewportList.emplace_back(Viewport(m_pDevice, 0, 0, 1440, 900));
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

		m_pDevice->Present(nullptr, nullptr, nullptr, nullptr);
		m_pSwapChain->Present(nullptr, nullptr, nullptr, nullptr, 0);
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

Shader* RenderTarget::GetShader(ShaderType* pShaderType /* = nullptr */)
{
	// 如果着色器类型为空，则默认返回哈希表中的第一个着色器
	if (pShaderType == nullptr)
	{
		if (m_hashShaders.empty())
		{
			// 如果hash表为空，创建一个类型为空的着色器
			return GetShader(ShaderManager::GetInstance().GetShaderType(0));
		}

		return m_hashShaders.begin()->second;
	}

	hash_map<ShaderType*, Shader*>::iterator it = m_hashShaders.find(pShaderType);

	// 如果找到了就返回着色器指针
	if (it != m_hashShaders.end())
	{
		return it->second;
	}

	// 如果没有找到就从文件里加载
	Shader* pShader = new Shader(pShaderType);
	pShader->Load(this, m_pEffectPool);

	// 将加载好的着色器加入哈希表
	m_hashShaders.insert(make_pair(pShaderType, pShader));

	return pShader;
}

VertexDeclaration* RenderTarget::GetVertexDeclarationInstance(VertexDeclarationType* pVertexDeclaration)
{
	hash_map<VertexDeclarationType*, VertexDeclaration*>::iterator it = m_hashVertexDeclarationInstances.find(pVertexDeclaration);

	// 如果找到了就返回着色器指针
	if (it != m_hashVertexDeclarationInstances.end())
	{
		return it->second;
	}

	// 如果没有找到就生成一个新的实例
	VertexDeclaration* pDeclaration = new VertexDeclaration(pVertexDeclaration);
	pDeclaration->Load(this);

	// 将加载好的着色器加入哈希表
	m_hashVertexDeclarationInstances.insert(make_pair(pVertexDeclaration, pDeclaration));

	return pDeclaration;
}

Texture* RenderTarget::GetTexture(TextureInfo* pInfo)
{
	hash_map<TextureInfo*, Texture*>::iterator it = m_hashTextures.find(pInfo);

	if (it != m_hashTextures.end())
	{
		return it->second;
	}

	Texture* pTexture = new Texture(pInfo);
	pTexture->Load(*this);

	m_hashTextures.insert(make_pair(pInfo, pTexture));

	return pTexture;
}

void RenderTarget::ApplyRenderState(const RenderState& renderState)
{
	Material* pCurrentMaterial = m_CurrentRenderState.pMaterial;
	Material* pNewMaterial = renderState.pMaterial;

	Shader* pNewShader = pNewMaterial->GetShaderType()->GetShader(this);

	// 如果当前材质为空，则直接绑定新材质
	if (pCurrentMaterial == nullptr)
	{
		pNewShader->Begin();
		pNewShader->SetMaterial(pNewMaterial, this);
	}
	// 如果当前材质与新材质不同
	else if (pCurrentMaterial != pNewMaterial)
	{
		Shader* pCurrentShader = pCurrentMaterial->GetShaderType()->GetShader(this);

		// 如果当前着色器为空，直接绑定新着色器（这里其实不可能成立，因为有材质一定有shader）
		if (pCurrentShader == nullptr)
		{
			pNewShader->Begin();
			pNewShader->SetMaterial(pNewMaterial, this);
		}
		// 如果当前着色器与新着色器不同，就解绑当前着色器，绑定新着色器
		else if (pCurrentShader != pNewShader)
		{
			pCurrentShader->End();

			pNewShader->Begin();
			pNewShader->SetMaterial(pNewMaterial, this);
		}
		// 如果当前着色器与新着色器相同，则只绑定纹理和常量
		else
		{
			pNewShader->SetMaterial(pNewMaterial, this);
		}
	}
	// 如果当前材质与新材质相同，则不做任何操作

	// 设置变换矩阵
	pNewShader->SetTransform(&renderState.transformWorld, &renderState.transformWVP);
	pNewShader->CommitChanges();

	// 设置新渲染状态为当前渲染状态
	m_CurrentRenderState = renderState;
}

void RenderTarget::ResetRenderState()
{
	Material* pMaterial = m_CurrentRenderState.pMaterial;

	if (pMaterial != nullptr)
	{
		Shader* pShader = pMaterial->GetShaderType()->GetShader(this);

		if (pShader != nullptr)
		{
			pShader->End();
		}
	}

	m_CurrentRenderState = RenderState();
}

void RenderTarget::ApplyVertexStreamAndIndexStream(VertexDeclaration* pVertexDeclarationInstance)
{
	if (m_pCurrentVertexDeclaration != pVertexDeclarationInstance)
	{
		// ======================== 设置D3D顶点声明 ========================
		HRESULT hResult = m_pDevice->SetVertexDeclaration(pVertexDeclarationInstance->m_pD3DVertexDeclaration);
		if (FAILED(hResult))
		{
			ErrorBox("Set vertex declaration failed : %X", hResult);
		}

		// ======================== 绑定Stream Buffer ========================
		VertexDeclarationType* pVertexDeclaration = pVertexDeclarationInstance->m_pVertexDeclarationType;
		
		unsigned int uNewStreamCount = pVertexDeclaration->GetStreamCount();
		unsigned int uMissingBufferCount = uNewStreamCount - m_listVertexBuffers.size();
		for (unsigned int i = 0; i < uMissingBufferCount; ++i)
		{
			m_listVertexBuffers.push_back(new VertexBuffer(*this, m_uVertexBufferSize));
		}

		unsigned int uStreamIndex = 0;
		list<VertexBuffer*>::iterator itVertexBuffer = m_listVertexBuffers.begin();
		for (; uStreamIndex < uNewStreamCount; ++uStreamIndex, ++itVertexBuffer)
		{
			m_pDevice->SetStreamSource(
				uStreamIndex,													// Stream ID
				(*itVertexBuffer)->GetD3DVertexStreamBuffer(),					// 绑定的StreamBuffer
				0,																// StreamBuffer的Offset
				pVertexDeclaration->GetStreamVertexSize(uStreamIndex));			// StreamBuffer的Stride
		}

		// ======================== 解绑未使用的Stream Buffer ========================
		if (m_pCurrentVertexDeclaration != nullptr)
		{
			unsigned int uCurrentStreamCount = m_pCurrentVertexDeclaration->m_pVertexDeclarationType->GetStreamCount();
			for (; uStreamIndex < uCurrentStreamCount; ++uStreamIndex)
			{
				m_pDevice->SetStreamSource(
					uStreamIndex,			// Stream ID
					nullptr,				// 绑定的StreamBuffer
					0,						// StreamBuffer的Offset
					0);						// StreamBuffer的Stride
			}
		}

		m_pCurrentVertexDeclaration = pVertexDeclarationInstance;
	}

	if (m_pCurrentIndexBuffer != m_pIndexBuffer)
	{
		HRESULT hResult = m_pDevice->SetIndices(m_pIndexBuffer->GetD3DIndexBuffer());

		if (FAILED(hResult))
		{
			ASSERT(true);
		}

		m_pCurrentIndexBuffer = m_pIndexBuffer;
	}
}

void RenderTarget::DrawPrimitive(const RenderPrimitive& primitive)
{
	// 绑定顶点流和索引流
	ApplyVertexStreamAndIndexStream(primitive.GetVertexDeclarationType()->GetVertexDeclarationInstance(this));

	// 更新顶点流数据
	unsigned int uStreamCount = primitive.GetVertexDeclarationType()->GetStreamCount();
	list<VertexStream*>::const_iterator itVertexStream = primitive.GetVertexStreamList().begin();
	list<VertexBuffer*>::iterator itVertexBuffer = m_listVertexBuffers.begin();
	for (unsigned int uStreamIndex = 0; uStreamIndex < uStreamCount; ++uStreamIndex, ++itVertexStream, ++itVertexBuffer)
	{
		VertexStream* pVertexStream = *itVertexStream;
		(*itVertexBuffer)->BindStreamData(pVertexStream->pVertexData, pVertexStream->uStreamSize);
	}

	// 更新索引流数据
	IndexStream* pIndexStream = primitive.GetIndexStream();
	m_pIndexBuffer->BindStreamData(pIndexStream->pIndexData, pIndexStream->uStreamSize);

	// 执行DP
	HRESULT hResult = m_pDevice->DrawIndexedPrimitive(
		primitive.GetPrimitiveType(),		// 图元类型
		0,									// 从第几个顶点开始匹配0号索引
		0,									// 最小顶点索引（？）
		primitive.GetVertexCount(),			// 顶点流中的顶点个数
		0,									// 从第几个索引开始绘制
		primitive.GetPrimitveCount());		// 图元个数

	if (FAILED(hResult))
	{
		ErrorBox("Draw Primitive Failed : %X", hResult);
	}
}

bool RenderTarget::operator==(const RenderTarget& renderTarget) const
{
	return this == &renderTarget;
}
