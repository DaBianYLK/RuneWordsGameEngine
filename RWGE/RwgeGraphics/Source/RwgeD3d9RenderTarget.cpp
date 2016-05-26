#include "RwgeD3d9RenderTarget.h"

#include <d3dx9.h>
#include "RwgeGraphics.h"
#include "RwgeD3d9SwapChain.h"
#include "RwgeD3d9Device.h"
#include <RwgeLog.h>

RD3d9RenderTarget::RD3d9RenderTarget(int s32Width, int s32Height) : 
	m_pD3dSurface(nullptr),
	m_DefaultViewport(0, 0, s32Width, s32Height)
{
	
}

RD3d9RenderTarget::~RD3d9RenderTarget()
{
	RwgeSafeRelease(m_pD3dSurface);
}

void RD3d9RenderTarget::Resize(int s32Width, int s32Height, RwgeAppWindow::EDisplayMode mode)
{
	m_DefaultViewport.Resize(s32Width, s32Height);
}

void RD3d9RenderTarget::AddViewport(RD3d9Viewport* pViewport)
{
	m_listViewports.push_back(pViewport);
}

void RD3d9RenderTarget::RemoveViewport(RD3d9Viewport* pViewport)
{
	m_listViewports.remove(pViewport);
}
