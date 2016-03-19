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
����RenderSystem��RenderQueue��
1.	RenderQueue��RenderSystem��һ���֣�����RenderTarget����ͬһ��RenderQueue
2.	����ÿ��Viewportʱ����������ǰ����RenderQueue���ᱻ���
3.	RenderQueue����Ƴɵ�����ԭ�������Ҫ���߳���Ⱦ�����ܻ���Ҫ���RenderQueue��
	A.	ʵ�ֶ��߳���Ⱦʱ��Ҫʹ��˫���壬��ʱ��Ҫ2��RenderQueue
	B.	����Ҫ֧��ͬʱ��Ⱦ����ӿڣ���Ҫ���RenderQueue
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

	RenderTarget* m_pActiveRenderTarget;			// ��ǰ���������ȾĿ��
	std::list<RenderTarget*> m_listRenderTarget;	// ��ȾĿ���б�

	RenderQueue m_RenderQueue;

	VertexDeclarationManager	m_VertexDeclarationManager;
	ShaderManager				m_ShaderManager;
	TextureManager				m_TextureManager;
};

#define g_RenderSystem RenderSystem::GetInstance()