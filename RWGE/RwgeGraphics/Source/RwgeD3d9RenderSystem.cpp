#include "RwgeD3d9RenderSystem.h"

#include "RwgeGraphics.h"
#include <RwgeAppWindow.h>
#include "RwgeD3d9Device.h"
#include "RwgeD3d9SwapChain.h"
#include "RwgeD3d9RenderTarget.h"
#include "RwgeD3d9ShaderManager.h"
#include "RwgeMaterial.h"
#include "RwgeD3d9VertexDeclaration.h"
#include "RwgeVertexStream.h"
#include "RwgeIndexStream.h"
#include "RwgeVertexDeclarationManager.h"
#include "RwgeTextureManager.h"
#include <RwgeLog.h>
#include "RwgeD3dx9Extension.h"

using namespace std;
using namespace RwgeD3dx9Extension;

RD3d9RenderSystem::RD3d9RenderSystem() : 
	m_pD3d9(nullptr),
	m_pDevice(nullptr),
	m_pActivedRenderTarget(nullptr),
	m_pFormerRenderTarget(nullptr)
{
	m_pD3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!m_pD3d9)
	{
		RwgeErrorBox(TEXT("Initialize Direct3D-9 failed."));
	}

	GlobalKey globalShaderKey;
	globalShaderKey.SetShaderSkinKey(false);
	m_RenderQueue.SetGlobalKey(globalShaderKey);
}

RD3d9RenderSystem::~RD3d9RenderSystem()
{
	RwgeSafeRelease(m_pD3d9);
}

RD3d9RenderTarget* RD3d9RenderSystem::RegWinodwForRenderTarget(RAppWindow& window)
{
#ifdef _DEBUG
	if (m_mapWindowsToRenderTargets.find(&window) != m_mapWindowsToRenderTargets.end())
	{
		RwgeErrorBox(TEXT("Window \"%s\" has already owned a render target."), window.GetName());
		return nullptr;
	}
#endif

	// 如果Device为空，则创建Device，否则创建SwapChain
	if (m_pDevice == nullptr)
	{
		m_pDevice = new RD3d9Device(window);
		m_mapWindowsToRenderTargets.insert(make_pair(&window, m_pDevice));

		// Device创建完成后初始化各个渲染模块
		m_pVertexDeclarationManager = new RVertexDeclarationManager();
		m_pShaderManager			= new RD3d9ShaderManager();
		m_pTextureManager			= new RTextureManager();

		return m_pDevice;
	}

	RD3d9SwapChain* pSwapChain = new RD3d9SwapChain(window);
	m_mapWindowsToRenderTargets.insert(make_pair(&window, pSwapChain));
	
	return pSwapChain;
}

bool RD3d9RenderSystem::DeRegWindowForRenderTarget(RAppWindow& window)
{
	auto itRenderTarget = m_mapWindowsToRenderTargets.find(&window);

	if (itRenderTarget == m_mapWindowsToRenderTargets.end())
	{
		return false;
	}
	
	if (itRenderTarget->second == m_pDevice)
	{
		m_pDevice = nullptr;
	}

	delete itRenderTarget->second;
	m_mapWindowsToRenderTargets.erase(itRenderTarget);

	return true;
}

void RD3d9RenderSystem::BeginScene()
{
	HRESULT hResult = g_pD3d9Device->BeginScene();
	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Begin scene failed - ErrorCode: %s"), D3dErrorCodeToString(hResult));
	}
}

void RD3d9RenderSystem::EndScene()
{
	HRESULT hResult = g_pD3d9Device->EndScene();
	if (FAILED(hResult))
	{
		RwgeLog(TEXT("End scene failed - ErrorCode: %s"), D3dErrorCodeToString(hResult));
	}
}

void RD3d9RenderSystem::SubmitRenderTarget(RD3d9RenderTarget* pRenderTarget)
{
	RwgeAssert(pRenderTarget);
	RwgeAssert(pRenderTarget != m_pActivedRenderTarget);	// RenderTarget被重复设置说明代码逻辑有问题

	HRESULT hResult = g_pD3d9Device->SetRenderTarget(0, pRenderTarget->GetD3dSurface());
	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Set render target failed - ErrorCode: %s"), D3dErrorCodeToString(hResult));
	}
	pRenderTarget->ReleaseD3dSurface();		// 释放Surface的引用计数

	m_pFormerRenderTarget = m_pActivedRenderTarget;
	m_pActivedRenderTarget = pRenderTarget;

	// 设置RenderTarget时，D3D会默认将Viewport设为和RenderTarget一样大
	m_pFormerViewport = nullptr;			// 旧RenderTarget中的Viewport已经失效
	m_pAcitvedViewport = pRenderTarget->GetDefaultViewport();
}

void RD3d9RenderSystem::SubmitFormerRenderTarget()
{
	if (m_pFormerRenderTarget == nullptr)
	{
		SubmitDefaultRenderTarget();
	}
	else
	{
		SubmitRenderTarget(m_pFormerRenderTarget);
	}
}

void RD3d9RenderSystem::SubmitDefaultRenderTarget()
{
	SubmitRenderTarget(m_pDevice);
}

void RD3d9RenderSystem::ClearActivedRenderTarget()
{
	RwgeAssert(m_pActivedRenderTarget);

	HRESULT hResult = g_pD3d9Device->Clear(
		0, 
		nullptr, 
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 
		m_pActivedRenderTarget->GetBackgroundColor(), 
		m_pActivedRenderTarget->GetMaxZ(), 
		0);

	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Clear render target failed - ErrorCode: %s"), D3dErrorCodeToString(hResult));
	}
}

void RD3d9RenderSystem::SubmitViewport(const RD3d9Viewport* pViewport)
{
	RwgeAssert(pViewport);
	RwgeAssert(pViewport != m_pAcitvedViewport);	// Viewport被重复设置说明代码逻辑有问题

	HRESULT hResult = g_pD3d9Device->SetViewport(pViewport->GetD3dViewport());
	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Set viewport failed - ErrorCode: %s"), D3dErrorCodeToString(hResult));
	}

	m_pFormerViewport = m_pAcitvedViewport;
	m_pAcitvedViewport = pViewport;
}

void RD3d9RenderSystem::SubmitFormerViewport()
{
	if (m_pFormerViewport == nullptr)
	{
		SubmitDefaultViewport();
	}
	else
	{
		SubmitViewport(m_pFormerViewport);
	}
}

void RD3d9RenderSystem::SubmitDefaultViewport()
{
	SubmitViewport(m_pActivedRenderTarget->GetDefaultViewport());
}

void RD3d9RenderSystem::ClearViewport(const RD3d9Viewport* pViewport)
{
	RwgeAssert(pViewport);

	HRESULT hResult = g_pD3d9Device->Clear(
		1,
		&pViewport->GetD3dRect(),
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		pViewport->GetBackgroundColor(),
		pViewport->GetMaxZ(),
		0);

	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Clear viewport failed - ErrorCode: %s"), D3dErrorCodeToString(hResult));
	}
}

void RD3d9RenderSystem::SubmitRenderState(const RenderState& newRenderState)
{
	RwgeAssert(newRenderState.pShader);
	RwgeAssert(newRenderState.pMaterial);

	RMaterial* pNewMaterial = newRenderState.pMaterial;
	if (m_ActivedRenderState.pMaterial != pNewMaterial)
	{
		SubmitShader(newRenderState.pShader);

		m_ActivedRenderState.pShader->SetMaterial(pNewMaterial);
		m_ActivedRenderState.pMaterial = pNewMaterial;
	}
}

void RD3d9RenderSystem::SubmitMaterial(RMaterial* pNewMaterial)
{
	RwgeAssert(pNewMaterial);

	if (m_ActivedRenderState.pMaterial != pNewMaterial)
	{
		// 注意：材质中缓存的Shader可能与正在被渲染的场景参数不兼容，需要开发者自行控制与确认
		SubmitShader(pNewMaterial->GetCachedShader());

		m_ActivedRenderState.pShader->SetMaterial(pNewMaterial);
		m_ActivedRenderState.pMaterial = pNewMaterial;
	}
}

void RD3d9RenderSystem::SubmitShader(RD3d9Shader* pNewShader)
{
	if (m_ActivedRenderState.pShader != pNewShader)
	{
		if (m_ActivedRenderState.pShader != nullptr)
		{
			m_ActivedRenderState.pShader->End();
		}
		
		if (pNewShader != nullptr)
		{
			pNewShader->Begin();
		}
		
		m_ActivedRenderState.pShader = pNewShader;
	}
}

void RD3d9RenderSystem::SubmitVertexDeclaration(const RD3d9VertexDeclaration* pVertexDeclaration)
{
	RwgeAssert(pVertexDeclaration);

	if (m_pActivedVertexDeclaration != pVertexDeclaration)
	{
		HRESULT hResult = g_pD3d9Device->SetVertexDeclaration(pVertexDeclaration->GetD3dVertexDeclaration());
		if (FAILED(hResult))
		{
			RwgeLog(TEXT("Set vertex declaration failed - ErrorCode: %s"), D3dErrorCodeToString(hResult));
		}

		m_pActivedVertexDeclaration = pVertexDeclaration;
	}
}

void RD3d9RenderSystem::SubmitVertexStream(const vector<VertexStream*>& vecVertexStreams)
{
	RwgeAssert(m_pActivedVertexDeclaration->GetStreamCount() == vecVertexStreams.size());

	// 解绑未使用的StreamSource
	for (unsigned int i = vecVertexStreams.size(); i < m_u8ActivedStreamCount; ++i)
	{
		HRESULT hResult = g_pD3d9Device->SetStreamSource(i, nullptr, 0, 0);
		if (FAILED(hResult))
		{
			RwgeLog(TEXT("Set stream source failed - ErrorCode: %s"), D3dErrorCodeToString(hResult));
		}
		m_vecActivedVertexStreams[i] = nullptr;
	}

	if (vecVertexStreams.size() > m_vecActivedVertexStreams.size())
	{
		m_vecActivedVertexStreams.resize(vecVertexStreams.size());
	}

	for (unsigned int i = 0; i < vecVertexStreams.size(); ++i)
	{
		if (m_vecActivedVertexStreams[i] != vecVertexStreams[i])
		{
			if (vecVertexStreams[i]->pD3dVertexBuffer == nullptr)
			{
				RwgeLog(TEXT("Vertex stream can't be submited before binding to a vertex buffer."));
				continue;
			}

			HRESULT hResult = g_pD3d9Device->SetStreamSource(
				i,											// Stream ID
				vecVertexStreams[i]->pD3dVertexBuffer,		// 绑定的StreamBuffer
				vecVertexStreams[i]->u32StreamOffset,		// StreamBuffer的Offset
				vecVertexStreams[i]->u8VertexSize);			// StreamBuffer的Stride

			if (FAILED(hResult))
			{
				RwgeLog(TEXT("Set stream source failed - ErrorCode: %s"), D3dErrorCodeToString(hResult));
			}

			m_vecActivedVertexStreams[i] = vecVertexStreams[i];
		}
	}

	m_u8ActivedStreamCount = vecVertexStreams.size();
}

void RD3d9RenderSystem::SubmitIndexStream(const IndexStream* pIndexStream)
{
	RwgeAssert(pIndexStream);

	if (m_pActivedIndexStream != pIndexStream)
	{
		HRESULT hResult = g_pD3d9Device->SetIndices(pIndexStream->pD3dIndexBuffer);
		if (FAILED(hResult))
		{
			RwgeLog(TEXT("Set indices failed - ErrorCode: %s"), D3dErrorCodeToString(hResult));
		}
		m_pActivedIndexStream = pIndexStream;
	}
}

void RD3d9RenderSystem::SubmitRenderUnit(const RRenderUnit& renderUnit)
{
	m_ActivedRenderState.pShader->SetTransform(renderUnit.GetWorldTransform(), &m_RenderQueue.m_ViewProjTransform);
	m_ActivedRenderState.pShader->CommitChanges();

	SubmitVertexDeclaration(renderUnit.GetVertexDeclaration());
	SubmitVertexStream(renderUnit.GetVertexStreams());
	SubmitIndexStream(renderUnit.GetIndexStream());

	// 执行DP
	HRESULT hResult = g_pD3d9Device->DrawIndexedPrimitive(
		renderUnit.GetPrimitiveType(),		// 图元类型
		0,									// 从第几个顶点开始匹配0号索引
		0,									// 最小顶点索引（？）
		renderUnit.GetVertexCount(),		// 顶点流中的顶点个数
		0,									// 从第几个索引开始绘制
		renderUnit.GetPrimitveCount());		// 图元个数

	if (FAILED(hResult))
	{
		RwgeLog(TEXT("Draw Primitive Failed : %X"), hResult);
	}
}

void RD3d9RenderSystem::SubmitRenderQueue(const RD3d9RenderQueue& renderQueue)
{
	// ================================ 设置场景的渲染状态 ================================
	RD3d9Shader* pSharedShader = RD3d9ShaderManager::GetInstance().GetSharedShader();
	if (pSharedShader == nullptr)
	{
		return;		// 如果不存在着色器，无法执行渲染，直接返回
	}
	pSharedShader->SetOppositeView(&renderQueue.m_ViewOppositeDirection);
	if (renderQueue.m_pLight != nullptr)
	{
		pSharedShader->SetLight(renderQueue.m_pLight);
	}

	// ================================ 遍历不透明组对象 ================================
	const OpaqueGroupMap& mapOpaqueGroup = renderQueue.m_mapOpaqueGroup;
	OpaqueGroupMap::const_iterator itOpaqueRenderUnitSet = mapOpaqueGroup.begin();
	for (; itOpaqueRenderUnitSet != mapOpaqueGroup.end(); ++itOpaqueRenderUnitSet)
	{
		/* 不同图元集合对应的渲染状态一定不同，所以材质一定发生了改变，因此这里没有必要检查材质的变化 */

		const RenderState& newRenderState = itOpaqueRenderUnitSet->first;
		SubmitShader(newRenderState.pShader);									// 提交着色器
		m_ActivedRenderState.pShader->SetMaterial(newRenderState.pMaterial);	// 提交材质

		for (const RenderUnitWithDepth& renderUnitWithDepth : itOpaqueRenderUnitSet->second)
		{
			SubmitRenderUnit(*renderUnitWithDepth.pRenderUnit);
		}
	}

	// ================================ 遍历透明组对象 ================================
	const TranslucentGroupMap& mapTranslucentGroup = renderQueue.m_mapTranslucentGroup;
	TranslucentGroupMap::const_iterator itTranslucentRenderUnitList = mapTranslucentGroup.begin();
	for (; itTranslucentRenderUnitList != mapTranslucentGroup.end(); ++itTranslucentRenderUnitList)
	{
		const RenderState& newRenderState = itTranslucentRenderUnitList->first;
		SubmitShader(newRenderState.pShader);									// 提交着色器
		m_ActivedRenderState.pShader->SetMaterial(newRenderState.pMaterial);	// 提交材质

		for (const RRenderUnit* pRenderUnit : itTranslucentRenderUnitList->second)
		{
			SubmitRenderUnit(*pRenderUnit);
		}
	}
}

void RD3d9RenderSystem::RenderOneFrame(float fDeltaTime)
{
	// 注意，此处auto需要加上引用，否则会创建副本
	for (auto& pairRenderTarget : m_mapWindowsToRenderTargets)
	{
		if (m_pActivedRenderTarget != pairRenderTarget.second)
		{
			SubmitRenderTarget(pairRenderTarget.second);
		}

		BeginScene();
		ClearActivedRenderTarget();

		// 如果RenderTarget使用默认的Viewport，则不需要手动设置Viewport
		if (m_pActivedRenderTarget->IsUsingDefaultViewport())
		{
			m_pActivedRenderTarget->m_DefaultViewport.UpdateCamera(m_RenderQueue);	// 更新相机所处场景的场景树，并获取渲染队列
			SubmitRenderQueue(m_RenderQueue);
		}
		// 否则手动设置Viewport再执行渲染
		else
		{
			for (RD3d9Viewport* pViewport : m_pActivedRenderTarget->m_listViewports)
			{
				SubmitViewport(pViewport);
				pViewport->UpdateCamera(m_RenderQueue);								// 更新相机所处场景的场景树，并获取渲染队列
				SubmitRenderQueue(m_RenderQueue);
			}
		}

		EndScene();
	}
}

void RD3d9RenderSystem::PresentFrame()
{
	for (auto& pairRenderTarget : m_mapWindowsToRenderTargets)
	{
		pairRenderTarget.second->Present();
	}
}
