#pragma once

#include "RenderQueue.h"
#include "RenderTarget.h"
#include "VertexDeclarationManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Singleton.h"

struct IDirect3D9;
class RenderPrimitive;

/*
关于RenderSystem与RenderQueue：
1.	RenderQueue是RenderSystem的一部分，所有RenderTarget共用同一个RenderQueue
2.	绘制每个Viewport时（遍历场景前），RenderQueue都会被清空
3.	RenderQueue不设计成单例的原因是如果要多线程渲染，可能会需要多个RenderQueue：
	A.	实现多线程渲染时需要使用双缓冲，此时需要2个RenderQueue
	B.	若需要支持同时渲染多个视口，需要多个RenderQueue
*/

class RenderSystem : public Singleton<RenderSystem>
{
public:
	RenderSystem();
	virtual ~RenderSystem();

private:
	bool InitD3DD9();
	bool Release();

public:
	void RenderOneFrame(float fDeltaTime);
	void RenderScene(SceneManager* pSceneManager);
	void DrawRenderQueue();
	void ApplyRenderState(const RenderState& renderState);
	void ResetRenderState();
	void DrawPrimitive(const RenderPrimitive& primitive);

	IDirect3D9* GetD3D9Ptr() const;

	RenderTarget* CreateRenderTarget(const DisplayWindow* pWindow);
	bool RemoveRenderTarget(RenderTarget* pTarget);

	RenderTarget* GetActiveRenderTarget() const;

	RenderQueue* GetRenderQueuePtr();

private:
	IDirect3D9* m_pD3D9;

	RenderTarget* m_pActiveRenderTarget;			// 当前被激活的渲染目标
	std::list<RenderTarget*> m_listRenderTarget;	// 渲染目标列表

	RenderQueue m_RenderQueue;

	VertexDeclarationManager	m_VertexDeclarationManager;
	ShaderManager				m_ShaderManager;
	TextureManager				m_TextureManager;
};

#define g_RenderSystem RenderSystem::GetInstance()