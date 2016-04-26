#include "RwgeD3d9RenderSystem.h"

#include "RwgeGraphics.h"
#include "RwgeSceneManager.h"
#include "RwgeLight.h"
#include "RwgeRenderPrimitive.h"
#include "RwgeCamera.h"

RD3d9RenderSystem::RD3d9RenderSystem()
{
	m_pD3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pD3D9)
	{
		RwgeErrorBox("Initialize Direct3D-9 failed.");
	}
}

RD3d9RenderSystem::~RD3d9RenderSystem()
{
	D3d9SafeRelease(m_pD3D9);
}

bool RD3d9RenderSystem::CreateDevice(const RAppWindow& window)
{

}

bool RD3d9RenderSystem::DestroyDevice()
{

}

void RD3d9RenderSystem::RenderOneFrame(float fDeltaTime)
{
	// 注意，此处auto需要加上引用，否则会创建副本
	for (auto pRenderTarget : m_listRenderTarget)
	{
		m_pActiveRenderTarget = pRenderTarget;

		pRenderTarget->Update();
	}
}

void RD3d9RenderSystem::RenderScene(SceneManager* pSceneManager)
{
	// 设置着色器共享常量
	RShader* pShader = m_pActiveRenderTarget->GetShader();

	//static bool bLighted = false;
	//if (!bLighted)
	{
		void* pLightData;
		unsigned char uSize;
		pSceneManager->GetLight()->GetConstantBuffer(pLightData, uSize);
		pShader->SetLight(pLightData, uSize);

	//	bLighted = true;
	}

	// 设置球谐函数系数（暂时先全部设为1）
	SHCoefficients coefficients;
	coefficients.R = D3DXVECTOR4(1, 1, 1, 1);
	coefficients.G = D3DXVECTOR4(1, 1, 1, 1);
	coefficients.B = D3DXVECTOR4(1, 1, 1, 1);
	pShader->SetSHCoefficients(&coefficients);

	D3DXVECTOR3 viewOppoisteDirection = -(pSceneManager->GetActiveCamera()->GetDirection());
	pShader->SetViewOppositeDirection(&viewOppoisteDirection);

	// 遍历渲染队列，并执行绘制函数
	DrawRenderQueue();
}

void RD3d9RenderSystem::DrawRenderQueue()
{
	m_pActiveRenderTarget->GetD3dDevice()->BeginScene();

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

	m_pActiveRenderTarget->GetD3dDevice()->EndScene();

	//ResetRenderState();
}

void RD3d9RenderSystem::ApplyRenderState(const RenderState& renderState)
{
	m_pActiveRenderTarget->ApplyRenderState(renderState);
}

void RD3d9RenderSystem::ResetRenderState()
{
	m_pActiveRenderTarget->ResetRenderState();
}

void RD3d9RenderSystem::DrawPrimitive(const RenderPrimitive& primitive)
{
	m_pActiveRenderTarget->DrawPrimitive(primitive);
}

IDirect3D9* RD3d9RenderSystem::GetD3d9() const
{
	return m_pD3D9;
}

RenderTarget* RD3d9RenderSystem::CreateRenderTarget(const RAppWindow* pWindow)
{
	m_pActiveRenderTarget = new RenderTarget(pWindow);

	m_listRenderTarget.push_back(m_pActiveRenderTarget);

	return m_pActiveRenderTarget;
}

bool RD3d9RenderSystem::RemoveRenderTarget(RenderTarget* pTarget)
{
	RwgeAssert(pTarget);

	if (pTarget == m_pActiveRenderTarget)
	{
		m_pActiveRenderTarget = nullptr;
	}

	m_listRenderTarget.remove(pTarget);

	return true;
}

RenderTarget* RD3d9RenderSystem::GetActiveRenderTarget() const
{
	return m_pActiveRenderTarget;
}

RenderQueue* RD3d9RenderSystem::GetRenderQueuePtr()
{
	return &m_RenderQueue;
}















#include "RwgeD3d9RenderTarget.h"

#include "RwgeAppWindow.h"
#include "RwgeShaderManager.h"
#include "RwgeVertexDeclarationType.h"
#include "RwgeRenderPrimitive.h"
#include "RwgeVertexStream.h"
#include "RwgeIndexStream.h"
#include <RwgeAssert.h>

using namespace std;

RenderTarget::RenderTarget(const RAppWindow* pWindow) :
RD3d9Device(*pWindow),
m_pWindow(pWindow),
m_pActiveViewport(nullptr),
m_pIndexBuffer(nullptr),
m_pEffectPool(nullptr),
m_pCurrentVertexDeclaration(nullptr),
m_pCurrentIndexBuffer(nullptr)
{
	HRESULT hResult = D3DXCreateEffectPool(&m_pEffectPool);
	if (FAILED(hResult))
	{
		RwgeErrorBox("Create effect pool failed.")
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
	m_ViewportList.emplace_back(Viewport(m_pD3dDevice, 0, 0, 1440, 900));
	m_pActiveViewport = &m_ViewportList.back();
	m_pActiveViewport->m_pRenderTarget = this;

	return m_pActiveViewport;
}

bool RenderTarget::RemoveViewport(Viewport* pViewport)
{
	RwgeAssert(pViewport);

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

		m_pD3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
		m_pD3dSwapChain->Present(nullptr, nullptr, nullptr, nullptr, 0);
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

RShader* RenderTarget::GetShader(ShaderType* pShaderType /* = nullptr */)
{
	// 如果着色器类型为空，则默认返回哈希表中的第一个着色器
	if (pShaderType == nullptr)
	{
		if (m_hashShaders.empty())
		{
			// 如果hash表为空，创建一个类型为空的着色器
			return GetShader(RShaderManager::GetInstance().GetShaderType(0));
		}

		return m_hashShaders.begin()->second;
	}

	hash_map<ShaderType*, RShader*>::iterator it = m_hashShaders.find(pShaderType);

	// 如果找到了就返回着色器指针
	if (it != m_hashShaders.end())
	{
		return it->second;
	}

	// 如果没有找到就从文件里加载
	RShader* pShader = new RShader(pShaderType);
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
	RMaterial* pCurrentMaterial = m_CurrentRenderState.pMaterial;
	RMaterial* pNewMaterial = renderState.pMaterial;

	RShader* pNewShader = pNewMaterial->GetShaderType()->GetShader(this);

	// 如果当前材质为空，则直接绑定新材质
	if (pCurrentMaterial == nullptr)
	{
		pNewShader->Begin();
		pNewShader->SetMaterial(pNewMaterial, this);
	}
	// 如果当前材质与新材质不同
	else if (pCurrentMaterial != pNewMaterial)
	{
		RShader* pCurrentShader = pCurrentMaterial->GetShaderType()->GetShader(this);

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
	RMaterial* pMaterial = m_CurrentRenderState.pMaterial;

	if (pMaterial != nullptr)
	{
		RShader* pShader = pMaterial->GetShaderType()->GetShader(this);

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
		HRESULT hResult = m_pD3dDevice->SetVertexDeclaration(pVertexDeclarationInstance->m_pD3DVertexDeclaration);
		if (FAILED(hResult))
		{
			RwgeErrorBox("Set vertex declaration failed : %X", hResult);
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
			m_pD3dDevice->SetStreamSource(
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
				m_pD3dDevice->SetStreamSource(
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
		HRESULT hResult = m_pD3dDevice->SetIndices(m_pIndexBuffer->GetD3DIndexBuffer());

		if (FAILED(hResult))
		{
			RwgeAssert(true);
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
	HRESULT hResult = m_pD3dDevice->DrawIndexedPrimitive(
		primitive.GetPrimitiveType(),		// 图元类型
		0,									// 从第几个顶点开始匹配0号索引
		0,									// 最小顶点索引（？）
		primitive.GetVertexCount(),			// 顶点流中的顶点个数
		0,									// 从第几个索引开始绘制
		primitive.GetPrimitveCount());		// 图元个数

	if (FAILED(hResult))
	{
		RwgeErrorBox("Draw Primitive Failed : %X", hResult);
	}
}

bool RenderTarget::operator==(const RenderTarget& renderTarget) const
{
	return this == &renderTarget;
}
