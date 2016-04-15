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
	// clear����ֻ�����ñ�־����������ջ���
	// ����һ���յ���ʱ���������swap���������ڴ棬��������ʱ��������������ͷ��ڴ�(�����Ҫ�����ͷſ���ͨ���������޶���ʱ������������)
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
	// ע�⣬�˴�auto��Ҫ�������ã�����ᴴ������
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
	// �����ɫ������Ϊ�գ���Ĭ�Ϸ��ع�ϣ���еĵ�һ����ɫ��
	if (pShaderType == nullptr)
	{
		if (m_hashShaders.empty())
		{
			// ���hash��Ϊ�գ�����һ������Ϊ�յ���ɫ��
			return GetShader(ShaderManager::GetInstance().GetShaderType(0));
		}

		return m_hashShaders.begin()->second;
	}

	hash_map<ShaderType*, Shader*>::iterator it = m_hashShaders.find(pShaderType);

	// ����ҵ��˾ͷ�����ɫ��ָ��
	if (it != m_hashShaders.end())
	{
		return it->second;
	}

	// ���û���ҵ��ʹ��ļ������
	Shader* pShader = new Shader(pShaderType);
	pShader->Load(this, m_pEffectPool);

	// �����غõ���ɫ�������ϣ��
	m_hashShaders.insert(make_pair(pShaderType, pShader));

	return pShader;
}

VertexDeclaration* RenderTarget::GetVertexDeclarationInstance(VertexDeclarationType* pVertexDeclaration)
{
	hash_map<VertexDeclarationType*, VertexDeclaration*>::iterator it = m_hashVertexDeclarationInstances.find(pVertexDeclaration);

	// ����ҵ��˾ͷ�����ɫ��ָ��
	if (it != m_hashVertexDeclarationInstances.end())
	{
		return it->second;
	}

	// ���û���ҵ�������һ���µ�ʵ��
	VertexDeclaration* pDeclaration = new VertexDeclaration(pVertexDeclaration);
	pDeclaration->Load(this);

	// �����غõ���ɫ�������ϣ��
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

	// �����ǰ����Ϊ�գ���ֱ�Ӱ��²���
	if (pCurrentMaterial == nullptr)
	{
		pNewShader->Begin();
		pNewShader->SetMaterial(pNewMaterial, this);
	}
	// �����ǰ�������²��ʲ�ͬ
	else if (pCurrentMaterial != pNewMaterial)
	{
		Shader* pCurrentShader = pCurrentMaterial->GetShaderType()->GetShader(this);

		// �����ǰ��ɫ��Ϊ�գ�ֱ�Ӱ�����ɫ����������ʵ�����ܳ�������Ϊ�в���һ����shader��
		if (pCurrentShader == nullptr)
		{
			pNewShader->Begin();
			pNewShader->SetMaterial(pNewMaterial, this);
		}
		// �����ǰ��ɫ��������ɫ����ͬ���ͽ��ǰ��ɫ����������ɫ��
		else if (pCurrentShader != pNewShader)
		{
			pCurrentShader->End();

			pNewShader->Begin();
			pNewShader->SetMaterial(pNewMaterial, this);
		}
		// �����ǰ��ɫ��������ɫ����ͬ����ֻ������ͳ���
		else
		{
			pNewShader->SetMaterial(pNewMaterial, this);
		}
	}
	// �����ǰ�������²�����ͬ�������κβ���

	// ���ñ任����
	pNewShader->SetTransform(&renderState.transformWorld, &renderState.transformWVP);
	pNewShader->CommitChanges();

	// ��������Ⱦ״̬Ϊ��ǰ��Ⱦ״̬
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
		// ======================== ����D3D�������� ========================
		HRESULT hResult = m_pDevice->SetVertexDeclaration(pVertexDeclarationInstance->m_pD3DVertexDeclaration);
		if (FAILED(hResult))
		{
			ErrorBox("Set vertex declaration failed : %X", hResult);
		}

		// ======================== ��Stream Buffer ========================
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
				(*itVertexBuffer)->GetD3DVertexStreamBuffer(),					// �󶨵�StreamBuffer
				0,																// StreamBuffer��Offset
				pVertexDeclaration->GetStreamVertexSize(uStreamIndex));			// StreamBuffer��Stride
		}

		// ======================== ���δʹ�õ�Stream Buffer ========================
		if (m_pCurrentVertexDeclaration != nullptr)
		{
			unsigned int uCurrentStreamCount = m_pCurrentVertexDeclaration->m_pVertexDeclarationType->GetStreamCount();
			for (; uStreamIndex < uCurrentStreamCount; ++uStreamIndex)
			{
				m_pDevice->SetStreamSource(
					uStreamIndex,			// Stream ID
					nullptr,				// �󶨵�StreamBuffer
					0,						// StreamBuffer��Offset
					0);						// StreamBuffer��Stride
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
	// �󶨶�������������
	ApplyVertexStreamAndIndexStream(primitive.GetVertexDeclarationType()->GetVertexDeclarationInstance(this));

	// ���¶���������
	unsigned int uStreamCount = primitive.GetVertexDeclarationType()->GetStreamCount();
	list<VertexStream*>::const_iterator itVertexStream = primitive.GetVertexStreamList().begin();
	list<VertexBuffer*>::iterator itVertexBuffer = m_listVertexBuffers.begin();
	for (unsigned int uStreamIndex = 0; uStreamIndex < uStreamCount; ++uStreamIndex, ++itVertexStream, ++itVertexBuffer)
	{
		VertexStream* pVertexStream = *itVertexStream;
		(*itVertexBuffer)->BindStreamData(pVertexStream->pVertexData, pVertexStream->uStreamSize);
	}

	// ��������������
	IndexStream* pIndexStream = primitive.GetIndexStream();
	m_pIndexBuffer->BindStreamData(pIndexStream->pIndexData, pIndexStream->uStreamSize);

	// ִ��DP
	HRESULT hResult = m_pDevice->DrawIndexedPrimitive(
		primitive.GetPrimitiveType(),		// ͼԪ����
		0,									// �ӵڼ������㿪ʼƥ��0������
		0,									// ��С��������������
		primitive.GetVertexCount(),			// �������еĶ������
		0,									// �ӵڼ���������ʼ����
		primitive.GetPrimitveCount());		// ͼԪ����

	if (FAILED(hResult))
	{
		ErrorBox("Draw Primitive Failed : %X", hResult);
	}
}

bool RenderTarget::operator==(const RenderTarget& renderTarget) const
{
	return this == &renderTarget;
}
