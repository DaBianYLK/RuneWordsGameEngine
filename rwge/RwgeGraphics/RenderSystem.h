#pragma once

#include "RenderTarget.h"
#include "Singleton.h"

class IDirect3D9;

class RenderSystem : public Singleton<RenderSystem>
{
public:
	RenderSystem();
	virtual ~RenderSystem();

private:
	bool Init();
	bool Release();

public:
	IDirect3D9* GetD3D9Ptr() const;

	RenderTarget* CreateRenderTarget(DisplayWindow& window);
	bool RemoveRenderTarget(RenderTarget* pTarget);

	RenderTarget* GetActiveRenderTarget();



private:
	IDirect3D9* m_pD3D9;

	RenderTarget* m_pActiveRenderTarget;		// ��ǰ���������ȾĿ��
	RenderTargetList m_RenderTargetList;		// ��ȾĿ���б�
};

#define g_RenderSystem RenderSystem::GetInstance()