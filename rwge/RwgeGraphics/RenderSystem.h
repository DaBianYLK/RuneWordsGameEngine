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
	IDirect3D9* m_pD3D9;

	RenderTarget* m_pActiveRenderTarget;		// 当前被激活的渲染目标
	RenderTargetList m_listRenderTargets;		// 渲染目标列表
};

