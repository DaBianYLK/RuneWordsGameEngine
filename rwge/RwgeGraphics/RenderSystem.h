#pragma once

#include "RenderQueue.h"
#include "RenderTarget.h"
#include "Singleton.h"

struct IDirect3D9;

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

	void RenderOneFrame(float fDeltaTime);
	void RenderScene(SceneManager* pSceneManager);
	void VisitRenderQueue();
	void ApplyRenderState(const RenderState& renderState);

private:
	bool InitD3DD9();
	bool Release();

public:
	IDirect3D9* GetD3D9Ptr() const;

	RenderTarget* CreateRenderTarget(DisplayWindow& window);
	bool RemoveRenderTarget(RenderTarget* pTarget);

	RenderTarget* GetActiveRenderTarget() const;

	RenderQueue* GetRenderQueuePtr();

private:
	IDirect3D9* m_pD3D9;

	RenderTarget* m_pActiveRenderTarget;		// ��ǰ���������ȾĿ��
	RenderTargetList m_RenderTargetList;		// ��ȾĿ���б�

	RenderQueue m_RenderQueue;

	RenderState m_CurrentRenderState;
};

#define g_RenderSystem RenderSystem::GetInstance()